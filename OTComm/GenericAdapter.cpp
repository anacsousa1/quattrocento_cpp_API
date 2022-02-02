#pragma once
#include "GenericAdapter.h"
#include "GenericException.h"
using namespace System;
using namespace System::Globalization;
//using namespace System::Collections;

namespace OTComm
{

	array<GenericChannel^>^ GenericAdapter::Channels::get()
	{
		return mChannels;
	}

	GenericAdapter::GenericAdapter()
	{
		mChannels = gcnew array<GenericChannel^>(MaxNumChannels);
	}

	unsigned int GenericAdapter::NumConnectedChannels::get()
	{

		unsigned int Num = 0;
		for (int k = 0; k < mChannels->Length; k++)
			if (mChannels[k] != nullptr)
				Num++;
		return Num;

	}

	GenericAdapter^ GenericAdapter::SetChannel(const unsigned int position, GenericChannel^ channel)
	{
		if (position < MaxNumChannels)
			mChannels[position] = channel;
		else throw gcnew InvalidChannelNumber(__FUNCTION__);
		return this;
	}

	GenericAdapter^ GenericAdapter::SetChannels(const unsigned int positionStart, const unsigned int positionEnd, GenericChannel^ channel)
	{
		for (unsigned int k = positionStart; k <= positionEnd; k++)
			SetChannel(k, channel);
		return this;
	}

