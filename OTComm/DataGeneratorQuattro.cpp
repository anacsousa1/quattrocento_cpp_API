#include <Windows.h>
#include "DataConverter.h"
#include "DataGeneratorQuattro.h"
#include "DebugLog.h"
#include "ErrorDescription.h"
#include "AdapterQuattro.h"
#include <stdio.h>

using namespace System::Runtime::InteropServices;
using namespace System::Xml;


namespace OTComm
{
	namespace Quattro
	{

#pragma managed

#pragma region Constructors and destructors
		DataGeneratorQuattro::DataGeneratorQuattro(String^ PortName, SerialBaudRate BaudRate) :
			DataGeneratorInterface(), mPortName(PortName), mBaudRate(BaudRate),
			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false), mDataReaderInterfaceHelper(NULL),
			mTestMode(false)
		{
			mConnectors = gcnew array<GenericConnector^>(4);
			for (int ii = 0; ii < 4; ii++)
				mConnectors[ii] = gcnew GenericConnector(1);//TODO controllare se non sia meglio 4 connettori
			mSources = gcnew array<DataGeneratorSingleSource^>(1);
			mSources[0] = gcnew DataGeneratorSingleSource(1024, 2, GetChannelsMapping());
			mDeviceTotalChannels = 4;
		}

		DataGeneratorQuattro::~DataGeneratorQuattro()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorQuattro();
		}

		DataGeneratorQuattro::!DataGeneratorQuattro()
		{

		}

#pragma endregion

