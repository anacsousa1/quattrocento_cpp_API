
#include <windows.h>
#include "DataConverter.h"
#include "DataGeneratorMEBA.h"
#include <guiddef.h>
#include <initguid.h>
#include <strsafe.h>
#include <setupapi.h>
#include <basetyps.h>
#include <string>

using namespace System::IO;
using namespace System::Reflection;
using namespace System::Xml;

namespace OTComm
{
	namespace MEBA
	{

DEFINE_GUID(GUID_DEVINTERFACE_EMGUSB2, 0x9d57f99f, 0x33cb, 0x488d, 0xa5, 0x57, 0x2c, 0x5d, 0x0f, 0xd5, 0x1d, 0x85);
#define IOCTL_INDEX             0x800
#define FILE_DEVICE_EMGUSB2   0x65500

#define SETTINGS_SIZE_BYTES_EMG_USB   26
#define SETTINGS_SIZE_BYTES_EMG_USB2  25

#define IOCTL_EMGUSB2_START_ACQUISITION CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 6, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_STOP_ACQUISITION	CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 7, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_GET_DEVICE_ID		CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 4, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_EMGUSB2_PROGRAM_FPGA	    CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 5, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_PROGRAM_PIC       CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 11,METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_GET_SETTINGS	    CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 8, METHOD_BUFFERED, FILE_READ_ACCESS)
#pragma managed

#pragma region Constructors and destructors

		DataGeneratorMEBA::DataGeneratorMEBA(Frequency SamplingFrequency) :
			DataGeneratorInterface(),
			mDeviceIsOpen(false), mDataReaderInterfaceHelper(NULL),
			mTestMode(false), mManagedAcquisitionThreadHandle(nullptr)
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
			}
			catch (Exception ^e)
			{
				ConstructorFailedException^ cfe = gcnew ConstructorFailedException(__FUNCTION__, e);
				throw cfe;
			}
		}

		DataGeneratorMEBA::~DataGeneratorMEBA()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorMEBA();
		}

		DataGeneratorMEBA::!DataGeneratorMEBA()
		{

		}

#pragma endregion

