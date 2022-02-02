#include "DataConsumerLogAuto.h"
#include "DebugLog.h"

namespace OTComm {

	DataConsumerLogAuto::DataConsumerLogAuto(String^ FileName, DataType DataSize) :DataConsumerLog(FileName), 
		m_DataSize(DataSize), m_DataSizeUsed(DataSize)
	{

	}

	DataConsumerLogAuto::DataConsumerLogAuto() : DataConsumerLog()
	{

	}

	/// <summary>
	/// Called when new data has been received from the associate DataGenerator. The data is first validated (if a valid DataValidator was supplied) and then is saved to disk.
	/// </summary>
	/// <param name="dri">The dri.</param>
	/// <param name="e">The e.</param>
	void DataConsumerLogAuto::OnNewDataAvailable(Object^ dri, EventArgs^ e)
	{
		if (mRecording)
		{
			if (e == nullptr)
				return;
			NewDataAvailableEventArgs^ nda = (NewDataAvailableEventArgs^)e;
			if (GetAssociatedSourceIndex() >= 0 && GetAssociatedSourceIndex() != nda->Data->source)
				return;
			
			

			if (m_DataSizeUsed == DataTypes::Auto)
			{
				if ((nda->Data)->data16 != nullptr)
					m_DataSizeUsed = DataTypes::SixteenBits;
				else if ((nda->Data)->data32 != nullptr)
					m_DataSizeUsed = DataTypes::ThirtytwoBits;
				else if ((nda->Data)->data8 != nullptr)
					m_DataSizeUsed = DataTypes::HeightBits;
			}
			

			if (m_DataSizeUsed == DataTypes::SixteenBits && (nda->Data)->data16 != nullptr)
			{
				SaveRaw16((DataGeneratorInterface^)dri,nda);
			}
			else if (m_DataSizeUsed == DataTypes::ThirtytwoBits && (nda->Data)->data32 != nullptr)
			{
				SaveRaw32((DataGeneratorInterface^)dri, nda);
			}
			else if (m_DataSizeUsed == DataTypes::HeightBits && (nda->Data)->data8 != nullptr)
			{
				SaveRaw8((DataGeneratorInterface^)dri, nda);
			}
			else if (m_DataSizeUsed == DataTypes::SixteenBits)
			{
				if ((nda->Data)->data32 != nullptr)
					SaveRaw16From32((DataGeneratorInterface^)dri, nda);
				else if ((nda->Data)->data8 != nullptr)
					SaveRaw8From32((DataGeneratorInterface^)dri, nda);
			}
			else if (m_DataSizeUsed == DataTypes::ThirtytwoBits)
			{
				if ((nda->Data)->data16 != nullptr)
					SaveRaw32From16((DataGeneratorInterface^)dri, nda);
				else if ((nda->Data)->data8 != nullptr)
					SaveRaw32From8((DataGeneratorInterface^)dri, nda);
			}
			else if (m_DataSizeUsed == DataTypes::HeightBits)
			{
				if ((nda->Data)->data32 != nullptr)
					SaveRaw8From32((DataGeneratorInterface^)dri, nda);
				else if ((nda->Data)->data16 != nullptr)
					SaveRaw8From16((DataGeneratorInterface^)dri, nda);
			}
			
		}
	}

	void DataConsumerLogAuto::SaveRaw16(DataGeneratorInterface^ dri,NewDataAvailableEventArgs^ nda)
	{
		int length = (nda->Data)->length;
		int channels = (nda->Data)->channels;
		int lengthBytes = 0;
		if (mBuffer == nullptr)
			mBuffer = gcnew array < unsigned char >(length * sizeof(__int16));
		try
		{
			array<unsigned __int16>^data = (nda->Data)->data16;
			//is there a valid validator?
			if (mValidator == nullptr)
			{
				//no validator, copy all the data in the char buffer					
				lengthBytes = length * sizeof(__int16);
				Buffer::BlockCopy(data, 0, mBuffer, 0, lengthBytes);
			}
			else
			{
				//there is a validator: validate the data
				List<ValidatorInterval^>^ interval = mValidator->Validate(dri, nda->Data);
				//is there at least a chunk of valid data?
				if (interval != nullptr && interval->Count != 0)
				{
					//reset length, which will contain the number of bytes to save to file
					lengthBytes = 0;
					//loop over all the valid data
					for each (ValidatorInterval^ vi in interval)
					{
						//start byte
						int startByte = vi->FirstRow*channels * sizeof(short);
						//end byte
						int sizeBytes = (vi->LastRow - vi->FirstRow + 1)*channels * sizeof(short);
						//copy that chunk into mBuffer
						Buffer::BlockCopy(data, lengthBytes, mBuffer, startByte, sizeBytes);
						lengthBytes += sizeBytes;
					}
				}
			}
			//if data is ready to be saved
			if (lengthBytes > 0)
			{
				mRecordingFileStream->Write(mBuffer, 0, lengthBytes);
				UpdateWriteStats(lengthBytes);
			}
		}
		catch (Exception^)
		{

		}
	}


