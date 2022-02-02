#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "DataGeneratorInterface.h"
#include "Sensors.h"
#include "AdapterSyncstation.h"
#include "OTCommTypes.h"
#include "DataValidator.h"

using namespace System::Net;
using namespace System::Net::Sockets;


namespace OTComm {

	namespace Syncstation {

		int totalDevices=0;
		public ref class Device {

		public:
			//int EMG = 0;
			int mode = 0;
			int enable = 0;
			int nSample = 0;
			int nChannel = 0;

		};

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
			static const Frequency Syncstation_512Hz = 500;
			static const Frequency Syncstation_2048Hz = 2000;
			static const Frequency Syncstation_5120Hz = 5120;
			static const Frequency Syncstation_10240Hz = 10240;
		};

		typedef unsigned int Model;
		///<summary>Models available</summary>
		public ref class Models abstract
		{
		public:
			static const Model Syncstation_96 = 40096;
			static const Model Syncstation_192 = 40192;
			static const Model Syncstation_288 = 40288;
			static const Model Syncstation_384 = 40384;
		};

		typedef unsigned int AnalogOutputConnector;
		///<summary>Analog outputs connector available</summary>
		public ref class AnalogOutputConnectors abstract
		{
		public:
			static const AnalogOutputConnector Syncstation_IN1 = 0;
			static const AnalogOutputConnector Syncstation_IN2 = 1;
			static const AnalogOutputConnector Syncstation_IN3 = 2;
			static const AnalogOutputConnector Syncstation_IN4 = 3;
			static const AnalogOutputConnector Syncstation_IN5 = 4;
			static const AnalogOutputConnector Syncstation_IN6 = 5;
			static const AnalogOutputConnector Syncstation_IN7 = 6;
			static const AnalogOutputConnector Syncstation_IN8 = 7;
			static const AnalogOutputConnector Syncstation_M1 = 8;
			static const AnalogOutputConnector Syncstation_M2 = 9;
			static const AnalogOutputConnector Syncstation_M3 = 10;
			static const AnalogOutputConnector Syncstation_M4 = 11;
			static const AnalogOutputConnector Syncstation_AUX = 12;
		};

		typedef unsigned int AnalogOutputGain;
		///<summary>Analog outputs gain available</summary>
		public ref class AnalogOutputGains abstract
		{
		public:
			static const AnalogOutputGain Gain16 = 48;
			static const AnalogOutputGain Gain4 = 32;
			static const AnalogOutputGain Gain2 = 16;
			static const AnalogOutputGain Gain1 = 0;
		};

		String^ GetEEGLabelFor20EEGFromIndex(int index);
		String^ GetEEGLabelFor64EEGFromIndex(int index);


