#pragma once
#include "GenericAdapter.h"
namespace OTComm
{
	namespace EMGUSB
	{
		///<summary>Generic Adapter for Multiple In</summary>
		public ref class AdapterMultipleIn : public GenericAdapter
		{
		public:
			AdapterMultipleIn() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "AdapterMultipleIn"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterMultipleIn"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int  MaxNumChannels { unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int  MinSizeSensor { unsigned int get() override { return 1; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 5; }}
		};
		/*///<summary>Generic Adapter for Multiple In RXERW3432</summary>
		public ref class AdapterMultipleInRXERW3432 : public AdapterMultipleIn
		{
		public:
			AdapterMultipleInRXERW3432() {}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "AdapterMultipleInRXERW3432"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterMultipleInRXERW3432"; }}
		};*/
		///<summary>Generic Adapter for Multiple In AD1x64</summary>
		public ref class AdapterMultipleInAD1x64 : public AdapterMultipleIn
		{
		public:
			AdapterMultipleInAD1x64() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "64 channels single adapter"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD1x64"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD1x64.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 64; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 64; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 5; }}
		};

		public ref class AdapterMultipleInAD64 : public AdapterMultipleIn
		{
		protected:
			unsigned int mAcquisitionMode = 0;
		public:
			AdapterMultipleInAD64() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "64 channels single adapter"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD64"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD64.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 64; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 64; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 5; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = 0; }
		};
		///<summary>Generic Adapter for Multiple In AD2x32</summary>
		public ref class AdapterMultipleInAD2x32 : public AdapterMultipleIn
		{
		public:
			AdapterMultipleInAD2x32() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "32 channels double adapter"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD2x32"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD2x32.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 32; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 32; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 5; }}
		};
		///<summary>Generic Adapter for Multiple In AD1x64</summary>
		public ref class AdapterMultipleSplitter : public AdapterMultipleIn
		{
		protected:
			unsigned int mAcquisitionMode = 0;
		public:
			AdapterMultipleSplitter() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "Splitter 64 channels"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "SP-Box-QR"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("SP-Box-QR.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 64; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 64; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 64; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain { unsigned int get() override { return 5; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }
		};
		///<summary>Generic Adapter for Multiple In AD1x64</summary>
		public ref class AdapterMultipleSplitter32 : public AdapterMultipleIn
		{
		protected:
			unsigned int mAcquisitionMode = 0;
		public:
			AdapterMultipleSplitter32() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "Splitter 32 channels"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD32"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD32.png", true); }}
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
			property unsigned int AdapterGain { unsigned int get() override { return 5; }}
			property unsigned int AcquisitionMode { unsigned int get() { return mAcquisitionMode; } }
			void SetAcquisitionMode(unsigned int val) { mAcquisitionMode = val; }
		};
		///<summary>Generic Adapter for AUX</summary>
		public ref class AdapterAux : public GenericAdapter
		{
		protected:
			double mScaleFactor = 1;
			String ^ mUnitOfMeasurement = "V";
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
		///<summary>Generic Adapter for Adapter In</summary>
		public ref class AdapterIn : public GenericAdapter
		{
		public:
			AdapterIn() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "AdapterIn"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterIn"; }}
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
		///<summary>Generic Adapter for Adapter In AD1x16SD5</summary>
		public ref class AdapterInAD1x16SD5 : public AdapterIn
		{
		public:
			AdapterInAD1x16SD5() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "16 channels single adapter differential acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD1x16SD5"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD1x16SD5_B.png", true); }}
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
		///<summary>Generic Adapter for Adapter In AdapterInSplitter64</summary>
		public ref class AdapterInSplitter64 : public AdapterIn
		{
		public:
			AdapterInSplitter64() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "Splitter 16 channels"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AdapterInSplitter64"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
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
		///<summary>Generic Adapter for Adapter In AdapterInSplitter32</summary>
		public ref class AdapterInSplitter32 : public AdapterIn
		{
		public:
			AdapterInSplitter32() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "Splitter 16 channels"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AdapterInSplitter32"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
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
		///<summary>Generic Adapter for Adapter In AD1x16SM5</summary>
		public ref class AdapterInAD1x16SM5 : public AdapterIn
		{
		public:
			AdapterInAD1x16SM5() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "16 channels single adapter monopolar acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD1x16SM5"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD1x16SM5_B.png", true); }}
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
		///<summary>Generic Adapter for Adapter In AD1x16</summary>
		public ref class AdapterInAD1x16 : public AdapterIn
		{
		public:
			AdapterInAD1x16() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "16 channels single adapter acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD1x16"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD1x16.png", true); }}
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
		///<summary>Generic Adapter for Adapter In AD16</summary>
		public ref class AdapterInAD16 : public AdapterIn
		{
		public:
			AdapterInAD16() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "16 channels single adapter acquisition"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD16"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD16.png", true); }}
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
		///<summary>Generic Adapter for Adapter In AD1x16SDD5</summary>
		public ref class AdapterInAD1x16SDD5 : public AdapterIn
		{
		public:
			AdapterInAD1x16SDD5() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "16 channels single adapter DC removal"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD1x16SDD5"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD1x16SDD5.png", true); }}
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
		///<summary>Generic Adapter for Adapter In AD2x8SD5</summary>
		public ref class AdapterInAD2x8 : public AdapterIn
		{
		public:
			AdapterInAD2x8() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "8 channels dual adapter"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD2x8"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD2x8.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 16; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 8; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 8; }}
		};
		///<summary>Generic Adapter for Adapter In AD4x4SD5</summary>
		public ref class AdapterInAD4x4 : public AdapterIn
		{
		public:
			AdapterInAD4x4() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "4 channels quad adapter"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD4x4"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD4x4.png", true); }}
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
		///<summary>Generic Adapter for Adapter In AD8x2JD</summary>
		public ref class AdapterInAD8x2JD : public AdapterIn
		{
		public:
			AdapterInAD8x2JD() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description {String^ get() override { return "16 channels bipolar adapter"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID {String^ get() override { return "AD8x2JD"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("AD8x2JD.png", true); }}
			///<summary>max number of channel available for adapter</summary>
			///<returns>max number of channel available for adapter</returns>
			property unsigned int MaxNumChannels {unsigned int get() override { return 16; }}
			///<summary>min size sensor available for adapter</summary>
			///<returns>min size sensor available for adapter</returns>
			property unsigned int MinSizeSensor {unsigned int get() override { return 1; }}
			///<summary>max size sensor available for adapter</summary>
			///<returns>max size sensor available for adapter</returns>
			property unsigned int  MaxSizeSensor { unsigned int get() override { return 1; }}
			///<summary>gain of adapter</summary>
			///<returns>gain of adapter</returns>
			property unsigned int AdapterGain {unsigned int get() override { return 5; }}
		};
	}
}