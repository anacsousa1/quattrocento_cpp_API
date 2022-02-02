#pragma once
#include "DataConsumerLog.h"

using namespace System::IO;

namespace OTComm {

	///<summary>
	/// DataConsumerInterface for saving data on temporary file with precision 16 bits
	///</summary>
	public ref class DataConsumerLog16 : public DataConsumerLog
	{
	public:
		///<summary>
		/// Constructor
		///</summary>
		///<param name="FileName">Path to file where to save data</param>
		DataConsumerLog16(String^ FileName);
		DataConsumerLog16();
	protected :
		///<summary>
		/// Event on new data available
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnNewDataAvailable(System::Object^ sender, EventArgs^ e) override;

	};



}