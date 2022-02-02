//TODO:
//Allocare correttamente il vettoer Adapters per ogni strumento
//Verificare che venga creato correttamente il vettore degli indici dei canali collegati
//restituire errore se si modifica/inserisce un adattatore/canale di tipo diverso da quello previsto (4 multiple in ma chiesto 64 canali, errori su AUX)
//creare le classi con i vari adattatori

#include <Windows.h>
#include "DataConverter.h"
#include "DataGeneratorForza.h"
#include "DebugLog.h"
#include "ErrorDescription.h"
#include <string>

using namespace System::Runtime::InteropServices;
using namespace System::Xml;

namespace OTComm
{
	namespace Forza
	{

#pragma managed

#pragma region Constructors and destructors
		DataGeneratorForza::DataGeneratorForza(String^ PortName, SerialBaudRate BaudRate) :
			DataGeneratorInterface(), mPortName(PortName), mBaudRate(BaudRate),
			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false), mDataReaderInterfaceHelper(NULL),
			mTestMode(false)
		{
			try
			{
				mConnectors = gcnew array<GenericConnector^>(1);
				mConnectors[0] = gcnew GenericConnector(1);
				mSources = gcnew array<DataGeneratorSingleSource^>(1);
				mSources[0] = gcnew DataGeneratorSingleSource(100, 2, GetChannelsMapping());
				mDeviceTotalChannels = 1;
				//HighResolution = (false);
			}
			catch (Exception ^e)
			{
				ConstructorFailedException^ cfe = gcnew ConstructorFailedException(__FUNCTION__, e);
				throw cfe;
			}
		}

		DataGeneratorForza::~DataGeneratorForza()
		{
			try
			{
				if (mDeviceIsOpen)
				{
					CloseDevice();
				}
				this->!DataGeneratorForza();
			}
			catch (Exception^)
			{
			}

		}

		DataGeneratorForza::!DataGeneratorForza()
		{

		}



#pragma endregion

#pragma region XML files

		String^ DataGeneratorForza::GetDeviceXMLConfiguration()
		{
			OpenXMLConfigStream(false);

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", mDeviceTotalChannels.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			mXMLTextStream->WriteAttributeString("COMPORT", mPortName);
			mXMLTextStream->WriteAttributeString("Sensibility", mSensibility.ToString());
			mXMLTextStream->WriteAttributeString("MaximumLoad", mMaximumLoad.ToString());
			mXMLTextStream->WriteAttributeString("ad_bits", "16");
			mXMLTextStream->WriteStartElement("Channels");
			if (mConnectors[0]->Adapter != nullptr)
				mConnectors[0]->Adapter->WriteXMLConfiguration(mXMLTextStream, 0, 0, 0);
			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		String^ DataGeneratorForza::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(false);

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			mXMLTextStream->WriteAttributeString("Sensibility", mSensibility.ToString());
			mXMLTextStream->WriteAttributeString("MaximumLoad", mMaximumLoad.ToString());
			mXMLTextStream->WriteAttributeString("ad_bits", "16");
			mXMLTextStream->WriteAttributeString("SampleFrequency", GetSamplingFrequency().ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}			mXMLTextStream->WriteAttributeString("COMPORT", mPortName);
			mXMLTextStream->WriteStartElement("Channels");
			if (mConnectors[0]->Adapter != nullptr)
				mConnectors[0]->Adapter->WriteXMLConfiguration(mXMLTextStream, 1, 0, 0);
			mXMLTextStream->WriteEndElement();//"Channels"

			mXMLTextStream->WriteEndElement();//"Device"



			return CloseXMLConfigStream();
		}

