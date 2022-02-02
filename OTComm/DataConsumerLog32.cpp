#include "DataConsumerLog32.h"
#include "DebugLog.h"

namespace OTComm {

	DataConsumerLog32::DataConsumerLog32(String^ FileName) :DataConsumerLog(FileName)
	{

	}

	DataConsumerLog32::DataConsumerLog32() : DataConsumerLog()
	{

	}

	/// <summary>
	/// Called when new data has been received from the associate DataGenerator. The data is first validated (if a valid DataValidator was supplied) and then is saved to disk.
	/// </summary>
	/// <param name="dri">The dri.</param>
	/// <param name="e">The e.</param>
	void DataConsumerLog32::OnNewDataAvailable(Object^ dri, EventArgs^ e)
	{
		if (mRecording)
		{
			if (e == nullptr)
				return;
			NewDataAvailableEventArgs^ nda = (NewDataAvailableEventArgs^)e;
			if (GetAssociatedSourceIndex() >= 0 && GetAssociatedSourceIndex() != nda->Data->source)
				return;
			array<unsigned __int32>^data = (nda->Data)->data32;
			if (data == nullptr) return;
			int length = (nda->Data)->length;
			int channels = (nda->Data)->channels;
			int lengthBytes = 0;
			if (mBuffer == nullptr)
				mBuffer = gcnew array < unsigned char >(length * sizeof(__int32));
			try
			{
				//is there a valid validator?
				if ((mValidator == nullptr) && (mValidatorExtra == nullptr))
				{
					//no validator, copy all the data in the char buffer					
					lengthBytes = length * sizeof(__int32);
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
			catch (Exception^)
			{

			}
		}
	}
}