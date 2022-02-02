#include "DataValidator.h"

namespace OTComm
{
	DataValidator::DataValidator() : mLastID(-1), mInitialized(false)
	{
		mListValidatorIntervals = gcnew List<ValidatorInterval^>;
	}

	bool DataValidator::Initialize(DataGeneratorInterface^ gen)
	{
		mInitialized = true;
		return true;
	}

	void DataValidator::Reset()
	{
		mInitialized = false;
		mListValidatorIntervals->Clear();
	}

	List<ValidatorInterval^>^ DataValidator::Validate(DataGeneratorInterface^ gen, OTData^ nda)
	{
		if (!mInitialized)
			//if required, initialize it now
			Initialize(gen);
		return mListValidatorIntervals;
	}

	DataValidatorNoCheck::DataValidatorNoCheck() :DataValidator()
	{
	}

	bool DataValidatorTrigger::Initialize(DataGeneratorInterface^ gen)
	{
		DataValidator::Initialize(gen);
		array<unsigned int>^ Channels = gen->GetAcquiredChannels();
		for (int k = 0; Channels->Length; k++)
		{
			if (Channels[k] == mChannel)
			{
				array<unsigned int>^ Mappings = gen->GetChannelsMapping();
				if (Mappings == nullptr)
					mIndexchannelsUnderAnalysis = Channels[k];
				else
					mIndexchannelsUnderAnalysis = Mappings[Channels[k]];
				return true;
			}
		}
		mIndexchannelsUnderAnalysis = -1;
		return false;
	}

	bool DataValidatorTrigger::Initialize(DataGeneratorInterface^ gen, int ch)
	{
		DataValidator::Initialize(gen);
		mIndexchannelsUnderAnalysis = ch;
		return true;
	}

	DataValidatorTrigger::DataValidatorTrigger(int channel, int threshold, TriggerMode mode) :
		mMode(mode), mTriggerStatus(false), mChannel(channel), mThreshold(threshold)
	{
	}

