#pragma once
#include "Sensors.h"
using namespace System;

namespace OTComm
{
	///<summary>Generic Adapter</summary>
	public ref class GenericAdapter abstract
	{
	protected:
		array<GenericChannel^>^ mChannels;

	public:
		///<summary>Get channels of adapter</summary>
		///<returns>the array of GenericChannels available for the adapter</returns>
		property array<GenericChannel^>^ Channels
		{
			array<GenericChannel^>^ get();
		}

		GenericAdapter();

		///<summary>Get connected channels of adapter</summary>
		///<returns>the number of GenericChannels connected to the adapter</returns>
		property unsigned int NumConnectedChannels
		{
			unsigned int get();
		}

		///<summary>Get the description of adapter</summary>
		///<returns>the description of the adapter</returns>
		virtual property String^ Description
		{
			String^ get() = 0;
		}

		///<summary>Get the ID of adapter</summary>
		///<returns>the ID of the adapter</returns>
		virtual property String^ ID
		{
			String^ get() = 0;
		}

		///<summary>Get the maximum number of channels connectable to the adapter</summary>
		///<returns>the maximum number of channels connectable to the adapter</returns>
		virtual property unsigned int MaxNumChannels
		{
			unsigned int get() = 0;
		}

		///<summary>Get the minimum size of sensor connectable to the adapter</summary>
		///<returns>the minimum size of sensor connectable to the adapter</returns>
		virtual property unsigned int MinSizeSensor
		{
			unsigned int get() = 0;
		}

		///<summary>Get the maximum size of sensor connectable to the adapter</summary>
		///<returns>the maximum size of sensor connectable to the adapter</returns>
		virtual property unsigned int MaxSizeSensor
		{
			unsigned int get() = 0;
		}

		///<summary>Get the gain of the adapter</summary>
		///<returns>the gain of the adapter</returns>
		virtual property unsigned int AdapterGain
		{
			unsigned int get() = 0;
		}

		///<summary>Get the image of the adapter</summary>
		///<returns>the image of the adapter</returns>
		virtual property Bitmap^ Image { Bitmap^ get(); }

		///<summary>Set the channel of the adapter</summary>
		///<param name="position">coordinate of sensor in the adapter</param>
		///<param name="channel">sensor to connect to adapter</param>
		GenericAdapter^ SetChannel(const unsigned int position, GenericChannel^ channel);

		///<summary>Set the channel of the adapter</summary>
		///<param name="positionStart">first coordinate of sensor in the adapter</param>
		///<param name="positionEnd">last coordinate of sensor in the adapter</param>
		///<param name="channel">sensor to connect to adapter</param>
		GenericAdapter^ SetChannels(const unsigned int positionStart, const unsigned int positionEnd, GenericChannel^ channel);

	internal:
		void WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream);

		void WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int channel);

		void WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channel);

		void WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channel, unsigned int detectionMode, unsigned int lowPassFilter, unsigned int highPassFilter);

		void GenericAdapter::WriteXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channel,
			float m_Gain, unsigned int detectionMode, unsigned int lowPassFilter, unsigned int highPassFilter);

		void WriteSavingXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channel);

		void WriteSavingXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channelarray, float m_Gain, float m_LPfilter, float m_HPfilter, unsigned int m_DetectionMode);

		void WriteSavingXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channelarray, float m_Gain, float m_LPfilter, float m_HPfilter, String^ m_DetectionMode);
	
		void AuxWriteSavingXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channel, float m_Gain,	float m_LPfilter, float m_HPfilter, unsigned int m_DetectionMode, float scaleFactor, String^ unitOfMeasurement);

		void AuxWriteSavingXMLConfiguration(System::Xml::XmlTextWriter ^% XMLTextStream, unsigned int adapter, unsigned int startIndex, unsigned int channel, float m_Gain, float scaleFactor, String^ unitOfMeasurement);
	};





}
