#pragma once
#include <winsock2.h>
//#include <ws2tcpip.h>
#include <ws2bth.h>
#include "DataGeneratorInterface.h"
#include "Sensors.h" 
#include "AdapterFormeter.h"

#define CXN_BDADDR_STR_LEN                17   // 6 two-digit hex values plus 5 colons
#define CXN_MAX_INQUIRY_RETRY             3
#define CXN_DELAY_NEXT_INQUIRY            15
#define CXN_SUCCESS                       0
#define CXN_ERROR                         -91
#define CXN_ERROR_NOTFOUND                -92
#define CXN_DEFAULT_LISTEN_BACKLOG        -94
#define CXN_ERROR_SOCKETNOTCREATED        -95
#define CXN_ERROR_CANNOTCONNECT           -96
#define CXN_ERROR_WSAFAILEDTOINITIALIZE   -97



using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
namespace OTComm {
	namespace Formeter
	{
		typedef unsigned int Frequency;
		///<summary>Frequencies available</summary>
		public ref class Frequencies abstract
		{
		public:
			static const Frequency Formeter_800Hz = 800;
		};
		
		typedef unsigned int Model;
		///<summary>Models available</summary>
		public ref class Models abstract
		{
		public:
			static const Model FormeterStandard = 80004;
			static const Model FormeterStandard16Bit = 160004;
			static const Model FormeterAutoDetect = 10004;

		};
		
		///<summary>DataGenerator for Formeter</summary>
		public ref class DataGeneratorForemg : public DataGeneratorInterface
		{
		public:
			~DataGeneratorForemg();
			!DataGeneratorForemg();
			///<param name="adapter">Adapter connected to the device</param>
			///<param name="aux">AUX Type</param>
			///<param name="SamplingFrequency">Sampling Frequency</param>
			DataGeneratorForemg();
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
			virtual void OpenDevice(String ^RemoteName, Model DeviceModel);
			///<summary>Initialize device</summary>
			virtual void InitializeDevice() override;
			///<summary>Start acquisition of device</summary>
			virtual void StartAcquisition(void) override;
			///<summary>Stop acquisition of device</summary>
			virtual void StopAcquisition(void) override;
			///<summary>Close device</summary>
			virtual void CloseDevice(void) override;
			void ConnectIn(AdapterFormeter^ adapter);
			///<summary>get acquired channels</summary>
			///<returns>array of acquired channels</returns>
			array<unsigned int>^ GetAcquiredChannels();
			///<summary>get DataGeneratoQuattrocento from xml</summary>
			///<param name="Filename">path to XML file</param>
			///<returns>DataGeneratorEMGUSB</returns>
			static DataGeneratorForemg^ FromXMLConfiguration(String^ Filename);
			///<summary>Get image of device</summary>
			///<returns>image of device</returns>
			virtual property Bitmap^ Image { Bitmap^ get() override; }
			static bool IsFormeterAvailable(String ^RemoteName, bool ForceFlush);
			static List<String^>^ ListPairedDevices(String^ RemoteName, bool ForceFlush);
			int GetFirmwareVersion(String^% version);
			int GetBatteryLevel(String^% bettrylevel);
			void SetDemoMode();
			static String^ UpdateFirmware(String^ filenamepath);


		protected:
			bool mDemoMode;
			static int FindFormeterDevice(String^ RemoteName, SOCKADDR_BTH& RemoteBtAddr, bool ForceFlush, bool TestConnection);
			void CloseConnection();
			int WriteByte(char Command);
			int ReadBytes(char* buffer, int lenght);
			int ReadDeviceID(UCHAR* dev_id);
			Model mModel;
			void  OuternThread();
			void InternalStopFormeterDataTransfer(void);
			volatile bool mMustStopAcquisitionThread;
			HANDLE mAsyncIoThreadHandle;
			bool mDeviceIsOpen;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			unsigned int GetNumberAuxChannels();
			unsigned int GetNumberTotalChannels();
			unsigned int GetNumberNonAuxChannels();
			array<unsigned int>^ CreateChannelsMapping();
			//NetworkStream^	mNetworkStream;	// NetworkStream through ServerSocket to send/receive data
		private:
			static AdapterFormeter^ GetAdapterFromID(String^ adapterID);
			static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);
			ULONG CreateSocketForDevice();
		};

		
#pragma unmanaged
		ULONG _stdcall AsyncIoFormeter(PVOID  ThreadParameter);

	}
}