		///<summary>DataGenerator for Syncstation</summary>
		public ref class DataGeneratorSyncstation : public DataGeneratorInterface
		{
		public:
			//device specify if the device at the index position is connected to the Syncstation
			//index - device
			//0		- MUOVI1
			//1		- MUOVI2
			//2		- MUOVI3
			//3		- MUOVI4
			//4		- SESSANTAQUATTRO1/MUOVI+
			//5		- SESSANTAQUATTRO2/MUOVI+
			//6		- DUEPROBE1
			//7		- DUEPROBE2
			//8		- DUEPROBE3
			//9		- DUEPROBE4
			//10	- DUEPROBE5
			//11	- DUEPROBE6
			//12	- DUEPROBE7
			//13	- DUEPROBE8
			//14	- DUEPROBE9
			//15    - DUEPROBE10

			String^ Latency = "200";

			array<Device^>^ devices = gcnew array<Device^>(22); //16 devices + 4 aux channels

			array<__int32>^ deviceEN = gcnew array<__int32>(16); 

			//EEG 500Hz  24bit
			//EMG 2000Hz 16bit
			array<__int32>^ EMG = gcnew array<__int32>(16); //This variable specify the acquisition mode of the device at the specific index (0 = EEG, 1= EMG)
			array<__int32>^ mode = gcnew array<__int32>(16); //This variable specify the modality in which data are send (0=standard, 1=remove average, 2=impedance check, 3=test mode)

			event OTComm::OpenDevice^ OnOpenDevice;
			void FireOpenDevice() { OnOpenDevice(this, gcnew System::EventArgs()); }
			~DataGeneratorSyncstation();
			!DataGeneratorSyncstation();
			///<param name="model">Model of device</param>
			///<param name="SamplingFrequency">Sampling Frequency</param>
			DataGeneratorSyncstation(Model model, Frequency SamplingFrequency, bool HighRes);
			DataGeneratorSyncstation(Frequency SamplingFrequency, bool HighRes);
			///<summary>get DataGeneratoSyncstation from xml</summary>
			///<param name="Filename">path to XML file</param>
			///<returns>DataGeneratorEMGUSB</returns>
			static DataGeneratorSyncstation^ FromXMLConfiguration(String^ Filename);
			static DataGeneratorSyncstation^ FromXMLConfigurationForSetup(String^ Filename);
			unsigned int DataGeneratorSyncstation::GetPositionFromAdapterPositionAndDeviceArray(int adPosition);
			unsigned int DataGeneratorSyncstation::GetPositionForAUXSync();
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
			unsigned int GetNumberConnectableMuovi();
			unsigned int GetNumberConnectableSessantaquattro();
			unsigned int GetNumberConnectableDue();


			unsigned int GetNumberOfChannelsConnectedMuovi();
			unsigned int GetNumberOfChannelsConnectedSessantaquattro();
			unsigned int GetNumberOfChannelsConnectedDue();

			void ConnectControl(const unsigned int port, AdapterQuaternions^ adapter);


			///<summary>get acquired channels</summary>
			///<returns>array of acquired channels</returns>
			array<unsigned int>^ GetAcquiredChannels();
			void DataGeneratorSyncstation::UpdateNumberOfConnectedDevice();
			///<summary>Connect Muovi device to syncstation</summary>
			///<param name="port">aux connector to fill</param>
			///<param name="adapter">AdapterAux to connect</param>
			void ConnectMuovi(const unsigned int port, unsigned int deviceIndex, AdapterMuoviDevice^ adapter);
			///<summary>Connect Sessantaquattro device to syncstation</summary>
			///<param name="port">aux connector to fill</param>
			///<param name="adapter">AdapterAux to connect</param>
			void ConnectSessantaquattro(const unsigned int port, unsigned int deviceIndex, AdapterSessantaquattroDevice^ adapter);
			///<summary>Connect Due device to syncstation</summary>
			///<param name="port">aux connector to fill</param>
			///<param name="adapter">AdapterAux to connect</param>
			void ConnectDue(const unsigned int port, unsigned int deviceIndex, AdapterDueDevice^ adapter);
			void ConnectLoadCell(const unsigned int port, AdapterLoadCell^ adapter);
			void ConnectAux(const unsigned int port, AdapterAux^ adapter);
			void ConnectSyncstationAux(const unsigned int port, AdapterSyncstationAux^ adapter);
			void ConnectControl(const unsigned int port, AdapterControl^ adapter);
			void ConnectMuoviPiu(const unsigned int port, unsigned int deviceIndex, AdapterMuoviPiuDevice^ adapter);
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

			void CreateConnector(int numberOfMuovi, int numberOfSessantaquattro, int numberOfDue);
			void CreateConnectorForMuovi(int numberOfMuovi);
			void CreateConnectorForSessantaquattro(int numberOfSessantaquattro);
			void CreateConnectorForDue(int numberOfDue);

			///<summary>set ip address for Syncstation</summary>
			///<param name="IPAddress">IP Address of Syncstation</param>
			void SetIPAddress(String^ IPAddress);
			///<summary>set ip port for Syncstation</summary>
			///<param name="IPPort">IP Port of Syncstation</param>
			void SetIPPort(unsigned int IPPort);

			void SetNumberOfMuoviConnected(int val);
			void SetNumberOfSessantaquattroConnected(int val);
			void SetNumberOfDueConnected(int val);
			int GetNumberOfMuoviConnected();
			int GetNumberOfSessantaquattroConnected();
			int GetNumberOfDueConnected();
			bool GetHighRes();
			void UpdateHighRes(bool HighRes);


			static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);
			static GenericChannel^ GetChannelFromString(String^ text);
			virtual property DataValidator^ DefaultTriggerValidator
			{
				DataValidator^ get() override;
			}

			int GetDeviceArrayValue(int i);
			void SetDeviceArrayValue(int i, int val);
			void RemoveMuovi(int i);
			void RemoveSessantaquattro(int i);
			void RemoveDue(int i);
			void RemoveDevice(int i);

			unsigned int connectedMuovi = 0;
			unsigned int connectedSessantaquattro = 0;
			unsigned int connectedDue = 0;
		protected:
			array<GenericChannel^>^ mAcquiredChannels;
			void CloseConnection();
			int WriteString(String^ command);
			int ReadString(String^% read);
			int ReadDeviceID(UCHAR* dev_id);
			int GetFirmwareVersion(String^% version);
			Model mModel;
			void  OuternThread();
			void InternalStopSyncstationDataTransfer(void);
			void SendLatencyCommand();
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
			const unsigned int mDefaultPortNumber = 54320;


			unsigned int mPortNumber = mDefaultPortNumber;
			unsigned mAnalogOutputConnector = 0;
			unsigned mAnalogOutputChannel = 0;
			unsigned mAnalogOutputGain = 0;
			String^ mDefaultIPAdress = "192.168.76.1";
			String^ mIPAdress;
			array<Byte>^ CreateStopCommand();
			array<Byte>^ CreateStartCommand();
			array<Byte>^ CreateRaiseTriggerCommand();
			array<Byte>^ CreateFallTriggerCommand();
			array<Byte>^ CreateStartStopCommands(bool Start, bool TriggerOn);
			array<Byte>^ CreateLatencyCommand();
			static Byte ComputeCRC(array<Byte>^ command, unsigned int Len);
			unsigned int RecognizeAdapterForCommand(String^ adapterID);
			unsigned int RecognizeSensorForCommand(String^ channelID);
			unsigned int RecognizeSideForCommand(String^ sideID);
			unsigned int RecognizeHPForCommand(unsigned int highpass);
			unsigned int RecognizeLPForCommand(unsigned int lowpass);
		};

#pragma unmanaged
		ULONG _stdcall AsyncIoSyncstation(PVOID  ThreadParameter);

	}

}