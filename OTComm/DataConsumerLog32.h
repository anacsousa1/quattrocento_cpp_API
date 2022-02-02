#pragma once
#include "DataConsumerLog.h"

using namespace System::IO;

namespace OTComm {

	///<summary>
	/// DataConsumerInterface for saving data on temporary file with precision 32 bit
	///</summary>
	public ref class DataConsumerLog32 : public DataConsumerLog
	{
	public:
		///<summary>
		/// Constructor
		///</summary>
		///<param name="FileName">Path to file where to save data</param>
		DataConsumerLog32(String^ FileName);
		DataConsumerLog32();
	protected:
		///<summary>
		/// Event on new data available
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnNewDataAvailable(System::Object^ sender, EventArgs^ e) override;

	};



}