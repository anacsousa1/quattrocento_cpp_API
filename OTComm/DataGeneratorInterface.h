#pragma once


#include "windows.h"
#include "Utilities.h"
#include "GenericAdapter.h"
#include "GenericConnector.h"
#include "ErrorDescription.h"
#include "GenericException.h"
#include "OTCommTypes.h"

using namespace System;
using namespace System::Threading;

#define WHILE(a) while(__pragma(warning(disable:4127)) a __pragma(warning(disable:4127)))
#define countof(x) (sizeof(x) / sizeof(x[0]))
#define DEVICE(x)	(((unsigned int)x) / 10000)
#define MAX_DEVPATH_LENGTH 256

namespace OTComm
{
	public class Device {

	public:
		int mode = 0;
		int enable = 0;
		int nSample = 0;
		int nChannel = 0;
	};


	public ref class DataGeneratorSingleSource
	{
	public:
		unsigned int mSourceTotalChannels;
		array<unsigned int>^ mAcquiredChannelsIndices;
		unsigned int mSamplingFrequency;
		unsigned __int16 mBytesPerChannel;
		array<unsigned int>^ mMapping;
		DataGeneratorSingleSource(int SamplingFrequency, __int16 BytesPerChannel, array<unsigned int>^ mapping);
		void UpdateAcquiredChannels(array<unsigned int>^ indices);
		//unsigned __int16 mDataRegroupFactor;
		
	};

