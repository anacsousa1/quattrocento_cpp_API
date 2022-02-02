#include "DataGeneratorTrentadue.h"
#include "stdio.h"

using namespace System::Xml;
namespace OTComm {
	namespace Trentadue {
#pragma comment(lib, "Ws2_32.lib")

#pragma managed

#pragma region Constructors and destructors
		DataGeneratorTrentadue::DataGeneratorTrentadue(AdapterTrentadue^ adapter, AUXType aux, Frequency SamplingFrequency) :
			DataGeneratorInterface(), mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false),
			mDataReaderInterfaceHelper(NULL), mUseAux(aux)
		{			
			try
			{
				mConnectors = gcnew array<GenericConnector^>(1);
				mConnectors[0] = gcnew GenericConnector(adapter->MaxNumChannels);// mDeviceTotalChannels
				mConnectors[0]->Adapter = adapter;
				mDeviceTotalChannels = GetNumberTotalChannels();
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
				if (mConnectors[0]->Adapter->ID == "AD1x32TR")
					mModel = Trentadue::Models::TrentadueMonopolar_32;
				else if (mConnectors[0]->Adapter->ID == "AD2x16TR")
					mModel = Trentadue::Models::TrentadueMonopolar_32;
				else if (mConnectors[0]->Adapter->ID == "AD1x16TR")
					mModel = Trentadue::Models::TrentadueMonopolar_16;
				else if (mConnectors[0]->Adapter->ID == "AD16x1TR_WIRE")
					mModel = Trentadue::Models::TrentadueBipolar_16_Wire;
				else if (mConnectors[0]->Adapter->ID == "AD16x1TR_JACK")
					mModel = Trentadue::Models::TrentadueBipolar_16_Jack;
				else if (mConnectors[0]->Adapter->ID == "AD8x1TR")
					mModel = Trentadue::Models::TrentadueBipolar_8;
				mSources = gcnew array<DataGeneratorSingleSource^>(1);
				mSources[0] = gcnew DataGeneratorSingleSource(SamplingFrequency, 2, GetChannelsMapping());
			}
			catch (Exception ^e)
			{
				ConstructorFailedException^ cfe = gcnew ConstructorFailedException(__FUNCTION__, e);
				throw cfe;
			}
		}

		DataGeneratorTrentadue::~DataGeneratorTrentadue()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorTrentadue();
		}

		DataGeneratorTrentadue::!DataGeneratorTrentadue()
		{

		}

#pragma endregion

#pragma region XML files
		String^ DataGeneratorTrentadue::GetDeviceXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			String^ Firmware = "Unknown";
			try
			{
				GetFirmwareVersion(Firmware);
			}
			catch(Exception^){}
				

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", GetNumberTotalChannels().ToString());// (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}			mXMLTextStream->WriteAttributeString("AUX", mUseAux.ToString());
			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
			mXMLTextStream->WriteAttributeString("ad_bits", "16");
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
					mConnectors[k]->Adapter->WriteXMLConfiguration(mXMLTextStream, k, startIndex, 0);

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

		String^ DataGeneratorTrentadue::GetSavingXMLConfiguration()
		{
			LOG_ENTERING;
			OpenXMLConfigStream(false);
			//if (GetAcquiredChannels() == nullptr) return"";
			String^ Firmware;
			if (GetFirmwareVersion(Firmware) <= 0)
				Firmware = "Unknown";

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}
			mXMLTextStream->WriteAttributeString("AUX", mUseAux.ToString());
			mXMLTextStream->WriteAttributeString("Firmware", Firmware);
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
					mConnectors[k]->Adapter->WriteSavingXMLConfiguration(mXMLTextStream, k, startIndex, 0);

					for (int zz = 0; zz < mConnectors[k]->Adapter->Channels->Length; zz++)
						if (mConnectors[k]->Adapter->Channels[zz] != nullptr)
							startIndex++;
				}

				//ChannelNum += mConnectors[k]->NumConnectableChannels;
			}
			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			LOG_EXITING;
			return CloseXMLConfigStream();
		}


		DataGeneratorTrentadue^ DataGeneratorTrentadue::FromXMLConfiguration(String^ Filename)
		{
			LOG_ENTERING;
			DataGeneratorTrentadue^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try{
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				unsigned int deviceModel = System::Convert::ToUInt16(DeviceNode->Attributes["Model"]->Value);
				unsigned int deviceFrequency = System::Convert::ToUInt16(DeviceNode->Attributes["SampleFrequency"]->Value);
				unsigned int aux = System::Convert::ToUInt16(DeviceNode->Attributes["AUX"]->Value);

				if (deviceName->Contains("TRENTADUE"))
				{
					System::Xml::XmlNode^ ChannelsNode = DeviceNode->ChildNodes[0];
					for (int ii = 0; ii < ChannelsNode->ChildNodes->Count; ii++)
					{
						System::Xml::XmlNode^ AdapterNode = ChannelsNode->ChildNodes[ii];
						/*DebugLog::Print(AdapterNode->Attributes["ID"]->Value->ToString());
						DebugLog::Print(Assembly::GetExecutingAssembly()->ToString());*/
						AdapterTrentadue^ ad = GetAdapterFromID(AdapterNode->Attributes["ID"]->Value->ToString());
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
						gdr = gcnew  OTComm::Trentadue::DataGeneratorTrentadue(ad, aux, deviceFrequency);
					}
				}
				textReader->Close();
			}
			catch (Exception^ ex) {
				textReader->Close();
				throw "Error in FromXMLConfiguration";
			}
			LOG_EXITING;
			return gdr;
		}