	void DataConsumerLogAuto::SaveRaw32(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda)
	{
		int length = (nda->Data)->length;
		int channels = (nda->Data)->channels;
		int lengthBytes = 0;
		if (mBuffer == nullptr)
			mBuffer = gcnew array < unsigned char >(length * sizeof(__int32));
		try
		{
			array<unsigned __int32>^data = (nda->Data)->data32;
			//is there a valid validator?
			if (mValidator == nullptr)
			{
				//no validator, copy all the data in the char buffer					
				lengthBytes = length * sizeof(__int16);
				Buffer::BlockCopy(data, 0, mBuffer, 0, lengthBytes);

			}
			else
			{
				//there is a validator: validate the data
				List<ValidatorInterval^>^ interval = mValidator->Validate(dri, nda->Data);
				//is there at least a chunk of valid data?
				if (interval != nullptr && interval->Count != 0)
				{
					//reset length, which will contain the number of bytes to save to file
					lengthBytes = 0;
					//loop over all the valid data
					for each (ValidatorInterval^ vi in interval)
					{
						//start byte
						int startByte = vi->FirstRow*channels * sizeof(__int32);
						//end byte
						int sizeBytes = (vi->LastRow - vi->FirstRow + 1)*channels * sizeof(__int32);
						//copy that chunk into mBuffer
						Buffer::BlockCopy(data, lengthBytes, mBuffer, startByte, sizeBytes);
						lengthBytes += sizeBytes;
					}
				}
			}
			//if data is ready to be saved
			if (lengthBytes > 0)
			{
				mRecordingFileStream->Write(mBuffer, 0, lengthBytes);
				UpdateWriteStats(lengthBytes);
			}
		}
		catch (Exception^)
		{

		}
	}

	void DataConsumerLogAuto::SaveRaw8(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda)
	{
		int length = (nda->Data)->length;
		int channels = (nda->Data)->channels;
		int lengthBytes = 0;
		if (mBuffer == nullptr)
			mBuffer = gcnew array < unsigned char >(length * sizeof(__int8));
		try
		{
			array<unsigned __int8>^data = (nda->Data)->data8;
			//is there a valid validator?
			if (mValidator == nullptr)
			{
				//no validator, copy all the data in the char buffer					
				lengthBytes = length * sizeof(__int8);
				Buffer::BlockCopy(data, 0, mBuffer, 0, lengthBytes);

			}
			else
			{
				//there is a validator: validate the data
				List<ValidatorInterval^>^ interval = mValidator->Validate(dri, nda->Data);
				//is there at least a chunk of valid data?
				if (interval != nullptr && interval->Count != 0)
				{
					//reset length, which will contain the number of bytes to save to file
					lengthBytes = 0;
					//loop over all the valid data
					for each (ValidatorInterval^ vi in interval)
					{
						//start byte
						int startByte = vi->FirstRow*channels * sizeof(__int8);
						//end byte
						int sizeBytes = (vi->LastRow - vi->FirstRow + 1)*channels * sizeof(__int8);
						//copy that chunk into mBuffer
						Buffer::BlockCopy(data, lengthBytes, mBuffer, startByte, sizeBytes);
						lengthBytes += sizeBytes;
					}
				}
			}
			//if data is ready to be saved
			if (lengthBytes > 0)
			{
				mRecordingFileStream->Write(mBuffer, 0, lengthBytes);
				UpdateWriteStats(lengthBytes);
			}
		}
		catch (Exception^)
		{

		}
	}

	void DataConsumerLogAuto::SaveRaw32From8(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda)
	{
		throw gcnew NotImplementedException();
	}

	void DataConsumerLogAuto::SaveRaw32From16(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda)
	{
		throw gcnew NotImplementedException();
	}

	void DataConsumerLogAuto::SaveRaw16From8(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda)
	{
		throw gcnew NotImplementedException();
	}

	void DataConsumerLogAuto::SaveRaw16From32(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda)
	{
		throw gcnew NotImplementedException();
	}

	void DataConsumerLogAuto::SaveRaw8From16(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda)
	{
		throw gcnew NotImplementedException();
	}

	void DataConsumerLogAuto::SaveRaw8From32(DataGeneratorInterface^ dri, NewDataAvailableEventArgs^ nda)
	{
		throw gcnew NotImplementedException();
	}

	void DataConsumerLogAuto::OnCloseDevice(System::Object^ sender, EventArgs^ e)
	{
		m_DataSizeUsed == m_DataSize;
		DataConsumerLog::OnCloseDevice(sender, e);
	}
}