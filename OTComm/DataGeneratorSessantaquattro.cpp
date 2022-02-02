#include "DataGeneratorSessantaquattro.h"
#include "stdio.h"
#include <string>
using namespace System::Xml;
namespace OTComm
{
	namespace Sessantaquattro
	{
#pragma comment(lib, "Ws2_32.lib")

#pragma managed


#pragma region Constructors and destructors
		DataGeneratorSessantaquattro::DataGeneratorSessantaquattro(Model model, Frequency SamplingFrequency, bool HighRes) :
			DataGeneratorInterface(), mModel(model),
			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false),
			mDataReaderInterfaceHelper(NULL), mPortNumber(mDefaultPortNumber)
		{
			try
			{
				mIPAdress = String::Copy(mDefaultIPAdress);
				mConnectors = gcnew array<GenericConnector^>(GetNumberConnectorsIn() + GetNumberConnectorsAux() + GetNumberControlAux());
				unsigned int k = 0;
				for (; k < GetNumberConnectorsIn(); k++)
					mConnectors[k] = gcnew GenericConnector(mModel % 10000);
				for (; k < (unsigned int)mConnectors->Length; k++)
					mConnectors[k] = gcnew GenericConnector(1);

				mSources = gcnew array<DataGeneratorSingleSource^>(1);
				mSources[0] = gcnew DataGeneratorSingleSource(SamplingFrequency, HighRes ? 3 : 2, GetChannelsMapping());

				//mSamplingFrequency = SamplingFrequency;
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
				mDeviceTotalChannels = GetNumberTotalChannels();
				mDefaulDataValidatorTrigger = nullptr;
			}
			catch (Exception ^e)
			{
				ConstructorFailedException^ cfe = gcnew ConstructorFailedException(__FUNCTION__, e);
				throw cfe;
			}
		}

		DataGeneratorSessantaquattro::~DataGeneratorSessantaquattro()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorSessantaquattro();
		}

		DataGeneratorSessantaquattro::!DataGeneratorSessantaquattro()
		{

		}

#pragma endregion

