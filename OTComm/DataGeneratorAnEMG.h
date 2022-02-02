#pragma once
#include "DataGeneratorInterface.h"
#include "GenericAdapter.h"
#include "Sensors.h"
#include <Windows.h>
#include "AdapterAnEMG.h"
using  namespace System::Threading;
namespace OTComm {


	namespace AnEMG
	{
		typedef unsigned int Model;
		typedef unsigned int Frequency;
		///<summary>Frequencies available</summary>
		public ref class Frequencies abstract
		{
		public:
			///<summary>512 Hz</summary>
			static const Frequency AnEMG_512Hz = 512;
			///<summary>2048 Hz</summary>
			static const Frequency AnEMG_2048Hz = 2048;
			///<summary>4096 Hz</summary>
			static const Frequency AnEMG_4096Hz = 4096;
			///<summary>5120 Hz</summary>
			static const Frequency AnEMG_5120Hz = 5120;
			///<summary>10240 Hz</summary>
			static const Frequency AnEMG_10240Hz = 10240;
		};
		///<summary>Models available</summary>
		public ref class Models abstract
		{
		public:
			///<summary>Model name for AnEMG 16 channels</summary>
			static const Model AnEMG_16 = 30064;
		};
		///<summary>DataGenerator for AnEMG devices</summary>
		public ref class DataGeneratorAnEMG : public DataGeneratorInterface
		{
		public:
			~DataGeneratorAnEMG();
			!DataGeneratorAnEMG();
			///<param name="model">Model of device</param>
			///<param name="SamplingFrequency">Sampling Frequency</param>
			DataGeneratorAnEMG(Frequency SamplingFrequency);
			///<summary>Get the ID of the device</summary>
			///<returns>the ID of the device</returns>
			virtual int GetDeviceID(void) override;
			///<summary>Get if device is open</summary>
			///<returns>true if open, false if closed</returns>
			virtual bool IsOpen(void) override;
			///<summary>Get description of device</summary>
			///<returns>description of device</returns>
			virtual String^ GetDeviceDescription(void) override;
			///<summary>Get xml configuration of device</summary>
			///<returns>xml configuration of device</returns>
			virtual String^ GetDeviceXMLConfiguration(void) override;
			///<summary>Get saving xml of device</summary>
			///<returns>saving xml of device</returns>
			virtual String^ GetSavingXMLConfiguration() override;
			///<summary>Get model of device</summary>
			///<returns>model of device</returns>
			virtual unsigned int GetDeviceModel(void) override;
			///<summary>Open device</summary>
			virtual void OpenDevice(void) override;
			///<summary>Initialize device</summary>
			virtual void InitializeDevice(void) override;
			///<summary>Start acquisition of device</summary>
			virtual void StartAcquisition(void) override;
			///<summary>Stop acquisition of device</summary>
			virtual void StopAcquisition(void) override;
			///<summary>Close device</summary>
			virtual void CloseDevice(void) override;
			///<summary>Get image of device</summary>
			///<returns>image of device</returns>
			virtual property Bitmap^ Image { Bitmap^ get() override; }
			///<summary>get DataGeneratorEMGUSB from xml</summary>
			///<param name="Filename">path to XML file</param>
			///<returns>DataGeneratorEMGUSB</returns>
			static DataGeneratorAnEMG^ FromXMLConfiguration(String^ Filename);
			///<summary>get number channel aux</summary>
			///<returns>number channel aux</returns>
			unsigned int GetNumberChannelsAux();
			///<summary>get total number of channel</summary>
			///<returns>total number of channel</returns>
			unsigned int GetNumberTotalChannels();
			///<summary>get number channel non aux</summary>
			///<returns>number channel non aux</returns>
			unsigned int GetNumberChannelsNonAux();
			///<summary>get number multiple in connectors</summary>
			///<returns>number multiple in connectors</returns>
			unsigned int GetNumberConnectorsMultipleIn();
			///<summary>get number in connectors</summary>
			///<returns>number in connectors</returns>
			unsigned int GetNumberConnectorsIn();
			///<summary>get number connectors aux</summary>
			///<returns>number connectors aux</returns>
			unsigned int GetNumberConnectorsAux();
			///<summary>get acquired channels</summary>
			///<returns>array of acquired channels</returns>
			array<unsigned int>^ GetAcquiredChannels();
			///<summary>Connect Aux to device</summary>
			///<param name="port">aux connector to fill</param>
			///<param name="adapter">AdapterAux to connect</param>
			
			void ConnectAux(const unsigned int port, AdapterAux^ adapter);
			///<summary>get mapping of channels</summary>
			///<returns>array of channel indices</returns>
			virtual array<unsigned int>^ GetChannelsMapping() override;

		protected:
			UCHAR ReadDeviceID();
			void ProgramPIC(String^ hexfile);
			String^ GetFirmwareVersion();
			Model mModel;
			String^ mPortName;
			void  OuternThread();
			void InternalStopUSBDataTransfer(void);
			volatile bool mMustStopAcquisitionThread;
			Thread^mManagedAcquisitionThreadHandle;
			//HANDLE mDeviceHandle;
			bool mDeviceIsOpen;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			bool mTestMode;
			void ProgramFPGA();
			array<unsigned int>^ CreateChannelsMapping();
			static GenericAdapter^ GetAdapterFromID(String^ adapterID);
			static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);

		};

#pragma unmanaged
		ULONG _stdcall AsyncIoAnEMG(PVOID  ThreadParameter);
		BOOL GetDevicePath(IN  LPGUID InterfaceGuid, __out_ecount(BufLen) PWSTR DevicePath, __in size_t BufLen, int DeviceIndex);
		UCHAR dev_id;
	}
}