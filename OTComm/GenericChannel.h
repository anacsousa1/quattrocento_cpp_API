#pragma once
#include "Muscles.h"
#include "DebugLog.h"
#include "ErrorDescription.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Drawing;
namespace OTComm
{
	///<summary>Generic Channel</summary>
	public ref class GenericChannel abstract
	{
	protected:
		double mGain;
		double mLowPassFilter;
		double mHighPassFilter;
		bool mAdditionalParamSet;
		SideID mSide;
		MuscleID mMuscleID;
		String^ mChannelPrefix = "";
		String^ mDescription;

		//Added bool to check if enable aux as trigger
		bool mEnableTrigger;

	internal:
		virtual void WriteXMLConfiguration(System::Xml::XmlTextWriter^ % XMLTextStream, const unsigned int Index);
	public:
		///<summary>Gain of the channel</summary>
		property double Gain
		{
			double get() { return mGain; }
		}
		///<summary>Low pass filter of the channel</summary>
		property double LowPassFilter
		{
			double get() { return mLowPassFilter; }
		}
		///<summary>High pass filter of the channel</summary>
		property double HighPassFilter
		{
			double get() { return mHighPassFilter; }
		}
		///<summary>Muscle ID of the channel</summary>
		property unsigned int MuscleID
		{
			unsigned int get() { return mMuscleID; }
			void set(unsigned int muscleID) { mMuscleID = muscleID; }
		}

		///<summary>Aux channel as trigger</summary>
		property bool EnableTrigger
		{
			bool get() { return mEnableTrigger; }
			void set(bool enableTrigger) { mEnableTrigger = enableTrigger; }
		}
		///<summary>Description of the channel</summary>
		virtual property String^ Description
		{
			String^ get() { return mDescription; };
			void set(String^ description) { mDescription = description; }
		}
		///<summary>ID of the channel</summary>
		virtual property String^ ChannelID
		{
			String^ get() = 0;
		}		
		///<summary>Dimension of the channel</summary>
		virtual property unsigned int Channels
		{
			unsigned int get() = 0;
		}
		///<summary>Muscle description</summary>
		property String^ MuscleDescription
		{
			String^ get() { return Muscles::Description(mMuscleID); }
			void set(String^ description) { mMuscleID = Muscles::ID(description); }
		}
		///<summary>Muscle side description</summary>
		property String^ MuscleSideDescription
		{
			String^ get() { return MuscleSide::Description(mSide); }
			void set(String^ description) { mSide = MuscleSide::ID(description); }
		}

		///<summary>Channel info description</summary>
		property String^ ChannelPrefix
		{
			String^ get() { return mChannelPrefix; }
			void set(String^ channelPrefix) { mChannelPrefix = gcnew String(channelPrefix); }
		}

		///<summary>Image of the channel</summary>
		virtual property Bitmap^ Image { Bitmap^ get(); }
		///<summary>Image of the channel</summary>
		virtual property Bitmap^ ImageRotation { Bitmap^ get(); }
		///<summary>Image of the channel</summary>
		virtual property Bitmap^ ImageMatrixChannel { Bitmap^ get(); }
		///<summary>Image of the channel</summary>
		virtual property Bitmap^ MapImage { Bitmap^ get(); }
		///<summary>Remap of the sensor</summary>
		virtual property array<array<unsigned short>^>^ Remap { array<array<unsigned short>^>^ get(); }
		///<summary>Remap of the sensor</summary>
		virtual property array<unsigned short>^ MatrixChannelRemap { array<unsigned short>^ get(); }


		GenericChannel();
		///<summary>Default Constructor</summary>
		///<param name="ID">Muscle ID</param>
		GenericChannel(OTComm::MuscleID ID);
		///<summary>Constructor with Gain=1 and without filter information</summary>
		///<param name="ID">Muscle ID</param>
		///<param name="side">Muscle side</param>
		GenericChannel(OTComm::MuscleID ID, OTComm::SideID side);
		///<summary>Constructor without filter information</summary>
		///<param name="ID">Muscle ID</param>
		///<param name="side">Muscle side</param>
		///<param name="Gain">Gain</param>
		GenericChannel(OTComm::MuscleID ID, OTComm::SideID side, double Gain);
		///<param name="ID">Muscle ID</param>
		///<param name="side">Muscle side</param>
		///<param name="Gain">Gain</param>
		///<param name="LowPass">LowPass</param>
		///<param name="HighPass">HighPass</param>
		GenericChannel(OTComm::MuscleID ID, OTComm::SideID side, double Gain, double LowPass, double HighPass);
		
		
	};	

}