#pragma region XML files
		String^ DataGeneratorMEBA::GetDeviceXMLConfiguration()
		{
			LOG_ENTERING;
			// Decode settings
			const int gainvalues[] = { 0, 100, 200, 500, 1000, 2000, 5000, 10000 };
			const int highpassvalues[] = { 3, 10, 100, 200 };
			const int lowpassvalues[] = { 130, 500, 900, 4400 };

			//ULONG index;
			//UCHAR ch, buf[128];
			//ZeroMemory(buf, sizeof(buf));
			//if (!DeviceIoControl(mDeviceHandle,
			//	IOCTL_EMGUSB2_GET_SETTINGS,
			//	&ch,		  // Ptr to InBuffer
			//	1,            // Length of InBuffer
			//	buf,          // Ptr to OutBuffer
			//	sizeof(buf), // Length of OutBuffer
			//	&index,       // BytesReturned
			//	0))
			//{         // Ptr to Overlapped structure
			//	LOG_EXITING;
			//		return "";
			//}

			//// Raw settings (for the abstract)
			//array<byte>^Firmware = gcnew array<byte>(3);
			//array<int>^ m_Gain = gcnew array<int>(mDeviceTotalChannels);
			//array<int>^ m_LowPassFilter = gcnew array<int>(mDeviceTotalChannels);
			//array<int>^ m_HighPassFilter = gcnew array<int>(mDeviceTotalChannels);

			//if (IsUSB23)// EMG-USB2
			//{
			//	Firmware[0] = buf[23];
			//	Firmware[2] = buf[24];
			//	Firmware[1] = buf[25];
			//	// Raw settings (for the abstract)
			//	BYTE global2 = buf[1];
			//	BYTE global3 = buf[2];
			//	// Global gain (bit 4)
			//	if (buf[3] & 0x10)
			//	{
			//		for (i = 0; i < m_Gain->Length - 16; i++)
			//		{
			//			m_Gain[i] = gainvalues[global2 & 0x07];
			//			m_LowPassFilter[i] = lowpassvalues[(global2 >> 6) & 0x03];
			//			m_HighPassFilter[i] = highpassvalues[(global2 >> 4) & 0x03];
			//		}
			//	}
			//	else
			//	{
			//		for (i = 0; i < 16; i++)
			//		{
			//			int gain = gainvalues[buf[i + 4] & 0x07];
			//			int lowpass = lowpassvalues[(buf[i + 4] >> 6) & 0x03];
			//			int highpass = highpassvalues[(buf[i + 4] >> 4) & 0x03];
			//			for (j = 0; j < 16; j++)
			//			{
			//				if ((i << 4) + j < m_Gain->Length - 16)
			//				{
			//					m_Gain[(i << 4) + j] = gain;
			//					m_LowPassFilter[(i << 4) + j] = lowpass;
			//					m_HighPassFilter[(i << 4) + j] = highpass;
			//				}
			//			}
			//		}
			//	}
			//	// AUX channels
			//	for (i = 0; i < 16; i++)
			//	{
			//		if (m_Gain->Length - i - 1 > 0)
			//		{
			//			m_Gain[m_Gain->Length - i - 1] = 1;
			//			m_LowPassFilter[m_Gain->Length - i - 1] = 1;
			//			m_HighPassFilter[m_Gain->Length - i - 1] = 1;
			//		}
			//	}
			//}
			//else
			//{
			//	// EMG-USB		
			//	Firmware[0] = buf[22];
			//	Firmware[2] = buf[23];
			//	Firmware[1] = buf[24];
			//	BYTE param5 = buf[4];

			//	// Chained mode (bit 5) or global gain (bit 6)
			//	if (param5 & 0x60)
			//	{
			//		for (i = 0; i < m_Gain->Length; i++)
			//			m_Gain[i] = gainvalues[param5 & 0x07];
			//	}
			//	else
			//	{
			//		for (i = 0; i < 16; i++)
			//		{
			//			int gain1 = gainvalues[(buf[i + 5] >> 4) & 0x07];
			//			int gain2 = gainvalues[buf[i + 5] & 0x07];
			//			for (j = 0; j < 4; j++)
			//			{
			//				if ((i << 3) + j < m_Gain->Length)
			//					m_Gain[(i << 3) + j] = gain1;
			//				if ((i << 3) + j + 4 < m_Gain->Length)
			//					m_Gain[(i << 3) + j + 4] = gain2;
			//			}
			//		}
			//	}
			//	for (i = 0; i < m_LowPassFilter->Length - 8; i++)
			//	{
			//		m_LowPassFilter[i] = 750;
			//		m_HighPassFilter[i] = 10;
			//	}
			//	// These are raw gains: C# code needs to correct gains according to setup
			//	// (only needed for EMG-USB)
			//}

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
					mConnectors[k]->Adapter->WriteXMLConfiguration(mXMLTextStream, k, startIndex, 0);
				}
			}

			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			String^ res = CloseXMLConfigStream();
			LOG_EXITING;
			return res;
		}

		String^ DataGeneratorMEBA::GetSavingXMLConfiguration()
		{
			LOG_ENTERING;
			int i, j;

			// Decode settings
			const float gainvalues[] = { 0.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f };
			const float highpassvalues[] = { 3.f, 10.f, 100.f, 200.f };
			const float highpassvaluesplus[] = { 0.3f, 10.f, 100.f, 200.f };
			const float lowpassvalues[] = { 130, 500, 900, 4400 };

			UCHAR  buf[128];
			ZeroMemory(buf, sizeof(buf));
			FillMemory(buf, 128, 19);			//for (int k = 0; k < 128; k++) buf[k] = 19; //PB2EB replaced by FillMemory, to TEST

												//////////////////////////////////////////////////////////////////////////////////////////////
			ULONG DecodedBufferSize = SETTINGS_SIZE_BYTES_EMG_USB2;
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

			ULONG SingleAcquisitionBufferSizeInBytes = SETTINGS_SIZE_BYTES_EMG_USB2;

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;

			SetLastError(0);
			if (DeviceIoControl(mDeviceHandle,
				IOCTL_EMGUSB2_GET_SETTINGS,
				NULL,			// Ptr to InBuffer
				0,				// Length of InBuffer
				&buf[0],        // Ptr to OutBuffer
				SETTINGS_SIZE_BYTES_EMG_USB2, // Length of OutBuffer
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

				Firmware[0] = buf[22];
				Firmware[1] = buf[23];
				Firmware[2] = buf[24];
				// Raw settings (for the abstract)
				BYTE global2 = buf[1];
				BYTE global3 = buf[2];
				// Global gain (bit 4)
				if (buf[3] & 0x10)
				{
					for (i = 0; i < m_Gain->Length - 8; i++)
					{
						m_Gain[i] = gainvalues[global2 & 0x07];
						m_LowPassFilter[i] = lowpassvalues[(global2 >> 6) & 0x03];
						m_HighPassFilter[i] = highpassvalues[(global2 >> 4) & 0x03];
					}
				}
				else
				{
					for (i = 0; i < 8; i++)
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
				for (i = 0; i < 8; i++)
				{
					if (m_Gain->Length - i - 1 > 0)
					{
						m_Gain[m_Gain->Length - i - 1] = 1;
						m_LowPassFilter[m_Gain->Length - i - 1] = 1;
						m_HighPassFilter[m_Gain->Length - i - 1] = 1;
					}
				}
			

			OpenXMLConfigStream(false);

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("ID", GetDeviceID().ToString());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//mXMLTextStream->WriteStartElement("Sources");
			for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			{
				mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			}
			//mXMLTextStream->WriteEndElement();//"Sources"
			mXMLTextStream->WriteAttributeString("Firmware", DataConverter::ByteArrayToHexString(Firmware));
			mXMLTextStream->WriteAttributeString("ad_bits", "12");
			mXMLTextStream->WriteStartElement("Settings");
			mXMLTextStream->WriteStartElement("RawSettings");

			array<byte>^ tmp = gcnew array<byte>(25);
			for (int k = 0; k < (25); k++)
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

		DataGeneratorMEBA^ DataGeneratorMEBA::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorMEBA^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try{
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				unsigned int deviceFrequency = System::Convert::ToUInt16(DeviceNode->Attributes["SampleFrequency"]->Value);

				if (deviceName->Contains("MEBA"))
				{
					gdr = gcnew OTComm::MEBA::DataGeneratorMEBA(deviceFrequency);
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
						else
							gdr->ConnectAux(adPosition - (unsigned int)gdr->GetNumberConnectorsIn() - (unsigned int)gdr->GetNumberConnectorsMultipleIn(), (AdapterAux^)ad);
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
		unsigned int DataGeneratorMEBA::GetNumberConnectorsAux()
		{
			return GetNumberChannelsAux();
		}

		unsigned int DataGeneratorMEBA::GetNumberConnectorsMultipleIn()
		{
			return 0;
		}

		unsigned int DataGeneratorMEBA::GetNumberConnectorsIn()
		{
			return 2;
		}

		array<unsigned int>^ DataGeneratorMEBA::CreateChannelsMapping()
		{
			return gcnew array<unsigned int>(40) {
				0, 3, 6, 9, 12, 15, 18, 21, 24, 26, 28, 30, 32, 34, 36, 38,
					1, 4, 7, 10, 13, 16, 19, 22, 25, 27, 29, 31, 33, 35, 37, 39, 2, 5, 8, 11,
					14, 17, 20, 23
			};
		}

		void DataGeneratorMEBA::ConnectAux(const unsigned int port, AdapterAux^ adapter)
		{
			if (port >= GetNumberConnectorsAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}
			unsigned int position = GetNumberConnectorsMultipleIn() + GetNumberConnectorsIn() + port;
			if (position >= (unsigned int) mConnectors->Length)
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}
			mConnectors[position]->Adapter = adapter;
		}

		void DataGeneratorMEBA::ConnectIn(const unsigned int port, AdapterIn^ adapter)
		{
			if (port >= GetNumberConnectorsIn())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;

			}
			mConnectors[port]->Adapter = adapter;
		}

		array<unsigned int>^ DataGeneratorMEBA::GetAcquiredChannels()
		{
			return mAcquiredChannelsIndices;
		}

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
		GenericAdapter^ DataGeneratorMEBA::GetAdapterFromID(String^ adapterID)
		{
			std::string converted = toStandardString(adapterID);

			if (adapterID == ("AD16"))
				return gcnew OTComm::MEBA::AdapterInAD16();
			else if (adapterID == ("AD1x16"))
				return gcnew OTComm::MEBA::AdapterInAD1x16();
			else if (adapterID == ("AD1x16SD5"))
				return gcnew OTComm::MEBA::AdapterInAD1x16SD5();
			else if (adapterID == ("AD1x16SM5"))
				return gcnew OTComm::MEBA::AdapterInAD1x16SM5();
			else if (adapterID == ("AD1x16SDD5"))
				return gcnew OTComm::MEBA::AdapterInAD1x16SDD5();
			else if (adapterID == ("AD2x8"))
				return gcnew OTComm::MEBA::AdapterInAD2x8();
			else if (adapterID == ("AD4x4"))
				return gcnew OTComm::MEBA::AdapterInAD4x4();
			else if (adapterID == ("AD8x2JD"))
				return gcnew OTComm::MEBA::AdapterInAD8x2JD();
			else if (StringContains("Direct connection to Auxiliary Input", converted))
				return gcnew OTComm::MEBA::AdapterAux();
			else if (adapterID == ("AdapterIn"))
				return gcnew OTComm::MEBA::AdapterIn();

			return nullptr;
		}

		GenericChannel^ DataGeneratorMEBA::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
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
			if (channelID == ("AUX  Acceleration"))
				return gcnew OTComm::Sensors::AUXAcc(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Torque"))
				return gcnew OTComm::Sensors::AUXTor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Force"))
				return gcnew OTComm::Sensors::AUXFor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == "AUX  Trigger")
				return gcnew OTComm::Sensors::AUXTrig(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			return nullptr;
		}


		unsigned int DataGeneratorMEBA::GetNumberTotalChannels()
		{
			return GetNumberChannelsNonAux() + GetNumberChannelsAux();
		}

		array<unsigned int>^ DataGeneratorMEBA::GetChannelsMapping()
		{
			return DataGeneratorMEBA::CreateChannelsMapping();
		}

		unsigned int DataGeneratorMEBA::GetNumberChannelsNonAux()
		{
			return 32;
		}

		unsigned int DataGeneratorMEBA::GetNumberChannelsAux()
		{
			return 8;
		}
		
#pragma endregion

#pragma region Device properties 

		

		
		int DataGeneratorMEBA::GetDeviceID()
		{
			LOG_ENTERING;
			try
			{
				return (int)ReadDeviceID();
			}
			catch(Exception^e)
			{
				throw e;
			}
			LOG_EXITING;
		}


		Bitmap^ DataGeneratorMEBA::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("MEBA.png", true);
		}

		unsigned int DataGeneratorMEBA::GetDeviceModel()
		{
			return 0;
		}

		bool DataGeneratorMEBA::IsOpen()
		{
			return mDeviceIsOpen;
		}

		String^ DataGeneratorMEBA::GetDeviceDescription(void)
		{
			String^ res = gcnew String("MEBA");
			//TODO: come mai è commentato????
			//if (mDeviceHandle != INVALID_HANDLE_VALUE)
			//{
			//	ULONG index;
			//	UCHAR ch = 0, dev_id = 0;
			//	if (DeviceIoControl(mDeviceHandle,
			//		IOCTL_EMGUSB2_GET_DEVICE_ID,
			//		&ch,  // Ptr to InBuffer
			//		1,            // Length of InBuffer
			//		&dev_id,      // Ptr to OutBuffer
			//		1,            // Length of OutBuffer
			//		&index,       // BytesReturned
			//		0)) {         // Ptr to Overlapped structure
			//		res += String::Format("; ID={0}", dev_id);
			//	}
			//}
			return res;
		}

#pragma endregion

#pragma region Public device operations

		void DataGeneratorMEBA::OpenDevice(void)
		{
			LOG_ENTERING;
			try
			{
				WCHAR completeDeviceName[MAX_DEVPATH_LENGTH];
				if (!GetDevicePath((LPGUID)&GUID_DEVINTERFACE_EMGUSB2, completeDeviceName, countof(completeDeviceName), 0))
				{
					throw gcnew InvalidDevicePath(__FUNCTION__);
				}
				mPortName = gcnew System::String(completeDeviceName);
				mDeviceHandle = CreateFile(completeDeviceName,
					GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_WRITE | FILE_SHARE_READ,
					NULL, // default security
					OPEN_EXISTING,
					(FILE_FLAG_OVERLAPPED | SECURITY_IMPERSONATION),
					NULL);
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}

				// Stop acquisition
				InternalStopUSBDataTransfer();
				mDeviceIsOpen = true;
				DataGeneratorInterface::OpenDevice();
			}
			catch (Exception^e)
			{
				OpenDeviceFailedException^odfe = gcnew OpenDeviceFailedException(__FUNCTION__, e);
				FireOnError(odfe);
				throw odfe;
			}
			LOG_EXITING;
		}

		void DataGeneratorMEBA::InitializeDevice()
		{
			LOG_ENTERING;
			try
			{
				InternalStopUSBDataTransfer();
				ProgramFPGA();
				DataGeneratorInterface::InitializeDevice();
			}
			catch (Exception ^e)
			{
				InitializeDeviceFailedException^ idfe = gcnew InitializeDeviceFailedException(__FUNCTION__, e);
				FireOnError(idfe);
				throw idfe;
			}
			LOG_EXITING;				
		}

		void DataGeneratorMEBA::StartAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				OVERLAPPED *OverlappedStruc;
				OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
				if (OverlappedStruc == NULL)
				{
					OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				}
				else
				{
					DataGeneratorInterface::StartAcquisition();
					mManagedAcquisitionThreadHandle =
						gcnew Thread(gcnew ThreadStart(this, &DataGeneratorMEBA::OuternThread));
					mManagedAcquisitionThreadHandle->Start();
				}
			}
			catch (Exception^e)
			{
				StartAcquisitionFailedException ^safe = gcnew StartAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}

		void DataGeneratorMEBA::StopAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}

				DataGeneratorInterface::StopAcquisition();
				if (mManagedAcquisitionThreadHandle != nullptr)
				{
					mManagedAcquisitionThreadHandle->Join(1000);
					InternalStopUSBDataTransfer();
				}
			}
			catch (Exception^e)
			{
				StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}

		void DataGeneratorMEBA::CloseDevice(void)
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
					throw gcnew InvalidHandleException(__FUNCTION__);
				}

				InternalStopUSBDataTransfer();
				CloseHandle(mDeviceHandle);
				mDeviceHandle = INVALID_HANDLE_VALUE;
				mDeviceIsOpen = false;
				DataGeneratorInterface::CloseDevice();
			}
			catch (Exception^e)
			{
				CloseDeviceFailedException^cdfe = gcnew CloseDeviceFailedException(__FUNCTION__, e);
				FireOnError(cdfe);
				throw cdfe;
			}
			LOG_EXITING;
		}

