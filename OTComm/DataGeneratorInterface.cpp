#include "DataGeneratorInterface.h"
#include "DataConsumerInterface.h"
//#include "DataGeneratorQuattrocento.h"
using namespace System::Reflection;
using namespace System::Xml;
using namespace System::Threading;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Runtime::Remoting::Messaging;

namespace OTComm
{

	DataGeneratorInterfaceHelper::DataGeneratorInterfaceHelper(DataGeneratorInterface^ dri): 
		mAuxData(0), mCustomData(NULL), mCompletionPortHandle(INVALID_HANDLE_VALUE)
	{
		mError = dri->GetFireOnError();
		mNewDataAvailable = dri->GetFireOnNewDataAvailable();
		mDeviceChanged = dri->GetFireDeviceChanged();
		mStopAcquisition = dri->GetFireOnStopAcquisition();
		mStopInitialization = dri->GetFireOnStopInitialization();

		mDeviceHandle = dri->GetDeviceHandle();
		mCompletionPortHandle = dri->GetCompletionPortHandle();
		mDeviceModel = dri->GetDeviceModel();
		mDeviceTotalChannels = dri->GetDeviceTotalChannels();

		mDeviceSources = dri->GetNumberSources();

		mSource = new DataGeneratorInterfaceHelperSingleSource[mDeviceSources];
		for (unsigned __int16 kCh = 0; kCh < mDeviceSources; kCh++)
		{
			DataGeneratorSingleSource^ dgss = dri->GetSource(kCh);
 			mSource[kCh].mSourceTotalChannels = dgss->mSourceTotalChannels;
			mSource[kCh].mSamplingFrequency = dgss->mSamplingFrequency;
			mSource[kCh].mDataRegroupFactor = mSource[kCh].mSamplingFrequency / dri->NewDataAvailableEventsPerSecond;
			mSource[kCh].mBytesPerChannel = dgss->mBytesPerChannel;

			array<unsigned int>^AcquiredChannels = dgss->mAcquiredChannelsIndices;
			mSource[kCh].mAcquiredChannels = dgss->mAcquiredChannelsIndices->Length;
			mSource[kCh].mMapping = new unsigned __int16[mSource[kCh].mAcquiredChannels];
			array<unsigned int>^map = dgss->mMapping;
			if (map == nullptr)
				for (unsigned int i = 0; i < mSource[kCh].mAcquiredChannels; i++)
					mSource[kCh].mMapping[i] = AcquiredChannels[i];
			else
				for (int i = 0; i < AcquiredChannels->Length; i++)
					mSource[kCh].mMapping[i] = map[AcquiredChannels[i]];
		}
	}

	DataGeneratorInterfaceHelper::~DataGeneratorInterfaceHelper()
	{
		if (mSource != NULL)
		{
			for (unsigned int kS = 0; kS < mDeviceSources; kS++)
			{
				if (mSource[kS].mMapping != NULL)
				{
					delete mSource[kS].mMapping;
					mSource[kS].mMapping = NULL;
				}
			}
		}
		if (mCustomData != NULL)
		{
			delete (void*)mCustomData;
			mCustomData = NULL;
		}
	}

	DataGeneratorSingleSource::DataGeneratorSingleSource(int SamplingFrequency, __int16 BytesPerChannel, array<unsigned int>^ mapping) :		
		mSamplingFrequency(SamplingFrequency), mBytesPerChannel(BytesPerChannel),mMapping(mapping)
	{
	}

	void DataGeneratorSingleSource::UpdateAcquiredChannels(array<unsigned int>^ indices)
	{
		mAcquiredChannelsIndices = indices;
	}

