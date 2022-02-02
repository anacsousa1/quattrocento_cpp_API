#pragma once
#pragma once
#include "GenericAdapter.h"
namespace OTComm
{
	namespace Syncstation
	{
		public ref class AdapterMuoviDevice : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			unsigned int mSamplingFrequency = 0;
		public:
			AdapterMuoviDevice() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "32 channels device"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "Muovi"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Muovi.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 32; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 32; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 32; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 1; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }				
			property unsigned int SamplingFrequency { unsigned int get() { return mSamplingFrequency; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }
			void SetSamplingFrequency(unsigned int val) { mSamplingFrequency = val; }
		};

		public ref class AdapterMuoviPiuDevice : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			unsigned int mSamplingFrequency = 0;

		public:
			AdapterMuoviPiuDevice() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "64 channels device"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "Muovi+"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Muovi+.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 62; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 64; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 1; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			property unsigned int SamplingFrequency { unsigned int get() { return mSamplingFrequency; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }
			void SetSamplingFrequency(unsigned int val) { mSamplingFrequency = val; }

		};

		public ref class AdapterSessantaquattroDevice : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			unsigned int mSamplingFrequency = 0;

		public:
			AdapterSessantaquattroDevice() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "64 channels device"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "Sessantaquattro"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Sessantaquattro.jpg", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 32; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 64; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 1; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			property unsigned int SamplingFrequency { unsigned int get() { return mSamplingFrequency; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }
			void SetSamplingFrequency(unsigned int val) { mSamplingFrequency = val; }

		};

		public ref class AdapterDueDevice : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;			
			unsigned int mSamplingFrequency = 0;
		public:
			AdapterDueDevice() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "2 channels device"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "Due+"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Due.jpg", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 2; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 2; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 2; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 1; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			property unsigned int SamplingFrequency { unsigned int get() { return mSamplingFrequency; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }
			void SetSamplingFrequency(unsigned int val) { mSamplingFrequency = val; }

		};

		///<summary>Generic Adapter for AUX</summary>
		public ref class AdapterSyncstationAux : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			double mScaleFactor = 1;
			String ^  mUnitOfMeasurement = "V";
		public:
			AdapterSyncstationAux() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "Direct connection to Syncstation Input"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "Direct connection to Syncstation Input"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("DirectConnection.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int  MaxNumChannels { unsigned int get() override { return 1; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 1; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 1; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }

			//Aux Scale Factor
			property double ScaleFactor { double get() { return mScaleFactor; } }
			property String^ UnitOfMeasurement { String^ get() override { return mUnitOfMeasurement; }}
			void SetScaleFactor(double val) {
				if (val == 0) {
					mScaleFactor = 1;
				}
				else {
					mScaleFactor = val;
				}
			}
			void SetUnitOfMeasurement(String^ val) { if (val != "") mUnitOfMeasurement = val; }

		};
		
		///<summary>Generic Adapter for AUX</summary>
		public ref class AdapterAux : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			double mScaleFactor = 1;
			String ^  mUnitOfMeasurement = "V";
		public:
			AdapterAux() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "Direct connection to Auxiliary Input"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "Direct connection to Auxiliary Input"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("DirectConnection.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int  MaxNumChannels { unsigned int get() override { return 1; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 1; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 1; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }

			//Aux Scale Factor
			property double ScaleFactor { double get() { return mScaleFactor; } }
			property String^ UnitOfMeasurement { String^ get() override { return mUnitOfMeasurement; }}
			void SetScaleFactor(double val) {
				if (val == 0) {
					mScaleFactor = 1;
				}
				else {
					mScaleFactor = val;
				}
			}
			void SetUnitOfMeasurement(String^ val) { if (val != "") mUnitOfMeasurement = val; }

		};


		public ref class AdapterQuaternions : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			double mScaleFactor = 1;
			String ^  mUnitOfMeasurement = "A.U";
		public:
			AdapterQuaternions() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "Adapter to contain 4 quaternion signal"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterQuaternions"; }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int  MaxNumChannels { unsigned int get() override { return 4; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 1; }}
			
				///<summary>max size sensor available for adapter</summary>
				///<returns>max size sensor available for adapter</returns>
				property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
				property unsigned int AdapterGain { unsigned int get() override { return 1; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }

			//Aux Scale Factor
			property double ScaleFactor { double get() { return mScaleFactor; } }
			property String^ UnitOfMeasurement { String^ get() override { return mUnitOfMeasurement; }}
			void SetScaleFactor(double val) {
				if (val == 0) {
					mScaleFactor = 1;
				}
				else {
					mScaleFactor = val;
				}
			}
			void SetUnitOfMeasurement(String^ val) { if (val != "") mUnitOfMeasurement = val; }

		};

		///<summary>AdapterLoad</summary>
		public ref class AdapterLoadCell : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			double mScaleFactor = 1;
			String ^  mUnitOfMeasurement = "V";
		public:
			AdapterLoadCell() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "Load Cell"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterLoadCell"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("DirectConnection.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int  MaxNumChannels { unsigned int get() override { return 1; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int  MinSizeSensor { unsigned int get() override { return 1; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int  AdapterGain { unsigned int get() override { return 1; }}

			property double ScaleFactor { double get() { return mScaleFactor; } }
			property String^ UnitOfMeasurement { String^ get() override { return mUnitOfMeasurement; }}
			void SetScaleFactor(double val) {
				if (val == 0) {
					mScaleFactor = 1;
				}
				else {
					mScaleFactor = val;
				}
			}
			void SetUnitOfMeasurement(String^ val) { if (val != "") mUnitOfMeasurement = val; }

		};

		///<summary>Generic Adapter for AUX</summary>
		public ref class AdapterControl : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
		public:
			AdapterControl() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "AdapterControl"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterControl"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("DirectConnection.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int  MaxNumChannels { unsigned int get() override { return 1; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 1; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 1; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }
		};
	}
}