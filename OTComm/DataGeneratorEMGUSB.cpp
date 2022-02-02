#include <windows.h>
#include "DataConverter.h"
#include "DataGeneratorEMGUSB.h"
#include <guiddef.h>
#include <initguid.h>
#include <strsafe.h>
#include <setupapi.h>
#include <basetyps.h>
#include "DataValidator.h"
#include <string>


using namespace System::IO;
using namespace System::Reflection;
using namespace System::Xml;

#define IOCTL_INDEX						0x800
#define FILE_DEVICE_EMGUSB2				0x65500
#define SETTINGS_SIZE_BYTES_EMG_USB		26
#define SETTINGS_SIZE_BYTES_EMG_USB2	25
#define SETTINGS_SIZE_BYTES_EMG_USB2p	24
#define NUM_SETT_IO  1

#define IOCTL_EMGUSB2_START_ACQUISITION CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 6, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_STOP_ACQUISITION	CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 7, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_GET_DEVICE_ID		CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 4, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_EMGUSB2_PROGRAM_FPGA	    CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 5, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_PROGRAM_PIC       CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 11,METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_GET_SETTINGS	    CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 8, METHOD_BUFFERED, FILE_READ_ACCESS)

DEFINE_GUID(GUID_DEVINTERFACE_EMGUSB2, 0x9d57f99f, 0x33cb, 0x488d, 0xa5, 0x57, 0x2c, 0x5d, 0x0f, 0xd5, 0x1d, 0x85);

namespace OTComm
{
	namespace EMGUSB
	{
#pragma managed

#pragma region Enums
		private enum EMGUSBFamilies { EMGUSB1 = 1, EMGUSB2 = 2, EMGUSB2p = 3, MEBA = 81, WIRELESS = 82 };
#pragma endregion




#pragma region Constructiors and destructors
		DataGeneratorEMGUSB::DataGeneratorEMGUSB(Model model, Frequency SamplingFrequency) :
			DataGeneratorInterface(),
			mDeviceIsOpen(false),
			mDataReaderInterfaceHelper(NULL),
			mModel(model),
			mManagedAcquisitionThreadHandle(nullptr)
		{
			try
			{
				mConnectors = gcnew array<GenericConnector^>(GetNumberConnectorsMultipleIn() + GetNumberConnectorsIn() + GetNumberConnectorsAux());
				unsigned int k = 0;
				for (; k < GetNumberConnectorsIn(); k++)
					mConnectors[k] = gcnew GenericConnector(16);
				for (; k < GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn(); k++)
					mConnectors[k] = gcnew GenericConnector(64);
				for (; k < (unsigned int)mConnectors->Length; k++)
					mConnectors[k] = gcnew GenericConnector(1);

				mSources = gcnew array<DataGeneratorSingleSource^>(1);
				mSources[0] = gcnew DataGeneratorSingleSource(SamplingFrequency, 2, GetChannelsMapping());

				mDeviceTotalChannels = GetNumberTotalChannels();
				mDefaultDataValidator = gcnew DataValidatorTrigger(0, 128, OTComm::TriggerModes::StartRaiseStopFall);
			}
			catch (Exception ^e)
			{
				ConstructorFailedException^ cfe = gcnew ConstructorFailedException(__FUNCTION__, e);
				throw cfe;
			}
		}

		DataGeneratorEMGUSB::~DataGeneratorEMGUSB()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorEMGUSB();
		}

		DataGeneratorEMGUSB::!DataGeneratorEMGUSB()
		{
		}
#pragma endregion

#pragma region XML
		String^ DataGeneratorEMGUSB::GetDeviceXMLConfiguration()
		{
			LOG_ENTERING;
			bool IsUSB23 = DEVICE(mModel) == EMGUSBFamilies::EMGUSB2 || DEVICE(mModel) == EMGUSBFamilies::EMGUSB2p;
			// Decode settings
			const int gainvalues[] = { 0, 100, 200, 500, 1000, 2000, 5000, 10000 };
			const int highpassvalues[] = { 3, 10, 100, 200 };
			const int lowpassvalues[] = { 130, 500, 900, 4400 };

			OpenXMLConfigStream(false);

			String^ idstring = "ID unknown";
			/*try
			{
				idstring = GetDeviceID().ToString();
			}
			catch (Exception^)
			{
			}*/

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("ID", idstring);
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetNumberTotalChannels()).ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			{
				mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			}
			mXMLTextStream->WriteAttributeString("ad_bits", "12");
			mXMLTextStream->WriteStartElement("Channels");

