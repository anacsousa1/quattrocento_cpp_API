#include "DataGeneratorQuattrocento.h"
#include "stdio.h"
#include <string>

using namespace System::Xml;
namespace OTComm
{
	namespace Quattrocento
	{
#pragma comment(lib, "Ws2_32.lib")

#pragma managed


#pragma region Constructors and destructors
		DataGeneratorQuattrocento::DataGeneratorQuattrocento(Model model, Frequency SamplingFrequency) :
			DataGeneratorInterface(), mModel(model),
			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false),
			mDataReaderInterfaceHelper(NULL), mPortNumber(mDefaultPortNumber)
		{
			try
			{
				mIPAdress = String::Copy(mDefaultIPAdress);
				mConnectors = gcnew array<GenericConnector^>(GetNumberConnectorsMultipleIn() + GetNumberConnectorsIn() + GetNumberConnectorsAux() + GetNumberControlAux());
				unsigned int k = 0;
				for (; k < GetNumberConnectorsIn(); k++)
					mConnectors[k] = gcnew GenericConnector(16);
				for (; k < GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn(); k++)
					mConnectors[k] = gcnew GenericConnector(64);
				for (; k < (unsigned int)mConnectors->Length; k++)
					mConnectors[k] = gcnew GenericConnector(1);

				mSources = gcnew array<DataGeneratorSingleSource^>(1);
				mSources[0] = gcnew DataGeneratorSingleSource(SamplingFrequency, 3, GetChannelsMapping());

				//mSamplingFrequency = SamplingFrequency;
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
				mDeviceTotalChannels = GetNumberTotalChannels();
				mDefaulDataValidatorTrigger = gcnew DataValidatorTrigger(0, 32767, OTComm::TriggerModes::WhenEqualTo);
			}
			catch (Exception ^e)
			{
				ConstructorFailedException^ cfe = gcnew ConstructorFailedException(__FUNCTION__, e);
				throw cfe;
			}
		}

		DataGeneratorQuattrocento::~DataGeneratorQuattrocento()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorQuattrocento();
		}

		DataGeneratorQuattrocento::!DataGeneratorQuattrocento()
		{

		}

#pragma endregion