	DataGeneratorInterface::DataGeneratorInterface() :
		mDeviceHandle(INVALID_HANDLE_VALUE), mMustStopAcquisition(false), mMustStopInitialization(false),
		mStringWriter(nullptr), mXMLTextStream(nullptr), mXMLFill(false),
		mAsyncRegroupFactor(0),mDeviceTotalChannels(1),
		mNewDataAvailableEventsPerSecond(1), mManagedAcquisitionThread(nullptr),
		mCurrentDataToDispatchIndex(0), mDataToDispatchSync8(nullptr),
		mDataToDispatchSync16(nullptr), mDataToDispatchSync32(nullptr),
		mDataToDispatchAsyncCurrpos(0), mTestMode(false),
		mSources(nullptr), mCompletionPortHandle(INVALID_HANDLE_VALUE)
	{
		const int NUM_CIRCULAR_MANAGED_BUFFERS = 10;
		
		mDataToDispatchAsync8 = gcnew array<array<unsigned __int8>^>(NUM_CIRCULAR_MANAGED_BUFFERS);
		mDataToDispatchAsync16 = gcnew array<array<unsigned __int16>^>(NUM_CIRCULAR_MANAGED_BUFFERS);
		mDataToDispatchAsync32 = gcnew array<array<unsigned __int32>^>(NUM_CIRCULAR_MANAGED_BUFFERS);
		mAsyncCallback = gcnew AsyncCallback(this, &DataGeneratorInterface::Callback);
		onNewDataAvailableAsyncHandlers = gcnew List<NewDataAvailable^>();
		//onNewOpenDeviceHandlers = gcnew List<OpenDevice^>();
		mMarshalError = gcnew MarshalError(this, &OTComm::DataGeneratorInterface::FireOnError);
		mMarshalNewDataAvailable = gcnew MarshalNewDataAvailable(this, &OTComm::DataGeneratorInterface::FireOnNewDataAvailable);
		mMarshalDeviceChanged = gcnew MarshalDeviceChanged(this, &OTComm::DataGeneratorInterface::FireOnDeviceChanged);
		mMarshalStopAcquisition = gcnew MarshalStopAcquisition(this, &OTComm::DataGeneratorInterface::FireOnStopAcquisition);
		mMarshalStopInitialization = gcnew MarshalStopInitialization(this, &OTComm::DataGeneratorInterface::FireOnStopInitialization);
		
	}

	DataValidator^ DataGeneratorInterface::DefaultTriggerValidator::get()
	{
		return gcnew DataValidatorNoCheck();
	}

	DataGeneratorInterface^ DataGeneratorInterface::FromXMLConfiguration(String^ Filename)
	{
		return nullptr;
	}

	void DataGeneratorInterface::OpenXMLConfigStream(bool Fill)
	{
		try
		{
			mStringWriter = gcnew System::IO::StringWriter();

			mXMLTextStream = gcnew System::Xml::XmlTextWriter(mStringWriter);
			mXMLTextStream->Formatting = System::Xml::Formatting::Indented;

			mXMLTextStream->WriteStartDocument();
			mXMLFill = Fill;
			if (mXMLFill)
			{
				mXMLTextStream->WriteStartElement("Device");
				mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
				mXMLTextStream->WriteAttributeString("DeviceTotalChannels", mDeviceTotalChannels.ToString());
			}
		}
		catch (Exception^ e)
		{
			LOG_ERROR("Cannot open XML file");
			XMLOpeningException ^xoe = gcnew XMLOpeningException(__FUNCTION__,e);
		}

	}

	String^ DataGeneratorInterface::CloseXMLConfigStream()
	{
		if (mXMLFill)
			mXMLTextStream->WriteEndElement();//closes the "Device" element opened in OpenXMLConfigStream
		mXMLTextStream->Flush();
		String^ res = mStringWriter->ToString()->Replace("utf-16", "utf-8");
		mXMLTextStream->Close();
		mStringWriter = nullptr;
		mXMLTextStream = nullptr;
		return res;
		//return System::Text::UTF8Encoding::GetString(ms->ToArray());	
	}

	array<GenericConnector^>^ DataGeneratorInterface::GetConnectors()
	{
		return mConnectors;
	}
	
	array<unsigned int>^ DataGeneratorInterface::GetAcquiredChannels()
	{
		
		return mAcquiredChannelsIndices;
	}

	array<unsigned int>^ DataGeneratorInterface::GetAcquiredChannelsSources()
	{
		return mAcquiredChannelsSources;
	}

	unsigned short DataGeneratorInterface::GetBytesPerChannel()
	{
		if (mSources != nullptr)
			return mSources[0]->mBytesPerChannel;
		return 1;
	}

	DataGeneratorSingleSource^ DataGeneratorInterface::GetSource(int sourceIndex)
	{
		if (mSources != nullptr && mSources->Length>sourceIndex)
			return  mSources[sourceIndex];
		return nullptr;
	}

	array<unsigned int>^ DataGeneratorInterface::GetChannelsMapping()
	{
		return nullptr;
	}
	
