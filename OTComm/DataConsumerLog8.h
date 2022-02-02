#pragma once
#include "DataConsumerLog.h"

using namespace System::IO;

namespace OTComm {

	///<summary>
	/// New type for the properties inside DataTokeep class
	///</summary>
	typedef int DataToKeep;

	///<summary>
	/// DataTruncation class for report what part need to be truncated (or not)
	///</summary>
	public ref class DataTruncation abstract
	{
	public:
		///<summary>
		/// Lower part of single sample is used as data
		///</summary>
		static const DataToKeep KeepLowerPart = 0;

		///<summary>
		/// Upper part of single sample is used as data
		///</summary>
		static const DataToKeep KeepUpperPart = 1;
	};

	///<summary>
	/// DataConsumerInterface for saving data on temporary file with precision 8 bits
	///</summary>
	/// 
	public ref class DataConsumerLog8 : public DataConsumerLog
	{
	public:
		///<summary>
		/// Constructor
		///</summary>
		///<param name="FileName">Path to file where to save data</param>
		///<param name="Keep">What part of received sample is used as 8 bit data, the lower or the upper</param>
		DataConsumerLog8(String^ FileName, DataToKeep Keep);
		DataConsumerLog8();

	protected:
		///<summary>
		/// Event on new data available
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnNewDataAvailable(System::Object^ sender, EventArgs^ e) override;
		DataToKeep mKeep;
	};



}