#pragma once
#include "DataConsumerInterface.h"
namespace OTComm
{

	void DataConsumerInterface::SetValidator(DataValidator^ validator, DataValidator^ extravalidator)
	{
		mValidator = validator;
		mValidatorExtra = extravalidator;
	}

	void DataConsumerInterface::SetValidator(DataValidator^ validator)
	{
		SetValidator(validator, nullptr);
	}

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

	DataConsumerInterface::DataConsumerInterface() :mDataGenerator
	(nullptr), mValidator(nullptr), mValidatorExtra(nullptr), mAssociatedSourceIndex(-1)
	{
	}

	void DataConsumerInterface::SetDataGeneratorInterface(DataGeneratorInterface^ dgi, __int16 source)
	{
		mAssociatedSourceIndex = source;
		mDataGenerator = dgi;
	}

	__int16 DataConsumerInterface::GetAssociatedSourceIndex()
	{
		return mAssociatedSourceIndex;
	}
}