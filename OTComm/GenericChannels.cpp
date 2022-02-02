#pragma once
#include "GenericChannel.h"
#include "Utilities.h"

using namespace System::IO;
using namespace System::Reflection;

namespace OTComm
{
	GenericChannel::GenericChannel() : GenericChannel(Muscles::NotDefined, MuscleSide::Undefined, 1, -1, -1)
	{
	}

	GenericChannel::GenericChannel(OTComm::MuscleID ID) : GenericChannel(ID, MuscleSide::Undefined, 1, -1, -1)
	{
	}

	GenericChannel::GenericChannel(OTComm::MuscleID ID, OTComm::SideID side) : GenericChannel(ID, side, 1, -1, -1)
	{
	}

	GenericChannel::GenericChannel(OTComm::MuscleID ID, OTComm::SideID side, double Gain) : GenericChannel(ID, side, Gain, -1, -1)
	{
	}

	GenericChannel::GenericChannel(OTComm::MuscleID ID, OTComm::SideID side, double Gain, double LowPass, double HighPass) :
		mMuscleID(ID), mSide(side), mGain(Gain), mLowPassFilter(LowPass), mHighPassFilter(HighPass)
	{
	}
	

	Bitmap^ GenericChannel::Image::get()
	{

		return Utilities::GetDefaultImage();
	}

	Bitmap^ GenericChannel::ImageRotation::get()
	{

		return Utilities::GetDefaultImage();
	}

	Bitmap^ GenericChannel::MapImage::get()
	{

		return Utilities::GetDefaultImage();
	}

	Bitmap^ GenericChannel::ImageMatrixChannel::get()
	{

		return Utilities::GetDefaultImage();
	}

	array<array<unsigned short>^>^ GenericChannel::Remap::get()
	{
		array<unsigned short>^ aa = gcnew array<unsigned short>{1};
		array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
		return bb;
	}

	array<unsigned short>^ GenericChannel::MatrixChannelRemap::get()
	{
		// array composed as offset_x, offset_y, offset_button_x, offset_button_y, size
		array<unsigned short>^ aa = gcnew array<unsigned short>{66,62,27,27,14};
		return aa;
	}


	void GenericChannel::WriteXMLConfiguration(System::Xml::XmlTextWriter^ % XMLTextStream, const unsigned int Index)
	{
		try
		{
			XMLTextStream->WriteStartElement("Channel");
			XMLTextStream->WriteAttributeString("ID", ChannelID);
			XMLTextStream->WriteAttributeString("Prefix", ChannelPrefix);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("Gain", String::Format("{0}", mGain));
			if (mLowPassFilter >= 0)
				XMLTextStream->WriteAttributeString("LowPass", String::Format("{0}", mLowPassFilter));
			if (mHighPassFilter >= 0)
				XMLTextStream->WriteAttributeString("HighPass", String::Format("{0}", mHighPassFilter));

			XMLTextStream->WriteAttributeString("Index", (Index).ToString());//"Channel"
			XMLTextStream->WriteAttributeString("Muscle", MuscleDescription);//
			XMLTextStream->WriteAttributeString("Side", MuscleSideDescription);//
			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch (Exception^)
		{

		}


	}
		

	
}