	unsigned int DataGeneratorInterface::GetNumberSources()
	{
		if (mSources != nullptr)
			return mSources->Length;
		return 0;
	}

	unsigned int DataGeneratorInterface::GetDeviceTotalChannels()
	{
		return mDeviceTotalChannels;
	}

	int DataGeneratorInterface::GetDeviceID()
	{
		return 0;
	}

	bool DataGeneratorInterface::IsOpen()
	{
		return false;
	}

	unsigned short DataGeneratorInterface::AsyncNewDataEventRegroupFactor::get()
	{
		return mAsyncRegroupFactor;
	}

	void DataGeneratorInterface::AsyncNewDataEventRegroupFactor::set(unsigned short RegroupFactor)
	{
		mAsyncRegroupFactor = RegroupFactor;
	}

	unsigned short DataGeneratorInterface::NewDataAvailableEventsPerSecond::get()
	{
		return mNewDataAvailableEventsPerSecond;
	}

	void DataGeneratorInterface::NewDataAvailableEventsPerSecond::set(unsigned short DataAvailableEventsPerSecond)
	{
		mNewDataAvailableEventsPerSecond = DataAvailableEventsPerSecond;
	}
	
	unsigned int DataGeneratorInterface::GetDeviceModel()
	{
		return 0;
	}

	unsigned int DataGeneratorInterface::GetSamplingFrequency()
	{
		if(mSources!=nullptr)
			return mSources[0]->mSamplingFrequency;
		return 1024;
	}

	void DataGeneratorInterface::AddDataConsumer(DataConsumerInterface^ du)
	{
		AddDataConsumer(du, -1);		
	}

	void DataGeneratorInterface::AddDataConsumer(DataConsumerInterface^ du, __int16 sourceindex)
	{
		if(sourceindex>=0 && sourceindex>= GetNumberSources())
			throw gcnew InvalidSourceIndex(__FUNCTION__);			
		du->Register(this, sourceindex);
	}


	void DataGeneratorInterface::NotifyError(Exception^ e)
	{
		FireOnError(e);
	}

	

	bool DataGeneratorInterface::TestMode::get(void)
	{
		return mTestMode;
	}

	void DataGeneratorInterface::TestMode::set(bool testmode)
	{
		mTestMode = testmode;
	}

	Bitmap^ DataGeneratorInterface::Image::get()
	{
		try
		{
			return Utilities::GetImageFromEmbeddedResource("Default.png", true);
		}
		catch (Exception^)
		{
			return nullptr;
		}
	}

	bool DataGeneratorInterface::IsAcquiring::get()
	{
		return mManagedAcquisitionThread != nullptr && mManagedAcquisitionThread->IsAlive;
	}

	HANDLE DataGeneratorInterface::GetDeviceHandle()
	{
		return mDeviceHandle;
	}

	HANDLE DataGeneratorInterface::GetCompletionPortHandle()
	{
		return mCompletionPortHandle;
	}

	void DataGeneratorInterface::OpenDevice(void)
	{
		try
		{
			FireOnOpenDevice();
		}
		catch (Exception^e)
		{
			throw gcnew OpenDeviceFailedException(__FUNCTION__,e);
		}
	}