#pragma endregion

#pragma region Connectors and adapters

		AdapterTrentadue^ DataGeneratorTrentadue::GetAdapterFromID(String^ adapterID)
		{
			if (adapterID == ("AD8x1TR"))
				return gcnew OTComm::Trentadue::AdapterAD8x1TR();
			if (adapterID == ("AD16x1TR_WIRE"))
				return gcnew OTComm::Trentadue::AdapterAD16x1TR_WIRE();
			if (adapterID == ("AD16x1TR_JACK"))
				return gcnew OTComm::Trentadue::AdapterAD16x1TR_JACK();
			if (adapterID == ("AD1x16TR"))
				return gcnew OTComm::Trentadue::AdapterAD1x16TR();
			if (adapterID == ("AD2x16TR"))
				return gcnew OTComm::Trentadue::AdapterAD2x16TR();
			if (adapterID == ("AD1x32TR"))
				return gcnew OTComm::Trentadue::AdapterAD1x32TR();
			return nullptr;
		}

		GenericChannel^ DataGeneratorTrentadue::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
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
			if (channelID == ("ELSCH064R3S"))
				return gcnew OTComm::Sensors::SensorELSCH064R3S(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("ELSCH064NM5"))
				return gcnew OTComm::Sensors::SensorELSCH064NM5(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
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
			if (channelID == ("AUX  Acceleration"))
				return gcnew OTComm::Sensors::AUXAcc(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Torque"))
				return gcnew OTComm::Sensors::AUXTor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("AUX  Force"))
				return gcnew OTComm::Sensors::AUXFor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			return nullptr;
		}


		unsigned int DataGeneratorTrentadue::GetNumberTotalChannels()
		{
			return mModel%10000;
		}

		unsigned int DataGeneratorTrentadue::GetNumberNonAuxChannels()
		{
			return 0;// (((unsigned int)mModel) % 10000);//TODO: inserire in base al tipo di adapter? aggiungere agli adapter un membro "numero non aux"?
		}

		unsigned int DataGeneratorTrentadue::GetNumberAuxChannels()
		{
			return 0;//TODO
		}

		array<unsigned int>^ DataGeneratorTrentadue::GetAcquiredChannels()
		{
			return mAcquiredChannelsIndices;
		}

		array<unsigned int>^ DataGeneratorTrentadue::CreateChannelsMapping()
		{
			return gcnew array<unsigned int>(32)
			{
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
					20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
			};
		}

#pragma endregion

#pragma region Device properties 

		Bitmap^ DataGeneratorTrentadue::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("32.jpg", true);
		}

		unsigned int DataGeneratorTrentadue::GetDeviceModel()
		{
			return (unsigned int)mModel;//TODO
		}


		String^ DataGeneratorTrentadue::GetDeviceDescription(void)
		{
			String^ res = gcnew String("TRENTADUE");			
			return res;
		}

		String^ DataGeneratorTrentadue::IPAdress::get()
		{
			String^ tmp = gcnew String(mIPAdress);
			return tmp;
		}

		int DataGeneratorTrentadue::PortNumber::get()
		{
			return mPortNumber;
		}

		AUXType DataGeneratorTrentadue::AuxType::get()
		{
			return mUseAux;
		}
#pragma endregion