			unsigned int ChannelNum = 0;
			int startIndex = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{
				startIndex = k < GetNumberConnectorsIn() ? 16 * k : (k < (GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn()) ? GetNumberConnectorsIn() * 16 + (k - GetNumberConnectorsIn()) * 64 : (k - GetNumberConnectorsIn() - GetNumberConnectorsMultipleIn()) + GetNumberConnectorsIn() * 16 + GetNumberConnectorsMultipleIn() * 64);

				if (mConnectors[k]->Adapter != nullptr)
				{
					if (mConnectors[k]->Adapter->ID->Contains("Direct connection to Auxiliary Input"))
					{
						mConnectors[k]->Adapter->AuxWriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							(float)mConnectors[k]->Adapter->AdapterGain,
							(float)mConnectors[k]->AuxScaleFactor,
							mConnectors[k]->unitOfMeasurement
						); //Aux Scale Factor
					}
					else
					mConnectors[k]->Adapter->WriteXMLConfiguration(mXMLTextStream, k, startIndex, 0);
				}
			}

			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			String^ res = CloseXMLConfigStream();
			LOG_EXITING;
			return res;
		}

		String^ DataGeneratorEMGUSB::GetSavingXMLConfiguration()
		{
			LOG_ENTERING;
			int i, j;
			bool IsUSB23 = DEVICE(mModel) == EMGUSBFamilies::EMGUSB2 || DEVICE(mModel) == EMGUSBFamilies::EMGUSB2p;
			bool IsUSB2p = DEVICE(mModel) == EMGUSBFamilies::EMGUSB2p;
			// Decode settings
			const float gainvalues[] = { 0.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f };
			const float highpassvalues[] = { 3.f, 10.f, 100.f, 200.f };
			const float highpassvaluesplus[] = { 0.3f, 10.f, 100.f, 200.f };
			const float lowpassvalues[] = { 130, 500, 900, 4400 };

			UCHAR  buf[128];
			ZeroMemory(buf, sizeof(buf));
			FillMemory(buf, 128, 19);			//for (int k = 0; k < 128; k++) buf[k] = 19; //PB2EB replaced by FillMemory, to TEST

			//////////////////////////////////////////////////////////////////////////////////////////////
			ULONG DecodedBufferSize = IsUSB23 ? SETTINGS_SIZE_BYTES_EMG_USB2 : SETTINGS_SIZE_BYTES_EMG_USB;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize;
			ULONG numberOfBytesTransferred;

			HANDLE hCompletionPort = CreateIoCompletionPort(mDeviceHandle, NULL, 1, 0);
			if (hCompletionPort == NULL)
			{
				//PB2EB	replaced string message with Exception: to TEST
				OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				throw ose;
			}

			OVERLAPPED *OverlappedStruc = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				CloseHandle(hCompletionPort);	//PB2EB added missing closing of hCompletionPort
				hCompletionPort = NULL;
				OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				FireOnError(ose);
				throw ose;
			}

			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			OverlappedStruc->Internal = STATUS_PENDING;

			ULONG SingleAcquisitionBufferSizeInBytes = IsUSB23 ? SETTINGS_SIZE_BYTES_EMG_USB2 : SETTINGS_SIZE_BYTES_EMG_USB;

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;

			SetLastError(0);
			if (DeviceIoControl(mDeviceHandle,
				IOCTL_EMGUSB2_GET_SETTINGS,
				NULL,			// Ptr to InBuffer
				0,				// Length of InBuffer
				&buf[0],        // Ptr to OutBuffer
				IsUSB23 ? SETTINGS_SIZE_BYTES_EMG_USB2 : SETTINGS_SIZE_BYTES_EMG_USB, // Length of OutBuffer
				NULL,			// BytesReturned
				OverlappedStruc) == 0)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					OVERLAPPED *completedOv;
					ULONG_PTR   key;

					if (GetQueuedCompletionStatus(hCompletionPort, &numberOfBytesTransferred,
						&key, &completedOv, INFINITE) == 0)
					{
						exitMainLoop = true;
					}
				}
				else
				{
					CloseHandle(hCompletionPort);
					hCompletionPort = NULL;
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__); //PB2EB modified to exception: to TEST
					throw ose;
				}
			}
			else //PB2ED added "else" case 
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_GET_SETTINGS");
			}

			// Raw settings (for the abstract)
			array<byte>^Firmware = gcnew array<byte>(3);
			array<float>^ m_Gain = gcnew array<float>(mConnectors->Length);
			array<float>^ m_LowPassFilter = gcnew array<float>(mConnectors->Length);
			array<float>^ m_HighPassFilter = gcnew array<float>(mConnectors->Length);
			array<int>^ m_DetectionMode = gcnew array<int>(mConnectors->Length);

			if (IsUSB2p)// EMG-USB2+
			{
				BYTE global1 = buf[0];
				BYTE global2 = buf[1];
				BYTE global3 = buf[2];
				BYTE global4 = buf[3];
				BYTE IN1_S = buf[4];
				BYTE IN1_M = buf[5];
				BYTE IN2_S = buf[6];
				BYTE IN2_M = buf[7];
				BYTE IN3_S = buf[8];
				BYTE IN3_M = buf[9];
				BYTE IN4_S = buf[10];
				BYTE IN4_M = buf[11];
				BYTE MI1_S = buf[12];
				BYTE MI1_M = buf[13];
				BYTE MI2_S = buf[14];
				BYTE MI2_M = buf[15];
				BYTE MI3_S = buf[16];
				BYTE MI3_M = buf[17];
				BYTE BATT = buf[18];
				Firmware[0] = buf[19];
				Firmware[1] = buf[20];
				Firmware[2] = buf[21];
				if ((global4 & 0x10)) // global settings active
				{
					for (int ii = 0; ii < m_Gain->Length - 16; ii++)
					{
						m_Gain[ii] = gainvalues[global2 & 0x07];
						m_LowPassFilter[ii] = lowpassvalues[(global2 >> 6) & 0x03];
						m_HighPassFilter[ii] = highpassvaluesplus[(global2 >> 4) & 0x03];
						m_DetectionMode[ii] = global4 & 0x07;
					}
					for (int ii = 0; ii < 16; ii++)
					{
						if (m_Gain->Length - ii - 1 > 0)
						{
							m_Gain[m_Gain->Length - ii - 1] = 0.5;
							m_LowPassFilter[m_LowPassFilter->Length - ii - 1] = 100000;
							m_DetectionMode[m_DetectionMode->Length - ii - 1] = 3;
						}
					}
				}
				else
				{
					for (int ii = 0; ii < 16; ii++)
					{
						if (m_Gain->Length - ii - 1 > 0)
						{
							m_Gain[m_Gain->Length - ii - 1] = 0.5;
							m_LowPassFilter[m_LowPassFilter->Length - ii - 1] = 100000;
							m_DetectionMode[m_DetectionMode->Length - ii - 1] = 3;
						}
					}
					// in 1
					m_Gain[0] = gainvalues[IN1_S & 0x07];
					m_LowPassFilter[0] = lowpassvalues[(IN1_S >> 6) & 0x03];
					m_HighPassFilter[0] = highpassvaluesplus[(IN1_S >> 4) & 0x03];
					m_DetectionMode[0] = IN1_M & 0x07;
					// in 2
					m_Gain[1] = gainvalues[IN2_S & 0x07];
					m_LowPassFilter[1] = lowpassvalues[(IN2_S >> 6) & 0x03];
					m_HighPassFilter[1] = highpassvaluesplus[(IN2_S >> 4) & 0x03];
					m_DetectionMode[1] = IN2_M & 0x07;
					// in 3
					m_Gain[2] = gainvalues[IN3_S & 0x07];
					m_LowPassFilter[2] = lowpassvalues[(IN3_S >> 6) & 0x03];
					m_HighPassFilter[2] = highpassvaluesplus[(IN3_S >> 4) & 0x03];
					m_DetectionMode[2] = IN3_M & 0x07;
					// in 4
					m_Gain[3] = gainvalues[IN4_S & 0x07];
					m_LowPassFilter[3] = lowpassvalues[(IN4_S >> 6) & 0x03];
					m_HighPassFilter[3] = highpassvaluesplus[(IN4_S >> 4) & 0x03];
					m_DetectionMode[3] = IN4_M & 0x07;
					if (GetNumberConnectorsMultipleIn() >= 1)
					{
						// m1
						m_Gain[4] = gainvalues[MI1_S & 0x07];
						m_LowPassFilter[4] = lowpassvalues[(MI1_S >> 6) & 0x03];
						m_HighPassFilter[4] = highpassvaluesplus[(MI1_S >> 4) & 0x03];
						m_DetectionMode[4] = MI1_M & 0x07;
					}
					if (GetNumberConnectorsMultipleIn() >= 2)
					{
						// m2
						m_Gain[5] = gainvalues[MI2_S & 0x07];
						m_LowPassFilter[5] = lowpassvalues[(MI2_S >> 6) & 0x03];
						m_HighPassFilter[5] = highpassvaluesplus[(MI2_S >> 4) & 0x03];
						m_DetectionMode[5] = MI2_M & 0x07;
					}
					if (GetNumberConnectorsMultipleIn() >= 3)
					{
						// m3
						m_Gain[6] = gainvalues[MI3_S & 0x07];
						m_LowPassFilter[6] = lowpassvalues[(MI3_S >> 6) & 0x03];
						m_HighPassFilter[6] = highpassvaluesplus[(MI3_S >> 4) & 0x03];
						m_DetectionMode[6] = MI3_M & 0x07;
					}
				}
			}
			else if (IsUSB23)// EMG-USB2
			{
				Firmware[0] = buf[22];
				Firmware[1] = buf[23];
				Firmware[2] = buf[24];
				// Raw settings (for the abstract)
				BYTE global2 = buf[1];
				BYTE global3 = buf[2];
				// Global gain (bit 4)
				if (buf[3] & 0x10)
				{
					for (i = 0; i < m_Gain->Length - 16; i++)
					{
						m_Gain[i] = gainvalues[global2 & 0x07];
						m_LowPassFilter[i] = lowpassvalues[(global2 >> 6) & 0x03];
						m_HighPassFilter[i] = highpassvalues[(global2 >> 4) & 0x03];
					}
				}
				else
				{
					for (i = 0; i < 16; i++)
					{
						float gain = gainvalues[buf[i + 4] & 0x07];
						float lowpass = lowpassvalues[(buf[i + 4] >> 6) & 0x03];
						float highpass = highpassvalues[(buf[i + 4] >> 4) & 0x03];
						for (j = 0; j < 16; j++)
						{
							if ((i << 4) + j < m_Gain->Length - 16)
							{
								m_Gain[(i << 4) + j] = gain;
								m_LowPassFilter[(i << 4) + j] = lowpass;
								m_HighPassFilter[(i << 4) + j] = highpass;
							}
						}
					}
				}
				// AUX channels
				for (i = 0; i < 16; i++)
				{
					if (m_Gain->Length - i - 1 > 0)
					{
						m_Gain[m_Gain->Length - i - 1] = 1;
						m_LowPassFilter[m_Gain->Length - i - 1] = 1;
						m_HighPassFilter[m_Gain->Length - i - 1] = 1;
					}
				}
			}
			else
			{
				// EMG-USB
				Firmware[0] = buf[22];
				Firmware[2] = buf[23];
				Firmware[1] = buf[24];
				BYTE param5 = buf[4];

				// Chained mode (bit 5) or global gain (bit 6)
				if (param5 & 0x60)
				{
					for (i = 0; i < m_Gain->Length; i++) {
						if (mConnectors[i]->Adapter != nullptr) {
							if (mConnectors[i]->Adapter->ID->Contains("Auxiliary Input")) {
								m_Gain[i] = 0.5;

							}
							else {

								m_Gain[i] = gainvalues[param5 & 0x07];
							}
						}
					}
				}
				else
				{
					for (i = 0; i < 16; i++)
					{
						float gain1 = gainvalues[(buf[i + 5] >> 4) & 0x07];
						float gain2 = gainvalues[buf[i + 5] & 0x07];
						for (j = 0; j < 4; j++)
						{
							if ((i << 3) + j < m_Gain->Length)
								m_Gain[(i << 3) + j] = gain1;
							if ((i << 3) + j + 4 < m_Gain->Length)
								m_Gain[(i << 3) + j + 4] = gain2;
						}
					}
				}
				for (i = 0; i < m_LowPassFilter->Length - 8; i++)
				{
					m_LowPassFilter[i] = 750;
					m_HighPassFilter[i] = 10;
				}
				// These are raw gains: C# code needs to correct gains according to setup
				// (only needed for EMG-USB)
			}

			OpenXMLConfigStream(false);

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("ID", GetDeviceID().ToString());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//mXMLTextStream->WriteStartElement("Sources");
			for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			{
				mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			}
			//mXMLTextStream->WriteEndElement();//"Sources"
			if (IsUSB2p)
				mXMLTextStream->WriteAttributeString("Firmware", Firmware[0].ToString() + "." + Firmware[1].ToString() + Firmware[2].ToString());
			else
				mXMLTextStream->WriteAttributeString("Firmware", DataConverter::ByteArrayToHexString(Firmware));
			mXMLTextStream->WriteAttributeString("ad_bits", "12");
			mXMLTextStream->WriteStartElement("Settings");
			mXMLTextStream->WriteStartElement("RawSettings");

			array<byte>^ tmp = gcnew array<byte>(IsUSB23 ? 25 : 26);
			for (int k = 0; k < (IsUSB23 ? 25 : 26); k++)
				tmp[k] = buf[k];
			mXMLTextStream->WriteString(DataConverter::ByteArrayToHexString(tmp));
			mXMLTextStream->WriteEndElement();//"RawSettings"
											  //if (IsUSB23)
											  //{
											  //	// Bit 0-3: 0 = Chained Differential, 1 = Looped Differential, 2 = Floating Monopolar,
											  //	// 3 = Referenced Monopolar, 4 = Bipolar, 5 = Hybrid
											  //	int DetectionMode = buf[3] & 0x07;
											  //	int ChainedMode = (DetectionMode == 0 ? 1 : 0);
											  //	mXMLTextStream->WriteStartElement("DetectionMode");
											  //	mXMLTextStream->WriteString(DetectionMode.ToString());
											  //	mXMLTextStream->WriteEndElement();//"DetectionMode"
											  //	mXMLTextStream->WriteStartElement("ChainedMode");
											  //	mXMLTextStream->WriteString(ChainedMode.ToString());
											  //	mXMLTextStream->WriteEndElement();//"ChainedMode"
											  //}
											  //else
											  //{
											  //	// Monopolar or single differential (bit 4)
											  //	int DetectionMode;
											  //	if (buf[4] & 0x10)
											  //		DetectionMode = 0;
											  //	else
											  //		DetectionMode = 1;
											  //	// Chained mode (bit 5)
											  //	if (DetectionMode == 1 && (buf[4] & 0x20) != 0)
											  //		DetectionMode = 2;
											  //	mXMLTextStream->WriteStartElement("DetectionMode");
											  //	mXMLTextStream->WriteString(DetectionMode.ToString());
											  //	mXMLTextStream->WriteEndElement();//DetectionMode"
											  //}
			mXMLTextStream->WriteEndElement();//"Settings"

			mXMLTextStream->WriteStartElement("Channels");

			unsigned int ChannelNum = 0;
			int startIndex = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{
				if (mConnectors[k]->Adapter != nullptr)
				{
					mConnectors[k]->Adapter->WriteSavingXMLConfiguration(
						mXMLTextStream,
						k,
						startIndex,
						0,
						m_Gain[k],
						m_LowPassFilter[k],
						m_HighPassFilter[k],
						m_DetectionMode[k]);

					for (int zz = 0; zz < mConnectors[k]->Adapter->Channels->Length; zz++)
						if (mConnectors[k]->Adapter->Channels[zz] != nullptr)
							startIndex++;
				}
			}

			mXMLTextStream->WriteEndElement();//"Channels"

			mXMLTextStream->WriteEndElement();//"Device"

			String^ res = CloseXMLConfigStream();
			LOG_EXITING;
			return res;
		}

		DataGeneratorEMGUSB^ DataGeneratorEMGUSB::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorEMGUSB^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try{
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				unsigned int deviceModel = System::Convert::ToUInt16(DeviceNode->Attributes["Model"]->Value);
				unsigned int deviceFrequency = System::Convert::ToUInt16(DeviceNode->Attributes["SampleFrequency"]->Value);

				if (deviceName->Contains("EMG-USB"))
				{
					gdr = gcnew OTComm::EMGUSB::DataGeneratorEMGUSB(deviceModel, deviceFrequency);
					System::Xml::XmlNode^ ChannelsNode = DeviceNode->ChildNodes[0];
					for (int ii = 0; ii < ChannelsNode->ChildNodes->Count; ii++)
					{
						System::Xml::XmlNode^ AdapterNode = ChannelsNode->ChildNodes[ii];
						/*DebugLog::Print(AdapterNode->Attributes["ID"]->Value->ToString());
						DebugLog::Print(Assembly::GetExecutingAssembly()->ToString());*/
						GenericAdapter^ ad = GetAdapterFromID(AdapterNode->Attributes["ID"]->Value->ToString());
						unsigned int adPosition = System::Convert::ToUInt16(AdapterNode->Attributes["AdapterIndex"]->Value);
						if (ad != nullptr)
						{
							for (int jj = 0; jj < AdapterNode->ChildNodes->Count; jj++)
							{
								System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[jj];
								GenericChannel^ ch = GetChannelFromID(
									ChannelNode->Attributes["ID"]->Value->ToString(),
									ChannelNode->Attributes["Muscle"]->Value->ToString(),
									ChannelNode->Attributes["Side"]->Value->ToString());
								if (ch != nullptr)
								{
									ad->SetChannel(System::Convert::ToUInt16(ChannelNode->Attributes["Index"]->Value->ToString()), ch);
									try {
										ad->Channels[System::Convert::ToUInt16(ChannelNode->Attributes["Index"]->Value->ToString())]->ChannelPrefix = ChannelNode->Attributes["Prefix"]->Value->ToString();
									}
									catch (Exception^ ex) {
									}
								}
							}
						}
						if (ad->Channels->Length == 16)
							gdr->ConnectIn(adPosition, (AdapterIn^)ad);
						else if (ad->Channels->Length == 64)
							gdr->ConnectMultipleIn(adPosition - (unsigned int)gdr->GetNumberConnectorsIn(), (AdapterMultipleIn ^)ad);
						else{//Aux Scale Factor
							gdr->ConnectAux(adPosition - (unsigned int)gdr->GetNumberConnectorsIn() - (unsigned int)gdr->GetNumberConnectorsMultipleIn(), (AdapterAux^)ad);
						
							try {

								gdr->GetConnectors()[adPosition]->AuxScaleFactor = System::Convert::ToDouble(AdapterNode->Attributes["AuxScaleFactor"]->Value);
								gdr->GetConnectors()[adPosition]->unitOfMeasurement = System::Convert::ToString(AdapterNode->Attributes["UnitOfMeasurement"]->Value);

							}
							catch (Exception^ ex) {
								Console::WriteLine(ex->ToString());
								gdr->GetConnectors()[adPosition]->AuxScaleFactor = 1;
								gdr->GetConnectors()[adPosition]->unitOfMeasurement = "";

							}

						}
					}
				}
				textReader->Close();
			}
			catch (Exception^ ex) {
				textReader->Close();
				throw "Error in FromXMLConfiguration";
			}
			return gdr;
		}

