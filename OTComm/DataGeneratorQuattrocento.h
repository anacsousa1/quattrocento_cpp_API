#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "DataGeneratorInterface.h"
#include "Sensors.h"
#include "AdapterQuattrocento.h"
#include "OTCommTypes.h"
#include "DataValidator.h"
using namespace System::Net;
using namespace System::Net::Sockets;


namespace OTComm {
	namespace Quattrocento {

		typedef unsigned int DetectionMode;
		///<summary>Frequencies available</summary>
		public ref class DetectionModes abstract
		{
		public:
			static const DetectionMode Monopolar = 0;
			static const DetectionMode Differential = 1;
			static const DetectionMode Bipolar = 2;
			///<summary>Detection Mode Description</summary>
			///<param name="detectionMode">Detection Mode Code</param>
			///<returns>description of detection mode</returns>
			static String^ Description(const unsigned int detectionMode)
			{
				switch (detectionMode)
				{
				case 2:			return "Bipolar";
				case 1:			return "Differential";
				case 0:
				default:		return "Monopolar";
				}
			}
		};

		typedef unsigned int LowPassFrequency;
		///<summary>Low Pass Frequencies available</summary>
		public ref class LowPassFrequencies abstract
		{
		public:
			static const LowPassFrequency LP_130Hz = 0;
			static const LowPassFrequency LP_500Hz = 1;
			static const LowPassFrequency LP_900Hz = 2;
			static const LowPassFrequency LP_4400Hz = 3;
			///<summary>Low Pass Frequency Description</summary>
			///<param name="lowPass">Low Pass Frequency Code</param>
			///<returns>description of Low Pass Frequency</returns>
			static double Value(const unsigned int lowPass)
			{
				switch (lowPass)
				{
				case 3:			return 4400;
				case 2:			return 900;
				case 1:			return 500;
				case 0:
				default:			return 130;
				}
			}
		};

		typedef unsigned int HighPassFrequency;
		///<summary>High Pass Frequencies available</summary>
		public ref class HighPassFrequencies abstract
		{
		public:
			static const HighPassFrequency HP_0_7Hz = 0;
			static const HighPassFrequency HP_10Hz = 1;
			static const HighPassFrequency HP_100Hz = 2;
			static const HighPassFrequency HP_200Hz = 3;
			///<summary>High Pass Frequency Description</summary>
			///<param name="highPass">High Pass Frequency Code</param>
			///<returns>description of High Pass Frequency</returns>
			static double Value(const unsigned int highPass)
			{
				switch (highPass)
				{
				case 3:			return 200;
				case 2:			return 100;
				case 1:			return 10;
				case 0:
				default:			return 0.7;
				}
			}
		};

		typedef unsigned int Frequency;
		///<summary>Frequencies available</summary>
		public ref class Frequencies abstract
		{
		public:
			static const Frequency Quattrocento_512Hz = 512;
			static const Frequency Quattrocento_2048Hz = 2048;
			static const Frequency Quattrocento_5120Hz = 5120;
			static const Frequency Quattrocento_10240Hz = 10240;
		};

		typedef unsigned int Model;
		///<summary>Models available</summary>
		public ref class Models abstract
		{
		public:
			static const Model Quattrocento_96 = 40096;
			static const Model Quattrocento_192 = 40192;
			static const Model Quattrocento_288 = 40288;
			static const Model Quattrocento_384 = 40384;
		};

		typedef unsigned int AnalogOutputConnector;
		///<summary>Analog outputs connector available</summary>
		public ref class AnalogOutputConnectors abstract
		{
		public:
			static const AnalogOutputConnector Quattrocento_IN1 = 0;
			static const AnalogOutputConnector Quattrocento_IN2 = 1;
			static const AnalogOutputConnector Quattrocento_IN3 = 2;
			static const AnalogOutputConnector Quattrocento_IN4 = 3;
			static const AnalogOutputConnector Quattrocento_IN5 = 4;
			static const AnalogOutputConnector Quattrocento_IN6 = 5;
			static const AnalogOutputConnector Quattrocento_IN7 = 6;
			static const AnalogOutputConnector Quattrocento_IN8 = 7;
			static const AnalogOutputConnector Quattrocento_M1 = 8;
			static const AnalogOutputConnector Quattrocento_M2 = 9;
			static const AnalogOutputConnector Quattrocento_M3 = 10;
			static const AnalogOutputConnector Quattrocento_M4 = 11;
			static const AnalogOutputConnector Quattrocento_AUX = 12;
		};

		typedef unsigned int AnalogOutputGain;
		///<summary>Analog outputs gain available</summary>
		public ref class AnalogOutputGains abstract
		{
		public:
			static const AnalogOutputGain Gain16 = 48;
			static const AnalogOutputGain Gain4= 32;
			static const AnalogOutputGain Gain2 = 16;
			static const AnalogOutputGain Gain1 = 0;
		};



