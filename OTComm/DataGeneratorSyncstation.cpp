#include "DataGeneratorSyncstation.h"
#include "stdio.h"
#include <string>
#include <math.h>

using namespace System::Xml;
namespace OTComm
{
	namespace Syncstation
	{
#pragma comment(lib, "Ws2_32.lib")

#pragma managed


#pragma region Constructors and destructors
		

		DataGeneratorSyncstation::DataGeneratorSyncstation(Model model, Frequency SamplingFrequency, bool HighRes) :
			DataGeneratorInterface(), mModel(model),
			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false),
			mDataReaderInterfaceHelper(NULL), mPortNumber(mDefaultPortNumber)
		{
			int numberOfAccessory = 3;
			int offset = 0;
			int deviceSource = 0;
			int EEG = 1; // 0 = EEG 500Hz 24bit    1 = EMG 2000Hz 16 bit

			if (SamplingFrequency != 2000) EEG = 0;

			//Inizializzo vettore dispositivo, mode e fsamp
			for (int i = 0; i < GetNumberConnectorsIn(); i++) {
				deviceEN[i] = 0;
				mode[i] = 3; //test modality
				EMG[i] = EEG;
			}

			for (int i = 0; i < 22; i++) { //22is total devices (16 + 4 aux + 2 control)
				devices[i] = gcnew Device;
				devices[i]->nSample = 2000;
				devices[i]->mode = 3;
				devices[i]->enable = 0;
			}
			for (int i = 16; i < 22; i++) {
				devices[i] = gcnew Device;
				devices[i]->nSample = 2000;
				devices[i]->mode = 0;
				devices[i]->enable = 1;
				devices[i]->nChannel = 1;
			}

			try
			{
				mIPAdress = String::Copy(mDefaultIPAdress);
				mConnectors = gcnew array<GenericConnector^>((GetNumberConnectableMuovi()+GetNumberConnectableSessantaquattro()+GetNumberConnectableDue())*4 + 6); //6 auxialiry channel of the station

				unsigned int k = 0;
				for (; k < GetNumberConnectableMuovi(); k++) {
					mConnectors[k + 0 + offset] = gcnew GenericConnector(32, 0);
					mConnectors[k + 1 + offset] = gcnew GenericConnector(4, 0);
					mConnectors[k + 2 + offset] = gcnew GenericConnector(1, 0);
					mConnectors[k + 3 + offset] = gcnew GenericConnector(1, 0);
					offset += 3;
				}

				for (k = GetNumberConnectableMuovi(); k < GetNumberConnectableSessantaquattro() + GetNumberConnectableMuovi(); k++) {
					mConnectors[k + 0 + offset] = gcnew GenericConnector(64, 0);
					mConnectors[k + 1 + offset] = gcnew GenericConnector(4, 0);
					mConnectors[k + 2 + offset] = gcnew GenericConnector(1, 0);
					mConnectors[k + 3 + offset] = gcnew GenericConnector(1, 0);
					offset += 3;

				}
				for (k = GetNumberConnectableSessantaquattro() + GetNumberConnectableMuovi(); k < GetNumberConnectableSessantaquattro() + GetNumberConnectableMuovi() + GetNumberConnectableDue(); k++) {
					mConnectors[k + 0 + offset] = gcnew GenericConnector(2, 0);
					mConnectors[k + 1 + offset] = gcnew GenericConnector(4, 0);
					mConnectors[k + 2 + offset] = gcnew GenericConnector(1, 0);
					mConnectors[k + 3 + offset] = gcnew GenericConnector(1, 0);
					offset += 3;
				}

				//Connect four auxiliary channel and two control
				int startAuxiliaryChannel = (GetNumberConnectableSessantaquattro() + GetNumberConnectableMuovi() + GetNumberConnectableDue())*4;
				int endAuxiliaryChannel = startAuxiliaryChannel + 3;
				for (int k = startAuxiliaryChannel; k <= endAuxiliaryChannel-1; k++) {
					mConnectors[k] = gcnew GenericConnector(1, 0);
					OTComm::Syncstation::AdapterSyncstationAux^ aux = gcnew OTComm::Syncstation::AdapterSyncstationAux();
					aux->SetChannel(0, gcnew OTComm::Sensors::AUXAcc);
					aux->Channels[0]->ChannelPrefix = "MustBeDeleted";
					ConnectSyncstationAux(k, aux);
				}

				mConnectors[endAuxiliaryChannel] = gcnew GenericConnector(1, 0);
				OTComm::Syncstation::AdapterLoadCell^ aux = gcnew OTComm::Syncstation::AdapterLoadCell();
				aux->SetChannel(0, gcnew OTComm::Sensors::AUXAcc);
				aux->Channels[0]->ChannelPrefix = "MustBeDeleted";
				ConnectLoadCell(endAuxiliaryChannel, aux);

				int bufferPos = startAuxiliaryChannel + 4;
				mConnectors[bufferPos] = gcnew GenericConnector(1, 0);
				OTComm::Syncstation::AdapterControl^ auxControl = gcnew OTComm::Syncstation::AdapterControl();
				auxControl->ID->Concat("Syncstation");
				auxControl->SetChannel(0, gcnew OTComm::Sensors::BufferChannel());
				auxControl->Channels[0]->ChannelPrefix = "MustBeDeleted";
				ConnectControl(bufferPos, auxControl);

				int rampPos = startAuxiliaryChannel + 5;
				mConnectors[rampPos] = gcnew GenericConnector(1, 0);
				OTComm::Syncstation::AdapterControl^ auxRamp = gcnew OTComm::Syncstation::AdapterControl();

				auxRamp->ID->Concat("Syncstation");
				auxRamp->SetChannel(0, gcnew OTComm::Sensors::RampChannel());
				auxRamp->Channels[0]->ChannelPrefix = "MustBeDeleted";
				ConnectControl(rampPos, auxRamp);



				mSources = gcnew array<DataGeneratorSingleSource^>(1);
				mSources[0] = gcnew DataGeneratorSingleSource(SamplingFrequency, HighRes ? 3 : 2, GetChannelsMapping());

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


		//NO MODEL, used for temp DGR
		DataGeneratorSyncstation::DataGeneratorSyncstation(Frequency SamplingFrequency, bool HighRes) :
			DataGeneratorInterface(), mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false),
			mDataReaderInterfaceHelper(NULL), mPortNumber(mDefaultPortNumber)
		{
			int numberOfAccessory = 6;
			int offset = 0;
			int deviceSource = 0;
			int EEG = 1; // 0 = EEG 500Hz 24bit    1 = EMG 2000Hz 16 bit

			if (SamplingFrequency == 500) EEG = 0;

			//DA SISTEMARE ?
			//Inizializzo vettore dispositivo, mode e fsamp
			for (int i = 0; i < GetNumberConnectorsIn(); i++) {

				deviceEN[i] = 0;
				mode[i] = 3; //test modality
				EMG[i] = EEG;
			}

			for (int i = 0; i < 22; i++) { //22is total devices (16 + 4 aux + 2 control)
				devices[i] = gcnew Device;
				devices[i]->nSample = 2000;
				devices[i]->mode = 3;
				devices[i]->enable = 0;
			}
			for (int i = 16; i < 22; i++) {
				devices[i] = gcnew Device;
				devices[i]->nSample = 2000;
				devices[i]->mode = 0;
				devices[i]->enable = 1;
				devices[i]->nChannel = 1;
			}

			try
			{
				mIPAdress = String::Copy(mDefaultIPAdress);				

				mSources = gcnew array<DataGeneratorSingleSource^>(1);
				mSources[0] = gcnew DataGeneratorSingleSource(SamplingFrequency, HighRes ? 3 : 2, GetChannelsMapping());

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

		DataGeneratorSyncstation::~DataGeneratorSyncstation()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorSyncstation();
		}

		DataGeneratorSyncstation::!DataGeneratorSyncstation()
		{

		}

#pragma endregion

#pragma region XML files

		String^ DataGeneratorSyncstation::GetDeviceXMLConfiguration()
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
			//DATO IL VETTORE DI ZERI E UNI PER sapere se il dispositivo è attivo
			//mi creo un numero. Quando leggo da xml farò l'operazione inversa per ottenere il vettore

			//totale connectors channels è usato per controllare quando cominciano i connector dei canali ausiliari.
			//mi serve per scrivere l'adapter index nell'xml
			int deviceConnectorsChannels = 0;
			__int32 ActiveChannels = 6; //AUX + Buffer & Ramp
			for (int i = 0; i < 16; i++) {
				if (deviceEN[i] == 1) {

					deviceConnectorsChannels += 4;

					if (i < 4) { //MUOVI
						ActiveChannels += 38;
					}
					else if (i == 4 || i == 5) {
						ActiveChannels += 70;
					}
					else {
						ActiveChannels += 8;
					}
				}
				//ActiveChannels += deviceEN[i] * pow(2, i); 
			}

			__int32 model = 0;
			for (int i = 0; i < 16; i++) {				
				model += deviceEN[i] * pow(2, i);
			}
			
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", ActiveChannels.ToString());// (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", model.ToString());
			mXMLTextStream->WriteAttributeString("IPAddress", mIPAdress);
			mXMLTextStream->WriteAttributeString("AnalogOutConn", mAnalogOutputConnector.ToString());
			mXMLTextStream->WriteAttributeString("AnalogOutCh", mAnalogOutputChannel.ToString());
			mXMLTextStream->WriteAttributeString("AnalogOutGain", mAnalogOutputGain.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			
			if (Latency == "")
				Latency = "200";
			mXMLTextStream->WriteAttributeString("Latency", Latency);
			for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			{
				mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			}

			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteStartElement("Channels");

			unsigned int ChannelNum = 0;
			int startIndex = 0;
			int adapterIndex = 0;
			int adapterIndexTEST = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{

				startIndex = k < GetNumberConnectorsIn() ? 16 * k : (k < (GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn()) ? GetNumberConnectorsIn() * 16 + (k - GetNumberConnectorsIn()) * 64 : (k - GetNumberConnectorsIn() - GetNumberConnectorsMultipleIn()) + GetNumberConnectorsIn() * 16 + GetNumberConnectorsMultipleIn() * 64);
				adapterIndexTEST = k + GetPositionFromAdapterPositionAndDeviceArray(k) * 4;
				if (startIndex > deviceConnectorsChannels) adapterIndexTEST = k + deviceConnectorsChannels;
				/*adapterIndex = GetPositionFromAdapterPositionAndDeviceArray(k);
				if (adapterIndex < deviceConnectorsChannels){
					adapterIndex = adapterIndex * 4;
				}
				else {
					adapterIndex = adapterIndex * 4 + (k-16);
				}*/
				if (mConnectors[k]->Adapter != nullptr)
				{
					if (mConnectors[k]->Adapter->ID->Contains("Direct connection to Auxiliary Input") || 
						mConnectors[k]->Adapter->ID->Contains("LoadCell") ||
						mConnectors[k]->Adapter->ID->Contains("Direct connection to Syncstation Input"))
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

		String^ DataGeneratorSyncstation::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			String^ Firmware;
			if (GetFirmwareVersion(Firmware) <= 0)
				Firmware = "Unknown";

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			__int32 ActiveChannels = 6; //AUX + Buffer & Ramp
			for (int i = 0; i < 16; i++) {
				if (deviceEN[i] == 1) {
					if (i < 4) { //MUOVI
						ActiveChannels += 38;
					}
					else if (i == 4 || i == 5) {
						ActiveChannels += 70;
					}
					else {
						ActiveChannels += 8;
					}
				}
				//ActiveChannels += deviceEN[i] * pow(2, i); 
			}

			__int32 model = 0;
			for (int i = 0; i < 16; i++) {
				model += deviceEN[i] * pow(2, i);
			}

			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", ActiveChannels.ToString());// (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", model.ToString()); //active devices
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
				/*if (mModel==Models::Syncstation_96)
				startIndex = k < 3 ? k * 16 : 96+k;
				else if (mModel == Models::Syncstation_192)
				{
				startIndex = k < 4 ? k * 16 : 64 + k * 64;
				startIndex = k >= 6 ? k * 16 : 64 + k * 64;
				}*/
				//startIndex = k < GetNumberConnectorsIn() ? 16 * k : (k < (GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn()) ? GetNumberConnectorsIn() * 16 + (k-GetNumberConnectorsIn()) * 64 : (k- GetNumberConnectorsIn() - GetNumberConnectorsMultipleIn()) + GetNumberConnectorsIn() * 16 + GetNumberConnectorsMultipleIn() * 64);

				if (mConnectors[k]->Adapter != nullptr)
				{
					if (mConnectors[k]->Adapter->ID->Contains("Direct connection to Auxiliary Input") || mConnectors[k]->Adapter->ID->Contains("Direct connection to Syncstation Input")  || mConnectors[k]->Adapter->ID->Contains("LoadCell"))
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

		DataGeneratorSyncstation^ DataGeneratorSyncstation::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorSyncstation^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();

			int numberOfMuovi = 0;
			int numberOfSessantaquattro = 0;
			int numberOfDue = 0;

			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try {
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
				if (deviceName->Contains("SYNCSTATION"))
				{

					System::Xml::XmlNode^ ChannelsNode = DeviceNode->ChildNodes[0];
					for (int ii = 0; ii < ChannelsNode->ChildNodes->Count; ii++)
					{
						System::Xml::XmlNode^ AdapterNode = ChannelsNode->ChildNodes[ii];
						/*DebugLog::Print(AdapterNode->Attributes["ID"]->Value->ToString());
						DebugLog::Print(Assembly::GetExecutingAssembly()->ToString());*/
						//GenericAdapter^ ad = GetAdapterFromID(AdapterNode->Attributes["ID"]->Value->ToString());


						if (AdapterNode->Attributes["ID"]->Value->ToString()->Equals("Muovi")) {
							numberOfMuovi++;
						}
						else if (AdapterNode->Attributes["ID"]->Value->ToString()->Equals("Muovi+")) {
							numberOfSessantaquattro++;
						}
						else if (AdapterNode->Attributes["ID"]->Value->ToString()->Equals("Due+")) {
							numberOfDue++;
						}
						else if (AdapterNode->Attributes["ID"]->Value->ToString()->Equals("Sessantaquattro")) {
							numberOfSessantaquattro++;
						}

					}
				}
				textReader->Close();
			}
				
			catch (Exception^ ex) {
				textReader->Close();
				throw "Error in FromXMLConfiguration";
			}

			textReader = gcnew XmlTextReader(Filename);
			xmlDoc->Load(textReader);
			DeviceNode = xmlDoc->ChildNodes[1];
			try {
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;


				__int32 totalDevices = System::Convert::ToUInt32(DeviceNode->Attributes["DeviceTotalChannels"]->Value);
				//remove aux and control channels from totale devices
				totalDevices = totalDevices - 6;

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
				String^ currentLatency = "";
				if (DeviceNode->Attributes["Latency"] != nullptr)
					currentLatency = DeviceNode->Attributes["Latency"]->Value;
				
				unsigned int deviceFrequency = System::Convert::ToUInt16(DeviceNode->Attributes["SampleFrequency"]->Value);
				if (deviceName->Contains("SYNCSTATION"))
				{
					gdr = gcnew OTComm::Syncstation::DataGeneratorSyncstation(deviceFrequency, false);
					gdr->Latency = currentLatency;

					//ASSIGN CONNECTED DEVICE
					if (deviceModel == 0) { //OLD FILE REGISTERED WITH SYNCSTATION, take channnels from TotalDevices
						for (int i = 0; i < 16; i++) {
							gdr->deviceEN[i] = (totalDevices >> i) & 0b0000000000000001;
						}
					}
					else {
						for (int i = 0; i < 16; i++) {
							gdr->deviceEN[i] = (deviceModel >> i) & 0b0000000000000001;
						}
					}

					/*
					if (numberOfMuovi != 0) numberOfMuovi = 4;
					if (numberOfSessantaquattro != 0) numberOfSessantaquattro = 2;
					if (numberOfDue != 0) numberOfDue = 10;*/
					gdr->CreateConnector(numberOfMuovi, numberOfSessantaquattro, numberOfDue);

					if (loadedipaddress != nullptr)
						gdr->SetIPAddress(loadedipaddress);
					gdr->SetAnalogOutputConnector(AOconn);
					gdr->SetAnalogOutputChannel(AOch);
					gdr->SetAnalogOutputGain(AOgain);
					
					int AUXSyncIndex = 0;

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
							if (ad->ID == "Sessantaquattro" || ad->ID == "Muovi+")
							{
								if (AdapterNode->ChildNodes->Count != 64) {
									//The true channels
									for (int jj = 0; jj < AdapterNode->ChildNodes->Count; jj++)
									{
										System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[jj];
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
											}
											catch (Exception^ ex) {
											}
										}
									}

									//INSERIMENTO CUFFIE EEG DA SETUP									
									if (AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("20") &&
										AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("EEG")){

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
												GenericChannel^ ch = GetChannelFromID(ID,Muscle, Side);
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
									else {

										int emptyChannelIndex = AdapterNode->ChildNodes->Count;

										for (int jj = 0; jj < AdapterNode->ChildNodes->Count; jj++) 
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
								else 
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
											ad->SetChannel(System::Convert::ToUInt16(jj.ToString()), ch);
											try {
												ad->Channels[jj]->ChannelPrefix = ChannelNode->Attributes["Prefix"]->Value->ToString();
												if(AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("64") &&
													AdapterNode->ChildNodes[0]->Attributes["Description"]->Value->ToString()->Contains("EEG")) {
													ad->Channels[jj]->Description = GetEEGLabelFor64EEGFromIndex(jj);
												}
											}
											catch (Exception^ ex) {
											}
										}
									}
								}
							}
							else if (ad->ID == "Muovi") {
								System::Xml::XmlNode^ TempChannelNode = AdapterNode->ChildNodes[0];
								String^ channel = TempChannelNode->Attributes["ID"]->Value->ToString();
								for (int jj = 0; jj < AdapterNode->ChildNodes->Count; jj++)
								{

									System::Xml::XmlNode^ ChannelNode = AdapterNode->ChildNodes[jj];
									GenericChannel^ ch = GetChannelFromID(
										ChannelNode->Attributes["ID"]->Value->ToString(),
										ChannelNode->Attributes["Muscle"]->Value->ToString(),
										ChannelNode->Attributes["Side"]->Value->ToString());
									if (ch != nullptr)
									{
										ad->SetChannel(System::Convert::ToUInt16(jj.ToString()), ch);
										try {
											ad->Channels[jj]->ChannelPrefix = ChannelNode->Attributes["Prefix"]->Value->ToString();
											ad->Channels[jj]->Description = GetEEGLabelFor20EEGFromIndex(jj);

										}
										catch (Exception^ ex) {
										}
									}
								}
								if (channel->Contains("020EEG")) {
									int emptyChannelIndex = AdapterNode->ChildNodes->Count;
									for (int jj = 20; jj < 32; jj++)
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
								}/*
								if (AdapterNode->ChildNodes->Count != 32) {
									//Initialization of first 32 empty channel
									int emptyChannelIndex = AdapterNode->ChildNodes->Count;
									for (int jj = 0; jj < AdapterNode->ChildNodes->Count; jj++)
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
								}	*/						
							}
							else 
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
										ad->SetChannel(System::Convert::ToUInt16(jj.ToString()), ch);
										try {
												ad->Channels[jj]->ChannelPrefix = ChannelNode->Attributes["Prefix"]->Value->ToString();	

										}
										catch (Exception^ ex) {
										}
									}
								}
							}
								
						}
						else
							continue;
						
