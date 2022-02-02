
#include "DataGeneratorTest.h"
#include "DebugLog.h"
#include <stdio.h>

namespace OTComm
{
#pragma managed
	namespace Test
	{
#pragma region Constructors and destructors
		DataGeneratorTest::DataGeneratorTest(unsigned int TotalChannels) :
			DataGeneratorInterface()
		{
			mDeviceTotalChannels = TotalChannels;
			OTComm::Test::AdapterTest^ ad= gcnew OTComm::Test::AdapterTest();			
			mConnectors = gcnew array<GenericConnector^>(1);			
			mConnectors[0] = gcnew GenericConnector(1);
			mConnectors[0]->Adapter = ad->SetChannels(0, 3, gcnew OTComm::Sensors::SensorGeneralPurpose(TotalChannels));;
		}

		DataGeneratorTest::~DataGeneratorTest()
		{
			CloseDevice();
			this->!DataGeneratorTest();
		}

		DataGeneratorTest::!DataGeneratorTest()
		{
		}

#pragma endregion

#pragma region XML files

		String^ DataGeneratorTest::GetDeviceXMLConfiguration()
		{
			OpenXMLConfigStream(true);
			return CloseXMLConfigStream();
		}

		String^ DataGeneratorTest::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(true);
			return CloseXMLConfigStream();
		}

#pragma endregion

#pragma region Device properties 

		String^ DataGeneratorTest::GetDeviceDescription(void)
		{
			return "Generic test device returning random data";
		}


#pragma endregion

#pragma region Public device operations

		void DataGeneratorTest::OpenDevice(void)
		{			
			DataGeneratorInterface::OpenDevice();
		}

		void DataGeneratorTest::InitializeDevice()
		{
			DataGeneratorInterface::InitializeDevice();
		}

		void DataGeneratorTest::StartAcquisition()
		{
			LOG_ENTERING;
			try
			{
				DataGeneratorInterface::StartAcquisition();
				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorTest::OuternThread));
				mManagedAcquisitionThread->Start();
			}
			catch (Exception ^e)
			{
				StartAcquisitionFailedException^ safe = gcnew StartAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}

		void DataGeneratorTest::StopAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				DataGeneratorInterface::StopAcquisition();
			}
			catch (Exception ^e)
			{
				StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}

		void DataGeneratorTest::CloseDevice(void)
		{
			LOG_ENTERING;
			try
			{
				StopAcquisition();
			}
			catch (Exception ^e)
			{
				CloseDeviceFailedException^ cdfe = gcnew CloseDeviceFailedException(__FUNCTION__, e);
				FireOnError(cdfe);
				throw cdfe;
			}
			LOG_EXITING;
		}
#pragma endregion 

#pragma region Non public device operations
		void DataGeneratorTest::OuternThread()
		{
			try
			{
				DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoTest,  // Thread Func
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
		}


#pragma endregion

#pragma region Unmanaged code

#pragma unmanaged
		ULONG _stdcall AsyncIoTest(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			unsigned short *DecodedBuffer;
			unsigned short *DecodedBufferPos;
			unsigned short *DecodedBufferEnd;
			UCHAR * AcquisitionBuffer;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;

			volatile int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;

			unsigned short *ChannelMappings = helper->mSource[0].mMapping;

			ULONG AcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels*sizeof(unsigned short)*helper->mSource[0].mDataRegroupFactor;
			AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, AcquisitionBufferSizeInBytes);
			if (AcquisitionBuffer == NULL)
			{
				return -1;
			}
			
			ULONG DecodedBufferSize = helper->mSource[0].mAcquiredChannels * NUM_DECODED_BUFFERS*helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize*sizeof(unsigned short);
			DecodedBuffer = (unsigned short *)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
			if (DecodedBuffer == NULL)
			{
				return -1;
			}

			//points to the beginning of the decoded buffer
			DecodedBufferPos = DecodedBuffer;
			DecodedBufferEnd = DecodedBuffer + DecodedBufferSize;

			int ki = 0;
			//this is the main acquisition loop

			WHILE(*mMustStopAcquisitionThread == false)
			{
				int k = 0;
				unsigned short* DecodedBufferCurrentStart = DecodedBufferPos;
				for (unsigned int koffset = 0; koffset < helper->mSource[0].mDataRegroupFactor; koffset++)
				{
					ULONG offset = koffset + helper->mDeviceTotalChannels;
					for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
					{
						//add control for buffer overflow
						*DecodedBufferPos = (UCHAR)((k) % 26 + 'A');// AcquisitionBuffer[ChannelMappings[channel]];
						*DecodedBufferPos = channel + 'A';
						if (channel == 3)
						{
							*DecodedBufferPos = k % 10 < 5 ? 0 : 222;
							k++;
						}
						DecodedBufferPos++;
					}
				}
				if (*mMustStopAcquisitionThread) break;
				try
				{
					helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels,0,helper->mSource[0].mDataRegroupFactor,0);
				}
				catch (int)
				{
				}
				if (DecodedBufferPos == DecodedBufferEnd)
					DecodedBufferPos = DecodedBuffer;
				if (*mMustStopAcquisitionThread) break;
				ki++;
			}//while
			if (!*mMustStopAcquisitionThread)
			{
				helper->mError("Acquisition_ended_unexpectedly", GetLastError(), __FUNCTION__);
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
			return 0;
		}
#pragma endregion
	}
}
