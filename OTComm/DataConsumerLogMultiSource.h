#pragma once
#include "DataConsumerInterface.h"
#include "DataConsumerLog.h"

using namespace System::IO;

namespace OTComm {

	///<summary>
	/// DataConsumerInterface for saving data on temporary file
	///</summary>
	public ref class DataConsumerLogMultiSource : public DataConsumerLog
	{
	public:
		///<summary>
		/// Constructor
		///</summary>
		///<param name="FileName">Path to file where to save data</param>
		DataConsumerLogMultiSource(String^ fileNameTemplate);
		~DataConsumerLogMultiSource();
		///<summary>
		/// Associate the DataGenerator to the DataConsumer
		///</summary>
		///<param name="gen">DataConsumerAggregator to associate to the DataConsumer</param>
		///<returns>Returns -1 if error, 0 if everything is fine</returns>
		virtual void Register(DataGeneratorInterface^ gen, __int16 source) override;
		///<summary>
		/// Remove the DataGenerator from the DataConsumer
		///</summary>
		///<param name="gen">DataGeneratorInterface to remove from the DataConsumer</param>
		///<returns>Returns -1 if error, 0 if everything is fine</returns>
		virtual void Unregister(DataGeneratorInterface^ gen) override;
		///<summary>
		/// Start recording on file
		///</summary>
		///<returns>Returns -1 if error, 0 if everything is fine</returns>
		virtual void StartRecording() override;
		///<summary>
		/// Stop recording on file
		///</summary>
		///<returns>Returns -1 if error, 0 if everything is fine</returns>
		virtual void StopRecording() override;
		///<summary>
		/// Get Description of consumer
		///</summary>
		///<returns>Returns the description of consumer</returns>
		virtual String^ GetConsumerDescription() override;
		///<summary>
		/// Get XML configuration of consumer
		///</summary>
		///<returns>Returns the XML configuration of consumer</returns>
		virtual String^ GetConsumerXMLConfiguration() override;		

	protected:
		///<summary>
		/// Template name to create the files used for all the DataConsumers (mDataConsumers) initialization
		///</summary>
		String^ FileTemplateName;
		///<summary>
		/// List of all DataConsumers used as source
		///</summary>
		List<DataConsumerLog^>^ mDataConsumers;		
		///<summary>
		/// Event on new data available
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnNewDataAvailable(System::Object^ sender, EventArgs^ e) override;
	};



}