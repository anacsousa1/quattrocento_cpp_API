#include "DataGeneratorFormeter.h"
#include "stdio.h"

using namespace System::Xml;
namespace OTComm {
	namespace Formeter {
#pragma comment(lib, "Ws2_32.lib")

#define SLEEPSMALL Sleep(10)
#define SLEEPLARGE Sleep(100)

#pragma managed

		//const char CommandStartAcq = 0b00001010;
		const unsigned char CommandStopAcq = 0b00001001;		//dec: 9
		const unsigned char CommandStopData = 0;// 0b00000100;		//dec: 4
		//const unsigned char CommandStatReq = 0b00001011;		//dec: 11
		//const unsigned char CommandSetDataTime = 0b00001100;    //dec: 12
		const unsigned char CommandFirmVer = 0b00001000;        //dec: 8
		const unsigned char CommandBatteryLevel = 0b00001110;        //dec: 14
		//const unsigned char CommandAcqAutoStop = 0b00001101;    //dec: 13
		const unsigned char CommandSetDeviceMode = 0b11100011;  //dec: 
		const unsigned char CommandSetDeviceModeDemo = 0b11101111;  //dec: 

		const unsigned char CommandSetDeviceMode2Byte = 0b11110011;  //dec: 
		const unsigned char CommandSetDeviceModeDemo2Byte = 0b11111111;  //dec: 255

		const GUID g_guidServiceClass = { 0x00001101, 0x0000, 0x1000,{ 0x80, 0x00, 0x00, 0x80, 0x5f, 0x9b, 0x34, 0xfb } };


		bool DataGeneratorForemg::IsFormeterAvailable(String^ RemoteName,bool ForceFlush)
		{
			SOCKADDR_BTH RemoteBtAddr;
			return FindFormeterDevice(RemoteName,RemoteBtAddr, ForceFlush,false)==CXN_SUCCESS;
		}


		int DataGeneratorForemg::FindFormeterDevice(String^ RemoteName, SOCKADDR_BTH& RemoteBtAddr, bool ForceFlush, bool TestConnection)
		{			
			INT             iResult = CXN_SUCCESS;
			BOOL            bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
			ULONG           ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
			HANDLE          hLookup = NULL;
			PWSAQUERYSET    pWSAQuerySet = NULL;

			wchar_t RemoteNameC[1024];
			if (String::IsNullOrWhiteSpace(RemoteName))
				RemoteName = "foremg";
			int RemoteNameLength = RemoteName->Length;
			for (int c = 0; c < RemoteNameLength; c++)
				RemoteNameC[c] = RemoteName[c];
			RemoteNameC[RemoteNameLength] = 0;

			WSADATA wsaData = { 0 };		


			ZeroMemory(&RemoteBtAddr, sizeof(RemoteBtAddr));

			if (CXN_SUCCESS != WSAStartup(MAKEWORD(2, 2), &wsaData))
			{
				LOG_ERROR("WSA failed to initialize");
				throw gcnew WSAException(__FUNCTION__, "WSA failed to initialize");
			}

			pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,ulPQSSize);
			if (NULL == pWSAQuerySet) 
			{
				LOG_ERROR("Unable to allocate memory for WSAQUERYSET");
				LOG_EXITING;
				throw gcnew WSAException(__FUNCTION__, "Unable to allocate memory for WSAQUERYSET");
			}

			//
			// Search for the device with the correct name
			//
			if (CXN_SUCCESS == iResult) 
			{
					
				//
				// WSALookupService is used for both service search and device inquiry
				// LUP_CONTAINERS is the flag which signals that we're doing a device inquiry.
				//
				ulFlags = LUP_CONTAINERS | LUP_RETURN_NAME | LUP_RETURN_ADDR;
				if (ForceFlush)
					ulFlags |= LUP_FLUSHCACHE;

		
				//
				// Start the lookup service
				//
				iResult = CXN_ERROR_NOTFOUND;
				hLookup = 0;
				bContinueLookup = TRUE;
				ZeroMemory(pWSAQuerySet, ulPQSSize);
				pWSAQuerySet->dwNameSpace = NS_BTH;
				pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
				 WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);			