		DataGeneratorForza^ DataGeneratorForza::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorForza^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try{
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				String^ comport = "";
				if (DeviceNode->Attributes["COMPORT"] != nullptr)
					comport = DeviceNode->Attributes["COMPORT"]->Value;
				double sensibility = System::Convert::ToDouble(DeviceNode->Attributes["Sensibility"]->Value);
				double maxload = System::Convert::ToDouble(DeviceNode->Attributes["MaximumLoad"]->Value);

				if (deviceName->Contains("FORZA"))
				{
					gdr = gcnew OTComm::Forza::DataGeneratorForza(comport, OTComm::Forza::SerialBaudRates::BR_230400);
					gdr->SetSensibility(sensibility);
					gdr->SetMaximumLoad(maxload);
					System::Xml::XmlNode^ ChannelsNode = DeviceNode->ChildNodes[0];
					for (int ii = 0; ii < ChannelsNode->ChildNodes->Count; ii++)
					{
						System::Xml::XmlNode^ AdapterNode = ChannelsNode->ChildNodes[ii];
						/*DebugLog::Print(AdapterNode->Attributes["ID"]->Value->ToString());
						DebugLog::Print(Assembly::GetExecutingAssembly()->ToString());*/
						AdapterAux^ ad = GetAdapterFromID(AdapterNode->Attributes["ID"]->Value->ToString());
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
							gdr->ConnectIn(ad);
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
		AdapterAux^ DataGeneratorForza::GetAdapterFromID(String^ adapterID)
		{
			std::string converted = toStandardString(adapterID);

			if (StringContains("Direct connection to Auxiliary Input", converted))
				return gcnew OTComm::Forza::AdapterAux;
			return nullptr;
		}

		GenericChannel^ DataGeneratorForza::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
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

		unsigned int DataGeneratorForza::GetNumberConnectorsIn()
		{
			return 1;//TODO
		}

		void DataGeneratorForza::ConnectIn(AdapterAux^ adapter)
		{
			mConnectors[0]->Adapter = adapter;
		}

		String^ DataGeneratorForza::GetDeviceDescription(void)
		{
			return "FORZA";
		}

#pragma endregion

#pragma region Device properties 

		Bitmap^ DataGeneratorForza::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("layout_Forza.jpg", true);
		}

		double^ DataGeneratorForza::GetMaximumLoad()
		{
			return mMaximumLoad;
		}

		double^ DataGeneratorForza::GetSensibility()
		{
			return mSensibility;
		}

		void DataGeneratorForza::SetMaximumLoad(double maximumLoad)
		{
			mMaximumLoad = maximumLoad;
		}

		void DataGeneratorForza::SetSensibility(double sensibility)
		{
			mSensibility = sensibility;
		}

		String^ DataGeneratorForza::GetComPort()
		{
			return mPortName;
		}

#pragma endregion

#pragma region Public device operations
		void DataGeneratorForza::OpenDevice(void)
		{
			LOG_ENTERING;
			IntPtr ptrToNativeString = Marshal::StringToHGlobalUni(mPortName);
			mDeviceHandle = CreateFile((LPCWSTR)ptrToNativeString.ToPointer(), GENERIC_READ | GENERIC_WRITE,
				0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
			Marshal::FreeHGlobal(ptrToNativeString);
			if (mDeviceHandle == INVALID_HANDLE_VALUE)
			{
				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
				FireOnError(ihe);
				throw ihe;
			}
			else
				DebugLog::Print("Forza opened");
			DCB PortDCB;
			// Initialize the DCBlength member. 
			PortDCB.DCBlength = sizeof(DCB);
			// Get the default port setting information.
			if (!GetCommState(mDeviceHandle, &PortDCB))
			{
				GetCommFailedException^ gcfe = gcnew GetCommFailedException(__FUNCTION__);
				FireOnError(gcfe);
				throw gcfe;
			}
			else
				DebugLog::Print("Forza: GetCommState OK");
			PortDCB.ByteSize = 8;
			PortDCB.StopBits = ONESTOPBIT;
			PortDCB.Parity = NOPARITY;
			PortDCB.BaudRate = mBaudRate;

			// Configure the port according to the specifications of the DCB structure.
			if (!SetCommState(mDeviceHandle, &PortDCB))
			{
				SetCommFailedException^ scfe = gcnew SetCommFailedException(__FUNCTION__);
				FireOnError(scfe);
				CloseHandle(mDeviceHandle);
				throw scfe;
			}

			if (!SetCommMask(mDeviceHandle, EV_RXCHAR))
			{
				SetCommFailedException^ scfe = gcnew SetCommFailedException(__FUNCTION__);
				FireOnError(scfe);
				CloseHandle(mDeviceHandle);
				throw scfe;
			}

			mCompletionPortHandle = CreateIoCompletionPort(mDeviceHandle, NULL, 1, 0);
			if (mCompletionPortHandle == NULL)
			{
				CompletionPortException^ cpe = gcnew CompletionPortException(__FUNCTION__);
				FireOnError(cpe);
				CloseHandle(mDeviceHandle);
				throw cpe;
			}
			mDeviceIsOpen = true;
			DataGeneratorInterface::OpenDevice();

			LOG_EXITING;
		}

		void DataGeneratorForza::InitializeDevice()
		{
			LOG_ENTERING;
			InternalStopSerialDataTransfer();
			DataGeneratorInterface::InitializeDevice();
			LOG_EXITING;
		}



		void DataGeneratorForza::StartAcquisition(void)
		{
			LOG_ENTERING;
			{
				char buf[1] = { 243 }; // Decimal value to write to serial port
				OVERLAPPED *OverlappedStruc;
				OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
				if (OverlappedStruc == NULL)
				{
					OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
					FireOnError(ose);
					throw ose;
				}

				ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
				OverlappedStruc->Internal = STATUS_PENDING;

				SetLastError(0);

				//write 128 to start transmission
				BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, 1, NULL, OverlappedStruc);
				DWORD numberOfBytesTransferred = 0;
				OVERLAPPED *completedOv;
				ULONG_PTR  key;
				if (GetQueuedCompletionStatus(mCompletionPortHandle, &numberOfBytesTransferred, &key, &completedOv, 1000) == 0)
				{
					StartAcquisitionFailedException^ safe = gcnew StartAcquisitionFailedException(__FUNCTION__);
					CloseHandle(OverlappedStruc->hEvent);
					FireOnError(safe);
					throw safe;
				}
				DataGeneratorInterface::StartAcquisition();
				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorForza::OuternThread));
				mManagedAcquisitionThread->Start();
			}
			LOG_EXITING;
		}

