#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "DataGeneratorInterface.h"
#include "Sensors.h"
#include "AdapterSessantaquattro.h"
#include "OTCommTypes.h"
#include "DataValidator.h"
using namespace System::Net;
using namespace System::Net::Sockets;


namespace OTComm {
	namespace Sessantaquattro {

		typedef unsigned int DetectionMode;
		///<summary>Frequencies available</summary>
		public ref class DetectionModes abstract
		{
		public:
			static const DetectionMode Monopolar = 0;
			static const DetectionMode Bipolar = 1;
			static const DetectionMode Differential = 2;
			static const DetectionMode ImpedenceCheck = 6;
			static const DetectionMode ImpedenceCheckAdvanced = 5;
			///<summary>Detection Mode Description</summary>
			///<param name="detectionMode">Detection Mode Code</param>
			///<returns>description of detection mode</returns>
			static String^ Description(const unsigned int detectionMode)
			{
				switch (detectionMode)
				{
				case 1:			return "Bipolar";
				case 2:			return "Differential";
				case 5:			return "Impedance check advanced";
				case 6:			return "Impedance check";
				case 0:
				default:		return "Monopolar";
				}
			}
		};

		typedef unsigned int HighPassFilter;
		///<summary>High Pass Frequencies available</summary>
		public ref class HighPassFilters abstract
		{
		public:
			static const HighPassFilter Disabled = 0;
			static const HighPassFilter Enabled = 1;
		};

		typedef unsigned int Frequency;
		///<summary>Frequencies available</summary>
		public ref class Frequencies abstract
		{
		public:
			static const Frequency Sessantaquattro_500Hz = 500;
			static const Frequency Sessantaquattro_1000Hz = 1000;
			static const Frequency Sessantaquattro_2000Hz = 2000;
			static const Frequency Sessantaquattro_4000Hz = 4000;
		};

		typedef unsigned int Model;
		///<summary>Models available</summary>
		public ref class Models abstract
		{
		public:
			static const Model Sessantaquattro_08= 10008;
			static const Model Sessantaquattro_16 = 10016;
			static const Model Sessantaquattro_32 = 10032;
			static const Model Sessantaquattro_64 = 10064;
		};

		String^ GetEEGLabelFor20EEGFromIndex(int index);
		String^ GetEEGLabelFor64EEGFromIndex(int index);

