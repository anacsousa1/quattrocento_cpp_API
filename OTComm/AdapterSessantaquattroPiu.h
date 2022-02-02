#pragma once
#include "GenericAdapter.h"
namespace OTComm
{
	namespace SessantaquattroPiu
	{
		///<summary>Generic Adapter for AUX</summary>
		public ref class AdapterAux : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			double mScaleFactor = 1;
			String^ mUnitOfMeasurement = "V";
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

		public ref class AdapterIsoAux : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			double mScaleFactor = 1;
			String^ mUnitOfMeasurement = "V";
		public:
			AdapterIsoAux() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "ISO-AUX"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "ISO-AUX"; }}
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
		///<summary>Generic Adapter for Adapter In</summary>
		public ref class AdapterSessantaquattroPiu : public GenericAdapter
		{
		public:
			AdapterSessantaquattroPiu() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "AdapterSessantaquattroPiu"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterSessantaquattroPiu"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int  MaxNumChannels { unsigned int get() override { return 16; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 1; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 5; }}

		};

		///<summary>Generic Adapter for Adapter In AD8x1SE</summary>
		public ref class AdapterInAD8x1SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInAD8x1SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "8 channels adapter bipolar acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD8x1SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD8x1SE.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 8; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 1; }}
		};

		///<summary>Generic Adapter for Adapter In AD2x32SE</summary>
		public ref class AdapterInAD2x32SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInAD2x32SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "64 channels adapter monopolar acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD2x32SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("fAD2x32SE.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 32; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 32; }}
		};

		///<summary>Generic Adapter for Adapter In AD2x32SE</summary>
		public ref class AdapterInAD2x16SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInAD2x16SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "32 channels adapter differential acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD2x16SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD2x16SE.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 32; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 16; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 16; }}
		};

		///<summary>Generic Adapter for Adapter In AD2x32SE</summary>
		public ref class AdapterInAD4x16SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInAD4x16SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "64 channels adapter monopolar acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD4x16SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int MaxSizeSensor { unsigned int get() override { return 16; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 16; }}
		};

		///<summary>Generic Adapter for Adapter In AD1x64SE</summary>
		public ref class AdapterInAD1x64SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInAD1x64SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "64 channels adapter monopolar acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD1x64SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD1x64SE.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 64; }}
		};

		///<summary>Generic Adapter for Adapter In AD1x64SE</summary>
		public ref class AdapterInADEEGCap64SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInADEEGCap64SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "64 channels adapter for EEG cap"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "ADEEGCap64SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 64; }}
		};

		///<summary>Generic Adapter for Adapter In AD1x64SE</summary>
		public ref class AdapterInADEEGCap20SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInADEEGCap20SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "20 channels adapter for EEG cap"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "ADEEGCap20SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 64; }}
		};

		///<summary>Generic Adapter for Adapter In AD1x16SE</summary>
		public ref class AdapterInAD1x16SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInAD1x16SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "16 channels adapter differential acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD1x16SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD1x16SE.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 16; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 16; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 16; }}
		};

		///<summary>Generic Adapter for Adapter In AD4x8SE</summary>
		public ref class AdapterInAD4x8SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInAD4x8SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "32 channels adapter bipolar acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD4x8SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD4x8SE.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 32; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 1; }}
		};

		///<summary>Generic Adapter for Adapter In AD4x8SE</summary>
		public ref class AdapterInAD8x4SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInAD8x4SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "32 channels adapter differential acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD8x4SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD8x4SE.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 32; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 4; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 4; }}
		};

		public ref class AdapterInAD4x4SE : public AdapterSessantaquattroPiu
		{
		public:
			AdapterInAD4x4SE() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "16 channels adapter differential acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD4x4SE"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD4x4SE.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 16; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 4; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 4; }}
		};

		public ref class AdapterQuaternions : public GenericAdapter
		{
		protected:
			unsigned int mAcquisitionMode = 0;
			double mScaleFactor = 1;
			String^ mUnitOfMeasurement = "A.U";
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