#pragma region XML files

		String^ DataGeneratorQuattrocento::GetDeviceXMLConfiguration()
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
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetNumberTotalChannels()-GetNumberControlAux()).ToString());// (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("IPAddress", mIPAdress);
			mXMLTextStream->WriteAttributeString("AnalogOutConn", mAnalogOutputConnector.ToString());
			mXMLTextStream->WriteAttributeString("AnalogOutCh", mAnalogOutputChannel.ToString());
			mXMLTextStream->WriteAttributeString("AnalogOutGain", mAnalogOutputGain.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			{
				mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			}

			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteStartElement("Channels");

			unsigned int ChannelNum = 0;
			int startIndex = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{
				startIndex = k < GetNumberConnectorsIn() ? 16 * k : (k < (GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn()) ? GetNumberConnectorsIn() * 16 + (k - GetNumberConnectorsIn()) * 64 : (k - GetNumberConnectorsIn() - GetNumberConnectorsMultipleIn()) + GetNumberConnectorsIn() * 16 + GetNumberConnectorsMultipleIn() * 64);

				if (mConnectors[k]->Adapter != nullptr)
				{
					if (mConnectors[k]->Adapter->ID->Contains("Direct connection to Auxiliary Input"))
					{
						mConnectors[k]->Adapter->AuxWriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							(float)mConnectors[k]->Adapter->AdapterGain,
							mConnectors[k]->LowPassFilter, 
							mConnectors[k]->HigPassFilter,
							mConnectors[k]->DetectionMode,
							(float)mConnectors[k]->AuxScaleFactor,
							mConnectors[k]->unitOfMeasurement
							); //Aux Scale Factor
					}
					else
					mConnectors[k]->Adapter->WriteXMLConfiguration(mXMLTextStream, k, startIndex, 0,
						mConnectors[k]->DetectionMode, mConnectors[k]->LowPassFilter, mConnectors[k]->HigPassFilter);
				}
			}

			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		String^ DataGeneratorQuattrocento::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			String^ Firmware;
			if (GetFirmwareVersion(Firmware) <= 0)
				Firmware = "Unknown";

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels()->Length) .ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			{
				mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			}
			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteAttributeString("ad_bits", "16");
			mXMLTextStream->WriteAttributeString("AnalogOutConn", mAnalogOutputConnector.ToString());
			mXMLTextStream->WriteAttributeString("AnalogOutCh", mAnalogOutputChannel.ToString());
			mXMLTextStream->WriteAttributeString("AnalogOutGain", mAnalogOutputGain.ToString());
			mXMLTextStream->WriteStartElement("Channels");

			unsigned int ChannelNum = 0;
			int startIndex = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{
				/*if (mModel==Models::Quattrocento_96)
				startIndex = k < 3 ? k * 16 : 96+k;
				else if (mModel == Models::Quattrocento_192)
				{
				startIndex = k < 4 ? k * 16 : 64 + k * 64;
				startIndex = k >= 6 ? k * 16 : 64 + k * 64;
				}*/
				//startIndex = k < GetNumberConnectorsIn() ? 16 * k : (k < (GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn()) ? GetNumberConnectorsIn() * 16 + (k-GetNumberConnectorsIn()) * 64 : (k- GetNumberConnectorsIn() - GetNumberConnectorsMultipleIn()) + GetNumberConnectorsIn() * 16 + GetNumberConnectorsMultipleIn() * 64);

				if (mConnectors[k]->Adapter != nullptr)
				{
					if (mConnectors[k]->Adapter->ID->Contains("Direct connection to Auxiliary Input"))
					{
						mConnectors[k]->Adapter->WriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							(float)0.5,
							(float)100000,
							(float)0,
							mConnectors[k]->DetectionMode);
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
					}
					else
					{
						mConnectors[k]->Adapter->WriteSavingXMLConfiguration(
							mXMLTextStream,
							k,
							startIndex,
							0,
							(float)150,
							(float)(LowPassFrequencies::Value(mConnectors[k]->LowPassFilter)),
							(float)(HighPassFrequencies::Value(mConnectors[k]->HigPassFilter)),
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

		DataGeneratorQuattrocento^ DataGeneratorQuattrocento::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorQuattrocento^ gdr = nullptr;
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
				unsigned int AOconn = 0;
				if (DeviceNode->Attributes["AnalogOutConn"] != nullptr)
					AOconn = System::Convert::ToUInt16(DeviceNode->Attributes["AnalogOutConn"]->Value);
				unsigned int AOch = 0;
				if (DeviceNode->Attributes["AnalogOutCh"] != nullptr)
					AOch = System::Convert::ToUInt16(DeviceNode->Attributes["AnalogOutCh"]->Value);
				unsigned int AOgain = 0;
				if (DeviceNode->Attributes["AnalogOutGain"] != nullptr)
					AOgain = System::Convert::ToUInt16(DeviceNode->Attributes["AnalogOutGain"]->Value);
				unsigned int deviceFrequency = System::Convert::ToUInt16(DeviceNode->Attributes["SampleFrequency"]->Value);
				if (deviceName->Contains("QUATTROCENTO"))
				{
					gdr = gcnew OTComm::Quattrocento::DataGeneratorQuattrocento(deviceModel, deviceFrequency);
					if (loadedipaddress != nullptr)
						gdr->SetIPAddress(loadedipaddress);
					gdr->SetAnalogOutputConnector(AOconn);
					gdr->SetAnalogOutputChannel(AOch);
					gdr->SetAnalogOutputGain(AOgain);
					//mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
					//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: duale della scrittura qui sotto <Source></Source>
					//{
					//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
					//}


					System::Xml::XmlNode^ ChannelsNode = DeviceNode->ChildNodes[0];
					for (int ii = 0; ii < ChannelsNode->ChildNodes->Count; ii++)
					{
						System::Xml::XmlNode^ AdapterNode = ChannelsNode->ChildNodes[ii];
						/*DebugLog::Print(AdapterNode->Attributes["ID"]->Value->ToString());
						DebugLog::Print(Assembly::GetExecutingAssembly()->ToString());*/
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

									}catch(Exception^ ex){
									}
								}
							}
						}
						else
							continue;
						if (ad->Channels->Length == 16)
							gdr->ConnectIn(adPosition, (AdapterIn^)ad);
						else if (ad->Channels->Length == 64)
							gdr->ConnectMultipleIn(adPosition - (unsigned int)gdr->GetNumberConnectorsIn(), (AdapterMultipleIn ^)ad);
						else if (adPosition - (unsigned int)gdr->GetNumberConnectorsIn() - (unsigned int)gdr->GetNumberConnectorsMultipleIn() < 16) // AUX
							gdr->ConnectAux(adPosition - (unsigned int)gdr->GetNumberConnectorsIn() - (unsigned int)gdr->GetNumberConnectorsMultipleIn(), (AdapterAux^)ad);
						else // control
							gdr->ConnectControl(adPosition - (unsigned int)gdr->GetNumberConnectorsIn() - (unsigned int)gdr->GetNumberConnectorsMultipleIn() - (unsigned int)gdr->GetNumberConnectorsAux(), (AdapterControl^)ad);

						if (ad->Channels->Length == 16 || ad->Channels->Length == 64)
						{
							gdr->GetConnectors()[adPosition]->DetectionMode = System::Convert::ToUInt16(AdapterNode->Attributes["DetectionMode"]->Value);
							gdr->GetConnectors()[adPosition]->LowPassFilter = System::Convert::ToUInt16(AdapterNode->Attributes["LowPassFilter"]->Value);
							gdr->GetConnectors()[adPosition]->HigPassFilter = System::Convert::ToUInt16(AdapterNode->Attributes["HighPassFilter"]->Value);
						}
						else if (ad->ID == "Direct connection to Auxiliary Input") {//Aux Scale Factor

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
#pragma endregion

#pragma region Connectors and adapters

		unsigned int DataGeneratorQuattrocento::RecognizeAdapterForCommand(String^ adapterID)
		{
			if (adapterID->Contains("AD1x16"))
				return 1;
			if (adapterID->Contains("AD2x8"))
				return 2;
			if (adapterID->Contains("AD4x4"))
				return 3;
			if (adapterID->Contains("AD1x64"))
				return 4;
			if (adapterID->Contains("AD64")) //SAME AS AD1x64
				return 4;
			if (adapterID->Contains("AD8x2"))
				return 5;
			return 0;
		}

		unsigned int DataGeneratorQuattrocento::RecognizeSensorForCommand(String^ channelID)
		{
			if (channelID->Contains("EEG"))
				return 1;
			if (channelID->Contains("iEMG"))
				return 2;
			if (channelID->Contains("CoDe"))
				return 3;
			if (channelID->Contains("Acceleromet"))
				return 4;
			if (channelID->Contains("DE1"))
				return 5;
			if (channelID->Contains("CDE"))
				return 6;
			if (channelID->Contains("ELSCH004"))
				return 8;
			if (channelID->Contains("ELSCH008"))
				return 9;
			if (channelID->Contains("SA8_10"))
				return 10;
			if (channelID->Contains("ELSCH064NM5"))
				return 11;
			if (channelID->Contains("ELSCH064R3S"))
				return 12;
			if (channelID->Contains("ELSCH064NM2"))
				return 12;
			if (channelID->Contains("ELSCH064NM3"))
				return 13;
			if (channelID->Contains("ELSCH064NM6"))
				return 13;
			if (channelID->Contains("ELSCH064NM7"))
				return 14;
			if (channelID->Contains("SA16_2_5"))
				return 15;
			if (channelID->Contains("SA16_5"))
				return 16;
			if (channelID->Contains("SA16_10"))
				return 17;
			if (channelID->Contains("ELSCH016"))
				return 18;
			return 0;
		}

		unsigned int DataGeneratorQuattrocento::RecognizeSideForCommand(String^ sideID)
		{
			if (sideID == "Left") return 64;
			if (sideID == "Right") return 128;
			return 0;
		}

		unsigned int DataGeneratorQuattrocento::RecognizeHPForCommand(unsigned int highpass)
		{
			unsigned int val = Convert::ToByte(highpass) << 4;
			return val;
		}

		unsigned int DataGeneratorQuattrocento::RecognizeLPForCommand(unsigned int lowpass)
		{
			unsigned int val = Convert::ToByte(lowpass) << 2;
			return val;
		}

		void DataGeneratorQuattrocento::SetAnalogOutputConnector(unsigned int analogOutputConnector)
		{
			mAnalogOutputConnector = analogOutputConnector;
		}

		unsigned int DataGeneratorQuattrocento::GetAnalogOutputConnector()
		{
			return mAnalogOutputConnector;
		}

		void DataGeneratorQuattrocento::SetAnalogOutputGain(unsigned int analogOutputGain)
		{
			mAnalogOutputGain = analogOutputGain;
		}

		unsigned int DataGeneratorQuattrocento::GetAnalogOutputGain()
		{
			return mAnalogOutputGain;
		}

		void DataGeneratorQuattrocento::SetAnalogOutputChannel(unsigned int analogOutputChannel)
		{
			mAnalogOutputChannel = analogOutputChannel;
		}

		unsigned int DataGeneratorQuattrocento::GetAnalogOutputChannel()
		{
			return mAnalogOutputChannel;
		}

		array<unsigned int>^ DataGeneratorQuattrocento::GetAcquiredChannels()
		{
			return mAcquiredChannelsIndices;
		}

		void DataGeneratorQuattrocento::ConnectAux(const unsigned int port, AdapterAux^ adapter)
		{
			if (port >= GetNumberConnectorsAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			mConnectors[GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn() + port]->Adapter = adapter;
		}

		void DataGeneratorQuattrocento::ConnectControl(const unsigned int port, AdapterControl^ adapter)
		{
			if (port >= GetNumberControlAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			mConnectors[GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn() + GetNumberConnectorsAux() + port]->Adapter = adapter;
		}

		void DataGeneratorQuattrocento::ConnectIn(const unsigned int port, AdapterIn^ adapter)
		{
			if (port >= GetNumberConnectorsIn())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			mConnectors[port]->Adapter = adapter;
		}

		void DataGeneratorQuattrocento::ConnectMultipleIn(const unsigned int port, AdapterMultipleIn^ adapter)
		{
			if (port >= GetNumberConnectorsMultipleIn())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			mConnectors[GetNumberConnectorsIn() + port]->Adapter = adapter;
		}

		array<unsigned int>^ DataGeneratorQuattrocento::GetChannelsMapping()
		{
			return DataGeneratorQuattrocento::CreateChannelsMapping();
		}

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

		GenericAdapter^ DataGeneratorQuattrocento::GetAdapterFromID(String^ adapterID)
		{
			std::string converted = toStandardString(adapterID);

			if (adapterID == ("AD16"))
				return gcnew OTComm::Quattrocento::AdapterInAD16();
			else if (adapterID == ("AD1x16"))
				return gcnew OTComm::Quattrocento::AdapterInAD1x16();
			else if (adapterID == ("AD1x16SD5"))
				return gcnew OTComm::Quattrocento::AdapterInAD1x16SD5();
			else 		if (adapterID == ("AD1x16SM5"))
				return gcnew OTComm::Quattrocento::AdapterInAD1x16SM5();
			else if (adapterID == ("AD1x16SDD5"))
				return gcnew OTComm::Quattrocento::AdapterInAD1x16SDD5();
			else if (adapterID == ("AD2x8"))
				return gcnew OTComm::Quattrocento::AdapterInAD2x8();
			else if (adapterID == ("AD4x4"))
				return gcnew OTComm::Quattrocento::AdapterInAD4x4();
			else if (adapterID == ("AD8x2JD"))
				return gcnew OTComm::Quattrocento::AdapterInAD8x2JD();
			else if (adapterID == ("AdapterInSplitter64"))
				return gcnew OTComm::Quattrocento::AdapterInSplitter64();
			else if (adapterID == ("AdapterInSplitter32"))
				return gcnew OTComm::Quattrocento::AdapterInSplitter32();
			/*if (adapterID == ("AdapterMultipleInRXERW3432"))
				return gcnew OTComm::Quattrocento::AdapterMultipleInRXERW3432();*/
			else if (adapterID == ("AD1x64"))
				return gcnew OTComm::Quattrocento::AdapterMultipleInAD1x64();
			else if (adapterID == ("AD64"))
				return gcnew OTComm::Quattrocento::AdapterMultipleInAD64();
			else if (adapterID == ("AD64S") || adapterID == ("AD64s"))
				return gcnew OTComm::Quattrocento::AdapterMultipleInAD64s();
			else if (adapterID == ("AD64F"))
				return gcnew OTComm::Quattrocento::AdapterMultipleInAD64F();
			else if (adapterID == ("SP-Box-QR"))
				return gcnew OTComm::Quattrocento::AdapterMultipleSplitter();
			else if (adapterID == ("SP-Box-32-QR"))
				return gcnew OTComm::Quattrocento::AdapterMultipleSplitter32();
			else if (adapterID == ("AdapterMultipleIn"))
				return gcnew OTComm::Quattrocento::AdapterMultipleIn();
			else if (StringContains("Direct connection to Auxiliary Input", converted))
				return gcnew OTComm::Quattrocento::AdapterAux();
			else if (adapterID == ("AdapterIn"))
				return gcnew OTComm::Quattrocento::AdapterIn();
			else if (adapterID == ("AdapterControl"))
				return gcnew OTComm::Quattrocento::AdapterControl();
			else 		if (adapterID == ("AD2x32"))
				return gcnew OTComm::Quattrocento::AdapterMultipleInAD2x32();

			return nullptr;
		}

		GenericChannel^ DataGeneratorQuattrocento::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
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
			if (channelID == ("ELSCH024TEST"))
				return gcnew OTComm::Sensors::SensorELSCH024TEST(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
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
			if (channelID == ("HD10MM0808"))
				return gcnew OTComm::Sensors::SensorHD10MM0808(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("HD04MM1305"))
				return gcnew OTComm::Sensors::SensorHD04MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("HD08MM1305"))
				return gcnew OTComm::Sensors::SensorHD08MM1305(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			return nullptr;
		}

		GenericChannel^ DataGeneratorQuattrocento::GetChannelFromString(String^ text)
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
			if (text->Contains("ELSCH064EEG"))
				return gcnew OTComm::Sensors::SensorELSCH064EEG();
			if (text->Contains("ELSCH020EEG"))
				return gcnew OTComm::Sensors::SensorELSCH020EEG();
			if (text->Contains("HD10MM0804"))
				return gcnew OTComm::Sensors::SensorHD10MM0804();
			if (text->Contains("HD10MM0808"))
				return gcnew OTComm::Sensors::SensorHD10MM0808();
			if (text->Contains("HD04MM1305"))
				return gcnew OTComm::Sensors::SensorHD04MM1305();
			if (text->Contains("HD08MM1305"))
				return gcnew OTComm::Sensors::SensorHD08MM1305();
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
			return nullptr;
		}

		unsigned int DataGeneratorQuattrocento::GetNumberTotalChannels()
		{
			return GetNumberChannelsNonAux() + GetNumberChannelsAux() + GetNumberControlAux();
		}

		unsigned int DataGeneratorQuattrocento::GetNumberChannelsNonAux()
		{
			return (((unsigned int)mModel) % 10000);
		}

		unsigned int DataGeneratorQuattrocento::GetNumberChannelsAux()
		{
			return 16;
		}

		unsigned int DataGeneratorQuattrocento::GetNumberControlAux()
		{
			return 8;
		}

		unsigned int DataGeneratorQuattrocento::GetNumberConnectorsAux()
		{
			return GetNumberChannelsAux();
		}

		unsigned int DataGeneratorQuattrocento::GetNumberConnectorsMultipleIn()
		{
			if (mModel == Models::Quattrocento_96)
				return 1;//TODO
			else if (mModel == Models::Quattrocento_192)
				return 2;
			else if (mModel == Models::Quattrocento_288)
				return 3;
			else if (mModel == Models::Quattrocento_384)
				return 4;
			return 0;
		}

		unsigned int DataGeneratorQuattrocento::GetNumberConnectorsIn()
		{
			if (mModel == Models::Quattrocento_96)
				return 2;
			else if (mModel == Models::Quattrocento_192)
				return 4;
			else if (mModel == Models::Quattrocento_288)
				return 6;
			else if (mModel == Models::Quattrocento_384)
				return 8;
			return 0;
		}

		array<unsigned int>^ DataGeneratorQuattrocento::CreateChannelsMapping()
		{
			return nullptr;
		}

		int DataGeneratorQuattrocento::PortNumber::get()
		{
			return mPortNumber;
		}
#pragma endregion

#pragma region Device properties 

		void DataGeneratorQuattrocento::SetIPAddress(String^ IPAddress)
		{
			mIPAdress = IPAddress;
		}

		void DataGeneratorQuattrocento::SetIPPort(unsigned int IPPort)
		{
			mPortNumber = IPPort;
		}

		unsigned int DataGeneratorQuattrocento::GetDeviceModel()
		{
			return (unsigned int)mModel;
		}

		Bitmap^ DataGeneratorQuattrocento::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("400_quattrocento.jpg", true);
		}

		String^ DataGeneratorQuattrocento::GetDeviceDescription(void)
		{
			String^ res = gcnew String("QUATTROCENTO");
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

		String^ DataGeneratorQuattrocento::IPAdress::get()
		{
			String^ tmp = gcnew String(mIPAdress);
			return tmp;
		}

#pragma endregion

#pragma region Public device operations
		void DataGeneratorQuattrocento::OpenDevice(void)
		{

			LOG_ENTERING;
			WSADATA wsaData = { 0 };
			if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			{
				WSAException^ wsae = gcnew WSAException(__FUNCTION__, "WSAStartup");
				FireOnError(wsae);
				throw wsae;
			}
			if ((mDeviceHandle = (HANDLE)WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED)) == (HANDLE)INVALID_SOCKET)
			{
				WSAException^ wsae = gcnew WSAException(__FUNCTION__, "WSASocket");
				FireOnError(wsae);
				throw wsae;
			}
			else
			{
				struct sockaddr_in address;  /* the libc network address data structure */

				IntPtr ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(mIPAdress);
				address.sin_addr.s_addr = inet_addr((LPCSTR)ptrtxt.ToPointer()); /* assign the address */
			//https://social.msdn.microsoft.com/Forums/vstudio/en-US/185c9147-4866-4831-a3b7-7370b6f9faec/inetaddr-use-inetpton-or-inetpton-instead-or-define-winsockdeprecatednowarnings-to?forum=vcgeneral
			//#include <WS2tcpip.h> 
			//inet_pton(AF_INET, (LPCSTR)ptrtxt.ToPointer(), &(address.sin_addr.s_addr));



				System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
				address.sin_port = htons(mPortNumber);            /* translate int2port num */
				address.sin_family = AF_INET;

				if (WSAConnect((SOCKET)mDeviceHandle, (SOCKADDR*)(&address), sizeof(address), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
				{
					WSAException^ wsae = gcnew WSAException(__FUNCTION__, "WSAConnect");
					FireOnError(wsae);
					throw wsae;
				}
				else
				{
					InternalStopQuattrocentoDataTransfer();
					mDeviceIsOpen = true;
					DataGeneratorInterface::OpenDevice();
					LOG_SUCCESS;
				}
			}
			LOG_EXITING;
		}

		void DataGeneratorQuattrocento::InitializeDevice()
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

		void DataGeneratorQuattrocento::StartAcquisition(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			{
				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
				FireOnError(ihe);
				throw ihe;
			}
			else
			{
				DataGeneratorInterface::StartAcquisition();
				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorQuattrocento::OuternThread));
				mManagedAcquisitionThread->Start();
			}
			LOG_EXITING;
		}

		void DataGeneratorQuattrocento::StopAcquisition(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				DataGeneratorInterface::StopAcquisition();
				InternalStopQuattrocentoDataTransfer();
			}
			LOG_EXITING;
		}

		void DataGeneratorQuattrocento::CloseDevice(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				InternalStopQuattrocentoDataTransfer();
				closesocket((SOCKET)mDeviceHandle);
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
				mDeviceIsOpen = false;
				LOG_SUCCESS;
			}
			else
			{
				LOG_ERROR(" already closed");
			}
			DataGeneratorInterface::CloseDevice();
			LOG_EXITING;
		}

		void DataGeneratorQuattrocento::RaiseTrigger(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				mMustRaiseTrigger = true;
			}
			LOG_EXITING;
		}

		void DataGeneratorQuattrocento::FallTrigger(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				mMustFallTrigger = true;
			}
			LOG_EXITING;
		}