	void DataGeneratorInterface::InitializeDevice()
	{
		try
		{
			int NumAcquiredChannels = 0;
			if (mConnectors == nullptr)
			{
				throw gcnew InvalidConnectorException(__FUNCTION__);
			}
			for (int k = 0; k < mConnectors->Length; k++)
			{
				if (mConnectors[k]->Adapter != nullptr)
					NumAcquiredChannels += mConnectors[k]->Adapter->NumConnectedChannels;
			}
			if (NumAcquiredChannels == 0)
			{
				throw gcnew NoAdaptersConnectedException(__FUNCTION__);
			}
			mAcquiredChannelsIndices = gcnew array<unsigned int>(NumAcquiredChannels);
			mAcquiredChannelsSources = gcnew array<unsigned int>(NumAcquiredChannels);

			unsigned int pos = 0;
			unsigned int ChannelIndex = 0;
			for (int k = 0; k < mConnectors->Length; k++)
			{
				if (mConnectors[k]->Adapter != nullptr && mConnectors[k]->Adapter->Channels != nullptr)
				{
					for (int c = 0; c < mConnectors[k]->Adapter->Channels->Length; c++)
					{
						if (mConnectors[k]->Adapter->Channels[c] != nullptr)
						{
							mAcquiredChannelsIndices[pos] = ChannelIndex;
							mAcquiredChannelsSources[pos] = mConnectors[k]->SourceIndex;
							pos++;
						}
						ChannelIndex++;
					}
				}
				else
					ChannelIndex += mConnectors[k]->NumConnectableChannels;

			}

			//PB2EB questo codice calcola l'indice del canale nella singola Source
			pos = 0;
			ChannelIndex = 0;
			for (unsigned int kSource = 0; kSource < GetNumberSources(); kSource++)
			{
				unsigned long StartIndexOnSource = 0;
				for (int k = 0; k < mConnectors->Length; k++)
				{
					if (mConnectors[k]->SourceIndex == kSource)
						break;
					else
						StartIndexOnSource += mConnectors[k]->NumConnectableChannels;
				}

				unsigned int ChannelsOnSource = 0;

				for (unsigned int k = 0; k < NumAcquiredChannels; k++)
				{
					if (mAcquiredChannelsSources[k] == kSource)
						ChannelsOnSource++;
					else if (mAcquiredChannelsSources[k] > kSource)
						break;
				}

				array<unsigned int>^ AcquiredChannels = gcnew array<unsigned int>(ChannelsOnSource);
				pos = 0;
				for (unsigned int k = 0; k < NumAcquiredChannels; k++)
				{
					if (mAcquiredChannelsSources[k] == kSource)
						AcquiredChannels[pos++] = mAcquiredChannelsIndices[k] - StartIndexOnSource;
				}
				mSources[kSource]->UpdateAcquiredChannels(AcquiredChannels);
			}








			FireOnInitializeDevice();
		}
		catch (Exception ^e)
		{
			throw gcnew InitializeDeviceFailedException(__FUNCTION__, e);
		}

	}

	void DataGeneratorInterface::StopInitialization(void)
	{
		mMustStopInitialization = true;
		FireOnStopInitialization();
	}

	void DataGeneratorInterface::StartAcquisition()
	{
		try
		{
			mMustStopAcquisition = false;
			FireOnStartAcquisition();
		}
		catch (Exception^ e)
		{
			throw gcnew StartAcquisitionFailedException(__FUNCTION__, e);
		}
	}

	void DataGeneratorInterface::StopAcquisition(void)
	{
		try
		{
			mMustStopAcquisition = true;
			FireOnStopAcquisitionRequested();
		}
		catch (Exception^ e)
		{
			throw gcnew StopAcquisitionFailedException(__FUNCTION__, e);
		}
	}

	void DataGeneratorInterface::CloseDevice(void)
	{
		try
		{
			FireOnCloseDevice();
		}
		catch (Exception^e)
		{
			throw gcnew CloseDeviceFailedException(__FUNCTION__, e);
		}
	}


	MARSHAL_ERROR DataGeneratorInterface::GetFireOnError()
	{
		return (MARSHAL_ERROR)Marshal::GetFunctionPointerForDelegate(mMarshalError).ToPointer();
	}
	MARSHAL_NEWDATAAVAILABLE DataGeneratorInterface::GetFireOnNewDataAvailable()
	{
		return (MARSHAL_NEWDATAAVAILABLE)Marshal::GetFunctionPointerForDelegate(mMarshalNewDataAvailable).ToPointer();
	}
	MARSHAL_DEVICECHANGED DataGeneratorInterface::GetFireDeviceChanged()
	{
		return (MARSHAL_DEVICECHANGED)Marshal::GetFunctionPointerForDelegate(mMarshalDeviceChanged).ToPointer();
	}
	MARSHAL_STOPACQUISITION DataGeneratorInterface::GetFireOnStopAcquisition()
	{
		return (MARSHAL_STOPACQUISITION)Marshal::GetFunctionPointerForDelegate(mMarshalStopAcquisition).ToPointer();
	}
	MARSHAL_STOPACQUISITION DataGeneratorInterface::GetFireOnStopAcquisitionRequested()
	{
		return (MARSHAL_STOPACQUISITIONREQUESTED)Marshal::GetFunctionPointerForDelegate(mMarshalStopAcquisitionRequested).ToPointer();
	}