	///<summary>Abstract interface for DataGenerator</summary>
	public ref class DataGeneratorInterface abstract
	{
	protected:
		AsyncCallback^ mAsyncCallback;
		List<NewDataAvailable^>^ onNewDataAvailableAsyncHandlers;

		//List<OpenDevice^>^ onNewOpenDeviceHandlers;

		//DataGeneratorInterface^ mDataGeneratorInterface;
		array<unsigned __int8>^mDataToDispatchSync8;
		array<unsigned __int8>^mExtraDataToDispatchSync8;
		array<array<unsigned __int8>^>^mDataToDispatchAsync8;

		array<unsigned __int16>^mDataToDispatchSync16;
		array<unsigned __int16>^mExtraDataToDispatchSync16;
		array<array<unsigned __int16>^>^mDataToDispatchAsync16;

		array<unsigned __int32>^mDataToDispatchSync32;
		array<unsigned __int32>^mExtraDataToDispatchSync32;
		array<array<unsigned __int32>^>^mDataToDispatchAsync32;

		unsigned int mCurrentDataToDispatchIndex;
		unsigned int mDataToDispatchAsyncCurrpos;
		void Callback(IAsyncResult^ ar);
		MarshalError^ mMarshalError;
		MarshalNewDataAvailable^ mMarshalNewDataAvailable;
		MarshalDeviceChanged^ mMarshalDeviceChanged;
		MarshalNewDataAvailable^ mMarshalNewDataAvailableAsync;
		MarshalStopAcquisition^ mMarshalStopAcquisition;
		MarshalStopAcquisitionRequested^ mMarshalStopAcquisitionRequested;
		MarshalStopInitialization^ mMarshalStopInitialization;
	internal:
		void FireOnError(Exception^ exc);
		void FireOnError(char* description, int code, char* function);
		void FireOnOpenDevice();
		void FireOnInitializeDevice();
		void FireOnStartAcquisition();
		void FireOnNewDataAvailable(unsigned __int16 sourceindex, DataSize datasize, void* data, void* extradata, __int16 Channels, __int16 ExtraChannels, int TemporalAcquisitions, __int32 AuxInfo);
		void FireOnDeviceChanged(int code, char* description, int length);
		void FireOnStopAcquisitionRequested();
		void FireOnStopAcquisition();
		void FireOnStopInitialization();
		void FireOnCloseDevice();

		MARSHAL_ERROR GetFireOnError();
		MARSHAL_NEWDATAAVAILABLE   GetFireOnNewDataAvailable();
		MARSHAL_DEVICECHANGED GetFireDeviceChanged();
		MARSHAL_STOPACQUISITION GetFireOnStopAcquisition();
		MARSHAL_STOPACQUISITIONREQUESTED GetFireOnStopAcquisitionRequested();
		MARSHAL_STOPINITIALIZATION GetFireOnStopInitialization();

	public:
		event Error^ OnError;
		event InitializeDevice^ OnInitializeDevice;
		event StartAcquisition^ OnStartAcquisition;
		event NewDataAvailable^ OnNewDataAvailable;
		event DeviceChanged^ OnDeviceChanged;
		event StopAcquisition^ OnStopAcquisition;
		event StopAcquisitionRequested^ OnStopAcquisitionRequested;
		event StopInitialization^ OnStopInitialization;
		event CloseDevice^ OnCloseDevice;
		event OpenDevice^ OnOpenDevice;		

		//I need to manually write the event since I need an Async call in the raise method
		event NewDataAvailable^ OnNewDataAvailableAsync
		{
			//add a callback delegate to the events list
			void add(NewDataAvailable ^ newHandler) {
				onNewDataAvailableAsyncHandlers->Add(newHandler);
			}
			//remove a callback delegate to the events list
			void remove(NewDataAvailable ^ Handler) {
				onNewDataAvailableAsyncHandlers->Remove(Handler);
			}
			//raise the event. 
			void raise(Object^ dri, EventArgs^ e)
			{
				if (onNewDataAvailableAsyncHandlers != nullptr)
				{
					bool fireEvent = false;
					for each(NewDataAvailable^ handler in onNewDataAvailableAsyncHandlers)
					{
						fireEvent = !fireEvent;
						if (fireEvent)
						{
							//handler(dri, data, length);
							handler->BeginInvoke(dri, e, mAsyncCallback, nullptr);
						}
					}
				}
			}
		}

	public:
		DataGeneratorInterface();
		///<summary> Creates DataGenerator from XML file</summary>
		///<param name="Filename">Path to XML file</param>
		///<returns>DataGeneratorInteface corresponding to XML file</returns>
		static DataGeneratorInterface^ FromXMLConfiguration(String^ Filename);
		///<summary>Asynchronous regroup factor for New Data Event</summary>
		///<param name="x">Asynchronous regroup factor</param>
		///<returns>Asynchronous regroup factor</returns>
		property unsigned short AsyncNewDataEventRegroupFactor
		{
			unsigned short get();
			void set(unsigned short x);
		}

		///<summary>Get the number of independent sources. This number is typically 1, it can be larger than 1 in aggregated devices</summary>
		///<returns>Total number of sources</returns>
		unsigned int GetNumberSources();
		///<summary>Get the number of channels available in the device</summary>
		///<returns>Total number of Device</returns>
		unsigned int GetDeviceTotalChannels();
		///<summary>Get the model of the Device</summary>
		///<returns>Model of the Device</returns>
		virtual unsigned int GetDeviceModel();
		///<summary>Get the number of channels acquired by the device</summary>
		///<returns>Array with the index of acquired channels</returns>
		array<unsigned int>^  GetAcquiredChannels();
		array<unsigned int>^  GetAcquiredChannelsSources();
		///<summary>Get the sampling frequency of the device</summary>
		///<returns>Sampling frequency of Device</returns>
		unsigned int GetSamplingFrequency();
		///<summary>Get the number of bytes per channel acquired by the device</summary>
		///<returns>the number of bytes per channel acquired by the device</returns>
		unsigned short GetBytesPerChannel();

		DataGeneratorSingleSource^ GetSource(int sourceIndex);
		///<summary>Get the ID of the device</summary>
		///<returns>the ID of the device</returns>
		virtual int GetDeviceID();
		/// <summary>Shows if the Device is open</summary>
		/// <returns> true if Device is open, false if it is close </returns>
		virtual bool IsOpen();
		/// <summary>Regroup factor for having new data available</summary>
		/// <returns> regroup factor </returns>
		virtual property unsigned short NewDataAvailableEventsPerSecond
		{
			unsigned short get();
			void set(unsigned short x);
		}

		/// <summary>Get description of Device</summary>
		/// <returns> device description </returns>
		virtual String^ GetDeviceDescription() = 0;
		/// <summary>Get XML Configuration of Device</summary>
		/// <returns> device XML configuration </returns>
		virtual String^ GetDeviceXMLConfiguration() = 0;
		/// <summary>Get XML configuration of saved data</summary>
		/// <returns> saved data XML configuration </returns>
		virtual String^ GetSavingXMLConfiguration() = 0;

		/// <summary>Open the device</summary>
		virtual void OpenDevice(void);
		/// <summary>Initialize the device</summary>
		virtual void InitializeDevice(void);
		/// <summary>Start acquisition from the device</summary>
		virtual void StartAcquisition(void);
		//NB NewData is called from the functions receiving the data, called by StartAcquisition
		/// <summary>Stop acquisition from the device</summary>
		virtual void StopAcquisition(void);
		/// <summary>Stop initialization the device</summary>
		virtual void StopInitialization(void);
		/// <summary>Close the device</summary>
		virtual void CloseDevice(void);

		/// <summary>
		/// Add data consumer to generator
		/// </summary>
		/// <param name="du">DataConsumerInterface containing the consumer</param>
		void AddDataConsumer(DataConsumerInterface^ du);
		void AddDataConsumer(DataConsumerInterface^ du, __int16 sourceindex);

		/// <summary>Image of the device</summary>
		/// <returns>image of the device</returns>
		virtual property Bitmap^ Image { Bitmap^ get(); }

		/// <summary>Get Connectors of Device</summary>
		/// <returns>Array of GenericConnectors available for the device</returns>
		array<GenericConnector^>^ GetConnectors();

		/// <summary>Get if Data Generator is acquiring</summary>
		/// <returns>True if data generator is acquiring, false if it is not acquiring</returns>
		virtual property bool IsAcquiring
		{
			bool get();
		}

		virtual property bool TestMode
		{
			bool get();
			void set(bool x);
		}
		
		property DataValidator^ DefaultTriggerValidator
		{
			virtual DataValidator^ get();
		}


	protected:

		unsigned int mDeviceTotalChannels;		
		array<unsigned int>^ mAcquiredChannelsIndices;	
		array<unsigned int>^ mAcquiredChannelsSources;

		array<DataGeneratorSingleSource^>^ mSources;
		//unsigned int mSamplingFrequency;		unsigned short mBytesPerChannel;
		//array<unsigned int>^ mMapping;

		Thread^ mManagedAcquisitionThread;
		array<GenericConnector^>^ mConnectors;
		void OpenXMLConfigStream(const bool FillWithDefault);
		String^ CloseXMLConfigStream();
		System::IO::StringWriter^ mStringWriter;
		System::Xml::XmlTextWriter^ mXMLTextStream;
		bool mXMLFill;
		HANDLE mDeviceHandle;
		HANDLE mCompletionPortHandle;
		void NotifyError(Exception^ e);
		unsigned short mAsyncRegroupFactor;
		//unsigned short mAcquisitionRegroupFactor;
		unsigned short mNewDataAvailableEventsPerSecond;
		int mMustStopAcquisition;
		int mMustRaiseTrigger;
		int mMustFallTrigger;
		int mMustStopInitialization;
		bool mTestMode;

	internal:
		virtual array<unsigned int>^ GetChannelsMapping();
		HANDLE GetDeviceHandle();
		HANDLE GetCompletionPortHandle();

	};