#pragma endregion

#pragma region Connectors and adapters
		bool StringContains(std::string s1, std::string s2) {
			if (s1.find(s2) != std::string::npos) {
				return true;
			}
			return false;
		}

		static std::string toStandardString(System::String^ string)
		{
			using System::Runtime::InteropServices::Marshal;

			if (string->Length == 0 || string->Length < 0)
			{


			}

			System::IntPtr pointer = Marshal::StringToHGlobalAnsi(string);
			char* charPointer = reinterpret_cast<char*>(pointer.ToPointer());
			std::string returnString(charPointer, string->Length);
			Marshal::FreeHGlobal(pointer);


			return returnString;
		}
		GenericAdapter^ DataGeneratorEMGUSB::GetAdapterFromID(String^ adapterID)
		{
			std::string converted = toStandardString(adapterID);

			if (adapterID == ("AD16"))
				return gcnew OTComm::EMGUSB::AdapterInAD16();
			else if (adapterID == ("AD1x16"))
				return gcnew OTComm::EMGUSB::AdapterInAD1x16();
			else if (adapterID == ("AD1x16SD5"))
				return gcnew OTComm::EMGUSB::AdapterInAD1x16SD5();
			else if (adapterID == ("AD1x16SM5"))
				return gcnew OTComm::EMGUSB::AdapterInAD1x16SM5();
			else if (adapterID == ("AD1x16SDD5"))
				return gcnew OTComm::EMGUSB::AdapterInAD1x16SDD5();
			else 	if (adapterID == ("AD2x8"))
				return gcnew OTComm::EMGUSB::AdapterInAD2x8();
			else 	if (adapterID == ("AD4x4"))
				return gcnew OTComm::EMGUSB::AdapterInAD4x4();
			else 	if (adapterID == ("AD8x2JD"))
				return gcnew OTComm::EMGUSB::AdapterInAD8x2JD();
			else if (adapterID == ("AdapterInSplitter64"))
				return gcnew OTComm::EMGUSB::AdapterInSplitter64();
			else if (adapterID == ("AdapterInSplitter32"))
				return gcnew OTComm::EMGUSB::AdapterInSplitter32();
			/*if (adapterID == ("AdapterMultipleInRXERW3432"))
				return gcnew OTComm::EMGUSB::AdapterMultipleInRXERW3432();*/
			else 		if (adapterID == ("AD1x64"))
				return gcnew OTComm::EMGUSB::AdapterMultipleInAD1x64();
			else 		if (adapterID == ("AD64"))
				return gcnew OTComm::EMGUSB::AdapterMultipleInAD64();
			else 		if (adapterID == ("AD2x32"))
				return gcnew OTComm::EMGUSB::AdapterMultipleInAD2x32();
			else 		if (adapterID == ("AdapterMultipleIn"))
				return gcnew OTComm::EMGUSB::AdapterMultipleIn();
			else 		if  (StringContains("Direct connection to Auxiliary Input", converted))
				return gcnew OTComm::EMGUSB::AdapterAux();
			else 		if (adapterID == ("AdapterIn"))
				return gcnew OTComm::EMGUSB::AdapterIn();
			

	

			
			return nullptr;


		}

		

		GenericChannel^ DataGeneratorEMGUSB::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
		{
			if (channelID == ("CoDe1"))
				return gcnew OTComm::Sensors::SensorCoDe1_0(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("CoDe2"))
				return gcnew OTComm::Sensors::SensorCoDe2_0(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("De1"))
				return gcnew OTComm::Sensors::SensorDe1(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("CDE"))
				return gcnew OTComm::Sensors::SensorCDE(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("General"))
				return gcnew OTComm::Sensors::SensorGeneralPurpose(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			//if (channelID == ("ELSCH004q"))
			//	return gcnew OTComm::Sensors::SensorELSCH004q(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH004"))
				return gcnew OTComm::Sensors::SensorELSCH004(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH008"))
				return gcnew OTComm::Sensors::SensorELSCH008(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("SA8_10"))
				return gcnew OTComm::Sensors::SensorSA8_10(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH016"))
				return gcnew OTComm::Sensors::SensorELSCH016(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("SA16_2_5"))
				return gcnew OTComm::Sensors::SensorSA16_2_5(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("SA16_5"))
				return gcnew OTComm::Sensors::SensorSA16_5(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("SA16_10"))
				return gcnew OTComm::Sensors::SensorSA16_10(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM7"))
				return gcnew OTComm::Sensors::SensorELSCH064NM7(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH032NM6"))
				return gcnew OTComm::Sensors::SensorELSCH032NM6(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064R3S"))
				return gcnew OTComm::Sensors::SensorELSCH064R3S(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM5"))
				return gcnew OTComm::Sensors::SensorELSCH064NM5(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM4"))
				return gcnew OTComm::Sensors::SensorELSCH064NM4(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM3"))
				return gcnew OTComm::Sensors::SensorELSCH064NM3(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM2"))
				return gcnew OTComm::Sensors::SensorELSCH064NM2(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("GR10MM0808"))
				return gcnew OTComm::Sensors::SensorGR10MM0808(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("GR08MM1305"))
				return gcnew OTComm::Sensors::SensorGR08MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("GR04MM1305"))
				return gcnew OTComm::Sensors::SensorGR04MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("GR10MM0804"))
				return gcnew OTComm::Sensors::SensorGR10MM0804(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH020EEG"))
				return gcnew OTComm::Sensors::SensorELSCH020EEG(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064EEG"))
				return gcnew OTComm::Sensors::SensorELSCH064EEG(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Acceleration"))
				return gcnew OTComm::Sensors::AUXAcc(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Torque"))
				return gcnew OTComm::Sensors::AUXTor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Force"))
				return gcnew OTComm::Sensors::AUXFor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == "AUX  Trigger")
				return gcnew OTComm::Sensors::AUXTrig(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			//Simone aggiunta controllo sui due "iEMG"
			if (channelID == "iEMGb")
				return gcnew OTComm::Sensors::SensoriEMGbipolar(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == "iEMGm")
				return gcnew OTComm::Sensors::SensoriEMGmonopolar(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));

			return nullptr;
		}

		unsigned int DataGeneratorEMGUSB::GetNumberTotalChannels()
		{
			return GetNumberChannelsNonAux() + GetNumberChannelsAux();
		}

		unsigned int DataGeneratorEMGUSB::GetNumberChannelsNonAux()
		{
			return (((unsigned int)mModel) % 10000);
		}

		unsigned int DataGeneratorEMGUSB::GetNumberChannelsAux()
		{
			switch DEVICE(mModel)
			{
			case EMGUSBFamilies::EMGUSB1: //USB1
				return 8;
			case EMGUSBFamilies::EMGUSB2:
				return 16;
			case EMGUSBFamilies::EMGUSB2p:
				return 16;//???????????????????????????????//PB2ED why ?
			case EMGUSBFamilies::WIRELESS:	//wireless
				return 8;
			case EMGUSBFamilies::MEBA: //MEBA
				return 8;
			default:
				return 0;
			}
		}
		unsigned int DataGeneratorEMGUSB::GetNumberConnectorsAux()
		{
			return GetNumberChannelsAux();
		}

		unsigned int DataGeneratorEMGUSB::GetNumberConnectorsMultipleIn()
		{
			if (mModel == Models::EMG_USB2p_256 || mModel == Models::EMG_USB2_256)
				return 3;
			else if (mModel == Models::EMG_USB2p_128 || mModel == Models::EMG_USB2_128)
				return 1;
			else if ((mModel == Models::EMG_USB2p_64 || mModel == Models::EMG_USB2_64))
				return 0;
			return 0;
		}

		unsigned int DataGeneratorEMGUSB::GetNumberConnectorsIn()
		{
			if (mModel == Models::EMG_USB2p_256 || mModel == Models::EMG_USB2_256)
				return 4;
			else if (mModel == Models::EMG_USB2p_128 || mModel == Models::EMG_USB2_128)
				return 4;
			else if ((mModel == Models::EMG_USB2p_64 || mModel == Models::EMG_USB2_64))
				return 4;
			else if (mModel == Models::EMG_USB1_64)
				return 4;
			else if (mModel == Models::EMG_USB1_128)
				return 8;
			return 0;
		}

		array<unsigned int>^ DataGeneratorEMGUSB::CreateChannelsMapping()
		{
			switch (mModel)
			{
			case Models::EMG_USB1_64:
			{
				return gcnew array<unsigned int>(72) {
					0, 9, 18, 27, 36, 45, 54, 63,
						1, 10, 19, 28, 37, 46, 55, 64, 2, 11, 20, 29, 38, 47, 56, 65, 3,
						12, 21, 30, 39, 48, 57, 66, 4, 13, 22, 31, 40, 49, 58, 67, 5, 14,
						23, 32, 41, 50, 59, 68,
						6, 15, 24, 33, 42, 51, 60, 69, 7, 16, 25, 34, 43, 52, 61, 70, 8,
						17, 26, 35, 44, 53, 62, 71
				};
			}
			default:
			case Models::EMG_USB1_128:
			{
				return gcnew array<unsigned int>(136) {
					0, 17, 34, 51, 68, 85, 102, 119, 1, 18, 35, 52, 69, 86,
						103, 120, 2, 19, 36, 53, 70, 87, 104, 121, 3, 20, 37, 54, 71, 88,
						105, 122, 4, 21, 38, 55, 72, 89, 106, 123, 5, 22, 39, 56, 73, 90,
						107, 124, 6, 23, 40, 57, 74, 91, 108, 125, 7, 24, 41, 58, 75, 92,
						109, 126, 8, 25, 42, 59, 76, 93, 110, 127, 9, 26, 43, 60, 77, 94,
						111, 128, 10, 27, 44, 61, 78, 95, 112, 129, 11, 28, 45, 62, 79, 96,
						113, 130, 12, 29, 46, 63, 80, 97, 114, 131, 13, 30, 47, 64, 81, 98,
						115, 132, 14, 31, 48, 65, 82, 99, 116, 133, 15, 32, 49, 66, 83, 100,
						117, 134, 16, 33, 50, 67, 84, 101, 118, 135
				};
			}
			case Models::EMG_USB2_64:
			case Models::EMG_USB2p_64:
			{
				return gcnew array<unsigned int>(80) {
					0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70,
						75, 1, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 2, 7,
						12, 17, 22, 27, 32, 37, 42, 47, 52, 57, 62, 67, 72, 77, 3, 8, 13, 18,
						23, 28, 33, 38, 43, 48, 53, 58, 63, 68, 73, 78, 4, 9, 14, 19, 24, 29,
						34, 39, 44, 49, 54, 59, 64, 69, 74, 79
				};
			}
			case Models::EMG_USB2_128:
			case Models::EMG_USB2p_128:
			{
				return gcnew array<unsigned int>(144) {
					0, 9, 18, 27, 36, 45, 54, 63, 72, 81, 90, 99, 108, 117,
						126, 135, 1, 10, 19, 28, 37, 46, 55, 64, 73, 82, 91, 100, 109, 118, 127,
						136, 2, 11, 20, 29, 38, 47, 56, 65, 74, 83, 92, 101, 110, 119, 128, 137,
						3, 12, 21, 30, 39, 48, 57, 66, 75, 84, 93, 102, 111, 120, 129, 138, 4, 13,
						22, 31, 40, 49, 58, 67, 76, 85, 94, 103, 112, 121, 130, 139, 5, 14, 23,
						32, 41, 50, 59, 68, 77, 86, 95, 104, 113, 122, 131, 140, 6, 15, 24, 33,
						42, 51, 60, 69, 78, 87, 96, 105, 114, 123, 132, 141, 7, 16, 25, 34, 43,
						52, 61, 70, 79, 88, 97, 106, 115, 124, 133, 142, 8, 17, 26, 35, 44, 53,
						62, 71, 80, 89, 98, 107, 116, 125, 134, 143
				};
			}
			case Models::EMG_USB2_256:
			case Models::EMG_USB2p_256:
			{
				{
					return gcnew array<unsigned int>(272) {
						0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255,
							1, 18, 35, 52, 69, 86, 103, 120, 137, 154, 171, 188, 205, 222, 239, 256,
							2, 19, 36, 53, 70, 87, 104, 121, 138, 155, 172, 189, 206, 223, 240, 257,
							3, 20, 37, 54, 71, 88, 105, 122, 139, 156, 173, 190, 207, 224, 241, 258,
							4, 21, 38, 55, 72, 89, 106, 123, 140, 157, 174, 191, 208, 225, 242, 259,
							5, 22, 39, 56, 73, 90, 107, 124, 141, 158, 175, 192, 209, 226, 243, 260,
							6, 23, 40, 57, 74, 91, 108, 125, 142, 159, 176, 193, 210, 227, 244, 261,
							7, 24, 41, 58, 75, 92, 109, 126, 143, 160, 177, 194, 211, 228, 245, 262,
							8, 25, 42, 59, 76, 93, 110, 127, 144, 161, 178, 195, 212, 229, 246, 263,
							9, 26, 43, 60, 77, 94, 111, 128, 145, 162, 179, 196, 213, 230, 247, 264,
							10, 27, 44, 61, 78, 95, 112, 129, 146, 163, 180, 197, 214, 231, 248, 265,
							11, 28, 45, 62, 79, 96, 113, 130, 147, 164, 181, 198, 215, 232, 249, 266,
							12, 29, 46, 63, 80, 97, 114, 131, 148, 165, 182, 199, 216, 233, 250, 267,
							13, 30, 47, 64, 81, 98, 115, 132, 149, 166, 183, 200, 217, 234, 251, 268,
							14, 31, 48, 65, 82, 99, 116, 133, 150, 167, 184, 201, 218, 235, 252, 269,
							15, 32, 49, 66, 83, 100, 117, 134, 151, 168, 185, 202, 219, 236, 253, 270,
							16, 33, 50, 67, 84, 101, 118, 135, 152, 169, 186, 203, 220, 237, 254, 271
					};
				}
				/*case Models::EMG_USB3:
				{
				return gcnew array<unsigned int>(1);
				}
				case Models::MEBA_32:
				{
				return gcnew array<unsigned int>(40) {
				0, 3, 6, 9, 12, 15, 18, 21, 24, 26, 28, 30, 32, 34, 36, 38,
				1, 4, 7, 10, 13, 16, 19, 22, 25, 27, 29, 31, 33, 35, 37, 39, 2, 5, 8, 11,
				14, 17, 20, 23
				};
				}
				case Models::WIRELESS_32:
				{
				return gcnew array<unsigned int>(40) {
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
				17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
				35, 36, 37, 38, 39
				};
				}*/
			}
			}
		}

		void DataGeneratorEMGUSB::ConnectAux(const unsigned int port, AdapterAux^ adapter)
		{
			if (port >= GetNumberConnectorsAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}
			mConnectors[GetNumberConnectorsMultipleIn() + GetNumberConnectorsIn() + port]->Adapter = adapter;
		}

		void DataGeneratorEMGUSB::ConnectIn(const unsigned int port, AdapterIn^ adapter)
		{
			if (port >= GetNumberConnectorsIn())
				throw gcnew InvalidConnectorNumberException(__FUNCTION__);
			mConnectors[port]->Adapter = adapter;
		}

		void DataGeneratorEMGUSB::ConnectMultipleIn(const unsigned int port, AdapterMultipleIn^ adapter)
		{
			if (port >= GetNumberConnectorsMultipleIn())
				throw gcnew InvalidConnectorNumberException(__FUNCTION__);
			if (GetNumberConnectorsIn() + port >= (unsigned int)mConnectors->Length)
				throw gcnew InvalidConnectorNumberException(__FUNCTION__);
			mConnectors[GetNumberConnectorsIn() + port]->Adapter = adapter;
		}

		array<unsigned int>^ DataGeneratorEMGUSB::GetAcquiredChannels()
		{
			return mAcquiredChannelsIndices;
		}

		array<unsigned int>^ DataGeneratorEMGUSB::GetChannelsMapping()
		{
			return DataGeneratorEMGUSB::CreateChannelsMapping();
		}
#pragma endregion

#pragma region Device properties

		int DataGeneratorEMGUSB::GetDeviceID()
		{
			LOG_ENTERING;
			try
			{
				return (int)ReadDeviceID();
			}
			catch (Exception ^e)
			{
				GetDeviceIDFailedException^ gdIDfe = gcnew GetDeviceIDFailedException(__FUNCTION__, e);
				FireOnError(gdIDfe);
				throw gdIDfe;
			}
			LOG_EXITING;
		}

		Bitmap^ DataGeneratorEMGUSB::Image::get()
		{
			if (mModel == EMGUSB::Models::EMG_USB1_128 || mModel == EMGUSB::Models::EMG_USB1_64)
				return Utilities::GetImageFromEmbeddedResource("EMG-USB.png", true);
			if (mModel == EMGUSB::Models::EMG_USB2_128 || mModel == EMGUSB::Models::EMG_USB2_64 || mModel == EMGUSB::Models::EMG_USB2_256)
				return Utilities::GetImageFromEmbeddedResource("EMG-USB2.png", true);
			if (mModel == EMGUSB::Models::EMG_USB2p_128 || mModel == EMGUSB::Models::EMG_USB2p_64 || mModel == EMGUSB::Models::EMG_USB2p_256)
				return Utilities::GetImageFromEmbeddedResource("EMG_USB2_Plus.jpg", true);
			return Utilities::GetImageFromEmbeddedResource("Default.png", true);
		}

		array<float>^ DataGeneratorEMGUSB::GetGainsConnectors()
		{
			this->CloseDevice();
			this->OpenDevice();
			this->InitializeDevice();

			LOG_ENTERING;
			bool IsUSB23 = DEVICE(mModel) == EMGUSBFamilies::EMGUSB2 || DEVICE(mModel) == EMGUSBFamilies::EMGUSB2p;
			bool IsUSB2p = DEVICE(mModel) == EMGUSBFamilies::EMGUSB2p;
			// Decode settings
			const int gainvalues[] = { 0, 100, 200, 500, 1000, 2000, 5000, 10000 };
			array<byte>^Firmware = gcnew array<byte>(3);
			array<float>^ m_Gain = gcnew array<float>(mConnectors->Length);

			UCHAR buf[128];
			ZeroMemory(buf, sizeof(buf));
			FillMemory(buf, 128, 19);			//for (int k = 0; k < 128; k++) buf[k] = 19; //PB2EB replaced by FillMemory, to TEST

			//////////////////////////////////////////////////////////////////////////////////////////////

			ULONG DecodedBufferSize = IsUSB23 ? SETTINGS_SIZE_BYTES_EMG_USB2 : SETTINGS_SIZE_BYTES_EMG_USB;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize;
			//volatile LONG *RecordingSamples;

			ULONG numberOfBytesTransferred;

			HANDLE hCompletionPort = CreateIoCompletionPort(mDeviceHandle, NULL, 1, 0);
			if (hCompletionPort == NULL)
			{
				return m_Gain;
			}
			OVERLAPPED *OverlappedStruc = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				CloseHandle(hCompletionPort);
				OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				FireOnError(ose);
				throw ose;
			}

			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			OverlappedStruc->Internal = STATUS_PENDING;

			ULONG SingleAcquisitionBufferSizeInBytes = IsUSB23 ? SETTINGS_SIZE_BYTES_EMG_USB2 : SETTINGS_SIZE_BYTES_EMG_USB;

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;

			SetLastError(0);
			if (DeviceIoControl(mDeviceHandle,
				IOCTL_EMGUSB2_GET_SETTINGS,
				NULL,				// Ptr to InBuffer
				0,					// Length of InBuffer
				&buf[0],			// Ptr to OutBuffer
				IsUSB23 ? SETTINGS_SIZE_BYTES_EMG_USB2 : SETTINGS_SIZE_BYTES_EMG_USB, // Length of OutBuffer
				NULL,				// BytesReturned
				OverlappedStruc) == 0)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					OVERLAPPED *completedOv;
					ULONG_PTR   key;

					if (GetQueuedCompletionStatus(hCompletionPort, &numberOfBytesTransferred,
						&key, &completedOv, INFINITE) == 0)
					{
						exitMainLoop = true;
					}
				}
				else
				{
					CloseHandle(hCompletionPort);
					hCompletionPort = NULL;
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__); //PB2EB modified to exception: to TEST

					return m_Gain;
				}
			}
			else //PB2ED added "else" case 
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_GET_SETTINGS");
			}

			if (IsUSB2p)// EMG-USB2+
			{
				BYTE global1 = buf[0];
				BYTE global2 = buf[1];
				BYTE global3 = buf[2];
				BYTE global4 = buf[3];
				BYTE IN1_S = buf[4];
				BYTE IN1_M = buf[5];
				BYTE IN2_S = buf[6];
				BYTE IN2_M = buf[7];
				BYTE IN3_S = buf[8];
				BYTE IN3_M = buf[9];
				BYTE IN4_S = buf[10];
				BYTE IN4_M = buf[11];
				BYTE MI1_S = buf[12];
				BYTE MI1_M = buf[13];
				BYTE MI2_S = buf[14];
				BYTE MI2_M = buf[15];
				BYTE MI3_S = buf[16];
				BYTE MI3_M = buf[17];
				BYTE BATT = buf[18];
				Firmware[0] = buf[19];
				Firmware[1] = buf[20];
				Firmware[2] = buf[21];
				if ((global4 & 0x10)) // global settings active
				{
					for (int ii = 0; ii < m_Gain->Length - 16; ii++)
					{
						m_Gain[ii] = (float)gainvalues[global2 & 0x07];
					}
					for (int ii = 0; ii < 16; ii++)
					{
						if (m_Gain->Length - ii - 1 > 0)
						{
							m_Gain[m_Gain->Length - ii - 1] = (float)0.5;
						}
					}
				}
				else
				{
					for (int ii = 0; ii < 16; ii++)
					{
						if (m_Gain->Length - ii - 1 > 0)
						{
							m_Gain[m_Gain->Length - ii - 1] = (float)0.5;
						}
					}
					//PB2EB why the filter parts are commented?
					// in 1
					m_Gain[0] = (float)gainvalues[IN1_S & 0x07];/*
														 m_LowPassFilter[0] = lowpassvalues[(IN1_S >> 6) & 0x03];
														 m_HighPassFilter[0] = highpassvaluesplus[(IN1_S >> 4) & 0x03];
														 m_DetectionMode[0] = IN1_M & 0x07;*/
														 // in 2
					m_Gain[1] = (float)gainvalues[IN2_S & 0x07];/*
														 m_LowPassFilter[1] = lowpassvalues[(IN2_S >> 6) & 0x03];
														 m_HighPassFilter[1] = highpassvaluesplus[(IN2_S >> 4) & 0x03];
														 m_DetectionMode[1] = IN2_M & 0x07;*/
														 // in 3
					m_Gain[2] = (float)gainvalues[IN3_S & 0x07];/*
														 m_LowPassFilter[2] = lowpassvalues[(IN3_S >> 6) & 0x03];
														 m_HighPassFilter[2] = highpassvaluesplus[(IN3_S >> 4) & 0x03];
														 m_DetectionMode[2] = IN3_M & 0x07;*/
														 // in 4
					m_Gain[3] = (float)gainvalues[IN4_S & 0x07];/*
														 m_LowPassFilter[3] = lowpassvalues[(IN4_S >> 6) & 0x03];
														 m_HighPassFilter[3] = highpassvaluesplus[(IN4_S >> 4) & 0x03];
														 m_DetectionMode[3] = IN4_M & 0x07;*/
					if (GetNumberConnectorsMultipleIn() >= 1)
					{
						// m1
						m_Gain[4] = (float)gainvalues[MI1_S & 0x07];/*
															 m_LowPassFilter[4] = lowpassvalues[(MI1_S >> 6) & 0x03];
															 m_HighPassFilter[4] = highpassvaluesplus[(MI1_S >> 4) & 0x03];
															 m_DetectionMode[4] = MI1_M & 0x07;*/
					}
					if (GetNumberConnectorsMultipleIn() >= 2)
					{
						// m2
						m_Gain[5] = (float)gainvalues[MI2_S & 0x07];/*
															 m_LowPassFilter[5] = lowpassvalues[(MI2_S >> 6) & 0x03];
															 m_HighPassFilter[5] = highpassvaluesplus[(MI2_S >> 4) & 0x03];
															 m_DetectionMode[5] = MI2_M & 0x07;*/
					}
					if (GetNumberConnectorsMultipleIn() >= 3)
					{
						// m3
						m_Gain[6] = (float)gainvalues[MI3_S & 0x07];/*
															 m_LowPassFilter[6] = lowpassvalues[(MI3_S >> 6) & 0x03];
															 m_HighPassFilter[6] = highpassvaluesplus[(MI3_S >> 4) & 0x03];
															 m_DetectionMode[6] = MI3_M & 0x07;*/
					}
				}
			}

			this->CloseDevice();
			return m_Gain;
		}

		array<float>^ DataGeneratorEMGUSB::GetHPsConnectors()
		{
			LOG_ENTERING;
			this->CloseDevice();
			this->OpenDevice();
			this->InitializeDevice();

			bool IsUSB23 = DEVICE(mModel) == EMGUSBFamilies::EMGUSB2 || DEVICE(mModel) == EMGUSBFamilies::EMGUSB2p;
			bool IsUSB2p = DEVICE(mModel) == EMGUSBFamilies::EMGUSB2p;
			// Decode settings
			const float highpassvalues[] = { 3.f, 10.f, 100.f, 200.f };
			const float highpassvaluesplus[] = { 0.3f, 10.f, 100.f, 200.f };

			array<byte>^Firmware = gcnew array<byte>(3);
			//array<float>^ m_Gain = gcnew array<float>(mConnectors->Length);
			//array<int>^ m_LowPassFilter = gcnew array<int>(mConnectors->Length);
			array<float>^ m_HighPassFilter = gcnew array<float>(mConnectors->Length);
			//array<int>^ m_DetectionMode = gcnew array<int>(mConnectors->Length);*/
			UCHAR buf[128];
			ZeroMemory(buf, sizeof(buf));
			FillMemory(buf, 128, 19);			//for (int k = 0; k < 128; k++) buf[k] = 19; //PB2EB replaced by FillMemory, to TEST
			//////////////////////////////////////////////////////////////////////////////////////////////

			ULONG DecodedBufferSize = IsUSB23 ? SETTINGS_SIZE_BYTES_EMG_USB2 : SETTINGS_SIZE_BYTES_EMG_USB;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize;
			//volatile LONG *RecordingSamples;

			ULONG numberOfBytesTransferred;

			HANDLE hCompletionPort = CreateIoCompletionPort(mDeviceHandle, NULL, 1, 0);
			if (hCompletionPort == NULL)
			{
				return m_HighPassFilter;
			}
			OVERLAPPED *OverlappedStruc = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				CloseHandle(hCompletionPort);
				OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				FireOnError(ose);
				throw ose;
			}

			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			OverlappedStruc->Internal = STATUS_PENDING;

			ULONG SingleAcquisitionBufferSizeInBytes = IsUSB23 ? SETTINGS_SIZE_BYTES_EMG_USB2 : SETTINGS_SIZE_BYTES_EMG_USB;

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;

			SetLastError(0);
			if (DeviceIoControl(mDeviceHandle,
				IOCTL_EMGUSB2_GET_SETTINGS,
				NULL, // Ptr to InBuffer
				0,            // Length of InBuffer
				&buf[0],          // Ptr to OutBuffer
				IsUSB23 ? SETTINGS_SIZE_BYTES_EMG_USB2 : SETTINGS_SIZE_BYTES_EMG_USB, // Length of OutBuffer
				NULL,       // BytesReturned
				OverlappedStruc) == 0)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					OVERLAPPED *completedOv;
					ULONG_PTR   key;

					if (GetQueuedCompletionStatus(hCompletionPort, &numberOfBytesTransferred,
						&key, &completedOv, INFINITE) == 0)
					{
						exitMainLoop = true;
					}
				}
				else

				{
					CloseHandle(hCompletionPort);
					hCompletionPort = NULL;
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					//DebugLog::Print("DecodedBuffer is null");
					return m_HighPassFilter;
				}
			}

			if (IsUSB2p)// EMG-USB2+
			{
				BYTE global1 = buf[0];
				BYTE global2 = buf[1];
				BYTE global3 = buf[2];
				BYTE global4 = buf[3];
				BYTE IN1_S = buf[4];
				BYTE IN1_M = buf[5];
				BYTE IN2_S = buf[6];
				BYTE IN2_M = buf[7];
				BYTE IN3_S = buf[8];
				BYTE IN3_M = buf[9];
				BYTE IN4_S = buf[10];
				BYTE IN4_M = buf[11];
				BYTE MI1_S = buf[12];
				BYTE MI1_M = buf[13];
				BYTE MI2_S = buf[14];
				BYTE MI2_M = buf[15];
				BYTE MI3_S = buf[16];
				BYTE MI3_M = buf[17];
				BYTE BATT = buf[18];
				Firmware[0] = buf[19];
				Firmware[1] = buf[20];
				Firmware[2] = buf[21];
				if ((global4 & 0x10)) // global settings active
				{
					for (int ii = 0; ii < m_HighPassFilter->Length - 16; ii++)
					{
						//m_Gain[ii] = gainvalues[global2 & 0x07];
						//m_LowPassFilter[ii] = lowpassvalues[(global2 >> 6) & 0x03];
						m_HighPassFilter[ii] = highpassvaluesplus[(global2 >> 4) & 0x03];
						//m_DetectionMode[ii] = global4 & 0x07;*/
					}
					for (int ii = 0; ii < 16; ii++)
					{
						if (m_HighPassFilter->Length - ii - 1 > 0)
						{
							//m_Gain[m_Gain->Length - ii - 1] = 0.5;
							//m_LowPassFilter[m_LowPassFilter->Length - ii - 1] = 100000;
							//m_DetectionMode[m_DetectionMode->Length - ii - 1] = 3;
						}
					}
				}
				else
				{
					for (int ii = 0; ii < 16; ii++)
					{
						if (m_HighPassFilter->Length - ii - 1 > 0)
						{
							/*m_Gain[m_Gain->Length - ii - 1] = 0.5;
							m_LowPassFilter[m_LowPassFilter->Length - ii - 1] = 100000;
							m_DetectionMode[m_DetectionMode->Length - ii - 1] = 3;*/
						}
					}
					// in 1
					//m_Gain[0] = gainvalues[IN1_S & 0x07];/*
					// m_LowPassFilter[0] = lowpassvalues[(IN1_S >> 6) & 0x03];
					m_HighPassFilter[0] = highpassvaluesplus[(IN1_S >> 4) & 0x03];
					// m_DetectionMode[0] = IN1_M & 0x07;*/
					// in 2
					//m_Gain[1] = gainvalues[IN2_S & 0x07];
					//m_LowPassFilter[1] = lowpassvalues[(IN2_S >> 6) & 0x03];
					m_HighPassFilter[1] = highpassvaluesplus[(IN2_S >> 4) & 0x03];
					//m_DetectionMode[1] = IN2_M & 0x07;*/
					// in 3
					//m_Gain[2] = gainvalues[IN3_S & 0x07];/*
					//m_LowPassFilter[2] = lowpassvalues[(IN3_S >> 6) & 0x03];
					m_HighPassFilter[2] = highpassvaluesplus[(IN3_S >> 4) & 0x03];
					//m_DetectionMode[2] = IN3_M & 0x07; */
					// in 4
					//m_Gain[3] = gainvalues[IN4_S & 0x07];/*
					// m_LowPassFilter[3] = lowpassvalues[(IN4_S >> 6) & 0x03];
					m_HighPassFilter[3] = highpassvaluesplus[(IN4_S >> 4) & 0x03];
					//m_DetectionMode[3] = IN4_M & 0x07;*/
					if (GetNumberConnectorsMultipleIn() >= 1)
					{
						// m1
						//m_Gain[4] = gainvalues[MI1_S & 0x07];/*
						// m_LowPassFilter[4] = lowpassvalues[(MI1_S >> 6) & 0x03];
						m_HighPassFilter[4] = highpassvaluesplus[(MI1_S >> 4) & 0x03];
						// m_DetectionMode[4] = MI1_M & 0x07;*/
					}
					if (GetNumberConnectorsMultipleIn() >= 2)
					{
						// m2
						//m_Gain[5] = gainvalues[MI2_S & 0x07];/*
						// m_LowPassFilter[5] = lowpassvalues[(MI2_S >> 6) & 0x03];
						m_HighPassFilter[5] = highpassvaluesplus[(MI2_S >> 4) & 0x03];
						// m_DetectionMode[5] = MI2_M & 0x07;*/
					}
					if (GetNumberConnectorsMultipleIn() >= 3)
					{
						// m3
						//m_Gain[6] = gainvalues[MI3_S & 0x07];/*
						// m_LowPassFilter[6] = lowpassvalues[(MI3_S >> 6) & 0x03];
						m_HighPassFilter[6] = highpassvaluesplus[(MI3_S >> 4) & 0x03];
						// m_DetectionMode[6] = MI3_M & 0x07;*/
					}
				}
			}
			this->CloseDevice();
			return m_HighPassFilter;
		}

		String^ DataGeneratorEMGUSB::GetDeviceDescription(void)
		{
			return gcnew String("EMG-USB");
		}

		unsigned int DataGeneratorEMGUSB::GetDeviceModel()
		{
			return (unsigned int)mModel;
		}

		bool DataGeneratorEMGUSB::IsOpen()
		{
			return mDeviceIsOpen;
		}

#pragma endregion

#pragma region Public device operations

		void DataGeneratorEMGUSB::OpenDevice(void)
		{
			LOG_ENTERING;
			try
			{
				WCHAR completeDeviceName[MAX_DEVPATH_LENGTH];
				if (!GetDevicePath((LPGUID)&GUID_DEVINTERFACE_EMGUSB2, completeDeviceName, countof(completeDeviceName), 0))
				{
					throw  gcnew OpenDeviceFailedException(__FUNCTION__, gcnew InvalidDevicePath(__FUNCTION__));
				}

				mPortName = gcnew System::String(completeDeviceName);
				//IntPtr  ptrToNativeString  = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(mPortName);
				mDeviceHandle = CreateFile(completeDeviceName,
					GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_WRITE | FILE_SHARE_READ,
					NULL, // default security
					OPEN_EXISTING,
					(FILE_FLAG_OVERLAPPED | SECURITY_IMPERSONATION),
					NULL);
				//System::Runtime::InteropServices::Marshal::FreeHGlobal(ptrToNativeString);
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				// Stop acquisition
				InternalStopUSBDataTransfer();
				DataGeneratorInterface::OpenDevice();
				mDeviceIsOpen = true;
			}
			catch (Exception^e)
			{
				OpenDeviceFailedException^ odfe = gcnew OpenDeviceFailedException(__FUNCTION__, e);
				FireOnError(odfe);
				throw odfe;
			}
			LOG_EXITING;
		}

		void DataGeneratorEMGUSB::InitializeDevice()
		{
			LOG_ENTERING;
			try
			{
				InternalStopUSBDataTransfer();
				ProgramFPGA();
				DataGeneratorInterface::InitializeDevice();
			}
			catch (Exception^e)
			{
				InitializeDeviceFailedException^ idfe = gcnew InitializeDeviceFailedException(__FUNCTION__, e);
				FireOnError(idfe);
				throw idfe;
			}
			LOG_EXITING;
		}

		void DataGeneratorEMGUSB::StartAcquisition(void)
		{
			LOG_ENTERING;
			try
			{

				//OVERLAPPED *OverlappedStruc = NULL; //PB2EB: I commented out these lines with OverlappedStruc, I think they are not required! TEST
				//OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
				//if (OverlappedStruc == NULL)
				//{
				//	throw gcnew OverlappedStructException(__FUNCTION__);
				//}
				DataGeneratorInterface::StartAcquisition();
				mManagedAcquisitionThreadHandle =
					gcnew Thread(gcnew ThreadStart(this, &DataGeneratorEMGUSB::OuternThread));
				mManagedAcquisitionThreadHandle->Start();
			}
			catch (Exception^e)
			{
				StartAcquisitionFailedException^ safe = gcnew StartAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}

		void DataGeneratorEMGUSB::StopAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				if (!mDeviceIsOpen)
				{
					throw gcnew DeviceIsClosedException(__FUNCTION__);
				}
				if (mManagedAcquisitionThreadHandle != nullptr)
				{
					mManagedAcquisitionThreadHandle->Join(1000);
					InternalStopUSBDataTransfer();
				}
				DataGeneratorInterface::StopAcquisition();
			}
			catch (Exception^e)
			{
				StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}

		void DataGeneratorEMGUSB::CloseDevice(void)
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				if (!mDeviceIsOpen)
				{
					throw gcnew DeviceIsClosedException(__FUNCTION__);
				}

				InternalStopUSBDataTransfer();
				CloseHandle(mDeviceHandle);
				mDeviceHandle = INVALID_HANDLE_VALUE;
				mDeviceIsOpen = false;
				DataGeneratorInterface::CloseDevice();
			}
			catch (Exception^e)
			{
				CloseDeviceFailedException^ cdfe = gcnew CloseDeviceFailedException(__FUNCTION__, e);
				FireOnError(cdfe);
				throw cdfe;
			}
			LOG_EXITING;
		}

#pragma endregion

#pragma region Non public device operations

		void realpath(const char *filename, wchar_t *pathbuf, DWORD size)
		{
			LOG_ENTERING;
			OFSTRUCT of;
			HANDLE file = (HANDLE)OpenFile(filename, &of, OF_READ);
			GetFinalPathNameByHandle(file, pathbuf, size, FILE_NAME_OPENED);
			CloseHandle(file);
			LOG_EXITING;
		}

		void DataGeneratorEMGUSB::ProgramFPGA()
		{
			LOG_ENTERING;
			String^ FileName = String::Empty;
			IntPtr ptrtxt = IntPtr::Zero;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
					throw gcnew InvalidHandleException(__FUNCTION__);
				String^ ResourceName = String::Empty;

				FileName = System::IO::Path::GetTempFileName();
				switch (DEVICE(mModel))
				{
				case EMGUSBFamilies::EMGUSB2:
					ResourceName = "emg256.hex";
					break;
				case EMGUSBFamilies::EMGUSB2p:
					ResourceName = "emg256p.hex";
					break;
				case EMGUSBFamilies::MEBA:
					ResourceName = "emg32.hex";
					break;
				case EMGUSBFamilies::WIRELESS:
					ResourceName = "wireless.hex";
					break;
				default:
					ResourceName = "emg128.hex";
					break;
				}

				Assembly ^myAssembly = Assembly::GetExecutingAssembly();
				Stream ^myStream = myAssembly->GetManifestResourceStream(ResourceName);
				FileStream^ fs = gcnew FileStream(FileName, FileMode::Append, FileAccess::Write, FileShare::Write);

				array<Byte>^ buffer = gcnew array<Byte>(32768);
				long long TempPos = myStream->Position;

				while (true)
				{
					int read = myStream->Read(buffer, 0, buffer->Length);
					if (read <= 0) break;
					fs->Write(buffer, 0, read);
				}
				myStream->Close();
				fs->Close();

				// Get a FileSecurity object that represents the
				// current security settings.
				System::Security::AccessControl::FileSecurity ^fSecurity = File::GetAccessControl(FileName);

				//BUG FIX for language(PC DANESE)
				System::Security::Principal::SecurityIdentifier ^everyone =
					gcnew System::Security::Principal::SecurityIdentifier(
						System::Security::Principal::WellKnownSidType::WorldSid, nullptr);

				// Add the FileSystemAccessRule to the security settings.
				fSecurity->AddAccessRule(
					gcnew System::Security::AccessControl::FileSystemAccessRule(everyone,
						System::Security::AccessControl::FileSystemRights::FullControl,
						System::Security::AccessControl::AccessControlType::Allow));

				// Set the new access settings.
				File::SetAccessControl(FileName, fSecurity);

				ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(FileName);
				TCHAR buf[1024];
				realpath((LPCSTR)ptrtxt.ToPointer(), buf, 1024);
				ULONG index;
				if (!DeviceIoControl(mDeviceHandle,
					IOCTL_EMGUSB2_PROGRAM_FPGA,
					buf,  // Ptr to InBuffer
					1024,          // Length of InBuffer
					NULL,         // Ptr to OutBuffer
					0,            // Length of OutBuffer
					&index,       // BytesReturned
					0))
				{
					throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_PROGRAM_FPGA");
				}
			}

			catch (Exception^ e)
			{
				throw e;
			}
			finally
			{
				if (ptrtxt != IntPtr::Zero)
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
				try
				{
					File::Delete(FileName);
				}
				catch (Exception^)
				{
				}
			}
			LOG_EXITING;
		}

		void DataGeneratorEMGUSB::OuternThread()
		{
			LOG_ENTERING;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = NULL;
			try
			{
				mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attribute
					0,				// Initial Stack Size,
					AsyncIoEMGUSB,  // Thread Function
					mDataReaderInterfaceHelper, // Thread parameter
					0,             // Creation Flags
					NULL);        // Don't need the Thread Id.
				if (mAsyncIoThreadHandle == NULL)
				{
					ThreadCreationException^ tce = gcnew ThreadCreationException(__FUNCTION__);
					FireOnError(tce);
					throw tce;
				}
				else
					WaitForSingleObject(mAsyncIoThreadHandle, INFINITE);
			}
			catch (Exception ^e)
			{
				AcquisitionThreadException^ ate = gcnew AcquisitionThreadException(__FUNCTION__, e);
				FireOnError(ate);
			}
			finally //PB2EB: added a finally clause to free mDataReaderInterfaceHelper which is allocated as "new"
			{
				if (mDataReaderInterfaceHelper != NULL)
				{
					delete(mDataReaderInterfaceHelper);
					mDataReaderInterfaceHelper = NULL;
				}

			}

			LOG_EXITING;
		}

		void DataGeneratorEMGUSB::InternalStopUSBDataTransfer(void)
		{
			LOG_ENTERING;
			if (mDeviceHandle != INVALID_HANDLE_VALUE)
			{
				// Stop acquisition
				DWORD read;
				if (!DeviceIoControl(mDeviceHandle,
					IOCTL_EMGUSB2_STOP_ACQUISITION,
					NULL,		// Ptr to InBuffer
					0,          // Length of InBuffer
					NULL,         // Ptr to OutBuffer
					0,            // Length of OutBuffer
					&read,       // BytesReturned
					0)) // Ptr to Overlapped structure
				{
					DeviceIoControlException^ dioce = gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_STOP_ACQUISITION");
					FireOnError(dioce);
					throw dioce;
				}
			}
			LOG_EXITING;
		}

		UCHAR DataGeneratorEMGUSB::ReadDeviceID()
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
					throw gcnew InvalidHandleException(__FUNCTION__);
				ULONG index;
				UCHAR ch = 0;
				UCHAR dev_id = 0;
				if (!DeviceIoControl(mDeviceHandle,
					IOCTL_EMGUSB2_GET_DEVICE_ID,
					&ch,  // Ptr to InBuffer
					1,            // Length of InBuffer
					&dev_id,      // Ptr to OutBuffer
					1,            // Length of OutBuffer
					&index,       // BytesReturned
					0)) {         // Ptr to Overlapped structure
					throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_GET_DEVICE_ID");
				}
				return dev_id;
			}
			catch (Exception^e)
			{
				GetDeviceIDFailedException^ gdIDfe = gcnew GetDeviceIDFailedException(__FUNCTION__, e);
				FireOnError(gdIDfe);
				throw gdIDfe;
			}
			LOG_ENTERING;
		}

		void DataGeneratorEMGUSB::ProgramPIC(String^ hexfile)
		{
			LOG_ENTERING;
			IntPtr ptrtxt = IntPtr::Zero;
			try
			{
				if (mDeviceHandle != INVALID_HANDLE_VALUE)
					throw gcnew InvalidHandleException(__FUNCTION__);
				ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(hexfile);
				TCHAR buf[1024];
				realpath((LPCSTR)ptrtxt.ToPointer(), buf, 1024);
				ULONG index;
				if (!DeviceIoControl(mDeviceHandle,
					IOCTL_EMGUSB2_PROGRAM_PIC,
					(LPVOID)ptrtxt.ToPointer(),  // Ptr to InBuffer
					1024,          // Length of InBuffer
					NULL,         // Ptr to OutBuffer
					0,            // Length of OutBuffer
					&index,       // BytesReturned
					0)) {         // Ptr to Overlapped structure
					throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_PROGRAM_PIC");
				}
			}
			catch (Exception^e)
			{
				ProgramPICFailedException^ ppfe = gcnew ProgramPICFailedException(__FUNCTION__, e);
				FireOnError(ppfe);
				throw ppfe;
			}
			finally
			{
				if (ptrtxt != IntPtr::Zero)
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
			}
		}

		String^ DataGeneratorEMGUSB::GetFirmwareVersion()
		{
			LOG_ENTERING;
			try
			{
				ULONG index;
				UCHAR ch, buf[128];
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}

				ZeroMemory(buf, sizeof(buf));
				if (!DeviceIoControl(mDeviceHandle,
					IOCTL_EMGUSB2_GET_SETTINGS,
					&ch,      // Ptr to InBuffer
					1,            // Length of InBuffer
					buf,          // Ptr to OutBuffer
					sizeof(buf), // Length of OutBuffer
					&index,       // BytesReturned
					0)) {         // Ptr to Overlapped structure
					throw gcnew DeviceIoControlException(__FUNCTION__);
				}
				char m_FirmwareVersion[3];
				if (DEVICE(mModel) != EMGUSBFamilies::EMGUSB2 && DEVICE(mModel) != EMGUSBFamilies::EMGUSB2p)
				{
					// EMG-USB
					m_FirmwareVersion[0] = buf[23];
					m_FirmwareVersion[1] = buf[24];
					m_FirmwareVersion[2] = buf[25];
				}
				else
				{
					// EMG-USB2
					m_FirmwareVersion[0] = buf[22];
					m_FirmwareVersion[1] = buf[23];
					m_FirmwareVersion[2] = buf[24];
				}
				return gcnew String(m_FirmwareVersion);
			}
			catch (Exception^e)
			{
				GetFirmwareFailedException^ gffe = gcnew GetFirmwareFailedException(__FUNCTION__, e);
				FireOnError(gffe);
				throw gffe;
			}
			LOG_EXITING;
		}
		

