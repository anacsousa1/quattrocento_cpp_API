#include <Windows.h>
#include "DataConverter.h"
#include "DataGeneratorSerial.h"
#include "DebugLog.h"
#include "ErrorDescription.h"
using namespace System::Runtime::InteropServices;

namespace OTComm
{
//	namespace Serial
//	{
//#define NUM_DECODED_BUFFERS 10
//
//#pragma managed
//
//		/// <summary>
//		/// Builder of the Serial Generator of Data
//		/// </summary>
//		/// <param PortName="string containing the serial port name"></param>
//		/// <param BaudRate="baud rate of communication"></param>
//		/// <param DeviceTotalChannels="number of channel used"></param>
//		DataGeneratorSerial::DataGeneratorSerial(String^ PortName, SerialBaudRate BaudRate,
//			unsigned int DeviceTotalChannels) :
//			DataGeneratorInterface(), mPortName(PortName), mBaudRate(BaudRate),
//			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false), mDataReaderInterfaceHelper(NULL),
//			mTestMode(false)
//		{
//			mConnectors = gcnew array<GenericConnector^> (1);
//			mConnectors[0]= gcnew GenericConnector(4);//TODO controllare se non sia meglio 4 connettori
//			mSamplingFrequency = 2048;
//			mDeviceTotalChannels = DeviceTotalChannels;
//		}
//
//
//		int DataGeneratorSerial::ConnectIn(AdapterSerial^ adapter)
//		{
//			mConnectors[0]->Adapter = adapter;
//			return 0;
//		}
//
//
//		/// <summary>
//		/// Get device description
//		/// </summary>
//		/// <returns>Returns description of the device</returns>
//		String^ DataGeneratorSerial::GetDeviceDescription(void)
//		{
//			return "Generic serial port class";
//		}
//
//		/// <summary>
//		/// Get device xml configuration
//		/// </summary>
//		/// <returns>Returns xml of the device configuration</returns>
//		String^ DataGeneratorSerial::GetDeviceXMLConfiguration()
//		{
//			if (!OpenXMLConfigStream(true))
//			{
//				return "";
//			}
//
//			mXMLTextStream->WriteStartElement("Device");
//			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
//			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", mDeviceTotalChannels.ToString());
//
//			mXMLTextStream->WriteStartElement("Channels");
//			if (mConnectors[0]->Adapter != nullptr)
//				mConnectors[0]->Adapter->WriteXMLConfiguration(mXMLTextStream,0);
//			mXMLTextStream->WriteEndElement();//"Channels"
//
//			mXMLTextStream->WriteEndElement();//"Device"
//
//
//
//			return CloseXMLConfigStream();
//		}
//
//		String^ DataGeneratorSerial::GetSavingXMLConfiguration()
//		{
//			if (!OpenXMLConfigStream(true))
//			{
//				return "";
//			}
//
//			mXMLTextStream->WriteStartElement("Device");
//			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
//			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", mDeviceTotalChannels.ToString());
//
//			mXMLTextStream->WriteStartElement("Channels");
//			if (mConnectors[0]->Adapter != nullptr)
//				mConnectors[0]->Adapter->WriteXMLConfiguration(mXMLTextStream, 0);
//			mXMLTextStream->WriteEndElement();//"Channels"
//
//			mXMLTextStream->WriteEndElement();//"Device"
//
//
//
//			return CloseXMLConfigStream();
//		}
//		
//		/// <summary>
//		/// Open the device communication
//		/// </summary>
//		/// <returns>Returns -1 if there is an error, returns 0 if everything is fine </returns>
//		int DataGeneratorSerial::OpenDevice(void)
//		{
//			LOG_ENTERING;
//				IntPtr ptrToNativeString = Marshal::StringToHGlobalUni(mPortName);
//			mDeviceHandle = CreateFile((LPCWSTR)ptrToNativeString.ToPointer(), GENERIC_READ | GENERIC_WRITE,
//				0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
//			Marshal::FreeHGlobal(ptrToNativeString);
//			if (mDeviceHandle == INVALID_HANDLE_VALUE)
//			{
//				Events->FireOnError("Open failed", GetLastError(), __FUNCTION__);
//				return -1;
//			}
//			else
//				DebugLog::Print("Serial port opened");
//			DCB PortDCB;
//			// Initialize the DCBlength member. 
//			PortDCB.DCBlength = sizeof(DCB);
//			// Get the default port setting information.
//			if (!GetCommState(mDeviceHandle, &PortDCB))
//			{
//				Events->FireOnError("GetCommState failed", GetLastError(), __FUNCTION__);
//				return -1;
//			}
//			else
//				DebugLog::Print("Serial port: GetCommState OK");
//			PortDCB.ByteSize = 8;
//			PortDCB.StopBits = ONESTOPBIT;
//			PortDCB.Parity = NOPARITY;
//			PortDCB.BaudRate = mBaudRate;
//
//			// Configure the port according to the specifications of the DCB structure.
//			if (!SetCommState(mDeviceHandle, &PortDCB))
//			{
//				Events->FireOnError("SetCommState failed", GetLastError(), __FUNCTION__);
//				DebugLog::PrintLastError("SetCommState failed", __FUNCTION__);
//				CloseHandle(mDeviceHandle);
//				return -1;
//			}
//
//			if (!SetCommMask(mDeviceHandle, EV_RXCHAR))
//			{
//				Events->FireOnError("SetCommMask failed", GetLastError(), __FUNCTION__);
//				DebugLog::PrintLastError("SetCommMask failed", __FUNCTION__);
//				CloseHandle(mDeviceHandle);
//				return -1;
//			}
//
//			DebugLog::Print("******Serial port: exiting OpenDevice");
//			mDeviceIsOpen = true;
//			return DataGeneratorInterface::OpenDevice();
//		}
//
//		/// <summary>
//		/// Initialize the device
//		/// </summary>
//		/// <param AcquiredChannels="array containing acquired channels"></param>
//		/// <param SamplingFrequency="sampling frequency of acquisition"></param>
//		/// <returns>Returns -1 if there is an error, returns 0 if everything is fine </returns>
//		int DataGeneratorSerial::InitializeDevice()
//		{
//			DebugLog::Print("******Serial port: entering InitializeDevice");
//
//			if ((mTemporaryResult = InternalStopSerialDataTransfer()) != 0)return mTemporaryResult;
//
//			DebugLog::Print("******Serial port: exiting InitializeDevice");
//			return DataGeneratorInterface::InitializeDevice();
//		}
//
//
//
//		/// <summary>
//		/// Function that creates the Async Acquisition Thread
//		/// </summary>
//		void DataGeneratorSerial::OuternThread()
//		{
//			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
//			pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
//			mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
//			HANDLE mAsyncIoThreadHandle = CreateThread(
//				NULL,			// Default Security Attrib.
//				0,				// Initial Stack Size,
//				AsyncIoSerial,  // Thread Func
//				mDataReaderInterfaceHelper, // Thread parameter
//				0,             // Creation Flags
//				NULL);        // Don't need the Thread Id.
//			if (mAsyncIoThreadHandle == NULL)
//			{
//				Events->FireOnError("Error creating thread", GetLastError(), __FUNCTION__);
//				DebugLog::PrintLastError("Error creating thread", __FUNCTION__);
//				return;
//			}
//			WaitForSingleObject(mAsyncIoThreadHandle, INFINITE);
//		}
//
//		/// <summary>
//		/// Starts the acquisition of data from the device
//		/// </summary>
//		/// <returns>Returns -1 if there is an error, returns 0 if everything is fine </returns>
//		int DataGeneratorSerial::StartAcquisition(void)
//		{
//			LOG_ENTERING;
//				mTemporaryResult = 0;
//			char buf[1] = { (char)((mBytesPerChannel == 1 ? 223 : 239) + (mTestMode ? 12 : 0) + mDeviceTotalChannels) }; // Decimal value to write to serial port
//
//			OVERLAPPED *OverlappedStruc;
//			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
//			if (OverlappedStruc == NULL)
//			{
//				Events->FireOnError("problem creating overlapped struct", GetLastError(), __FUNCTION__);
//				DebugLog::PrintLastError("problem creating overlapped struct", __FUNCTION__);
//				return -1;
//			}
//			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
//			OverlappedStruc->Internal = STATUS_PENDING;
//			SetLastError(0);
//			BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, 1, NULL, OverlappedStruc);
//			if (WriteFileRes == FALSE)
//			{
//				if (GetLastError() == ERROR_IO_PENDING)
//				{
//					Events->FireOnError("problem creating overlapped struct", GetLastError(), __FUNCTION__);
//					return -1;
//				}
//				ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
//				OverlappedStruc->Internal = STATUS_PENDING;
//				SetLastError(0);
//				BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, 1, NULL, OverlappedStruc);
//				DWORD numberOfBytesTransferred = 0;
//				if (WriteFileRes == FALSE)
//				{
//					if (GetLastError() == ERROR_IO_PENDING)
//					{
//						if (!GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
//						{
//							Events->FireOnError("problem sending channels number", GetLastError(), __FUNCTION__);
//							DebugLog::PrintLastError("problem sending channels number", __FUNCTION__);
//							mTemporaryResult = -1;
//						}
//						else
//							Sleep(1);
//					}
//					else
//					{
//						Events->FireOnError("problem sending channels number", GetLastError(), __FUNCTION__);
//						DebugLog::PrintLastError("problem sending channels number", __FUNCTION__);
//						mTemporaryResult = -1;
//					}
//
//
//				}
//				else
//				{
//					Events->FireOnError("problem sending channels number", GetLastError(), __FUNCTION__);
//					mTemporaryResult = -1;
//				}
//			}
//
//			if (mTemporaryResult == 0)
//			{
//				mTemporaryResult = DataGeneratorInterface::StartAcquisition();
//				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorSerial::OuternThread));
//				mManagedAcquisitionThread->Start();
//				LOG_SUCCESS("Serial Port: thread started");
//			}
//			LOG_EXITING;
//				return mTemporaryResult;
//		}
//
//		/// <summary>
//		/// Stops the acquisition of data from the device
//		/// </summary>
//		/// <returns>Returns -1 if there is an error, returns 0 if everything is fine </returns>
//		int DataGeneratorSerial::StopAcquisition(void)
//		{
//			LOG_ENTERING;
//				DataGeneratorInterface::StopAcquisition();
//			mTemporaryResult = InternalStopSerialDataTransfer();
//			LOG_EXITING;
//				return mTemporaryResult;
//		}
//
//		/// <summary>
//		/// Close the device communication
//		/// </summary>
//		/// <returns>Returns -1 if there is an error, returns 0 if everything is fine </returns>
//		int DataGeneratorSerial::CloseDevice(void)
//		{
//			LOG_ENTERING;
//				if (mDeviceHandle != INVALID_HANDLE_VALUE && mDeviceIsOpen)
//				{
//					InternalStopSerialDataTransfer();
//					CloseHandle(mDeviceHandle);
//					mDeviceHandle = INVALID_HANDLE_VALUE;
//				}
//			DebugLog::Print("Serial port closed");
//			mDeviceIsOpen = false;
//			mTemporaryResult = DataGeneratorInterface::CloseDevice();
//			LOG_EXITING;
//				return mTemporaryResult;
//		}
//
//		/// <summary>
//		/// Destroyer
//		/// </summary>
//		DataGeneratorSerial::~DataGeneratorSerial()
//		{
//			if (mDeviceIsOpen)
//			{
//				CloseDevice();
//			}
//			this->!DataGeneratorSerial();
//		}
//
//		/// <summary>
//		/// 
//		/// </summary>
//		DataGeneratorSerial::!DataGeneratorSerial()
//		{
//
//		}
//
//		/// <summary>
//		/// Stop Acquisition of device
//		/// </summary>
//		/// <returns> Returns -1 if there is an error, returns 0 if everything is fine </returns>
//		int DataGeneratorSerial::InternalStopSerialDataTransfer(void)
//		{
//			int res = 0;
//			DebugLog::Print("Serial Port: sending ZERO to STOP ACQUISITION");
//			char buf[1] = { 0 };
//			ULONG numberOfBytesTransferred;
//			OVERLAPPED *OverlappedStruc;
//			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
//			if (OverlappedStruc == NULL)
//			{
//				DebugLog::PrintLastError("problem creating overlapped struct", __FUNCTION__);
//				return -1;
//			}
//			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
//			OverlappedStruc->Internal = STATUS_PENDING;
//			SetLastError(0);
//			BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, 1, NULL, OverlappedStruc);
//			if (WriteFileRes == FALSE)
//			{
//				if (GetLastError() == ERROR_IO_PENDING)
//				{
//					if (!GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
//					{
//						DebugLog::PrintLastError("problem stopping acquisition", __FUNCTION__);
//						res = -1;
//					}
//					else
//						Sleep(1);
//				}
//				else
//				{
//					DebugLog::PrintLastError("problem stopping acquisition", __FUNCTION__);
//					res = -1;
//				}
//			}
//			LocalFree((void *)OverlappedStruc);
//			OverlappedStruc = NULL;
//			return res;
//		}
//
//		bool DataGeneratorSerial::TestMode::get(void)
//		{
//			return mTestMode;
//		}
//
//		void DataGeneratorSerial::TestMode::set(bool testmode)
//		{
//			mTestMode = testmode;
//		}
//
//		bool DataGeneratorSerial::HighResolution::get(void)
//		{
//			return mBytesPerChannel == 2;
//		}
//
//		void DataGeneratorSerial::HighResolution::set(bool res)
//		{
//			mBytesPerChannel = res ? 2 : 1;
//		}
//
//
//
//
//#pragma unmanaged
//
//		/// <summary>
//		/// Function of async serial acquisition
//		/// </summary>
//		/// <param ThreadParameter="ThreadParameter"></param>
//		ULONG _stdcall AsyncIoSerial(PVOID  ThreadParameter)
//		{
//			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
//			unsigned __int16 *ChannelMappings = helper->mMapping;
//
//			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;
//
//			ULONG DecodedBufferSize = helper->mAcquiredChannels * NUM_DECODED_BUFFERS;
//			//volatile LONG *RecordingSamples;
//			unsigned short *DecodedBuffer;
//			unsigned short *DecodedBufferPos;
//			unsigned short *DecodedBufferEnd;
//			UCHAR * AcquisitionBuffer;
//
//			ULONG numberOfBytesTransferred;
//			OVERLAPPED *OverlappedStruc;
//			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
//
//			if (OverlappedStruc == NULL)
//			{
//				return -1;
//			}
//			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
//			OverlappedStruc->Internal = STATUS_PENDING;
//			ULONG AcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels*sizeof(char)*helper->mBytesPerChannel*helper->mDataRegroupFactor;
//			ULONG DecodedBufferSizeInBytes = DecodedBufferSize*sizeof(__int16)*helper->mDataRegroupFactor;
//			AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, AcquisitionBufferSizeInBytes);
//
//			if (AcquisitionBuffer == NULL)
//			{
//				LocalFree((void *)OverlappedStruc);
//				OverlappedStruc = NULL;
//				//DebugLog::Print("Acquisition buffer is null");
//				return -1;
//			}
//			else
//				;//DebugLog::Print("Acquisition buffer is OK");
//
//
//
//			DecodedBuffer = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
//			if (DecodedBuffer == NULL)
//			{
//				LocalFree((void *)OverlappedStruc);
//				OverlappedStruc = NULL;
//				LocalFree((void *)AcquisitionBuffer);
//				AcquisitionBuffer = NULL;
//				//DebugLog::Print("DecodedBuffer is null");
//				return -1;
//			}
//			else
//				;//DebugLog::Print("DecodedBuffer is OK");
//
//			//points to the beginning of the decoded buffer
//			DecodedBufferPos = DecodedBuffer;
//			DecodedBufferEnd = DecodedBuffer + DecodedBufferSize;
//
//			//this is the main acquisition loop
//			bool exitMainLoop = false;
//			DWORD lettitot = 0;
//			WHILE(*mMustStopAcquisitionThread == false)
//			{
//				SetLastError(0);
//				//try an async read
//				BOOL ReadFileRes = ReadFile(helper->mDeviceHandle, AcquisitionBuffer,
//					AcquisitionBufferSizeInBytes, &numberOfBytesTransferred, OverlappedStruc);
//				if (ReadFileRes == FALSE)
//				{
//					if (GetLastError() == ERROR_IO_PENDING)
//					{
//						while (exitMainLoop == false && !GetOverlappedResult(helper->mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, false))
//						{
//							DWORD err = GetLastError();
//							if (err != ERROR_IO_PENDING && err != ERROR_IO_INCOMPLETE)
//							{
//								//error->exit
//								helper->mError("", err, __FUNCTION__);
//								exitMainLoop = true;
//							}
//							else if (*mMustStopAcquisitionThread != false)
//							{
//								//exit
//								exitMainLoop = true;
//							}
//							else
//								Sleep(1);
//						}
//					}
//
//					else
//					{
//						exitMainLoop = true;
//					}
//				}
//
//				if (exitMainLoop)break;
//				lettitot += numberOfBytesTransferred;
//				unsigned short* DecodedBufferCurrentStart = DecodedBufferPos;
//				if (helper->mBytesPerChannel == 1)
//				{
//					for (unsigned int channel = 0; channel < helper->mAcquiredChannels; channel++)
//					{
//						//add control for buffer overflow
//						*DecodedBufferPos = AcquisitionBuffer[ChannelMappings[channel]];
//						DecodedBufferPos++;
//					}
//				}
//				else
//				{
//					for (unsigned int koffset = 0; koffset < helper->mDataRegroupFactor; koffset++)
//					{
//						ULONG offset = koffset + helper->mDeviceTotalChannels;
//						for (unsigned int channel = 0; channel < helper->mAcquiredChannels; channel++)
//						{
//							//add control for buffer overflow
//							int cm = offset + ChannelMappings[channel] * 2;
//							*DecodedBufferPos = AcquisitionBuffer[cm + 1];
//							*DecodedBufferPos <<= 8;
//							*DecodedBufferPos += AcquisitionBuffer[cm];
//							DecodedBufferPos++;
//						}
//					}
//				}
//
//				if (*mMustStopAcquisitionThread) break;
//				//////////////////////////////////////
//				helper->mNewDataAvailable(DecodedBufferCurrentStart, helper->mAcquiredChannels, helper->mDataRegroupFactor);
//
//				if (DecodedBufferPos == DecodedBufferEnd)
//					DecodedBufferPos = DecodedBuffer;
//				if (*mMustStopAcquisitionThread) break;
//
//
//			}//while
//
//			CancelIo(helper->mDeviceHandle);
//
//
//			// We are not recording any more
//			//InterlockedExchange(RecordingSamples, 0);
//
//
//			if (!*mMustStopAcquisitionThread)
//			{
//				// Signal that acquisition ended before time
//				//GlobalErrorDispatcher("An error occurred",helper->mError);
//			}
//
//			if (AcquisitionBuffer)
//			{
//				LocalFree((void *)AcquisitionBuffer);
//				AcquisitionBuffer = NULL;
//			}
//
//			/*if (OverlappedStruc)
//			{
//				LocalFree((void *)OverlappedStruc);
//				OverlappedStruc = NULL;
//			}*/
//
//			if (DecodedBuffer)
//			{
//				LocalFree((void *)DecodedBuffer);
//				DecodedBuffer = NULL;
//			}
//			helper->mStopAcquisition();
//			return 0;
//		}
//
//	}
//
}