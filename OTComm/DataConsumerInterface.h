#pragma once
#include "DataGeneratorInterface.h"
#include "DataValidator.h"

namespace OTComm
{
	public ref class DataConsumerInterface abstract
	{
	private:
		/// <summary>
		/// DataGenerator associated to the consumer
		/// </summary>
		DataGeneratorInterface^ mDataGenerator;

		/// <summary>
		/// Source index associated to the consumer
		/// </summary>
		__int16 mAssociatedSourceIndex;

	protected:
		/// <summary>
		/// DataValidator associated to the consumer
		/// </summary>
		DataValidator^ mValidator;
		/// <summary>
		/// An extra DataValidator (used when necessary) associated to the consumer
		/// </summary>
		DataValidator^ mValidatorExtra;
	public:		
		/// <summary>
		/// Common Set method for DataValidator protected property
		/// </summary>
		void SetValidator(DataValidator^ validator);

		/// <summary>
		/// Different Set method for both DataValidator protected properties
		/// </summary>
		void SetValidator(DataValidator^ validator, DataValidator^ extravalidator);

		DataConsumerInterface();
		/// <summary>
		/// Register the consumer to the device
		/// </summary>
		/// <param name="gen">DataGeneratorInterface to register</param>
		virtual void Register(DataGeneratorInterface^ gen, __int16 sourceindex) = 0;
		/// <summary>
		/// Unregister the consumer to the device
		/// </summary>
		/// <param name="gen">DataGeneratorInterface to unregister</param>
		virtual void Unregister(DataGeneratorInterface^ gen) = 0;
		/// <summary>
		/// Description of consumer
		/// </summary>
		/// <returns> Returns the name of the consumer</returns>
		virtual String^ GetConsumerDescription() = 0;
		/// <summary>
		/// XML Configuration of consumer
		/// </summary>
		/// <returns> Returns the xml configuration of the consumer</returns>
		virtual String^ GetConsumerXMLConfiguration() = 0;
		/// <summary>
		/// Description of device associated to consumer
		/// </summary>
		/// <returns> Returns the name of the device associated to the consumer</returns>
		virtual String^ GetDeviceDescription();
		/// <summary>
		/// XML Configuration of Device associated to consumer
		/// </summary>
		/// <returns> Returns the XML Configuration of Device associated to consumer</returns>
		virtual String^ GetDeviceXMLConfiguration();

		/// <summary>
		/// Get method to return the associated source index
		/// </summary>
		__int16 GetAssociatedSourceIndex();
	protected:
		/// <summary>
		/// Set DatageneratorInterface associated to DataConsumerInterface
		/// </summary>
		/// <param name="dgi">DataGeneratorInterface to set</param>
		virtual void SetDataGeneratorInterface(DataGeneratorInterface^ dgi, __int16 sourceindex);
	};
}
