#pragma once

#include "DataGeneratorInterface.h"
#include "AdapterQuattro.h"
#include "Sensors.h"

using namespace System;
using namespace System::Collections::Generic;

namespace OTComm
{
	namespace Quattro
	{
		typedef DWORD SerialBaudRate;
		///<summary>Baudrates available</summary>
		public ref class SerialBaudRates abstract
		{
		public:
			static const SerialBaudRate BR_110 = CBR_110, BR_300 = CBR_300, BR_600 = CBR_600, BR_1200 = CBR_1200, BR_2400 = CBR_2400,
				BR_4800 = CBR_4800, BR_9600 = CBR_9600, BR_14400 = CBR_14400, BR_19200 = CBR_19200,
				BR_38400 = CBR_38400, BR_56000 = CBR_56000, BR_57600 = CBR_57600, BR_115200 = CBR_115200,
				BR_128000 = CBR_128000, BR_230400 = 230400, BR_256000 = CBR_256000, BR_460800 = 460800;
		};


		///<summary>DataGenerator for Quattro</summary>
		public ref class DataGeneratorQuattro : public DataGeneratorInterface
		{
		public:
			~DataGeneratorQuattro();
			!DataGeneratorQuattro();
			/// <param name="Port">string containing the serial port name</param>
			/// <param name="BaudRate">baud rate of communication</param>
			DataGeneratorQuattro(String ^ Port, SerialBaudRate BaudRate);
			/// <summary>
			/// Gets the DataGeneratorQuattro from an xml file containing configuration of Device
			/// </summary>
			/// <param name="Filename">Complete path to xml file</param>
			///<returns>DataGeneratorQuattro associated to XML file</returns>
			static DataGeneratorQuattro^ FromXMLConfiguration(String^ Filename);
			/// <summary>
			/// Gets the description of the device
			/// </summary>
			///<returns>the description of the device</returns>
			virtual String^ GetDeviceDescription() override;
			/// <summary>
			/// Gets the xml of the configuration of the Device
			/// </summary>
			///<returns>the xml of the configuration of the Device</returns>
			virtual String^ GetDeviceXMLConfiguration() override;
			///<summary>Get saving xml of device</summary>
			///<returns>saving xml of device</returns>
			virtual String^ GetSavingXMLConfiguration() override;
			/// <summary>
			/// Open the device communication
			/// </summary>
			virtual void OpenDevice(void) override;
			/// <summary>
			/// Initialize the device
			/// </summary>
			virtual void InitializeDevice(void) override;
			/// <summary>
			/// Starts the acquisition of data from the device
			/// </summary>
			virtual void StartAcquisition(void) override;
			/// <summary>
			/// Stops the acquisition of data from the device
			/// </summary>
			virtual void StopAcquisition(void) override;
			/// <summary>
			/// Close the device communication
			/// </summary>
			virtual void CloseDevice(void) override;
			/// <summary>
			/// Connects Adapter to Quattro
			/// </summary>
			/// <param name="port">unsigned int containing the coordinate of adapter on the Quattro</param>
			/// <param name="adapter">AdapterQuattro to add</param>
			void ConnectIn(const unsigned int port, AdapterQuattro^ adapter);
			/// <summary>
			/// Connects Adapter to Quattro
			/// </summary>
			/// <param name="adapter">AdapterQuattro to add int the firts connector</param>
			/// <returns>Returns -1 if there is an error, returns 0 if everything is fine </returns>
			void ConnectIn(AdapterQuattro^ adapter);
			/// <summary>
			/// Gets the number of connectors available
			/// </summary>
			/// <returns>the number of connectors available </returns>
			unsigned int GetNumberConnectorsIn();			
			/// <summary>
			/// Gets image of device as Bitmap
			/// </summary>
			virtual property Bitmap^ Image { Bitmap^ get() override; }
			/// <summary>
			/// Gets Com Port of Device
			/// </summary>
			/// <returns>the Com Port of Device </returns>
			String^ GetComPort();
			
		protected:
			String^ mPortName;
			DWORD mBaudRate;
			void  OuternThread();
			void InternalStopSerialDataTransfer(void);
			volatile bool mMustStopAcquisitionThread;
			HANDLE mAsyncIoThreadHandle;
			bool mDeviceIsOpen;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			bool mTestMode;
			array<GenericChannel^>^ mAcquiredChannels;
			array<OTComm::GenericAdapter^>^ mAdapter;

		private:
			static GenericAdapter^ GetAdapterFromID(String^ adapterID);
			static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);

		};

#pragma unmanaged
		ULONG _stdcall AsyncIoSerial(PVOID  ThreadParameter);
		ULONG _stdcall AsyncIoSerialEugenio(PVOID  ThreadParameter);
		ULONG _stdcall AsyncIoEMGUSB(PVOID  ThreadParameter);
		ULONG _stdcall AsyncIoSerialBt(PVOID  ThreadParameter);

		ULONG _stdcall AsyncIoSerialTest(PVOID  ThreadParameter);
		void FreeBuffers();
	}
}