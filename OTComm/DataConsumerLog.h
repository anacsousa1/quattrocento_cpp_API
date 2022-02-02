#pragma once
#include "DataConsumerInterface.h"

using namespace System::IO;

namespace OTComm {

	///<summary>
	/// DataConsumerInterface for saving data on temporary file
	///</summary>
	public ref class DataConsumerLog abstract  : public DataConsumerInterface
	{
	public:
		///<summary>
		/// Constructor
		///</summary>
		///<param name="FileName">Path to file where to save data</param>
		DataConsumerLog(String^ FileName);
		DataConsumerLog();
		~DataConsumerLog();

		///<summary>
		/// Method to close the file if open
		///</summary>
		void CloseFileIfOpen();

		///<summary>
		/// Associate the DataGenerator to the DataConsumer
		///</summary>
		///<param name="gen">DataGeneratorInterface to associate to the DataConsumer</param>
		///<returns>Returns -1 if error, 0 if everything is fine</returns>
		virtual void Register(DataGeneratorInterface^ gen, __int16 sourceindex) override;
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
		virtual void StartRecording();
		///<summary>
		/// Stop recording on file
		///</summary>
		///<returns>Returns -1 if error, 0 if everything is fine</returns>
		virtual void StopRecording();
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
		///<summary>
		/// Filename where data are saved
		///</summary>
		property String^ FileName
		{
			String^ get();
			void set(String^ x);		
		}
		///<summary>
		/// Number of bytes saved in the file
		///</summary>
		property __int64 SavedBytes
		{
			__int64 get();
		}

		///<summary>
		/// Last saving date
		///</summary>
		property DateTime^ LastSavedDateTime
		{
			DateTime^ get();
		}

		///<summary>
		/// Recording is activated or not
		///</summary>
		bool IsRecording();


	protected :


		///<summary>
		/// Recording status
		///</summary>
		bool mRecording;

		///<summary>
		/// Validity of file
		///</summary>
		bool mIsvalidFile;

		///<summary>
		/// Previous buffer length 
		///</summary>
		int mPrevLength = 0;

		///<summary>
		/// Number of saved bytes during a writing operation
		///</summary>
		__int64 mSavedBytes;

		///<summary>
		/// Date of the last writing operation
		///</summary>
		System::DateTime^ mLastWriteAction;

		///<summary>
		/// Filestream for the logic communication with a file on a specific path 
		///</summary>
		FileStream^ mRecordingFileStream;

		///<summary>
		///Path to a file
		///</summary>
		String^ mFileName;

		///<summary>
		/// Array used for data transmission
		///</summary>
		array<unsigned char>^ mBuffer;

		///<summary>
		/// Method to reset the counter of saved bytes
		///</summary>
		void ResetSavedBytes();

		///<summary>
		/// Method to reset the last saving date
		///</summary>
		void ResetLastSavedDateTime();

		///<summary>
		/// Method to reset both the counter of saved bytes and last saving date
		///</summary>
		void ResetWriteStats();

		///<summary>
		/// Method to updates the status of writing operation, the last saving date is updated with the current date of the function call and the saved bytes is set equal to lengthBytes
		///</summary>
		///<param name="lengthBytes">Number of transferred bytes</param>
		void UpdateWriteStats(int lengthBytes);
		///<summary>
		/// Event on open device
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnOpenDevice(System::Object^ sender, EventArgs^ e);
		///<summary>
		/// Event on close device
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnCloseDevice(System::Object^ sender, EventArgs^ e);
		///<summary>
		/// Event on new data available
		///</summary>
		///<param name="sender">Sender</param>
		///<param name="e">Arguments</param>
		virtual void OnNewDataAvailable(System::Object^ sender, EventArgs^ e)=0;

	};



}