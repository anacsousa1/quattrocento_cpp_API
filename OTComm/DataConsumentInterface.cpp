#pragma once
#include "DataConsumerInterface.h"
namespace OTComm
{
	String^ DataConsumerInterface::GetDeviceDescription()
	{
		if (mDataGenerator != nullptr)
			return mDataGenerator->GetDeviceDescription();
		return "";
	}

	String^ DataConsumerInterface::GetDeviceXMLConfiguration()
	{
		if (mDataGenerator != nullptr)
			return mDataGenerator->GetDeviceXMLConfiguration();
		return "";
	}

	DataConsumerInterface::DataConsumerInterface() :mDataGenerator(nullptr)
	{
	}
}