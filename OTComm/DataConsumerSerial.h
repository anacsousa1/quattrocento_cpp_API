#pragma once
#include "DataConsumerInterface.h"
#include "DataValidator.h"
using namespace System::IO;

namespace OTComm {

	///<summary>
	/// DataConsumerInterface for saving data on temporary file
	///</summary>

	//Read(Byte[],?Int32,?Int32)
	//ReadByte()
	//ReadLine()
	//ReadTimeout
	//ReceivedBytesThreshold
	//BytesToRead

	public ref class DataConsumerSerial : public DataConsumerInterface
	{
	public:
		///<summary>
		/// Constructs a DataConsumerSerial object
		///</summary>
		DataConsumerSerial();
		~DataConsumerSerial();
		int Read(int Channel, double^ data);
		///<summary>
		/// Associate the DataGenerator to the DataConsumer
		///</summary>
		///<param name="gen">DataGeneratorInterface to associate to the DataConsumer</param>
		virtual void Register(DataGeneratorInterface^ gen) override;
		///<summary>
		/// Remove the DataGenerator from the DataConsumer
		///</summary>
		///<param name="gen">DataGeneratorInterface to remove from the DataConsumer</param>
		virtual void Unregister(DataGeneratorInterface^ gen) override;
		///<summary>
		/// Start recording on file
		///</summary>
		virtual String^ GetConsumerDescription() override;
		///<summary>
		/// Get XML configuration of consumer
		///</summary>
		///<returns>Returns the XML configuration of consumer</returns>
		virtual String^ GetConsumerXMLConfiguration() override;
		///<summary>
		/// Filename where data are saved
		///</summary>
	
	protected:
		long mReadBytes;
		array<unsigned char>^ mBuffer;
		void ResetSavedBytes();
		property long^ SavedBytes
		{
			long^ get();
		}
		DataValidator^ mValidator;
		///<summary>
		/// Event on start acquisition
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnStartAcquisition(System::Object^ sender, EventArgs^ e);
		///<summary>
		/// Event on stop acquisition
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnStopAcquisition(System::Object^ sender, EventArgs^ e);
		///<summary>
		/// Event on new data available
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnNewDataAvailable(System::Object^ sender, EventArgs^ e);

	};



}