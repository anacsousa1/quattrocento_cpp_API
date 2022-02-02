#pragma once
#include "DataGeneratorInterface.h"
#include "AdapterTest.h"

using namespace System;


namespace OTComm
{
	namespace Test
	{
		public ref class DataGeneratorTest : public DataGeneratorInterface
		{
		public:
			~DataGeneratorTest();
			!DataGeneratorTest();
			DataGeneratorTest(unsigned int TotalChannels);
			virtual String^ GetDeviceDescription() override;
			virtual String^ GetDeviceXMLConfiguration() override;
			virtual String^ GetSavingXMLConfiguration() override;
			virtual void OpenDevice(void) override;
			virtual void InitializeDevice() override;
			virtual void StartAcquisition(void) override;
			//NB NewData is called from the functions receiving the data, called by StartAcquisition
			virtual void StopAcquisition(void) override;
			virtual void CloseDevice(void) override;
		protected:
			void  OuternThread();
			volatile bool mMustStopAcquisitionThread;
		};

#pragma unmanaged
		ULONG _stdcall AsyncIoTest(PVOID  ThreadParameter);
	}
}