						int EEGorEMG; //0 = EEG | 1 = EMG
						if (AdapterNode->Attributes["DetectionMode"]->Value == "1") {
							EEGorEMG = 1;
						}
						else EEGorEMG = 0;

						if (ad->ID->Equals("Muovi")) {
							AdapterMuoviDevice^ adapterM = (AdapterMuoviDevice^)ad;
							adapterM->SetSamplingFrequency(EEGorEMG);

							unsigned int adapterFinalPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							gdr->ConnectMuovi(adPosition/4, adapterFinalPosition,(AdapterMuoviDevice^)ad);
							//this line update the EEG/EMG modality of the connected device

							gdr->mode[adapterFinalPosition] = adapterM->AcquisitionMode;
							gdr->devices[adapterFinalPosition]->mode = gdr->mode[adapterFinalPosition];

							gdr->deviceEN[adapterFinalPosition] = 1; //set device as active
							gdr->EMG[adapterFinalPosition] = ((AdapterMuoviDevice^)ad)->SamplingFrequency;

							gdr->devices[adapterFinalPosition]->nSample = EEGorEMG == 1 ? 2000 : 500;
							gdr->devices[adapterFinalPosition]->enable = 1;
							gdr->devices[adapterFinalPosition]->nChannel = 38;

						}
						else if (ad->ID->Equals("Muovi+")) {
							AdapterMuoviPiuDevice^ adapterM = (AdapterMuoviPiuDevice^)ad;
							adapterM->SetSamplingFrequency(EEGorEMG);

							unsigned int adapterFinalPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							gdr->ConnectMuoviPiu(adPosition / 4, adapterFinalPosition, (AdapterMuoviPiuDevice^)ad);
							
							gdr->mode[adapterFinalPosition] = adapterM->AcquisitionMode;
							gdr->devices[adapterFinalPosition]->mode = gdr->mode[adapterFinalPosition];


							gdr->deviceEN[adapterFinalPosition] = 1; //set device as active
							gdr->EMG[adapterFinalPosition] = ((AdapterMuoviPiuDevice^)ad)->SamplingFrequency;
							gdr->devices[adapterFinalPosition]->nSample = EEGorEMG == 1 ? 2000 : 500;;
							gdr->devices[adapterFinalPosition]->enable = 1;
							gdr->devices[adapterFinalPosition]->nChannel = 70;

						}
						else if (ad->ID->Equals("Due+")) {
							AdapterDueDevice^ adapterM = (AdapterDueDevice^)ad;
							adapterM->SetSamplingFrequency(EEGorEMG);

							unsigned int adapterFinalPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							gdr->ConnectDue(adPosition/4, adapterFinalPosition, (AdapterDueDevice^)ad);
							
							gdr->deviceEN[adapterFinalPosition] = 1; //set device as active
							gdr->EMG[adapterFinalPosition] = ((AdapterDueDevice^)ad)->SamplingFrequency;
							gdr->devices[adapterFinalPosition]->nSample = EEGorEMG == 1 ? 2000 : 500;;
							gdr->devices[adapterFinalPosition]->enable = 1;
							gdr->devices[adapterFinalPosition]->nChannel = 8;

						}
						else if (ad->ID->Equals("Sessantaquattro")) {
							AdapterSessantaquattroDevice^ adapterM = (AdapterSessantaquattroDevice^)ad;
							adapterM->SetSamplingFrequency(EEGorEMG);

							unsigned int adapterFinalPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							gdr->ConnectSessantaquattro(adPosition/4, adapterFinalPosition, (AdapterSessantaquattroDevice^)ad);

							gdr->mode[adapterFinalPosition] = adapterM->AcquisitionMode;
							gdr->devices[adapterFinalPosition]->mode = gdr->mode[adapterFinalPosition];

							gdr->deviceEN[adapterFinalPosition] = 1; //set device as active
							gdr->EMG[adapterFinalPosition] = ((AdapterSessantaquattroDevice^)ad)->SamplingFrequency;
							gdr->devices[adapterFinalPosition]->nSample = EEGorEMG == 1 ? 2000 : 500;;
							gdr->devices[adapterFinalPosition]->enable = 1;
							gdr->devices[adapterFinalPosition]->nChannel = 70;
						}
						else if (ad->ID->Contains("Direct connection to Syncstation Input")) {
							unsigned int adapterTempPosition = gdr->GetPositionForAUXSync();
							int adapterFinalPosition = adPosition;
							gdr->GetConnectors()[adapterFinalPosition]->Adapter->Channels[0]->ChannelPrefix = ad->Channels[0]->ChannelPrefix;
							gdr->GetConnectors()[adapterFinalPosition]->AuxScaleFactor = System::Convert::ToUInt16(AdapterNode->Attributes["AuxScaleFactor"]->Value);
							gdr->GetConnectors()[adapterFinalPosition]->unitOfMeasurement = AdapterNode->Attributes["UnitOfMeasurement"]->Value;

							AUXSyncIndex++;

							//non serve assegnare nel vettore device perchè viene fatto nella creazione dei connettori
						}
						else if (ad->ID->Contains("AdapterLoadCell")) {
							unsigned int adapterTempPosition = gdr->GetPositionForAUXSync();
							int adapterFinalPosition = adPosition;
							gdr->GetConnectors()[adapterFinalPosition]->Adapter->Channels[0]->ChannelPrefix = ad->Channels[0]->ChannelPrefix;
							gdr->GetConnectors()[adapterFinalPosition]->AuxScaleFactor = System::Convert::ToUInt16(AdapterNode->Attributes["AuxScaleFactor"]->Value);
							gdr->GetConnectors()[adapterFinalPosition]->unitOfMeasurement = AdapterNode->Attributes["UnitOfMeasurement"]->Value;

							AUXSyncIndex++;
							//non serve assegnare nel vettore device perchè viene fatto nella creazione dei connettori

						}

					}
					bool resolution = gdr->GetHighRes();
					gdr->UpdateHighRes(resolution);
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

		DataGeneratorSyncstation^ DataGeneratorSyncstation::FromXMLConfigurationForSetup(String^ Filename)
		{
			DataGeneratorSyncstation^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();

			int numberOfMuovi = 0;
			int numberOfSessantaquattro = 0;
			int numberOfDue = 0;

			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try {
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
				
				if (deviceName->Contains("SYNCSTATION"))
				{
					System::Xml::XmlNode^ ChannelsNode = DeviceNode->ChildNodes[0];
					for (int ii = 0; ii < ChannelsNode->ChildNodes->Count; ii++)
					{
						System::Xml::XmlNode^ AdapterNode = ChannelsNode->ChildNodes[ii];
						/*DebugLog::Print(AdapterNode->Attributes["ID"]->Value->ToString());
						DebugLog::Print(Assembly::GetExecutingAssembly()->ToString());*/
						//GenericAdapter^ ad = GetAdapterFromID(AdapterNode->Attributes["ID"]->Value->ToString());


						if (AdapterNode->Attributes["ID"]->Value->ToString()->Equals("Muovi")) {
							numberOfMuovi++;
						}
						else if (AdapterNode->Attributes["ID"]->Value->ToString()->Equals("Muovi+")) {
							numberOfSessantaquattro++;
						}
						else if (AdapterNode->Attributes["ID"]->Value->ToString()->Equals("Due+")) {
							numberOfDue++;
						}
						else if (AdapterNode->Attributes["ID"]->Value->ToString()->Equals("Sessantaquattro")) {
							numberOfSessantaquattro++;
						}

					}
				}
				textReader->Close();
			}

			catch (Exception^ ex) {
				textReader->Close();
				throw "Error in FromXMLConfiguration";
			}

			textReader = gcnew XmlTextReader(Filename);
			xmlDoc->Load(textReader);
			DeviceNode = xmlDoc->ChildNodes[1];
			try {
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				__int32 totalDevices = System::Convert::ToUInt32(DeviceNode->Attributes["DeviceTotalChannels"]->Value)-6;
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
				
				String^ currentLatency = "200";
				if (DeviceNode->Attributes["Latency"] != nullptr)
					currentLatency = DeviceNode->Attributes["Latency"]->Value;
				
				if (deviceName->Contains("SYNCSTATION"))
				{
					gdr = gcnew OTComm::Syncstation::DataGeneratorSyncstation(deviceFrequency, false);

					//ASSIGN CONNECTED DEVICE
					if (deviceModel == 0) { //OLD FILE REGISTERED WITH SYNCSTATION, take channnels from TotalDevices
						for (int i = 0; i < 16; i++) {
							gdr->deviceEN[i] = (totalDevices >> i) & 0b0000000000000001;
						}
					}
					else {
						for (int i = 0; i < 16; i++) {
							gdr->deviceEN[i] = (deviceModel >> i) & 0b0000000000000001;
						}
					}

					//define maximum number of connectors: FULL SYNCSTATION for setup
					numberOfMuovi = 4;
					numberOfSessantaquattro = 2;
					numberOfDue = 10;
					
					gdr->Latency = currentLatency;
					gdr->CreateConnector(numberOfMuovi, numberOfSessantaquattro, numberOfDue);

					if (loadedipaddress != nullptr)
						gdr->SetIPAddress(loadedipaddress);
					gdr->SetAnalogOutputConnector(AOconn);
					gdr->SetAnalogOutputChannel(AOch);
					gdr->SetAnalogOutputGain(AOgain);

					//used to connect aux channels
					int AUXSyncIndex = 0;

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
								}
							}
						}
						else
							continue;

						int EEGorEMG; //0 = EEG | 1 = EMG 
						if (AdapterNode->Attributes["DetectionMode"]->Value == "0") {
							EEGorEMG = 0;
						}
						else EEGorEMG = 1;

						if (ad->ID->Equals("Muovi")) {
							AdapterMuoviDevice^ adapterM = (AdapterMuoviDevice^)ad;
							adapterM->SetSamplingFrequency(EEGorEMG);
							unsigned int adapterFinalPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							gdr->ConnectMuovi(adapterFinalPosition, adapterFinalPosition, (AdapterMuoviDevice^)ad);

							gdr->deviceEN[adapterFinalPosition] = 1; //set device as active
							gdr->EMG[adapterFinalPosition] = ((AdapterMuoviDevice^)ad)->SamplingFrequency;

							gdr->devices[adapterFinalPosition]->nSample = EEGorEMG == 1 ? 2000 : 500;
							gdr->devices[adapterFinalPosition]->enable = 1;
							gdr->devices[adapterFinalPosition]->nChannel = 38;
						}
						else if (ad->ID->Equals("Muovi+")) {
							AdapterMuoviPiuDevice^ adapterM = (AdapterMuoviPiuDevice^)ad;
							adapterM->SetSamplingFrequency(EEGorEMG);
							unsigned int adapterFinalPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							gdr->ConnectMuoviPiu(adapterFinalPosition, adapterFinalPosition, (AdapterMuoviPiuDevice^)ad);
							gdr->deviceEN[adapterFinalPosition] = 1; //set device active
							gdr->EMG[adapterFinalPosition] = ((AdapterMuoviPiuDevice^)ad)->SamplingFrequency;

							gdr->devices[adapterFinalPosition]->nSample = EEGorEMG == 1 ? 2000 : 500;;
							gdr->devices[adapterFinalPosition]->enable = 1;
							gdr->devices[adapterFinalPosition]->nChannel = 70;
						}
						else if (ad->ID->Equals("Due+")) {
							AdapterDueDevice^ adapterM = (AdapterDueDevice^)ad;
							adapterM->SetSamplingFrequency(EEGorEMG);						
							unsigned int adapterFinalPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							gdr->ConnectDue(adapterFinalPosition, adapterFinalPosition, (AdapterDueDevice^)ad);
							gdr->deviceEN[adapterFinalPosition] = 1; //set device active
							gdr->EMG[adapterFinalPosition] = ((AdapterDueDevice^)ad)->SamplingFrequency;

							gdr->devices[adapterFinalPosition]->nSample = EEGorEMG == 1 ? 2000 : 500;;
							gdr->devices[adapterFinalPosition]->enable = 1;
							gdr->devices[adapterFinalPosition]->nChannel = 8;
						}
						else if (ad->ID->Equals("Sessantaquattro")) {
							AdapterSessantaquattroDevice^ adapterM = (AdapterSessantaquattroDevice^)ad;
							adapterM->SetSamplingFrequency(EEGorEMG);

							unsigned int adapterFinalPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							if (numberOfMuovi == 0)	adapterFinalPosition -= 4;

							gdr->ConnectSessantaquattro(adapterFinalPosition, adapterFinalPosition, (AdapterSessantaquattroDevice^)ad);
							gdr->deviceEN[adapterFinalPosition] = 1; //set device active
							gdr->EMG[adapterFinalPosition] = ((AdapterSessantaquattroDevice^)ad)->SamplingFrequency;

							gdr->devices[adapterFinalPosition]->nSample = EEGorEMG == 1 ? 2000 : 500;;
							gdr->devices[adapterFinalPosition]->enable = 1;
							gdr->devices[adapterFinalPosition]->nChannel = 70;
						}
						else if (ad->ID->Contains("Direct connection to Syncstation Input")) {
							unsigned int adapterTempPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							int adapterFinalPosition = (adapterTempPosition*4) + AUXSyncIndex;
							gdr->ConnectSyncstationAux(adapterFinalPosition, (AdapterSyncstationAux^)ad);

							gdr->GetConnectors()[adapterFinalPosition]->Adapter->Channels[0]->ChannelPrefix = ad->Channels[0]->ChannelPrefix;
							gdr->GetConnectors()[adapterFinalPosition]->AuxScaleFactor = System::Convert::ToUInt16(AdapterNode->Attributes["AuxScaleFactor"]->Value);
							gdr->GetConnectors()[adapterFinalPosition]->unitOfMeasurement = AdapterNode->Attributes["UnitOfMeasurement"]->Value;

							AUXSyncIndex++;

							//non serve assegnare nel vettore device perchè viene fatto nella creazione dei connettori
						}
						else if (ad->ID->Contains("AdapterLoadCell")) {
							unsigned int adapterTempPosition = gdr->GetPositionFromAdapterPositionAndDeviceArray(adPosition);
							int adapterFinalPosition = (adapterTempPosition)*4+AUXSyncIndex;
							gdr->ConnectLoadCell(adapterFinalPosition, (AdapterLoadCell^)ad);

							gdr->GetConnectors()[adapterFinalPosition]->Adapter->Channels[0]->ChannelPrefix = ad->Channels[0]->ChannelPrefix;
							gdr->GetConnectors()[adapterFinalPosition]->AuxScaleFactor = System::Convert::ToUInt16(AdapterNode->Attributes["AuxScaleFactor"]->Value);
							gdr->GetConnectors()[adapterFinalPosition]->unitOfMeasurement = AdapterNode->Attributes["UnitOfMeasurement"]->Value;

							AUXSyncIndex++;

							//non serve assegnare nel vettore device perchè viene fatto nella creazione dei connettori
						}

					}
					bool resolution = gdr->GetHighRes();
					gdr->UpdateHighRes(resolution);
				}
				textReader->Close();
			}
			catch (Exception^ ex) {
				textReader->Close();
				throw "Error in FromXMLConfiguration";
			}
			return gdr;
		}

		unsigned int DataGeneratorSyncstation::GetPositionFromAdapterPositionAndDeviceArray(int adPosition) {
			
			int numberOfPreviousAdapter = adPosition / 4; //this is the number of adapter from the given position: 0->0; 4->1; 8->2
														  //if I have adPosition = 4 this not means the adapter is in position 1 but I have to check the deviceEN array to know the exact position

			int i = 0;
			int n = 0;
			for (i = 0; i < 16; i++) {
				if (deviceEN[i] == 1) {
					n++;
					if (n > numberOfPreviousAdapter) {
						break;
					}
				}
			}
			return i;
		}

		unsigned int DataGeneratorSyncstation::GetPositionForAUXSync() {
			int i = 0;
			int n = 0;
			for (i = 0; i < 16; i++) {
				if (deviceEN[i] == 1) { //count number of inserted channels
					n++;					
				}
			}
			return n;

		}


