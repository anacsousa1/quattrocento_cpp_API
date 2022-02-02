#pragma once
#include "Channel.h"

using namespace System;
namespace OTComm
{
		Channel::Channel(): Channel(Muscles::NotDefined,1,-1,-1)
		{
		}

		Channel::Channel(OTComm::MuscleID ID) : Channel(ID, 1, -1, -1)
		{
		}

		Channel::Channel(OTComm::MuscleID ID, double Gain) : Channel(ID, Gain, -1, -1)
		{
		}

		Channel::Channel(OTComm::MuscleID ID, double Gain, double LowPass, double HighPass) :
			mMuscleID(ID), mGain(Gain), mLowPassFilter(LowPass), mHighPassFilter(HighPass)
		{
		}

		void Channel::WriteXMLConfiguration(System::Xml::XmlTextWriter^ % XMLTextStream, const unsigned int Index)
		{
			XMLTextStream->WriteStartElement("Channel");
			XMLTextStream->WriteAttributeString("Gain", String::Format("{0}",mGain));
			if(mLowPassFilter>=0)
				XMLTextStream->WriteAttributeString("LowPass", String::Format("{0}", mLowPassFilter));
			if(mHighPassFilter>=0)
				XMLTextStream->WriteAttributeString("HighPass", String::Format("{0}", mHighPassFilter));
			
			XMLTextStream->WriteAttributeString("Index", (Index + 1).ToString());//"Channel"
			XMLTextStream->WriteString(MuscleDescription);//
			XMLTextStream->WriteEndElement();//"Channel"		
		}
		

	
}