		///<summary>DataGenerator for Sessantaquattro</summary>
		public ref class DataGeneratorSessantaquattro : public DataGeneratorInterface
		{
		public:
			event OTComm::OpenDevice^ OnOpenDevice;
			void FireOpenDevice() { OnOpenDevice(this, gcnew System::EventArgs()); }
			~DataGeneratorSessantaquattro();
			!DataGeneratorSessantaquattro();
			///<param name="model">Model of device</param>
			///<param name="SamplingFrequency">Sampling Frequency</param>
			DataGeneratorSessantaquattro(Model model, Frequency SamplingFrequency, bool HighRes);
			///<summary>get DataGeneratoSessantaquattro from xml</summary>
			///<param name="Filename">path to XML file</param>
			///<returns>DataGeneratorEMGUSB</returns>
			static DataGeneratorSessantaquattro^ FromXMLConfiguration(String^ Filename);

			///<summary>Get description of device</summary>
			///<returns>description of device</returns>
			virtual String^ GetDeviceDescription() override;
			///<summary>Get xml configuration of device</summary>
			///<returns>xml configuration of device</returns>
			virtual String^ GetDeviceXMLConfiguration() override;
			///<summary>Get saving xml of device</summary>
			///<returns>saving xml of device</returns>
			virtual String^ GetSavingXMLConfiguration() override;
			///<summary>Get model of device</summary>
			///<returns>model of device</returns>
			virtual unsigned int GetDeviceModel() override;
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
			///<summary>get number channel aux</summary>
			///<returns>number channel aux</returns>
			unsigned int GetNumberChannelsAux();
			///<summary>get total number of channel</summary>
			///<returns>total number of channel</returns>
			unsigned int GetNumberTotalChannels();
			///<summary>get number channel non aux</summary>
			///<returns>number channel non aux</returns>
			unsigned int GetNumberChannelsNonAux();
			///<summary>get number in connectors</summary>
			///<returns>number in connectors</returns>
			unsigned int GetNumberConnectorsIn();
			///<summary>get number connectors aux</summary>
			///<returns>number connectors aux</returns>
			unsigned int GetNumberConnectorsAux();
			///<summary>get number control aux</summary>
			///<returns>number control aux</returns>
			unsigned int GetNumberControlAux();
			///<summary>Connect Aux to device</summary>
			///<param name="port">aux connector to fill</param>
			///<param name="adapter">AdapterAux to connect</param>
			void ConnectAux(const unsigned int port, AdapterAux^ adapter);
			void ConnectIsoAux(const unsigned int port, AdapterIsoAux^ adapter);

			///<summary>Connect Aux to device</summary>
			///<param name="port">aux connector to fill</param>
			///<param name="adapter">AdapterAux to connect</param>
			void ConnectControl(const unsigned int port, AdapterControl^ adapter);
			///<summary>Connect In to device</summary>
			///<param name="port">in connector to fill</param>
			///<param name="adapter">AdapterIn to connect</param>
			void ConnectIn(const unsigned int port, AdapterSessantaquattro^ adapter);
			///<summary>get IP Address</summary>
			///<returns>IP Address</returns>
			property String^ IPAdress
			{
				String^ get();
			}
			///<summary>get PortNumber</summary>
			///<returns>PortNumber</returns>
			property int PortNumber
			{
				int get();
			}
			///<summary>set ip address for Sessantaquattro</summary>
			///<param name="IPAddress">IP Address of Sessantaquattro</param>
			void SetIPAddress(String^ IPAddress);

			///<summary>set ip port for Sessantaquattro</summary>
			///<param name="IPPort">IP Port of Sessantaquattro</param>
			void SetIPPort(unsigned int IPPort);
			static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);
			static GenericChannel^ GetChannelFromString(String^ text);

			virtual property DataValidator^ DefaultTriggerValidator
			{
				DataValidator^ get() override;
			}


		protected:
			array<GenericChannel^>^ mAcquiredChannels;
			void CloseConnection();
			int WriteString(String^ command);
			int ReadString(String^% read);
			int ReadDeviceID(UCHAR* dev_id);
			int GetFirmwareVersion(String^% version);
			Model mModel;
			void  OuternThread();
			void InternalStopSessantaquattroDataTransfer(void);
			volatile bool mMustStopAcquisitionThread;
			HANDLE mAsyncIoThreadHandle;
			bool mDeviceIsOpen;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			DataValidatorTrigger^ mDefaulDataValidatorTrigger;
			//NetworkStream^	mNetworkStream;	// NetworkStream through ServerSocket to send/receive data
		private:
			static GenericAdapter^ GetAdapterFromID(String^ adapterID);
			//static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);
			const unsigned int mDefaultPortNumber = 45454;
			unsigned int mPortNumber = mDefaultPortNumber;
			unsigned mAnalogOutputConnector = 0;
			unsigned mAnalogOutputChannel = 0;
			unsigned mAnalogOutputGain = 0;
			String^ mDefaultIPAdress = "192.168.1.1";
			String^ mIPAdress;
			array<Byte>^ CreateSetCommand();
			array<Byte>^ CreateImpedenceCheckCommand();
			array<Byte>^ CreateAdvancedImpedenceCheckCommand();
			array<Byte>^ CreateStartCommand();
			array<Byte>^ CreateStopCommand();
			array<Byte>^ CreateGetCommand();
			static Byte ComputeCRC(array<Byte>^ command, unsigned int Len);
		};

#pragma unmanaged
		ULONG _stdcall AsyncIoSessantaquattro(PVOID  ThreadParameter);
	}
}