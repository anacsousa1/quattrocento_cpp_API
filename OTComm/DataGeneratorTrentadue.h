#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "DataGeneratorInterface.h"
#include "Sensors.h" 
#include "AdapterTrentadue.h"

using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;
namespace OTComm {
	namespace Trentadue
	{
		typedef unsigned int Frequency;
		///<summary>Frequencies available</summary>
		public ref class Frequencies abstract
		{
		public:
			static const Frequency Trentadue_250Hz = 250;
			static const Frequency Trentadue_500Hz = 500;
			static const Frequency Trentadue_1000Hz = 1000;
			static const Frequency Trentadue_2000Hz = 2000;
		};

		typedef unsigned int AUXType;
		///<summary>AUX Types Available</summary>
		public ref class AUXTypes abstract
		{
		public:
			static const AUXType None = 0;
			static const AUXType AUX = 1;
			static const AUXType Buffer = 2;
		};

		typedef unsigned int Model;
		///<summary>Models available</summary>
		public ref class Models abstract
		{
		public:
			static const Model Trentadue_8 = 8;
			static const Model Trentadue_16 = 16;
			static const Model Trentadue_32 = 32;

			static const Model TrentadueMonopolar_8 = 30008;
			static const Model TrentadueMonopolar_16 = 30016;
			static const Model TrentadueMonopolar_32 = 30032;
			static const Model TrentadueBipolar_8 = 8;
			static const Model TrentadueBipolar_16_Jack = 10016;
			static const Model TrentadueBipolar_16_Wire = 20016;
		};

		
		
		/*enum TrentadueAdapters
		{
			AD8x1TR = 0, AD16x1TR_JACK = 1, AD16x1TR_WIRE = 2, GenericMonopolar = 3
		};
*/
///<summary>DataGenerator for Trentadue</summary>
		public ref class DataGeneratorTrentadue : public DataGeneratorInterface
		{
		public:
			~DataGeneratorTrentadue();
			!DataGeneratorTrentadue();
			///<param name="adapter">Adapter connected to the device</param>
			///<param name="aux">AUX Type</param>
			///<param name="SamplingFrequency">Sampling Frequency</param>
			DataGeneratorTrentadue(AdapterTrentadue^ adapter, AUXType aux, Frequency SamplingFrequency);
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
			virtual void InitializeDevice() override;
			///<summary>Start acquisition of device</summary>
			virtual void StartAcquisition(void) override;
			///<summary>Stop acquisition of device</summary>
			virtual void StopAcquisition(void) override;
			///<summary>Close device</summary>
			virtual void CloseDevice(void) override;
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
			property AUXType AuxType
			{
				AUXType get();
			}
			///<summary>get acquired channels</summary>
			///<returns>array of acquired channels</returns>
			array<unsigned int>^ GetAcquiredChannels();
			///<summary>get DataGeneratoQuattrocento from xml</summary>
			///<param name="Filename">path to XML file</param>
			///<returns>DataGeneratorEMGUSB</returns>
			static DataGeneratorTrentadue^ FromXMLConfiguration(String^ Filename);
			///<summary>Get image of device</summary>
			///<returns>image of device</returns>
			virtual property Bitmap^ Image { Bitmap^ get() override; }


		protected:
			AUXType mUseAux;
			void CloseConnection();
			int WriteString(String^ command);
			int ReadString(String^% read);
			UCHAR ReadDeviceID(void);
			int GetFirmwareVersion(String^% version);
			Model mModel;
			void  OuternThread();
			void InternalStopTrentadueDataTransfer(void);
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
			int mPortNumber = 9760;
			String^ mIPAdress = "192.168.1.1";
			static AdapterTrentadue^ GetAdapterFromID(String^ adapterID);
			static GenericChannel^ GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription);

		};

#pragma unmanaged
		ULONG _stdcall AsyncIoTrentadue(PVOID  ThreadParameter);
	}
}