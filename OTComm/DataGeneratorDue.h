#pragma once

#include "Sensors.h"
#include "AdapterDue.h"
#include "DataGeneratorInterface.h"

using namespace System;
using namespace System::Collections::Generic;

namespace OTComm
{
	namespace Due
	{
		typedef DWORD SerialBaudRate;
		
		///<summary>Baudrates available</summary>
		public ref class SerialBaudRates abstract
		{
		public:
			///<summary>CBR_110</summary>
			static const SerialBaudRate BR_110 = CBR_110;
			///<summary>CBR_300</summary>
			static const SerialBaudRate 	BR_300 = CBR_300;
			///<summary>CBR_600</summary>
			static const SerialBaudRate 	BR_600 = CBR_600;
			///<summary>CBR_1200</summary>
			static const SerialBaudRate 	BR_1200 = CBR_1200;
			///<summary>CBR_2400</summary>
			static const SerialBaudRate 	BR_2400 = CBR_2400;
			///<summary>CBR_4800</summary>
			static const SerialBaudRate 	BR_4800 = CBR_4800;
			///<summary>CBR_9600</summary>
			static const SerialBaudRate 	BR_9600 = CBR_9600;
			///<summary>CBR_14400</summary>
			static const SerialBaudRate 	BR_14400 = CBR_14400;
			///<summary>CBR_19200</summary>
			static const SerialBaudRate 	BR_19200 = CBR_19200;
			///<summary>CBR_38400</summary>
			static const SerialBaudRate 	BR_38400 = CBR_38400;
			///<summary>CBR_56000</summary>
			static const SerialBaudRate 	BR_56000 = CBR_56000;
			///<summary>CBR_57600</summary>
			static const SerialBaudRate 	BR_57600 = CBR_57600;
			///<summary>CBR_115200</summary>
			static const SerialBaudRate 	BR_115200 = CBR_115200;
			///<summary>CBR_128000</summary>
			static const SerialBaudRate 	BR_128000 = CBR_128000;
			///<summary>230400</summary>
			static const SerialBaudRate 	BR_230400 = 230400;
			///<summary>CBR_256000</summary>
			static const SerialBaudRate 	BR_256000 = CBR_256000;
			///<summary>460800</summary>
			static const SerialBaudRate 	BR_460800 = 460800;

			static const SerialBaudRate		BR_921600 = 921600;
		};
		
		///<summary>DataGenerator for DUEPRO devices</summary>
		public ref class DataGeneratorDue : public DataGeneratorInterface
		{
		public:
			///<summary>get DataGeneratorDue from xml</summary>
			///<param name="Filename">path to XML file</param>
			///<returns>DataGeneratorDue</returns>
			static DataGeneratorDue^ FromXMLConfiguration(String^ Filename);
			~DataGeneratorDue();
			!DataGeneratorDue();
			///<param name="Port">COM PORT of Due Token</param>
			///<param name="BaudRate">BaudRate</param>
			///<param name="TotalChannels">Total Channels</param>
			DataGeneratorDue(String ^ Port, SerialBaudRate BaudRate);
			///<summary>Get description of device</summary>
			///<returns>description of device</returns>
			virtual String^ GetDeviceDescription(void) override;
			///<summary>Get xml configuration of device</summary>
			///<returns>xml configuration of device</returns>
			virtual String^ GetDeviceXMLConfiguration(void) override;
			///<summary>Get saving xml of device</summary>
			///<returns>saving xml of device</returns>
			virtual String^ GetSavingXMLConfiguration() override;
			///<summary>Open device</summary>
			virtual void OpenDevice(void) override;
			///<summary>Initialize device</summary>
			virtual void InitializeDevice(void) override;
			///<summary>Start acquisition of device</summary>
			virtual void StartAcquisition(void) override;
			///<summary>Stop initialization of device</summary>
			virtual void StopAcquisition(void) override;
			///<summary>Close device</summary>
			virtual void CloseDevice(void) override;

			void ConnectIn(const unsigned int source, const unsigned int port, AdapterDue^ adapter);
			///<summary>Connect In to device</summary>
			///<param name="adapter">AdapterDue to connect</param>
			///<summary>Connect In to device</summary>
			///<param name="port">in connector to fill</param>
			///<param name="adapter">AdapterDue to connect</param>
			void ConnectIn(const unsigned int port, AdapterDue^ adapter);
			///<summary>Connect In to device</summary>
			///<param name="adapter">AdapterDue to connect</param>
			void ConnectIn(AdapterDue^ adapter);
			///<summary>get number in connectors</summary>
			///<returns>number in connectors</returns>

			unsigned int GetNumberConnectorsIn();			
			unsigned int GetNumberConnectorsIn(const unsigned int source);
			
		protected:
			String^ mPortName;
			DWORD mBaudRate;
			void  OuternThread();
			void InternalStopSerialDataTransfer(void);
			volatile bool mMustStopAcquisitionThread;
			HANDLE mAsyncIoThreadHandle;
			bool mDeviceIsOpen;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			array<GenericChannel^>^ mAcquiredChannels;
			array<OTComm::GenericAdapter^>^ mAdapter;			
			
			static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);
			

		private:
			static GenericAdapter^ GetAdapterFromID(String^ adapterID);

		};

		/*public ref class DataGeneratorDue : public DataGeneratorSerial
		{
		public:
			DataGeneratorDue(String ^ Port, unsigned int TotalChannels) :DataGeneratorSerial(Port, 230400, TotalChannels) {}
		};*/


#pragma unmanaged
		ULONG _stdcall AsyncIoSerial(PVOID  ThreadParameter);
		ULONG _stdcall AsyncIoSerialEugenio(PVOID  ThreadParameter);
	}
}