	List<ValidatorInterval^>^ DataValidatorTrigger::Validate(DataGeneratorInterface^ gen, OTData^ nda)
	{
		//check if the object has already been initialized
		if (!mInitialized)
			//if required, initialize it now
			Initialize(gen, mChannel);
		//was data already processed? if so, then send the last results. 
		//This may occur if many DataConsumer share the same DataValidator
		/*if (mLastID == nda->ID)
		{
			return mListValidatorIntervals;
		}
		mLastID = nda->ID;*/
		mListValidatorIntervals->Clear();
		if (nda != nullptr)
		{
			//is the index of the channel to monitor valid?
			int channels = nda->channels;
			if (mIndexchannelsUnderAnalysis >= 0 && mIndexchannelsUnderAnalysis < channels)
			{
				if (nda->data16 != nullptr)
				{


					array<unsigned __int16>^data = nda->data16;
					int length = nda->length;

					int channel = mIndexchannelsUnderAnalysis;
					//number of received rows of data (each row contains data from all the channels)
					int rowindex = 0;
					int rowstart = 0;
					do {
						//we have 4 cases: 1)state off, trigger onraise , 2) state on, trigger onrXXXXXXXX
						if (mTriggerStatus == false)//no valid data yet
						{
							while (channel < length)
							{
								if (mMode == TriggerModes::StartRaiseStopFall ||
									mMode == TriggerModes::StartRaiseStopRaise)
								{
									if (data[channel] > mThreshold)
									{
										rowstart = rowindex;
										mTriggerStatus = true;
										//move to the next sample of this channel
										channel += channels;
										rowindex++;
										break;
									}
									channel += channels;
									rowindex++;
								}
								else if (mMode == TriggerModes::WhenEqualTo)
								{
									if (data[channel] == mThreshold)
									{
										rowstart = rowindex;
										mTriggerStatus = true;
										//move to the next sample of this channel
										channel += channels;
										rowindex++;
										break;
									}
									channel += channels;
									rowindex++;
								}
								else if (mMode == TriggerModes::WhenNotEqualTo)
								{
									if (data[channel] != mThreshold)
									{
										rowstart = rowindex;
										mTriggerStatus = true;
										//move to the next sample of this channel
										channel += channels;
										rowindex++;
										break;
									}
									channel += channels;
									rowindex++;
								}
							}
						}
						else
						{
							if (mMode == TriggerModes::StartRaiseStopFall)
							{
								while (channel < length)
								{
									if (data[channel] < mThreshold)
									{
										mListValidatorIntervals->Add(gcnew ValidatorInterval(rowstart, rowindex - 1));/// allocate here a new ValidateRange and add it to the list
										mTriggerStatus = false;
										//move to the next sample of this channel
										channel += channels;
										rowindex++;
										break;
									}
									channel += channels;
									rowindex++;

								}
							}
							else if (mMode == TriggerModes::StartRaiseStopRaise)
							{
								while (channel < length)
								{
									if (data[channel] > mThreshold)
									{
										mListValidatorIntervals->Add(gcnew ValidatorInterval(rowstart, rowindex - 1));/// allocate here a new ValidateRange and add it to the list
										mTriggerStatus = false;
										//move to the next sample of this channel
										channel += channels;
										rowindex++;
										break;
									}
									channel += channels;
									rowindex++;

								}
							}
							else if (mMode == TriggerModes::WhenEqualTo)
							{
								while (channel < length)
								{
									if (data[channel] != mThreshold)
									{
										mListValidatorIntervals->Add(gcnew ValidatorInterval(rowstart, rowindex - 1));/// allocate here a new ValidateRange and add it to the list
										mTriggerStatus = false;
										//move to the next sample of this channel
										channel += channels;
										rowindex++;
										break;
									}
									channel += channels;
									rowindex++;

								}
							}
							else if (mMode == TriggerModes::WhenNotEqualTo)
							{
								while (channel < length)
								{
									if (data[channel] == mThreshold)
									{
										mListValidatorIntervals->Add(gcnew ValidatorInterval(rowstart, rowindex - 1));/// allocate here a new ValidateRange and add it to the list
										mTriggerStatus = false;
										//move to the next sample of this channel
										channel += channels;
										rowindex++;
										break;
									}
									channel += channels;
									rowindex++;

								}
							}
						}



					} while (channel < length);
					if (mTriggerStatus == true)
					{
						mListValidatorIntervals->Add(gcnew ValidatorInterval(rowstart, rowindex - 1));/// allocate here a new ValidateRange and add it to the list
					}
				}

			}
		}

		return mListValidatorIntervals;
	}

	void DataValidatorTrigger::Reset()
	{
		DataValidator::Reset();
		mTriggerStatus = false;
	}

	List<ValidatorInterval^>^ ValidatorIntervalMerger::Merge(List<ValidatorInterval^>^ A, List<ValidatorInterval^>^ B)
	{
		if (B == nullptr || B->Count == 0)
			return A;
		//B not empty
		if (A == nullptr || A->Count == 0)
			return B;

		//merge them
		List<ValidatorInterval^>^ merged = gcnew List<ValidatorInterval^>;
		int ka = 0;
		int kb = 0;

		for (ka = 0; ka < A->Count; ka++)
		{
			ValidatorInterval^ a = A[ka];
			for (; kb < B->Count; kb++)
			{
				ValidatorInterval^ b = B[kb];
				if (a->FirstRow <= b->LastRow && a->LastRow >= b->FirstRow)
				{
					merged->Add(gcnew ValidatorInterval(min(a->FirstRow, b->FirstRow), max(a->LastRow, b->LastRow)));
					kb++;
				}
				else if (b->FirstRow > a->LastRow)
				{
					//there could be an additional intersection with the last b segment (in the it covers many A segments); therefore I move back
					kb--;
					continue;
				}
			}
		}
		return merged;
	}
}