	class DataGeneratorInterfaceHelperSingleSource
	{
	public:
		unsigned int mSourceTotalChannels;
		unsigned int mSamplingFrequency;
		unsigned __int16 mBytesPerChannel;
		unsigned __int16 mAcquiredChannels;
		unsigned __int16 mDataRegroupFactor;
		unsigned __int16 *mMapping;
		unsigned int mAuxData;
		LPARAM mCustomData;
	};

	class DataGeneratorInterfaceHelper
	{
	public:

		DataGeneratorInterfaceHelperSingleSource *mSource;
		HANDLE mDeviceHandle;
		HANDLE mCompletionPortHandle;
		Device devices[22];
		unsigned int mDeviceTotalChannels;
		unsigned int mDeviceModel;		
		unsigned __int16 mDeviceSources;
		unsigned __int16 mNewDataAvailableEventsPerSecond;		
		unsigned int mAuxData;
		LPARAM mCustomData;
		LPARAM mRaiseTriggerCommand;
		LPARAM mFallTriggerCommand;
		LPARAM mStop64Command;
		int*mMustStopAcquisitionPtr;
		int*mMustRaiseTriggerPtr;
		int*mMustFallTriggerPtr;
		OTComm::MARSHAL_NEWDATAAVAILABLE mNewDataAvailable;
		OTComm::MARSHAL_DEVICECHANGED mDeviceChanged;
		OTComm::MARSHAL_ERROR mError;
		OTComm::MARSHAL_STOPACQUISITION mStopAcquisition;
		OTComm::MARSHAL_STOPINITIALIZATION mStopInitialization;
		DataGeneratorInterfaceHelper(DataGeneratorInterface^ dri);
		~DataGeneratorInterfaceHelper();
		char * ipAddress;
	};
	/*
	public class DataGeneratorInterfaceHelperSyncstation :DataGeneratorInterfaceHelper
	{
	public:
		array<Device^>^ devices = gcnew array<Device^>(22); //16 devices + 4 aux channels


	};*/

}