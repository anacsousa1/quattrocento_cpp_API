#include "DataConsumerLog.h"
#include "DebugLog.h"

namespace OTComm {
	DataConsumerLog::DataConsumerLog(String^ filename) :
		mRecording(false), mBuffer(nullptr), mIsvalidFile(false), mSavedBytes(0), mLastWriteAction(DateTime::MaxValue)
	{
 		FileName = filename;
	}

	DataConsumerLog::DataConsumerLog() :DataConsumerLog(nullptr)
	{
	}

	DataConsumerLog::~DataConsumerLog()
	{
		//close the file
		CloseFileIfOpen();
	}

	String^ DataConsumerLog::GetConsumerXMLConfiguration()
	{
		return "";
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="gen"></param>
	/// <returns></returns>
	void DataConsumerLog::Register(DataGeneratorInterface^ gen,  __int16 sourceindex)
	{
		try
		{
			this->SetDataGeneratorInterface(gen, sourceindex);
			gen->OnOpenDevice += gcnew OTComm::OpenDevice(this, &OTComm::DataConsumerLog::OnOpenDevice);
			gen->OnNewDataAvailable += gcnew OTComm::NewDataAvailable(this, &OTComm::DataConsumerLog::OnNewDataAvailable);
			gen->OnCloseDevice += gcnew OTComm::CloseDevice(this, &OTComm::DataConsumerLog::OnCloseDevice);
		}
		catch(Exception^)
		{
			throw gcnew RegistrationFailedException(__FUNCTION__);
		}
	}

	void DataConsumerLog::Unregister(DataGeneratorInterface^ gen )
	{
		try
		{
			this->SetDataGeneratorInterface(nullptr,-1);
			gen->OnOpenDevice -= gcnew OTComm::OpenDevice(this, &OTComm::DataConsumerLog::OnOpenDevice);
			gen->OnNewDataAvailable -= gcnew OTComm::NewDataAvailable(this, &OTComm::DataConsumerLog::OnNewDataAvailable);
			gen->OnCloseDevice -= gcnew OTComm::CloseDevice(this, &OTComm::DataConsumerLog::OnCloseDevice);
		}
		catch (Exception^)
		{
			throw gcnew RegistrationFailedException(__FUNCTION__);
		}
	}

	void DataConsumerLog::OnOpenDevice(Object^ dri, EventArgs^ e)
	{

	}
	
	/// <summary>
	/// Return a synthetic description of the DataConsumer.
	/// </summary>
	/// <returns>A string similar to "Data logger saving data to " followed by the name of the currently set 
	/// file where the received data is stored</returns>
	String^ DataConsumerLog::GetConsumerDescription()
	{
		return "Data logger saving data to " + mFileName;
	}

	void DataConsumerLog::OnCloseDevice(Object^ dri, EventArgs^ e)
	{
		CloseFileIfOpen();
	}

	void DataConsumerLog::ResetSavedBytes()
	{
		mSavedBytes = 0;
	}

	__int64 DataConsumerLog::SavedBytes::get()
	{
		return mSavedBytes;
	}
	
	void DataConsumerLog::ResetLastSavedDateTime()
	{
		mLastWriteAction = DateTime::MaxValue;
	}

	DateTime^ DataConsumerLog::LastSavedDateTime::get()
	{
		return mLastWriteAction;
	}


	void DataConsumerLog::FileName::set(String^ filename)
	{
		if (mRecording == true)
			// throw an exception if already recording
			throw gcnew RecordInProgressException(__FUNCTION__);

		// open the file
		if (!String::IsNullOrEmpty(filename))		
		{
			try
			{
				//try to open new file
				System::IO::FileStream^ tmp= File::Open(filename, FileMode::Create, FileAccess::Write, FileShare::None);
				//it works, close the current file if it is already open
				mIsvalidFile = false;
				CloseFileIfOpen();
				//you we get here, no error occurred, and we can assign the new stream to the class property
				mRecordingFileStream = tmp;
				LOG_MESSAGE("***" + GetConsumerDescription() + ": file opened");				
				mIsvalidFile = true;
				mFileName = filename;
				
			}
			catch (Exception^ e)
			{
				//if an exception occurred, simply rethrow: mIsvalidFile is set consistently in the try block
				throw e;
			}
		}
	}

	String^ DataConsumerLog::FileName::get(void)
	{
		return mFileName;
	}
		
	void DataConsumerLog::CloseFileIfOpen()
	{
		if (mIsvalidFile)
		{
			mRecording = false;

			try
			{
				mRecordingFileStream->Close();
				delete mRecordingFileStream;
				mIsvalidFile = false;
				mFileName = nullptr;
				System::Diagnostics::Debug::Print("***" + GetConsumerDescription() + " file closed");
			}
			catch (Exception^ e)
			{
				mIsvalidFile = false;
				mFileName = nullptr;
				System::Diagnostics::Debug::Print("***" + GetConsumerDescription() + " file NOT closed; there is a problem");
				throw e;
			}
		}
	}

	bool DataConsumerLog::IsRecording()
	{
		return mRecording;
	}


	void DataConsumerLog::ResetWriteStats()
	{
		ResetSavedBytes();
		ResetLastSavedDateTime();
	}

	void DataConsumerLog::UpdateWriteStats(int lengthBytes)
	{
		mLastWriteAction = DateTime::Now;
		//update total number of written bytes
		mSavedBytes += lengthBytes;
	}

	void DataConsumerLog::StartRecording()
	{
		if (mIsvalidFile)
		{
			ResetWriteStats();
			mRecording = true;
			LOG_MESSAGE("***" + GetConsumerDescription() + ": enabled");		
		}
		else
		{			
			throw gcnew InvalidFileException(__FUNCTION__);
		}
	}

	void DataConsumerLog::StopRecording()
	{
		CloseFileIfOpen();		
	}
}