		void DataGeneratorForza::StopAcquisition(void)
		{
			LOG_ENTERING;
			DataGeneratorInterface::StopAcquisition();
			InternalStopSerialDataTransfer();
			LOG_EXITING;
		}

		void DataGeneratorForza::CloseDevice(void)
		{
			LOG_ENTERING;
			if (mDeviceHandle != INVALID_HANDLE_VALUE && mDeviceIsOpen)
			{
				CloseHandle(mDeviceHandle);
				mDeviceHandle = INVALID_HANDLE_VALUE;
			}
			if (mCompletionPortHandle != INVALID_HANDLE_VALUE && mDeviceIsOpen)
			{
				CloseHandle(mCompletionPortHandle);
				mCompletionPortHandle = INVALID_HANDLE_VALUE;
			}

			mDeviceIsOpen = false;
			DataGeneratorInterface::CloseDevice();
			LOG_EXITING;
		}

#pragma endregion

#pragma region Non public device operations

		void DataGeneratorForza::OuternThread()
		{
			LOG_ENTERING;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			try
			{
				mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);


				int* tmpData = new int[mSources->Length];

				mDataReaderInterfaceHelper->mCustomData = (LPARAM)tmpData;

				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoSerialEugenio,  // Thread Func
					mDataReaderInterfaceHelper, // Thread parameter
					0,             // Creation Flags
					NULL);        // Don't need the Thread Id.
				if (mAsyncIoThreadHandle == NULL)
				{
					ThreadCreationException^ tce = gcnew ThreadCreationException(__FUNCTION__);
					FireOnError(tce);
					throw tce;
				}
				WaitForSingleObject(mAsyncIoThreadHandle, INFINITE);
			}
			catch (Exception ^e)
			{
				AcquisitionThreadException^ ate = gcnew AcquisitionThreadException(__FUNCTION__, e);
				FireOnError(ate);
			}
			finally
			{

				if (mDataReaderInterfaceHelper->mCustomData != NULL)
				{
					int* tmp = (int*)mDataReaderInterfaceHelper->mCustomData;
					delete(tmp);
					mDataReaderInterfaceHelper->mCustomData = NULL;
				}
			}

			LOG_EXITING;
		}

		void DataGeneratorForza::InternalStopSerialDataTransfer(void)
		{
			LOG_ENTERING;
			int res = 0;
			char buf[1] = { 0 };
			ULONG numberOfBytesTransferred;
			OVERLAPPED *OverlappedStruc;
			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				FireOnError(ose);
				throw ose;
			}
			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			OverlappedStruc->Internal = STATUS_PENDING;
			SetLastError(0);
			OverlappedStruc->hEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("OTCOMMWriteEvent"));

			BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, 1, NULL, OverlappedStruc);
			if (WriteFileRes == FALSE)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					if (WaitForSingleObject(OverlappedStruc->hEvent, 1000) == WAIT_OBJECT_0)
					{
						if (::GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, FALSE) == FALSE)
							//if (!GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
						{
							StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__);
							CloseHandle(OverlappedStruc->hEvent);
							FireOnError(safe);
							throw safe;
						}
					}
				}
				else
				{
					StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__);
					CloseHandle(OverlappedStruc->hEvent);
					FireOnError(safe);
					throw safe;
				}
			}
			CloseHandle(OverlappedStruc->hEvent);
			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;
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
				//try an async read
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
					//for (unsigned int koffset = 0; koffset < helper->mDataRegroupFactor; koffset++)
					//{
					//	ULONG offset = koffset + helper->mDeviceTotalChannels;
					//	for (unsigned int channel = 0; channel < helper->mAcquiredChannels; channel++)
					//	{
					//		//add control for buffer overflow
					//		int cm = offset + ChannelMappings[channel] * 2;
					//		*DecodedBufferPos = AcquisitionBuffer[cm + 1];
					//		*DecodedBufferPos <<= 8;
					//		*DecodedBufferPos += AcquisitionBuffer[cm];
					//		DecodedBufferPos++;
					//	}
					//}
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
				helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);

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

		ULONG _stdcall AsyncIoSerialEugenio(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;
			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

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
			float start = GetTickCount() / 1000.0f;
			int Iteration = 0;

			OVERLAPPED *completedOv;
			ULONG_PTR   key;

			DCB PortDCB;
			// Initialize the DCBlength member. 
			PortDCB.DCBlength = sizeof(DCB);
			SetLastError(0);

			if (GetCommState(helper->mDeviceHandle, &PortDCB))
			{
				WHILE(*mMustStopAcquisitionThread == false)
				{
					SetLastError(0);
					//try an async read
					BOOL ReadFileRes = ReadFile(helper->mDeviceHandle, AcquisitionBuffer,
						AcquisitionBufferSizeInBytes, &numberOfBytesTransferred, OverlappedStruc);
					//try to dequeue a completion packet. loop until the operation is succesfull
					while (GetQueuedCompletionStatus(helper->mCompletionPortHandle, &numberOfBytesTransferred,
						&key, &completedOv, 1000) == 0)
					{
						if (*mMustStopAcquisitionThread == true)
						{
							exitMainLoop = true;
							break;
						}
						else
						{
							if (!SetCommState(helper->mDeviceHandle, &PortDCB))
							{
								exitMainLoop = true;
								break;
							}
						}
					}

					unsigned short* DecodedBufferCurrentStart = DecodedBufferPos;
					for (unsigned int channel = 0; channel < AcquisitionBufferSizeInBytes; channel += helper->mDeviceTotalChannels * 2)
					{
						for (unsigned int acqChannel = 0; acqChannel < helper->mSource[0].mAcquiredChannels; acqChannel++)
						{
							*DecodedBufferPos = AcquisitionBuffer[channel + 2 * (ChannelMappings[acqChannel]) + 1];
							*DecodedBufferPos <<= 8;
							*DecodedBufferPos += AcquisitionBuffer[channel + 2 * (ChannelMappings[acqChannel])];
							DecodedBufferPos++;
						}
					}
					Sleep(1);
					if (*mMustStopAcquisitionThread) break;
					//////////////////////////////////////
					helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);

					if (DecodedBufferPos >= DecodedBufferEnd)
						DecodedBufferPos = DecodedBuffer;
					if (*mMustStopAcquisitionThread) break;


				}//while
			}
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