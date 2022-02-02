#pragma once
#include "GenericAdapter.h"

namespace OTComm
{

	///<summary>Generic Connector</summary>
	public ref class GenericConnector
	{
		
	protected:
		GenericAdapter^ mAdapter;
		unsigned int mSourceIndex;
		unsigned int mNumConnectableChannels;
		unsigned int mDetectionMode;
		unsigned int mLowPassFilter;
		unsigned int mHighPassFilter;
		double mAuxScaleFactor;
		String^ mUnitOfMeasurement;

	public:
		///<summary> Adapter connected to Connector</summary>
		property GenericAdapter^ Adapter
		{
			GenericAdapter^ get() { return mAdapter; };
			void set(GenericAdapter^ value) { mAdapter = value; };
		}

		///<summary>Default constructor</summary>
		GenericConnector() :GenericConnector(1,0)
		{

		}
		///<summary>Constructor where you can set the number of connectable channels</summary>
		///<param name="numConnectableChannels">Number of connectable channels</param>
		GenericConnector(unsigned int numConnectableChannels):GenericConnector(numConnectableChannels,0)
		{
		}
		///<summary>Constructor where you can set the number of connectable channels</summary>
		///<param name="numConnectableChannels">Number of connectable channels</param>
		GenericConnector(unsigned int numConnectableChannels, unsigned int SourceIndex)
		{
			mAdapter = nullptr;
			mNumConnectableChannels = numConnectableChannels;
			mSourceIndex = SourceIndex;
		}

		///<summary>Connectable channels</summary>
		property unsigned int NumConnectableChannels
		{
			unsigned int get() {
				return mNumConnectableChannels;
			}
		}

		///<summary>Connected channels</summary>
		property unsigned int NumConnectedChannels
		{
			unsigned int get() { return mAdapter == nullptr ? 0 : mAdapter->NumConnectedChannels; }
		}

		///<summary>Connected channels</summary>
		property unsigned int SourceIndex
		{
			unsigned int get() { return mSourceIndex; }
		}

		///<summary>Detection Mode</summary>
		property unsigned int DetectionMode
		{
			unsigned int get() { return mDetectionMode; };
			void set(unsigned int value) { mDetectionMode = value; };
		}

		///<summary>Low Pass Filter</summary>
		property unsigned int LowPassFilter
		{
			unsigned int get() { return mLowPassFilter; };
			void set(unsigned int value) { mLowPassFilter = value; };
		}

		///<summary>High Pass Filter</summary>
		property unsigned int HigPassFilter
		{
			unsigned int get() { return mHighPassFilter; };
			void set(unsigned int value) { mHighPassFilter = value; };
		}

		property double AuxScaleFactor {
			double get() { return mAuxScaleFactor; };
			void set(double value) { mAuxScaleFactor = value; };
		}

		property String^ unitOfMeasurement {
			String^ get() { return mUnitOfMeasurement; };
			void set(String^ val) { mUnitOfMeasurement = val; }
		}

	};
}