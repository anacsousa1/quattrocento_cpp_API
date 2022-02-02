#include "DataGeneratorMuoviPiu.h"
#include "stdio.h"
#include <string>
#include <winsock.h>

using namespace System::Xml;

namespace OTComm
{
	namespace MuoviPiu
	{
#pragma managed

#pragma comment(lib, "Ws2_32.lib")

#pragma region Constructors and destructors
		DataGeneratorMuoviPiu::DataGeneratorMuoviPiu(Model model, Frequency SamplingFrequency, bool HighRes, bool RemoveAverage) :
			DataGeneratorInterface(), mModel(model),
			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false),
			mDataReaderInterfaceHelper(NULL), mPortNumber(mDefaultPortNumber)
		{
			try
			{
				mIPAdress = String::Copy(mDefaultIPAdress);
				unsigned int k = 0;
				//int nProbe = (mModel % 10000) / 32;
				int totalNumberOfProbe = 1;
				int maxChannelsPerProbe = 64;
				int numberOfAccessoryConnector=2;
				int numberOfQuaternion = 1;

				mDeviceTotalChannels = (maxChannelsPerProbe+numberOfAccessoryConnector+numberOfQuaternion*4)*totalNumberOfProbe;
				mConnectors = gcnew array<GenericConnector^>(totalNumberOfProbe * (GetNumberConnectorsIn() + GetNumberConnectorsAux() + GetNumberQuaternion()));
				remove_average = RemoveAverage;
				int connectedprobe = model % 10000;
				

				mSources = gcnew array<DataGeneratorSingleSource^>(totalNumberOfProbe);
 				for (int j = 0; j < totalNumberOfProbe; j++) {
					mSources[j] = gcnew DataGeneratorSingleSource(SamplingFrequency, HighRes ? 3 : 2, GetChannelsMapping());
				}

				int offset = 0;
				for (; k < totalNumberOfProbe; k++) { //Max four probes
					mConnectors[k + offset + 0] = gcnew GenericConnector(maxChannelsPerProbe, k);//mModel % 10000); //ottengo 32, 64,96 o 128 che indicano i canali
					mConnectors[k + offset + 1] = gcnew GenericConnector(4, k);
					mConnectors[k + offset + 2] = gcnew GenericConnector(1, k);
					mConnectors[k + offset + 3] = gcnew GenericConnector(1, k);

					offset += 3;
				}
				/*
				int sourceIndex = 0;
				for (k= totalNumberOfProbe; k < totalNumberOfProbe+numberOfAccessory*totalNumberOfProbe; k++) { //Max four probes
					if (k >= totalNumberOfProbe+6*(sourceIndex+1)) sourceIndex++;
					mConnectors[k] = gcnew GenericConnector(1, sourceIndex);//ottengo 32, 64,96 o 128 che indicano i canali
				}*/

				mDeviceHandle = (HANDLE)INVALID_SOCKET;
				mDefaulDataValidatorTrigger = nullptr;
			}
			catch (Exception ^e)
			{
				ConstructorFailedException^ cfe = gcnew ConstructorFailedException(__FUNCTION__, e);
				throw cfe;
			}
			
		}

		DataGeneratorMuoviPiu::~DataGeneratorMuoviPiu()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorMuoviPiu();
		}

		DataGeneratorMuoviPiu::!DataGeneratorMuoviPiu()
		{

		}

#pragma endregion

