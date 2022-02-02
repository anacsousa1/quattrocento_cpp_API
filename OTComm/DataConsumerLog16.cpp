#include "DataConsumerLog16.h"
#include "DebugLog.h"


namespace OTComm {

	DataConsumerLog16::DataConsumerLog16(String^ FileName) :DataConsumerLog(FileName)
	{

	}

	DataConsumerLog16::DataConsumerLog16() : DataConsumerLog()
	{

	}

	/// <summary>
	/// Called when new data has been received from the associate DataGenerator. The data is first validated (if a valid DataValidator was supplied) and then is saved to disk.
	/// </summary>
	/// <param name="dri">The dri.</param>
	/// <param name="e">The e.</param>
	void DataConsumerLog16::OnNewDataAvailable(Object^ dri, EventArgs^ e)
	{
		if (mRecording)
		{
			if (e == nullptr)
				return;
			NewDataAvailableEventArgs^ nda = (NewDataAvailableEventArgs^)e;		
			if (GetAssociatedSourceIndex() >= 0 && GetAssociatedSourceIndex() != nda->Data->source)
				return;
			array<unsigned __int16>^data = (nda->Data)->data16;
			if (data == nullptr) return;
			int length = (nda->Data)->length;
			int channels= (nda->Data)->channels	;
			int lengthBytes=0;
			if ((mBuffer == nullptr)||(length!=mPrevLength))
			{
				mBuffer = gcnew array < unsigned char >(length * sizeof(short));
				mPrevLength = length;
			}
			try
			{
				//is there a valid validator?
				if ((mValidator == nullptr) && (mValidatorExtra == nullptr))
				{
					//no validator, copy all the data in the char buffer					
					lengthBytes = length * sizeof(short);
					Buffer::BlockCopy(data, 0, mBuffer, 0, lengthBytes);					

				}
				else
				{
					//there is a validator: validate the data
					List<ValidatorInterval^>^ interval = nullptr;
					List<ValidatorInterval^>^ intervalExtra = nullptr;
					if (mValidator)
						interval = mValidator->Validate((DataGeneratorInterface^)dri, nda->Data);
					if (mValidatorExtra)
						intervalExtra = mValidatorExtra->Validate((DataGeneratorInterface^)dri, nda->Extra);

					interval = ValidatorIntervalMerger::Merge(interval, intervalExtra);

					//is there at least a chunk of valid data?
					if (interval != nullptr && interval->Count!=0)
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
							Buffer::BlockCopy(data, startByte, mBuffer, lengthBytes, sizeBytes);
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
			catch (Exception^ ex)
			{

			}
		}
	}
}