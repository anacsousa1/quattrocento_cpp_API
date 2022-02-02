#pragma once
namespace OTComm
{
	ref class DataConsumerInterface;
	//ref class DataGeneratorInterface;
	ref class GenericAdapter;
	ref class GenericConnector;
	ref class DataGeneratorInterface;
	ref class DataValidator;


	ref class ErrorDescription;

	public ref class OTData
	{
	public:
		static unsigned __int32 ID = 0;
		array<unsigned __int8>^data8;
		array<unsigned __int16>^data16;
		array<unsigned __int32>^data32;
		unsigned __int16 channels;
		unsigned __int32 length;
		unsigned __int16 source;

		OTData(unsigned __int16 Source, array<unsigned __int8>^Data, __int16 Channels, __int32 Length)
		{
			ID++;
			data8 = Data;
			data16 = nullptr;
			data32 = nullptr;
			channels = Channels;
			length = Length;
			source = Source;
		}

		OTData(unsigned __int16 Source, array<unsigned __int16>^Data, __int16 Channels, __int32 Length)
		{
			ID++;
			data8 = nullptr;
			data16 = Data;
			data32 = nullptr;
			channels = Channels;
			length = Length;
			source = Source;
		}

		OTData(unsigned __int16 Source, array<unsigned __int32>^Data, __int16 Channels, __int32 Length)
		{
			ID++;
			data8 = nullptr;
			data16 = nullptr;
			data32 = Data;
			channels = Channels;
			length = Length;
			source = Source;
		}

	};

	public ref class NewDataAvailableEventArgs : public System::EventArgs
	{
	public:
		OTData^ Data;
		OTData^ Extra;
		__int32 AuxiliaryInfo;
		NewDataAvailableEventArgs(unsigned __int16 Source, array<unsigned __int8>^Data, __int8 Channels, array<unsigned __int8>^ExtraData, __int16 ExtraChannels, __int32 TemporalAcquisitions, __int32 AuxInfo)
		{
			this->Data = gcnew OTData(Source, Data, Channels, TemporalAcquisitions*Channels);
			this->Extra = gcnew OTData(Source, ExtraData, ExtraChannels, TemporalAcquisitions*ExtraChannels);
			this->AuxiliaryInfo = AuxInfo;
		}
		NewDataAvailableEventArgs(unsigned __int16 Source, array<unsigned __int16>^Data, __int16 Channels, array<unsigned __int16>^ExtraData, __int16 ExtraChannels, __int32 TemporalAcquisitions, __int32 AuxInfo)
		{
			this->Data = gcnew OTData(Source, Data, Channels, TemporalAcquisitions*Channels);
			this->Extra = gcnew OTData(Source, ExtraData, ExtraChannels, TemporalAcquisitions*ExtraChannels);
			this->AuxiliaryInfo = AuxInfo;
		}
		NewDataAvailableEventArgs(unsigned __int16 Source, array<unsigned __int32>^Data, __int16 Channels, array<unsigned __int32>^ExtraData, __int16 ExtraChannels, __int32 TemporalAcquisitions, __int32 AuxInfo)
		{
			this->Data = gcnew OTData(Source, Data, Channels, TemporalAcquisitions*Channels);
			this->Extra = gcnew OTData(Source, ExtraData, ExtraChannels, TemporalAcquisitions*ExtraChannels);
			this->AuxiliaryInfo = AuxInfo;
		}


	};

	public ref class DeviceChangedEventArgs : public System::EventArgs
	{
	public:
		System::String^ DeviceChangedDescription;
		int DeviceChangedCode;

		DeviceChangedEventArgs(System::String^ Description, int Code)
		{
			DeviceChangedCode = Code;
			DeviceChangedDescription = Description;
		}
	};

	public enum DataSize
	{
		EightBits = 8,
		SixteenBits = 16,
		ThirtytwoBits = 32,
	};