#pragma endregion

#pragma region Connectors and adapters

		void DataGeneratorSyncstation::UpdateNumberOfConnectedDevice() {
			mDeviceTotalChannels = GetNumberTotalChannels();
		}

		unsigned int DataGeneratorSyncstation::RecognizeAdapterForCommand(String^ adapterID)
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

		unsigned int DataGeneratorSyncstation::RecognizeSensorForCommand(String^ channelID)
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

		unsigned int DataGeneratorSyncstation::RecognizeSideForCommand(String^ sideID)
		{
			if (sideID == "Left") return 64;
			if (sideID == "Right") return 128;
			return 0;
		}

		unsigned int DataGeneratorSyncstation::RecognizeHPForCommand(unsigned int highpass)
		{
			unsigned int val = Convert::ToByte(highpass) << 4;
			return val;
		}

		unsigned int DataGeneratorSyncstation::RecognizeLPForCommand(unsigned int lowpass)
		{
			unsigned int val = Convert::ToByte(lowpass) << 2;
			return val;
		}

		void DataGeneratorSyncstation::SetAnalogOutputConnector(unsigned int analogOutputConnector)
		{
			mAnalogOutputConnector = analogOutputConnector;
		}

		unsigned int DataGeneratorSyncstation::GetAnalogOutputConnector()
		{
			return mAnalogOutputConnector;
		}

		void DataGeneratorSyncstation::SetAnalogOutputGain(unsigned int analogOutputGain)
		{
			mAnalogOutputGain = analogOutputGain;
		}

		unsigned int DataGeneratorSyncstation::GetAnalogOutputGain()
		{
			return mAnalogOutputGain;
		}

		void DataGeneratorSyncstation::SetAnalogOutputChannel(unsigned int analogOutputChannel)
		{
			mAnalogOutputChannel = analogOutputChannel;
		}

		unsigned int DataGeneratorSyncstation::GetAnalogOutputChannel()
		{
			return mAnalogOutputChannel;
		}

		array<unsigned int>^ DataGeneratorSyncstation::GetAcquiredChannels()
		{
			return mAcquiredChannelsIndices;
		}

		void DataGeneratorSyncstation::ConnectControl(const unsigned int port, AdapterQuaternions^ adapter)
		{
			mConnectors[port]->Adapter = adapter;
		}

		void DataGeneratorSyncstation::ConnectMuovi(const unsigned int port, unsigned int deviceIndex, AdapterMuoviDevice^ adapter) {
			if(port > 3) {
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			if (mConnectors[port * 4]->Adapter == nullptr)
				connectedMuovi++;

			//Connect muovi
			mConnectors[port*4]->Adapter = adapter;
			mConnectors[port * 4]->DetectionMode = adapter->SamplingFrequency;

			//connect Quaternion
			OTComm::Syncstation::AdapterQuaternions^ adQuaternion = gcnew OTComm::Syncstation::AdapterQuaternions();
			
			//Single quaternion initialization. If i use the same variable strange thinks happen (reference saving)
			OTComm::Sensors::Quaternion^ quaternion1 = gcnew OTComm::Sensors::Quaternion();
			quaternion1->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;			

			OTComm::Sensors::Quaternion^ quaternion2 = gcnew OTComm::Sensors::Quaternion();
			quaternion2->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion3 = gcnew OTComm::Sensors::Quaternion();
			quaternion3->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion4 = gcnew OTComm::Sensors::Quaternion();
			quaternion4->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;


			adQuaternion->SetChannel(0, quaternion1);
			adQuaternion->SetChannel(1, quaternion2);
			adQuaternion->SetChannel(2, quaternion3);
			adQuaternion->SetChannel(3, quaternion4);
			ConnectControl(port * 4 + 1, adQuaternion);
			mConnectors[port * 4 + 1]->DetectionMode = adapter->SamplingFrequency;


			//connect Buffer/Ramp
			OTComm::Syncstation::AdapterControl^ auxControl = gcnew OTComm::Syncstation::AdapterControl();
			OTComm::Sensors::BufferChannel^ bufferChannel = gcnew OTComm::Sensors::BufferChannel();
			bufferChannel->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			auxControl->SetChannel(0, bufferChannel);
			ConnectControl(port * 4 + 2, auxControl);
			mConnectors[port * 4 + 2]->DetectionMode = adapter->SamplingFrequency;

			OTComm::Syncstation::AdapterControl^ auxRamp = gcnew OTComm::Syncstation::AdapterControl();
			
			OTComm::Sensors::RampChannel^ rampChannel = gcnew OTComm::Sensors::RampChannel();
			rampChannel->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;
			auxRamp->SetChannel(0, rampChannel);
			ConnectControl(port * 4 + 3, auxRamp);
			mConnectors[port * 4 + 3]->DetectionMode = adapter->SamplingFrequency;

			//update total number of channels
			mDeviceTotalChannels = GetNumberTotalChannels();

			mode[deviceIndex] = adapter->AcquisitionMode;
			devices[deviceIndex]->mode = mode[deviceIndex];
		}

		void DataGeneratorSyncstation::ConnectSessantaquattro(const unsigned int port, unsigned int deviceIndex, AdapterSessantaquattroDevice^ adapter) {
			/*
			if (port < 4 || port > 5) {
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}*/

			//You are adding a new device just if the connector is empty. if the connector is not empty, this means that a device is already connected.
			if(mConnectors[port*4]->Adapter==nullptr)
				connectedSessantaquattro++;


			mConnectors[port * 4]->Adapter = adapter;
			mConnectors[port * 4]->DetectionMode = adapter->SamplingFrequency;

			//connect Quaternion
			OTComm::Syncstation::AdapterQuaternions^ adQuaternion = gcnew OTComm::Syncstation::AdapterQuaternions();

			//Single quaternion initialization. If i use the same variable strange thinks happen (reference saving)
			OTComm::Sensors::Quaternion^ quaternion1 = gcnew OTComm::Sensors::Quaternion();
			quaternion1->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion2 = gcnew OTComm::Sensors::Quaternion();
			quaternion2->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion3 = gcnew OTComm::Sensors::Quaternion();
			quaternion3->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion4 = gcnew OTComm::Sensors::Quaternion();
			quaternion4->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;


			adQuaternion->SetChannel(0, quaternion1);
			adQuaternion->SetChannel(1, quaternion2);
			adQuaternion->SetChannel(2, quaternion3);
			adQuaternion->SetChannel(3, quaternion4);
			ConnectControl(port * 4 + 1, adQuaternion);
			mConnectors[port * 4 + 1]->DetectionMode = adapter->SamplingFrequency;

			//connect Buffer/Ramp
			OTComm::Syncstation::AdapterControl^ auxControl = gcnew OTComm::Syncstation::AdapterControl();
			OTComm::Sensors::BufferChannel^ bufferChannel = gcnew OTComm::Sensors::BufferChannel();
			bufferChannel->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			auxControl->SetChannel(0, bufferChannel);
			ConnectControl(port * 4 + 2, auxControl);
			mConnectors[port * 4 + 2]->DetectionMode = adapter->SamplingFrequency;

			OTComm::Syncstation::AdapterControl^ auxRamp = gcnew OTComm::Syncstation::AdapterControl();

			OTComm::Sensors::RampChannel^ rampChannel = gcnew OTComm::Sensors::RampChannel();
			rampChannel->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;
			auxRamp->SetChannel(0, rampChannel);
			ConnectControl(port * 4 + 3, auxRamp);
			mConnectors[port * 4 + 3]->DetectionMode = adapter->SamplingFrequency;

			//update total number of channels
			mDeviceTotalChannels = GetNumberTotalChannels();

			mode[deviceIndex] = adapter->AcquisitionMode;
			devices[deviceIndex]->mode = mode[deviceIndex];

		}

		void DataGeneratorSyncstation::ConnectMuoviPiu(const unsigned int port, unsigned int deviceIndex, AdapterMuoviPiuDevice^ adapter) {
			/*
			if (port < 4 || port > 5) {
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}*/

			if (mConnectors[port * 4]->Adapter == nullptr)
				connectedSessantaquattro++;

			//Connect muovi
			mConnectors[port * 4]->Adapter = adapter;
			mConnectors[port * 4]->DetectionMode = adapter->SamplingFrequency;

			OTComm::Syncstation::AdapterQuaternions^ adQuaternion = gcnew OTComm::Syncstation::AdapterQuaternions();

			//Single quaternion initialization. If i use the same variable strange thinks happen (reference saving)
			OTComm::Sensors::Quaternion^ quaternion1 = gcnew OTComm::Sensors::Quaternion();
			quaternion1->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion2 = gcnew OTComm::Sensors::Quaternion();
			quaternion2->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion3 = gcnew OTComm::Sensors::Quaternion();
			quaternion3->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion4 = gcnew OTComm::Sensors::Quaternion();
			quaternion4->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;


			adQuaternion->SetChannel(0, quaternion1);
			adQuaternion->SetChannel(1, quaternion2);
			adQuaternion->SetChannel(2, quaternion3);
			adQuaternion->SetChannel(3, quaternion4);
			ConnectControl(port * 4 + 1, adQuaternion);
			mConnectors[port * 4 + 1]->DetectionMode = adapter->SamplingFrequency;

			//connect Buffer/Ramp
			OTComm::Syncstation::AdapterControl^ auxControl = gcnew OTComm::Syncstation::AdapterControl();
			OTComm::Sensors::BufferChannel^ bufferChannel = gcnew OTComm::Sensors::BufferChannel();
			bufferChannel->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			auxControl->SetChannel(0, bufferChannel);
			ConnectControl(port * 4 + 2, auxControl);
			mConnectors[port * 4 + 2]->DetectionMode = adapter->SamplingFrequency;

			OTComm::Syncstation::AdapterControl^ auxRamp = gcnew OTComm::Syncstation::AdapterControl();

			OTComm::Sensors::RampChannel^ rampChannel = gcnew OTComm::Sensors::RampChannel();
			rampChannel->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;
			auxRamp->SetChannel(0, rampChannel);
			ConnectControl(port * 4 + 3, auxRamp);
			mConnectors[port * 4 + 3]->DetectionMode = adapter->SamplingFrequency;

			//update total number of channels
			mDeviceTotalChannels = GetNumberTotalChannels();

			mode[deviceIndex] = adapter->AcquisitionMode;
			devices[deviceIndex]->mode = mode[deviceIndex];
		}

		void DataGeneratorSyncstation::ConnectDue(const unsigned int port, unsigned int deviceIndex, AdapterDueDevice^ adapter) {
			/*
			if (port < 6) {
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}*/
			
			//I need to remove position in case muovi or sessantaquattro are not connected

			//int connectorIndex = GetNumberConnectableMuovi() * 4 + GetNumberConnectableSessantaquattro() * 4 + (port - (GetNumberConnectableMuovi() + GetNumberConnectableSessantaquattro()))*7;
			mConnectors[port*4+0]->DetectionMode = adapter->SamplingFrequency;

			if (mConnectors[port * 4]->Adapter == nullptr)
				connectedDue++;

			//CONNECT DUE DEVICE
			mConnectors[port*4+0]->Adapter = adapter;

			OTComm::Syncstation::AdapterQuaternions^ adQuaternion = gcnew OTComm::Syncstation::AdapterQuaternions();

			//Single quaternion initialization. If i use the same variable strange thinks happen (reference saving)
			OTComm::Sensors::Quaternion^ quaternion1 = gcnew OTComm::Sensors::Quaternion();
			quaternion1->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion2 = gcnew OTComm::Sensors::Quaternion();
			quaternion2->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion3 = gcnew OTComm::Sensors::Quaternion();
			quaternion3->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			OTComm::Sensors::Quaternion^ quaternion4 = gcnew OTComm::Sensors::Quaternion();
			quaternion4->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;


			adQuaternion->SetChannel(0, quaternion1);
			adQuaternion->SetChannel(1, quaternion2);
			adQuaternion->SetChannel(2, quaternion3);
			adQuaternion->SetChannel(3, quaternion4);
			ConnectControl(port * 4 + 1, adQuaternion);
			mConnectors[port * 4 + 1]->DetectionMode = adapter->SamplingFrequency;

			//connect Buffer/Ramp
			OTComm::Syncstation::AdapterControl^ auxControl = gcnew OTComm::Syncstation::AdapterControl();
			OTComm::Sensors::BufferChannel^ bufferChannel = gcnew OTComm::Sensors::BufferChannel();
			bufferChannel->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;

			auxControl->SetChannel(0, bufferChannel);
			ConnectControl(port * 4 + 2, auxControl);
			mConnectors[port * 4 + 2]->DetectionMode = adapter->SamplingFrequency;

			OTComm::Syncstation::AdapterControl^ auxRamp = gcnew OTComm::Syncstation::AdapterControl();

			OTComm::Sensors::RampChannel^ rampChannel = gcnew OTComm::Sensors::RampChannel();
			rampChannel->ChannelPrefix = adapter->Channels[0]->ChannelPrefix;
			auxRamp->SetChannel(0, rampChannel);
			ConnectControl(port * 4 + 3, auxRamp);
			mConnectors[port * 4 + 3]->DetectionMode = adapter->SamplingFrequency;

			//update total number of channels
			mDeviceTotalChannels = GetNumberTotalChannels();

			mode[deviceIndex] = adapter->AcquisitionMode;
			devices[deviceIndex]->mode = mode[deviceIndex];

		}

		void DataGeneratorSyncstation::ConnectLoadCell(const unsigned int port, AdapterLoadCell^ adapter)
		{
			mConnectors[port]->Adapter = adapter;
			mConnectors[port]->DetectionMode = 1; //always EMG

		}

		void DataGeneratorSyncstation::ConnectAux(const unsigned int port, AdapterAux^ adapter)
		{			
			mConnectors[port]->Adapter = adapter;
		}

		void DataGeneratorSyncstation::ConnectSyncstationAux(const unsigned int port, AdapterSyncstationAux^ adapter)
		{
			mConnectors[port]->Adapter = adapter;
			mConnectors[port]->DetectionMode = 1; //always EMG
		}

		void DataGeneratorSyncstation::ConnectControl(const unsigned int port, AdapterControl^ adapter)
		{
			mConnectors[port]->Adapter = adapter;
		}

		array<unsigned int>^ DataGeneratorSyncstation::GetChannelsMapping()
		{
			return DataGeneratorSyncstation::CreateChannelsMapping();
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

		GenericAdapter^ DataGeneratorSyncstation::GetAdapterFromID(String^ adapterID)
		{
			std::string converted = toStandardString(adapterID);
			if (adapterID == ("Muovi"))
				return gcnew OTComm::Syncstation::AdapterMuoviDevice;
			else if (adapterID == ("Muovi+"))
				return gcnew OTComm::Syncstation::AdapterMuoviPiuDevice;
			else if (adapterID == ("Sessantaquattro"))
				return gcnew OTComm::Syncstation::AdapterSessantaquattroDevice;
			else if (adapterID == ("Due+"))
				return gcnew OTComm::Syncstation::AdapterDueDevice;
			else if (adapterID == ("Direct connection to Auxiliary Input")) 
				return gcnew OTComm::Syncstation::AdapterAux;			
			else if (adapterID == ("Direct connection to Syncstation Input")) 
				return gcnew OTComm::Syncstation::AdapterSyncstationAux;			
			else if (adapterID == ("AdapterLoadCell")) 
				return gcnew OTComm::Syncstation::AdapterLoadCell;
			
			return nullptr;
		}

		GenericChannel^ DataGeneratorSyncstation::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
		{
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
			if (channelID == ("AUX  Trigger"))
				return gcnew OTComm::Sensors::AUXTrig(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("RampChannel"))
				return gcnew OTComm::Sensors::RampChannel(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("BufferChannel"))
				return gcnew OTComm::Sensors::BufferChannel(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("iEMGb"))
				return gcnew OTComm::Sensors::SensoriEMGbipolar(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("iEMGm"))
				return gcnew OTComm::Sensors::SensoriEMGmonopolar(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));

			return nullptr;
		}

		GenericChannel^ DataGeneratorSyncstation::GetChannelFromString(String^ text)
		{
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

		unsigned int DataGeneratorSyncstation::GetNumberTotalChannels()
		{
			unsigned int total = 38 * connectedMuovi + 70 * connectedSessantaquattro + 8 * connectedDue + 6;
				return total;
		}

		unsigned int DataGeneratorSyncstation::GetNumberChannelsNonAux()
		{
			return 0;
		}

		unsigned int DataGeneratorSyncstation::GetNumberChannelsAux()
		{
			return 4;
		}

		unsigned int DataGeneratorSyncstation::GetNumberControlAux()
		{
			return 0;
		}

		unsigned int DataGeneratorSyncstation::GetNumberConnectorsAux()
		{
			return GetNumberChannelsAux();
		}

		unsigned int DataGeneratorSyncstation::GetNumberConnectorsMultipleIn()
		{
			if (mModel == Models::Syncstation_96)
				return 1;//TODO
			else if (mModel == Models::Syncstation_192)
				return 2;
			else if (mModel == Models::Syncstation_288)
				return 3;
			else if (mModel == Models::Syncstation_384)
				return 4;
			return 0;
		}

		unsigned int DataGeneratorSyncstation::GetNumberConnectorsIn()
		{
			/*
			if (mModel == Models::Syncstation_96)
				return 2;
			else if (mModel == Models::Syncstation_192)
				return 4;
			else if (mModel == Models::Syncstation_288)
				return 6;
			else if (mModel == Models::Syncstation_384)
				return 8;*/

			return 16;
		}

		unsigned int DataGeneratorSyncstation::GetNumberConnectableMuovi()
		{
			return 4;
		}

		unsigned int DataGeneratorSyncstation::GetNumberConnectableSessantaquattro()
		{
			return 2;
		}

		unsigned int DataGeneratorSyncstation::GetNumberConnectableDue()
		{
			return 10;
		}

		//return number of connectors to copy 
		unsigned int DataGeneratorSyncstation::GetNumberOfChannelsConnectedMuovi()
		{
			return connectedMuovi*7; //multiplied by 7 because each device has 7 connectors
		}

		unsigned int DataGeneratorSyncstation::GetNumberOfChannelsConnectedSessantaquattro()
		{
			return connectedSessantaquattro*7; //multiplied by 7 because each device has 7 connectors
		}

		unsigned int DataGeneratorSyncstation::GetNumberOfChannelsConnectedDue()
		{
			return connectedDue*7; //multiplied by 7 because each device has 7 connectors
		}

		array<unsigned int>^ DataGeneratorSyncstation::CreateChannelsMapping()
		{
			return nullptr;
		}


		int DataGeneratorSyncstation::PortNumber::get()
		{
			return mPortNumber;
		}


#pragma endregion

#pragma region Device properties 

		void DataGeneratorSyncstation::CreateConnector(int numberOfMuovi, int numberOfSessantaquattro, int numberOfDue) {

			mConnectors = gcnew array<GenericConnector^>((numberOfMuovi+ numberOfSessantaquattro+ numberOfDue)*4 + 6);
			unsigned int k = 0;
			int offset = 0;


			for (; k < numberOfMuovi; k++) {
				mConnectors[k + offset] = gcnew GenericConnector(32, 0);
				mConnectors[k + offset + 1] = gcnew GenericConnector(4, 0);
				mConnectors[k + offset + 2] = gcnew GenericConnector(1, 0);
				mConnectors[k + offset + 3] = gcnew GenericConnector(1, 0);
				offset += 3;
			}

			for (k = numberOfMuovi; k < numberOfMuovi+numberOfSessantaquattro; k++) {
				mConnectors[k + offset + 0] = gcnew GenericConnector(64, 0);
				mConnectors[k + offset + 1] = gcnew GenericConnector(4, 0);
				mConnectors[k + offset + 2] = gcnew GenericConnector(1, 0);
				mConnectors[k + offset + 3] = gcnew GenericConnector(1, 0);
				offset += 3;

			}
			for (k = numberOfMuovi + numberOfSessantaquattro; k < numberOfMuovi + numberOfSessantaquattro + numberOfDue; k++) {
				mConnectors[k + offset + 0] = gcnew GenericConnector(2, 0);
				mConnectors[k + offset + 1] = gcnew GenericConnector(4, 0);
				mConnectors[k + offset + 2] = gcnew GenericConnector(1, 0);
				mConnectors[k + offset + 3] = gcnew GenericConnector(1, 0);
				offset += 3;
			}

			int startAuxiliaryChannel = (numberOfMuovi + numberOfSessantaquattro + numberOfDue)*4; //+ (numberOfSessantaquattro*9);
			int endAuxiliaryChannel = startAuxiliaryChannel + 3;
			for (int k = startAuxiliaryChannel; k <= endAuxiliaryChannel-1; k++) {
				mConnectors[k] = gcnew GenericConnector(1, 0);
				OTComm::Syncstation::AdapterSyncstationAux^ aux = gcnew OTComm::Syncstation::AdapterSyncstationAux();
				aux->SetChannel(0, gcnew OTComm::Sensors::AUXAcc);
				aux->Channels[0]->ChannelPrefix = "MustBeDeleted";
				ConnectSyncstationAux(k, aux);
			}
			devices[16]->enable = 1;
			devices[16]->mode = 0;
			devices[16]->nSample = 2000;
			devices[16]->nChannel = 1;

			devices[17]->enable = 1;
			devices[17]->mode = 0;
			devices[17]->nSample = 2000;
			devices[17]->nChannel = 1;

			devices[18]->enable = 1;
			devices[18]->mode = 0;
			devices[18]->nSample = 2000;
			devices[18]->nChannel = 1;

			devices[19]->enable = 1;
			devices[19]->mode = 0;
			devices[19]->nSample = 2000;
			devices[19]->nChannel = 1;

			//CONTROL CHANNEL
			devices[20]->enable = 1;
			devices[20]->mode = 0;
			devices[20]->nSample = 2000;
			devices[20]->nChannel = 1;

			devices[21]->enable = 1;
			devices[21]->mode = 0;
			devices[21]->nSample = 2000;
			devices[21]->nChannel = 1;

			mConnectors[endAuxiliaryChannel] = gcnew GenericConnector(1, 0);
			OTComm::Syncstation::AdapterLoadCell^ aux = gcnew OTComm::Syncstation::AdapterLoadCell();
			aux->SetChannel(0, gcnew OTComm::Sensors::AUXAcc);
			ConnectLoadCell(endAuxiliaryChannel, aux);

			int bufferPos = startAuxiliaryChannel + 4;
			mConnectors[bufferPos] = gcnew GenericConnector(1, 0);
			OTComm::Syncstation::AdapterControl^ auxControl = gcnew OTComm::Syncstation::AdapterControl();
			auxControl->ID->Concat("Syncstation");
			auxControl->SetChannel(0, gcnew OTComm::Sensors::BufferChannel());
			auxControl->Channels[0]->ChannelPrefix = "MustBeDeleted";
			ConnectControl(bufferPos, auxControl);
			mConnectors[bufferPos]->DetectionMode = 1; //Always EMG

			int rampPos = startAuxiliaryChannel + 5;
			mConnectors[rampPos] = gcnew GenericConnector(1, 0);
			OTComm::Syncstation::AdapterControl^ auxRamp = gcnew OTComm::Syncstation::AdapterControl();
			auxRamp->ID->Concat("Syncstation");
			auxRamp->SetChannel(0, gcnew OTComm::Sensors::RampChannel());
			auxRamp->Channels[0]->ChannelPrefix = "MustBeDeleted";
			ConnectControl(rampPos, auxRamp);
			mConnectors[rampPos]->DetectionMode = 1; //Always EMG


		}

		void DataGeneratorSyncstation::CreateConnectorForMuovi(int numberOfMuovi) {
			
		}
		void DataGeneratorSyncstation::CreateConnectorForSessantaquattro(int numberOfSessantaquattro) {

		}
		void DataGeneratorSyncstation::CreateConnectorForDue(int numberOfDue) {

		}

		void DataGeneratorSyncstation::SetIPAddress(String^ IPAddress)
		{
			mIPAdress = IPAddress;
		}

		void DataGeneratorSyncstation::SetIPPort(unsigned int IPPort)
		{
			mPortNumber = IPPort;
		}

		unsigned int DataGeneratorSyncstation::GetDeviceModel()
		{
			return (unsigned int)mModel;
		}

		Bitmap^ DataGeneratorSyncstation::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("400_Syncstation.jpg", true);
		}

		String^ DataGeneratorSyncstation::GetDeviceDescription(void)
		{
			String^ res = gcnew String("SYNCSTATION");
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


		String^ DataGeneratorSyncstation::IPAdress::get()
		{
			String^ tmp = gcnew String(mIPAdress);
			return tmp;
		}

		void DataGeneratorSyncstation::SetNumberOfMuoviConnected(int val) {
			connectedMuovi = val;

			//update total number of channels
			mDeviceTotalChannels = GetNumberTotalChannels();
		}
		void DataGeneratorSyncstation::SetNumberOfSessantaquattroConnected(int val) {
			connectedSessantaquattro = val;

			//update total number of channels
			mDeviceTotalChannels = GetNumberTotalChannels();
		}
		void DataGeneratorSyncstation::SetNumberOfDueConnected(int val) {
			connectedDue = val;

			//update total number of channels
			mDeviceTotalChannels = GetNumberTotalChannels();
		}


		int DataGeneratorSyncstation::GetNumberOfMuoviConnected() {
			return connectedMuovi;
		}
		int DataGeneratorSyncstation::GetNumberOfSessantaquattroConnected() {
			return connectedSessantaquattro;
		}
		int DataGeneratorSyncstation::GetNumberOfDueConnected() {
			return connectedDue;
		}

		bool DataGeneratorSyncstation::GetHighRes() {
			bool res = true;
			for (int i = 0; i < 16; i++) {
				//check only connected device
				if (deviceEN[i] == 1) {
					if (EMG[i] == 1) {
						res = false;
						break;
					}
				}
			}
			return res;
		}

		void DataGeneratorSyncstation::UpdateHighRes(bool HighRes) {
			int SamplingFrequency = mSources[0]->mSamplingFrequency;
			mSources[0] = gcnew DataGeneratorSingleSource(SamplingFrequency, HighRes ? 3 : 2, GetChannelsMapping());
		}

		int DataGeneratorSyncstation::GetDeviceArrayValue(int i) {
			return deviceEN[i];
		}

		void DataGeneratorSyncstation::SetDeviceArrayValue(int i, int val) {
			deviceEN[i] = val;			
			
		}

		void DataGeneratorSyncstation::RemoveMuovi(int i) {
			deviceEN[i] = 0;
			connectedMuovi--;
		}

		void DataGeneratorSyncstation::RemoveSessantaquattro(int i) {
			deviceEN[i] = 0;
			connectedSessantaquattro--;
		}

		void DataGeneratorSyncstation::RemoveDue(int i) {
			deviceEN[i] = 0;
			connectedDue--;
		}

		void DataGeneratorSyncstation::RemoveDevice(int i) {
			if (i < 4) {
				RemoveMuovi(i);
			}
			else if (i < 6) {
				RemoveSessantaquattro(i);
			}
			else {
				RemoveDue(i);
			}
		}


#pragma endregion

#pragma region Public device operations
		void DataGeneratorSyncstation::OpenDevice(void)
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
					InternalStopSyncstationDataTransfer();
					mDeviceIsOpen = true;
					DataGeneratorInterface::OpenDevice();
					LOG_SUCCESS;
				}
			}
			LOG_EXITING;
		}

		void DataGeneratorSyncstation::InitializeDevice()
		{
			LOG_ENTERING;
			try
			{
				//InternalStopSyncstationDataTransfer();
				DataGeneratorInterface::InitializeDevice();
				/*SendLatencyCommand();*/
			}
			catch (Exception ^e)
			{
				InitializeDeviceFailedException^idfe = gcnew InitializeDeviceFailedException(__FUNCTION__, "", e);
				FireOnError(idfe);
				throw idfe;
			}
			LOG_EXITING;
		}

		void DataGeneratorSyncstation::StartAcquisition(void)
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
				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorSyncstation::OuternThread));
				mManagedAcquisitionThread->Start();
			}
			LOG_EXITING;
		}

		void DataGeneratorSyncstation::StopAcquisition(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				DataGeneratorInterface::StopAcquisition();
				InternalStopSyncstationDataTransfer();
			}
			LOG_EXITING;
		}

		void DataGeneratorSyncstation::CloseDevice(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				InternalStopSyncstationDataTransfer();
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

		void DataGeneratorSyncstation::RaiseTrigger(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				mMustRaiseTrigger = true;
			}
			LOG_EXITING;
		}

		void DataGeneratorSyncstation::FallTrigger(void)
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


		int DataGeneratorSyncstation::ReadDeviceID(UCHAR* dev_id)
		{
			if (mDeviceHandle == INVALID_HANDLE_VALUE)
				return -5;
			return 0;
		}

		int DataGeneratorSyncstation::GetFirmwareVersion(String^% version)
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

		array<Byte>^ DataGeneratorSyncstation::CreateRaiseTriggerCommand()
		{
			return CreateStartStopCommands(true, true);
		}

		array<Byte>^ DataGeneratorSyncstation::CreateFallTriggerCommand()
		{
			return CreateStartStopCommands(true, false);
		}

		array<Byte>^ DataGeneratorSyncstation::CreateStartCommand()
		{
			return CreateStartStopCommands(true, false);
		}

		array<Byte>^ DataGeneratorSyncstation::CreateStopCommand()
		{
			/*
			array<Byte>^ command = gcnew array<Byte>(2); //totaldevices + start + crc8
			command[0] = 0; //stop
			command[1] = ComputeCRC(command, 2);;
			return command;*/
			return CreateStartStopCommands(false, false);
		}

		array<Byte>^ DataGeneratorSyncstation::CreateStartStopCommands(bool Start, bool TriggerOn)
		{
			int totaldevices = 0; 
			for (int i = 0; i < GetNumberConnectorsIn(); i++) {
				totaldevices = totaldevices + deviceEN[i];
			}
			/*
			totaldevices = MUOVI1 + MUOVI2 + MUOVI3 + MUOVI4 + 
				SESSANTAQUATTRO1 + SESSANTAQUATTRO2 + 
				DUEPROBE1 + DUEPROBE2 + DUEPROBE3 + DUEPROBE4 + DUEPROBE5 + DUEPROBE6 + DUEPROBE7 + DUEPROBE8 + DUEPROBE9 + DUEPROBE10;
			*/

			array<Byte>^ command = gcnew array<Byte>(totaldevices+2); //totaldevices + start + crc8
			if (Start) {
				command[0] = (byte)totaldevices << 1 | 1; //start byte
			}
			else {
				command[0] = (byte)totaldevices << 1 | 0; //stop byte
			}

			int commandIndex = 1;
			for (int i = 0; i < GetNumberConnectorsIn(); i++) {
				if (deviceEN[i] == 1) {					
					if (Start) {
						
						command[commandIndex] = i << 4 | //device
							EMG[i] << 3 | //Fsamp 
							mode[i] << 1 | //modality
							1; // enable data transfer (deviceEN[i])
						commandIndex++;
					}
					else {
						command[commandIndex] = i << 4 | //device
							EMG[i] << 3 | //Fsamp 
							0 << 1 | //modality
							0; // enable data transfer (deviceEN[i])
						commandIndex++;					
					}
				}				
			}
			
			//AGGIUNGERE SONDE DUE 
			command[commandIndex] = ComputeCRC(command, totaldevices + 1 ); //startcommand included in length 
			totalDevices = totaldevices;
			return command;
		}
		
		array<Byte>^ DataGeneratorSyncstation::CreateLatencyCommand()
		{

			array<Byte>^ command = gcnew array<Byte>(3); //totaldevices + start + crc8
			
			command[0] = 1 << 7 | //START BYTE B
				1 << 1| 0; //start byte
			
			int timeWindow;
			int::TryParse(Latency, timeWindow);
			command[1] = timeWindow;

			//AGGIUNGERE SONDE DUE 
			command[2] = ComputeCRC(command, 2); //startcommand included in length 
			return command;
		}

		Byte DataGeneratorSyncstation::ComputeCRC(array<Byte>^ command, unsigned int Len)
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

		void DataGeneratorSyncstation::OuternThread()
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
				
				/*array<Byte>^ raisetriggercommand = CreateRaiseTriggerCommand();
				pin_ptr<Byte> RaiseTriggerCommandPinned = &raisetriggercommand[0];   // entire array is now pinned  
				mDataReaderInterfaceHelper->mRaiseTriggerCommand = (LPARAM)RaiseTriggerCommandPinned;
				array<Byte>^ falltriggercommand = CreateFallTriggerCommand();
				pin_ptr<Byte> FallTriggerCommandPinned = &falltriggercommand[0];   // entire array is now pinned  
				mDataReaderInterfaceHelper->mFallTriggerCommand = (LPARAM)FallTriggerCommandPinned;
				*/

				//Assegno devices
				for (int i = 0; i < 22; i++) {
					mDataReaderInterfaceHelper->devices[i].enable = devices[i]->enable;
					mDataReaderInterfaceHelper->devices[i].mode = devices[i]->mode;
					mDataReaderInterfaceHelper->devices[i].nChannel = devices[i]->nChannel;
					mDataReaderInterfaceHelper->devices[i].nSample = devices[i]->nSample;
				}

				SendLatencyCommand();

				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoSyncstation,  // Thread Func
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

		void DataGeneratorSyncstation::SendLatencyCommand() 
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
					array<Byte>^ command = CreateLatencyCommand();
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

					int totaldevices = 0;
					for (int i = 0; i < GetNumberConnectorsIn(); i++) {
						totaldevices = totaldevices + deviceEN[i];
					}

					BOOL WriteFileRes = WriteFile(mDeviceHandle, (LPCVOID)&CommandPinned[0], 3, &numberOfBytesTransferred, OverlappedStruc);

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

		void DataGeneratorSyncstation::InternalStopSyncstationDataTransfer(void)
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

					int totaldevices = 0;
					for (int i = 0; i < GetNumberConnectorsIn(); i++) {
						totaldevices = totaldevices + deviceEN[i];
					}

					BOOL WriteFileRes = WriteFile(mDeviceHandle, (LPCVOID)&CommandPinned[0], totaldevices+2, &numberOfBytesTransferred, OverlappedStruc);

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

		void DataGeneratorSyncstation::CloseConnection()
		{
			DebugLog::Print(String::Format("Connection closed"));
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				closesocket((SOCKET)mDeviceHandle);
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
			}
		}

		int DataGeneratorSyncstation::WriteString(String^ command)
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

		int DataGeneratorSyncstation::ReadString(String^ %read)
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
		DataValidator^ DataGeneratorSyncstation::DefaultTriggerValidator::get()
		{
			return mDefaulDataValidatorTrigger;
		}