				while (bContinueLookup) 
				{							
					if (NO_ERROR == WSALookupServiceNext(hLookup,ulFlags,&ulPQSSize,pWSAQuerySet)) 
					{

						//
						// Compare the name to see if this is the device we are looking for.
						//
						if (pWSAQuerySet->lpszServiceInstanceName != NULL)
						{

						if (0 == _wcsnicmp(pWSAQuerySet->lpszServiceInstanceName, 
							(LPCTSTR)RemoteNameC, RemoteNameLength))
						{
							//
								// Found a remote bluetooth device with matching name.
								// Get the address of the device and exit the lookup.
								//
							CopyMemory(&RemoteBtAddr,
								(PSOCKADDR_BTH)pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr,
								sizeof(RemoteBtAddr));
							if (TestConnection)
							{
								SOCKET s = INVALID_SOCKET;
								RemoteBtAddr.addressFamily = AF_BTH;
								RemoteBtAddr.serviceClassId = g_guidServiceClass;
								RemoteBtAddr.port = -1;
								// Open a bluetooth socket using RFCOMM protocol
								s = WSASocket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM, NULL, 0, WSA_FLAG_OVERLAPPED);
								if (INVALID_SOCKET == s)
								{
									iResult = CXN_ERROR_NOTFOUND;
								}
								else if (SOCKET_ERROR == WSAConnect(s, (struct sockaddr *) &RemoteBtAddr, sizeof(SOCKADDR_BTH), NULL, NULL, NULL, NULL))
								{
									iResult = CXN_ERROR_NOTFOUND;
								}
								else
								{
									bRemoteDeviceFound = TRUE;
									bContinueLookup = FALSE;
									iResult = CXN_SUCCESS;
								}
								closesocket(s);
								SLEEPSMALL;
							}
							else
							{
								bRemoteDeviceFound = TRUE;
								bContinueLookup = FALSE;
								iResult = CXN_SUCCESS;
							}

							
						}
						}
					}
					else 
					{
						iResult = WSAGetLastError();
						if (WSA_E_NO_MORE == iResult) // No more devices found.  Exit the lookup.
						{ 							
							bContinueLookup = FALSE;
						}
						else if (WSAEFAULT == iResult) 
						{
							//
							// The buffer for QUERYSET was insufficient.
							// In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
							// the required size.  So we can use this parameter to reallocate memory for QUERYSET.
							//
							HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
							pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
								HEAP_ZERO_MEMORY,
								ulPQSSize);
							if (NULL == pWSAQuerySet) {
								iResult = STATUS_NO_MEMORY;
								bContinueLookup = FALSE;
							}
						}
						else 
						{
							bContinueLookup = FALSE;
						}
					}
				}
				WSALookupServiceEnd(hLookup);// End the lookup service
			}

			if (NULL != pWSAQuerySet) 
			{
				HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
				pWSAQuerySet = NULL;
			}			
			return iResult;
		}




		List<String^>^ DataGeneratorForemg::ListPairedDevices(String^ RemoteName, bool ForceFlush)
		{

			INT             iResult = CXN_SUCCESS;
			BOOL            bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
			ULONG           ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
			HANDLE          hLookup = NULL;
			PWSAQUERYSET    pWSAQuerySet = NULL;
			List<String^>^ Result = gcnew List<String^>;
			wchar_t RemoteNameC[1024];
			/*if (String::IsNullOrWhiteSpace(RemoteName))
				RemoteName = "foremg";*/
			int RemoteNameLength = RemoteName->Length;
			for (int c = 0; c < RemoteNameLength; c++)
				RemoteNameC[c] = RemoteName[c];
			RemoteNameC[RemoteNameLength] = 0;
			
			WSADATA wsaData = { 0 };
			LOG_ENTERING;
				
			if (CXN_SUCCESS != WSAStartup(MAKEWORD(2, 2), &wsaData))
			{
				LOG_ERROR("WSA failed to initialize");
				throw gcnew WSAException(__FUNCTION__, "WSA failed to initialize");
			}

			pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulPQSSize);
			if (NULL == pWSAQuerySet)
			{
				LOG_ERROR("Unable to allocate memory for WSAQUERYSET");
				LOG_EXITING;
				throw gcnew WSAException(__FUNCTION__, "Unable to allocate memory for WSAQUERYSET");
			}

			//
			// Search for the device with the correct name
			//
			if (CXN_SUCCESS == iResult)
			{

				//
				// WSALookupService is used for both service search and device inquiry
				// LUP_CONTAINERS is the flag which signals that we're doing a device inquiry.
				//
				ulFlags = LUP_CONTAINERS | LUP_RETURN_NAME | LUP_RETURN_ADDR;
				if (ForceFlush)
					ulFlags |= LUP_FLUSHCACHE;


				//
				// Start the lookup service
				//
				iResult = CXN_ERROR_NOTFOUND;
				hLookup = 0;
				bContinueLookup = TRUE;
				ZeroMemory(pWSAQuerySet, ulPQSSize);
				pWSAQuerySet->dwNameSpace = NS_BTH;
				pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
				WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);

				while (bContinueLookup)
				{
					if (NO_ERROR == WSALookupServiceNext(hLookup, ulFlags, &ulPQSSize, pWSAQuerySet))
					{

						//
						// Compare the name to see if this is the device we are looking for.
						//
						if (pWSAQuerySet->lpszServiceInstanceName != NULL)
						{

							if (0 == _wcsnicmp(pWSAQuerySet->lpszServiceInstanceName, (LPCTSTR)RemoteNameC, RemoteNameLength))
							{
								Result->Add(gcnew String(pWSAQuerySet->lpszServiceInstanceName));
							}
						}
					}
					else
					{
						iResult = WSAGetLastError();
						if (WSA_E_NO_MORE == iResult) // No more devices found.  Exit the lookup.
						{
							bContinueLookup = FALSE;
						}
						else if (WSAEFAULT == iResult)
						{
							//
							// The buffer for QUERYSET was insufficient.
							// In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
							// the required size.  So we can use this parameter to reallocate memory for QUERYSET.
							//
							HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
							pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
								HEAP_ZERO_MEMORY,
								ulPQSSize);
							if (NULL == pWSAQuerySet) {								
								bContinueLookup = FALSE;
							}
						}
						else
						{
							bContinueLookup = FALSE;
						}
					}
				}
				WSALookupServiceEnd(hLookup);// End the lookup service
			}

			if (NULL != pWSAQuerySet)
			{
				HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
				pWSAQuerySet = NULL;
			}
			
			LOG_EXITING;
			Result->Sort();
			return Result;
		}


		DataGeneratorForemg::DataGeneratorForemg() :
			DataGeneratorInterface(),mDemoMode(false),
			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false),
			mDataReaderInterfaceHelper(NULL)
		{

			mConnectors = gcnew array<GenericConnector^>(1);
			AdapterFormeter^ adapter = gcnew AdapterStandard();
			mConnectors[0] = gcnew GenericConnector(adapter->MaxNumChannels);// mDeviceTotalChannels
			mConnectors[0]->Adapter = adapter;
			mDeviceTotalChannels = GetNumberTotalChannels();

			mSources = gcnew array<DataGeneratorSingleSource^>(1);
			mSources[0] = gcnew DataGeneratorSingleSource(800, 1, GetChannelsMapping());

			mDeviceHandle = (HANDLE)INVALID_SOCKET;
			mModel = Formeter::Models::FormeterStandard;
		}

		void DataGeneratorForemg::SetDemoMode()
		{
			mDemoMode = true;
		}

		Bitmap^ DataGeneratorForemg::Image::get()
		{
			try
			{
				return Utilities::GetImageFromEmbeddedResource("Default.jpg", true);
			}
			catch(Exception^)
			{
				return nullptr;
			}
		}

		DataGeneratorForemg^ DataGeneratorForemg::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorForemg^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try{
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				unsigned int deviceModel = System::Convert::ToUInt16(DeviceNode->Attributes["Model"]->Value);

				if (deviceName->Contains("FORMETER"))
				{
					System::Xml::XmlNode^ ChannelsNode = DeviceNode->ChildNodes[0];
					for (int ii = 0; ii < ChannelsNode->ChildNodes->Count; ii++)
					{					
						gdr = gcnew  OTComm::Formeter::DataGeneratorForemg();
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

		AdapterFormeter^ DataGeneratorForemg::GetAdapterFromID(String^ adapterID)
		{			
			return gcnew AdapterStandard;
		}

		GenericChannel^ DataGeneratorForemg::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
		{
			return nullptr;
		}

		unsigned int DataGeneratorForemg::GetDeviceModel()
		{
			return (unsigned int)mModel;//TODO
		}

		unsigned int DataGeneratorForemg::GetNumberTotalChannels()
		{
			return mModel % 10000;
		}

		unsigned int DataGeneratorForemg::GetNumberNonAuxChannels()
		{
			return 0;
		}

		unsigned int DataGeneratorForemg::GetNumberAuxChannels()
		{
			return 0;
		}

		array<unsigned int>^ DataGeneratorForemg::CreateChannelsMapping()
		{
			return gcnew array<unsigned int>(4)
			{
				0, 1, 2, 3
			};
		}

		String^ DataGeneratorForemg::GetDeviceDescription(void)
		{
			String^ res = gcnew String("TRENTADUE");			
			return res;
		}
			
		void DataGeneratorForemg::OpenDevice(String ^RemoteName, Model DeviceModel)
		{
			LOG_ENTERING;
			try
			{
				SOCKADDR_BTH RemoteBtAddr;
				int FormeterFound = FindFormeterDevice(RemoteName, RemoteBtAddr, false, false);
				if (FormeterFound == CXN_SUCCESS)
				{
					mDeviceHandle = (HANDLE)INVALID_SOCKET;
					RemoteBtAddr.addressFamily = AF_BTH;
					RemoteBtAddr.serviceClassId = g_guidServiceClass;
					RemoteBtAddr.port = -1;
					// Open a bluetooth socket using RFCOMM protocol
					mDeviceHandle = (HANDLE)WSASocket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM, NULL, 0, WSA_FLAG_OVERLAPPED);
					if (INVALID_SOCKET == (SOCKET)mDeviceHandle)
					{
						throw gcnew WSAException(__FUNCTION__, "WSASocket_INVALID_SOCKET");
					}

					//
					// Connect the socket (pSocket) to a given remote socket represented by address (pServerAddr)
					//
					if (SOCKET_ERROR == WSAConnect((SOCKET)mDeviceHandle, (struct sockaddr *) &RemoteBtAddr, sizeof(SOCKADDR_BTH), NULL, NULL, NULL, NULL))
					{
						throw gcnew WSAException(__FUNCTION__, "WSAConnect_CXN_ERROR_CANNOTCONNECT");
					}
				}
				else
				{
					throw gcnew DeviceNotFoundException(__FUNCTION__, "Device not found");
				}

				mDeviceIsOpen = true;


				if (DeviceModel == Models::FormeterAutoDetect)
				{
					String^ Firmware=String::Empty;
					if (GetFirmwareVersion(Firmware) <= 0)
					{
						mModel = Models::FormeterStandard;
						mSources[0]->mSamplingFrequency = 800;
					}
					else if (Firmware->StartsWith("quattroti"))
					{
						mModel = Models::FormeterStandard16Bit;
						mSources[0]->mSamplingFrequency = 1000;
					}
					else
					{
						mModel = Models::FormeterStandard;
						mSources[0]->mSamplingFrequency = 800;
					}
				}

				mSources[0]->mBytesPerChannel = (mModel == Models::FormeterStandard) ? 1 : 2;

				mDeviceTotalChannels = GetNumberTotalChannels();
				DataGeneratorInterface::OpenDevice();
				StopAcquisition();
			}
			catch (Exception^e)
			{
				OpenDeviceFailedException^ idfe = gcnew OpenDeviceFailedException(__FUNCTION__, e);
				FireOnError(idfe);
				throw e;
			}
			LOG_EXITING;
			
		}

		void DataGeneratorForemg::InitializeDevice()
		{
			LOG_ENTERING;
			try
			{
				DataGeneratorInterface::InitializeDevice();
			}
			catch (Exception^e)
			{
				InitializeDeviceFailedException^ idfe = gcnew InitializeDeviceFailedException(__FUNCTION__, e);
				FireOnError(idfe);
				throw e;
			}
			LOG_EXITING;				
		}

		void DataGeneratorForemg::OuternThread()
		{
			LOG_ENTERING;
			try
			{
				mDeviceTotalChannels = GetNumberTotalChannels();
				DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				mDataReaderInterfaceHelper->mAuxData = 0;
				mDataReaderInterfaceHelper->mCustomData = mDemoMode;
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoFormeter,  // Thread Func
					mDataReaderInterfaceHelper, // Thread parameter
					0,             // Creation Flags
					NULL);        // Don't need the Thread Id.
				if (mAsyncIoThreadHandle == NULL)
				{
					ThreadCreationException^ tce = gcnew ThreadCreationException(__FUNCTION__);
					FireOnError(tce);
				}
				else
					WaitForSingleObject(mAsyncIoThreadHandle, INFINITE);
			}
			catch (Exception ^e)
			{
				AcquisitionThreadException^ ate = gcnew AcquisitionThreadException(__FUNCTION__, e);
				FireOnError(ate);
			}

			LOG_EXITING;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <exception cref="StartAcquisitionFailedException"></exception>
		void DataGeneratorForemg::StartAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				DataGeneratorInterface::StartAcquisition();
				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorForemg::OuternThread));
				mManagedAcquisitionThread->Start();
			}
			catch (Exception^e)
			{
				StartAcquisitionFailedException^ safe = gcnew StartAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}


		void DataGeneratorForemg::StopAcquisition(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			{
				throw gcnew InvalidHandleException(__FUNCTION__);
			}
			DataGeneratorInterface::StopAcquisition();
			InternalStopFormeterDataTransfer();
			LOG_EXITING;
		}

		void DataGeneratorForemg::CloseDevice(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			{
				throw gcnew InvalidHandleException(__FUNCTION__);
			}
			
			InternalStopFormeterDataTransfer();
			closesocket((SOCKET)mDeviceHandle);
			mDeviceHandle = (HANDLE)INVALID_SOCKET;
			mDeviceIsOpen = false;
			LOG_SUCCESS;
			
			DataGeneratorInterface::CloseDevice();
			LOG_EXITING;			
		}

		DataGeneratorForemg::~DataGeneratorForemg()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorForemg();
		}

		DataGeneratorForemg::!DataGeneratorForemg()
		{

		}

		void DataGeneratorForemg::InternalStopFormeterDataTransfer(void)
		{
			LOG_ENTERING;
			int tmpResult = 0;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				
				char echo[2048];
				
				SLEEPLARGE;
				if (!WriteByte(CommandStopData))
				{

					CloseConnection();
					FireOnError("failed stop acquisition", GetLastError(), __FUNCTION__);
					LOG_ERROR("failed stop acquisition");
					tmpResult = -1;
				}
				else
				{
					SLEEPLARGE;
					int IterMax = 1000;
					DWORD numberOfBytesTransferred;
					while (IterMax > 0)
					{
						IterMax--;
						numberOfBytesTransferred = 0;
						ReadFile(mDeviceHandle, echo, sizeof(echo) / sizeof(echo[0]),
							&numberOfBytesTransferred, NULL);
						if (numberOfBytesTransferred == 0)
							break;

					}

					if (IterMax <= 0)
					{
						LOG_ERROR("failed stop acquisition");						
						DeviceIoControlException^ dioce = gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_STOP_ACQUISITION");
						FireOnError(dioce);
						throw dioce;						
					}
				}
			}
			LOG_EXITING;
		}

		void DataGeneratorForemg::ConnectIn(AdapterFormeter^ adapter)
		{
			mConnectors[0]->Adapter = adapter;			
		}

		int DataGeneratorForemg::ReadDeviceID(UCHAR* dev_id)
		{
			//TODO completare??????????????
			if (mDeviceHandle == INVALID_HANDLE_VALUE)
				return -5;
			return 0;
		}

		void DataGeneratorForemg::CloseConnection()
		{
			DebugLog::Print(String::Format("Connection closed"));
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				closesocket((SOCKET)mDeviceHandle);
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
			}
		}

		int DataGeneratorForemg::WriteByte(char Command)
		{
			if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			{
				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
				FireOnError(ihe);
				throw ihe;
			}
			
			DWORD numberOfBytesTransferred;
			OVERLAPPED *OverlappedStruc;
			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				FireOnError(ose);
				throw ose;
			}
			//OverlappedStruc->Internal = STATUS_PENDING;
			SetLastError(0);
			BOOL WriteFileRes = WriteFile(mDeviceHandle, &Command, 1, &numberOfBytesTransferred, OverlappedStruc);
			SLEEPSMALL;
			if (WriteFileRes == FALSE)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					if (!GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
					{
						throw gcnew OverlappedCommunicationException(__FUNCTION__);
						numberOfBytesTransferred = 0;
					}
					else
					{
						//wait between read/write operations
					}
				}
				else
				{
					numberOfBytesTransferred = 0;
					throw gcnew OverlappedCommunicationException(__FUNCTION__);
					
				}
			}
			else
			{
				SLEEPSMALL;
			}
			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;
			return numberOfBytesTransferred;
		}

		int DataGeneratorForemg::ReadBytes(char *ByteArray, int maxlen)
		{
			if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			{
				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
				FireOnError(ihe);
				throw ihe;
			}

			DWORD numberOfBytesTransferred;
			OVERLAPPED *OverlappedStruc;
			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				FireOnError(ose);
				throw ose;
			}
			OverlappedStruc->Internal = STATUS_PENDING;
			SetLastError(0);
			SLEEPSMALL;
			if (FALSE== ReadFile(mDeviceHandle, ByteArray, maxlen, &numberOfBytesTransferred, OverlappedStruc))
			{
				if (ERROR_IO_PENDING==GetLastError())
				{
					if (WAIT_OBJECT_0 != WaitForSingleObject(OverlappedStruc->hEvent, 1000) ||
						!::GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
					{
						CloseHandle(OverlappedStruc->hEvent);
						FireOnError("problem reading", GetLastError(), __FUNCTION__);
						LOG_ERROR("problem reading");
						LocalFree((void *)OverlappedStruc);
						OverlappedStruc = NULL;
						return -1;
					}					
				}
				else
				{
					FireOnError("problem reading", GetLastError(), __FUNCTION__);
					LOG_ERROR("problem reading");
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					return -1;
				}
			}
			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;			
			return numberOfBytesTransferred;
		}
				
		int DataGeneratorForemg::GetFirmwareVersion(String^% version)
		{
			//TODO controllare che non stia acquisendo
			LOG_ENTERING;
			int readbytes = -1;
			if (WriteByte(CommandFirmVer) > 0)
			{
				SLEEPLARGE;
				char buffer[128];
				readbytes = ReadBytes(buffer,128);
				if(readbytes>=0)
				version= gcnew String(buffer, 0, readbytes);
				else
					version = gcnew String("");
			}
			LOG_EXITING;
			return readbytes;
		}

		int DataGeneratorForemg::GetBatteryLevel(String^% batterylevel)
		{

			//controllare che non stia acquisendo
			LOG_ENTERING;
			int readbytes = -1;
			if (WriteByte(CommandBatteryLevel) > 0)
			{
				SLEEPLARGE;
				char buffer[128];
				readbytes = ReadBytes(buffer, 128);
				if(readbytes>=0)
					batterylevel = gcnew String(buffer, 0, readbytes);
				else
					batterylevel = gcnew String("");
			}
			LOG_EXITING;
				return readbytes;
		}


		array<unsigned int>^ DataGeneratorForemg::GetAcquiredChannels()
		{
			return mAcquiredChannelsIndices;
		}


		String^ DataGeneratorForemg::GetDeviceXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			//if (GetAcquiredChannels() == nullptr) return"";
			String^ Firmware;
			if (GetFirmwareVersion(Firmware) <= 0)
				Firmware = "Unknown";

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", GetNumberTotalChannels().ToString());// (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}
			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteStartElement("Channels");
			unsigned int ChannelNum = 0;
			int startIndex = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{
				if (mConnectors[k]->Adapter != nullptr)
				{
					mConnectors[k]->Adapter->WriteXMLConfiguration(mXMLTextStream, k, startIndex, 0);

					for (int zz = 0; zz < mConnectors[k]->Adapter->Channels->Length; zz++)
						if (mConnectors[k]->Adapter->Channels[zz] != nullptr)
							startIndex++;
				}

				//ChannelNum += mConnectors[k]->NumConnectableChannels;
			}
			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		String^ DataGeneratorForemg::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			//if (GetAcquiredChannels() == nullptr) return"";
			String^ Firmware;
			if (GetFirmwareVersion(Firmware) <= 0)
				Firmware = "Unknown";

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}
			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteStartElement("Channels");
			unsigned int ChannelNum = 0;
			int startIndex = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{

				if (mConnectors[k]->Adapter != nullptr)
				{
					mConnectors[k]->Adapter->WriteSavingXMLConfiguration(mXMLTextStream, k, startIndex, 0);

					for (int zz = 0; zz < mConnectors[k]->Adapter->Channels->Length; zz++)
						if (mConnectors[k]->Adapter->Channels[zz] != nullptr)
							startIndex++;
				}

				//ChannelNum += mConnectors[k]->NumConnectableChannels;
			}
			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		String^ DataGeneratorForemg::UpdateFirmware(String^ filenamepath)
		{
			// control if device connected to pc

			// import hex file


			return "Update failed";
		}


		//
		// NameToBthAddr converts a bluetooth device name to a bluetooth address,
		// if required by performing inquiry with remote name requests.
		// This function demonstrates device inquiry, with optional LUP flags.
		//
		
		

		//ULONG DataGeneratorFormeter::CreateSocketForDevice()
		//{

		//	INT             iResult = CXN_SUCCESS;
		//	BOOL            bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
		//	ULONG           ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
		//	HANDLE          hLookup = NULL;
		//	PWSAQUERYSET    pWSAQuerySet = NULL;
		//	const LPWSTR pszRemoteName = L"bruxoff";
		//	SOCKADDR_BTH RemoteBtAddr;
		//	ZeroMemory(&RemoteBtAddr, sizeof(RemoteBtAddr));
		//	pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
		//		HEAP_ZERO_MEMORY,
		//		ulPQSSize);
		//	if (NULL == pWSAQuerySet) {
		//		iResult = STATUS_NO_MEMORY;
		//		wprintf(L"!ERROR! | Unable to allocate memory for WSAQUERYSET\n");
		//	}
		//	
		//	if (CXN_SUCCESS == iResult) {

		//		for (INT iRetryCount = 0;
		//			!bRemoteDeviceFound && (iRetryCount < CXN_MAX_INQUIRY_RETRY);
		//			iRetryCount++) {
		//			ulFlags = LUP_CONTAINERS | LUP_RETURN_NAME | LUP_RETURN_ADDR;

		//			if (0 == iRetryCount) {
		//				wprintf(L"*INFO* | Inquiring device from cache...\n");
		//			}
		//			else {						
		//				ulFlags |= LUP_FLUSHCACHE;
		//				wprintf(L"*INFO* | Unable to find device.  Waiting for %d seconds before re-inquiry...\n", CXN_DELAY_NEXT_INQUIRY);
		//				Sleep(CXN_DELAY_NEXT_INQUIRY * 1000);

		//				wprintf(L"*INFO* | Inquiring device ...\n");
		//			}

		//			//
		//			// Start the lookup service
		//			//
		//			iResult = CXN_SUCCESS;
		//			hLookup = 0;
		//			bContinueLookup = FALSE;
		//			ZeroMemory(pWSAQuerySet, ulPQSSize);
		//			pWSAQuerySet->dwNameSpace = NS_BTH;
		//			pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
		//			iResult = WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);

		//			//
		//			// Even if we have an error, we want to continue until we
		//			// reach the CXN_MAX_INQUIRY_RETRY
		//			//
		//			if ((NO_ERROR == iResult) && (NULL != hLookup)) {
		//				bContinueLookup = TRUE;
		//			}
		//			else if (0 < iRetryCount) {
		//				wprintf(L"=CRITICAL= | WSALookupServiceBegin() failed with error code %d, WSAGetLastError = %d\n", iResult, WSAGetLastError());
		//				break;
		//			}

		//			while (bContinueLookup) {
		//				//
		//				// Get information about next bluetooth device
		//				//
		//				if (NO_ERROR == WSALookupServiceNext(hLookup,
		//					ulFlags,
		//					&ulPQSSize,
		//					pWSAQuerySet)) {

		//					//
		//					// Compare the name to see if this is the device we are looking for.
		//					//
		//					if ((pWSAQuerySet->lpszServiceInstanceName != NULL) &&
		//						(CXN_SUCCESS == _wcsicmp(pWSAQuerySet->lpszServiceInstanceName, pszRemoteName))) {
		//						//
		//						// Found a remote bluetooth device with matching name.
		//						// Get the address of the device and exit the lookup.
		//						//								
		//						bRemoteDeviceFound = TRUE;
		//						bContinueLookup = FALSE;
		//					}
		//				}
		//				else {
		//					iResult = WSAGetLastError();
		//					if (WSA_E_NO_MORE == iResult) { //No more data
		//													//
		//													// No more devices found.  Exit the lookup.
		//													//
		//						bContinueLookup = FALSE;
		//					}
		//					else if (WSAEFAULT == iResult) {
		//						//
		//						// The buffer for QUERYSET was insufficient.
		//						// In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
		//						// the required size.  So we can use this parameter to reallocate memory for QUERYSET.
		//						//
		//						HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
		//						pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
		//							HEAP_ZERO_MEMORY,
		//							ulPQSSize);
		//						if (NULL == pWSAQuerySet) {
		//							iResult = STATUS_NO_MEMORY;
		//							bContinueLookup = FALSE;
		//						}
		//					}
		//					else {
		//						bContinueLookup = FALSE;
		//					}
		//				}
		//			}
		//			//
		//			// End the lookup service
		//			//
		//			WSALookupServiceEnd(hLookup);

		//			if (STATUS_NO_MEMORY == iResult) {
		//				break;
		//			}
		//		}
		//	}
		//	if (NULL != pWSAQuerySet) {
		//		HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
		//		pWSAQuerySet = NULL;
		//	}
		//	if (bRemoteDeviceFound) {
		//		iResult = CXN_SUCCESS;
		//	}
		//	else {
		//		iResult = CXN_ERROR_NOTFOUND;
		//	}			
		//	return iResult;
		//}

		ULONG DataGeneratorForemg::CreateSocketForDevice()
		{

			INT             iResult = CXN_SUCCESS;
			BOOL            bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
			ULONG           ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
			HANDLE          hLookup = NULL;
			PWSAQUERYSET    pWSAQuerySet = NULL;
			const LPWSTR pszRemoteName = L"bruxoff";			

			SOCKADDR_BTH RemoteBtAddr;

			ZeroMemory(&RemoteBtAddr, sizeof(RemoteBtAddr));

			pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				ulPQSSize);
			if (NULL == pWSAQuerySet) {
				iResult = STATUS_NO_MEMORY;
				wprintf(L"!ERROR! | Unable to allocate memory for WSAQUERYSET\n");
			}

			//
			// Search for the device with the correct name
			//
			if (CXN_SUCCESS == iResult) {

				for (INT iRetryCount = 0;
					!bRemoteDeviceFound && (iRetryCount < CXN_MAX_INQUIRY_RETRY);
					iRetryCount++) {
					//
					// WSALookupService is used for both service search and device inquiry
					// LUP_CONTAINERS is the flag which signals that we're doing a device inquiry.
					//
					ulFlags = LUP_CONTAINERS;

					//
					// Friendly device name (if available) will be returned in lpszServiceInstanceName
					//
					ulFlags |= LUP_RETURN_NAME;

					//
					// BTH_ADDR will be returned in lpcsaBuffer member of WSAQUERYSET
					//
					ulFlags |= LUP_RETURN_ADDR;

					if (0 == iRetryCount) {
						wprintf(L"*INFO* | Inquiring device from cache...\n");
					}
					else {
						//
						// Flush the device cache for all inquiries, except for the first inquiry
						//
						// By setting LUP_FLUSHCACHE flag, we're asking the lookup service to do
						// a fresh lookup instead of pulling the information from device cache.
						//
						ulFlags |= LUP_FLUSHCACHE;

						//
						// Pause for some time before all the inquiries after the first inquiry
						//
						// Remote Name requests will arrive after device inquiry has
						// completed.  Without a window to receive IN_RANGE notifications,
						// we don't have a direct mechanism to determine when remote
						// name requests have completed.
						//
						wprintf(L"*INFO* | Unable to find device.  Waiting for %d seconds before re-inquiry...\n", CXN_DELAY_NEXT_INQUIRY);
						Sleep(CXN_DELAY_NEXT_INQUIRY * 1000);

						wprintf(L"*INFO* | Inquiring device ...\n");
					}

					//
					// Start the lookup service
					//
					iResult = CXN_SUCCESS;
					hLookup = 0;
					bContinueLookup = FALSE;
					ZeroMemory(pWSAQuerySet, ulPQSSize);
					pWSAQuerySet->dwNameSpace = NS_BTH;
					pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
					iResult = WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);

					//
					// Even if we have an error, we want to continue until we
					// reach the CXN_MAX_INQUIRY_RETRY
					//
					if ((NO_ERROR == iResult) && (NULL != hLookup)) {
						bContinueLookup = TRUE;
					}
					else if (0 < iRetryCount) {
						wprintf(L"=CRITICAL= | WSALookupServiceBegin() failed with error code %d, WSAGetLastError = %d\n", iResult, WSAGetLastError());
						break;
					}

					while (bContinueLookup) {
						//
						// Get information about next bluetooth device
						//
						// Note you may pass the same WSAQUERYSET from LookupBegin
						// as long as you don't need to modify any of the pointer
						// members of the structure, etc.
						//
						// ZeroMemory(pWSAQuerySet, ulPQSSize);
						// pWSAQuerySet->dwNameSpace = NS_BTH;
						// pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);
						if (NO_ERROR == WSALookupServiceNext(hLookup,
							ulFlags,
							&ulPQSSize,
							pWSAQuerySet)) {

							//
							// Compare the name to see if this is the device we are looking for.
							//
							if ((pWSAQuerySet->lpszServiceInstanceName != NULL) &&
								(CXN_SUCCESS == _wcsicmp(pWSAQuerySet->lpszServiceInstanceName, pszRemoteName))) {
								//
								// Found a remote bluetooth device with matching name.
								// Get the address of the device and exit the lookup.
								//
								CopyMemory(&RemoteBtAddr,
									(PSOCKADDR_BTH)pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr,
									sizeof(RemoteBtAddr));
								bRemoteDeviceFound = TRUE;
								bContinueLookup = FALSE;
							}
						}
						else {
							iResult = WSAGetLastError();
							if (WSA_E_NO_MORE == iResult) { //No more data
															//
															// No more devices found.  Exit the lookup.
															//
								bContinueLookup = FALSE;
							}
							else if (WSAEFAULT == iResult) {
								//
								// The buffer for QUERYSET was insufficient.
								// In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
								// the required size.  So we can use this parameter to reallocate memory for QUERYSET.
								//
								HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
								pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(),
									HEAP_ZERO_MEMORY,
									ulPQSSize);
								if (NULL == pWSAQuerySet) {
									iResult = STATUS_NO_MEMORY;
									bContinueLookup = FALSE;
								}
							}
							else {
								bContinueLookup = FALSE;
							}
						}
					}
					//
					// End the lookup service
					//
					WSALookupServiceEnd(hLookup);

					if (STATUS_NO_MEMORY == iResult) {
						break;
					}
				}
			}

			if (NULL != pWSAQuerySet) {
				HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
				pWSAQuerySet = NULL;
			}

			if (bRemoteDeviceFound) {
				iResult = CXN_SUCCESS;
			}
			else {
				iResult = CXN_ERROR_NOTFOUND;
			}
			if (iResult == CXN_SUCCESS)
			{
				//SOCKET          LocalSocket = INVALID_SOCKET;
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
				SOCKADDR_BTH    SockAddrBthServer = RemoteBtAddr;
				SockAddrBthServer.addressFamily = AF_BTH;
				SockAddrBthServer.serviceClassId = g_guidServiceClass;
				SockAddrBthServer.port = -1;

				//
				// Open a bluetooth socket using RFCOMM protocol
				//
				mDeviceHandle = (HANDLE)WSASocket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM,NULL,0, WSA_FLAG_OVERLAPPED);
				if (INVALID_SOCKET == (SOCKET)mDeviceHandle)
				{
					iResult = CXN_ERROR_SOCKETNOTCREATED;
				}

				//
				// Connect the socket (pSocket) to a given remote socket represented by address (pServerAddr)
				//
				if (SOCKET_ERROR == WSAConnect((SOCKET)mDeviceHandle,
					(struct sockaddr *) &SockAddrBthServer,
					sizeof(SOCKADDR_BTH), NULL, NULL, NULL, NULL)) 
				{
					iResult = CXN_ERROR_CANNOTCONNECT;
				}
			}
			return iResult;
		}