#pragma region XML files

		String^ DataGeneratorSessantaquattro::GetDeviceXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			String^ Firmware;
			try
			{
				if (GetFirmwareVersion(Firmware) <= 0)
					Firmware = "Unknown";
			}
			catch (Exception^)
			{
				Firmware = "Unknown";
			}


			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", GetNumberTotalChannels().ToString());// (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("IPAddress", mIPAdress);
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			{
				mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			}
			mXMLTextStream->WriteAttributeString("Resolution", mSources[0]->mBytesPerChannel.ToString());
			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteStartElement("Channels");

			unsigned int ChannelNum = 0;
			int startIndex = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{
				if (mConnectors[k]->Adapter != nullptr)
				{	
					if (mConnectors[k]->Adapter->ID->Contains("Direct connection to Auxiliary Input"))
					{	

						mConnectors[k]->Adapter->AuxWriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							(float)0.5,
							(float)100000,
							(float)0,
							mConnectors[k]->DetectionMode,
							(float)mConnectors[k]->AuxScaleFactor,
							mConnectors[k]->unitOfMeasurement
						); //Aux Scale Factor
					}
					else if (mConnectors[k]->Adapter->ID->Contains("ISO-AUX"))
					{

						mConnectors[k]->Adapter->AuxWriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							(float)0.5,
							(float)100000,
							(float)0,
							mConnectors[k]->DetectionMode,
							(float)mConnectors[k]->AuxScaleFactor,
							mConnectors[k]->unitOfMeasurement
						); //Aux Scale Factor
					}/*
					else if (mConnectors[k]->Adapter->ID->Contains("AD4x8SE"))
					{

						mConnectors[k]->Adapter->WriteXMLConfiguration(mXMLTextStream, k, startIndex, 0,
							(float)0.2, mConnectors[k]->DetectionMode, mConnectors[k]->LowPassFilter, mConnectors[k]->HigPassFilter);
					}*/
					else
					mConnectors[k]->Adapter->WriteXMLConfiguration(mXMLTextStream, k, startIndex, 0,
						mConnectors[k]->DetectionMode, mConnectors[k]->LowPassFilter, mConnectors[k]->HigPassFilter);
				}
			}

			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		String^ DataGeneratorSessantaquattro::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			String^ Firmware;
			if (GetFirmwareVersion(Firmware) <= 0)
				Firmware = "Unknown";

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			{
				mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			}
			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteAttributeString("ad_bits", mSources[0]->mBytesPerChannel == 2 ? "16" : "24");
			mXMLTextStream->WriteStartElement("Channels");

			unsigned int ChannelNum = 0;
			int startIndex = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{
				if (mConnectors[k]->Adapter != nullptr)
				{
					if (mConnectors[k]->Adapter->ID->Contains("Direct connection to Auxiliary Input"))
					{
						mConnectors[k]->Adapter->AuxWriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							(float)0.5,
							(float)100000,
							(float)0,
							mConnectors[k]->DetectionMode,
							(float)mConnectors[k]->AuxScaleFactor,
							mConnectors[k]->unitOfMeasurement);
					}
					else if (mConnectors[k]->Adapter->ID->Contains("AdapterControl"))
					{
						mConnectors[k]->Adapter->WriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							(float)1,
							(float)100000,
							(float)0,
							mConnectors[k]->DetectionMode);
					}/*
					else if (mConnectors[k]->Adapter->ID->Contains("AD4x8SE"))
					{
						mConnectors[k]->Adapter->WriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							0.2,
							mSources[0]->mBytesPerChannel == 2 ? (float)500 : (float)125,
							mSources[0]->mBytesPerChannel == 2 ? (float)10 : (float)0,
							mConnectors[k]->DetectionMode);
					}*/
					else
					{
						mConnectors[k]->Adapter->WriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							mSources[0]->mBytesPerChannel == 2 ? (float)256 : (float)1,
							mSources[0]->mBytesPerChannel == 2 ? (float)500 : (float)125,
							mSources[0]->mBytesPerChannel == 2 ? (float)10 : (float)0,
							mConnectors[k]->DetectionMode);
					}

					for (int zz = 0; zz < mConnectors[k]->Adapter->Channels->Length; zz++)
						if (mConnectors[k]->Adapter->Channels[zz] != nullptr)
							startIndex++;
				}

				//ChannelNum += mConnectors[k]->NumConnectableChannels;
			}

			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		DataGeneratorSessantaquattro^ DataGeneratorSessantaquattro::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorSessantaquattro^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try{
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				unsigned int deviceModel = System::Convert::ToUInt16(DeviceNode->Attributes["Model"]->Value);
				String^ loadedipaddress = nullptr;
				if (DeviceNode->Attributes["IPAddress"] != nullptr)
					loadedipaddress = DeviceNode->Attributes["IPAddress"]->Value;
				unsigned int deviceFrequency = System::Convert::ToUInt16(DeviceNode->Attributes["SampleFrequency"]->Value);
				int resolution = System::Convert::ToInt16(DeviceNode->Attributes["Resolution"]->Value);
				if (deviceName->Contains("SESSANTAQUATTRO"))
				{

					gdr = gcnew OTComm::Sessantaquattro::DataGeneratorSessantaquattro(deviceModel, deviceFrequency,
						resolution == 3);
					if (loadedipaddress != nullptr)
						gdr->SetIPAddress(loadedipaddress);

					System::Xml::XmlNode^ ChannelsNode = DeviceNode->ChildNodes[0];
					for (int ii = 0; ii < ChannelsNode->ChildNodes->Count; ii++)
					{
						System::Xml::XmlNode^ AdapterNode = ChannelsNode->ChildNodes[ii];
						GenericAdapter^ ad = GetAdapterFromID(AdapterNode->Attributes["ID"]->Value->ToString());
						unsigned int adPosition = System::Convert::ToUInt16(AdapterNode->Attributes["AdapterIndex"]->Value);
						if (ad != nullptr)
						{

							for (int jj = 0; jj < AdapterNode->ChildNodes->Count; jj++)
							{
								System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[jj];
								GenericChannel^ ch = GetChannelFromID(
									ChannelNode->Attributes["ID"]->Value->ToString(),
									ChannelNode->Attributes["Muscle"]->Value->ToString(),
									ChannelNode->Attributes["Side"]->Value->ToString());
								if (ch != nullptr)
								{
									ad->SetChannel(System::Convert::ToUInt16(ChannelNode->Attributes["Index"]->Value->ToString()), ch);
									try {
										ad->Channels[System::Convert::ToUInt16(ChannelNode->Attributes["Index"]->Value->ToString())]->ChannelPrefix = ChannelNode->Attributes["Prefix"]->Value->ToString();
									}
									catch (Exception^ ex) {
									}

									//If I'm using an EEG sensor I set the channel description with the respective
									//label (EEGCap or AD1x64+cap)
									if (ad->ID->Contains("ADEEGCap20SE")) {
										ad->Channels[jj]->Description = GetEEGLabelFor20EEGFromIndex(jj);
									}
									else if (ad->ID->Contains("ADEEGCap64SE")) {
										ad->Channels[jj]->Description = GetEEGLabelFor64EEGFromIndex(jj);
									}
									else if (AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("20") &&
										AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("EEG")) {
										ad->Channels[jj]->Description = GetEEGLabelFor20EEGFromIndex(jj);
									}
									else if (AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("64") &&
										AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("EEG")) {
										ad->Channels[jj]->Description = GetEEGLabelFor64EEGFromIndex(jj);
									}
								}
							}
						}
						else
							continue;
						if (adPosition < 1)
							gdr->ConnectIn(adPosition, (AdapterSessantaquattro^)ad);
						else if (adPosition < 3) // AUX
							if ((ad->Description == "ISO-AUX")) {
								gdr->ConnectIsoAux(adPosition - 1, (AdapterIsoAux^)ad);
							}
							else {
								gdr->ConnectAux(adPosition - 1, (AdapterAux^)ad);
							}
						else // control
							gdr->ConnectControl(adPosition - 3, (AdapterControl^)ad);

						if (adPosition == 0)
						{
							gdr->GetConnectors()[adPosition]->DetectionMode = System::Convert::ToUInt16(AdapterNode->Attributes["DetectionMode"]->Value);
							gdr->GetConnectors()[adPosition]->LowPassFilter = System::Convert::ToUInt16(AdapterNode->Attributes["LowPassFilter"]->Value);
							gdr->GetConnectors()[adPosition]->HigPassFilter = System::Convert::ToUInt16(AdapterNode->Attributes["HighPassFilter"]->Value);

						}//Aux Scale Factor
						else if (adPosition < 3) {
							try {

								gdr->GetConnectors()[adPosition]->AuxScaleFactor = System::Convert::ToDouble(AdapterNode->Attributes["AuxScaleFactor"]->Value);
								gdr->GetConnectors()[adPosition]->unitOfMeasurement = System::Convert::ToString(AdapterNode->Attributes["UnitOfMeasurement"]->Value);

							}
							catch (Exception^ ex) {
								Console::WriteLine(ex->ToString());
								gdr->GetConnectors()[adPosition]->AuxScaleFactor = 1;
								gdr->GetConnectors()[adPosition]->unitOfMeasurement = "";
							}
						}

					}
				}
				textReader->Close();
			}
			catch (Exception^ ex) {
				textReader->Close();
				throw "Error in FromXMLConfiguration";
			}
			return gdr;
		}
		//Dato l'indice della traccia, ritorniamo la label assegnatali
		String^ GetEEGLabelFor20EEGFromIndex(int index)
		{
			String^ label = "";
			index++;
			switch (index)
			{
			default:
				break;
			case 1:
				label = "FP1";
				break;
			case 2:
				label = "FP2";
				break;
			case 3:
				label = "AFz";
				break;
			case 4:
				label = "F7";
				break;
			case 5:
				label = "F3";
				break;
			case 6:
				label = "Fz";
				break;
			case 7:
				label = "F4";
				break;
			case 8:
				label = "F8";
				break;
			case 9:
				label = "T7";
				break;
			case 10:
				label = "C3";
				break;
			case 11:
				label = "Cz";
				break;
			case 12:
				label = "C4";
				break;
			case 13:
				label = "T8";
				break;
			case 14:
				label = "P7";
				break;
			case 15:
				label = "P3";
				break;
			case 16:
				label = "Pz";
				break;
			case 17:
				label = "P4";
				break;
			case 18:
				label = "P8";
				break;
			case 19:
				label = "O1";
				break;
			case 20:
				label = "O2";
				break;
			}
			return label;
		}
		String^ GetEEGLabelFor64EEGFromIndex(int index)
		{
			String^ label = "";
			index++;
			switch (index)
			{
			default:
				break;
			case 1:
				label = "FP1";
				break;
			case 2:
				label = "FPZ";
				break;
			case 3:
				label = "FP2";
				break;
			case 4:
				label = "AF7";
				break;
			case 5:
				label = "AF3";
				break;
			case 6:
				label = "AFz";
				break;
			case 7:
				label = "AF4";
				break;
			case 8:
				label = "AF8";
				break;
			case 9:
				label = "F9";
				break;
			case 10:
				label = "F7";
				break;
			case 11:
				label = "F3";
				break;
			case 12:
				label = "F1";
				break;
			case 13:
				label = "Fz";
				break;
			case 14:
				label = "F2";
				break;
			case 15:
				label = "F4";
				break;
			case 16:
				label = "F8";
				break;
			case 17:
				label = "F10";
				break;
			case 18:
				label = "FT9";
				break;
			case 19:
				label = "FT7";
				break;
			case 20:
				label = "FC5";
				break;
			case 21:
				label = "FC3";
				break;
			case 22:
				label = "FC1";
				break;
			case 23:
				label = "FCz";
				break;
			case 24:
				label = "FC2";
				break;
			case 25:
				label = "FC4";
				break;
			case 26:
				label = "FC6";
				break;
			case 27:
				label = "FT8";
				break;
			case 28:
				label = "FT10";
				break;
			case 29:
				label = "T9";
				break;
			case 30:
				label = "T7";
				break;
			case 31:
				label = "C5";
				break;
			case 32:
				label = "C3";
				break;
			case 33:
				label = "C1";
				break;
			case 34:
				label = "Cz";
				break;
			case 35:
				label = "C2";
				break;
			case 36:
				label = "C4";
				break;
			case 37:
				label = "C6";
				break;
			case 38:
				label = "T8";
				break;
			case 39:
				label = "T10";
				break;
			case 40:
				label = "TP9";
				break;
			case 41:
				label = "TP7";
				break;
			case 42:
				label = "CP5";
				break;
			case 43:
				label = "CP3";
				break;
			case 44:
				label = "CP1";
				break;
			case 45:
				label = "CPz";
				break;
			case 46:
				label = "CP2";
				break;
			case 47:
				label = "CP4";
				break;
			case 48:
				label = "CP6";
				break;
			case 49:
				label = "TP8";
				break;
			case 50:
				label = "TP10";
				break;
			case 51:
				label = "P9";
				break;
			case 52:
				label = "P7";
				break;
			case 53:
				label = "P3";
				break;
			case 54:
				label = "P1";
				break;
			case 55:
				label = "Pz";
				break;
			case 56:
				label = "P2";
				break;
			case 57:
				label = "P4";
				break;
			case 58:
				label = "P8";
				break;
			case 59:
				label = "P10";
				break;
			case 60:
				label = "PO7";
				break;
			case 61:
				label = "POz";
				break;
			case 62:
				label = "PO8";
				break;
			case 63:
				label = "O1";
				break;
			case 64:
				label = "O2";
				break;
			}
			return label;
		}


#pragma endregion

