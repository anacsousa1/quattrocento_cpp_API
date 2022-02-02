#include <Windows.h>
#include "DataConverter.h"
#include "DataGeneratorCal4Met.h"
#include "DebugLog.h"
#include "ErrorDescription.h"
#include <string>

using namespace System::Runtime::InteropServices;
using namespace System::Xml;

namespace OTComm
{
	namespace Cal4Met
	{

#pragma managed

#pragma region Constructors and destructors
		DataGeneratorCal4Met::DataGeneratorCal4Met(String^ PortName, SerialBaudRate BaudRate) :
			DataGeneratorInterface(), mPortName(PortName), mBaudRate(BaudRate),
			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false), mDataReaderInterfaceHelper(NULL),
			mTestMode(false)
		{
			mConnectors = gcnew array<GenericConnector^>(GetNumberConnectorsAux());
			for (unsigned int ii = 0; ii < GetNumberConnectorsAux(); ii++)
				mConnectors[ii] = gcnew GenericConnector(1);
			mSources = gcnew array<DataGeneratorSingleSource^>(1);
			mSources[0] = gcnew DataGeneratorSingleSource(100, 2, GetChannelsMapping());
			mDeviceTotalChannels = 4;
		}

		DataGeneratorCal4Met::~DataGeneratorCal4Met()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorCal4Met();
		}

		DataGeneratorCal4Met::!DataGeneratorCal4Met()
		{

		}
#pragma endregion  

#pragma region XML files 
		String^ DataGeneratorCal4Met::GetDeviceXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", mDeviceTotalChannels.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}
			mXMLTextStream->WriteAttributeString("COMPORT", mPortName);
			mXMLTextStream->WriteAttributeString("Sensibility", mSensibility.ToString());
			mXMLTextStream->WriteAttributeString("MaximumLoad", mMaximumLoad.ToString());
			mXMLTextStream->WriteAttributeString("ad_bits", "16");
			mXMLTextStream->WriteStartElement("Channels");
			for (int nCon=0; nCon<mConnectors->Length; nCon++)
				if (mConnectors[nCon]->Adapter != nullptr)
					mConnectors[nCon]->Adapter->WriteXMLConfiguration(mXMLTextStream, nCon, nCon, 0);
			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		String^ DataGeneratorCal4Met::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}
			mXMLTextStream->WriteAttributeString("COMPORT", mPortName);
			mXMLTextStream->WriteStartElement("Channels");
			if (mConnectors[0]->Adapter != nullptr)
				mConnectors[0]->Adapter->WriteXMLConfiguration(mXMLTextStream, 1, 0, 0);
			mXMLTextStream->WriteEndElement();//"Channels"

			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		DataGeneratorCal4Met^ DataGeneratorCal4Met::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorCal4Met^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			String^ deviceName = DeviceNode->Attributes["Name"]->Value;
			unsigned int deviceFrequency = System::Convert::ToUInt16(DeviceNode->Attributes["SampleFrequency"]->Value);
			String^ comport = DeviceNode->Attributes["COMPORT"]->Value;
			double sensibility = System::Convert::ToDouble(DeviceNode->Attributes["Sensibility"]->Value);
			double maxload = System::Convert::ToDouble(DeviceNode->Attributes["MaximumLoad"]->Value);

			if (deviceName->Contains("CAL4MET"))
			{
				gdr = gcnew OTComm::Cal4Met::DataGeneratorCal4Met(comport, deviceFrequency);
				gdr->SetSensibility(sensibility);
				gdr->SetMaximumLoad(maxload);
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
							DebugLog::Print(ChannelNode->Attributes["ID"]->Value->ToString());
							DebugLog::Print(ChannelNode->Attributes["Muscle"]->Value->ToString());
							DebugLog::Print(ChannelNode->Attributes["Side"]->Value->ToString());
							DebugLog::Print(ChannelNode->Attributes["Index"]->Value->ToString());

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
					gdr->ConnectAux(adPosition, (AdapterAux^)ad);
				}
			}
			textReader->Close();
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

		AdapterAux^ DataGeneratorCal4Met::GetAdapterFromID(String^ adapterID)
		{
			std::string converted = toStandardString(adapterID);
			if (StringContains("Direct connection to Auxiliary Input", converted))
				return gcnew OTComm::Cal4Met::AdapterAux;
			return nullptr;
		}

		GenericChannel^ DataGeneratorCal4Met::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
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
			if (channelID == ("GR04MM1305"))
				return gcnew OTComm::Sensors::SensorGR04MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("GR08MM1305"))
				return gcnew OTComm::Sensors::SensorGR08MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
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

		unsigned int DataGeneratorCal4Met::GetNumberConnectorsAux()
		{
			return 4;
		}

		void DataGeneratorCal4Met::ConnectAux(const unsigned int port, AdapterAux^ adapter)
		{
			if (port >= GetNumberConnectorsAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			mConnectors[port]->Adapter = adapter;

		}

		void DataGeneratorCal4Met::ConnectIn(AdapterAux^ adapter)
		{
			mConnectors[0]->Adapter = adapter;			
		}

#pragma endregion  

#pragma region Device properties 

		String^ DataGeneratorCal4Met::GetDeviceDescription(void)
		{
			return "CAL4MET";
		}

		
		Bitmap^ DataGeneratorCal4Met::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("layout_Cal4Met.jpg", true);
		}

		double^ DataGeneratorCal4Met::GetMaximumLoad()
		{
			return mMaximumLoad;
		}

		double^ DataGeneratorCal4Met::GetSensibility()
		{
			return mSensibility;
		}

		void DataGeneratorCal4Met::SetMaximumLoad(double maximumLoad)
		{
			mMaximumLoad = maximumLoad;
		}

		void DataGeneratorCal4Met::SetSensibility(double sensibility)
		{
			mSensibility = sensibility;
		}

		String^ DataGeneratorCal4Met::GetComPort()
		{
			return mPortName;
		}

