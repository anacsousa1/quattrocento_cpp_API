#pragma once

#include "DataGeneratorInterface.h"
#include "AdapterForza.h"
#include "Sensors.h"

using namespace System;
using namespace System::Collections::Generic;

namespace OTComm
{
	namespace Forza
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
		
		typedef unsigned int Frequency;
		///<summary>Frequencies available</summary>
		public ref class Frequencies abstract
		{
		public:
			static const Frequency FORZA_100Hz = 100;
		};


		///<summary>DataGenerator for Forza</summary>
		public ref class DataGeneratorForza : public DataGeneratorInterface
		{
		public:
			~DataGeneratorForza();
			!DataGeneratorForza();
			/// <summary>
			/// Builder of the Forza Generator of Data
			/// </summary>
			/// <param name="Port">string containing the serial port name</param>
			/// <param name="BaudRate">baud rate of communication</param>
			DataGeneratorForza(String ^ Port, SerialBaudRate BaudRate);
			/// <summary>
			/// Get device description
			/// </summary>
			/// <returns>Returns description of the device</returns>
			virtual String^ GetDeviceDescription() override;
			/// <summary>
			/// Get device xml configuration
			/// </summary>
			/// <returns>Returns xml of the device configuration</returns>
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
			///<summary>Connect In to device</summary>
			///<param name="adapter">AdapterForza to connect</param>
			void ConnectIn(AdapterAux^ adapter);
			///<summary>get number in connectors</summary>
			///<returns>number in connectors</returns>
			unsigned int GetNumberConnectorsIn();							
			///<summary>Get image of device</summary>
			///<returns>image of device</returns>
			virtual property Bitmap^ Image { Bitmap^ get() override; }
			///<summary>get DataGeneratorEMGUSB from xml</summary>
			///<param name="Filename">path to XML file</param>
			///<returns>DataGeneratorEMGUSB</returns>
			static DataGeneratorForza^ FromXMLConfiguration(String^ Filename);
			///<summary>Get sensibility of cell</summary>
			///<returns>sensibility of cell</returns>
			double^ GetSensibility();
			///<summary>Set sensibility of cell</summary>
			///<param name="sensibility">sensibility</param>
			void SetSensibility(double sensibility);
			///<summary>Get maxmimum load of cell</summary>
			///<returns>maxmimum load of cell</returns>
			double^ GetMaximumLoad();
			///<summary>Set maxmimum load of cell</summary>
			///<param name="maximumLoad">maximumLoad</param>
			void SetMaximumLoad(double maximumLoad);
			///<summary>Get COMPORT of Forza</summary>
			///<returns> COMPORT of Forza</returns>
			String^ GetComPort();

		protected:
			array<OTComm::GenericAdapter^>^ mAdapter;
			String^ mPortName;
			DWORD mBaudRate;
			void  OuternThread();
			double mSensibility=1.0;
			double mMaximumLoad=1.0;
			void InternalStopSerialDataTransfer(void);
			volatile bool mMustStopAcquisitionThread;
			HANDLE mAsyncIoThreadHandle;
			bool mDeviceIsOpen;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			bool mTestMode;
			array<GenericChannel^>^ mAcquiredChannels;

		private:
			static AdapterAux^ GetAdapterFromID(String^ adapterID);
			static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);

		};

#pragma unmanaged
		ULONG _stdcall AsyncIoSerialEugenio(PVOID  ThreadParameter);
		ULONG _stdcall AsyncIoSerial(PVOID  ThreadParameter);
	}
}