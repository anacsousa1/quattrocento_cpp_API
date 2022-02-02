#pragma once
#include "GenericAdapter.h"
namespace OTComm
{
	namespace Quattro
	{
		///<summary>Adapter for Quattro</summary>
		public ref class AdapterQuattro : public GenericAdapter
		{
		public:
			AdapterQuattro() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "AdapterQuattro"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AdapterQuattro"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
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

		///<summary>Adapter ADx5JC</summary>
		public ref class AdapterADx5JC : public AdapterQuattro
		{
		public:
			AdapterADx5JC() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "bipolar adapter"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "ADx5JC"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ADx5JC.png", true); }}
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
		///<summary>Adapter ADx5</summary>
		public ref class AdapterADx5 : public AdapterQuattro
		{
		public:
			AdapterADx5() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "bipolar adapter"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "ADx5"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ADx5.png", true); }}
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
		///<summary>Adapter ADx5JCG</summary>
		public ref class AdapterADx5JCG : public AdapterQuattro
		{
		public:
			AdapterADx5JCG() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "bipolar adapter with ground"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "ADx5JCG"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ADx5JC.png", true); }}
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
		///<summary>Adapter ADx5JB</summary>
		public ref class AdapterADx5JB : public AdapterQuattro
		{
		public:
			AdapterADx5JB() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "bipolar adapter banana connector"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "ADx5JB"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ADx5JB.png", true); }}
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
		///<summary>Adapter ADx5JBG</summary>
		public ref class AdapterADx5JBG : public AdapterQuattro
		{
		public:
			AdapterADx5JBG() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "bipolar adapter banana connector with ground"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "ADx5JBG"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ADx5JB.png", true); }}
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
		///<summary>Adapter ADx5JS</summary>
		public ref class AdapterADx5JS : public AdapterQuattro
		{
		public:
			AdapterADx5JS() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "bipolar adapter snap on connector"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "ADx5JS"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ADx5JS.png", true); }}
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
		///<summary>Adapter ADx5JSG</summary>
		public ref class AdapterADx5JSG : public AdapterQuattro
		{
		public:
			AdapterADx5JSG() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "bipolar adapter snap on connector with ground"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "ADx5JSG"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ADx5JS.png", true); }}
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
		///<summary>Adapter AD3x1SD5</summary>
		public ref class AdapterAD3x1SD5 : public AdapterQuattro
		{
		public:
			AdapterAD3x1SD5() {}
			///<summary>Description of adapter</summary>
			///<returns>description of adapter</returns>
			property String^ Description { String^ get() override { return "3 electrode array sEMG adapter"; }}
			///<summary>ID of adapter</summary>
			///<returns>ID of adapter</returns>
			property String^ ID { String^ get() override { return "AD3x1SD5"; }}
			///<summary>Image of adapter</summary>
			///<returns>Image of adapter</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
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
		public ref class AdapterAux : public AdapterQuattro
		{
		protected:
			unsigned int mAcquisitionMode = 0;
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
			}			void SetUnitOfMeasurement(String^ val) { if (val != "") mUnitOfMeasurement = val; }

		};
	}
}