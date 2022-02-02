#pragma once
#include "DataGeneratorInterface.h"
#include "GenericAdapter.h"
#include "Sensors.h"
#include <Windows.h>
#include "AdapterEMGUSB.h"
#include "DataValidator.h"
using  namespace System::Threading;
namespace OTComm {


	namespace EMGUSB
	{
		typedef unsigned int Model;
		typedef unsigned int Frequency;
		///<summary>Frequencies available</summary>
		public ref class Frequencies abstract
		{
		public:
			///<summary>512 Hz</summary>
			static const Frequency EMGUSB_512Hz = 512;
			///<summary>2048 Hz</summary>
			static const Frequency EMGUSB_2048Hz = 2048;
			///<summary>4096 Hz</summary>
			static const Frequency EMGUSB_4096Hz = 4096;
			///<summary>5120 Hz</summary>
			static const Frequency EMGUSB_5120Hz = 5120;
			///<summary>10240 Hz</summary>
			static const Frequency EMGUSB_10240Hz = 10240;
		};
		///<summary>Models available</summary>
		public ref class Models abstract
		{
		public:
			static const Model EMG_USB1_128 = 10128;
			static const Model EMG_USB1_64 = 10064;

			static const Model EMG_USB2_64 = 20064;
			static const Model EMG_USB2_128 = 20128;
			static const Model EMG_USB2_256 = 20256;

			static const Model EMG_USB2p_64 = 30064;
			static const Model EMG_USB2p_128 = 30128;
			static const Model EMG_USB2p_256 = 30256;
/*
			static const Model EMG_USB3 = 30000;
			static const Model MEBA_32 = 810032;
			static const Model WIRELESS_32 = 820032;*/
		};
		///<summary>DataGenerator for EMG-USB devices</summary>
		public ref class DataGeneratorEMGUSB : public DataGeneratorInterface
		{
		public:
			//void Test();
			~DataGeneratorEMGUSB();
			!DataGeneratorEMGUSB();
			///<param name="model">Model of device</param>
			///<param name="SamplingFrequency">Sampling Frequency</param>
			DataGeneratorEMGUSB(Model model, Frequency SamplingFrequency);
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
			static DataGeneratorEMGUSB^ FromXMLConfiguration(String^ Filename);

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
			///<summary>get gains of connectors. Datagenerator must be initialized</summary>
			///<returns>array of gains of connectors</returns>
			array<float>^ GetGainsConnectors();
			///<summary>get low pass of connectors. Datagenerator must be initialized</summary>
			///<returns>array of low pass of connectors</returns>
			//array<int>^ GetLPsConnectors();
			///<summary>get high pass of connectors. Datagenerator must be initialized</summary>
			///<returns>array of high pass of connectors</returns>
			array<float>^ GetHPsConnectors();
			///<summary>get mode of connectors. Datagenerator must be initialized</summary>
			///<returns>array of mode of connectors</returns>
			//array<int>^ GetModesConnectors();

			///<summary>Connect Aux to device</summary>
			///<param name="port">aux connector to fill</param>
			///<param name="adapter">AdapterAux to connect</param>
			void ConnectAux(const unsigned int port, AdapterAux^ adapter);
			///<summary>Connect Multiple In to device</summary>
			///<param name="port">multiple in connector to fill</param>
			///<param name="adapter">AdapterMultipleIn to connect</param>
			void ConnectMultipleIn(const unsigned int port, AdapterMultipleIn^ adapter);
			///<summary>Connect In to device</summary>
			///<param name="port">in connector to fill</param>
			///<param name="adapter">AdapterIn to connect</param>
			void ConnectIn(const unsigned int port, AdapterIn^ adapter);
			///<summary>get mapping of channels</summary>
			///<returns>array of channel indices</returns>
			virtual array<unsigned int>^ GetChannelsMapping() override;
			property DataValidator^ DataGeneratorEMGUSB::DefaultTriggerValidator
			{
				virtual DataValidator^ get() override;
			}
		protected:
			UCHAR ReadDeviceID(void);
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
			void ProgramFPGA();
			array<unsigned int>^ CreateChannelsMapping();
			static GenericAdapter^ GetAdapterFromID(String^ adapterID);
			static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);
			DataValidator^ mDefaultDataValidator;		

		};

#pragma unmanaged
		ULONG _stdcall AsyncIoEMGUSB(PVOID  ThreadParameter);
		BOOL GetDevicePath(IN  LPGUID InterfaceGuid, __out_ecount(BufLen) PWSTR DevicePath, __in size_t BufLen, int DeviceIndex);
		UCHAR dev_id;
	}
}