#pragma endregion  

#pragma region Public device operations

		void DataGeneratorCal4Met::OpenDevice(void)
		{
			LOG_ENTERING;
			mDeviceHandle = INVALID_HANDLE_VALUE;
			try
			{
				IntPtr ptrToNativeString = Marshal::StringToHGlobalUni(mPortName);
				mDeviceHandle = CreateFile((LPCWSTR)ptrToNativeString.ToPointer(), GENERIC_READ | GENERIC_WRITE,
					0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
				Marshal::FreeHGlobal(ptrToNativeString);
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				DCB PortDCB;
				// Initialize the DCBlength member. 
				PortDCB.DCBlength = sizeof(DCB);
				// Get the default port setting information.
				if (!GetCommState(mDeviceHandle, &PortDCB))
				{
					throw gcnew GetCommFailedException(__FUNCTION__);
				}
				PortDCB.ByteSize = 8;
				PortDCB.StopBits = ONESTOPBIT;
				PortDCB.Parity = NOPARITY;
				PortDCB.BaudRate = mBaudRate;
				// Configure the port according to the specifications of the DCB structure.
				if (!SetCommState(mDeviceHandle, &PortDCB))
				{
					throw gcnew SetCommFailedException(__FUNCTION__);
				}

				if (!SetCommMask(mDeviceHandle, EV_RXCHAR))
				{					
					throw gcnew SetCommFailedException(__FUNCTION__);
				}

				mDeviceIsOpen = true;
				DataGeneratorInterface::OpenDevice();
			}
			catch (Exception^ e)
			{
				OpenDeviceFailedException^odfe = gcnew OpenDeviceFailedException(__FUNCTION__, e);
				FireOnError(odfe);
				throw odfe;
			}
			finally
			{
				if(mDeviceHandle != INVALID_HANDLE_VALUE)
					CloseHandle(mDeviceHandle);
			}
			LOG_EXITING;
		}

		void DataGeneratorCal4Met::InitializeDevice()
		{
			LOG_ENTERING;
			try 
			{
				InternalStopSerialDataTransfer();
				DataGeneratorInterface::InitializeDevice();
			}
			catch (Exception ^e)
			{
				InitializeDeviceFailedException^idfe = gcnew InitializeDeviceFailedException(__FUNCTION__,e);
				FireOnError(idfe);
				throw idfe;
			}
			LOG_EXITING;
		}

		void DataGeneratorCal4Met::StartAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				byte buf[1] = { (byte)((mSources[0]->mBytesPerChannel == 2 ? 243 : 227) + (mTestMode ? 12 : 0)) }; // Decimal value to write to serial port

				OVERLAPPED *OverlappedStruc;
				OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
				if (OverlappedStruc == NULL)
				{
					throw gcnew OverlappedStructException(__FUNCTION__);
				}
				ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
				OverlappedStruc->Internal = STATUS_PENDING;
				SetLastError(0);
				BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, 1, NULL, OverlappedStruc);

				if (WriteFileRes == FALSE)
				{
					if (GetLastError() == ERROR_IO_PENDING)
					{
						DWORD numberOfBytesTransferred = 0;
						if (!GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
						{
							throw gcnew OverlappedCommunicationException(__FUNCTION__);
						}
						else
							Sleep(1);
					}
					else
					{
						throw gcnew OverlappedCommunicationException(__FUNCTION__);
					}
				}
				DataGeneratorInterface::StartAcquisition();
				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorCal4Met::OuternThread));
				mManagedAcquisitionThread->Start();
			}
			catch (Exception ^e)
			{
				StartAcquisitionFailedException^safe = gcnew StartAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}		
			LOG_EXITING;				
		}


		void DataGeneratorCal4Met::StopAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				DataGeneratorInterface::StopAcquisition();
				InternalStopSerialDataTransfer();
			}
			catch (Exception^e)
			{
				StopAcquisitionFailedException^safe = gcnew StopAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}

		void DataGeneratorCal4Met::CloseDevice(void)
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
				InternalStopSerialDataTransfer();
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

