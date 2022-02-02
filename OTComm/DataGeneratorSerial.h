#pragma once

#include "DataGeneratorInterface.h"
#include "Sensors.h"

using namespace System;
using namespace System::Collections::Generic;

namespace OTComm
{
	namespace Serial
	{
		/*typedef DWORD SerialBaudRate;
		public ref class SerialBaudRates abstract
		{
		public:
			static const SerialBaudRate BR_110 = CBR_110, BR_300 = CBR_300, BR_600 = CBR_600, BR_1200 = CBR_1200, BR_2400 = CBR_2400,
				BR_4800 = CBR_4800, BR_9600 = CBR_9600, BR_14400 = CBR_14400, BR_19200 = CBR_19200,
				BR_38400 = CBR_38400, BR_56000 = CBR_56000, BR_57600 = CBR_57600, BR_115200 = CBR_115200,
				BR_128000 = CBR_128000, BR_230400 = 230400, BR_256000 = CBR_256000, BR_460800 = 460800;
		};
		

		public ref class AdapterSerial : public GenericAdapter
		{
		public:
			AdapterSerial() {}
			property String^ Description { String^ get() override { return "AdapterSerial"; }}
			property String^ ID { String^ get() override { return "AdapterSerial"; }}
			property unsigned int  MaxNumChannels { unsigned int get() override { return 4; }}
			property unsigned int  MinSizeSensor { unsigned int get() override { return 1; }}
			property unsigned int  AdapterGain { unsigned int get() override { return 1; }}
		};

		public ref class DataGeneratorSerial : public DataGeneratorInterface
		{
		public:
			~DataGeneratorSerial();
			!DataGeneratorSerial();
			DataGeneratorSerial(String ^ Port, SerialBaudRate BaudRate, unsigned int TotalChannels);
			virtual String^ GetDeviceDescription() override;
			virtual String^ GetDeviceXMLConfiguration() override;
			virtual String^ GetSavingXMLConfiguration() override;
			virtual int OpenDevice(void) override;
			virtual int InitializeDevice(void) override;
			virtual int StartAcquisition(void) override;
			virtual int StopAcquisition(void) override;
			virtual int CloseDevice(void) override;
			int ConnectIn(AdapterSerial^ adapter);
			property bool TestMode
			{
				bool get();
				void set(bool x);
			}
			property bool HighResolution
			{
				bool get();
				void set(bool x);
			}		
			
			array<OTComm::GenericAdapter^>^ mAdapter;

		protected:
			String^ mPortName;
			DWORD mBaudRate;
			void  OuternThread();
			int InternalStopSerialDataTransfer(void);
			volatile bool mMustStopAcquisitionThread;
			HANDLE mAsyncIoThreadHandle;
			bool mDeviceIsOpen;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			bool mTestMode;
			array<GenericChannel^>^ mAcquiredChannels;
		};*/

		/*public ref class DataGeneratorQuattro : public DataGeneratorSerial
		{
		public:
			DataGeneratorQuattro(String ^ Port, unsigned int TotalChannels) :DataGeneratorSerial(Port, 230400, TotalChannels) {}
		};*/


#pragma unmanaged
		ULONG _stdcall AsyncIoSerial(PVOID  ThreadParameter);
	}
}