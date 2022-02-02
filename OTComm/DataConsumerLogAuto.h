#pragma once
#include "DataConsumerLog.h"

using namespace System::IO;

namespace OTComm {

	///<summary>
	/// DataConsumerInterface for saving data on temporary file
	///</summary>
	public ref class DataConsumerLogAuto : public DataConsumerLog
	{
	public:
		DataConsumerLogAuto(String^ FileName, DataType DataSize);
		DataConsumerLogAuto();
	protected:
		DataType m_DataSizeUsed;
		DataType m_DataSize;
		///<summary>
		/// Event on new data available
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnNewDataAvailable(System::Object^ sender, EventArgs^ e) override;
		virtual void OnCloseDevice(System::Object^ sender, EventArgs^ e) override;

		void DataConsumerLogAuto::SaveRaw32From8(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda);
		void DataConsumerLogAuto::SaveRaw32From16(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda);
		void DataConsumerLogAuto::SaveRaw16From8(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda);
		void DataConsumerLogAuto::SaveRaw16From32(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda);
		void DataConsumerLogAuto::SaveRaw8From16(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda);
		void DataConsumerLogAuto::SaveRaw8From32(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda);
		void DataConsumerLogAuto::SaveRaw16(DataGeneratorInterface^dri,NewDataAvailableEventArgs^ nda);
		void DataConsumerLogAuto::SaveRaw32(DataGeneratorInterface^dri, NewDataAvailableEventArgs^ nda);
		void DataConsumerLogAuto::SaveRaw8(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda);
	};



}