	MARSHAL_STOPINITIALIZATION DataGeneratorInterface::GetFireOnStopInitialization()
	{
		return (MARSHAL_STOPINITIALIZATION)Marshal::GetFunctionPointerForDelegate(mMarshalStopInitialization).ToPointer();
	}


	void DataGeneratorInterface::FireOnError(Exception^ exc)
	{
		/*try
		{*/
		static bool AlreadyIn;
		if (AlreadyIn == false)
		{
			AlreadyIn = true;
			ErrorDescription^ err = gcnew ErrorDescription(exc);
			LOG_EXCEPTION(exc);
			OnError(this, err);
			AlreadyIn = false;
		}
		/*}
		catch (Exception^)
		{

		}*/
	}

	void DataGeneratorInterface::FireOnError(char* description, int code, char* function)
	{
		FireOnError(gcnew OTCommException(gcnew String(function), gcnew String(description)));
	}

	void DataGeneratorInterface::FireOnOpenDevice()
	{
		/*try
		{*/


		OnOpenDevice(this, gcnew System::EventArgs());

		/*}
		catch (Exception^)
		{

		}*/

	}

	void DataGeneratorInterface::FireOnInitializeDevice()
	{
		/*try
		{*/
		OnInitializeDevice(this, nullptr);
		/*}
		catch (Exception^)
		{

		}*/
	}

	void DataGeneratorInterface::FireOnStartAcquisition()
	{
		/*try
		{*/
		OnStartAcquisition(this, nullptr);
		/*}
		catch (Exception^)
		{

		}*/
	}

	void DataGeneratorInterface::Callback(IAsyncResult^ ar)
	{
		AsyncResult^ result = (AsyncResult^)ar;
		NewDataAvailable^ caller = (NewDataAvailable^)result->AsyncDelegate;
		caller->EndInvoke(ar);
	}