#pragma endregion

#pragma region Non public device operations


		int DataGeneratorQuattrocento::ReadDeviceID(UCHAR* dev_id)
		{
			if (mDeviceHandle == INVALID_HANDLE_VALUE)
				return -5;
			return 0;
		}

		int DataGeneratorQuattrocento::GetFirmwareVersion(String^% version)
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

		array<Byte>^ DataGeneratorQuattrocento::CreateRaiseTriggerCommand()
		{
			return CreateStartStopCommands(true, true);
		}

		array<Byte>^ DataGeneratorQuattrocento::CreateFallTriggerCommand()
		{
			return CreateStartStopCommands(true, false);
		}

		array<Byte>^ DataGeneratorQuattrocento::CreateStartCommand()
		{
			return CreateStartStopCommands(true, false);
		}

		array<Byte>^ DataGeneratorQuattrocento::CreateStopCommand()
		{
			return CreateStartStopCommands(false, false);
		}

		array<Byte>^ DataGeneratorQuattrocento::CreateStartStopCommands(bool Start, bool TriggerOn)
		{
			array<Byte>^ command = gcnew array<Byte>(41);
			int frequenza = 0;
			if (mSources[0]->mSamplingFrequency == 10240)
				frequenza = 24;
			else if (mSources[0]->mSamplingFrequency == 5120)
				frequenza = 16;
			else if (mSources[0]->mSamplingFrequency == 2048)
				frequenza = 8;
			int modello = 0;
			if (mModel % 10000 > 300)
				modello = 6;
			else if (mModel % 10000 > 200)
				modello = 4;
			else if (mModel % 10000 > 100)
				modello = 2;
			command[0] = (Byte)(128 + frequenza + modello + (Start ? 1 : 0) + (TriggerOn ? 32 : 0));
			command[1] = (Byte)(mAnalogOutputConnector + mAnalogOutputGain * 16);
			command[2] = (Byte)(mAnalogOutputChannel);
			unsigned int NumberConnectorsIn = GetNumberConnectorsIn();
			for (unsigned int ii = 0; ii < NumberConnectorsIn; ii++)
			{
				GenericConnector ^Connector = mConnectors[ii];
				unsigned int posStart = 3 + ii * 3;
				if (Connector != nullptr && Connector->Adapter != nullptr && Connector->Adapter->Channels[0] != nullptr)
				{
					command[posStart + 0] = (Byte)Connector->Adapter->Channels[0]->MuscleID;
					command[posStart + 1] = (Byte)(
						(RecognizeSensorForCommand(Connector->Adapter->Channels[0]->ChannelID) * 8) +
						RecognizeAdapterForCommand(Connector->Adapter->ID));
					command[posStart + 2] = (Byte)(
						RecognizeSideForCommand(Connector->Adapter->Channels[0]->MuscleSideDescription) +
						RecognizeHPForCommand(Connector->HigPassFilter) +
						RecognizeLPForCommand(Connector->LowPassFilter) +
						(Connector->DetectionMode));
				}
			}
			for (unsigned int ii = 0; ii < GetNumberConnectorsMultipleIn(); ii++)
			{
				GenericConnector ^Connector = mConnectors[ii + NumberConnectorsIn];
				if (Connector != nullptr && Connector->Adapter != nullptr && Connector->Adapter->Channels[0] != nullptr)
				{
					unsigned int posStart = 3 + 3 * 8 + ii * 3;
					command[posStart + 0] = (Byte)Connector->Adapter->Channels[0]->MuscleID;
					command[posStart + 1] = (Byte)((RecognizeSensorForCommand(Connector->Adapter->Channels[0]->ChannelID) * 8) +
						RecognizeAdapterForCommand(Connector->Adapter->ID));
					command[posStart + 2] = (Byte)(
						RecognizeSideForCommand(Connector->Adapter->Channels[0]->MuscleSideDescription) +
						RecognizeHPForCommand(Connector->HigPassFilter) +
						RecognizeLPForCommand(Connector->LowPassFilter) +
						(Connector->DetectionMode));
				}
			}
			command[39] = ComputeCRC(command, 39);
			command[40] = 0;
			return command;
		}

		Byte DataGeneratorQuattrocento::ComputeCRC(array<Byte>^ command, unsigned int Len)
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

		void DataGeneratorQuattrocento::OuternThread()
		{
			try
			{
				DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				//mDataReaderInterfaceHelper->mAuxData = mUseAux;
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				pin_ptr<int> MustRaiseTriggerPinned = &mMustRaiseTrigger; // pin pointer 
				mDataReaderInterfaceHelper->mMustRaiseTriggerPtr = MustRaiseTriggerPinned;
				pin_ptr<int> MustFallTriggerPinned = &mMustFallTrigger; // pin pointer 
				mDataReaderInterfaceHelper->mMustFallTriggerPtr = MustFallTriggerPinned;

				//genero stringa con i comandi

				array<Byte>^ command = CreateStartCommand();
				pin_ptr<Byte> CommandPinned = &command[0];   // entire array is now pinned  
				mDataReaderInterfaceHelper->mCustomData = (LPARAM)CommandPinned;
				array<Byte>^ raisetriggercommand = CreateRaiseTriggerCommand();
				pin_ptr<Byte> RaiseTriggerCommandPinned = &raisetriggercommand[0];   // entire array is now pinned  
				mDataReaderInterfaceHelper->mRaiseTriggerCommand = (LPARAM)RaiseTriggerCommandPinned;
				array<Byte>^ falltriggercommand = CreateFallTriggerCommand();
				pin_ptr<Byte> FallTriggerCommandPinned = &falltriggercommand[0];   // entire array is now pinned  
				mDataReaderInterfaceHelper->mFallTriggerCommand = (LPARAM)FallTriggerCommandPinned;


				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoQuattrocento,  // Thread Func
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
			}
			catch (Exception ^e)
			{
				AcquisitionThreadException^ ate = gcnew AcquisitionThreadException(__FUNCTION__, e);
				FireOnError(ate);
			}
			LOG_EXITING;
		}

		void DataGeneratorQuattrocento::InternalStopQuattrocentoDataTransfer(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				// Stop acquisition
				try
				{
					//WriteString("stop");
					LOG_ENTERING; \
						if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
						{
							InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
							FireOnError(ihe);
							throw ihe;
						}
					DWORD res = 0;
					array<Byte>^ command = CreateStopCommand();
					pin_ptr<Byte> CommandPinned = &command[0];   // entire array is now pinned  
					//IntPtr ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(command);
					//const char* buf = (LPCSTR)command[0].ToPointer();
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
					BOOL WriteFileRes = WriteFile(mDeviceHandle, (LPCVOID)&CommandPinned[0], 40, &numberOfBytesTransferred, OverlappedStruc);
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
					return;
				}
				catch (Exception ^e)
				{
					CloseConnection();
					StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__, "", e);
					FireOnError(safe);
					throw safe;
				}
			}
			LOG_EXITING;
		}

		void DataGeneratorQuattrocento::CloseConnection()
		{
			DebugLog::Print(String::Format("Connection closed"));
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				closesocket((SOCKET)mDeviceHandle);
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
			}
		}

		int DataGeneratorQuattrocento::WriteString(String^ command)
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

		int DataGeneratorQuattrocento::ReadString(String^ %read)
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





					/*					if (!GetOverlappedResultEx(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, 1000, true))
					{
					FireOnError("problem reading", GetLastError(), __FUNCTION__);
					LOG_ERROR("problem reading");
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					return -1;
					}
					*/
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
		DataValidator^ DataGeneratorQuattrocento::DefaultTriggerValidator::get()
		{
			return mDefaulDataValidatorTrigger;
		}
