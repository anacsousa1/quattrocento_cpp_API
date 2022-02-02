#pragma once
#include "GenericChannel.h"
#include "Muscles.h"
#include "DebugLog.h"
#include "ErrorDescription.h"
#include "Utilities.h"
using namespace System;
using namespace System::Collections::Generic;
namespace OTComm
{

	namespace Sensors
	{

		///<summary>Sensor for generic device</summary>
		public ref class SensorGeneralPurpose : public GenericChannel
		{

		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "General purpose sensor"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "General"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorGeneralPurpose() {};
			///<param name="ID">muscle ID</param>
			SensorGeneralPurpose(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorGeneralPurpose(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		///<summary>Sensor for Formeter device</summary>
		public ref class SensorFormeter : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Sensor for Formeter"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "Formeter"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 4; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorFormeter() {};
			///<param name="ID">muscle ID</param>
			SensorFormeter(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorFormeter(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		///<summary>Sensor for Formeter device</summary>
		public ref class SensorOtto : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Sensor for Otto"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "Otto"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 4; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorOtto() {};
			///<param name="ID">muscle ID</param>
			SensorOtto(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorOtto(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};


		///<summary>Sensor CoDe 1</summary>
		public ref class SensorCoDe1_0 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Sensor CoDe 1.0"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "CoDe1"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("CODE1.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorCoDe1_0() {};
			///<param name="ID">muscle ID</param>
			SensorCoDe1_0(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorCoDe1_0(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor CoDe 2</summary>
		public ref class SensorCoDe2_0 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Sensor CoDe 2.0"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "CoDe2"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("CoDe 2.0 C.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorCoDe2_0() {};
			///<param name="ID">muscle ID</param>
			SensorCoDe2_0(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorCoDe2_0(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor De 1</summary>
		public ref class SensorDe1 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Sensor De 1"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "De1"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("DE1.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorDe1() {};
			///<param name="ID">muscle ID</param>
			SensorDe1(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorDe1(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor CDE</summary>
		public ref class SensorCDE : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Sensor CDE"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "CDE"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("CDE.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorCDE() {};
			///<param name="ID">muscle ID</param>
			SensorCDE(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorCDE(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor intramuscolar monopolar</summary>
		public ref class SensoriEMGmonopolar : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Sensor iEMG monopolar"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "iEMGm"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Fi-Wi.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensoriEMGmonopolar() {};
			///<param name="ID">muscle ID</param>
			SensoriEMGmonopolar(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensoriEMGmonopolar(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor intramuscolar monopolar</summary>
		public ref class SensoriEMGbipolar : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Sensor iEMG bipolar"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "iEMGb"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Fi-Wi.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensoriEMGbipolar() {};
			///<param name="ID">muscle ID</param>
			SensoriEMGbipolar(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensoriEMGbipolar(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		/*///<summary>Sensor ELSCH004 for Quattro</summary>
		public ref class SensorELSCH004q : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "4 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH004q"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH004_B.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 4; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorELSCH004q() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH004q(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH004q(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};*/
		///<summary>Sensor ELSCH004</summary>
		public ref class SensorELSCH004 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "4 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH004"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH004.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 4; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorELSCH004() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH004(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH004(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH008</summary>
		public ref class SensorELSCH008 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "8 Adhesive Array 5 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH008"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH008.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 8; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return  gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorELSCH008() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH008(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH008(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH008</summary>
		public ref class SensorIMU : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "9 channel IMU sensor"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "SensorIMU"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 9; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return  gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorIMU() {};
			///<param name="ID">muscle ID</param>
			SensorIMU(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorIMU(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor SA8_10</summary>
		public ref class SensorSA8_10 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "8 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "SA8_10"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("SA8_10_B.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 8; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorSA8_10() {};
			///<param name="ID">muscle ID</param>
			SensorSA8_10(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorSA8_10(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH016</summary>
		public ref class SensorELSCH016 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "16 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH016"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH016.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 16; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorELSCH016() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH016(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH016(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor SA16_2_5</summary>
		public ref class SensorSA16_2_5 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "16 Adhesive Array 2.5 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "SA16_2_5"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("SA 16-2.5.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 16; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorSA16_2_5() {};
			///<param name="ID">muscle ID</param>
			SensorSA16_2_5(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorSA16_2_5(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor SA16_5</summary>
		public ref class SensorSA16_5 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "16 Adhesive Array 5 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "SA16_5"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("SA 16-5.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 16; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorSA16_5() {};
			///<param name="ID">muscle ID</param>
			SensorSA16_5(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorSA16_5(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor SA16_10</summary>
		public ref class SensorSA16_10 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "16 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "SA16_10"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("SA 16-10.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 16; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorSA16_10() {};
			///<param name="ID">muscle ID</param>
			SensorSA16_10(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorSA16_10(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH032NM6</summary>
		public ref class SensorELSCH032NM6 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "32 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH032NM6"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH4x8NM6.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("nm6.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_4x8.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_ELSCH032NM6.png", true); }}
			property unsigned int Channels { unsigned int get() override { return 32; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{8, 7, 6, 5, 4, 3, 2, 1};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{16, 15, 14, 13, 12, 11, 10, 9};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{24, 23, 22, 21, 20, 19, 18, 17};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{32, 31, 30, 29, 28, 27, 26, 25};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{65, 150, 35, 50, 20};
				}
			}
			SensorELSCH032NM6() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH032NM6(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH032NM6(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH064NM5</summary>
		public ref class SensorELSCH064NM7 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 12.5 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH064NM7"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH064NM7.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("nm7.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_13x5.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_ELSCH064NM7.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{60,61,62,63,64};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{59,58,57,56,55};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{50,51,52,53,54};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{49,48,47,46,45};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{40,41,42,43,44};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{39,38,37,36,35};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{32,31,30,33,34};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{29,28,27,26,25};
					array<unsigned short>^ row9 = gcnew array<unsigned short>{20,21,22,23,24};
					array<unsigned short>^ row10 = gcnew array<unsigned short>{19,18,17,16,15};
					array<unsigned short>^ row11 = gcnew array<unsigned short>{10,11,12,13,14};
					array<unsigned short>^ row12 = gcnew array<unsigned short>{9,8,7,6,5};
					array<unsigned short>^ row13 = gcnew array<unsigned short>{0,1,2,3,4};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11, row12, row13};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{150, 50, 20, 29, 14};
				}
			}SensorELSCH064NM7() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH064NM7(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH064NM7(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH031NM6</summary>
		public ref class SensorELSCH031NM6 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "31 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH031NM6"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH4x8NM6.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("nm6.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_4x8.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 31; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{8, 7, 6, 5, 4, 3, 2, 1};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{16, 15, 14, 13, 12, 11, 10, 9};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{24, 23, 22, 21, 20, 19, 18, 17};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{0, 31, 30, 29, 28, 27, 26, 25};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{65, 150, 35, 50, 20};
				}
			}
			SensorELSCH031NM6() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH031NM6(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH031NM6(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH064R3S</summary>
		public ref class SensorELSCH064R3S : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 8 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH064R3S"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH064R3S.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("R3S.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

					/*

										array<unsigned short>^ row1 = gcnew array<unsigned short>{0, 25, 26, 51, 52};
										array<unsigned short>^ row2 = gcnew array<unsigned short>{1, 24, 27, 50, 53};
										array<unsigned short>^ row3 = gcnew array<unsigned short>{2, 23, 28, 49, 54};
										array<unsigned short>^ row4 = gcnew array<unsigned short>{3, 22, 29, 48, 55};
										array<unsigned short>^ row5 = gcnew array<unsigned short>{4, 21, 30, 47, 56};
										array<unsigned short>^ row6 = gcnew array<unsigned short>{5, 20, 31, 46, 57};
										array<unsigned short>^ row7 = gcnew array<unsigned short>{6, 19, 32, 45, 58};
										array<unsigned short>^ row8 = gcnew array<unsigned short>{7, 18, 33, 44, 59};
										array<unsigned short>^ row9 = gcnew array<unsigned short>{8, 17, 34, 43, 60};
										array<unsigned short>^ row10 = gcnew array<unsigned short>{9, 16, 35, 42, 61};
										array<unsigned short>^ row11 = gcnew array<unsigned short>{10, 15, 36, 41, 62};
										array<unsigned short>^ row12 = gcnew array<unsigned short>{11, 14, 37, 40, 63};
										array<unsigned short>^ row13 = gcnew array<unsigned short>{12, 13, 38, 39, 64};*/

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						//row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11, row12, row13};
						row1, row2, row3, row4, row5};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{205, 130, 18, 15, 16};
				}
			}
			SensorELSCH064R3S() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH064R3S(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH064R3S(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		public ref class SensorGR08MM1305 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 8 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "GR08MM1305"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR08MM1305.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR1305.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_13x5.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_GR08MM1305.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{64, 39, 38, 13, 12};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{63, 40, 37, 14, 11};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{62, 41, 36, 15, 10};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{61, 42, 35, 16, 9};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{60, 43, 34, 17, 8};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{59, 44, 33, 18, 7};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{58, 45, 32, 19, 6};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{57, 46, 31, 20, 5};
					array<unsigned short>^ row9 = gcnew array<unsigned short>{56, 47, 30, 21, 4};
					array<unsigned short>^ row10 = gcnew array<unsigned short>{55, 48, 29, 22, 3};
					array<unsigned short>^ row11 = gcnew array<unsigned short>{54, 49, 28, 23, 2};
					array<unsigned short>^ row12 = gcnew array<unsigned short>{53, 50, 27, 24, 1};
					array<unsigned short>^ row13 = gcnew array<unsigned short>{52, 51, 26, 25, 0};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11, row12, row13};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{183, 70, 25, 26, 14};
				}
			}
			SensorGR08MM1305() {};
			///<param name="ID">muscle ID</param>
			SensorGR08MM1305(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorGR08MM1305(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		public ref class SensorGR10MM0808 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "GR10MM0808"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR10MM0808.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR0808.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_8x8.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_GR10MM0808.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{64, 63, 62, 61, 60, 59, 58, 57};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{56, 55, 54, 53, 52, 51, 50, 49};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{48, 47, 46, 45, 44, 43, 42, 41};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{40, 39, 38, 37, 36, 35, 34, 33};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{32, 31, 30, 29, 28, 27, 26, 25};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{24, 23, 22, 21, 20, 19, 18, 17};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{16, 15, 14, 13, 12, 11, 10, 9};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{8, 7, 6, 5, 4, 3, 2, 1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{93, 90, 37, 38, 20};
				}
			}
			SensorGR10MM0808() {};
			///<param name="ID">muscle ID</param>
			SensorGR10MM0808(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorGR10MM0808(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		public ref class SensorHD10MM0808 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "HD10MM0808"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("HD10MM0808.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR0808.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_8x8.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_GR10MM0808.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row8 = gcnew array<unsigned short>{8, 7, 6, 5, 4, 3, 2, 1};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{16, 15, 14, 13, 12, 11, 10, 9};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{24, 23, 22, 21, 20, 19, 18, 17};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{32, 31, 30, 29, 28, 27, 26, 25};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{40, 39, 38, 37, 36, 35, 34, 33};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{48, 47, 46, 45, 44, 43, 42, 41};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{56, 55, 54, 53, 52, 51, 50, 49};
					array<unsigned short>^ row1 = gcnew array<unsigned short>{64, 63, 62, 61, 60, 59, 58, 57};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{93, 90, 37, 38, 20};
				}
			}
			SensorHD10MM0808() {};
			///<param name="ID">muscle ID</param>
			SensorHD10MM0808(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorHD10MM0808(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		public ref class SensorHD10MM0804 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "32 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "HD10MM0804"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("HD10MM0804.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR0804.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_4x8.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_ELSCH032NM6.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 32; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{

					array<unsigned short>^ row1 = gcnew array<unsigned short>{32, 31, 30, 29, 28, 27, 26, 25};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{24, 23, 22, 21, 20, 19, 18, 17};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{16, 15, 14, 13, 12, 11, 10, 9};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{8, 7, 6, 5, 4, 3, 2, 1};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4};
					return bb;
				}
			}

			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{65, 160, 35, 35, 20};
				}
			}

			SensorHD10MM0804() {};
			///<param name="ID">muscle ID</param>
			SensorHD10MM0804(OTComm::MuscleID ID) { mMuscleID = ID; };
			SensorHD10MM0804(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };

		};

		public ref class SensorHD04MM1305 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 4 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "HD04MM1305"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("HD04MM1305.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("HD1305.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_13x5.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_GR04MM1305.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{12, 25, 38, 51, 64};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{11, 24, 37, 50, 63};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{10, 23, 36, 49, 62};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{9, 22, 35, 48, 61};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{8, 21, 34, 47, 60};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{7, 20, 33, 46, 59};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{6, 19, 32, 45, 58};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{5, 18, 31, 44, 57};
					array<unsigned short>^ row9 = gcnew array<unsigned short>{4, 17, 30, 43, 56};
					array<unsigned short>^ row10 = gcnew array<unsigned short>{3, 16, 29, 42, 55};
					array<unsigned short>^ row11 = gcnew array<unsigned short>{2, 15, 28, 41, 54};
					array<unsigned short>^ row12 = gcnew array<unsigned short>{1, 14, 27, 40, 53};
					array<unsigned short>^ row13 = gcnew array<unsigned short>{0, 13, 26, 39, 52};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11, row12, row13};
					return bb;
				}
			}

			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{195, 70, 25, 26, 14};
				}
			}

			SensorHD04MM1305() {};
			///<param name="ID">muscle ID</param>
			SensorHD04MM1305(OTComm::MuscleID ID) { mMuscleID = ID; };
			SensorHD04MM1305(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };

		};

		public ref class SensorHD08MM1305 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 4 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "HD08MM1305"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("HD08MM1305.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("HD1305.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_13x5.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_GR04MM1305.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{64, 39, 38, 13, 12};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{63, 40, 37, 14, 11};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{62, 41, 36, 15, 10};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{61, 42, 35, 16, 9};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{60, 43, 34, 17, 8};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{59, 44, 33, 18, 7};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{58, 45, 32, 19, 6};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{57, 46, 31, 20, 5};
					array<unsigned short>^ row9 = gcnew array<unsigned short>{56, 47, 30, 21, 4};
					array<unsigned short>^ row10 = gcnew array<unsigned short>{55, 48, 29, 22, 3};
					array<unsigned short>^ row11 = gcnew array<unsigned short>{54, 49, 28, 23, 2};
					array<unsigned short>^ row12 = gcnew array<unsigned short>{53, 50, 27, 24, 1};
					array<unsigned short>^ row13 = gcnew array<unsigned short>{52, 51, 26, 25, 0};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11, row12, row13};
					return bb;
				}
			}

			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{195, 70, 25, 26, 14};
				}
			}

			SensorHD08MM1305() {};
			///<param name="ID">muscle ID</param>
			SensorHD08MM1305(OTComm::MuscleID ID) { mMuscleID = ID; };
			SensorHD08MM1305(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };

		};

		public ref class SensorGR04MM1305 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 4 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "GR04MM1305"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR04MM1305.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR1305.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_13x5.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_GR04MM1305.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{64, 39, 38, 13, 12};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{63, 40, 37, 14, 11};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{62, 41, 36, 15, 10};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{61, 42, 35, 16, 9};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{60, 43, 34, 17, 8};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{59, 44, 33, 18, 7};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{58, 45, 32, 19, 6};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{57, 46, 31, 20, 5};
					array<unsigned short>^ row9 = gcnew array<unsigned short>{56, 47, 30, 21, 4};
					array<unsigned short>^ row10 = gcnew array<unsigned short>{55, 48, 29, 22, 3};
					array<unsigned short>^ row11 = gcnew array<unsigned short>{54, 49, 28, 23, 2};
					array<unsigned short>^ row12 = gcnew array<unsigned short>{53, 50, 27, 24, 1};
					array<unsigned short>^ row13 = gcnew array<unsigned short>{52, 51, 26, 25, 0};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11, row12, row13};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{183, 70, 25, 26, 14};
				}
			}
			SensorGR04MM1305() {};
			///<param name="ID">muscle ID</param>
			SensorGR04MM1305(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorGR04MM1305(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		public ref class SensorGR10MM0804 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "32 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "GR10MM0804"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR10MM0804.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("GR0804.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_4x8.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_ELSCH032NM6.png", true); }}
			property unsigned int Channels { unsigned int get() override { return 32; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{32, 31, 30, 29, 28, 27, 26, 25};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{24, 23, 22, 21, 20, 19, 18, 17}; 
					array<unsigned short>^ row3 = gcnew array<unsigned short>{16, 15, 14, 13, 12, 11, 10, 9};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{8, 7, 6, 5, 4, 3, 2, 1}; 

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>

			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{65, 160, 35, 35, 20};
				}
			}
			SensorGR10MM0804() {};
			///<param name="ID">muscle ID</param>
			SensorGR10MM0804(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorGR10MM0804(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};


		///<summary>Sensor ELSCH064NM5</summary>
		public ref class SensorELSCH064NM5 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 2.5 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH064NM5"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH064NM5.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("nm5.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_13x5.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_ELSCH064NM5.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{64, 63, 62, 61, 60};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{55, 56, 57, 58, 59};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{54, 53, 52, 51, 50};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{45, 46, 47, 48, 49};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{44, 43, 42, 41, 40};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{35, 36, 37, 38, 39};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{34, 33, 32, 31, 30};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{25, 26, 27, 28, 29};
					array<unsigned short>^ row9 = gcnew array<unsigned short>{24, 23, 22, 21, 20};
					array<unsigned short>^ row10 = gcnew array<unsigned short>{15, 16, 17, 18, 19};
					array<unsigned short>^ row11 = gcnew array<unsigned short>{14, 13, 12, 11, 10};
					array<unsigned short>^ row12 = gcnew array<unsigned short>{5, 6, 7, 8, 9};
					array<unsigned short>^ row13 = gcnew array<unsigned short>{4, 3, 2, 1, 0};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11, row12, row13};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{140, 62, 20, 26, 14};
				}
			}
			SensorELSCH064NM5() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH064NM5(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH064NM5(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH064NM4</summary>
		public ref class SensorELSCH064NM4 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 4 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH064NM4"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH064NM4.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("nm4.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_13x5.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_ELSCH064NM4.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{60, 61, 62, 63, 64};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{59, 58, 57, 56, 55};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{50, 51, 52, 53, 54};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{49, 48, 47, 46, 45};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{40, 41, 42, 43, 44};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{39, 38, 37, 36, 35};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{32, 31, 30, 33, 34};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{29, 28, 27, 26, 25};
					array<unsigned short>^ row9 = gcnew array<unsigned short>{20, 21, 22, 23, 24};
					array<unsigned short>^ row10 = gcnew array<unsigned short>{19, 18, 17, 16, 15};
					array<unsigned short>^ row11 = gcnew array<unsigned short>{10, 11, 12, 13, 14};
					array<unsigned short>^ row12 = gcnew array<unsigned short>{9, 8, 7, 6, 5};
					array<unsigned short>^ row13 = gcnew array<unsigned short>{0, 1, 2, 3, 4};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11, row12, row13};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{190, 45, 20, 29, 14};
				}
			}
			SensorELSCH064NM4() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH064NM4(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH064NM4(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH064NM3</summary>
		public ref class SensorELSCH064NM3 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 10 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH064NM3"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH064NM3.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("nm3.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_8x8.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_ELSCH064NM3.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{64, 63, 62, 61, 60, 59, 58, 57};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{56, 55, 54, 53, 52, 51, 50, 49};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{48, 47, 46, 45, 44, 43, 42, 41};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{40, 39, 38, 37, 36, 35, 34, 33};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{32, 31, 30, 29, 28, 27, 26, 25};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{24, 23, 22, 21, 20, 19, 18, 17};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{16, 15, 14, 13, 12, 11, 10, 9};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{8, 7, 6, 5, 4, 3, 2, 1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{93, 60, 29, 44, 20};
				}
			}
			SensorELSCH064NM3() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH064NM3(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH064NM3(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH064NM2</summary>
		public ref class SensorELSCH064NM2 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "64 Adhesive Array 8 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH064NM2"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH064NM2.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("nm2.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_13x5.png", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_ELSCH064NM2.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{61, 60, 59, 58, 57};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{62, 63, 64, 56, 55};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{50, 51, 52, 53, 54};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{49, 41, 42, 43, 44};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{40, 48, 47, 46, 45};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{39, 38, 37, 36, 35};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{34, 33, 27, 26, 25};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{32, 31, 30, 29, 28};
					array<unsigned short>^ row9 = gcnew array<unsigned short>{24, 23, 22, 21, 20};
					array<unsigned short>^ row10 = gcnew array<unsigned short>{10, 9, 17, 18, 19};
					array<unsigned short>^ row11 = gcnew array<unsigned short>{11, 12, 13, 14, 15};
					array<unsigned short>^ row12 = gcnew array<unsigned short>{5, 6, 7, 8, 16};
					array<unsigned short>^ row13 = gcnew array<unsigned short>{4, 3, 2, 1, 0};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8, row9, row10, row11, row12, row13};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{146, 50, 20, 29, 14};
				}
			}
			SensorELSCH064NM2() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH064NM2(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH064NM2(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		///<summary>Sensor AUXAcc</summary>
		public ref class Quaternion : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Quaternion"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "Quaternion"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 4; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			Quaternion() {};
			///<param name="ID">muscle ID</param>
			Quaternion(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			Quaternion(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		///<summary>Sensor AUXAcc</summary>
		public ref class AUXAcc : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Accelerometer Transducer"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "AUX  Acceleration"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("accelerometer.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			AUXAcc() {};
			///<param name="ID">muscle ID</param>
			AUXAcc(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			AUXAcc(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor AUXTor</summary>
		public ref class AUXTor : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Torque Transducer"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "AUX  Torque"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Torque.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			AUXTor() {};
			///<param name="ID">muscle ID</param>
			AUXTor(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			AUXTor(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor AUXFor</summary>
		public ref class AUXFor : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Force Transducer"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "AUX  Force"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("force.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			AUXFor() {};
			///<param name="ID">muscle ID</param>
			AUXFor(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			AUXFor(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor AUXFor</summary>
		public ref class AUXTrig : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Trigger Transducer"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "AUX  Trigger"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("trigger.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			AUXTrig() {};
			///<param name="ID">muscle ID</param>
			AUXTrig(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			AUXTrig(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor AUXAcc</summary>
		public ref class RampChannel : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Ramp Channel"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "RampChannel"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			RampChannel() {};
			///<param name="ID">muscle ID</param>
			RampChannel(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			RampChannel(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor AUXAcc</summary>
		public ref class BufferChannel : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "Buffer Channel"; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "BufferChannel"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 1; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ aa = gcnew array<unsigned short>{1};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{aa};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			BufferChannel() {};
			///<param name="ID">muscle ID</param>
			BufferChannel(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			BufferChannel(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};
		///<summary>Sensor ELSCH064NM2</summary>
		public ref class SensorELSCH020EEG : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description {
				String^ get() override { return mDescription; }
				void set(String^ descr) override {
					mDescription = descr;
				}
			};
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH020EEG"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("EEG.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH020EEG.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 20; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{1,2};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{3};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{4,5,6,7,8};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{9,10,11,12,13};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{14,15,16,17,18};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{19,20};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorELSCH020EEG() { mDescription = "20 Monopolar EEG channels"; };
			///<param name="ID">muscle ID</param>
			SensorELSCH020EEG(OTComm::MuscleID ID) { 
				mMuscleID = ID; 
				mDescription = "20 Monopolar EEG channels";
			};
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH020EEG(OTComm::MuscleID ID, OTComm::SideID side) { 
				mMuscleID = ID; 
				mSide = side; 
				mDescription = "20 Monopolar EEG channels";
			};
		};
		///<summary>Sensor ELSCH064NM2</summary>
		public ref class SensorELSCH064EEG : public GenericChannel
		{			
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description {
				String^ get() override { return mDescription; }
				void set(String^ descr) override {
					mDescription = descr;
				}
			};
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH064EEG"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("EEG.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("ELSCH064EEG.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 64; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{1, 2, 3};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{4, 5, 6, 7, 8};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{9, 10, 11, 12, 13,14,15,16,17};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{18,19,20,21,22,23,24,25,26,27,28};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{29, 30,31,32,33,34,35,36,37,38,39};
					array<unsigned short>^ row6 = gcnew array<unsigned short>{40,41,42,43,44,45,46,47,48,49,50};
					array<unsigned short>^ row7 = gcnew array<unsigned short>{51,52,53,54,55,56,57,58,59};
					array<unsigned short>^ row8 = gcnew array<unsigned short>{60,61,62};
					array<unsigned short>^ row9 = gcnew array<unsigned short>{63,64};


					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5, row6, row7, row8, row9};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorELSCH064EEG() {
				mDescription = "64 Monopolar EEG channels";
			};
			///<param name="ID">muscle ID</param>
			SensorELSCH064EEG(OTComm::MuscleID ID) { 
				mMuscleID = ID; 
				mDescription = "64 Monopolar EEG channels";
			};
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH064EEG(OTComm::MuscleID ID, OTComm::SideID side) { 
				mMuscleID = ID; 
				mSide = side; 
				mDescription = "64 Monopolar EEG channels";
			};
		};
		///<summary>Sensor ELSCH064NM2</summary>
		public ref class SensorELSCH024TEST : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "24 Monopolar EEG channels "; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "ELSCH024TEST"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("SensorELSCH024TEST.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>Image of Sensor Matrix Channel</summary>
			///<returns>Image of Sensor Matrix Channel</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("Default.jpg", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property unsigned int Channels { unsigned int get() override { return 24; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{4, 5, 11, 10, 24, 32, 34, 39, 40, 49, 50, 62, 61};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{3, 6, 12, 9, 23, 31, 33, 38, 48, 41, 51, 63, 60};
					array<unsigned short>^ row3 = gcnew array<unsigned short>{2, 7, 13, 17, 22, 30, 27, 37, 47, 42, 52, 64, 59};
					array<unsigned short>^ row4 = gcnew array<unsigned short>{1, 8, 14, 18, 21, 29, 26, 36, 46, 43, 53, 56, 58};
					array<unsigned short>^ row5 = gcnew array<unsigned short>{0, 16, 15, 19, 20, 28, 25, 35, 45, 44, 54, 55, 57};
					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2, row3, row4, row5};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{66, 62, 27, 27, 14};
				}
			}
			SensorELSCH024TEST() {};
			///<param name="ID">muscle ID</param>
			SensorELSCH024TEST(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorELSCH024TEST(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};

		
		public ref class SensorHD20MM1602 : public GenericChannel
		{
		public:
			///<summary>Description of Sensor</summary>
			///<returns>description of Sensor</returns>
			property String^ Description { String^ get() override { return "32 Adhesive Array 15 mm i.e.d."; }}
			///<summary>ID of Sensor</summary>
			///<returns>ID of Sensor</returns>
			property String^ ChannelID { String^ get() override { return "HD20MM1602"; }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ Image { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("HD20MM1602.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ ImageRotation { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("HD20MM1602.png", true); }}
			///<summary>Image of Sensor</summary>
			///<returns>Image of Sensor</returns>
			property Bitmap^ MapImage { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrix_2x16.png", true); }}
			///<summary>number of channel available for Sensor</summary>
			///<returns>number of channel available for Sensor</returns>
			property Bitmap^ ImageMatrixChannel { Bitmap^ get() override { return Utilities::GetImageFromEmbeddedResource("matrixChannel_ELSCH032NM6.png", true); }}
			property unsigned int Channels { unsigned int get() override { return 32; }}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<array<unsigned short>^>^ Remap
			{
				array<array<unsigned short>^>^ get() override
				{
					array<unsigned short>^ row1 = gcnew array<unsigned short>{32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17};
					array<unsigned short>^ row2 = gcnew array<unsigned short>{16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

					array<array<unsigned short>^>^ bb = gcnew array<array<unsigned short>^>{
						row1, row2};
					return bb;
				}
			}
			///<summary>remap of channel available for Sensor</summary>
			///<returns>remap of channel available for Sensor</returns>
			property array<unsigned short>^ MatrixChannelRemap
			{
				array<unsigned short>^ get() override
				{
					return gcnew array<unsigned short>{65, 150, 35, 50, 20};
				}
			}
			SensorHD20MM1602() {};
			///<param name="ID">muscle ID</param>
			SensorHD20MM1602(OTComm::MuscleID ID) { mMuscleID = ID; };
			///<param name="ID">muscle ID</param>
			///<param name="side">muscle side</param>
			SensorHD20MM1602(OTComm::MuscleID ID, OTComm::SideID side) { mMuscleID = ID; mSide = side; };
		};		

	}
}