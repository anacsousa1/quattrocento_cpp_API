#include "DataConsumerLog8.h"
#include "DebugLog.h"

namespace OTComm {



	DataConsumerLog8::DataConsumerLog8(String^ FileName, DataToKeep Keep)
		:DataConsumerLog(FileName), mKeep(Keep)
	{

	}

	DataConsumerLog8::DataConsumerLog8() : DataConsumerLog()
	{

	}

	void DataConsumerLog8::OnNewDataAvailable(Object^ dri, EventArgs^ e)
	{

		if (mRecording)
		{
			if (e == nullptr)
				return;
			NewDataAvailableEventArgs^ nda = (NewDataAvailableEventArgs^)e;
			if (GetAssociatedSourceIndex() >= 0 && GetAssociatedSourceIndex() != nda->Data->source)
				return;
			array<unsigned short>^data = (nda->Data)->data16;
			if (data == nullptr)
				return;
			int length = (nda->Data)->length;
			int channels = (nda->Data)->channels;
			int lengthBytes = 0;
			//if (mBuffer == nullptr)
			//	mBuffer = gcnew array < unsigned char >(length * sizeof(short));
			try
			{
				if (mBuffer == nullptr)
					mBuffer = gcnew array < unsigned char >(length * sizeof(unsigned char));
				//is there a valid validator?
				if (mValidator == nullptr && mValidatorExtra == nullptr)
				{
					//no validator, copy all the data in the char buffer					
					lengthBytes = length * sizeof(unsigned char);
					if (mKeep == DataTruncation::KeepLowerPart)
					{
						for (int k = 0; k < length; k++)
						{
							mBuffer[k] = (unsigned char)data[k];
						}
					}
					else
					{
						for (int k = 0; k < length; k++)
						{
							mBuffer[k] = (unsigned char)(data[k] / 256);
						}
					}


					//Buffer::BlockCopy(data, 0, mBuffer, 0, lengthBytes);

				}
				else
				{
					//there is a validator: validate the data
					List<ValidatorInterval^>^ interval = nullptr;
					List<ValidatorInterval^>^ intervalextra = nullptr;

					if (mValidator)
						interval = mValidator->Validate((DataGeneratorInterface^)dri, (nda->Data));

					if (mValidatorExtra)
					{
						intervalextra = mValidatorExtra->Validate((DataGeneratorInterface^)dri, (nda->Extra));
					}

					interval = ValidatorIntervalMerger::Merge(interval, intervalextra);

					//is there at least a chunk of valid data?
					if (interval != nullptr && interval->Count != 0)
					{
						//reset length, which will contain the number of bytes to save to file
						lengthBytes = 0;
						//loop over all the valid data
						for each (ValidatorInterval^ vi in interval)
						{
							//start byte
							int start = vi->FirstRow*channels;
							//end byte
							int end = vi->LastRow *channels;
							//copy that chunk into mBuffer							
							if (mKeep == DataTruncation::KeepLowerPart)
							{
								for (int k = start; k <= end; k++)
								{
									mBuffer[k] = (unsigned char)data[k];
								}
							}
							else
							{
								for (int k = start; k <= end; k++)
								{
									mBuffer[k] = (unsigned char)(data[k] / 256);
								}
							}

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
			catch (Exception^ exc)
			{
				System::Diagnostics::Debug::WriteLine("-----------------------------------------recording failed" + exc->Message);
			}
		}
	}

}