#pragma region Connectors and adapters

		bool StringContains(std::string s1, std::string s2) {
			if (s1.find(s2) != std::string::npos) {
				return true;
			}
			return false;
		}

		static std::string toStandardString(System::String^ string)
		{
			using System::Runtime::InteropServices::Marshal;

			if (string->Length == 0 || string->Length < 0)
			{


			}

			System::IntPtr pointer = Marshal::StringToHGlobalAnsi(string);
			char* charPointer = reinterpret_cast<char*>(pointer.ToPointer());
			std::string returnString(charPointer, string->Length);
			Marshal::FreeHGlobal(pointer);


			return returnString;
		}

		void DataGeneratorSessantaquattro::ConnectAux(const unsigned int port, AdapterAux^ adapter)
		{
			if (port >= GetNumberConnectorsAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			mConnectors[GetNumberConnectorsIn() + port]->Adapter = adapter;
		}

		void DataGeneratorSessantaquattro::ConnectIsoAux(const unsigned int port, AdapterIsoAux^ adapter)
		{
			if (port >= GetNumberConnectorsAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}
			//connect the first aux
			mConnectors[GetNumberConnectorsIn() + port]->Adapter = adapter;
			//insert a second aux 
			AdapterIsoAux^ adapterNew = gcnew AdapterIsoAux;
			adapterNew->SetScaleFactor(adapter->ScaleFactor);
			adapterNew->SetUnitOfMeasurement(adapter->UnitOfMeasurement);

			GenericChannel^ sensorToConnect = GetChannelFromString(adapter->Channels[0]->ChannelID);
			adapterNew->SetChannels(0, sensorToConnect->Channels-1, sensorToConnect);
			mConnectors[GetNumberConnectorsIn() + port + 1]->Adapter = adapterNew;
		}

		void DataGeneratorSessantaquattro::ConnectControl(const unsigned int port, AdapterControl^ adapter)
		{
			if (port >= GetNumberControlAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			mConnectors[GetNumberConnectorsIn() + GetNumberConnectorsAux() + port]->Adapter = adapter;
		}

		void DataGeneratorSessantaquattro::ConnectIn(const unsigned int port, AdapterSessantaquattro^ adapter)
		{
			if (port >= GetNumberConnectorsIn())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			mConnectors[port]->Adapter = adapter;
		}

		GenericAdapter^ DataGeneratorSessantaquattro::GetAdapterFromID(String^ adapterID)
		{
			std::string converted = toStandardString(adapterID);


			if (adapterID == ("AD1x16SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInAD1x16SE();
			if (adapterID == ("AD4x16SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInAD4x16SE();
			if (adapterID == ("AD1x64SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInAD1x64SE();
			if (adapterID == ("AD2x32SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInAD2x32SE();
			if (adapterID == ("AD2x16SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInAD2x16SE();
			if (adapterID == ("AD4x8SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInAD4x8SE();
			if (adapterID == ("AD8x4SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInAD8x4SE();
			if (adapterID == ("AD4x4SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInAD4x4SE();
			if (adapterID == ("AD8x1SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInAD8x1SE();
			if (StringContains("Direct connection to Auxiliary Input", converted))
				return gcnew OTComm::Sessantaquattro::AdapterAux();
			if (adapterID == ("ISO-AUX"))
				return gcnew OTComm::Sessantaquattro::AdapterIsoAux();
			if (adapterID == ("AdapterControl"))
				return gcnew OTComm::Sessantaquattro::AdapterControl();
			if (adapterID == ("ADEEGCap20SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInADEEGCap20SE();
			if (adapterID == ("ADEEGCap64SE"))
				return gcnew OTComm::Sessantaquattro::AdapterInADEEGCap64SE();
			return nullptr;
		}

		GenericChannel^ DataGeneratorSessantaquattro::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
		{
			if (channelID == ("CoDe1"))
				return gcnew OTComm::Sensors::SensorCoDe1_0(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("CoDe2"))
				return gcnew OTComm::Sensors::SensorCoDe2_0(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("De1"))
				return gcnew OTComm::Sensors::SensorDe1(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("CDE"))
				return gcnew OTComm::Sensors::SensorCDE(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("General"))
				return gcnew OTComm::Sensors::SensorGeneralPurpose(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			//if (channelID == ("ELSCH004q"))
			//	return gcnew OTComm::Sensors::SensorELSCH004q(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH004"))
				return gcnew OTComm::Sensors::SensorELSCH004(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH008"))
				return gcnew OTComm::Sensors::SensorELSCH008(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("SA8_10"))
				return gcnew OTComm::Sensors::SensorSA8_10(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH016"))
				return gcnew OTComm::Sensors::SensorELSCH016(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("SA16_2_5"))
				return gcnew OTComm::Sensors::SensorSA16_2_5(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("SA16_5"))
				return gcnew OTComm::Sensors::SensorSA16_5(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("SA16_10"))
				return gcnew OTComm::Sensors::SensorSA16_10(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH032NM6"))
				return gcnew OTComm::Sensors::SensorELSCH032NM6(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH031NM6"))
				return gcnew OTComm::Sensors::SensorELSCH031NM6(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064R3S"))
				return gcnew OTComm::Sensors::SensorELSCH064R3S(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM5"))
				return gcnew OTComm::Sensors::SensorELSCH064NM5(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM7"))
				return gcnew OTComm::Sensors::SensorELSCH064NM7(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM4"))
				return gcnew OTComm::Sensors::SensorELSCH064NM4(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM3"))
				return gcnew OTComm::Sensors::SensorELSCH064NM3(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM2"))
				return gcnew OTComm::Sensors::SensorELSCH064NM2(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("GR10MM0808"))
				return gcnew OTComm::Sensors::SensorGR10MM0808(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("GR08MM1305"))
				return gcnew OTComm::Sensors::SensorGR08MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("GR04MM1305"))
				return gcnew OTComm::Sensors::SensorGR04MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("GR10MM0804"))
				return gcnew OTComm::Sensors::SensorGR10MM0804(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH020EEG"))
				return gcnew OTComm::Sensors::SensorELSCH020EEG(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064EEG"))
				return gcnew OTComm::Sensors::SensorELSCH064EEG(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Acceleration"))
				return gcnew OTComm::Sensors::AUXAcc(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Torque"))
				return gcnew OTComm::Sensors::AUXTor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Force"))
				return gcnew OTComm::Sensors::AUXFor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == "AUX  Trigger")
				return gcnew OTComm::Sensors::AUXTrig(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("RampChannel"))
				return gcnew OTComm::Sensors::RampChannel(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("BufferChannel"))
				return gcnew OTComm::Sensors::BufferChannel(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("iEMGb"))
				return gcnew OTComm::Sensors::SensoriEMGbipolar(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("iEMGm"))
				return gcnew OTComm::Sensors::SensoriEMGmonopolar(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("HD10MM0804"))
				return gcnew OTComm::Sensors::SensorHD10MM0804(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("HD08MM1305"))
				return gcnew OTComm::Sensors::SensorHD08MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("HD10MM0808"))
				return gcnew OTComm::Sensors::SensorHD10MM0808(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("HD04MM1305"))
				return gcnew OTComm::Sensors::SensorHD04MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("HD20MM1602"))
				return gcnew OTComm::Sensors::SensorHD20MM1602(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			return nullptr;
		}

		GenericChannel^ DataGeneratorSessantaquattro::GetChannelFromString(String^ text)
		{
			if (text->Contains("CoDe1"))
				return gcnew OTComm::Sensors::SensorCoDe1_0();
			if (text->Contains("CoDe2"))
				return gcnew OTComm::Sensors::SensorCoDe2_0();
			if (text->Contains("De1"))
				return gcnew OTComm::Sensors::SensorDe1();
			if (text->Contains("CDE"))
				return gcnew OTComm::Sensors::SensorCDE();
			//if (text->Contains("ELSCH004q"))
			//	return gcnew OTComm::Sensors::SensorELSCH004q();
			if (text->Contains("ELSCH004"))
				return gcnew OTComm::Sensors::SensorELSCH004();
			if (text->Contains("ELSCH008"))
				return gcnew OTComm::Sensors::SensorELSCH008();
			if (text->Contains("SA8_10"))
				return gcnew OTComm::Sensors::SensorSA8_10();
			if (text->Contains("ELSCH016"))
				return gcnew OTComm::Sensors::SensorELSCH016();
			if (text->Contains("SA16_2_5"))
				return gcnew OTComm::Sensors::SensorSA16_2_5();
			if (text->Contains("SA16_5"))
				return gcnew OTComm::Sensors::SensorSA16_5();
			if (text->Contains("SA16_10"))
				return gcnew OTComm::Sensors::SensorSA16_10();
			if (text->Contains("ELSCH032NM6"))
				return gcnew OTComm::Sensors::SensorELSCH032NM6();
			if (text->Contains("ELSCH064R3S"))
				return gcnew OTComm::Sensors::SensorELSCH064R3S();
			if (text->Contains("ELSCH064NM5"))
				return gcnew OTComm::Sensors::SensorELSCH064NM5();
			if (text->Contains("ELSCH064NM7"))
				return gcnew OTComm::Sensors::SensorELSCH064NM7();
			if (text->Contains("ELSCH064NM4"))
				return gcnew OTComm::Sensors::SensorELSCH064NM4();
			if (text->Contains("ELSCH064NM3"))
				return gcnew OTComm::Sensors::SensorELSCH064NM3();
			if (text->Contains("ELSCH064NM2"))
				return gcnew OTComm::Sensors::SensorELSCH064NM2();
			if (text->Contains("GR10MM0808"))
				return gcnew OTComm::Sensors::SensorGR10MM0808();
			if (text->Contains("GR08MM1305"))
				return gcnew OTComm::Sensors::SensorGR08MM1305();
			if (text->Contains("GR04MM1305"))
				return gcnew OTComm::Sensors::SensorGR04MM1305();
			if (text->Contains("GR10MM0804"))
				return gcnew OTComm::Sensors::SensorGR10MM0804();
			if (text->Contains("AUX  Acceleration"))
				return gcnew OTComm::Sensors::AUXAcc();
			if (text->Contains("AUX  Torque"))
				return gcnew OTComm::Sensors::AUXTor();
			if (text->Contains("AUX  Force"))
				return gcnew OTComm::Sensors::AUXFor();
			if (text->Contains("AUX  Trigger"))
				return gcnew OTComm::Sensors::AUXTrig();
			if (text->Contains("iEMGb"))
				return gcnew OTComm::Sensors::SensoriEMGbipolar();
			if (text->Contains("iEMGm"))
				return gcnew OTComm::Sensors::SensoriEMGmonopolar();
			if (text->Contains("HD10MM0804"))
				return gcnew OTComm::Sensors::SensorHD10MM0804();
			if (text->Contains("HD08MM1305"))
				return gcnew OTComm::Sensors::SensorHD08MM1305();
			if (text->Contains("HD10MM0808"))
				return gcnew OTComm::Sensors::SensorHD10MM0808();
			if (text->Contains("HD04MM1305"))
				return gcnew OTComm::Sensors::SensorHD04MM1305();
			if (text->Contains("HD20MM1602"))
				return gcnew OTComm::Sensors::SensorHD20MM1602();
			return nullptr;
		}

		unsigned int DataGeneratorSessantaquattro::GetNumberTotalChannels()
		{
			return GetNumberChannelsNonAux() + GetNumberChannelsAux() + GetNumberControlAux();
		}

		unsigned int DataGeneratorSessantaquattro::GetNumberChannelsNonAux()
		{
			return (((unsigned int)mModel) % 10000);
		}

		unsigned int DataGeneratorSessantaquattro::GetNumberChannelsAux()
		{
			return 2;
		}

		unsigned int DataGeneratorSessantaquattro::GetNumberControlAux()
		{
			return 2;
		}

		unsigned int DataGeneratorSessantaquattro::GetNumberConnectorsAux()
		{
			return GetNumberChannelsAux();
		}

		unsigned int DataGeneratorSessantaquattro::GetNumberConnectorsIn()
		{
			return 1;
		}

		int DataGeneratorSessantaquattro::PortNumber::get()
		{
			return mPortNumber;
		}
#pragma endregion

#pragma region Device properties 

		void DataGeneratorSessantaquattro::SetIPAddress(String^ IPAddress)
		{
			mIPAdress = IPAddress;
		}

		void DataGeneratorSessantaquattro::SetIPPort(unsigned int IPPort)
		{
			mPortNumber = IPPort;
		}

		unsigned int DataGeneratorSessantaquattro::GetDeviceModel()
		{
			return (unsigned int)mModel;
		}

		Bitmap^ DataGeneratorSessantaquattro::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("64_sessantaquattro.jpg", true);
		}

		String^ DataGeneratorSessantaquattro::GetDeviceDescription(void)
		{
			String^ res = gcnew String("SESSANTAQUATTRO");
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				String^ version;
				if (GetFirmwareVersion(version) > 0)
					res += "; ID=" + version;
				else
					res += "; ID=Unknown";
			}
			return res;
		}

		String^ DataGeneratorSessantaquattro::IPAdress::get()
		{
			String^ tmp = gcnew String(mIPAdress);
			return tmp;
		}

#pragma endregion

#pragma region Public device operations
		void DataGeneratorSessantaquattro::OpenDevice(void)
		{

			LOG_ENTERING;
			try
			{
				//InternalStopQuattrocentoDataTransfer();
				DataGeneratorInterface::OpenDevice();
			}
			catch (Exception ^e)
			{
				OpenDeviceFailedException^odfe = gcnew OpenDeviceFailedException(__FUNCTION__, "", e);
				FireOnError(odfe);
				throw odfe;
			}
			mDeviceIsOpen = true;
			LOG_SUCCESS;
			LOG_EXITING;
		}

		void DataGeneratorSessantaquattro::InitializeDevice()
		{
			LOG_ENTERING;
			try
			{
				//InternalStopQuattrocentoDataTransfer();
				DataGeneratorInterface::InitializeDevice();
			}
			catch (Exception ^e)
			{
				InitializeDeviceFailedException^idfe = gcnew InitializeDeviceFailedException(__FUNCTION__, "", e);
				FireOnError(idfe);
				throw idfe;
			}
			LOG_EXITING;
		}

		void DataGeneratorSessantaquattro::StartAcquisition(void)
		{
			LOG_ENTERING;
			/*if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			{
				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
				FireOnError(ihe);
				throw ihe;
			}
			else
			{*/
			DataGeneratorInterface::StartAcquisition();
			mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorSessantaquattro::OuternThread));
			mManagedAcquisitionThread->Start();
			//}
			LOG_EXITING;
		}

		void DataGeneratorSessantaquattro::StopAcquisition(void)
		{
			LOG_ENTERING;
			/*if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{*/
			DataGeneratorInterface::StopAcquisition();
			/*InternalStopSessantaquattroDataTransfer();
		}*/
			LOG_EXITING;
		}

		void DataGeneratorSessantaquattro::CloseDevice(void)
		{
			LOG_ENTERING;
			/*if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				InternalStopSessantaquattroDataTransfer();
				closesocket((SOCKET)mDeviceHandle);
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
				mDeviceIsOpen = false;
				LOG_SUCCESS;
			}
			else
			{
				LOG_ERROR(" already closed");
			}*/
			DataGeneratorInterface::CloseDevice();
			LOG_EXITING;
		}

#pragma endregion

#pragma region Non public device operations


		int DataGeneratorSessantaquattro::ReadDeviceID(UCHAR* dev_id)
		{
			if (mDeviceHandle == INVALID_HANDLE_VALUE)
				return -5;
			return 0;
		}

		int DataGeneratorSessantaquattro::GetFirmwareVersion(String^% version)
		{
			LOG_ENTERING;
			int ver = 0;
			/*if (WriteString("firm") > 0)
			{
			ver = ReadString(version);
			}*/
			LOG_EXITING;
			return ver;
		}

		array<Byte>^ DataGeneratorSessantaquattro::CreateStartCommand()
		{
			array<Byte>^ command = CreateSetCommand();
			command[1] += 1;
			if (GetConnectors()[0]->DetectionMode == DetectionModes::ImpedenceCheck)
				command = CreateImpedenceCheckCommand();
			else if (GetConnectors()[0]->DetectionMode == DetectionModes::ImpedenceCheckAdvanced) {
				command = CreateAdvancedImpedenceCheckCommand();
			}
			return command;
		}

		array<Byte>^ DataGeneratorSessantaquattro::CreateStopCommand()
		{
			array<Byte>^ command = CreateSetCommand();
			command[1] += 0;
			return command;
		}

		array<Byte>^ DataGeneratorSessantaquattro::CreateSetCommand()
		{
			array<Byte>^ command = gcnew array<Byte>(13);

			// Byte 0
			command[0] += GetConnectors()[0]->DetectionMode;
			if (GetConnectors()[0]->Adapter != nullptr)
			{
				if (GetConnectors()[0]->DetectionMode == Sessantaquattro::DetectionModes::Bipolar)// bipolar
				{
					if (GetConnectors()[0]->Adapter->Channels->Length == 8)
						command[0] += 8;
					else if (GetConnectors()[0]->Adapter->Channels->Length == 16)
						command[0] += 16;
					else if (GetConnectors()[0]->Adapter->Channels->Length == 32)
						command[0] += 24;
				}
				else
				{
					if (GetConnectors()[0]->Adapter->Channels->Length == 16)
						command[0] += 8;
					if (GetConnectors()[0]->Adapter->Channels->Length == 32)
						command[0] += 16;
					if (GetConnectors()[0]->Adapter->Channels->Length == 64)
						command[0] += 24;
				}
			}
			else // if I do not have an adapter, I use the AD8x1 bipolar
			{
				//if (GetConnectors()[0]->DetectionMode == Sessantaquattro::DetectionModes::Bipolar)// bipolar
				//{
				//	if (GetConnectors()[0]->Adapter->Channels->Length == 8)
				command[0] = 1; // bipolar
				command[0] += 8; // nchannels
			}
			if (GetSamplingFrequency() == 1000)
				command[0] += 32;
			else if (GetSamplingFrequency() == 2000)
				command[0] += 64;
			else if (GetSamplingFrequency() == 4000)
				command[0] += 96;

			// Byte 1
			if (mSources[0]->mBytesPerChannel == 3)
				command[1] = 128; // hd without high pass filter
			else
				command[1] = 64; // high pass filter

			return command;
		}

		array<Byte>^ DataGeneratorSessantaquattro::CreateImpedenceCheckCommand()
		{
			array<Byte>^ command = gcnew array<Byte>(13);

			// Byte 0
			command[0] += GetConnectors()[0]->DetectionMode;
			if (GetConnectors()[0]->Adapter != nullptr)
			{
				if (GetConnectors()[0]->DetectionMode == Sessantaquattro::DetectionModes::Bipolar)// bipolar
				{
					if (GetConnectors()[0]->Adapter->Channels->Length == 8)
						command[0] += 8;
					else if (GetConnectors()[0]->Adapter->Channels->Length == 16)
						command[0] += 16;
					else if (GetConnectors()[0]->Adapter->Channels->Length == 32)
						command[0] += 24;
				}
				else
				{
					if (GetConnectors()[0]->Adapter->Channels->Length == 16)
						command[0] += 8;
					if (GetConnectors()[0]->Adapter->Channels->Length == 32)
						command[0] += 16;
					if (GetConnectors()[0]->Adapter->Channels->Length == 64)
						command[0] += 24;
				}
			}
			else // if I do not have an adapter, I use the AD8x1 bipolar
			{
				//if (GetConnectors()[0]->DetectionMode == Sessantaquattro::DetectionModes::Bipolar)// bipolar
				//{
				//	if (GetConnectors()[0]->Adapter->Channels->Length == 8)
				command[0] = 1; // bipolar
				command[0] += 8; // nchannels
			}
			if (GetSamplingFrequency() == 1000)
				command[0] += 32;
			else if (GetSamplingFrequency() == 2000)
				command[0] += 64;
			else if (GetSamplingFrequency() == 4000)
				command[0] += 96;

			// Byte 1
			command[1] = 128; // hd without high pass filter
			command[1] += 64; // high pass filter
			command[1] += 1; // GO

			return command;
		}

		array<Byte>^ DataGeneratorSessantaquattro::CreateAdvancedImpedenceCheckCommand()
		{
			array<Byte>^ command = gcnew array<Byte>(13);

			// Byte 0
			command[0] += GetConnectors()[0]->DetectionMode;
			if (GetConnectors()[0]->Adapter != nullptr)
			{
				if (GetConnectors()[0]->DetectionMode == Sessantaquattro::DetectionModes::Bipolar)// bipolar
				{
					if (GetConnectors()[0]->Adapter->Channels->Length == 8)
						command[0] += 8;
					else if (GetConnectors()[0]->Adapter->Channels->Length == 16)
						command[0] += 16;
					else if (GetConnectors()[0]->Adapter->Channels->Length == 32)
						command[0] += 24;
				}
				else
				{
					if (GetConnectors()[0]->Adapter->Channels->Length == 16)
						command[0] += 8;
					if (GetConnectors()[0]->Adapter->Channels->Length == 32)
						command[0] += 16;
					if (GetConnectors()[0]->Adapter->Channels->Length == 64)
						command[0] += 24;
				}
			}
			else // if I do not have an adapter, I use the AD8x1 bipolar
			{
				//if (GetConnectors()[0]->DetectionMode == Sessantaquattro::DetectionModes::Bipolar)// bipolar
				//{
				//	if (GetConnectors()[0]->Adapter->Channels->Length == 8)
				command[0] = 1; // bipolar
				command[0] += 8; // nchannels
			}
			if (GetSamplingFrequency() == 1000)
				command[0] += 32;
			else if (GetSamplingFrequency() == 2000)
				command[0] += 64;
			else if (GetSamplingFrequency() == 4000)
				command[0] += 96;

			// Byte 1
			command[1] = 128; // hd without high pass filter
			command[1] += 64; // high pass filter
			command[1] += 1; // GO

			return command;
		}

		array<Byte>^ DataGeneratorSessantaquattro::CreateGetCommand()
		{
			array<Byte>^ command = gcnew array<Byte>(13);
			command[0] += 128;
			return command;
		}

		Byte DataGeneratorSessantaquattro::ComputeCRC(array<Byte>^ command, unsigned int Len)
		{
			unsigned int crc = 0x00;
			unsigned int extract = 0x00;
			int j = 0;
			int sum = 0;
			while (Len > 0)
			{
				extract = Convert::ToUInt16(command[j]);
				for (int tempI = 8; tempI > 0; tempI--)
				{
					sum = (crc ^ extract) & 0x01;
					crc >>= 1;
					if (sum > 0)
						crc ^= 0x8C;

					extract >>= 1;
				}
				Len--;
				j++;
			}

			return Convert::ToByte(crc);
		}

		void DataGeneratorSessantaquattro::OuternThread()
		{
			try
			{
				DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				//mDataReaderInterfaceHelper->mAuxData = mUseAux;
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;

				//genero stringa con i comandi

				array<Byte>^ command = CreateStartCommand();
				pin_ptr<Byte> CommandPinned = &command[0];   // entire array is now pinned  
				mDataReaderInterfaceHelper->mCustomData = (LPARAM)CommandPinned;
				array<Byte>^ stopcommand = CreateStopCommand();
				pin_ptr<Byte> StopCommandPinned = &stopcommand[0];   // entire array is now pinned  
				mDataReaderInterfaceHelper->mStop64Command = (LPARAM)StopCommandPinned;

				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoSessantaquattro,  // Thread Func
					mDataReaderInterfaceHelper, // Thread parameter
					0,             // Creation Flags
					NULL);        // Don't need the Thread Id.
				if (mAsyncIoThreadHandle == NULL)
				{
					ThreadCreationException^ tce = gcnew ThreadCreationException(__FUNCTION__);
					FireOnError(tce);
					throw tce;
				}

				WaitForSingleObject(mAsyncIoThreadHandle, INFINITE);

				//when comunication it's closed, save ip address
				try {
					String^ IP = gcnew String(mDataReaderInterfaceHelper->ipAddress);
					this->SetIPAddress(IP);
				}
				catch(Exception^ ex){
					this->SetIPAddress("192.168.1.1");
				}

			}
			catch (Exception ^e)
			{
				AcquisitionThreadException^ ate = gcnew AcquisitionThreadException(__FUNCTION__, e);
				FireOnError(ate);
			}
			LOG_EXITING;
		}

		void DataGeneratorSessantaquattro::InternalStopSessantaquattroDataTransfer(void)
		{
			LOG_ENTERING;
			//if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			//{
			//	// Stop acquisition
			//	try
			//	{
			//		//WriteString("stop");
			//		LOG_ENTERING; \
			//			if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			//			{
			//				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
			//				FireOnError(ihe);
			//				throw ihe;
			//			}
			//		DWORD res = 0;
			//		array<Byte>^ command = CreateStopCommand();
			//		pin_ptr<Byte> CommandPinned = &command[0];   // entire array is now pinned  
			//													 //IntPtr ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(command);
			//													 //const char* buf = (LPCSTR)command[0].ToPointer();
			//		DWORD numberOfBytesTransferred;
			//		OVERLAPPED *OverlappedStruc;
			//		OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			//		if (OverlappedStruc == NULL)
			//		{
			//			OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
			//			FireOnError(ose);
			//			throw ose;
			//		}
			//		ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			//		//OverlappedStruc->Internal = STATUS_PENDING;
			//		SetLastError(0);
			//		BOOL WriteFileRes = WriteFile(mDeviceHandle, (LPCVOID)&CommandPinned[0], 40, &numberOfBytesTransferred, OverlappedStruc);
			//		if (WriteFileRes == FALSE)
			//		{
			//			if (GetLastError() == ERROR_IO_PENDING)
			//			{
			//				if (!GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
			//				{
			//					throw gcnew OverlappedCommunicationException(__FUNCTION__);
			//					numberOfBytesTransferred = -1;
			//				}
			//				else
			//				{
			//					//wait between read/write operations
			//					Sleep(10);
			//					res = numberOfBytesTransferred;
			//				}
			//			}
			//			else
			//			{
			//				throw gcnew OverlappedCommunicationException(__FUNCTION__);
			//				numberOfBytesTransferred = -1;
			//			}
			//		}
			//		else
			//		{
			//			Sleep(10);
			//			res = numberOfBytesTransferred;
			//		}
			//		LocalFree((void *)OverlappedStruc);
			//		OverlappedStruc = NULL;
			//		LOG_EXITING;
			//		return;
			//	}
			//	catch (Exception ^e)
			//	{
			//		CloseConnection();
			//		StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__, "", e);
			//		FireOnError(safe);
			//		throw safe;
			//	}
			//}
			LOG_EXITING;
		}

		void DataGeneratorSessantaquattro::CloseConnection()
		{
			DebugLog::Print(String::Format("Connection closed"));
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				closesocket((SOCKET)mDeviceHandle);
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
			}
		}

		int DataGeneratorSessantaquattro::WriteString(String^ command)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			{
				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
				FireOnError(ihe);
				throw ihe;
			}
			DWORD res = 0;
			IntPtr ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(command);
			const char* buf = (LPCSTR)ptrtxt.ToPointer();
			DWORD numberOfBytesTransferred;
			OVERLAPPED *OverlappedStruc;
			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				FireOnError(ose);
				throw ose;
			}
			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			//OverlappedStruc->Internal = STATUS_PENDING;
			SetLastError(0);
			BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, (DWORD)strlen(buf), &numberOfBytesTransferred, OverlappedStruc);
			if (WriteFileRes == FALSE)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					if (!GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
					{
						throw gcnew OverlappedCommunicationException(__FUNCTION__);
						numberOfBytesTransferred = -1;
					}
					else
					{
						//wait between read/write operations
						Sleep(10);
						res = numberOfBytesTransferred;
					}
				}
				else
				{
					throw gcnew OverlappedCommunicationException(__FUNCTION__);
					numberOfBytesTransferred = -1;
				}
			}
			else
			{
				Sleep(10);
				res = numberOfBytesTransferred;
			}
			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;
			LOG_EXITING;
			return res;
		}

		int DataGeneratorSessantaquattro::ReadString(String^ %read)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			{
				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
				FireOnError(ihe);
				throw ihe;
			}

			char ByteArray[128];

			DWORD numberOfBytesTransferred;
			OVERLAPPED *OverlappedStruc;
			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
				FireOnError(ose);
				throw ose;
			}

			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			//OverlappedStruc->Internal = STATUS_PENDING;
			SetLastError(0);
			BOOL ReadFileRes = ReadFile(mDeviceHandle, ByteArray, countof(ByteArray), &numberOfBytesTransferred, OverlappedStruc);
			if (ReadFileRes == FALSE)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{


					DWORD dwResult = WaitForSingleObject(OverlappedStruc->hEvent, 1000);
					if (dwResult != WAIT_OBJECT_0 ||
						!::GetOverlappedResult(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE)
						)
					{
						CloseHandle(OverlappedStruc->hEvent);
						LocalFree((void *)OverlappedStruc);
						OverlappedStruc = NULL;
						OverlappedCommunicationException^ oce = gcnew OverlappedCommunicationException(__FUNCTION__);
						FireOnError(oce);
						throw oce;
						return -1;
					}
					else
						Sleep(10);
				}
				else
				{
					FireOnError("problem reading", GetLastError(), __FUNCTION__);
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					return -1;
				}
			}
			else
				Sleep(10);

			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;

			read = gcnew String(ByteArray, 0, numberOfBytesTransferred);
			return numberOfBytesTransferred;
			LOG_EXITING;
		}

#pragma endregion		

#pragma region Validators
		DataValidator^ DataGeneratorSessantaquattro::DefaultTriggerValidator::get()
		{
			return mDefaulDataValidatorTrigger;
		}
#pragma endregion 

#pragma region Unmanaged code

#pragma unmanaged


		union SwapShort
		{
			unsigned char Bytes[2];
			unsigned short Short;
		};

		ULONG _stdcall AsyncIoSessantaquattro(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ASYNCH_IO = 10;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			unsigned int NumBytesPerSample = helper->mSource[0].mBytesPerChannel; //2 or 3
			unsigned char * StartCommand = (unsigned char *)helper->mCustomData;
			unsigned char * StopCommand = (unsigned char *)helper->mStop64Command;


			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;

			//"acquition" (i.e, read from device) buffer size. This is the size, in bytes of each read buffer, which will be later multiplied by NUM_ASYNC_IO
			ULONG SingleAcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels*helper->mSource[0].mDataRegroupFactor * NumBytesPerSample;
			UCHAR * AcquisitionBuffer = NULL;

			//Temporary buffer used to store the acquired data before selecting only the samples from the used channels.
			ULONG TempBufferSizeInBytes = helper->mDeviceTotalChannels*helper->mSource[0].mDataRegroupFactor*NumBytesPerSample;
			UCHAR* TempBuffer = NULL;
			UCHAR *TempBufferPos;

			/////////////////////////////////////////////////////////
			ULONG DecodedBufferSizeUnitary = helper->mSource[0].mAcquiredChannels *helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSize = DecodedBufferSizeUnitary * NUM_DECODED_BUFFERS;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize * (NumBytesPerSample == 2 ? 2 : 4);
			unsigned short *DecodedBuffer = NULL;
			unsigned short *DecodedBufferPos = NULL;
			unsigned short *DecodedBufferCurrentStart = NULL;

			unsigned __int32 *DecodedBuffer32 = NULL;
			unsigned __int32 *DecodedBufferPos32 = NULL;
			unsigned __int32 *DecodedBufferCurrentStart32 = NULL;
			ULONG numberOfBytesTransferred;




			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////CREAZIONE DEL SOCKET IN ASCOLTO/////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			WSADATA wsaData;
			SOCKET ListenSocket;
			SOCKADDR_IN InternetAddr;
			if (WSAStartup((2, 2), &wsaData) != 0)
			{
				WSACleanup();
				return 1;
			}
			if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
			{
				WSACleanup();
				return -1;
			}

			InternetAddr.sin_family = AF_INET;
			InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //inet_addr("192.168.1.1");
															   //InetPton(AF_INET, _T("192.168.1.1"), &InternetAddr.sin_addr.s_addr);
			InternetAddr.sin_port = htons(45454);				//EUGENIO: da sistemare in base alla configurazione 
			if (bind(ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
			{
				closesocket(ListenSocket);
				WSACleanup();
				helper->mError("Timeout, socket error. Listener could not be started", -1, __FUNCTION__);
				return -1;
			}
			int ls;
			if (ls = listen(ListenSocket, 1))
			{
				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}
			// Setup the listening socket for connections
			DWORD NonBlock = 1;
			if (ioctlsocket(ListenSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
			{
				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}
			
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////ATTESA DEL SESSANTAQUATTRO//////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			fd_set ReadSet;
			fd_set WriteSet;
			
			int rv;
			FD_ZERO(&ReadSet); /* clear the set */
			FD_ZERO(&WriteSet);
			FD_SET(ListenSocket, &ReadSet); /* add our file descriptor to the set */
			FD_SET(ListenSocket, &WriteSet); /* add our file descriptor to the set */
			struct timeval timeout;
			timeout.tv_sec = 10;								//PRENDERE DA CONFIGURAZIONE?
			timeout.tv_usec = 0;

			char recvbuf[100];
			int recvbuflen = 100;

			rv = select(1, &ReadSet, &WriteSet, NULL, &timeout);
			if (rv == -1)
			{
				closesocket(ListenSocket);
				WSACleanup();
				helper->mError("Timeout, rv returned -1", -1, __FUNCTION__);
				return -1;
			}
			else if (rv == 0)   //timeout
			{
				closesocket(ListenSocket);
				WSACleanup();
				helper->mError("Timeout, no socket accepted", -2, __FUNCTION__);
				return -2;
			}

			SOCKET AcceptSocket;
			DWORD Flags = 0;
			DWORD RecvBytes;

			// Accept inbound connections
			if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) == INVALID_SOCKET)
			{
				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}


			//GET IP ADDRESS OF CONNECTED SESSANTAQUATTRO
			sockaddr_in SockAddr;
			int addrlen = sizeof(SockAddr);
			if (getpeername(AcceptSocket, (LPSOCKADDR)&SockAddr, &addrlen) == SOCKET_ERROR)
			{
				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}
			char *ipstr = new char[16];
			strcpy(ipstr, inet_ntoa(SockAddr.sin_addr));
			helper->ipAddress = ipstr;

			OVERLAPPED *OverlappedStructWrite;
			OverlappedStructWrite = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			if (OverlappedStructWrite == NULL)
			{
				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}
			int res = 0;

			HANDLE hCompletionPort = CreateIoCompletionPort((HANDLE)AcceptSocket, NULL, 1, 0);
			if (hCompletionPort == NULL)
			{
				LocalFree((void *)OverlappedStructWrite);
				OverlappedStructWrite = NULL;
				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}
			OVERLAPPED *OverlappedStruc = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, NUM_ASYNCH_IO * sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				closesocket(ListenSocket);
				WSACleanup();
				CloseHandle(hCompletionPort);
				LocalFree((void *)OverlappedStructWrite);
				OverlappedStructWrite = NULL;
				return -1;
			}
			for (int i = 0; i < NUM_ASYNCH_IO; i++)
			{
				ZeroMemory(&OverlappedStruc[i], sizeof(OVERLAPPED));
				OverlappedStruc[i].Internal = STATUS_PENDING;
			}

			//data acquired from the instrument
			AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, SingleAcquisitionBufferSizeInBytes*NUM_ASYNCH_IO);
			if (AcquisitionBuffer == NULL)
			{

				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStructWrite);
				OverlappedStructWrite = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}

			//temporary buffer
			TempBuffer = (UCHAR*)LocalAlloc(LPTR, TempBufferSizeInBytes);
			if (TempBuffer == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStructWrite);
				OverlappedStructWrite = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}
			TempBufferPos = TempBuffer;

			if (NumBytesPerSample == 2)
			{
				//decoded buffer
				DecodedBuffer = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
				if (DecodedBuffer == NULL)
				{
					CloseHandle(hCompletionPort);
					hCompletionPort = NULL;
					LocalFree((void *)OverlappedStructWrite);
					OverlappedStructWrite = NULL;
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					LocalFree((void *)AcquisitionBuffer);
					AcquisitionBuffer = NULL;
					LocalFree((void *)TempBuffer);
					TempBuffer = NULL;
					closesocket(ListenSocket);
					WSACleanup();
					return -1;
				}
				DecodedBufferPos = DecodedBuffer;
				DecodedBufferCurrentStart = DecodedBufferPos;
			}
			else //3
			{
				//decoded buffer
				DecodedBuffer32 = (unsigned __int32*)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
				if (DecodedBuffer32 == NULL)
				{
					CloseHandle(hCompletionPort);
					hCompletionPort = NULL;
					LocalFree((void *)OverlappedStructWrite);
					OverlappedStructWrite = NULL;
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					LocalFree((void *)AcquisitionBuffer);
					AcquisitionBuffer = NULL;
					LocalFree((void *)TempBuffer);
					TempBuffer = NULL;
					closesocket(ListenSocket);
					WSACleanup();
					return -1;
				}
				DecodedBufferPos32 = DecodedBuffer32;
				DecodedBufferCurrentStart32 = DecodedBufferPos32;
			}

			OverlappedStructWrite = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			if (OverlappedStructWrite == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStructWrite);
				OverlappedStructWrite = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				LocalFree((void *)TempBuffer);
				TempBuffer = NULL;
				if (DecodedBuffer != NULL)
				{
					LocalFree((void *)DecodedBuffer);
					DecodedBuffer = NULL;
				}
				if (DecodedBuffer32 != NULL)
				{
					LocalFree((void *)DecodedBuffer32);
					DecodedBuffer32 = NULL;
				}

				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}
			ZeroMemory(OverlappedStructWrite, sizeof(OVERLAPPED));

			SetLastError(0);
			WSABUF DataBufWrite;

			DataBufWrite.len = 2;
			DataBufWrite.buf = (char*)StartCommand;

			if (WSASend(AcceptSocket, &DataBufWrite, 1, &numberOfBytesTransferred, 0, OverlappedStructWrite, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() == ERROR_IO_PENDING)
				{
					DWORD dwResult = WaitForSingleObject(OverlappedStructWrite->hEvent, 1000);
					if (dwResult != WAIT_OBJECT_0 ||
						!::GetOverlappedResult(helper->mDeviceHandle, OverlappedStructWrite, &numberOfBytesTransferred, TRUE)
						)
					{
						res = -1;
					}
				}
			}

			Sleep(10);
			if (res != 0)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStructWrite);
				OverlappedStructWrite = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				LocalFree((void *)TempBuffer);
				TempBuffer = NULL;
				if (DecodedBuffer != NULL)
				{
					LocalFree((void *)DecodedBuffer);
					DecodedBuffer = NULL;
				}
				if (DecodedBuffer32 != NULL)
				{
					LocalFree((void *)DecodedBuffer32);
					DecodedBuffer32 = NULL;
				}
				closesocket(ListenSocket);
				WSACleanup();
				return -1;
			}


			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;
			unsigned int nLettura = 0;

			WSABUF DataBuf[NUM_ASYNCH_IO];

			for (int i = 0; i < NUM_ASYNCH_IO; i++)
			{
				DataBuf[i].buf = (char*)&AcquisitionBuffer[i*SingleAcquisitionBufferSizeInBytes];
				DataBuf[i].len = SingleAcquisitionBufferSizeInBytes;
				if (WSARecv(AcceptSocket, &DataBuf[i], 1, &numberOfBytesTransferred, &Flags, &(OverlappedStruc[i]), NULL) == SOCKET_ERROR)
				{

					if (WSAGetLastError() != ERROR_IO_PENDING)
					{
						CloseHandle(hCompletionPort);
						hCompletionPort = NULL;
						LocalFree((void *)OverlappedStruc);
						OverlappedStruc = NULL;
						LocalFree((void *)OverlappedStructWrite);
						OverlappedStructWrite = NULL;
						LocalFree((void *)AcquisitionBuffer);
						AcquisitionBuffer = NULL;
						LocalFree((void *)TempBuffer);
						TempBuffer = NULL;
						if (DecodedBuffer != NULL)
						{
							LocalFree((void *)DecodedBuffer);
							DecodedBuffer = NULL;
						}
						if (DecodedBuffer32 != NULL)
						{
							LocalFree((void *)DecodedBuffer32);
							DecodedBuffer32 = NULL;
						}
						closesocket(ListenSocket);
						WSACleanup();
						return -1;
					}
				}
			}

			WHILE(*mMustStopAcquisitionThread == false)
			{
				SetLastError(0);
				OVERLAPPED *completedOv;
				ULONG_PTR   key;

				if (GetQueuedCompletionStatus(hCompletionPort, &numberOfBytesTransferred, &key, &completedOv, INFINITE) == 0)
				{
					helper->mError("", GetLastError(), __FUNCTION__);
					exitMainLoop = true;
				}
				if (numberOfBytesTransferred == 0)
				{
					helper->mError("No more data", 0, __FUNCTION__);
					exitMainLoop = true;
				}

				size_t ReceivedBufferIndex = completedOv - OverlappedStruc;
				if (ReceivedBufferIndex < NUM_ASYNCH_IO)
				{
					UCHAR* AcquisitionBufferCurrentReadStart =
						&AcquisitionBuffer[ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes];
					//calculate the remaining space
					size_t FreeTempBufferBytes = TempBuffer + TempBufferSizeInBytes - TempBufferPos;

					if (FreeTempBufferBytes > numberOfBytesTransferred)
					{
						memcpy(TempBufferPos, AcquisitionBufferCurrentReadStart, numberOfBytesTransferred);
						TempBufferPos += numberOfBytesTransferred;
					}
					else
					{
						memcpy(TempBufferPos, AcquisitionBufferCurrentReadStart, FreeTempBufferBytes);
						//the buffer is full: select the data to transfer and send it
						if (NumBytesPerSample == 2)
						{
							DecodedBufferPos = DecodedBufferCurrentStart;

							unsigned short *TempBufferAsShort = (unsigned short*)TempBuffer;
							for (unsigned int koffset = 0; koffset < helper->mSource[0].mDataRegroupFactor; koffset++)
							{
								for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
								{
									//add control for buffer overflow
									SwapShort ss;
									ss.Short = TempBufferAsShort[ChannelMappings[channel]];
									unsigned char tmp = ss.Bytes[0];
									ss.Bytes[0] = ss.Bytes[1];
									ss.Bytes[1] = tmp;// +128;
									*DecodedBufferPos = ss.Short;// TempBufferAsShort[ChannelMappings[channel]];
									DecodedBufferPos++;
								}
								TempBufferAsShort += helper->mDeviceTotalChannels;
							}
							helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);
							DecodedBufferCurrentStart = DecodedBufferPos;
							if (DecodedBufferCurrentStart >= DecodedBufferSize + DecodedBuffer)
								DecodedBufferCurrentStart = DecodedBuffer;
						}
						else //3
						{
							DecodedBufferPos32 = DecodedBufferCurrentStart32;
							UCHAR *TempBufferAsInt32 = (UCHAR*)TempBuffer;

							for (unsigned int koffset = 0; koffset < helper->mSource[0].mDataRegroupFactor; koffset++)
							{
								for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
								{
									//add control for buffer overflow
									size_t posMappedChannel = 3 * ChannelMappings[channel];
									unsigned __int32 B0 = TempBufferAsInt32[posMappedChannel];
									unsigned __int32 B1 = TempBufferAsInt32[posMappedChannel + 1];
									unsigned __int32 B2 = TempBufferAsInt32[posMappedChannel + 2];

									if (B0 & 0x80)
										*DecodedBufferPos32 = 0xFF000000 | (((B0) << 16) | (B1 << 8) | B2);// TempBufferAsShort[ChannelMappings[channel]];
									else
										*DecodedBufferPos32 = (B0 << 16) | (B1 << 8) | B2;// TempBufferAsShort[ChannelMappings[channel]];


									/*if (*DecodedBufferPos32 >= 8388608)
										*DecodedBufferPos32 -= 16777216;*/
									DecodedBufferPos32++;
								}
								TempBufferAsInt32 += helper->mDeviceTotalChannels * 3;
							}
							helper->mNewDataAvailable(0, DataSize::ThirtytwoBits, DecodedBufferCurrentStart32, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);
							DecodedBufferCurrentStart32 = DecodedBufferPos32;
							if (DecodedBufferCurrentStart32 >= DecodedBufferSize + DecodedBuffer32)
								DecodedBufferCurrentStart32 = DecodedBuffer32;
						}


						nLettura++;

						//copy the possibly remaining acquired data
						//if (numberOfBytesTransferred - FreeTempBufferBytes != 0)
						{
							memcpy(TempBuffer, AcquisitionBufferCurrentReadStart + FreeTempBufferBytes, numberOfBytesTransferred - FreeTempBufferBytes);
							TempBufferPos = TempBuffer + numberOfBytesTransferred - FreeTempBufferBytes;
						}
					}


					if (*mMustStopAcquisitionThread) break;
					SetLastError(0);

					if (WSARecv(AcceptSocket, &DataBuf[ReceivedBufferIndex], 1, &numberOfBytesTransferred, &Flags, completedOv, NULL) == SOCKET_ERROR)
					{
						if (WSAGetLastError() != ERROR_IO_PENDING)
						{
							*mMustStopAcquisitionThread = true;
							//return -1;
						}
					}
				}

			}//while

			DataBufWrite.len = 2;
			DataBufWrite.buf = (char*)StopCommand;

			if (WSASend(AcceptSocket, &DataBufWrite, 1, &numberOfBytesTransferred, 0, OverlappedStructWrite, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() == ERROR_IO_PENDING)
				{
					DWORD dwResult = WaitForSingleObject(OverlappedStructWrite->hEvent, 1000);
					if (dwResult != WAIT_OBJECT_0 ||
						!::GetOverlappedResult(helper->mDeviceHandle, OverlappedStructWrite, &numberOfBytesTransferred, TRUE)
						)
					{
						res = -1;
					}
				}
			}

			Sleep(10);


			if (!*mMustStopAcquisitionThread)
			{
				// Signal that acquisition ended before time
				//GlobalErrorDispatcher("An error occurred",helper->mError);
			}
			if (AcquisitionBuffer)
			{
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
			}
			if (TempBuffer)
			{
				LocalFree((void *)TempBuffer);
				TempBuffer = NULL;
			}
			if (DecodedBuffer)
			{
				LocalFree((void *)DecodedBuffer);
				DecodedBuffer = NULL;
			}
			if (DecodedBuffer32)
			{
				LocalFree((void *)DecodedBuffer32);
				DecodedBuffer32 = NULL;
			}

			CloseHandle(hCompletionPort);
			hCompletionPort = NULL;
			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;
			LocalFree((void *)OverlappedStructWrite);
			OverlappedStructWrite = NULL;

			closesocket(ListenSocket);
			WSACleanup();




			helper->mStopAcquisition();
			return 0;
		}

		
#pragma endregion
	}

}