		///<summary>DataGenerator for Quattrocento</summary>
		public ref class DataGeneratorQuattrocento : public DataGeneratorInterface
		{
		public:
			event OTComm::OpenDevice^ OnOpenDevice;
			void FireOpenDevice() { OnOpenDevice(this, gcnew System::EventArgs()); }
			~DataGeneratorQuattrocento();
			!DataGeneratorQuattrocento();
			///<param name="model">Model of device</param>
			///<param name="SamplingFrequency">Sampling Frequency</param>
			DataGeneratorQuattrocento(Model model, Frequency SamplingFrequency);
			///<summary>get DataGeneratoQuattrocento from xml</summary>
			///<param name="Filename">path to XML file</param>
			///<returns>DataGeneratorEMGUSB</returns>
			static DataGeneratorQuattrocento^ FromXMLConfiguration(String^ Filename);

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
			///<summary>Raise trigger</summary>
			void RaiseTrigger(void);
			///<summary>Fall trigger</summary>
			void FallTrigger(void);
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
			///<summary>get number multiple in connectors</summary>
			///<returns>number multiple in connectors</returns>
			unsigned int GetNumberConnectorsMultipleIn();
			///<summary>get number in connectors</summary>
			///<returns>number in connectors</returns>
			unsigned int GetNumberConnectorsIn();
			///<summary>get number connectors aux</summary>
			///<returns>number connectors aux</returns>
			unsigned int GetNumberConnectorsAux();
			///<summary>get number control aux</summary>
			///<returns>number control aux</returns>
			unsigned int GetNumberControlAux();
			///<summary>get acquired channels</summary>
			///<returns>array of acquired channels</returns>
			array<unsigned int>^ GetAcquiredChannels();
			///<summary>Connect Aux to device</summary>
			///<param name="port">aux connector to fill</param>
			///<param name="adapter">AdapterAux to connect</param>
			void ConnectAux(const unsigned int port, AdapterAux^ adapter);
			///<summary>Connect Aux to device</summary>
			///<param name="port">aux connector to fill</param>
			///<param name="adapter">AdapterAux to connect</param>
			void ConnectControl(const unsigned int port, AdapterControl^ adapter);
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
			///<summary>Set connector you want as analog output</summary>
			///<param name="analogOutputConnector">analog output to connect</param>
			void SetAnalogOutputConnector(unsigned int analogOutputConnector);
			///<summary>Get connector you set analog output</summary>
			unsigned int GetAnalogOutputConnector();
			///<summary>Set gain you want as analog output</summary>
			///<param name="analogOutputGain">analog output gain</param>
			void SetAnalogOutputGain(unsigned int analogOutputGain);
			///<summary>Set channel you want as analog output</summary>
			///<param name="analogOutputChannel">analog channel to connect</param>
			void SetAnalogOutputChannel(unsigned int analogOutputChannel);
			///<summary>Get channel set as analog output</summary>
			unsigned int GetAnalogOutputChannel();
			///<summary>Get gain set as analog output gain</summary>
			unsigned int GetAnalogOutputGain();
			///<summary>set ip address for Quattrocento</summary>
			///<param name="IPAddress">IP Address of Quattrocento</param>
			void SetIPAddress(String^ IPAddress);
			///<summary>set ip port for Quattrocento</summary>
			///<param name="IPPort">IP Port of Quattrocento</param>
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
			void InternalStopQuattrocentoDataTransfer(void);
			volatile bool mMustStopAcquisitionThread;
			HANDLE mAsyncIoThreadHandle;
			bool mDeviceIsOpen;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			array<unsigned int>^ CreateChannelsMapping();
			DataValidatorTrigger^ mDefaulDataValidatorTrigger;
			//NetworkStream^	mNetworkStream;	// NetworkStream through ServerSocket to send/receive data
		private:
			static GenericAdapter^ GetAdapterFromID(String^ adapterID);
			//static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);
			const unsigned int mDefaultPortNumber = 23456;
			unsigned int mPortNumber = mDefaultPortNumber;
			unsigned mAnalogOutputConnector = 0;
			unsigned mAnalogOutputChannel = 0;
			unsigned mAnalogOutputGain = 0;
			String^ mDefaultIPAdress = "169.254.1.10";
			String^ mIPAdress;
			array<Byte>^ CreateStopCommand();
			array<Byte>^ CreateStartCommand();
			array<Byte>^ CreateRaiseTriggerCommand();
			array<Byte>^ CreateFallTriggerCommand();
			array<Byte>^ CreateStartStopCommands(bool Start, bool TriggerOn);
			static Byte ComputeCRC(array<Byte>^ command, unsigned int Len);
			unsigned int RecognizeAdapterForCommand(String^ adapterID);
			unsigned int RecognizeSensorForCommand(String^ channelID);
			unsigned int RecognizeSideForCommand(String^ sideID);
			unsigned int RecognizeHPForCommand(unsigned int highpass);
			unsigned int RecognizeLPForCommand(unsigned int lowpass);
		};

#pragma unmanaged
		ULONG _stdcall AsyncIoQuattrocento(PVOID  ThreadParameter);
	}
}