#pragma unmanaged
		ULONG _stdcall AsyncIoFormeter(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ASYNCH_IO = 10; //TODO controllare come mai non è 10
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			unsigned int NumBytes = helper->mDeviceModel/80000; //either 1600XX or 800XX->returns 2 or 1
			bool demomode = helper->mCustomData!=0;
			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;
			ULONG DecodedBufferSizeUnitary = helper->mSource[0].mAcquiredChannels *helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSize = DecodedBufferSizeUnitary * NUM_DECODED_BUFFERS;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize * sizeof(__int16);
			ULONG SingleAcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels*helper->mSource[0].mDataRegroupFactor*sizeof(char)*helper->mSource[0].mBytesPerChannel;

			ULONG numberOfBytesTransferred;
			unsigned short *DecodedBuffer;
			unsigned short *DecodedBufferPos;
			unsigned char *DecodedBufferPosAsChar;
			unsigned short *DecodedBufferEnd;
			UCHAR * AcquisitionBuffer = NULL;
			OVERLAPPED *OverlappedStructRead = NULL;
			OVERLAPPED *OverlappedStructWrite = NULL;
			OVERLAPPED *OverlappedStruct = NULL;
			HANDLE hCompletionPort = NULL;

			DWORD totalNumberOfBytesTransferred = 0;
			SetLastError(0);			
			
			//Overlapped structure creation
			OverlappedStruct = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, NUM_ASYNCH_IO * sizeof(OVERLAPPED));
			if (OverlappedStruct == NULL)
				goto EmergencyExit;
			for (int i = 0; i < NUM_ASYNCH_IO; i++)
			{
				//ZeroMemory(&OverlappedStruc[i], sizeof(OVERLAPPED));
				//OverlappedStruc[i].Internal = STATUS_PENDING;
			}

			//Acquisition buffer allocation		
			AcquisitionBuffer = (UCHAR *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, SingleAcquisitionBufferSizeInBytes*NUM_ASYNCH_IO);
			if (AcquisitionBuffer == NULL)
				goto EmergencyExit;

			//Decoded buffer allocation
			DecodedBuffer = (unsigned short*)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, DecodedBufferSizeInBytes);
			if (DecodedBuffer == NULL)
				goto EmergencyExit;

			//Temporary overlapped structure used to send start command
			OverlappedStructRead = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, sizeof(OVERLAPPED));
			if (OverlappedStructRead == NULL)
				goto EmergencyExit;
			OverlappedStructWrite = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, sizeof(OVERLAPPED));
			if (OverlappedStructWrite == NULL)
				goto EmergencyExit;








			//modficare qui per leggere tutto
			UCHAR buff[10000];
			if (FALSE == ReadFile(helper->mDeviceHandle, buff, 10000, &numberOfBytesTransferred, OverlappedStructRead))
			{
				if (ERROR_IO_PENDING == GetLastError())
				{
					if (WAIT_OBJECT_0 != WaitForSingleObject(OverlappedStructRead->hEvent, 1000) ||
						!::GetOverlappedResult(helper->mDeviceHandle, OverlappedStructRead, &numberOfBytesTransferred, TRUE))
						DO_NOTHING;
				}
				else
					goto EmergencyExit;
			}
			//send start command
			//OverlappedStrucTmp->Internal = STATUS_PENDING;			

			unsigned char Command;
			if (NumBytes==1)
			{
				Command = demomode ? CommandSetDeviceModeDemo : CommandSetDeviceMode;
			}
			else
			{
				Command = demomode ? CommandSetDeviceModeDemo2Byte : CommandSetDeviceMode2Byte;
			}
			

			SetLastError(0);			
			if (FALSE == WriteFile(helper->mDeviceHandle, &Command, 1, &numberOfBytesTransferred, OverlappedStructWrite))
			{
				if (ERROR_IO_PENDING == GetLastError())
				{
					if (WAIT_OBJECT_0 != WaitForSingleObject(OverlappedStructWrite->hEvent, 1000) ||
						!::GetOverlappedResult(helper->mDeviceHandle, OverlappedStructWrite, &numberOfBytesTransferred, TRUE))
						goto EmergencyExit;
				}
				else
					goto EmergencyExit;
			}	
		

			SLEEPLARGE;
			//IO Completion Port creation
			hCompletionPort = CreateIoCompletionPort(helper->mDeviceHandle, NULL, 1, 0);
			if (hCompletionPort == NULL)
				goto EmergencyExit;

			//points to the beginning of the decoded buffer
			DecodedBufferPos = DecodedBuffer;
			DecodedBufferPosAsChar = (unsigned char*)DecodedBufferPos;
			DecodedBufferEnd = DecodedBuffer + DecodedBufferSize-1;

			unsigned __int16 * DecodedBufferCurrentStart = DecodedBufferPos;
			unsigned __int16 * DecodedBufferCurrentEnd = DecodedBufferCurrentStart+ DecodedBufferSizeUnitary-1;
			
			//Initialize Overlapped reading calling ReadFile on each overlapped structure item
			for (int i = 0; i < NUM_ASYNCH_IO; i++)
			{
				if (FALSE == ReadFile(helper->mDeviceHandle,
					AcquisitionBuffer + (i * SingleAcquisitionBufferSizeInBytes),
					SingleAcquisitionBufferSizeInBytes, &numberOfBytesTransferred, &OverlappedStruct[i]))
				{
					if (ERROR_IO_PENDING != GetLastError())
						goto EmergencyExit;
				}
			}
			SLEEPLARGE;

			//this is the main acquisition loop
			WHILE(*mMustStopAcquisitionThread == false)
			{
				SetLastError(0);
				OVERLAPPED *completedOv;
				ULONG_PTR   key;

				if (0 == GetQueuedCompletionStatus(hCompletionPort, &numberOfBytesTransferred,
					&key, &completedOv, INFINITE))
				{
					helper->mError("", GetLastError(), __FUNCTION__);
					break;
				}
				size_t ReceivedBufferIndex = completedOv - OverlappedStruct;
				if (ReceivedBufferIndex < NUM_ASYNCH_IO)
				{

					totalNumberOfBytesTransferred += numberOfBytesTransferred;
					
					UCHAR* AcquisitionBufferCurrentReadStart =
						&AcquisitionBuffer[ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes];					

					if (helper->mSource[0].mBytesPerChannel == 2)
					{
						for (unsigned int koffset = 0; koffset < numberOfBytesTransferred; koffset++)
						{
							*DecodedBufferPosAsChar = *AcquisitionBufferCurrentReadStart;
							AcquisitionBufferCurrentReadStart++;

							//If buffer is full, send data with newDataAvailable
							if (DecodedBufferPosAsChar == ((unsigned char*)DecodedBufferCurrentEnd)+1)
							{
								helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr,  helper->mSource[0].mAcquiredChannels, 0,helper->mSource[0].mDataRegroupFactor,0);

								if (DecodedBufferPosAsChar == ((unsigned char*) DecodedBufferEnd)+1)
								{
									DecodedBufferPosAsChar = (unsigned char*)DecodedBuffer;
								}
								else
								{
									DecodedBufferPosAsChar++;
								}
								DecodedBufferCurrentStart = (unsigned short*)DecodedBufferPosAsChar;
								DecodedBufferCurrentEnd = DecodedBufferCurrentStart + DecodedBufferSizeUnitary - 1;
							}
							else
							{
								DecodedBufferPosAsChar++;
							}
						}

					}
					else
					{
						for (unsigned int koffset = 0; koffset < numberOfBytesTransferred; koffset++)
						{						
							*DecodedBufferPos = *AcquisitionBufferCurrentReadStart;
							AcquisitionBufferCurrentReadStart++;
						
							if (DecodedBufferPos == DecodedBufferCurrentEnd)
							{
								helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels, 0,helper->mSource[0].mDataRegroupFactor,0);

								if (DecodedBufferPos == DecodedBufferEnd)
								{
									DecodedBufferPos = DecodedBuffer;
								}
								else
								{
									DecodedBufferPos++;
								}
								DecodedBufferCurrentStart = DecodedBufferPos;
								DecodedBufferCurrentEnd = DecodedBufferCurrentStart + DecodedBufferSizeUnitary - 1;
							}
							else
							{
								DecodedBufferPos++;
							}
						}
					}


					
					//ignore buffers which are not completely full. 
					//This could happen only if the device stops the transmission because of external causes/problems.
					
					if (*mMustStopAcquisitionThread) break;
					SetLastError(0);
					//start new reading
					if (FALSE == ReadFile(helper->mDeviceHandle,
						AcquisitionBuffer + (ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes),
						SingleAcquisitionBufferSizeInBytes, NULL, completedOv))
					{
						if (ERROR_IO_PENDING != GetLastError())
						{
							goto EmergencyExit;
						}
					}
				}


			}//while

			CancelIo(helper->mDeviceHandle);
			// We are not recording any more
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

			if (DecodedBuffer)
			{
				LocalFree((void *)DecodedBuffer);
				DecodedBuffer = NULL;
			}
			helper->mStopAcquisition();

			CloseHandle(hCompletionPort);
			LocalFree((void *)OverlappedStruct);
			LocalFree((void *)AcquisitionBuffer);
			LocalFree((void *)DecodedBuffer);
			return 0;
		
		EmergencyExit:
			helper->mError("", GetLastError(), __FUNCTION__);
			if (hCompletionPort) 
				CloseHandle(hCompletionPort); 
			if (OverlappedStruct) 
				LocalFree((void *)OverlappedStruct); 
			if (AcquisitionBuffer) 
				LocalFree((void *)AcquisitionBuffer); 
			if (DecodedBuffer) 
				LocalFree((void *)DecodedBuffer); 
			if (OverlappedStructRead) 
				LocalFree((void *)OverlappedStructRead);
			if (OverlappedStructWrite)
				LocalFree((void *)OverlappedStructWrite);
			return -1;
		}

	}
}