#pragma region XML files

		String^ DataGeneratorMuoviPiu::GetDeviceXMLConfiguration()
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
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", GetNumberTotalChannels().ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("IPAddress", mIPAdress);
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[0]->mSamplingFrequency.ToString());			
			mXMLTextStream->WriteAttributeString("Resolution", mSources[0]->mBytesPerChannel.ToString());
			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteAttributeString("RemoveAverage", remove_average.ToString());
			mXMLTextStream->WriteStartElement("Channels");
			for (int ii = 0; ii < mConnectors->Length; ii++)
			{
				if (mConnectors[ii]->Adapter != nullptr)
					mConnectors[ii]->Adapter->WriteXMLConfiguration(mXMLTextStream, ii, ii + 1, 0,
						mConnectors[ii]->DetectionMode, mConnectors[ii]->LowPassFilter, mConnectors[ii]->HigPassFilter);
			}


			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		String^ DataGeneratorMuoviPiu::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			String^ Firmware;
			if (GetFirmwareVersion(Firmware) <= 0)
				Firmware = "Unknown";

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[0]->mSamplingFrequency.ToString());			
			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteAttributeString("RemoveAverage", remove_average.ToString());
			mXMLTextStream->WriteAttributeString("ad_bits", mSources[0]->mBytesPerChannel == 2 ? "16" : "24");
			mXMLTextStream->WriteStartElement("Channels");
			for (int ii = 0; ii < mConnectors->Length; ii++)
			{
				if (mConnectors[ii]->Adapter != nullptr)
					mConnectors[ii]->Adapter->WriteXMLConfiguration(mXMLTextStream, ii, ii+1, 0,
						mConnectors[ii]->DetectionMode, mConnectors[ii]->LowPassFilter, mConnectors[ii]->HigPassFilter);
			}

			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		DataGeneratorMuoviPiu^ DataGeneratorMuoviPiu::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorMuoviPiu^ gdr = nullptr;
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
				String^ TextRemoveAverage = DeviceNode->Attributes["RemoveAverage"]->Value;
				bool removeAverage = (TextRemoveAverage == "True");

				int controlIndex = 4;
				if (deviceName->Contains("MUOVI+"))
				{

					gdr = gcnew OTComm::MuoviPiu::DataGeneratorMuoviPiu(deviceModel, deviceFrequency,
						resolution == 3, removeAverage);
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
							//CREA I CANALI DELL'ADATTATORE IN BASE AL NUMERO DI CANALI DEL SENSORE
							for (int jj = 0; jj < AdapterNode->ChildNodes->Count; jj++)
								{
									System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[jj];
									GenericChannel^ ch = GetChannelFromID(
										ChannelNode->Attributes["ID"]->Value->ToString(),
										ChannelNode->Attributes["Muscle"]->Value->ToString(),
										ChannelNode->Attributes["Side"]->Value->ToString());
									if (ch != nullptr) {
										ad->SetChannel(System::Convert::ToUInt16(ChannelNode->Attributes["Index"]->Value->ToString()), ch);
										if (ch->ChannelID->Contains("064EEG")) {
											ad->Channels[jj]->Description = GetEEGLabelFor64EEGFromIndex(jj);
										}
									}
								}

							if (AdapterNode->ChildNodes->Count != 64) {
								
								//INSERIMENTO CUFFIE EEG DA SETUP	
								//IN QUESTO CASO DEVO AGGIUNGERE 64 CANALI ANZICHè 20, DI MODO TALE DA CREARE COMUNQUE PIU TRACCE DEL DOVUTO.
								//IL FORM REALTIME PLOTTER SI OCCUPERA DI TOGLIERE LE TRACCE CON "MustBeDeleted" da quelle visualizzate ma il numero corretto
								//contribuira a una corretta distribuzione dei valore nel NewDataAvailable
								if (AdapterNode->ChildNodes[0]->Attributes["ID"]->Value->ToString()->Contains("020EEG")) {

									String^ ID = AdapterNode->ChildNodes[0]->Attributes["ID"]->Value->ToString();
									String^ Muscle = AdapterNode->ChildNodes[0]->Attributes["Muscle"]->Value->ToString();
									String^ Side = AdapterNode->ChildNodes[0]->Attributes["Side"]->Value->ToString();
									String^ Prefix = AdapterNode->ChildNodes[0]->Attributes["Prefix"]->Value->ToString();

									for (int jj = 0; jj < 64; jj++)
									{
										System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[jj];
										//se imposto la cuffia 20 canali al 21 esimo troverò il ChannelNode nullo
										if (ChannelNode != nullptr) {
											GenericChannel^ ch = GetChannelFromID(
												ChannelNode->Attributes["ID"]->Value->ToString(),
												ChannelNode->Attributes["Muscle"]->Value->ToString(),
												ChannelNode->Attributes["Side"]->Value->ToString());
											if (ch != nullptr)
											{
												ad->SetChannel(System::Convert::ToUInt16(jj.ToString()), ch);
												try
												{
													ad->Channels[jj]->ChannelPrefix = ChannelNode->Attributes["Prefix"]->Value->ToString();
													ad->Channels[jj]->Description = GetEEGLabelFor20EEGFromIndex(jj);
												}
												catch (Exception^ ex) {
												}
											}
										}
										else {
											GenericChannel^ ch = GetChannelFromID(ID, Muscle, Side);
											ad->SetChannel(System::Convert::ToUInt16(jj.ToString()), ch);
											try
											{
												ad->Channels[jj]->ChannelPrefix = Prefix;
											}
											catch (Exception^ ex) {
											}
										}
									}

									int emptyChannelIndex = AdapterNode->ChildNodes->Count;
									for (int jj = 20; jj < 64; jj++)
									{
										System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[0];
										GenericChannel^ ch = GetChannelFromID(
											ChannelNode->Attributes["ID"]->Value->ToString(),
											ChannelNode->Attributes["Muscle"]->Value->ToString(),
											ChannelNode->Attributes["Side"]->Value->ToString());
										if (ch != nullptr)
										{
											ad->SetChannel(System::Convert::ToUInt16(emptyChannelIndex.ToString()), ch);
											try {
												ad->Channels[emptyChannelIndex]->ChannelPrefix = "MustBeDeleted";
												emptyChannelIndex++;
											}
											catch (Exception^ ex) {
											}
										}
									}
								}
								



							}
						}
						else
							continue;
						if (ad->Description->Contains("MuoviPiu")) { 
							gdr->ConnectIn(adPosition, (AdapterMuoviPiu^)ad); 
						}
						else if (ad->Description->Contains("Control")) {
							//gdr->ConnectControl(adPosition, (AdapterControl^)ad);
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
		DataGeneratorMuoviPiu^ DataGeneratorMuoviPiu::FromXMLConfigurationForSetup(String^ Filename)
		{
			DataGeneratorMuoviPiu^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try {
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				unsigned int deviceModel = System::Convert::ToUInt16(DeviceNode->Attributes["Model"]->Value);
				String^ loadedipaddress = nullptr;
				if (DeviceNode->Attributes["IPAddress"] != nullptr)
					loadedipaddress = DeviceNode->Attributes["IPAddress"]->Value;
				unsigned int deviceFrequency = System::Convert::ToUInt16(DeviceNode->Attributes["SampleFrequency"]->Value);
				int resolution = System::Convert::ToInt16(DeviceNode->Attributes["Resolution"]->Value);
				String^ TextRemoveAverage = DeviceNode->Attributes["RemoveAverage"]->Value;
				bool removeAverage = (TextRemoveAverage == "True");

				int controlIndex = 4;
				if (deviceName->Contains("MUOVI+"))
				{

					gdr = gcnew OTComm::MuoviPiu::DataGeneratorMuoviPiu(deviceModel, deviceFrequency,
						resolution == 3, removeAverage);
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
							//CREA I CANALI DELL'ADATTATORE IN BASE AL NUMERO DI CANALI DEL SENSORE
							for (int jj = 0; jj < AdapterNode->ChildNodes->Count; jj++)
							{
								System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[jj];
								GenericChannel^ ch = GetChannelFromID(
									ChannelNode->Attributes["ID"]->Value->ToString(),
									ChannelNode->Attributes["Muscle"]->Value->ToString(),
									ChannelNode->Attributes["Side"]->Value->ToString());
								if (ch != nullptr) {
									ad->SetChannel(System::Convert::ToUInt16(ChannelNode->Attributes["Index"]->Value->ToString()), ch);
									if (AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("64") &&
										AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("EEG")) {
										ad->Channels[jj]->Description = GetEEGLabelFor64EEGFromIndex(jj);
									}
								}
							}

							if (AdapterNode->ChildNodes->Count != 64) {

								//INSERIMENTO CUFFIE EEG DA SETUP	
								//IN QUESTO CASO DEVO AGGIUNGERE 64 CANALI ANZICHè 20, DI MODO TALE DA CREARE COMUNQUE PIU TRACCE DEL DOVUTO.
								//IL FORM REALTIME PLOTTER SI OCCUPERA DI TOGLIERE LE TRACCE CON "MustBeDeleted" da quelle visualizzate ma il numero corretto
								//contribuira a una corretta distribuzione dei valore nel NewDataAvailable
								if (AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("20") &&
									AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("EEG")) {

									String^ ID = AdapterNode->ChildNodes[0]->Attributes["ID"]->Value->ToString();
									String^ Muscle = AdapterNode->ChildNodes[0]->Attributes["Muscle"]->Value->ToString();
									String^ Side = AdapterNode->ChildNodes[0]->Attributes["Side"]->Value->ToString();
									String^ Prefix = AdapterNode->ChildNodes[0]->Attributes["Prefix"]->Value->ToString();

									for (int jj = 0; jj < 64; jj++)
									{
										System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[jj];
										//se imposto la cuffia 20 canali al 21 esimo troverò il ChannelNode nullo
										if (ChannelNode != nullptr) {
											GenericChannel^ ch = GetChannelFromID(
												ChannelNode->Attributes["ID"]->Value->ToString(),
												ChannelNode->Attributes["Muscle"]->Value->ToString(),
												ChannelNode->Attributes["Side"]->Value->ToString());
											if (ch != nullptr)
											{
												ad->SetChannel(System::Convert::ToUInt16(jj.ToString()), ch);
												try
												{
													ad->Channels[jj]->ChannelPrefix = ChannelNode->Attributes["Prefix"]->Value->ToString();
													ad->Channels[jj]->Description = GetEEGLabelFor20EEGFromIndex(jj);
												}
												catch (Exception^ ex) {
												}
											}
										}
										else {
											GenericChannel^ ch = GetChannelFromID(ID, Muscle, Side);
											ad->SetChannel(System::Convert::ToUInt16(jj.ToString()), ch);
											try
											{
												ad->Channels[jj]->ChannelPrefix = Prefix;
											}
											catch (Exception^ ex) {
											}
										}
									}

									int emptyChannelIndex = AdapterNode->ChildNodes->Count;
									for (int jj = 20; jj < 64; jj++)
									{
										System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[0];
										GenericChannel^ ch = GetChannelFromID(
											ChannelNode->Attributes["ID"]->Value->ToString(),
											ChannelNode->Attributes["Muscle"]->Value->ToString(),
											ChannelNode->Attributes["Side"]->Value->ToString());
										if (ch != nullptr)
										{
											ad->SetChannel(System::Convert::ToUInt16(emptyChannelIndex.ToString()), nullptr);
											try {
												//ad->Channels[emptyChannelIndex]->ChannelPrefix = "MustBeDeleted";
												emptyChannelIndex++;
											}
											catch (Exception^ ex) {
											}
										}
									}
								}




							}
						}
						else
							continue;
						if (ad->Description->Contains("MuoviPiu")) {
							gdr->ConnectIn(adPosition, (AdapterMuoviPiu^)ad);
						}
						else if (ad->Description->Contains("Control")) {
							//gdr->ConnectControl(adPosition, (AdapterControl^)ad);
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

		void DataGeneratorMuoviPiu::ConnectIn(const unsigned int port, AdapterMuoviPiu^ adapter)
		{
			if (port >= GetNumberConnectorsIn())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			//Connect EMG connector
			mConnectors[port]->Adapter = adapter;

			//Create and connect quaternion connector
			OTComm::MuoviPiu::AdapterQuaternions^ quaternionAd = gcnew OTComm::MuoviPiu::AdapterQuaternions();
			quaternionAd->SetChannel(0, gcnew OTComm::Sensors::Quaternion());
			quaternionAd->SetChannel(1, gcnew OTComm::Sensors::Quaternion());
			quaternionAd->SetChannel(2, gcnew OTComm::Sensors::Quaternion());
			quaternionAd->SetChannel(3, gcnew OTComm::Sensors::Quaternion());

			ConnectQuaternion(port + 1, quaternionAd);

			//Connect control aux (Buffer and Ramp)
			OTComm::MuoviPiu::AdapterControl^ auxControl = gcnew OTComm::MuoviPiu::AdapterControl();
			auxControl->SetChannel(0, gcnew OTComm::Sensors::BufferChannel());
			ConnectControl(port + 2, auxControl);

			OTComm::MuoviPiu::AdapterControl^ auxRamp = gcnew OTComm::MuoviPiu::AdapterControl();
			auxRamp->SetChannel(0, gcnew OTComm::Sensors::RampChannel());
			ConnectControl(port + 3, auxRamp);
		}

		void DataGeneratorMuoviPiu::ConnectAux(const unsigned int port, AdapterAux^ adapter)
		{
			/*
			if (port >= GetNumberChannelsAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}*/

			mConnectors[port]->Adapter = adapter;
		}

		void DataGeneratorMuoviPiu::ConnectControl(const unsigned int port, AdapterControl^ adapter)
		{
			mConnectors[port]->Adapter = adapter;
		}

		void DataGeneratorMuoviPiu::ConnectQuaternion(const unsigned int port, AdapterQuaternions^ adapter)
		{
			mConnectors[port]->Adapter = adapter;
		}

		GenericAdapter^ DataGeneratorMuoviPiu::GetAdapterFromID(String^ adapterID)
		{
			std::string converted = toStandardString(adapterID);

			if (adapterID == ("AdapterMuoviPiu"))
				return gcnew OTComm::MuoviPiu::AdapterMuoviPiu();
			if (StringContains("Direct connection to Auxiliary Input", converted))
				return gcnew OTComm::MuoviPiu::AdapterAux();		
			if (adapterID == ("AdapterControl"))
				return gcnew OTComm::MuoviPiu::AdapterControl();

			return nullptr;
		}

		GenericChannel^ DataGeneratorMuoviPiu::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
		{
			if (channelID == ("ELSCH020EEG"))
				return gcnew OTComm::Sensors::SensorELSCH020EEG(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064EEG"))
				return gcnew OTComm::Sensors::SensorELSCH064EEG(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
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
			if (channelID == ("GR10MM0804"))
				return gcnew OTComm::Sensors::SensorGR10MM0804(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH032NM6"))
				return gcnew OTComm::Sensors::SensorELSCH032NM6(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("CDE"))
				return gcnew OTComm::Sensors::SensorCDE(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("BufferChannel"))
				return gcnew OTComm::Sensors::BufferChannel(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			return nullptr;
		}

		GenericChannel^ DataGeneratorMuoviPiu::GetChannelFromString(String^ text)
		{
			if (text->Contains("ELSCH020EEG"))
				return gcnew OTComm::Sensors::SensorELSCH020EEG();
			if (text->Contains("ELSCH064EEG"))
				return gcnew OTComm::Sensors::SensorELSCH064EEG();
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

		unsigned int DataGeneratorMuoviPiu::GetNumberTotalChannels()
		{
			return GetNumberChannelsNonAux() + GetNumberChannelsAux() + GetNumberQuaternion()*4;
		}

		unsigned int DataGeneratorMuoviPiu::GetNumberChannelsNonAux()
		{
			return 64;
		}

		unsigned int DataGeneratorMuoviPiu::GetNumberChannelsAux()
		{
			return 2;
		}

		//unsigned int DataGeneratorMuoviPiu::GetNumberControlAux()
		//{
		//	return 4;
		//}

		unsigned int DataGeneratorMuoviPiu::GetNumberQuaternion() 
		{
			return 1;

		}

		unsigned int DataGeneratorMuoviPiu::GetNumberConnectorsAux()
		{
			return GetNumberChannelsAux();
		}

		unsigned int DataGeneratorMuoviPiu::GetNumberConnectorsIn()
		{
			return 1;
		}

		int DataGeneratorMuoviPiu::PortNumber::get()
		{
			return mPortNumber;
		}
#pragma endregion

#pragma region Device properties 

		void DataGeneratorMuoviPiu::SetIPAddress(String^ IPAddress)
		{
			mIPAdress = IPAddress;
		}

		void DataGeneratorMuoviPiu::SetIPPort(unsigned int IPPort)
		{
			mPortNumber = IPPort;
		}

		unsigned int DataGeneratorMuoviPiu::GetDeviceModel()
		{
			return (unsigned int)mModel;
		}

		Bitmap^ DataGeneratorMuoviPiu::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("64_Muovi.jpg", true);
		}

		String^ DataGeneratorMuoviPiu::GetDeviceDescription(void)
		{
			String^ res = gcnew String("MUOVI+");
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

		String^ DataGeneratorMuoviPiu::IPAdress::get()
		{
			String^ tmp = gcnew String(mIPAdress);
			return tmp;
		}

		bool DataGeneratorMuoviPiu::GetRemoveAverage() {
			return remove_average;
		}


#pragma endregion

#pragma region Public device operations
		void DataGeneratorMuoviPiu::OpenDevice(void)
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

		void DataGeneratorMuoviPiu::InitializeDevice()
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

		void DataGeneratorMuoviPiu::StartAcquisition(void)
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
			mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorMuoviPiu::OuternThread));
			mManagedAcquisitionThread->Start();
			//}
			LOG_EXITING;
		}

		void DataGeneratorMuoviPiu::StopAcquisition(void)
		{
			LOG_ENTERING;
			/*if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{*/
			DataGeneratorInterface::StopAcquisition();
			/*InternalStopMuoviDataTransfer();
			}*/
			LOG_EXITING;
		}

		void DataGeneratorMuoviPiu::CloseDevice(void)
		{
			LOG_ENTERING;
			/*if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
			InternalStopMuoviDataTransfer();
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

		Byte CreateStartCommand(byte workingmode, byte detectionmode){
			Byte command = (byte) 0;

			command = 0 << 4 |
				workingmode << 3 |
				detectionmode << 1 |
				1 << 0;

			return command;
		}

		Byte CreateTestCommand() {
			Byte command = (byte)0;

			command = 0 << 3 |
				DetectionModes::TestMode << 1 |
				1 << 0;

			return command;
		}
		
		int DataGeneratorMuoviPiu::ReadDeviceID(UCHAR* dev_id)
		{
			if (mDeviceHandle == INVALID_HANDLE_VALUE)
				return -5;
			return 0;
		}

		int DataGeneratorMuoviPiu::GetFirmwareVersion(String^% version)
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

		array<Byte>^ DataGeneratorMuoviPiu::CreateStartCommand()
		{	
			array<Byte>^ command;
			command = CreateStartCommandForProbe();
					
			
			if (GetConnectors()[0]->DetectionMode == DetectionModes::ImpedenceCheck)
				command = CreateImpedenceCheckCommand();
			return command;
		}

		array<Byte>^ DataGeneratorMuoviPiu::CreateStopCommand()
		{
			array<Byte>^ command;
			command = CreateStopCommandForProbe();
			

			return command;
		}

		array<Byte>^ DataGeneratorMuoviPiu::CreateStartCommandForProbe()
		{	
			Byte probe;
			Byte DetectionMode;
			/*
			for (int i = 0; i < mConnectors->Length; i++) {
				if (mConnectors[i]->Adapter != nullptr) {
					probe = (Byte)(i);
					DetectionMode = mConnectors[i]->DetectionMode;
					break;
				}
			}*/
			Byte workingMode = WorkingModes::EEG;

			if (remove_average) DetectionMode = DetectionModes::Monopolar64RemoveAverage;
			else DetectionMode = DetectionModes::Monopolar64Standard;
			if (GetSamplingFrequency() == 500) {
				workingMode = WorkingModes::EEG;
			}
			else {
				workingMode = WorkingModes::EMG;
			}
			array<Byte>^ command = gcnew array<Byte>(1);
			command[0] =	0 << 7 |
							probe << 5 |
							workingMode << 3 |
							DetectionMode << 1 |
							GoStops::Go;
						
			return command;
		}

		array<Byte>^ DataGeneratorMuoviPiu::CreateStopCommandForProbe() 
		{
			Byte probe;
			Byte DetectionMode;

			for (int i = 0; i < mConnectors->Length; i++) {
				if (mConnectors[i]->Adapter != nullptr) {
					probe = (Byte)(i);
					break;
				}
			}
			if (remove_average) DetectionMode = DetectionModes::Monopolar64RemoveAverage;
			else DetectionMode = DetectionModes::Monopolar64Standard;
			
			Byte workingMode = WorkingModes::EEG;
			if (GetSamplingFrequency() == 500) {
				workingMode = WorkingModes::EEG;
			}
			else {
				workingMode = WorkingModes::EMG;
			}

 			array<Byte>^ command = gcnew array<Byte>(1);
			command[0] = 0 << 7 |
				probe << 5 |
				workingMode << 3 |
				DetectionMode << 1 |
				GoStops::Stop;
			
			return command;
		}
		
		array<Byte>^ DataGeneratorMuoviPiu::CreateImpedenceCheckCommand()
		{	
			Byte probe;
			for (int i = 0; i < mConnectors->Length; i++) {
				if (mConnectors[i]->Adapter != nullptr) {
					probe = (Byte)(i);
					break;
				}
			}
			array<Byte>^ command = gcnew array<Byte>(1);
			command[0] = 0 << 5 |
				WorkingModes::EEG << 3 |
				DetectionModes::ImpedenceCheck << 1 |
				GoStops::Go;
			
			return command;
		}

		array<Byte>^ DataGeneratorMuoviPiu::CreateGetCommand()
		{
			array<Byte>^ command = gcnew array<Byte>(13);
			command[0] += 128;
			return command;
		}

		Byte DataGeneratorMuoviPiu::ComputeCRC(array<Byte>^ command, unsigned int Len)
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

		void DataGeneratorMuoviPiu::OuternThread()
		{
			try
			{
				DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				//mDataReaderInterfaceHelper->mAuxData = mUseAux;
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				
				//genero stringa con i comandi
				unsigned int mode = mConnectors[0]->DetectionMode;
				//array<Byte>^ command = CreateStartCommand();
				pin_ptr<unsigned int> CommandPinned = &mode;   // entire array is now pinned  
				mDataReaderInterfaceHelper->mCustomData = (LPARAM)CommandPinned;
				array<Byte>^ stopcommand = CreateStopCommand();
				pin_ptr<Byte> StopCommandPinned = &stopcommand[0];   // entire array is now pinned  
				mDataReaderInterfaceHelper->mStop64Command = (LPARAM)StopCommandPinned;

				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoMuoviPiu,  // Thread Func
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

		void DataGeneratorMuoviPiu::InternalStopMuoviDataTransfer(void)
		{
			LOG_ENTERING;
			
			LOG_EXITING;
		}

		void DataGeneratorMuoviPiu::CloseConnection()
		{
			DebugLog::Print(String::Format("Connection closed"));
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				closesocket((SOCKET)mDeviceHandle);
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
			}
		}

		int DataGeneratorMuoviPiu::WriteString(String^ command)
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

		int DataGeneratorMuoviPiu::ReadString(String^ %read)
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





					/*if (!GetOverlappedResultEx(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, 1000, true))
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
		DataValidator^ DataGeneratorMuoviPiu::DefaultTriggerValidator::get()
		{
			return mDefaulDataValidatorTrigger;
		}
#pragma endregion 

#pragma region Unmanaged code

//#pragma unmanaged


		union SwapShort
		{
			unsigned char Bytes[2];
			unsigned short Short;
		};

		ULONG _stdcall AsyncIoMuoviPiu(PVOID  ThreadParameter) 
		{						

			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ASYNCH_IO = 10;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			unsigned int NumBytesPerSample = helper->mSource[0].mBytesPerChannel; //2 or 3
			byte commandStart;
			unsigned char * Mode = (unsigned char *)helper->mCustomData;

			int detectionMode;

			if (*Mode == 2) {
				commandStart = CreateStartCommand(0, (byte)2); //IMPEDENCE
			}
			else {
				if (helper->mSource[0].mSamplingFrequency == 2000) {
					commandStart = CreateStartCommand(1, (byte)0); //EMG
				}
				else if (helper->mSource[0].mSamplingFrequency == 500) {
					commandStart = CreateStartCommand(0, (byte) 0); //EEG
				}
			}


			//commandStart = CreateTestCommand();

			byte commandStop = 0;
			unsigned char * StartCommand = &commandStart;//(unsigned char *)helper->mCustomData;
			unsigned char * StopCommand = &commandStop;  //(unsigned char *)helper->mStop64Command;


			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;

			//"acquition" (i.e, read from device) buffer size. This is the size, in bytes of each read buffer, which will be later multiplied by NUM_ASYNC_IO
			ULONG SingleAcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels*helper->mSource[0].mDataRegroupFactor * NumBytesPerSample; //95076;//190152;//
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
			InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //inet_addr("192.168.14.3"); //
															   //InetPton(AF_INET, _T("192.168.1.1"), &InternetAddr.sin_addr.s_addr);
			InternetAddr.sin_port = htons(54321);				
			if (bind(ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
			{
				closesocket(ListenSocket);
				WSACleanup();
				helper->mError("Timeout, socket error. Listener could not be started", -1, __FUNCTION__);
				return -1;
			}

			if (listen(ListenSocket, 1))
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
			/////////////////////////////////////////////////ATTESA DEL MUOVI//////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			fd_set ReadSet;
			fd_set WriteSet;

			int rv;
			FD_ZERO(&ReadSet); // clear the set 
			FD_ZERO(&WriteSet);
			FD_SET(ListenSocket, &ReadSet); // add our file descriptor to the set 
			FD_SET(ListenSocket, &WriteSet); // add our file descriptor to the set 
			struct timeval timeout;
			timeout.tv_sec = 15;								//PRENDERE DA CONFIGURAZIONE?
			timeout.tv_usec = 0;

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

			///////////NOTA: provare a chiudere qui closesocket(ListenSocket);

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

			DataBufWrite.len = 1;
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
					/*
					helper->mError("No more data", 0, __FUNCTION__);
					exitMainLoop = true;*/
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
									size_t posMappedChannel = 3 * channel;
									unsigned __int32 B0 = TempBufferAsInt32[posMappedChannel];
									unsigned __int32 B1 = TempBufferAsInt32[posMappedChannel + 1];
									unsigned __int32 B2 = TempBufferAsInt32[posMappedChannel + 2];

									/*
									int newInt = B0 + (B1 * 256) + (B2 * 65536);
									if((newInt & 0x00800000) > 0) {
										newInt |= 0xFF000000;
									}
									else{
										newInt &= 0x00FFFFFF;
									}
									
									*DecodedBufferPos32 = newInt;*/
									
									if (B0 & 0x80)
										*DecodedBufferPos32 = (0xFF << 24) | ((B0) << 16) | (B1 << 8) | B2;// TempBufferAsShort[ChannelMappings[channel]];
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

							return -1;
						}
					}
					//helper->mNewDataAvailable(0, DataSize::ThirtytwoBits, DecodedBufferCurrentStart32, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);
				}
			}//while

			DataBufWrite.len = 1;
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
		
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

		
#pragma endregion

	}
}