	void DataGeneratorInterface::FireOnNewDataAvailable(
		unsigned __int16 sourceindex, DataSize datasize, void* data, void* extradata,
		
		__int16 Channels, __int16 ExtraChannels, int TemporalAcquisitions, 
		__int32 AuxiliaryInfo)
	{
 		if (datasize == 16)
		{

			try
			{
				int length = Channels*TemporalAcquisitions;
				int Extralength = ExtraChannels*TemporalAcquisitions;
				if (mDataToDispatchSync16 == nullptr || mDataToDispatchSync16->Length != length)
				{
					mDataToDispatchSync16 = gcnew array<unsigned __int16>(length);
				}
				pin_ptr<System::UInt16> pinPtrArray = &mDataToDispatchSync16[0];
				memcpy_s(pinPtrArray, length * sizeof(unsigned __int16), data, length * sizeof(unsigned __int16));

				if (Extralength > 0 && extradata != nullptr)
				{
					if (mExtraDataToDispatchSync16 == nullptr || mExtraDataToDispatchSync16->Length != Extralength)
					{
						mExtraDataToDispatchSync16 = gcnew array<unsigned __int16>(Extralength);
					}
					pin_ptr<System::UInt16> pinPtrExtraArray = &mExtraDataToDispatchSync16[0];
					memcpy_s(pinPtrExtraArray, Extralength * sizeof(unsigned __int16), extradata, Extralength * sizeof(unsigned __int16));
				}

				NewDataAvailableEventArgs^ ndaea = gcnew NewDataAvailableEventArgs(sourceindex, mDataToDispatchSync16, Channels,
					mExtraDataToDispatchSync16, ExtraChannels, TemporalAcquisitions, AuxiliaryInfo);

				OnNewDataAvailable(this, ndaea);

				//here async
				if (this->AsyncNewDataEventRegroupFactor > 0)
				{
					if (mDataToDispatchAsync16[mCurrentDataToDispatchIndex] == nullptr ||
						mDataToDispatchAsync16[mCurrentDataToDispatchIndex]->Length != length*AsyncNewDataEventRegroupFactor)
					{
						mDataToDispatchAsync16[mCurrentDataToDispatchIndex] = gcnew array<unsigned __int16>(length*AsyncNewDataEventRegroupFactor);
						mDataToDispatchAsyncCurrpos = 0;
					}

					pin_ptr<System::UInt16> pinPtrArray = &mDataToDispatchAsync16[mCurrentDataToDispatchIndex][mDataToDispatchAsyncCurrpos];
					memcpy_s(pinPtrArray, length * sizeof(unsigned __int16), data, length * sizeof(unsigned __int16));


					mDataToDispatchAsyncCurrpos += length;
					if (mDataToDispatchAsyncCurrpos == length*AsyncNewDataEventRegroupFactor)
					{
						OnNewDataAvailableAsync(this,
							gcnew NewDataAvailableEventArgs(sourceindex, mDataToDispatchAsync16[mCurrentDataToDispatchIndex], Channels, nullptr, 0, TemporalAcquisitions, AuxiliaryInfo));
						mDataToDispatchAsyncCurrpos = 0;
						mCurrentDataToDispatchIndex++;
						if (mCurrentDataToDispatchIndex == mDataToDispatchAsync16->Length) //==NUM_CIRCULAR_MANAGED_BUFFERS
							mCurrentDataToDispatchIndex = 0;
					}
				}
			}
			catch (Exception^ e)
			{
				throw e;
			}
		}
		else if (datasize == 32)
		{
			try
			{
				int length = Channels*TemporalAcquisitions;
				int Extralength = ExtraChannels*TemporalAcquisitions;
				if (mDataToDispatchSync32 == nullptr || mDataToDispatchSync32->Length != length)
				{
					mDataToDispatchSync32 = gcnew array<unsigned __int32>(length);
				}
				pin_ptr<System::UInt32> pinPtrArray = &mDataToDispatchSync32[0];
				memcpy_s(pinPtrArray, length * sizeof(unsigned __int32), data, length * sizeof(unsigned __int32));

				if (Extralength > 0 && extradata != nullptr)
				{
					if (mExtraDataToDispatchSync32 == nullptr || mExtraDataToDispatchSync32->Length != Extralength)
					{
						mExtraDataToDispatchSync32 = gcnew array<unsigned __int32>(Extralength);
					}
					pin_ptr<System::UInt32> pinPtrExtraArray = &mExtraDataToDispatchSync32[0];
					memcpy_s(pinPtrExtraArray, Extralength * sizeof(unsigned __int32), extradata, Extralength * sizeof(unsigned __int32));
				}

				NewDataAvailableEventArgs^ ndaea = gcnew NewDataAvailableEventArgs(sourceindex, mDataToDispatchSync32, Channels, 
					mExtraDataToDispatchSync32, ExtraChannels, TemporalAcquisitions, AuxiliaryInfo);

				OnNewDataAvailable(this, ndaea);

				//here async
				if (this->AsyncNewDataEventRegroupFactor > 0)
				{
					if (mDataToDispatchAsync32[mCurrentDataToDispatchIndex] == nullptr ||
						mDataToDispatchAsync32[mCurrentDataToDispatchIndex]->Length != length*AsyncNewDataEventRegroupFactor)
					{
						mDataToDispatchAsync32[mCurrentDataToDispatchIndex] = gcnew array<unsigned __int32>(length*AsyncNewDataEventRegroupFactor);
						mDataToDispatchAsyncCurrpos = 0;
					}

					pin_ptr<System::UInt32> pinPtrArray = &mDataToDispatchAsync32[mCurrentDataToDispatchIndex][mDataToDispatchAsyncCurrpos];
					memcpy_s(pinPtrArray, length * sizeof(unsigned __int32), data, length * sizeof(unsigned __int32));


					mDataToDispatchAsyncCurrpos += length;
					if (mDataToDispatchAsyncCurrpos == length*AsyncNewDataEventRegroupFactor)
					{
						OnNewDataAvailableAsync(this,
							gcnew NewDataAvailableEventArgs(sourceindex, mDataToDispatchAsync32[mCurrentDataToDispatchIndex], Channels, 
								nullptr, 0, TemporalAcquisitions, AuxiliaryInfo));
						mDataToDispatchAsyncCurrpos = 0;
						mCurrentDataToDispatchIndex++;
						if (mCurrentDataToDispatchIndex == mDataToDispatchAsync32->Length) //==NUM_CIRCULAR_MANAGED_BUFFERS
							mCurrentDataToDispatchIndex = 0;
					}
				}
			}
			catch (Exception^ e)
			{
				throw e;
			}
		}
		else if (datasize == 8)
		{
			try
			{
				int length = Channels*TemporalAcquisitions;
				int Extralength = ExtraChannels*TemporalAcquisitions;
				if (mDataToDispatchSync8 == nullptr || mDataToDispatchSync8->Length != length)
				{
					mDataToDispatchSync8 = gcnew array<unsigned __int8>(length);
				}
				pin_ptr<System::Byte> pinPtrArray = &mDataToDispatchSync8[0];
				memcpy_s(pinPtrArray, length * sizeof(unsigned __int8), data, length * sizeof(unsigned __int8));

				if (Extralength > 0 && extradata != nullptr)
				{
					if (mExtraDataToDispatchSync8 == nullptr || mExtraDataToDispatchSync8->Length != Extralength)
					{
						mExtraDataToDispatchSync8 = gcnew array<unsigned __int8>(Extralength);
					}
					pin_ptr<System::Byte> pinPtrExtraArray = &mExtraDataToDispatchSync8[0];
					memcpy_s(pinPtrExtraArray, Extralength * sizeof(unsigned __int8), extradata, Extralength * sizeof(unsigned __int8));
				}

				NewDataAvailableEventArgs^ ndaea = gcnew NewDataAvailableEventArgs(sourceindex, mDataToDispatchSync8, Channels,
					mExtraDataToDispatchSync8, ExtraChannels, TemporalAcquisitions, AuxiliaryInfo);

				OnNewDataAvailable(this, ndaea);

				//here async
				if (this->AsyncNewDataEventRegroupFactor > 0)
				{
					if (mDataToDispatchAsync8[mCurrentDataToDispatchIndex] == nullptr ||
						mDataToDispatchAsync8[mCurrentDataToDispatchIndex]->Length != length*AsyncNewDataEventRegroupFactor)
					{
						mDataToDispatchAsync8[mCurrentDataToDispatchIndex] = gcnew array<unsigned __int8>(length*AsyncNewDataEventRegroupFactor);
						mDataToDispatchAsyncCurrpos = 0;
					}

					pin_ptr<System::Byte> pinPtrArray = &mDataToDispatchAsync8[mCurrentDataToDispatchIndex][mDataToDispatchAsyncCurrpos];
					memcpy_s(pinPtrArray, length * sizeof(unsigned __int8), data, length * sizeof(unsigned __int8));


					mDataToDispatchAsyncCurrpos += length;
					if (mDataToDispatchAsyncCurrpos == length*AsyncNewDataEventRegroupFactor)
					{
						OnNewDataAvailableAsync(this,
							gcnew NewDataAvailableEventArgs(sourceindex, mDataToDispatchAsync8[mCurrentDataToDispatchIndex],
								Channels, nullptr, 0, TemporalAcquisitions, AuxiliaryInfo));
						mDataToDispatchAsyncCurrpos = 0;
						mCurrentDataToDispatchIndex++;
						if (mCurrentDataToDispatchIndex == mDataToDispatchAsync8->Length) //==NUM_CIRCULAR_MANAGED_BUFFERS
							mCurrentDataToDispatchIndex = 0;
					}
				}
			}
			catch (Exception^ e)
			{
				throw e;
			}
		}
	}

	void DataGeneratorInterface::FireOnDeviceChanged(int code, char* description, int length)
	{
		/*try
		{*/
		String^ str = gcnew String(description);
		OnDeviceChanged(this, gcnew DeviceChangedEventArgs(str, code));
		/*}
		catch (Exception^)
		{

		}*/
	}

	void DataGeneratorInterface::FireOnStopAcquisition()
	{
		/*try
		{*/
		OnStopAcquisition(this, nullptr);
		/*}
		catch (Exception^)
		{

		}*/
	}

	void DataGeneratorInterface::FireOnStopAcquisitionRequested()
	{
		/*try
		{*/
		OnStopAcquisitionRequested(this, nullptr);
		/*}
		catch (Exception^)
		{

		}*/
	}


	void DataGeneratorInterface::FireOnStopInitialization()
	{
		/*try
		{*/
		OnStopInitialization(this, nullptr);
		/*}
		catch(Exception^)
		{
		}*/
	}

	void DataGeneratorInterface::FireOnCloseDevice()
	{
		/*try
		{*/
		OnCloseDevice(this, nullptr);
		/*}
		catch (Exception^)
		{

		}*/
	}



	
}