#pragma region Non public device operations
		void DataGeneratorCal4Met::InternalStopSerialDataTransfer(void)
		{
			LOG_ENTERING;
			OVERLAPPED *OverlappedStruc = NULL;
			try
			{
				char buf[1] = { 0 };
				ULONG numberOfBytesTransferred;				
				OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
				if (OverlappedStruc == NULL)
				{
					throw gcnew OverlappedStructException(__FUNCTION__);
				}
				ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
				OverlappedStruc->Internal = STATUS_PENDING;
				SetLastError(0);
				BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, 1, NULL, OverlappedStruc);
				if (WriteFileRes == FALSE)
				{
					if (GetLastError() == ERROR_IO_PENDING)
					{
						if (!GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
						{
							throw gcnew OverlappedCommunicationException(__FUNCTION__);
						}
						else
							Sleep(1);
					}
					else
					{
						throw gcnew OverlappedCommunicationException(__FUNCTION__);
					}
				}				
			}			
			catch(Exception^ e)
			{
				throw e;
			}
			finally
			{
				if (OverlappedStruc != NULL)
				{
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
				}
			}
			LOG_EXITING;
		}
		
		void DataGeneratorCal4Met::OuternThread()
		{
			LOG_ENTERING;
			try
			{
				DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attribute.
					0,				// Initial Stack Size,
					AsyncIoSerial,  // Thread Function
					mDataReaderInterfaceHelper, // Thread parameter
					0,             // Creation Flags
					NULL);        // Don't need the Thread Id.
				if (mAsyncIoThreadHandle == NULL)
				{
					throw gcnew ThreadCreationException(__FUNCTION__);
				}
				WaitForSingleObject(mAsyncIoThreadHandle, INFINITE);
			}
			catch (Exception ^e)
			{
				AcquisitionThreadException^ ate = gcnew AcquisitionThreadException(__FUNCTION__, e);
				FireOnError(ate);
			}
			LOG_EXITING;
		}
		

#pragma endregion		

#pragma region Unmanaged code
#pragma unmanaged

		ULONG _stdcall AsyncIoSerial(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;

			ULONG DecodedBufferSize = helper->mSource[0].mAcquiredChannels * NUM_DECODED_BUFFERS;
			//volatile LONG *RecordingSamples;
			unsigned short *DecodedBuffer;
			unsigned short *DecodedBufferPos;
			unsigned short *DecodedBufferEnd;
			UCHAR * AcquisitionBuffer;

			ULONG numberOfBytesTransferred;
			OVERLAPPED *OverlappedStruc;
			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));

			if (OverlappedStruc == NULL)
			{
				return -1;
			}
			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			OverlappedStruc->Internal = STATUS_PENDING;
			ULONG AcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels * sizeof(char)*helper->mSource[0].mBytesPerChannel*helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize * sizeof(__int16)*helper->mSource[0].mDataRegroupFactor;
			AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, AcquisitionBufferSizeInBytes);

			if (AcquisitionBuffer == NULL)
			{
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				//DebugLog::Print("Acquisition buffer is null");
				return -1;
			}
			else
				;//DebugLog::Print("Acquisition buffer is OK");



			DecodedBuffer = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
			if (DecodedBuffer == NULL)
			{
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
			WHILE(*mMustStopAcquisitionThread == false)
			{
				SetLastError(0);
				//try an asynchronous read
				BOOL ReadFileRes = ReadFile(helper->mDeviceHandle, AcquisitionBuffer,
					AcquisitionBufferSizeInBytes, &numberOfBytesTransferred, OverlappedStruc);
				if (ReadFileRes == FALSE)
				{
					if (GetLastError() == ERROR_IO_PENDING)
					{
						while (exitMainLoop == false && !GetOverlappedResult(helper->mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, false))
						{
							DWORD err = GetLastError();
							if (err != ERROR_IO_PENDING && err != ERROR_IO_INCOMPLETE)
							{
								//error->exit
								helper->mError("", err, __FUNCTION__);
								exitMainLoop = true;
							}
							else if (*mMustStopAcquisitionThread != false)
							{
								//exit
								exitMainLoop = true;
							}
							else
								Sleep(1);
						}
					}

					else
					{
						exitMainLoop = true;
					}
				}

				if (exitMainLoop)break;
				lettitot += numberOfBytesTransferred;
				unsigned short* DecodedBufferCurrentStart = DecodedBufferPos;
				if (helper->mSource[0].mBytesPerChannel == 1)
				{
					for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
					{
						//add control for buffer overflow
						*DecodedBufferPos = AcquisitionBuffer[ChannelMappings[channel]];
						DecodedBufferPos++;
					}
				}
				else
				{					
					for (unsigned int channel = 0; channel < AcquisitionBufferSizeInBytes; channel += 2)
					{
						*DecodedBufferPos = AcquisitionBuffer[channel + 1];
						*DecodedBufferPos <<= 8;
						*DecodedBufferPos += AcquisitionBuffer[channel];
						DecodedBufferPos++;
					}
				}

				if (*mMustStopAcquisitionThread) break;
				//////////////////////////////////////
				helper->mNewDataAvailable(0, DataSize::SixteenBits,DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels, 0,helper->mSource[0].mDataRegroupFactor,0);

				if (DecodedBufferPos >= DecodedBufferEnd)
					DecodedBufferPos = DecodedBuffer;
				if (*mMustStopAcquisitionThread) break;


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
#pragma endregion 
	}

}