#pragma region Public device operations
		
		void DataGeneratorTrentadue::OpenDevice(void)
		{
			LOG_ENTERING;
			IntPtr ptrtxt = IntPtr::Zero;
			try
			{
				WSADATA wsaData = { 0 };
				if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
				{
					throw gcnew WSAException(__FUNCTION__, "WSAStartup");
				}
				if ((mDeviceHandle = (HANDLE)WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED)) == (HANDLE)INVALID_SOCKET)
				{
					throw  gcnew WSAException(__FUNCTION__, "WSASocket");
				}
				else
				{
					struct sockaddr_in address;  /* the libc network address data structure */

					ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(mIPAdress);

					address.sin_addr.s_addr = inet_addr((LPCSTR)ptrtxt.ToPointer()); /* assign the address */
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
					address.sin_port = htons(mPortNumber);            /* translate int2port num */
					address.sin_family = AF_INET;

					if (SOCKET_ERROR == WSAConnect((SOCKET)mDeviceHandle, (SOCKADDR*)(&address), sizeof(address), NULL, NULL, NULL, NULL))
					{
						throw  gcnew WSAException(__FUNCTION__, "WSAConnect");
					}

					mDeviceIsOpen = true;
					DataGeneratorInterface::OpenDevice();
				}
			}
			catch (Exception^e)
			{
				OpenDeviceFailedException^ idfe = gcnew OpenDeviceFailedException(__FUNCTION__, e);
				FireOnError(idfe);
				throw e;
			}
			LOG_EXITING;
		}

		void DataGeneratorTrentadue::InitializeDevice()
		{
			LOG_ENTERING;				
			try
			{
				DataGeneratorInterface::InitializeDevice();
			}
			catch (Exception ^e)
			{
				InitializeDeviceFailedException^ idfe = gcnew InitializeDeviceFailedException(__FUNCTION__, e);
				FireOnError(idfe);
				throw idfe;
			}
			LOG_EXITING;
		}


		void DataGeneratorTrentadue::StartAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				DataGeneratorInterface::StartAcquisition();
				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorTrentadue::OuternThread));
				mManagedAcquisitionThread->Start();				
			}
			catch(Exception ^e)
			{
				StartAcquisitionFailedException^ cdfe = gcnew StartAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(cdfe);
				throw cdfe;
			}
			LOG_EXITING;
		}

		void DataGeneratorTrentadue::StopAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				DataGeneratorInterface::StopAcquisition();
				InternalStopTrentadueDataTransfer();
			}
			catch(Exception ^e)
			{
				StopAcquisitionFailedException^ cdfe = gcnew StopAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(cdfe);
				throw cdfe;
			}
			LOG_EXITING;
		}

		void DataGeneratorTrentadue::CloseDevice(void)
		{
			LOG_ENTERING;
			try
			{
				if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				//InternalStopTrentadueDataTransfer();
				closesocket((SOCKET)mDeviceHandle);
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
				mDeviceIsOpen = false;
				LOG_SUCCESS;
				DataGeneratorInterface::CloseDevice();
			}
			catch (Exception ^e)
			{
				CloseDeviceFailedException^ cdfe = gcnew CloseDeviceFailedException(__FUNCTION__, e);
				FireOnError(cdfe);
				throw cdfe;
			}
			LOG_EXITING;
		}


#pragma endregion

