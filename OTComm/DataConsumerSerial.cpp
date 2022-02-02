#include "DataConsumerSerial.h"
#include "DebugLog.h"

namespace OTComm {

	DataConsumerSerial::DataConsumerSerial() :
		mBuffer(nullptr), mValidator(nullptr), mReadBytes(0)
	{
	}

	int DataConsumerSerial::Read(int Channel, double^ data)
	{
		return 0;
	}

	DataConsumerSerial::~DataConsumerSerial()
	{		
	}

	String^ DataConsumerSerial::GetConsumerXMLConfiguration()
	{
		return "";
	}

	void DataConsumerSerial::Register(DataGeneratorInterface^ gen)
	{
		try
		{
			this->SetDataGeneratorInterface(gen);
			gen->Events->OnStartAcquisition += gcnew OTComm::StartAcquisition(this, &OTComm::DataConsumerSerial::OnStartAcquisition);
			gen->Events->OnNewDataAvailable += gcnew OTComm::NewDataAvailable(this, &OTComm::DataConsumerSerial::OnNewDataAvailable);
			gen->Events->OnStopAcquisition += gcnew OTComm::StopAcquisition(this, &OTComm::DataConsumerSerial::OnStopAcquisition);
		}
		catch(Exception^)
		{
			throw gcnew RegistrationFailedException();
		}
	}

	int DataConsumerSerial::Unregister(DataGeneratorInterface^ gen)
	{
		try
		{
			this->SetDataGeneratorInterface(nullptr);
			gen->Events->OnStartAcquisition -= gcnew OTComm::StartAcquisition(this, &OTComm::DataConsumerSerial::OnStartAcquisition);
			gen->Events->OnNewDataAvailable -= gcnew OTComm::NewDataAvailable(this, &OTComm::DataConsumerSerial::OnNewDataAvailable);
			gen->Events->OnStopAcquisition -= gcnew OTComm::StopAcquisition(this, &OTComm::DataConsumerSerial::OnStopAcquisition);
		}
		catch (Exception^)
		{
			throw gcnew RegistrationFailedException();
		}
	}

	void DataConsumerSerial::OnStartAcquisition(Object^ dri, EventArgs^ e)
	{
		//allocate memory and start thread
	}


	String^ DataConsumerSerial::GetConsumerDescription()
	{

		return "Serial reader";
	}

	void DataConsumerSerial::OnStopAcquisition(Object^ dri, EventArgs^ e)
	{
		//free memory
	}

	void DataConsumerSerial::ResetSavedBytes()
	{
		mReadBytes = 0;
	}

	long^ DataConsumerSerial::SavedBytes::get()
	{
		return mReadBytes;
	}

	void DataConsumerSerial::OnNewDataAvailable(Object^ dri, EventArgs^ e)
	{
		//save the buffer, and make it available for reading in Read Function
		//
		/*if (mRecording)
		{
			if (e == nullptr)
				return;
			array<unsigned short>^data = ((NewDataAvailableEventArgs^)e)->data;
			int length = ((NewDataAvailableEventArgs^)e)->length;
			if (mBuffer == nullptr)
				mBuffer = gcnew array < unsigned char >(length * sizeof(short));
			Buffer::BlockCopy(data, 0, mBuffer, 0, length * sizeof(short));
			try
			{
				if (Validator == nullptr || Validator->Validate((DataGeneratorInterface^)dri, data, length))
				{
					mRecordingFileStream->Write(mBuffer, 0, length * sizeof(short));
					mSavedBytes += length;
				}
			}
			catch (Exception^)
			{
				System::Diagnostics::Debug::WriteLine("-----------------------------------------recording failed");
			}
			
		}*/
	}
}