#pragma endregion


#pragma region Validators
		DataValidator^ DataGeneratorEMGUSB::DefaultTriggerValidator::get()
		{
			return mDefaultDataValidator;
		}
#pragma endregion 

		


#pragma region Managed code

		ULONG _stdcall AsyncIoEMGUSB(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ASYNCH_IO = 10;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;

			ULONG DecodedBufferSize = helper->mSource[0].mAcquiredChannels * NUM_DECODED_BUFFERS * helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize * sizeof(__int16);
			//volatile LONG *RecordingSamples;
			unsigned __int16 *DecodedBuffer;
			unsigned __int16  *DecodedBufferPos;
			unsigned __int16  *DecodedBufferEnd;
			UCHAR * AcquisitionBuffer;

			ULONG numberOfBytesTransferred;

			HANDLE hCompletionPort = CreateIoCompletionPort(helper->mDeviceHandle, NULL, 1, 0);
			if (hCompletionPort == NULL)
			{
				return -1;
			}
			OVERLAPPED *OverlappedStruc = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, NUM_ASYNCH_IO * sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				CloseHandle(hCompletionPort);
				return -1;
			}
			for (int i = 0; i < NUM_ASYNCH_IO; i++)
			{
				ZeroMemory(&OverlappedStruc[i], sizeof(OVERLAPPED));
				OverlappedStruc[i].Internal = STATUS_PENDING;
			}


			ULONG SingleAcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels*helper->mSource[0].mDataRegroupFactor * sizeof(char)*helper->mSource[0].mBytesPerChannel;

			AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, SingleAcquisitionBufferSizeInBytes*NUM_ASYNCH_IO);

			if (AcquisitionBuffer == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				//DebugLog::Print("Acquisition buffer is null");
				return -1;
			}
			else
				;//DebugLog::Print("Acquisition buffer is OK");

			DecodedBuffer = (unsigned __int16 *)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
			if (DecodedBuffer == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				//DebugLog::Print("DecodedBuffer is null");
				return -1;
			}
			else
				;//DebugLog::Print("DecodedBuffer is OK");

			//points to the beginning of the decoded buffer
			DecodedBufferPos = DecodedBuffer;
			DecodedBufferEnd = DecodedBuffer + DecodedBufferSize;

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;

			char OptionsByte = 0;/////////////////FIX
			switch (helper->mDeviceTotalChannels)
			{
			case 72: //64 non aux
			case 80:
				break;
				//case 128:
			default:
				OptionsByte = OptionsByte | 0x04;
				break;
				//case 192:			OptionsByte = OptionsByte | 0x08;			break;
			case 272:// 256:
				OptionsByte = OptionsByte | 0x0c;
				break;
			}
			switch (helper->mSource[0].mSamplingFrequency)
			{
			case 512:
				break;
			case 2048:
			default:
				OptionsByte = OptionsByte | 0x10;
				break;
			case 4096:
			case 5012: // Correzione bug 5012 => 5120
			case 5120:
				OptionsByte = OptionsByte | 0x20;
				break;
			case 10240:
				OptionsByte = OptionsByte | 0x30;
				break;
			}

			OptionsByte = OptionsByte & 0xFC;
			if (!DeviceIoControl(helper->mDeviceHandle,
				IOCTL_EMGUSB2_START_ACQUISITION,
				&OptionsByte,			// Ptr to InBuffer
				1,						// Length of InBuffer
				NULL,					// Ptr to OutBuffer
				0,						// Length of OutBuffer
				&lettitot,				// BytesReturned
				0))						// Ptr to Overlapped structure
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				//DebugLog::Print("DecodedBuffer is null");
				return -1;
			}

			for (int i = 0; i < NUM_ASYNCH_IO; i++)
			{
				BOOL ReadFileRes = ReadFile(helper->mDeviceHandle,
					AcquisitionBuffer + (i * SingleAcquisitionBufferSizeInBytes),
					SingleAcquisitionBufferSizeInBytes, NULL, &OverlappedStruc[i]);

				if (ReadFileRes == FALSE)
				{
					if (GetLastError() != ERROR_IO_PENDING)
					{
						CloseHandle(hCompletionPort);
						hCompletionPort = NULL;
						LocalFree((void *)OverlappedStruc);
						OverlappedStruc = NULL;
						LocalFree((void *)AcquisitionBuffer);
						AcquisitionBuffer = NULL;
						return -1;
					}
				}
			}

			WHILE(*mMustStopAcquisitionThread == false)
			{
				SetLastError(0);
				OVERLAPPED *completedOv;
				ULONG_PTR   key;

				if (GetQueuedCompletionStatus(hCompletionPort, &numberOfBytesTransferred,
					&key, &completedOv, INFINITE) == 0)
				{
					helper->mError("", GetLastError(), __FUNCTION__);
					exitMainLoop = true;
				}

				if (exitMainLoop)break;
				lettitot += numberOfBytesTransferred;
				size_t ReceivedBufferIndex = completedOv - OverlappedStruc;

				unsigned __int16 * DecodedBufferCurrentStart = DecodedBufferPos;
				unsigned __int16* AcquisitionBufferCurrentReadStart =
					(unsigned __int16*)&AcquisitionBuffer[ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes];

				int index = 0;
				for (unsigned int koffset = 0; koffset < helper->mSource[0].mDataRegroupFactor; koffset++)
				{
					for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
					{
						//add control for buffer overflow
						*DecodedBufferPos = AcquisitionBufferCurrentReadStart[ChannelMappings[channel]];

						//index= (ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes/2) + (offset + ChannelMappings[channel]);
						//*DecodedBufferPos = ((unsigned __int16*)AcquisitionBuffer)[index];

						//index = (i * SingleAcquisitionBufferSizeInBytes) + helper->mBytesPerChannel * (offset + ChannelMappings[channel]);
						//*DecodedBufferPos = (__int16)(AcquisitionBuffer[index + 1] << 8) + AcquisitionBuffer[index];

						//*DecodedBufferPos = (__int16)((AcquisitionBuffer[(i * SingleAcquisitionBufferSizeInBytes) + helper->mBytesPerChannel * (offset + ChannelMappings[channel])+1]) << 8) + (__int16)((AcquisitionBuffer[(i * SingleAcquisitionBufferSizeInBytes) + helper->mBytesPerChannel * (offset + ChannelMappings[channel])]));
						//*DecodedBufferPos <<= 8;
						//*DecodedBufferPos += AcquisitionBuffer[cm];
						DecodedBufferPos++;
					}
					AcquisitionBufferCurrentReadStart += helper->mDeviceTotalChannels;
				}

				if (*mMustStopAcquisitionThread) break;

				helper->mNewDataAvailable(0, DataSize::SixteenBits,
					DecodedBufferCurrentStart,
					nullptr,
					helper->mSource[0].mAcquiredChannels,
					0,
					helper->mSource[0].mDataRegroupFactor,
					0);

				if (DecodedBufferPos == DecodedBufferEnd)
					DecodedBufferPos = DecodedBuffer;
				if (*mMustStopAcquisitionThread) break;

				BOOL ReadFileRes = ReadFile(helper->mDeviceHandle,
					AcquisitionBuffer + (ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes),
					SingleAcquisitionBufferSizeInBytes, NULL, completedOv);

				if (ReadFileRes == FALSE)
				{
					if (GetLastError() != ERROR_IO_PENDING)
					{
					}
				}
			}//while

			CancelIo(helper->mDeviceHandle);

			// We are not recording any more
			//InterlockedExchange(RecordingSamples, 0);

			if (!*mMustStopAcquisitionThread)
			{
				// Signal that acquisition ended before time
				//GlobalErrorDispatcher("An error occurred",helper->mError);
			}

			if (AcquisitionBuffer)
			{
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
			}

			/*if (OverlappedStruc)
			{
			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;
			}*/

			if (DecodedBuffer)
			{
				LocalFree((void *)DecodedBuffer);
				DecodedBuffer = NULL;
			}
			helper->mStopAcquisition();
			return 0;
		}

		static BOOL GetDevicePath(IN  LPGUID InterfaceGuid, __out_ecount(BufLen) PWSTR DevicePath, __in size_t BufLen, int DeviceIndex)
		{
			HDEVINFO HardwareDeviceInfo;
			SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
			PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData = NULL;
			ULONG Length, RequiredLength = 0;
			BOOL bResult;
			HRESULT hr;

			HardwareDeviceInfo = SetupDiGetClassDevs(InterfaceGuid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

			if (HardwareDeviceInfo == INVALID_HANDLE_VALUE) {
				return FALSE;
			}
			DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

			bResult = SetupDiEnumDeviceInterfaces(HardwareDeviceInfo, 0, InterfaceGuid, DeviceIndex, &DeviceInterfaceData);
			if (bResult == FALSE)
			{
				SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
				return FALSE;
			}

			SetupDiGetDeviceInterfaceDetail(
				HardwareDeviceInfo,
				&DeviceInterfaceData,
				NULL,
				0,
				&RequiredLength,
				NULL
			);

			DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, RequiredLength);

			if (DeviceInterfaceDetailData == NULL)
			{
				SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
				return FALSE;
			}

			DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			Length = RequiredLength;
			bResult = SetupDiGetDeviceInterfaceDetail(
				HardwareDeviceInfo,
				&DeviceInterfaceData,
				DeviceInterfaceDetailData,
				Length,
				&RequiredLength,
				NULL);

			if (bResult == FALSE)
			{
				SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
				LocalFree(DeviceInterfaceDetailData);
				return FALSE;
			}

			hr = StringCchCopy(DevicePath, BufLen, DeviceInterfaceDetailData->DevicePath);
			if (FAILED(hr))
			{
				SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
				LocalFree(DeviceInterfaceDetailData);
				return FALSE;
			}

			SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
			LocalFree(DeviceInterfaceDetailData);
			return TRUE;
		}
#pragma endregion
	}
}