#pragma region Non public device operations

		void DataGeneratorTrentadue::CloseConnection()
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle == INVALID_SOCKET)
			{
				throw gcnew InvalidHandleException(__FUNCTION__);
			}
			try
			{
				closesocket((SOCKET)mDeviceHandle);
				mDeviceHandle = (HANDLE)INVALID_SOCKET;
				mDeviceIsOpen = false;
				LOG_SUCCESS;
			}
			catch (Exception^)
			{
				//TODO completare
			}
			LOG_EXITING;
		}

		void DataGeneratorTrentadue::OuternThread()
		{
			LOG_ENTERING;
			try
			{
				mDeviceTotalChannels = GetNumberTotalChannels();
				DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				mDataReaderInterfaceHelper->mAuxData = mUseAux;
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				HANDLE AsyncIoThreadHandle = CreateThread(NULL, 0, AsyncIoTrentadue, mDataReaderInterfaceHelper, 0, NULL);
				if (AsyncIoThreadHandle == NULL)
				{
					throw gcnew ThreadCreationException(__FUNCTION__);
				}
				WaitForSingleObject(AsyncIoThreadHandle, INFINITE);
			}
			catch (Exception ^e)
			{
				AcquisitionThreadException^ ate = gcnew AcquisitionThreadException(__FUNCTION__, e);
				FireOnError(ate);
			}
			LOG_EXITING;
		}


		void DataGeneratorTrentadue::InternalStopTrentadueDataTransfer(void)
		{
			LOG_ENTERING;
			if ((SOCKET)mDeviceHandle != INVALID_SOCKET)
			{
				// Stop acquisition
				if (!WriteString("stop"))
				{
					CloseConnection();
					StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__);
					FireOnError(safe);
					throw safe;
				}
			}
			LOG_EXITING;
		}

		UCHAR DataGeneratorTrentadue::ReadDeviceID(void)
		{
			//TODO completare!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if (mDeviceHandle == INVALID_HANDLE_VALUE)
			{
				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
				FireOnError(ihe);
				throw ihe;
			}
			return 0;
		}

		int DataGeneratorTrentadue::WriteString(String^ command)
		{
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
						OverlappedCommunicationException^ oce = gcnew OverlappedCommunicationException(__FUNCTION__);
						FireOnError(oce);
						throw oce;
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
					OverlappedCommunicationException^ oce = gcnew OverlappedCommunicationException(__FUNCTION__);
					FireOnError(oce);
					throw oce;
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
			return res;
		}

		int DataGeneratorTrentadue::ReadString(String^ %read)
		{
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
						FireOnError("problem reading", GetLastError(), __FUNCTION__);
						LOG_ERROR("problem reading");
						LocalFree((void *)OverlappedStruc);
						OverlappedStruc = NULL;
						return -1;
					}






					/*if (!GetOverlappedResultEx(mDeviceHandle, OverlappedStruc, &numberOfBytesTransferred, 1000, true))
					{
						FireOnError("problem reading", GetLastError(), __FUNCTION__);
						LOG_ERROR("problem reading");
						LocalFree((void *)OverlappedStruc);
						OverlappedStruc = NULL;
						return -1;
					}*/
					else
						Sleep(10);
				}
				else
				{
					FireOnError("problem reading", GetLastError(), __FUNCTION__);
					LOG_ERROR("problem reading");
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
		}

		int DataGeneratorTrentadue::GetFirmwareVersion(String^% version)
		{
			LOG_ENTERING;
			int ver = 0;
			if (WriteString("firm") > 0)
			{
				ver = ReadString(version);
			}
			LOG_EXITING;
			return ver;
		}



#pragma endregion

#pragma region Unmanaged code