	void GenericAdapter::WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream)
	{
		WriteXMLConfiguration(XMLTextStream, 0);
	}
	
	void GenericAdapter::WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channel)
	{
		try
		{
			XMLTextStream->WriteStartElement("Adapter");
			XMLTextStream->WriteAttributeString("ID", ID);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("Gain", AdapterGain.ToString());
			XMLTextStream->WriteAttributeString("AdapterIndex", adapter.ToString());
			XMLTextStream->WriteAttributeString("ChannelStartIndex", startIndex.ToString());


			for each(GenericChannel^ c in mChannels)
			{
				if (c != nullptr)
				{
					c->WriteXMLConfiguration(XMLTextStream, channel);
				}
				channel++;
			}
			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch (Exception^)
		{

		}
	}

	void GenericAdapter::WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channel,
		unsigned int detectionMode, unsigned int lowPassFilter, unsigned int highPassFilter)
	{
		try
		{
			XMLTextStream->WriteStartElement("Adapter");
			XMLTextStream->WriteAttributeString("ID", ID);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("DetectionMode", detectionMode.ToString());
			XMLTextStream->WriteAttributeString("LowPassFilter", lowPassFilter.ToString());
			XMLTextStream->WriteAttributeString("HighPassFilter", highPassFilter.ToString());
			XMLTextStream->WriteAttributeString("Gain", AdapterGain.ToString());
			XMLTextStream->WriteAttributeString("AdapterIndex", adapter.ToString());
			XMLTextStream->WriteAttributeString("ChannelStartIndex", startIndex.ToString());


			for each(GenericChannel^ c in mChannels)
			{
				if (c != nullptr)
				{
					c->WriteXMLConfiguration(XMLTextStream, channel);
				}
				channel++;
			}
			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch (Exception^)
		{

		}
	}

	void GenericAdapter::WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channel,
		float m_Gain, unsigned int detectionMode, unsigned int lowPassFilter, unsigned int highPassFilter)
	{
		try
		{
			XMLTextStream->WriteStartElement("Adapter");
			XMLTextStream->WriteAttributeString("ID", ID);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("DetectionMode", detectionMode.ToString());
			XMLTextStream->WriteAttributeString("LowPassFilter", lowPassFilter.ToString());
			XMLTextStream->WriteAttributeString("HighPassFilter", highPassFilter.ToString());
			XMLTextStream->WriteAttributeString("Gain", m_Gain.ToString());
			XMLTextStream->WriteAttributeString("AdapterIndex", adapter.ToString());
			XMLTextStream->WriteAttributeString("ChannelStartIndex", startIndex.ToString());


			for each(GenericChannel^ c in mChannels)
			{
				if (c != nullptr)
				{
					c->WriteXMLConfiguration(XMLTextStream, channel);
				}
				channel++;
			}
			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch (Exception^)
		{

		}
	}

	void GenericAdapter::WriteSavingXMLConfiguration(
		System::Xml::XmlTextWriter ^% XMLTextStream, 
		unsigned int adapter, 
		unsigned int startIndex,
		unsigned int channel)
	{
		try
		{
			XMLTextStream->WriteStartElement("Adapter");
			XMLTextStream->WriteAttributeString("ID", ID);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("Gain", AdapterGain.ToString());
			XMLTextStream->WriteAttributeString("AdapterIndex", adapter.ToString());
			XMLTextStream->WriteAttributeString("ChannelStartIndex", startIndex.ToString());


			for each(GenericChannel^ c in mChannels)
				if (c != nullptr)
				{
					channel++;
					c->WriteXMLConfiguration(XMLTextStream, channel);
				}

			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch(Exception^)
		{ }
	}

	void GenericAdapter::WriteSavingXMLConfiguration(
		System::Xml::XmlTextWriter ^% XMLTextStream,
		unsigned int adapter,
		unsigned int startIndex,
		unsigned int channel,
		float m_Gain,
		float m_LPfilter,
		float m_HPfilter,
		unsigned int m_DetectionMode)
	{
		try
		{
			XMLTextStream->WriteStartElement("Adapter");
			XMLTextStream->WriteAttributeString("ID", ID);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("Gain", m_Gain.ToString("G", CultureInfo::CreateSpecificCulture("en-US")));
			XMLTextStream->WriteAttributeString("HighPassFilter", m_HPfilter.ToString("G", CultureInfo::CreateSpecificCulture("en-US")));
			XMLTextStream->WriteAttributeString("LowPassFilter", m_LPfilter.ToString());
			XMLTextStream->WriteAttributeString("DetectionMode", m_DetectionMode.ToString());
			XMLTextStream->WriteAttributeString("AdapterIndex", adapter.ToString());
			XMLTextStream->WriteAttributeString("ChannelStartIndex", startIndex.ToString());



			for each(GenericChannel^ c in mChannels)
				if (c != nullptr)
				{
					c->WriteXMLConfiguration(XMLTextStream, channel);
					channel++;
				}

			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch (Exception^)
		{

		}
	}

	void GenericAdapter::WriteSavingXMLConfiguration(
		System::Xml::XmlTextWriter ^% XMLTextStream,
		unsigned int adapter,
		unsigned int startIndex,
		unsigned int channel,
		float m_Gain,
		float m_LPfilter,
		float m_HPfilter,
		String^ m_DetectionMode)
	{
		try
		{
			XMLTextStream->WriteStartElement("Adapter");
			XMLTextStream->WriteAttributeString("ID", ID);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("Gain", m_Gain.ToString("G", CultureInfo::CreateSpecificCulture("en-US")));
			XMLTextStream->WriteAttributeString("HighPassFilter", m_HPfilter.ToString("G", CultureInfo::CreateSpecificCulture("en-US")));
			XMLTextStream->WriteAttributeString("LowPassFilter", m_LPfilter.ToString());
			XMLTextStream->WriteAttributeString("DetectionMode", m_DetectionMode);
			XMLTextStream->WriteAttributeString("AdapterIndex", adapter.ToString());
			XMLTextStream->WriteAttributeString("ChannelStartIndex", startIndex.ToString());



			for each(GenericChannel^ c in mChannels)
				if (c != nullptr)
				{
					c->WriteXMLConfiguration(XMLTextStream, channel);
					channel++;
				}

			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch (Exception^)
		{

		}
	}

	void GenericAdapter::AuxWriteSavingXMLConfiguration(
		System::Xml::XmlTextWriter ^% XMLTextStream,
		unsigned int adapter,
		unsigned int startIndex,
		unsigned int channel,
		float m_Gain,
		float m_LPfilter,
		float m_HPfilter,
		unsigned int m_DetectionMode,
		float scaleFactor,
		String^ unitOfMeasurement)
	{
		try
		{
			XMLTextStream->WriteStartElement("Adapter");
			XMLTextStream->WriteAttributeString("ID", ID);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("Gain", m_Gain.ToString("G", CultureInfo::CreateSpecificCulture("en-US")));
			XMLTextStream->WriteAttributeString("HighPassFilter", m_HPfilter.ToString("G", CultureInfo::CreateSpecificCulture("en-US")));
			XMLTextStream->WriteAttributeString("LowPassFilter", m_LPfilter.ToString());
			XMLTextStream->WriteAttributeString("DetectionMode", m_DetectionMode.ToString());
			XMLTextStream->WriteAttributeString("AdapterIndex", adapter.ToString());
			XMLTextStream->WriteAttributeString("ChannelStartIndex", startIndex.ToString());
			XMLTextStream->WriteAttributeString("AuxScaleFactor", scaleFactor.ToString()); //Aux Scale Factor
			XMLTextStream->WriteAttributeString("UnitOfMeasurement", unitOfMeasurement); //Aux Scale Factor



			for each(GenericChannel^ c in mChannels)
				if (c != nullptr)
				{
					c->WriteXMLConfiguration(XMLTextStream, channel);
					channel++;
				}

			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch (Exception^)
		{

		}
	}

	void GenericAdapter::AuxWriteSavingXMLConfiguration(
		System::Xml::XmlTextWriter ^% XMLTextStream,
		unsigned int adapter,
		unsigned int startIndex,
		unsigned int channel,
		float m_Gain,
		float scaleFactor,
		String^ unitOfMeasurement )
	{
		try
		{
			XMLTextStream->WriteStartElement("Adapter");
			XMLTextStream->WriteAttributeString("ID", ID);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("Gain", m_Gain.ToString("G", CultureInfo::CreateSpecificCulture("en-US")));
			XMLTextStream->WriteAttributeString("AdapterIndex", adapter.ToString());
			XMLTextStream->WriteAttributeString("ChannelStartIndex", startIndex.ToString());
			XMLTextStream->WriteAttributeString("AuxScaleFactor", scaleFactor.ToString()); //Aux Scale Factor
			XMLTextStream->WriteAttributeString("UnitOfMeasurement", unitOfMeasurement); //Aux Scale Factor



			for each(GenericChannel^ c in mChannels)
				if (c != nullptr)
				{
					c->WriteXMLConfiguration(XMLTextStream, channel);
					channel++;
				}

			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch (Exception^)
		{

		}
	}

	Bitmap^ GenericAdapter::Image::get()
	{
		try
		{
			return Utilities::GetImageFromEmbeddedResource("EMG_USB2_Plus.jpg", true);
		}
		catch (Exception^)
		{
			return nullptr;
		}
	}

	void GenericAdapter::WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int channel)
	{
		try
		{
			XMLTextStream->WriteStartElement("Adapter");
			XMLTextStream->WriteAttributeString("ID", ID);
			XMLTextStream->WriteAttributeString("Description", Description);
			XMLTextStream->WriteAttributeString("Gain", AdapterGain.ToString());


			for each(GenericChannel^ c in mChannels)
			{
				channel++;
				if (c != nullptr)
					c->WriteXMLConfiguration(XMLTextStream, channel);
			}

			XMLTextStream->WriteEndElement();//"Channel"	
		}
		catch (Exception^)
		{

		}
	}
}