#pragma region XML files

		String^ DataGeneratorQuattro::GetDeviceXMLConfiguration()
		{
			OpenXMLConfigStream(false);

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", mDeviceTotalChannels.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			mXMLTextStream->WriteAttributeString("COMPORT", mPortName);
			mXMLTextStream->WriteStartElement("Channels");
			for (int ii = 0; ii < mConnectors->Length; ii++)
			{
				if (mConnectors[ii]->Adapter != nullptr) {

					if (mConnectors[ii]->Adapter->ID->Contains("Direct connection to Auxiliary Input"))
					{
						mConnectors[ii]->Adapter->AuxWriteSavingXMLConfiguration(
							mXMLTextStream,
							ii,
							ii,
							0,
							(float)mConnectors[ii]->Adapter->AdapterGain,
							(float)mConnectors[ii]->AuxScaleFactor,
							mConnectors[ii]->unitOfMeasurement); //Aux Scale Factor
					}
					else
						mConnectors[ii]->Adapter->WriteXMLConfiguration(mXMLTextStream, ii, ii, 0);

				}
			}

			mXMLTextStream->WriteEndElement();//"Channels"

			mXMLTextStream->WriteEndElement();//"Device"



			return CloseXMLConfigStream();
		}

		String^ DataGeneratorQuattro::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", mDeviceTotalChannels.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}			mXMLTextStream->WriteAttributeString("COMPORT", mPortName);
			mXMLTextStream->WriteAttributeString("ad_bits", "16");
			mXMLTextStream->WriteAttributeString("SampleFrequency", "1024");
			mXMLTextStream->WriteStartElement("Channels");
			for (int ii = 0; ii < mConnectors->Length; ii++)
			{
				if (mConnectors[ii]->Adapter != nullptr)
					mConnectors[ii]->Adapter->WriteXMLConfiguration(mXMLTextStream, ii + 1, ii, 0);
			}

			mXMLTextStream->WriteEndElement();//"Channels"

			mXMLTextStream->WriteEndElement();//"Device"



			return CloseXMLConfigStream();
		}

		DataGeneratorQuattro^ DataGeneratorQuattro::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorQuattro^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try {
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				String^ comport = DeviceNode->Attributes["COMPORT"]->Value;
				if (deviceName->Contains("QUATTRO"))
				{
					gdr = gcnew OTComm::Quattro::DataGeneratorQuattro(comport, OTComm::Quattro::SerialBaudRates::BR_230400);//BR_230400
					System::Xml::XmlNode^ ChannelsNode = DeviceNode->ChildNodes[0];
					for (int ii = 0; ii < ChannelsNode->ChildNodes->Count; ii++)
					{
						System::Xml::XmlNode^ AdapterNode = ChannelsNode->ChildNodes[ii];
						DebugLog::Print(AdapterNode->Attributes["ID"]->Value->ToString());
						//DebugLog::Print(Assembly::GetExecutingAssembly()->ToString());

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
						gdr->ConnectIn(adPosition, (AdapterQuattro^)ad);

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

		unsigned int DataGeneratorQuattro::GetNumberConnectorsIn()
		{
			return 4;//TODO
		}

		void DataGeneratorQuattro::ConnectIn(const unsigned int port, AdapterQuattro^ adapter)
		{
			if (port >= GetNumberConnectorsIn())
				throw gcnew InvalidConnectorNumberException(__FUNCTION__);
			mConnectors[port]->Adapter = adapter;
		}

		void DataGeneratorQuattro::ConnectIn(AdapterQuattro^ adapter)
		{
			mConnectors[0]->Adapter = adapter;
		}

		GenericAdapter^ DataGeneratorQuattro::GetAdapterFromID(String^ adapterID)
		{
			if (adapterID == ("AdapterQuattro"))
				return gcnew OTComm::Quattro::AdapterQuattro;
			if (adapterID == ("ADx5JC"))
				return gcnew OTComm::Quattro::AdapterADx5JC;
			if (adapterID == ("ADx5"))
				return gcnew OTComm::Quattro::AdapterADx5;
			if (adapterID == ("ADx5JCG"))
				return gcnew OTComm::Quattro::AdapterADx5JCG;
			if (adapterID == ("ADx5JB"))
				return gcnew OTComm::Quattro::AdapterADx5JB;
			if (adapterID == ("ADx5JBG"))
				return gcnew OTComm::Quattro::AdapterADx5JBG;
			if (adapterID == ("ADx5JS"))
				return gcnew OTComm::Quattro::AdapterADx5JS;
			if (adapterID == ("ADx5JSG"))
				return gcnew OTComm::Quattro::AdapterADx5JSG;
			if (adapterID == ("AD3x1SD5"))
				return gcnew OTComm::Quattro::AdapterAD3x1SD5;

			return nullptr;
		}

		GenericChannel^ DataGeneratorQuattro::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
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
			return nullptr;
		}

#pragma endregion

#pragma region Device properties 

		String^ DataGeneratorQuattro::GetDeviceDescription(void)
		{
			return "QUATTRO";
		}

		Bitmap^ DataGeneratorQuattro::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("4.jpg", true);
		}

		String^ DataGeneratorQuattro::GetComPort()
		{
			return mPortName;
		}

#pragma endregion

#pragma region Public device operations

		void DataGeneratorQuattro::OpenDevice(void)
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
				DebugLog::Print("Quattro opened");
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
				DebugLog::Print("Quattro: GetCommState OK");
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

		void DataGeneratorQuattro::InitializeDevice()
		{
			LOG_ENTERING;
			InternalStopSerialDataTransfer();
			DataGeneratorInterface::InitializeDevice();
			LOG_EXITING;
		}



		void DataGeneratorQuattro::StartAcquisition(void)
		{
			LOG_ENTERING;
			{
				char buf[1] = { (char)((mSources[0]->mBytesPerChannel == 1 ? 223 : 239) + (mTestMode ? 12 : 0) + mDeviceTotalChannels) }; // Decimal value to write to serial port
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
				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorQuattro::OuternThread));
				mManagedAcquisitionThread->Start();
			}
			LOG_EXITING;
		}

		void DataGeneratorQuattro::StopAcquisition(void)
		{
			LOG_ENTERING;
			DataGeneratorInterface::StopAcquisition();
			InternalStopSerialDataTransfer();
			LOG_EXITING;
		}

		void DataGeneratorQuattro::CloseDevice(void)
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

		void DataGeneratorQuattro::OuternThread()
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
					AsyncIoSerialTest,  // Thread Func
					mDataReaderInterfaceHelper, // Thread parameter
					0,             // Creation Flags
					NULL);        // Don't need the Thread Id.
				if (mAsyncIoThreadHandle == NULL)
				{
					ThreadCreationException^ tce = gcnew ThreadCreationException(__FUNCTION__);
					FireOnError(tce);
					throw tce;
				}
				SetThreadPriority(mAsyncIoThreadHandle, THREAD_PRIORITY_HIGHEST);
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

		void DataGeneratorQuattro::InternalStopSerialDataTransfer(void)
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
#pragma managed
		//ULONG _stdcall AsyncIoSerial(PVOID  ThreadParameter)
		//{
		//	const int NUM_DECODED_BUFFERS = 10;
		//	DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
		//	unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

		//	int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;

		//	ULONG DecodedBufferSize = helper->mSource[0].mAcquiredChannels * NUM_DECODED_BUFFERS;
		//	//volatile LONG *RecordingSamples;
		//	unsigned short *DecodedBuffer;
		//	unsigned short *DecodedBufferPos;
		//	unsigned short *DecodedBufferEnd;
		//	UCHAR * AcquisitionBuffer;

		//	ULONG numberOfBytesTransferred;
		//	OVERLAPPED *OverlappedStruc;
		//	OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));

		//	if (OverlappedStruc == NULL)
		//	{
		//		return -1;
		//	}
		//	ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
		//	OverlappedStruc->Internal = STATUS_PENDING;
		//	ULONG AcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels * sizeof(char)*helper->mSource[0].mBytesPerChannel*helper->mSource[0].mDataRegroupFactor;
		//	ULONG DecodedBufferSizeInBytes = DecodedBufferSize * sizeof(__int16)*helper->mSource[0].mDataRegroupFactor;
		//	AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, AcquisitionBufferSizeInBytes);

		//	if (AcquisitionBuffer == NULL)
		//	{
		//		LocalFree((void *)OverlappedStruc);
		//		OverlappedStruc = NULL;
		//		//DebugLog::Print("Acquisition buffer is null");
		//		return -1;
		//	}
		//	else
		//		;//DebugLog::Print("Acquisition buffer is OK");



		//	DecodedBuffer = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
		//	if (DecodedBuffer == NULL)
		//	{
		//		LocalFree((void *)OverlappedStruc);
		//		OverlappedStruc = NULL;
		//		LocalFree((void *)AcquisitionBuffer);
		//		AcquisitionBuffer = NULL;
		//		//DebugLog::Print("DecodedBuffer is null");
		//		return -1;
		//	}
		//	else
		//		;//DebugLog::Print("DecodedBuffer is OK");

		//	//points to the beginning of the decoded buffer
		//	DecodedBufferPos = DecodedBuffer;
		//	DecodedBufferEnd = DecodedBuffer + DecodedBufferSize;

		//	//this is the main acquisition loop
		//	bool exitMainLoop = false;
		//	DWORD lettitot = 0;
		//	float start = GetTickCount()/1000.0f;
		//	int Iteration = 0;

		//	HANDLE hCompletionPort = CreateIoCompletionPort(helper->mDeviceHandle, NULL, 1, 0);
		//	if (hCompletionPort == NULL)
		//	{
		//		return -1;
		//	}
		//	OVERLAPPED *completedOv;
		//	ULONG_PTR   key;
		//	WHILE(*mMustStopAcquisitionThread == false)
		//	{
		//		SetLastError(0);
		//		//try an async read
		//		BOOL ReadFileRes = ReadFile(helper->mDeviceHandle, AcquisitionBuffer,
		//			AcquisitionBufferSizeInBytes, &numberOfBytesTransferred, OverlappedStruc);


		//		if (GetQueuedCompletionStatus(hCompletionPort, &numberOfBytesTransferred,
		//			&key, &completedOv, INFINITE) == 0)
		//		{
		//			helper->mError("", GetLastError(), __FUNCTION__);
		//			exitMainLoop = true;
		//		}
		//		
		//		if (exitMainLoop)break;
		//		lettitot += numberOfBytesTransferred;
		//		Iteration++;
		//		float current = GetTickCount() / 1000.0f;
		//		printf("%d\t %f s\t %d B\t %f s\n", Iteration ,current, lettitot, (current -start));
		//		start = current;
		//		

		//		unsigned short* DecodedBufferCurrentStart = DecodedBufferPos;
		//		if (helper->mSource[0].mBytesPerChannel == 1)
		//		{
		//			for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
		//			{
		//				//add control for buffer overflow
		//				*DecodedBufferPos = AcquisitionBuffer[ChannelMappings[channel]];
		//				DecodedBufferPos++;
		//			}
		//		}
		//		else
		//		{					
		//			for (unsigned int channel = 0; channel < AcquisitionBufferSizeInBytes; channel += helper->mSource[0].mSourceTotalChannels *2)
		//			{
		//				for (unsigned int acqChannel = 0; acqChannel < helper->mSource[0].mAcquiredChannels; acqChannel++)
		//				{
		//					*DecodedBufferPos = AcquisitionBuffer[channel + 2*(ChannelMappings[acqChannel]) + 1];
		//					*DecodedBufferPos <<= 8;
		//					*DecodedBufferPos += AcquisitionBuffer[channel + 2 * (ChannelMappings[acqChannel])];
		//					DecodedBufferPos++;
		//				}
		//			}

		//		}
		//		Sleep(1);
		//		if (*mMustStopAcquisitionThread) break;
		//		//////////////////////////////////////
		//		helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels, 0,helper->mSource[0].mDataRegroupFactor,0);

		//		if (DecodedBufferPos >= DecodedBufferEnd)
		//			DecodedBufferPos = DecodedBuffer;
		//		if (*mMustStopAcquisitionThread) break;


		//	}//while

		//	CancelIo(helper->mDeviceHandle);


		//	// We are not recording any more
		//	//InterlockedExchange(RecordingSamples, 0);


		//	if (!*mMustStopAcquisitionThread)
		//	{
		//		// Signal that acquisition ended before time
		//		//GlobalErrorDispatcher("An error occurred",helper->mError);
		//	}

		//	if (AcquisitionBuffer)
		//	{
		//		LocalFree((void *)AcquisitionBuffer);
		//		AcquisitionBuffer = NULL;
		//	}

		//	/*if (OverlappedStruc)
		//	{
		//		LocalFree((void *)OverlappedStruc);
		//		OverlappedStruc = NULL;
		//	}*/

		//	if (DecodedBuffer)
		//	{
		//		LocalFree((void *)DecodedBuffer);
		//		DecodedBuffer = NULL;
		//	}
		//	helper->mStopAcquisition();
		//	return 0;
		//}

		//ULONG _stdcall AsyncIoSerialEugenio(PVOID  ThreadParameter)
		//{
		//	const int NUM_DECODED_BUFFERS = 10;
		//	DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
		//	int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;
		//	unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

		//	ULONG DecodedBufferSize = helper->mSource[0].mAcquiredChannels * NUM_DECODED_BUFFERS;
		//	//volatile LONG *RecordingSamples;
		//	unsigned short *DecodedBuffer;
		//	unsigned short *DecodedBufferPos;
		//	unsigned short *DecodedBufferEnd;
		//	UCHAR * AcquisitionBuffer;

		//	ULONG numberOfBytesTransferred;
		//	OVERLAPPED *OverlappedStruc;
		//	OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));

		//	if (OverlappedStruc == NULL)
		//	{
		//		return -1;
		//	}
		//	ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
		//	OverlappedStruc->Internal = STATUS_PENDING;
		//	ULONG AcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels * sizeof(char)*helper->mSource[0].mBytesPerChannel*helper->mSource[0].mDataRegroupFactor;
		//	ULONG DecodedBufferSizeInBytes = DecodedBufferSize * sizeof(__int16)*helper->mSource[0].mDataRegroupFactor;
		//	AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, AcquisitionBufferSizeInBytes);

		//	if (AcquisitionBuffer == NULL)
		//	{
		//		LocalFree((void *)OverlappedStruc);
		//		OverlappedStruc = NULL;
		//		//DebugLog::Print("Acquisition buffer is null");
		//		return -1;
		//	}
		//	else
		//		;//DebugLog::Print("Acquisition buffer is OK");



		//	DecodedBuffer = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
		//	if (DecodedBuffer == NULL)
		//	{
		//		LocalFree((void *)OverlappedStruc);
		//		OverlappedStruc = NULL;
		//		LocalFree((void *)AcquisitionBuffer);
		//		AcquisitionBuffer = NULL;
		//		//DebugLog::Print("DecodedBuffer is null");
		//		return -1;
		//	}
		//	else
		//		;//DebugLog::Print("DecodedBuffer is OK");

		//		 //points to the beginning of the decoded buffer
		//	DecodedBufferPos = DecodedBuffer;
		//	DecodedBufferEnd = DecodedBuffer + DecodedBufferSize;

		//	//this is the main acquisition loop
		//	bool exitMainLoop = false;
		//	DWORD lettitot = 0;
		//	float start = GetTickCount() / 1000.0f;
		//	int Iteration = 0;

		//	OVERLAPPED *completedOv;
		//	ULONG_PTR   key;

		//	DCB PortDCB;
		//	// Initialize the DCBlength member. 
		//	PortDCB.DCBlength = sizeof(DCB);
		//	SetLastError(0);

		//	if (GetCommState(helper->mDeviceHandle, &PortDCB))
		//	{
		//		WHILE(*mMustStopAcquisitionThread == false)
		//		{
		//			SetLastError(0);
		//			//try an async read
		//			BOOL ReadFileRes = ReadFile(helper->mDeviceHandle, AcquisitionBuffer,
		//				AcquisitionBufferSizeInBytes, &numberOfBytesTransferred, OverlappedStruc);
		//			//printf("Acqbuffer size %lu	number of bytes transferred %lu\n");
		//			while (GetQueuedCompletionStatus(helper->mCompletionPortHandle, &numberOfBytesTransferred,
		//				&key, &completedOv, 1000) == 0)
		//			{
		//				if (*mMustStopAcquisitionThread == true)
		//				{
		//					exitMainLoop = true;
		//					break;
		//				}
		//				else
		//				{
		//					if (!SetCommState(helper->mDeviceHandle, &PortDCB))
		//					{
		//						exitMainLoop = true;
		//						break;
		//					}
		//				}
		//			}
		//			
		//			unsigned short* DecodedBufferCurrentStart = DecodedBufferPos;
		//			for (unsigned int channel = 0; channel < AcquisitionBufferSizeInBytes; channel += helper->mDeviceTotalChannels * 2)
		//			{
		//				for (unsigned int acqChannel = 0; acqChannel < helper->mSource[0].mAcquiredChannels; acqChannel++)
		//				{
		//					*DecodedBufferPos = AcquisitionBuffer[channel + 2 * (ChannelMappings[acqChannel]) + 1];
		//					*DecodedBufferPos <<= 8;
		//					*DecodedBufferPos += AcquisitionBuffer[channel + 2 * (ChannelMappings[acqChannel])];
		//					DecodedBufferPos++;
		//				}
		//			}
		//			Sleep(1);
		//			if (*mMustStopAcquisitionThread) break;
		//			//////////////////////////////////////
		//			helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);

		//			if (DecodedBufferPos >= DecodedBufferEnd)
		//				DecodedBufferPos = DecodedBuffer;
		//			if (*mMustStopAcquisitionThread) break;
		//			

		//		}//while
		//	}
		//	CancelIo(helper->mDeviceHandle);
		//	if (!*mMustStopAcquisitionThread)
		//	{
		//		// Signal that acquisition ended before time
		//		//GlobalErrorDispatcher("An error occurred",helper->mError);
		//	}

		//	if (AcquisitionBuffer)
		//	{
		//		LocalFree((void *)AcquisitionBuffer);
		//		AcquisitionBuffer = NULL;
		//	}
		//	if (DecodedBuffer)
		//	{
		//		LocalFree((void *)DecodedBuffer);
		//		DecodedBuffer = NULL;
		//	}
		//	helper->mStopAcquisition();
		//	return 0;
		//}
		ULONG _stdcall AsyncIoSerialTest(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ASYNCH_IO = 10;

			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;
			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;


			SetLastError(0);
			ULONG numberOfBytesTransferred;
			HANDLE hCompletionPort = helper->mCompletionPortHandle; //CreateIoCompletionPort(helper->mDeviceHandle, NULL, 1, 0);
			if (hCompletionPort == NULL)
			{
				int i = GetLastError();
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

			ULONG SingleAcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels * sizeof(char)*helper->mSource[0].mBytesPerChannel*helper->mSource[0].mDataRegroupFactor;
			UCHAR * AcquisitionBuffer;
			AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, SingleAcquisitionBufferSizeInBytes*NUM_ASYNCH_IO);

			//int temp = (int) AcquisitionBufferSizeInBytes;
			//DebugLog::Print(String::Format("ACQUISITION BUFFER LENGHT {0}", temp.ToString()));
			//System::Console::WriteLine("ACQUISITION BUFFER LENGHT {0}", temp.ToString());

			if (AcquisitionBuffer == NULL)
			{
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				//DebugLog::Print("Acquisition buffer is null");
				return -1;
			}
			else
				;//DebugLog::Print("Acquisition buffer is OK");

			ULONG DecodedBufferSize = helper->mSource[0].mAcquiredChannels * NUM_DECODED_BUFFERS*helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize * sizeof(__int16);

			//volatile LONG *RecordingSamples;
			unsigned short *DecodedBuffer;
			unsigned short *DecodedBufferPos;
			unsigned short *DecodedBufferEnd;

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
				for (int i = 0; i < NUM_ASYNCH_IO; i++)
				{
					BOOL ReadFileRes = ReadFile(helper->mDeviceHandle,
						AcquisitionBuffer + (i * SingleAcquisitionBufferSizeInBytes),
						SingleAcquisitionBufferSizeInBytes, &numberOfBytesTransferred, &OverlappedStruc[i]);

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
							//LocalFree((void *)TempBuffer);
							//TempBuffer = NULL;
							return -1;
						}
					}
				}
				WHILE(*mMustStopAcquisitionThread == false)
				{
					SetLastError(0);
					OVERLAPPED *completedOv;
					ULONG_PTR   key;

					//dice che abbiamo ricevuto i dati
					if (GetQueuedCompletionStatus(hCompletionPort, &numberOfBytesTransferred,
						&key, &completedOv, INFINITE) == 0)
					{
						helper->mError("", GetLastError(), __FUNCTION__);
						exitMainLoop = true;
					}
					size_t ReceivedBufferIndex = completedOv - OverlappedStruc; //indicate which overlapped struct read the data

					if (ReceivedBufferIndex < NUM_ASYNCH_IO)
					{
						UCHAR* AcquisitionBufferCurrentReadStart =
							&AcquisitionBuffer[ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes];
					
					unsigned short* DecodedBufferCurrentStart = DecodedBufferPos;
					
					for (unsigned int channel = 0; channel < SingleAcquisitionBufferSizeInBytes; channel += helper->mDeviceTotalChannels * 2)
					{
						//converto dati in DecodedBuffers per essere mandati alla soundtrack
						for (unsigned int acqChannel = 0; acqChannel < helper->mSource[0].mAcquiredChannels; acqChannel++)
						{							
							*DecodedBufferPos = AcquisitionBufferCurrentReadStart[channel + 2 * (ChannelMappings[acqChannel]) + 1];
							*DecodedBufferPos <<= 8;
							*DecodedBufferPos += AcquisitionBufferCurrentReadStart[channel + 2 * (ChannelMappings[acqChannel])];
							DecodedBufferPos++;
						}
					}
					Sleep(1);
					if (*mMustStopAcquisitionThread) break;
					//mando dati alla parte c# con enw data available
					helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);

					if (DecodedBufferPos >= DecodedBufferEnd)
						DecodedBufferPos = DecodedBuffer;
					if (*mMustStopAcquisitionThread) break;
					
					//rilancia la lettura della overlapped struct 
					SetLastError(0);
					//ZeroMemory(AcquisitionBuffer + (ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes), SingleAcquisitionBufferSizeInBytes);
					BOOL ReadFileRes = ReadFile(helper->mDeviceHandle,
						AcquisitionBuffer + (ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes),
						SingleAcquisitionBufferSizeInBytes, NULL, completedOv);

						
						if (ReadFileRes == FALSE)
						{
							if (GetLastError() != ERROR_IO_PENDING)
							{
							}
						}

					}// da aggiustare chiusura della parentesi
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




		//test FOR QUATTRO
		volatile LONG ForceStopAsyncIoThread = 0;
		volatile LONG RecordedTimeMs;
		volatile int IdxCurrentBuffer = 0;
		volatile int CurrentAcqSize = 0;
		volatile int PosCurrentBuffer = 0;
		volatile LONG RecordingSamples = 0;
		volatile HANDLE RecordingFileHandle = INVALID_HANDLE_VALUE;
		unsigned char buffer[2] = { 0,0 };
		volatile WCHAR Serial[50];
		unsigned long length;
		struct CommunicationType
		{
			enum Type
			{
				dataBlock,
				error,
				feedback,
				socket
			};
		};
		typedef void(__stdcall *PFN_NEW_DATA_CALLBACK)(__int16 *buffer, enum CommunicationType::Type type, int error);
		UCHAR *AcquisitionBuffer = NULL;
		volatile LONG nBufferSize = 0;
		extern volatile LONG DeltaBufferSize = 0;
		__int16 *DecodedBuffers[10] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
		volatile LONG SizeAcqBuffer = 0;
		volatile LONG nSizeOfDecodedSample = sizeof(__int16);
		volatile LONG nSizeOfRawSample = sizeof(__int16);
		volatile LONG SizeRawBuffer = 0;