#pragma endregion 

#pragma region managed code

#pragma managed

		union SwapShort
		{
			unsigned char Bytes[2];
			unsigned short Short;
		};

		ULONG _stdcall AsyncIoSyncstation(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ASYNCH_IO = 10;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			unsigned int NumBytesPerSample = 2;// helper->mSource[0].mBytesPerChannel; // sizeof(__int16);

			//if at least one device is at 500Hz-24 bit I set 3 as numbyte per sample for all device
			
			for (int dev = 0; dev < 22; dev++) {
				if (helper->devices[dev].enable) {
					if (helper->devices[dev].nSample == 500) {
						NumBytesPerSample = 3;
						break;
					}
				}
			}

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
			unsigned __int32 *DecodedBuffer32 = NULL;
			unsigned __int32 *DecodedBufferPos32 = NULL;
			unsigned __int32 *DecodedBufferCurrentStart32 = NULL;

			/*
			ULONG DecodedBufferSizeUnitaryTrigger = 4 * helper->mSource[0].mDataRegroupFactor;// added quality control
			ULONG DecodedBufferSizeTrigger = DecodedBufferSizeUnitaryTrigger * NUM_DECODED_BUFFERS;
			ULONG DecodedBufferSizeInBytesTrigger = DecodedBufferSizeTrigger * NumBytesPerSample;
			unsigned short *DecodedBufferTrigger;
			unsigned short *DecodedBufferPosTrigger;
			unsigned short *DecodedBufferCurrentStartTrigger;
			unsigned int channelTrigger = helper->mDeviceTotalChannels - 7; // il canale del trigger è sempre il secondo controllo, quindi totale meno 7
			unsigned int channelQuality = helper->mDeviceTotalChannels - 6; // il canale del controllo qualità è sempre il terzo controllo, quindi totale meno 6
			*/
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
				


			/***************************************************/
			/*
			int nchannel = 0;
			//allocate the array
			int** channelSample = new int*[helper->mDeviceTotalChannels];
			for (int i = 0; i < helper->mDeviceTotalChannels; i++)
				channelSample[i] = new int[1];*/
			
			/***************************************************/



			if (NumBytesPerSample == 2) {
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
			}
			else {
				int nbyte = 0;
				int decodedNByte = helper->mDeviceTotalChannels*4;
				int k = 0;
				for (int dev = 0; dev < 22; dev++) {
					if (helper->devices[dev].enable) {
						if (helper->devices[dev].nSample == 500) {
							nbyte += 3* helper->devices[dev].nChannel;
							/*for (int nc = 0; nc < helper->devices[dev].nChannel; nc++) {
								*channelSample[k] = 500;
								k++;
							}*/
						}
						else {
							nbyte += 2 * helper->devices[dev].nChannel;
							/*for (int nc = 0; nc < helper->devices[dev].nChannel; nc++) {
								*channelSample[k] = 2000;
								k++;
							}*/
						}
					}
				}

				SingleAcquisitionBufferSizeInBytes = nbyte*helper->mSource[0].mDataRegroupFactor;
				TempBufferSizeInBytes = nbyte*helper->mSource[0].mDataRegroupFactor;
				DecodedBufferSizeInBytes = decodedNByte*helper->mSource[0].mDataRegroupFactor*NUM_DECODED_BUFFERS;

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
					WSACleanup();
					return -1;
				}
				DecodedBufferPos32 = DecodedBuffer32;
				DecodedBufferCurrentStart32 = DecodedBufferPos32;
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

			//decoded buffer for trigger
			/*
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

			*/


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
				LocalFree((void *)DecodedBuffer32);
				DecodedBuffer32 = NULL;
				//LocalFree((void *)DecodedBufferTrigger);
				//DecodedBufferTrigger = NULL;

				return -1;
			}
			ZeroMemory(OverlappedStructWrite, sizeof(OVERLAPPED));

			SetLastError(0);
			/*
			LOG_ENTERING;
			if ((SOCKET)helper->mDeviceHandle != INVALID_SOCKET)
			{
				// Stop acquisition
				try
				{
					//WriteString("stop");
					LOG_ENTERING; \
						if ((SOCKET)helper->mDeviceHandle == INVALID_SOCKET)
						{
							InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
							throw ihe;
						}
					DWORD res = 0;
					array<Byte>^ command;
					pin_ptr<Byte> CommandPinned = &command[0];   // entire array is now pinned  
																 //IntPtr ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(command);
																 //const char* buf = (LPCSTR)command[0].ToPointer();
					DWORD numberOfBytesTransferred;
					OVERLAPPED *OverlappedStruc;
					OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
					if (OverlappedStruc == NULL)
					{
						OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
						throw ose;
					}
					ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
					//OverlappedStruc->Internal = STATUS_PENDING;

					SetLastError(0);


					BOOL WriteFileRes = WriteFile(helper->mDeviceHandle, (LPCVOID)&CommandPinned[0], 3, &numberOfBytesTransferred, OverlappedStruc);

					if (WriteFileRes == FALSE)
					{
						if (GetLastError() == ERROR_IO_PENDING)
						{
							if (!GetOverlappedResult(helper->mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, TRUE))
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
					
					return;
				}
				catch (Exception ^e)
				{

				}
			}
			*/
			ZeroMemory(OverlappedStructWrite, sizeof(OVERLAPPED));

			SetLastError(0);

			BOOL WriteFileRes = WriteFile(helper->mDeviceHandle, StartCommand, (totalDevices+2), &numberOfBytesTransferred, OverlappedStructWrite);
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
						if (NumBytesPerSample == 2) //CASE OF SYNCSTATION WITH NO 500Hz-24bit DEVICE CONNECTED
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
								int index = 0;
								for (int dev = 0; dev < 22; dev++) {
									if (helper->devices[dev].enable) {
										if (helper->devices[dev].nSample == 2000) {
											
											for (int nch = 0; nch < helper->devices[dev].nChannel; nch++) {
																								
												int pos = index;
												UCHAR B0 = TempBufferAsInt32[pos];
												UCHAR B1 = TempBufferAsInt32[pos+1];
												/*
												if (B0 & 0x80)
													*DecodedBufferPos32 = (0xFF << 24) | (0 << 16) | (B0 << 8) | B1;// TempBufferAsShort[ChannelMappings[channel]];
												else
													*DecodedBufferPos32 = (0 << 16) | (B0 << 8) | B1;// TempBufferAsShort[ChannelMappings[channel]];
												*/
												if (B0 & 0x80)
													*DecodedBufferPos32 = (0xFF << 24) | (0xFF << 16) | (B0 << 8) | B1;// TempBufferAsShort[ChannelMappings[channel]];
												else
													*DecodedBufferPos32 = (B0 << 8) | B1;


												DecodedBufferPos32++;

												index+=2;
											}
											//TempBufferAsInt32 += helper->devices[dev].nChannel * 2;

										}
										else if (helper->devices[dev].nSample == 500) {
											for (int nch = 0; nch < helper->devices[dev].nChannel; nch++) {
												
												//int posMappedChannel = index;
												size_t posMappedChannel = index;
												//add control for buffer overflow
												unsigned __int32 B0 = TempBufferAsInt32[posMappedChannel];
												unsigned __int32 B1 = TempBufferAsInt32[posMappedChannel + 1];
												unsigned __int32 B2 = TempBufferAsInt32[posMappedChannel + 2];

												if (B0 & 0x80)
													*DecodedBufferPos32 = 0xFF000000 | (((B0) << 16) | (B1 << 8) | B2);// TempBufferAsShort[ChannelMappings[channel]];
												else
													*DecodedBufferPos32 = (B0 << 16) | (B1 << 8) | B2;// TempBufferAsShort[ChannelMappings[channel]];

												/* ORIGINAL CONVERSION
												UCHAR B0 = TempBufferAsInt32[posMappedChannel];
												UCHAR B1 = TempBufferAsInt32[posMappedChannel + 1];
												UCHAR B2 = TempBufferAsInt32[posMappedChannel + 2];

												if (B0 & 0x80)
													*DecodedBufferPos32 = (0xFF << 24) | ((B0) << 16) | (B1 << 8) | B2;// TempBufferAsShort[ChannelMappings[channel]];
												else
													*DecodedBufferPos32 = (B0 << 16) | (B1 << 8) | B2;// TempBufferAsShort[ChannelMappings[channel]];
												*/
												
												DecodedBufferPos32++;
												index += 3;

											}
										}
									}
								}
								TempBufferAsInt32 += index;
							}
														
							helper->mNewDataAvailable(0, DataSize::ThirtytwoBits, DecodedBufferCurrentStart32, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);
							
							DecodedBufferCurrentStart32 = DecodedBufferPos32;
							
							if (DecodedBufferCurrentStart32 >= DecodedBufferSize + DecodedBuffer32)
								DecodedBufferCurrentStart32 = DecodedBuffer32;

							/*					
							***********************************************************************************/
							/*
							DecodedBufferPos32 = DecodedBufferCurrentStart32;
							UCHAR *TempBufferAsInt32 = (UCHAR*)TempBuffer;

							for (unsigned int koffset = 0; koffset < helper->mSource[0].mDataRegroupFactor; koffset++)
							{
								int index = 0;
								for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
								{
									if (*channelSample[channel] == 2000) {

										int pos = index;
										unsigned __int32 B0 = TempBufferAsInt32[pos]*256;
										unsigned __int32 B1 = TempBufferAsInt32[pos + 1];
										*DecodedBufferPos32 = B0 + B1;
										if (*DecodedBufferPos32 >= 32768) {
											*DecodedBufferPos32 = *DecodedBufferPos32 - 65536;
										}
										index += 2;
									}
									else if(*channelSample[channel] == 500){

										int pos = index;
										unsigned __int32 B0 = TempBufferAsInt32[pos] * 65536;
										unsigned __int32 B1 = TempBufferAsInt32[pos + 1] * 256;
										unsigned __int32 B2 = TempBufferAsInt32[pos + 2];

										*DecodedBufferPos32 = B0 + B1 + B2;
										if (*DecodedBufferPos32 >= 8388608) {
											*DecodedBufferPos32 = *DecodedBufferPos32 - 16777216;
										}
										
										//size_t posMappedChannel = 3 * channel;
										//unsigned __int32 B0 = TempBufferAsInt32[posMappedChannel];
										//unsigned __int32 B1 = TempBufferAsInt32[posMappedChannel + 1];
										//unsigned __int32 B2 = TempBufferAsInt32[posMappedChannel + 2];
										

										//if (B0 & 0x80)
											//*DecodedBufferPos32 = (0xFF << 24) | (B0 << 16) | (B1 << 8) | B2;// TempBufferAsShort[ChannelMappings[channel]];
										//else
											//*DecodedBufferPos32 = (B0 << 16) | (B1 << 8) | B2;// TempBufferAsShort[ChannelMappings[channel]];
										index += 3;

									}
									DecodedBufferPos32++;
								}
								//TempBufferAsInt32 += helper->mDeviceTotalChannels * 3;
							}
							helper->mNewDataAvailable(0, DataSize::ThirtytwoBits, DecodedBufferCurrentStart32, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);
							DecodedBufferCurrentStart32 = DecodedBufferPos32;
							if (DecodedBufferCurrentStart32 >= DecodedBufferSize + DecodedBuffer32)
								DecodedBufferCurrentStart32 = DecodedBuffer32;
							*/
							//*******************************************************************************
							/*
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
									
									DecodedBufferPos32++;
								}
								TempBufferAsInt32 += helper->mDeviceTotalChannels * 3;
							}
							helper->mNewDataAvailable(0, DataSize::ThirtytwoBits, DecodedBufferCurrentStart32, nullptr, helper->mSource[0].mAcquiredChannels, 0, helper->mSource[0].mDataRegroupFactor, 0);
							DecodedBufferCurrentStart32 = DecodedBufferPos32;
							if (DecodedBufferCurrentStart32 >= DecodedBufferSize + DecodedBuffer32)
								DecodedBufferCurrentStart32 = DecodedBuffer32;
														
							*/

							




							//*******************************************************************************
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
			if (NumBytesPerSample == 2) {
				if (DecodedBuffer)
				{
					LocalFree((void *)DecodedBuffer);
					DecodedBuffer = NULL;
				}
			}
			else {
				if (DecodedBuffer32)
				{
					LocalFree((void *)DecodedBuffer32);
					DecodedBuffer32 = NULL;
				}
			}

			/*if (DecodedBufferTrigger)
			{
				LocalFree((void *)DecodedBufferTrigger);
				DecodedBufferTrigger = NULL;
			}*/


			helper->mStopAcquisition();
			return 0;
		}
#pragma endregion


	}

}