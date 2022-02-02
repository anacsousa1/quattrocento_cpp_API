#pragma once
#include "GenericAdapter.h"
namespace OTComm
{
	namespace Due
	{
		///<summary>Adapter for Due</summary>
		public ref class AdapterDue : public GenericAdapter
		{
		public:
			AdapterDue() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "AdapterDue"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterDue"; }}
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
			property unsigned int  AdapterGain { unsigned int get() override { return 5; }}
		};
		///<summary>AdapterAUXDirectIn</summary>
		public ref class AdapterAuxDirectIn : public AdapterDue
		{
		public:
			AdapterAuxDirectIn() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "AUX channel Direct In"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterAuxDirectIn"; }}
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
			property unsigned int  AdapterGain { unsigned int get() override { return 5; }}
		};
		///<summary>AdapterEMGDirectIn</summary>
		public ref class AdapterEMGDirectIn : public AdapterDue
		{
		public:
			AdapterEMGDirectIn() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "EMG channel Direct In"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterEMGDirectIn"; }}
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
			property unsigned int  AdapterGain { unsigned int get() override { return 5; }}
		};
		///<summary>AdapterEMGDirectIn</summary>
		public ref class AdapterLoadDirectIn : public AdapterDue
		{
		public:
			AdapterLoadDirectIn() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "Load Cell Direct In"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterLoadDirectIn"; }}
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
			property unsigned int  AdapterGain { unsigned int get() override { return 5; }}
		};
		///<summary>AdapterEMGDirectIn</summary>
		public ref class AdapterIMU : public AdapterDue
		{
		public:
			AdapterIMU() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "IMU Direct In"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterIMU"; }}
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
			property unsigned int  AdapterGain { unsigned int get() override { return 5; }}
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
	}
}