#define MIN(a,b) ((a)>(b)?(b):(a))
		volatile LONG nPhysicalChannels = 0;
		int Iter = 0;
		volatile LONG nSetupChannels = 0;
		volatile LONG ChannelMappingsReorder[4096];
		volatile LONG ChannelMappingsSetup[4096];
		volatile WCHAR RecordingPath;
		volatile double AbsoluteTime;
		ULONG _stdcall AsyncIoSerialBt(PVOID  ThreadParameter)
		{
			static const int ERROR_ACQUISITION_ENDED_UNEXPECTEDLY = 1;
			static const int ERROR_WRITE_FILE_ERROR = 2;
			static const int ERROR_DEVICE_NOT_REACHABLE = 3;
			const int NUM_DECODED_BUFFERS = 10;
			const int SamplingFrequency = 1024;

			const int NUM_ASYNCH_IO = 1;
			//static const int ERROR_ACQUISITION_ENDED_UNEXPECTEDLY = 1;
			//static const int ERROR_WRITE_FILE_ERROR = 2;

			HANDLE hSerial = INVALID_HANDLE_VALUE;

			HANDLE hCompletionPort = NULL;
			ULONG_PTR i;
			ULONG   index;
			ULONG   error;
			UCHAR	ch;

			//bool JustResetted;
			//OutputDebug("just entered");

			InterlockedExchange(&ForceStopAsyncIoThread, 0);
			InterlockedExchange(&RecordedTimeMs, 0);
			IdxCurrentBuffer = 0;
			CurrentAcqSize = 0;
			PosCurrentBuffer = 0;
			// We are not recording at the beginning
			InterlockedExchange(&RecordingSamples, 0);
			// Check if an old recording file was left pending
			if (RecordingFileHandle != INVALID_HANDLE_VALUE)
			{
				printf("RecordingFileHandle");
				CloseHandle(RecordingFileHandle);
				RecordingFileHandle = INVALID_HANDLE_VALUE;
			}

			PFN_NEW_DATA_CALLBACK InternalNewDataCallback = (PFN_NEW_DATA_CALLBACK)ThreadParameter;


			/////////////////OPEN SERIAL PORT HERe!////////////

			hSerial = CreateFile((LPCWSTR)Serial,
				GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_WRITE | FILE_SHARE_READ,
				NULL, // default security
				OPEN_EXISTING,
				0,
				NULL);

			OVERLAPPED overlapped_write_stop;
			memset(&overlapped_write_stop, 0, sizeof(overlapped_write_stop));
			overlapped_write_stop.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			buffer[0] = 0;
			//puts("Stopping acquisition...");	
			WriteFile(hSerial, &buffer, 1, NULL, &overlapped_write_stop);
			//wait for a result
			GetOverlappedResult(hSerial, &overlapped_write_stop, &length, true);
			/*if(!result)
			printf("error sendiong terminate to the serial device: %d", GetLastError());*/
			//else
			//	puts("Acquisition stopped.\n");
			//clear all the serial port buffers. In this way we discard data received but not read
			PurgeComm(hSerial, -1);

			CloseHandle(hSerial);


			//ASYNC
			hSerial = CreateFile((LPCWSTR)Serial,
				GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_WRITE | FILE_SHARE_READ,
				NULL, // default security
				OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED,
				NULL);

			if (hSerial == INVALID_HANDLE_VALUE) {
				CloseHandle(hSerial);
			}

			printf("Serial Port OPENED ");

			DCB PortDCB;
			// Initialize the DCBlength member. 
			PortDCB.DCBlength = sizeof(DCB);
			// Get the default port setting information.

			//dcbSerial.DCBlength=sizeof(dcbSerialParams);
			if (!GetCommState(hSerial, &PortDCB)) {
				//cout<<"Serial Port Do not Exist failed" <<endl;
				printf("Serial Port Do not Exist failed 2");
				CloseHandle(hSerial);
			}

			PortDCB.ByteSize = 8;
			PortDCB.StopBits = ONESTOPBIT;
			PortDCB.Parity = NOPARITY;
			PortDCB.BaudRate = 230400;

			//PortDCB.BaudRate = 115200;

			// Configure the port according to the specifications of the DCB structure.
			if (!SetCommState(hSerial, &PortDCB))
			{
				// Could not configure the serial port.
				printf("Serial Port Do not Exist failed 3");
				CloseHandle(hSerial);
			}

			// Retrieve the timeout parameters for all read and write operations on the port. 


			BOOL Result;					//Result of read/write operations
											//ASYNC
											//configuration of the serial port. NB: it works also without all this stuff
			COMMTIMEOUTS commtimeouts;
			GetCommTimeouts(hSerial, &commtimeouts);
			commtimeouts.ReadIntervalTimeout = 10000;
			commtimeouts.WriteTotalTimeoutConstant = 10000;
			commtimeouts.ReadTotalTimeoutConstant = 1000;
			commtimeouts.WriteTotalTimeoutConstant = 1000;
			commtimeouts.ReadTotalTimeoutMultiplier = 0;
			commtimeouts.WriteTotalTimeoutMultiplier = 0;
			Result = SetCommTimeouts(hSerial, &commtimeouts);

			//SetCommMask(hSerial, EV_RXCHAR | EV_TXEMPTY);

			if (!SetCommTimeouts(hSerial, &commtimeouts)) {
				OutputDebugStringA("SetCommTimeouts() failed ");
			}

			FreeBuffers();

			//InterlockedExchange(&nBufferSize,CHANNELBUFFERSIZE*(nSetupChannels));//PB

			AcquisitionBuffer = (UCHAR *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, NUM_ASYNCH_IO * (nBufferSize + DeltaBufferSize));

			if (AcquisitionBuffer == NULL) {
				//goto Error;
				printf("Acquisition buffer is null");
			}


			for (i = 0; i < NUM_DECODED_BUFFERS; i++)
			{
				DecodedBuffers[i] = (__int16 *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, SizeAcqBuffer * nSizeOfDecodedSample);
			}

			//////////////WRITE FREEING BUFFER of the DEVICe/////////////////

			OVERLAPPED overlapped_write;
			memset(&overlapped_write, 0, sizeof(overlapped_write));
			overlapped_write.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			OVERLAPPED overlapped_read;
			memset(&overlapped_read, 0, sizeof(overlapped_read));
			overlapped_read.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
			BOOL fRes;

			//clear all the serial port buffers. In this way we discard data received but not read
			PurgeComm(hSerial, -1);


			//first writing to the serial port. Stop and start!
			buffer[0] = 0; //stop whatever you are doing!
			buffer[1] = 243;
			//OutputDebug("Serial Port WRITE OK 4 channel");

			//		if (nSetupChannels ==1)
			//		{
			//			buffer[1] = 240; // Decimal value to write to serial port
			//			OutputDebug("Serial Port WRITE OK 1 channel");
			//		}
			//					else if (nSetupChannels ==2)
			//		{
			//			buffer[1] = 241; // Decimal value to write to serial port
			//			OutputDebug("Serial Port WRITE OK 2 channel");
			//		}
			//								else if (nSetupChannels ==3)
			//		{
			//			buffer[1] = 242; // Decimal value to write to serial port
			//			OutputDebug("Serial Port WRITE OK 3 channel");
			//		}
			//								else
			//		{
			//			buffer[1] = 243; // Decimal value to write to serial port
			//			OutputDebug("Serial Port WRITE OK 4 channel");
			//		}

			WriteFile(hSerial, &buffer, 2, NULL, &overlapped_write);
			//wait for the write operation to complete. This should be fast, and there is no need to do other things in the meantime
			fRes = GetOverlappedResult(hSerial, &overlapped_write, &length, true);


			//			for (i = 0; i < NUM_ASYNCH_IO; i++) {
			//	OutputDebug("READ FILE from Serial Port");	
			//	if (ReadFile(hSerial,
			//		AcquisitionBuffer+ (i * (nBufferSize + DeltaBufferSize)),
			//		(nBufferSize + DeltaBufferSize),
			//		&length,
			//		&overlapped_read)== 0) {
			//			error = GetLastError();

			//					//read a buffer
			//	//Result should always be 0 when passing an overlapped I/O structure

			//		//char TmpBuffer[256];
			//		//time_t rawtime;
			//		//time ( &rawtime );
			//		//sprintf_s ( TmpBuffer, " %d  %d - %s","READ FILE ", length, nBufferSize_SP, ctime (&rawtime));
			//		//OutputDebugStringA(TmpBuffer);

			//			if (error != ERROR_IO_PENDING) {
			//				//cout<<"FAILED TO Acquire from Serial Port" <<endl;
			//				OutputDebug("FAILED TO Acquire from Serial Port");				
			//				goto Error2;
			//			}
			//	
			//	}
			//}

			Result = CloseHandle(overlapped_write.hEvent);

			i = 0;



			//debug file
			//FILE* DebugFile =fopen("data.txt", "wb");
			//FILE* DebugFile2 =fopen("data2.txt", "wb");
			UCHAR* TmpAcquisitionBuffer = AcquisitionBuffer;
			int size = MIN((nBufferSize + DeltaBufferSize) / nSizeOfRawSample, SizeRawBuffer - CurrentAcqSize);
			int sizechan = size / nPhysicalChannels;
			int howmany = (nBufferSize + DeltaBufferSize);//sizechan*nSizeOfRawSample;//(nBufferSize + DeltaBufferSize);
			while (howmany != 0 && ForceStopAsyncIoThread == false)
			{
				//read a buffer
				Result = ReadFile(hSerial,
					TmpAcquisitionBuffer,
					howmany,
					&length,
					&overlapped_read);

				//Result should always be 0 when passing an overlapped I/O structure
				if (Result == 0 && GetLastError() == ERROR_IO_PENDING)//Readfile started
				{
					//loop until the buffer is full
					while ((Result = GetOverlappedResult(hSerial, &overlapped_read, &length, false)) == false)
					{
						if (GetLastError() != ERROR_IO_INCOMPLETE)
						{
							//goto Error;
						}
					}
				}
				howmany -= length;
				TmpAcquisitionBuffer += length;
			}
			WHILE(ForceStopAsyncIoThread == false) {

				char TmpBuffer[256];
				/*if(DebugFile)
				fwrite(AcquisitionBuffer,1,length,DebugFile);*/



				Iter++;

				ULONG     numberOfBytesTransferred;


				time_t rawtime;
				//time(&rawtime);

#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif


				__int16 *raw_buffer = (__int16 *)(AcquisitionBuffer + (i * (nBufferSize + DeltaBufferSize)));

				// TODO: USB2Board.cpp linea 116 (per feedback)
				__int16 *cur_buf = DecodedBuffers[IdxCurrentBuffer];
				//__int16 *cur_buf_swap = (__int16 *)(AcquisitionBuffer + (i * (nBufferSize + DeltaBufferSize)));

				//				sprintf_s ( TmpBuffer, " %s %d %d %d","After READ", size,sizechan, IdxCurrentBuffer);
				//OutputDebugStringA(TmpBuffer);

				for (int nsamp = 0; nsamp < sizechan; nsamp++)
				{
					int databufbase = PosCurrentBuffer + nsamp*nSetupChannels;
					int rawbufbase = nsamp*nPhysicalChannels;

					//			sprintf_s ( TmpBuffer, " %s %d %d %d %d","After READ", databufbase,rawbufbase,nsamp, nSetupChannels);
					//OutputDebugStringA(TmpBuffer);
					/*sprintf_s ( TmpBuffer, " %s %d %d %d %d","After READ", databufbase,rawbufbase,nsamp, nSetupChannels);
					OutputDebugStringA(TmpBuffer);*/

					for (int channel = 0; channel < nSetupChannels; channel++)
					{
						if (databufbase + channel < SizeAcqBuffer)
						{

							// EMG-USB, EMG-USB2, MEBA, SERIAL
							cur_buf[databufbase + channel] =
								raw_buffer[rawbufbase + ChannelMappingsReorder[ChannelMappingsSetup[channel]]];

							/*cur_buf[databufbase+channel]=
							raw_buffer[rawbufbase + ChannelMappingsReorder[ChannelMappingsSetup[channel]]];*/
						}
						else
							CloseHandle(hSerial);//goto Error2; // Should never happen
					}
				}

				//JustResetted=false;
				if (RecordingSamples)
				{
					//for (int channel=0; channel<SizeAcqBuffer; channel++)
					//{
					//	
					//		
					//			// SERIAL
					//			cur_buf_swap[channel]=
					//				_byteswap_ushort (cur_buf [channel]);

					//			/*cur_buf[databufbase+channel]=
					//				raw_buffer[rawbufbase + ChannelMappingsReorder[ChannelMappingsSetup[channel]]];*/
					//
					//}

					{
						// Write data to recording file, opening it if it is not open
						if (RecordingFileHandle == INVALID_HANDLE_VALUE)
						{
							// Xsens
							LARGE_INTEGER Clock_Frequency, Clock_Final_Time;
							QueryPerformanceFrequency(&Clock_Frequency);
							QueryPerformanceCounter(&Clock_Final_Time);
							double abs_time = (double)(1 << 22) / Clock_Frequency.LowPart;
							abs_time = abs_time * (double)(Clock_Final_Time.HighPart*(1 << 10));
							abs_time = abs_time + (double)(Clock_Final_Time.LowPart) / (double)(Clock_Frequency.LowPart);
							AbsoluteTime = abs_time;

							RecordingFileHandle = CreateFile(
								(LPCWSTR)RecordingPath,
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								OPEN_ALWAYS,
								0,
								NULL
							);
							if (RecordingFileHandle != INVALID_HANDLE_VALUE)
								SetFilePointer(RecordingFileHandle, 0, NULL, FILE_END);
						}
						if (RecordingFileHandle != INVALID_HANDLE_VALUE && !ForceStopAsyncIoThread)
						{

							DWORD NumberOfBytesWritten;
							if (!WriteFile(
								RecordingFileHandle,
								((unsigned char *)cur_buf) + PosCurrentBuffer * nSizeOfDecodedSample,
								sizechan * nSetupChannels * nSizeOfDecodedSample,
								&NumberOfBytesWritten,
								NULL))
							{
								InternalNewDataCallback(NULL, CommunicationType::error, ERROR_WRITE_FILE_ERROR);
							}
							LARGE_INTEGER li;
							if (GetFileSizeEx(RecordingFileHandle, &li))
							{
								InterlockedExchange(&RecordedTimeMs, (long)(li.QuadPart * 500 / nSetupChannels / SamplingFrequency));
							}
						}
					}
				}
				else
				{

					if (RecordingFileHandle != INVALID_HANDLE_VALUE)
					{
						// Close file after recording is complete
						CloseHandle(RecordingFileHandle);
						RecordingFileHandle = INVALID_HANDLE_VALUE;
					}
				}

				//SizeAcqBuffer � lo spazio occupato in 1 secondo
				//io voglio controllare lo spazio allocato ogni 1/125 s, quindi ogni ottavo di SizeAcqBuffer
				//if(PosCurrentBuffer(SizeAcqBuffer/8)%==0)
				if (ForceStopAsyncIoThread)
				{
					/*fclose(DebugFile);
					fclose(DebugFile2);*/
					//stop the acquisition
					//buffer[0]=0;
					////puts("Stopping acquisition...");	
					//WriteFile(hSerial, &buffer, 1, NULL, &overlapped_write);
					////wait for a result
					//Result=GetOverlappedResult( hSerial,&overlapped_write,&length,true);						
					//if(!Result)
					//	printf("error sendiong terminate to the serial device: %d", GetLastError());
					////else
					////	puts("Acquisition stopped.\n");
					////clear all the serial port buffers. In this way we discard data received but not read
					//PurgeComm(hSerial,-1);

					//CloseHandle(hSerial);

					break;
				}
				////////////////////// BLZ Feedback

				if (PosCurrentBuffer % (SizeAcqBuffer / 8) == 0)
				{

					if (!ForceStopAsyncIoThread)
					{
						InternalNewDataCallback(&cur_buf[PosCurrentBuffer], CommunicationType::feedback, PosCurrentBuffer);
					}

				}

				if (!ForceStopAsyncIoThread)
				{
					InternalNewDataCallback(&cur_buf[PosCurrentBuffer], CommunicationType::socket, sizechan*nSetupChannels);
				}

				PosCurrentBuffer += sizechan*nSetupChannels;
				CurrentAcqSize += size;

				if (PosCurrentBuffer >= SizeAcqBuffer)// Buffer full
				{


					IdxCurrentBuffer++;
					if (IdxCurrentBuffer >= NUM_DECODED_BUFFERS)
						IdxCurrentBuffer = 0;
					CurrentAcqSize = 0;
					PosCurrentBuffer = 0;
					// Callback to managed C++ code (same thread)
					if (ForceStopAsyncIoThread)
					{


						break;
					}

					if (!ForceStopAsyncIoThread)
					{


						//for (int channel=0; channel<SizeAcqBuffer; channel++)
						//	{
						//							
						//				// SERIAL
						//				cur_buf[channel]=
						//					_byteswap_ushort (cur_buf [channel]);

						//				/*cur_buf[databufbase+channel]=
						//					raw_buffer[rawbufbase + ChannelMappingsReorder[ChannelMappingsSetup[channel]]];*/
						//	
						//	}

						InternalNewDataCallback(cur_buf, CommunicationType::dataBlock, SizeAcqBuffer);

						/*sprintf_s ( TmpBuffer, " %s %d %d","Send Data2 ", cur_buf, SizeAcqBuffer);
						OutputDebugStringA(TmpBuffer);*/
					}

				}

				////////////////////

				//wprintf(L"Number of bytes read by request number %d is %d\n",
				//                    i, numberOfBytesTransferred);

				if (ForceStopAsyncIoThread)
				{

					break;
				}
				TmpAcquisitionBuffer = AcquisitionBuffer;
				howmany = (nBufferSize + DeltaBufferSize);//sizechan*nSizeOfRawSample;//(nBufferSize + DeltaBufferSize);
				while (howmany != 0 && !ForceStopAsyncIoThread)
				{
					//read a buffer
					Result = ReadFile(hSerial,
						TmpAcquisitionBuffer,
						howmany,
						&length,
						&overlapped_read);

					//Result should always be 0 when passing an overlapped I/O structure
					if (Result == 0 && GetLastError() == ERROR_IO_PENDING)//Readfile started
					{
						//loop until the buffer is full
						while ((Result = GetOverlappedResult(hSerial, &overlapped_read, &length, false)) == false)
						{
							if (GetLastError() != ERROR_IO_INCOMPLETE)
							{
								//goto Error;
							}
						}
					}
					howmany -= length;
					TmpAcquisitionBuffer += length;
				}


				CloseHandle(hSerial);

				printf("Freeing buffers");
				FreeBuffers();

				// We are not recording any more
				InterlockedExchange(&RecordingSamples, 0);

				if (RecordingFileHandle != INVALID_HANDLE_VALUE)
				{
					CloseHandle(RecordingFileHandle);
					RecordingFileHandle = INVALID_HANDLE_VALUE;
				}

				if (!ForceStopAsyncIoThread)
				{
					// Signal that acquisition ended before time
					InternalNewDataCallback(NULL, CommunicationType::error, ERROR_ACQUISITION_ENDED_UNEXPECTEDLY);
				}

				/////////////////OPEN SERIAL PORT HERe!////////////

				hSerial = CreateFile((LPCWSTR)Serial,
					GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_WRITE | FILE_SHARE_READ,
					NULL, // default security
					OPEN_EXISTING,
					0,
					NULL);

				OVERLAPPED overlapped_write_end;
				memset(&overlapped_write_end, 0, sizeof(overlapped_write_end));
				overlapped_write_end.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

				buffer[0] = 0;
				//puts("Stopping acquisition...");	
				WriteFile(hSerial, &buffer, 1, NULL, &overlapped_write_end);
				//wait for a result
				GetOverlappedResult(hSerial, &overlapped_write_end, &length, true);
				/*if(!result)
				printf("error sendiong terminate to the serial device: %d", GetLastError());*/
				//else
				//	puts("Acquisition stopped.\n");
				//clear all the serial port buffers. In this way we discard data received but not read
				PurgeComm(hSerial, -1);

				CloseHandle(hSerial);

				return 0;
			}
		}
		void FreeBuffers()
		{
			int i;

			if (AcquisitionBuffer)
			{
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
			}



			for (i = 0; i<10; i++)
			{
				if (DecodedBuffers[i])
				{
					LocalFree((void *)DecodedBuffers[i]);
					DecodedBuffers[i] = NULL;
				}
			}
		}

#pragma endregion

	}

}