#pragma endregion 

#pragma region Unmanaged code

#pragma unmanaged

		ULONG _stdcall AsyncIoQuattrocento(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ASYNCH_IO = 10;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			unsigned int NumBytesPerSample = sizeof(__int16);
			unsigned char * StartCommand = (unsigned char *)helper->mCustomData;
			unsigned char * RaiseTriggerCommand = (unsigned char *)helper->mRaiseTriggerCommand;
			unsigned char * FallTriggerCommand = (unsigned char *)helper->mFallTriggerCommand;

			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;
			int *mMustRaiseTrigger = helper->mMustRaiseTriggerPtr;
			int *mMustFallTrigger = helper->mMustFallTriggerPtr;

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
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize * NumBytesPerSample;
			unsigned short *DecodedBuffer;
			unsigned short *DecodedBufferPos;
			unsigned short *DecodedBufferCurrentStart;

			ULONG DecodedBufferSizeUnitaryTrigger = 2 * helper->mSource[0].mDataRegroupFactor;// added quality control
			ULONG DecodedBufferSizeTrigger = DecodedBufferSizeUnitaryTrigger * NUM_DECODED_BUFFERS;
			ULONG DecodedBufferSizeInBytesTrigger = DecodedBufferSizeTrigger * NumBytesPerSample;
			unsigned short *DecodedBufferTrigger;
			unsigned short *DecodedBufferPosTrigger;
			unsigned short *DecodedBufferCurrentStartTrigger;
			unsigned int channelTrigger = helper->mDeviceTotalChannels - 7; // il canale del trigger è sempre il secondo controllo, quindi totale meno 7
			unsigned int channelQuality = helper->mDeviceTotalChannels - 6; // il canale del controllo qualità è sempre il terzo controllo, quindi totale meno 6

			ULONG numberOfBytesTransferred;

			OVERLAPPED *OverlappedStructWrite;
			OverlappedStructWrite = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			if (OverlappedStructWrite == NULL)
			{
				return -1;
			}
			int res = 0;

			HANDLE hCompletionPort = CreateIoCompletionPort(helper->mDeviceHandle, NULL, 1, 0);
			if (hCompletionPort == NULL)
			{
				return -1;
			}
			OVERLAPPED *OverlappedStruc = (OVERLAPPED *)LocalAlloc(LMEM_FIXED | LMEM_ZEROINIT, NUM_ASYNCH_IO * sizeof(OVERLAPPED));
			if (OverlappedStruc == NULL)
			{
				CloseHandle(hCompletionPort);
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
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				return -1;
			}

			//temporary buffer
			TempBuffer = (UCHAR*)LocalAlloc(LPTR, TempBufferSizeInBytes);
			if (TempBuffer == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				return -1;
			}
			TempBufferPos = TempBuffer;

			//decoded buffer
			DecodedBuffer = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
			if (DecodedBuffer == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				LocalFree((void *)TempBuffer);
				TempBuffer = NULL;
				return -1;
			}
			DecodedBufferPos = DecodedBuffer;
			DecodedBufferCurrentStart = DecodedBufferPos;

			//decoded buffer for trigger
			DecodedBufferTrigger = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeInBytesTrigger);
			if (DecodedBufferTrigger == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				LocalFree((void *)TempBuffer);
				TempBuffer = NULL;
				LocalFree((void *)DecodedBuffer);
				DecodedBuffer = NULL;
				return -1;
			}
			DecodedBufferPosTrigger = DecodedBufferTrigger;
			DecodedBufferCurrentStartTrigger = DecodedBufferPosTrigger;

			OverlappedStructWrite = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			if (OverlappedStructWrite == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				LocalFree((void *)TempBuffer);
				TempBuffer = NULL;
				LocalFree((void *)DecodedBuffer);
				DecodedBuffer = NULL;
				LocalFree((void *)DecodedBufferTrigger);
				DecodedBufferTrigger = NULL;

				return -1;
			}
			ZeroMemory(OverlappedStructWrite, sizeof(OVERLAPPED));

			SetLastError(0);
			BOOL WriteFileRes = WriteFile(helper->mDeviceHandle, StartCommand, 40, &numberOfBytesTransferred, OverlappedStructWrite);
			res = 0;
			if (WriteFileRes == FALSE)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					DWORD dwResult = WaitForSingleObject(OverlappedStructWrite->hEvent, 1000);
					if (dwResult != WAIT_OBJECT_0 ||
						!::GetOverlappedResult(helper->mDeviceHandle, OverlappedStructWrite, &numberOfBytesTransferred, TRUE)
						)
					{
						res = -1;
					}
				}
				else
					Sleep(10);

				res = -1;
			}
			else
				Sleep(10);
			LocalFree((void *)OverlappedStructWrite);
			OverlappedStructWrite = NULL;
			if (res != 0)
			{
				return -1;
			}
			//points to the beginning of the decoded buffer
			//DecodedBufferCurrentStart = DecodedBuffer;
			//DecodedBufferPos = DecodedBuffer; //PB REMOVE IF PROBLEMS OCCURS!!!!!!!!!!!!!! 

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;

			unsigned int nLettura = 0;

			for (int i = 0; i < NUM_ASYNCH_IO; i++)
			{
				BOOL ReadFileRes = ReadFile(helper->mDeviceHandle,
					AcquisitionBuffer + (i * SingleAcquisitionBufferSizeInBytes),
					SingleAcquisitionBufferSizeInBytes, &numberOfBytesTransferred, &OverlappedStruc[i]);

				if (ReadFileRes == FALSE)
				{
					if (GetLastError() != ERROR_IO_PENDING)
					{
						CloseHandle(hCompletionPort);
						hCompletionPort = NULL;
						LocalFree((void *)OverlappedStruc);
						OverlappedStruc = NULL;
						LocalFree((void *)AcquisitionBuffer);
						AcquisitionBuffer = NULL;
						LocalFree((void *)TempBuffer);
						TempBuffer = NULL;
						return -1;
					}
				}
			}

			WHILE(*mMustStopAcquisitionThread == false)
			{
				// insert here the control about trigger commands
				if (*mMustRaiseTrigger == true)
				{
					// send the command to raise the trigger out
					OverlappedStructWrite = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
					if (OverlappedStructWrite == NULL)
					{
						CloseHandle(hCompletionPort);
						hCompletionPort = NULL;
						LocalFree((void *)OverlappedStruc);
						OverlappedStruc = NULL;
						LocalFree((void *)AcquisitionBuffer);
						AcquisitionBuffer = NULL;
						LocalFree((void *)TempBuffer);
						TempBuffer = NULL;
						LocalFree((void *)DecodedBuffer);
						DecodedBuffer = NULL;
						LocalFree((void *)DecodedBufferTrigger);
						DecodedBufferTrigger = NULL;

						return -1;
					}
					ZeroMemory(OverlappedStructWrite, sizeof(OVERLAPPED));

					SetLastError(0);
					BOOL WriteFileRes = WriteFile(helper->mDeviceHandle, RaiseTriggerCommand, 40, &numberOfBytesTransferred, OverlappedStructWrite);
					res = 0;
					if (WriteFileRes == FALSE)
					{
						if (GetLastError() == ERROR_IO_PENDING)
						{
							DWORD dwResult = WaitForSingleObject(OverlappedStructWrite->hEvent, 1000);
							if (dwResult != WAIT_OBJECT_0 ||
								!::GetOverlappedResult(helper->mDeviceHandle, OverlappedStructWrite, &numberOfBytesTransferred, TRUE)
								)
							{
								res = -1;
							}
						}
						else
							Sleep(10);

						res = -1;
					}
					else
						Sleep(10);
					LocalFree((void *)OverlappedStructWrite);
					OverlappedStructWrite = NULL;
					// remove the must raise trigger
					*mMustRaiseTrigger = false;
				}
				if (*mMustFallTrigger == true)
				{
					// send the command to fall the trigger out
					OverlappedStructWrite = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
					if (OverlappedStructWrite == NULL)
					{
						CloseHandle(hCompletionPort);
						hCompletionPort = NULL;
						LocalFree((void *)OverlappedStruc);
						OverlappedStruc = NULL;
						LocalFree((void *)AcquisitionBuffer);
						AcquisitionBuffer = NULL;
						LocalFree((void *)TempBuffer);
						TempBuffer = NULL;
						LocalFree((void *)DecodedBuffer);
						DecodedBuffer = NULL;
						LocalFree((void *)DecodedBufferTrigger);
						DecodedBufferTrigger = NULL;

						return -1;
					}
					ZeroMemory(OverlappedStructWrite, sizeof(OVERLAPPED));

					SetLastError(0);
					BOOL WriteFileRes = WriteFile(helper->mDeviceHandle, FallTriggerCommand, 40, &numberOfBytesTransferred, OverlappedStructWrite);
					res = 0;
					if (WriteFileRes == FALSE)
					{
						if (GetLastError() == ERROR_IO_PENDING)
						{
							DWORD dwResult = WaitForSingleObject(OverlappedStructWrite->hEvent, 1000);
							if (dwResult != WAIT_OBJECT_0 ||
								!::GetOverlappedResult(helper->mDeviceHandle, OverlappedStructWrite, &numberOfBytesTransferred, TRUE)
								)
							{
								res = -1;
							}
						}
						else
							Sleep(10);

						res = -1;
					}
					else
						Sleep(10);
					LocalFree((void *)OverlappedStructWrite);
					OverlappedStructWrite = NULL;
					// remove the must fall trigger
					*mMustFallTrigger = false;
				}






				SetLastError(0);
				OVERLAPPED *completedOv;
				ULONG_PTR   key;

				if (GetQueuedCompletionStatus(hCompletionPort, &numberOfBytesTransferred,
					&key, &completedOv, INFINITE) == 0)
				{
					helper->mError("", GetLastError(), __FUNCTION__);
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
						DecodedBufferPos = DecodedBufferCurrentStart;
						DecodedBufferPosTrigger = DecodedBufferCurrentStartTrigger;
						unsigned short *TempBufferAsShort = (unsigned short*)TempBuffer;
						for (unsigned int koffset = 0; koffset < helper->mSource[0].mDataRegroupFactor; koffset++)
						{
							for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
							{
								//add control for buffer overflow
								*DecodedBufferPos = TempBufferAsShort[ChannelMappings[channel]];
								DecodedBufferPos++;
							}
							//trigger
							*DecodedBufferPosTrigger = TempBufferAsShort[channelTrigger];
							DecodedBufferPosTrigger++;
							// quality controller
							*DecodedBufferPosTrigger = TempBufferAsShort[channelQuality];
							DecodedBufferPosTrigger++;

							TempBufferAsShort += helper->mDeviceTotalChannels;
						}
						helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, DecodedBufferCurrentStartTrigger, helper->mSource[0].mAcquiredChannels, 2, helper->mSource[0].mDataRegroupFactor, 0);

						nLettura++;


						DecodedBufferCurrentStart = DecodedBufferPos;
						//if (DecodedBufferCurrentStart - DecodedBuffer >= DecodedBufferSize)
						if (DecodedBufferCurrentStart >= DecodedBufferSize + DecodedBuffer)
							DecodedBufferCurrentStart = DecodedBuffer;

						DecodedBufferCurrentStartTrigger = DecodedBufferPosTrigger;
						//if (DecodedBufferCurrentStart - DecodedBuffer >= DecodedBufferSize)
						if (DecodedBufferCurrentStartTrigger >= DecodedBufferSizeTrigger + DecodedBufferTrigger)
							DecodedBufferCurrentStartTrigger = DecodedBufferTrigger;

						//copy the possibly remaining acquired data
						memcpy(TempBuffer, AcquisitionBufferCurrentReadStart + FreeTempBufferBytes, numberOfBytesTransferred - FreeTempBufferBytes);
						TempBufferPos = TempBuffer + numberOfBytesTransferred - FreeTempBufferBytes;
					}

					if (*mMustStopAcquisitionThread) break;
					SetLastError(0);
					BOOL ReadFileRes = ReadFile(helper->mDeviceHandle,
						AcquisitionBuffer + (ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes),
						SingleAcquisitionBufferSizeInBytes, NULL, completedOv);

					if (ReadFileRes == FALSE)
					{
						if (GetLastError() != ERROR_IO_PENDING)
						{
						}
					}
				}

			}//while

			CancelIo(helper->mDeviceHandle);

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
			if (DecodedBufferTrigger)
			{
				LocalFree((void *)DecodedBufferTrigger);
				DecodedBufferTrigger = NULL;
			}



			helper->mStopAcquisition();
			return 0;
		}
#pragma endregion
	}

}