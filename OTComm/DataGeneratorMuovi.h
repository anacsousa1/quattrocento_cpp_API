#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "DataGeneratorInterface.h"
#include "Sensors.h"
#include "OTCommTypes.h"
#include "DataValidator.h"
#include "AdapterMuovi.h"
using namespace System::Net;
using namespace System::Net::Sockets;

namespace OTComm {
	namespace Muovi {

		typedef Byte ProbNumber;
		typedef Byte WorkingMode;
		typedef Byte DetectionMode;
		typedef Byte GoStop;
		
		public ref class ProbNumbers abstract
		{
		public:
			static const ProbNumber Prob1 = 0;
			static const ProbNumber Prob2 = 1;
			static const ProbNumber Prob3 = 2;
			static const ProbNumber Prob4 = 3;

		};

		public ref class WorkingModes abstract
		{
		public:
			static const WorkingMode EEG = 0; //fsamp 500Hz
			static const WorkingMode EMG = 1; //fsamp 2000Hz
		};

		public ref class DetectionModes abstract
		{
		public:
			static const DetectionMode Monopolar32Standard = 0;
			static const DetectionMode Monopolar32RemoveAverage = 1;
			static const DetectionMode ImpedenceCheck = 2;
			static const DetectionMode TestMode = 3;
			///<summary>Detection Mode Description</summary>
			///<param name="detectionMode">Detection Mode Code</param>
			///<returns>description of detection mode</returns>
			static String^ Description(const unsigned int detectionMode)
			{
				switch (detectionMode)
				{
				case 0:			return "Monopolar 32 signals + 6 accessory";
				case 1:			return "Monopolar 16 signals + 6 accessory";
				case 2:			return "Bipolar 16 EMG signals + 6 accessory";
				case 3:			return "Bipolar 8 EMG signals + 6 accessory";
				case 4:			return "Differential 32 signals";
				case 5:			return "Differential 16 signals";
				case 6:			return "Impedance check";
				case 7:			return "Test Mode with ramp on 32 channels";
				default:		return "Monopolar";
				}
			}
		};

		public ref class GoStops abstract
		{
		public:
			static const GoStop Stop = 0;
			static const GoStop Go = 1;
		};
		/*
		typedef unsigned int DetectionMode;
		///<summary>Frequencies available</summary>
		public ref class DetectionModes abstract
		{
		public:
			static const DetectionMode Monopolar = 0;
			static const DetectionMode Bipolar = 1;
			static const DetectionMode Differential = 2;
			static const DetectionMode ImpedenceCheck = 6;
			///<summary>Detection Mode Description</summary>
			///<param name="detectionMode">Detection Mode Code</param>
			///<returns>description of detection mode</returns>
			static String^ Description(const unsigned int detectionMode)
			{
				switch (detectionMode)
				{
				case 1:			return "Bipolar";
				case 2:			return "Differential";
				case 6:			return "Impedance check";
				case 0:
				default:		return "Monopolar";
				}
			}
		};*/

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
			static const Frequency Muovi_500Hz = 500;
			static const Frequency Muovi_1000Hz = 1000;
			static const Frequency Muovi_2000Hz = 2000;
			static const Frequency Muovi_4000Hz = 4000;
		};

		typedef unsigned int Model;
		///<summary>Models available</summary>
		public ref class Models abstract
		{
		public:
			static const Model Muovi_1P = 10001;
			static const Model Muovi_2P = 10002;
			static const Model Muovi_3P = 10003;
			static const Model Muovi_4P = 10004;
		};
		///<summary>DataGenerator for Muovi</summary>
		public ref class DataGeneratorMuovi : public DataGeneratorInterface
		{
		public:
			event OTComm::OpenDevice^ OnOpenDevice;
			void FireOpenDevice() { OnOpenDevice(this, gcnew System::EventArgs()); }
			~DataGeneratorMuovi();
			!DataGeneratorMuovi();
			///<param name="model">Model of device</param>
			///<param name="SamplingFrequency">Sampling Frequency</param>
			DataGeneratorMuovi(Model model, Frequency SamplingFrequency, bool HighRes, bool RemoveAverage);
			///<summary>get DataGeneratoMuovi from xml</summary>
			///<param name="Filename">path to XML file</param>
			///<returns>DataGeneratorEMGUSB</returns>
			static DataGeneratorMuovi^ FromXMLConfiguration(String^ Filename);

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
			///<summary>Get the number of quaternion for one MUOVI</summary>
			///<returns>number of quaternion</returns>
			unsigned int GetNumberAdapterQuaternion();

			///<summary>Connect In to device</summary>
			///<param name="port">in connector to fill</param>
			///<param name="adapter">AdapterIn to connect</param>
			void ConnectIn(const unsigned int port, AdapterMuovi^ adapter);
			void ConnectAux(const unsigned int port, AdapterAux^ adapter);
			void ConnectControl(const unsigned int port, AdapterControl^ adapter);
			void ConnectControl(const unsigned int port, AdapterQuaternions^ adapter);

			bool GetRemoveAverage();

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
			///<summary>set ip address for Muovi</summary>
			///<param name="IPAddress">IP Address of Muovi</param>
			void SetIPAddress(String^ IPAddress);
			///<summary>set ip port for Muovi</summary>
			///<param name="IPPort">IP Port of Muovi</param>
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
			void InternalStopMuoviDataTransfer(void);
			volatile bool mMustStopAcquisitionThread;
			HANDLE mAsyncIoThreadHandle;
			bool mDeviceIsOpen;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			DataValidatorTrigger^ mDefaulDataValidatorTrigger;
			//NetworkStream^	mNetworkStream;	// NetworkStream through ServerSocket to send/receive data
		private:
			static GenericAdapter^ GetAdapterFromID(String^ adapterID);
			//static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);
			const unsigned int mDefaultPortNumber = 54321;
			unsigned int mPortNumber = mDefaultPortNumber;
			unsigned mAnalogOutputConnector = 0;
			unsigned mAnalogOutputChannel = 0;
			unsigned mAnalogOutputGain = 0;
			bool remove_average = false;
			String^ mDefaultIPAdress = "192.168.14.1";
			String^ mIPAdress;
			array<Byte>^ CreateStartCommandForProbe();
			array<Byte>^ CreateStopCommandForProbe();
			array<Byte>^ CreateStopCommandForOtbStation();
			array<Byte>^ CreateImpedenceCheckCommand();
			array<Byte>^ CreateStartCommand();
			array<Byte>^ CreateStopCommand();
			array<Byte>^ CreateGetCommand();
			static Byte ComputeCRC(array<Byte>^ command, unsigned int Len);
		};

		#pragma unmanaged
		ULONG _stdcall AsyncIoMuovi(PVOID  ThreadParameter); 
	}
	
}