	typedef unsigned char DataType;
	///<summary>Frequencies available</summary>
	public ref class DataTypes abstract
	{
	public:
		static const DataType Auto = 0;
		static const DataType EightBits = 8;
		static const DataType SixteenBits = 16;
		static const DataType ThirtytwoBits = 32;
	};



	


	///<summary>Delegate function when device opened</summary>
	public delegate void OpenDevice(System::Object^ sender, System::EventArgs^ e);
	///<summary>Delegate function when error</summary>
	public delegate void Error(System::Object^ sender, System::EventArgs^ e);
	///<summary>Delegate function when device initialized</summary>
	public delegate void InitializeDevice(System::Object^ sender, System::EventArgs^ e);
	///<summary>Delegate function when start acquisition</summary>
	public delegate void StartAcquisition(System::Object^ sender, System::EventArgs^ e);
	///<summary>Delegate function when new data available</summary>
	public delegate void NewDataAvailable(System::Object^ sender, System::EventArgs^ e);
	///<summary>Delegate function when device changed</summary>
	public delegate void DeviceChanged(System::Object^ sender, System::EventArgs^ e);
	///<summary>Delegate function when stop acquisition</summary>
	public delegate void StopAcquisition(System::Object^ sender, System::EventArgs^ e);
	///<summary>Delegate function when requested to stop acquisition is sent to HW</summary>
	public delegate void StopAcquisitionRequested(System::Object^ sender, System::EventArgs^ e);
	///<summary>Delegate function when stop initialization</summary>
	public delegate void StopInitialization(System::Object^ sender, System::EventArgs^ e);
	///<summary>Delegate function when close device</summary>
	public delegate void CloseDevice(System::Object^ sender, System::EventArgs^ e);

	///<summary>Managed delegate function when error</summary>
	public delegate void MarshalError(char* description, int code, char* function);
	///<summary>Managed delegate function when open device</summary>
	public delegate void MarshalOpenDevice();
	///<summary>Managed delegate function when initialize device</summary>
	public delegate void MarshalInitializeDevice();
	///<summary>Managed delegate function when start acquisition</summary>
	public delegate void MarshalStartAcquisition();
	///<summary>Managed delegate function when new data available</summary>
	public delegate void MarshalNewDataAvailable(unsigned __int16 sourceindex, DataSize datasize, void* data, void* Extradata, __int16 Channels, __int16 ExtraChannels, int TemporalAcquisitions, __int32 AuxiliaryInfo);
	///<summary>Managed delegate function when device changed</summary>
	public delegate void MarshalDeviceChanged(int code, char* description, int length);
	///<summary>Managed delegate function when stop acquisition</summary>
	public delegate void MarshalStopAcquisition();
	///<summary>Managed delegate function when stop acquisition</summary>
	public delegate void MarshalStopAcquisitionRequested();
	///<summary>Managed delegate function when stop initialization</summary>
	public delegate void MarshalStopInitialization();
	///<summary>Managed delegate function when close device</summary>
	public delegate void MarshalCloseDevice();
	typedef void(__stdcall *MARSHAL_ERROR)(char* description, int code, char* function);
	typedef void(__stdcall *MARSHAL_OPENDEVICE)();
	typedef void(__stdcall *MARSHAL_INITIALIZEDEVICE)();
	typedef void(__stdcall *MARSHAL_STARTACQUISITION)();
	typedef void(__stdcall *MARSHAL_NEWDATAAVAILABLE)(unsigned __int16 sourceindex, DataSize datasize, void* data, void* extradata,  __int16 Channels, __int16 ExtraChannels, int TemporalAcquisitions, unsigned __int32 AuxInfo);
	typedef void(__stdcall *MARSHAL_DEVICECHANGED)(int code, char* description, int length);
	typedef void(__stdcall *MARSHAL_STOPACQUISITION)();
	typedef void(__stdcall *MARSHAL_STOPACQUISITIONREQUESTED)();
	typedef void(__stdcall *MARSHAL_STOPINITIALIZATION)();
	typedef void(__stdcall *MARSHAL_CLOSEDEVICE)();



}