#pragma endregion

#pragma region Public device operations

		UCHAR DataGeneratorMEBA::ReadDeviceID()
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
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
				throw e;
			}
			LOG_EXITING;
		}
		void DataGeneratorMEBA::InternalStopUSBDataTransfer(void)
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
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
					throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_STOP_ACQUISITION");
				}
			}
			catch (Exception^e)
			{
				throw e;
			}
			LOG_EXITING;
		}

		void realpath(const char *filename, wchar_t *pathbuf, DWORD size)
		{
			OFSTRUCT of;
			HANDLE file = (HANDLE)(unsigned int)OpenFile(filename, &of, OF_READ);
			GetFinalPathNameByHandle(file, pathbuf, size, FILE_NAME_OPENED);
			CloseHandle(file);
		}

		void DataGeneratorMEBA::ProgramPIC(String^ hexfile)
		{
			LOG_ENTERING;
			IntPtr ptrtxt = IntPtr::Zero;
			try
			{
				
				if (mDeviceHandle != INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
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
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
					throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_PROGRAM_PIC");
				}				
			}
			catch (Exception^e)
			{
				throw e;
			}
			finally
			{
				if(ptrtxt!=IntPtr::Zero)
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
			}
			LOG_EXITING;
		}

		String^ DataGeneratorMEBA::GetFirmwareVersion()
		{
			return gcnew String("");
		}

		void DataGeneratorMEBA::ProgramFPGA()
		{
			LOG_ENTERING;
			IntPtr ptrtxt = IntPtr::Zero;
			String^ FileName = String::Empty;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}

				String^ ResourceName;
				FileName = System::IO::Path::GetTempFileName();

				ResourceName = "emg32.hex";

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
				{         // Ptr to Overlapped structure
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
					File::Delete(FileName);
					throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_PROGRAM_FPGA");
				}

			}
			catch (Exception^ e)
			{
				ProgramFPGAFailedException^ pffe = gcnew ProgramFPGAFailedException(__FUNCTION__, e);
				throw pffe;
			}
			finally
			{
				if (ptrtxt != IntPtr::Zero)
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
				if (FileName != String::Empty)
				{
					try
					{
						File::Delete(FileName);
					}
					catch (Exception ^)
					{
					}
				}
			}
			LOG_EXITING;
		}

		void DataGeneratorMEBA::OuternThread()
		{
			LOG_ENTERING;
			try
			{
				DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoEMGUSB,  // Thread Func
					mDataReaderInterfaceHelper, // Thread parameter
					0,             // Creation Flags
					NULL);        // Don't need the Thread Id.
				if (mAsyncIoThreadHandle == NULL)
				{
					ThreadCreationException^ tce = gcnew ThreadCreationException(__FUNCTION__);
				}
				WaitForSingleObject(mAsyncIoThreadHandle, INFINITE);
			}
			catch (Exception^e)
			{
				AcquisitionThreadException^ ate = gcnew AcquisitionThreadException(__FUNCTION__, e);
				FireOnError(ate);
			}
			LOG_EXITING;
		}

#pragma endregion

#pragma region Unmanaged code
#pragma unmanaged


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
				1,				// Length of InBuffer
				NULL,			// Ptr to OutBuffer
				0,				// Length of OutBuffer
				&lettitot,      // BytesReturned
				0))  // Ptr to Overlapped structure
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
					//ULONG offset = koffset * helper->mDeviceTotalChannels;

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

				helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels, 0,helper->mSource[0].mDataRegroupFactor,0);

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
				//wprintf(L"SetupDiGetClassDevs failed!\n");
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