#pragma unmanaged

		ULONG _stdcall AsyncIoTrentadue(PVOID  ThreadParameter)
		{
			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ASYNCH_IO=10;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;

			ULONG DecodedBufferSize = helper->mSource[0].mAcquiredChannels * NUM_DECODED_BUFFERS * helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize*sizeof(__int16);
			//volatile LONG *RecordingSamples;
			unsigned short *DecodedBuffer;
			unsigned short *DecodedBufferPos;
			unsigned short *DecodedBufferEnd;
			UCHAR * AcquisitionBuffer;

			ULONG numberOfBytesTransferred;

			char Settings[] = { "SET:0 0 0 0 0 0" };
			unsigned int Channels = helper->mDeviceTotalChannels / 16; //0 for 8, 1 for 16, 2 for 32;
			unsigned int Frequency = helper->mSource[0].mSamplingFrequency == 2000 ? 3 : helper->mSource[0].mSamplingFrequency / 500;
			unsigned int Resolution = 1;
			unsigned int adapter = helper->mDeviceModel / 10000;
			unsigned int aux = helper->mAuxData;
			if (aux == 1)
			{
				if (helper->mDeviceTotalChannels == 8)
					aux = 3;
				else
					aux = 4;
			}
			else if (aux == 2)
				aux = 5;

			sprintf_s(Settings, "SET:%d %d %d %d %d %d", Channels, Frequency, Resolution, adapter, aux, aux == 0 ? 0 : helper->mDeviceTotalChannels);
			OVERLAPPED *OverlappedStrucTmp;
			OverlappedStrucTmp = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			if (OverlappedStrucTmp == NULL)
			{
				return -1;
			}
			ZeroMemory(OverlappedStrucTmp, sizeof(OVERLAPPED));
			//OverlappedStrucTmp->Internal = STATUS_PENDING;
			SetLastError(0);
			BOOL WriteFileRes = WriteFile(helper->mDeviceHandle, Settings, (DWORD)strlen(Settings), &numberOfBytesTransferred, OverlappedStrucTmp);
			int res = 0;
			if (WriteFileRes == FALSE)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					DWORD dwResult = WaitForSingleObject(OverlappedStrucTmp->hEvent, 1000);
					if (dwResult != WAIT_OBJECT_0 ||
						!::GetOverlappedResult(helper->mDeviceHandle, OverlappedStrucTmp, &numberOfBytesTransferred, TRUE)
						)
					{
						res = -1;
					}
					//if (!GetOverlappedResultEx(helper->mDeviceHandle, OverlappedStrucTmp, &numberOfBytesTransferred, 1000, false)) //true
					//{
					//	res = -1;
					//}
				}
				else
				{
					//DebugLog::Print("error in writing: "+GetLastError());
					//Sleep(10);
					res = -1;
				}

				
			}
			else
				Sleep(10);
			LocalFree((void *)OverlappedStrucTmp);
			OverlappedStrucTmp = NULL;
			if (res != 0)
			{
				//DebugLog::Print("DecodedBuffer is null");
				return -1;
			}





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
			ULONG SingleAcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels*helper->mSource[0].mDataRegroupFactor*sizeof(char)*helper->mSource[0].mBytesPerChannel;

			AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, SingleAcquisitionBufferSizeInBytes*NUM_ASYNCH_IO);

			if (AcquisitionBuffer == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				//DebugLog::Print("Acquisition buffer is null");
				return -1;
			}
			else
				;//DebugLog::Print("Acquisition buffer is OK");



			DecodedBuffer = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
			if (DecodedBuffer == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				return -1;
			}

			OverlappedStrucTmp = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
			if (OverlappedStrucTmp == NULL)
			{
				return -1;
			}
			ZeroMemory(OverlappedStrucTmp, sizeof(OVERLAPPED));
			//OverlappedStrucTmp->Internal = STATUS_PENDING;
			SetLastError(0);
			WriteFileRes = WriteFile(helper->mDeviceHandle, "start\0", 6, &numberOfBytesTransferred, OverlappedStrucTmp);
			res = 0;
			if (WriteFileRes == FALSE)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					DWORD dwResult = WaitForSingleObject(OverlappedStrucTmp->hEvent, 1000);
					if (dwResult != WAIT_OBJECT_0 ||
						!::GetOverlappedResult(helper->mDeviceHandle, OverlappedStrucTmp, &numberOfBytesTransferred, TRUE)
						)
					{
						res = -1;
					}
					//if (!GetOverlappedResultEx(helper->mDeviceHandle, OverlappedStrucTmp, &numberOfBytesTransferred, 1000, false)) //true
					//{
					//	res = -1;
					//}
				}
				else
					Sleep(10);

				res = -1;
			}
			else
				Sleep(10);
			LocalFree((void *)OverlappedStrucTmp);
			OverlappedStrucTmp = NULL;
			if (res != 0)
			{
				//DebugLog::Print("DecodedBuffer is null");
				return -1;
			}


			//points to the beginning of the decoded buffer
			DecodedBufferPos = DecodedBuffer;
			DecodedBufferEnd = DecodedBuffer + DecodedBufferSize;

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;



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

				if (exitMainLoop)break;
				lettitot += numberOfBytesTransferred;
				size_t ReceivedBufferIndex = completedOv - OverlappedStruc;

				unsigned __int16 * DecodedBufferCurrentStart = DecodedBufferPos;
				unsigned __int16* AcquisitionBufferCurrentReadStart =
					(unsigned __int16*)&AcquisitionBuffer[ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes];




				int index = 0;
				for (unsigned int koffset = 0; koffset < helper->mSource[0].mDataRegroupFactor; koffset++)
				{
					//ULONG offset = koffset * helper->mDeviceTotalChannels;

					for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
					{
						//add control for buffer overflow
						*DecodedBufferPos = AcquisitionBufferCurrentReadStart[ChannelMappings[channel]];
						DecodedBufferPos++;
					}
					AcquisitionBufferCurrentReadStart += helper->mDeviceTotalChannels;
				}

				if (*mMustStopAcquisitionThread) break;

				helper->mNewDataAvailable(0, DataSize::SixteenBits, DecodedBufferCurrentStart, nullptr, helper->mSource[0].mAcquiredChannels,0,helper->mSource[0].mDataRegroupFactor,0);

				if (DecodedBufferPos == DecodedBufferEnd)
					DecodedBufferPos = DecodedBuffer;
				if (*mMustStopAcquisitionThread) break;

				BOOL ReadFileRes = ReadFile(helper->mDeviceHandle,
					AcquisitionBuffer + (ReceivedBufferIndex * SingleAcquisitionBufferSizeInBytes),
					SingleAcquisitionBufferSizeInBytes, NULL, completedOv);

				if (ReadFileRes == FALSE)
				{
					if (GetLastError() != ERROR_IO_PENDING)
					{
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

			if (DecodedBuffer)
			{
				LocalFree((void *)DecodedBuffer);
				DecodedBuffer = NULL;
			}
			helper->mStopAcquisition();
			return 0;
		}
#pragma endregion
	}
}