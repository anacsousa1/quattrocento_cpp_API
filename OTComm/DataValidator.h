#pragma once
#include "DataGeneratorInterface.h"

namespace OTComm
{
	typedef unsigned int TriggerMode;
	public ref class TriggerModes abstract
	{
	public:
		static const TriggerMode StartRaiseStopRaise = 1;
		static const TriggerMode StartRaiseStopFall = 2;
		static const TriggerMode WhenEqualTo = 3;
		static const TriggerMode WhenNotEqualTo = 4;
	};

	public ref class ValidatorInterval
	{	
	protected:
		int mFirstRow;
		int mLastRow;
	public:
		property int FirstRow { int get() { return mFirstRow; } }
		property int LastRow { int get() { return mLastRow; } }
		property int Count { int get() { return mLastRow -mFirstRow+1; } }
		ValidatorInterval(int StartRowIndex, int EndRowIndex) :mFirstRow(StartRowIndex), mLastRow(EndRowIndex) {};
	};

	public ref class ValidatorIntervalMerger 
	{
	public:
		static List<ValidatorInterval^>^ Merge(List<ValidatorInterval^>^A, List<ValidatorInterval^>^B);
	};

	///<summary>Data validator. Check if a condition is satisfied before perform operations</summary>
	
	public ref class DataValidator abstract
	{	
	protected:
		bool mInitialized;
		unsigned __int32 mLastID;
		List<ValidatorInterval^>^ mListValidatorIntervals;
	public:
		DataValidator();
		///<summary>Reset the Data Validator</summary>
		virtual void Reset();
		///<summary>Initialize a DataValidator object.</summary>
		///<returns>True if initialization succeeds, False if not</returns>
		virtual bool Initialize(DataGeneratorInterface^ gen);
		///<summary>Validate Data validator.</summary>
		///<returns></returns>
		virtual List<ValidatorInterval^>^ Validate(DataGeneratorInterface^ gen, OTData^ nda);
	};


	public ref class DataValidatorNoCheck : public DataValidator
	{
	public:
		DataValidatorNoCheck();
	};

	public ref class DataValidatorTrigger : public DataValidator
	{
	protected:
		int mIndexchannelsUnderAnalysis;
		int mChannel;
		TriggerMode mMode;
		bool mTriggerStatus;
		int mThreshold;
		
	public:
		virtual void Reset() override;
		virtual bool Initialize(DataGeneratorInterface^ gen) override;
		virtual bool Initialize(DataGeneratorInterface^ gen, int ch) override;
		DataValidatorTrigger(int channel, int threshold, TriggerMode mode);
		virtual List<ValidatorInterval^>^ Validate(DataGeneratorInterface^ gen, OTData^ nda) override;
	};
}