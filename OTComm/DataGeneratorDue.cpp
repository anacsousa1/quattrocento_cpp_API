
#include <Windows.h>
#include "DataConverter.h"
#include "DataGeneratorDue.h"
#include "DebugLog.h"
#include "ErrorDescription.h"
using namespace System::Runtime::InteropServices;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Xml;

namespace OTComm
{
	namespace Due
	{

#pragma managed

		/// <summary>
		/// Builder of the Due Generator of Data
		/// </summary>
		/// <param PortName="string containing the serial port name"></param>
		/// <param BaudRate="baud rate of communication"></param>
		/// <param DeviceTotalChannels="number of channel used"></param>
		DataGeneratorDue::DataGeneratorDue(String^ PortName, SerialBaudRate BaudRate) :
			DataGeneratorInterface(), mPortName(PortName), mBaudRate(BaudRate),
			mAsyncIoThreadHandle(NULL), mDeviceIsOpen(false), mDataReaderInterfaceHelper(NULL)
		{
			const int NumberOfSources = 7;
			const int ConnectorsPerSource = 2;
			mSources = gcnew array<DataGeneratorSingleSource^>(NumberOfSources);
			mDeviceTotalChannels = NumberOfSources*ConnectorsPerSource;
			for (int ii = 0; ii < mDeviceTotalChannels / 2; ii++)
			{
				mSources[ii] = gcnew DataGeneratorSingleSource(2048, 2, GetChannelsMapping());
			}
			int TotalConnectors = mDeviceTotalChannels;
			mConnectors = gcnew array<GenericConnector^>(TotalConnectors);
			for (int ii = 0; ii < TotalConnectors; ii++)
				mConnectors[ii] = gcnew GenericConnector(1, ii / 2);//TODO controllare se non sia meglio 4 connettori
		}

		unsigned int DataGeneratorDue::GetNumberConnectorsIn()
		{
			return mConnectors->Length;
		}

		unsigned int DataGeneratorDue::GetNumberConnectorsIn(const unsigned int source)
		{
			if (source >= mSources->Length)
				throw gcnew InvalidSourceIndex(__FUNCTION__);
			//each source has 2 connectors. TODO: this property should be read from the DataGeneratorSingleSource object
			return 2;
		}


		void DataGeneratorDue::ConnectIn(const unsigned int port, AdapterDue^ adapter)
		{
			if (port >= GetNumberConnectorsIn())
				throw gcnew InvalidConnectorNumberException(__FUNCTION__);
			mConnectors[port]->Adapter = adapter;
		}

		void DataGeneratorDue::ConnectIn(const unsigned int source, const unsigned int port, AdapterDue^ adapter)
		{
			if (source >= mSources->Length)
				throw gcnew InvalidSourceIndex(__FUNCTION__);
			if (port >= GetNumberConnectorsIn(source))
				throw gcnew InvalidConnectorNumberException(__FUNCTION__);
			mConnectors[source * 2 + port]->Adapter = adapter;
		}

		void DataGeneratorDue::ConnectIn(AdapterDue^ adapter)
		{
			mConnectors[0]->Adapter = adapter;
		}


		/// <summary>
		/// Get device description
		/// </summary>
		/// <returns>Returns description of the device</returns>
		String^ DataGeneratorDue::GetDeviceDescription(void)
		{
			return "DUE";
		}

		DataGeneratorDue^ DataGeneratorDue::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorDue^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try{
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;

				if (deviceName->Contains("DUE"))
				{
					gdr = gcnew OTComm::Due::DataGeneratorDue("", OTComm::Due::SerialBaudRates::BR_230400);
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
						gdr->ConnectIn(adPosition, (AdapterDue^)ad);
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

		GenericAdapter^ DataGeneratorDue::GetAdapterFromID(String^ adapterID)
		{
			if (adapterID == ("AdapterDue"))
				return gcnew OTComm::Due::AdapterDue();
			if (adapterID == ("AdapterAuxDirectIn"))
				return gcnew OTComm::Due::AdapterAuxDirectIn();
			if (adapterID == ("AdapterEMGDirectIn"))
				return gcnew OTComm::Due::AdapterEMGDirectIn();
			if (adapterID == ("AdapterLoadDirectIn"))
				return gcnew OTComm::Due::AdapterLoadDirectIn();
			if (adapterID == ("AdapterIMU"))
				return gcnew OTComm::Due::AdapterIMU();
			return nullptr;
		}

		GenericChannel^ DataGeneratorDue::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
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
			if (channelID == ("AUX  Force"))
				return gcnew OTComm::Sensors::AUXFor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == "AUX  Trigger")
				return gcnew OTComm::Sensors::AUXTrig(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == ("SensorIMU"))
				return gcnew OTComm::Sensors::SensorIMU(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			return nullptr;
		}

		/// <summary>
		/// Get device XML configuration
		/// </summary>
		/// <returns>Returns XML of the device configuration</returns>
		String^ DataGeneratorDue::GetDeviceXMLConfiguration()
		{
			OpenXMLConfigStream(false);

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", mDeviceTotalChannels.ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}

			mXMLTextStream->WriteAttributeString("PortName", mPortName);
			mXMLTextStream->WriteAttributeString("BaudRate", mBaudRate.ToString());
			mXMLTextStream->WriteAttributeString("OTCommClass", this->GetType()->FullName);
			mXMLTextStream->WriteAttributeString("ad_bits", "16");
			mXMLTextStream->WriteStartElement("Channels");
			for (int ii = 0; ii < mConnectors->Length; ii++)
			{
				if (mConnectors[ii]->Adapter != nullptr)
					mConnectors[ii]->Adapter->WriteXMLConfiguration(mXMLTextStream, ii, ii * 2, 0);
			}
			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		String^ DataGeneratorDue::GetSavingXMLConfiguration()
		{
			OpenXMLConfigStream(false);
			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", mDeviceTotalChannels.ToString());

			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			mXMLTextStream->WriteAttributeString("ad_bits", "16");
			mXMLTextStream->WriteAttributeString("SampleFrequency", "2048");
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}			mXMLTextStream->WriteAttributeString("PortName", mPortName);
			mXMLTextStream->WriteAttributeString("BaudRate", mBaudRate.ToString());
			mXMLTextStream->WriteAttributeString("OTCommClass", this->GetType()->FullName);
			mXMLTextStream->WriteStartElement("Channels");
			for (int ii = 0; ii < mConnectors->Length; ii++)
			{
				if (mConnectors[ii]->Adapter != nullptr)
					mConnectors[ii]->Adapter->WriteXMLConfiguration(mXMLTextStream, ii + 1, ii + 1, 0);
			}
			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			return CloseXMLConfigStream();
		}

		/// <summary>
		/// Open the device communication
		/// </summary>
		/// <returns>Returns -1 if there is an error, returns 0 if everything is fine </returns>
		void DataGeneratorDue::OpenDevice(void)
		{
			LOG_ENTERING;
			IntPtr ptrToNativeString = Marshal::StringToHGlobalUni(mPortName);
			mDeviceHandle = CreateFile((LPCWSTR)ptrToNativeString.ToPointer(), GENERIC_READ | GENERIC_WRITE,
				0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
			Marshal::FreeHGlobal(ptrToNativeString);
			if (mDeviceHandle == INVALID_HANDLE_VALUE)
			{
				InvalidHandleException^ ihe = gcnew InvalidHandleException(__FUNCTION__);
				FireOnError(ihe);
				throw ihe;
			}
			else
				DebugLog::Print("Due opened");
			DCB PortDCB;
			// Initialize the DCBlength member. 
			PortDCB.DCBlength = sizeof(DCB);
			// Get the default port setting information.
			if (!GetCommState(mDeviceHandle, &PortDCB))
			{
				GetCommFailedException^ gcfe = gcnew GetCommFailedException(__FUNCTION__);
				FireOnError(gcfe);
				throw gcfe;
			}
			else
				DebugLog::Print("Due: GetCommState OK");
			PortDCB.ByteSize = 8;
			PortDCB.StopBits = ONESTOPBIT;
			PortDCB.Parity = NOPARITY;
			PortDCB.BaudRate = mBaudRate;

			// Configure the port according to the specifications of the DCB structure.
			if (!SetCommState(mDeviceHandle, &PortDCB))
			{
				SetCommFailedException^ scfe = gcnew SetCommFailedException(__FUNCTION__);
				FireOnError(scfe);
				CloseHandle(mDeviceHandle);
				throw scfe;
			}

			if (!SetCommMask(mDeviceHandle, EV_RXCHAR))
			{
				SetCommFailedException^ scfe = gcnew SetCommFailedException(__FUNCTION__);
				FireOnError(scfe);
				CloseHandle(mDeviceHandle);
				throw scfe;
			}

			mCompletionPortHandle = CreateIoCompletionPort(mDeviceHandle, NULL, 1, 0);
			if (mCompletionPortHandle == NULL)
			{
				CompletionPortException^ cpe = gcnew CompletionPortException(__FUNCTION__);
				FireOnError(cpe);
				CloseHandle(mDeviceHandle);
				throw cpe;
			}
			mDeviceIsOpen = true;
			DataGeneratorInterface::OpenDevice();

			LOG_EXITING;
		}

		/// <summary>
		/// Initialize the device
		/// </summary>
		/// <param AcquiredChannels="array containing acquired channels"></param>
		/// <param SamplingFrequency="sampling frequency of acquisition"></param>
		void DataGeneratorDue::InitializeDevice()
		{
			LOG_ENTERING;
			InternalStopSerialDataTransfer();
			DataGeneratorInterface::InitializeDevice();
			LOG_EXITING;
		}

		/// <summary>
		/// Function that creates the Async Acquisition Thread
		/// </summary>
		void DataGeneratorDue::OuternThread()
		{
			LOG_ENTERING;
			DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper;
			try
			{
				mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);


				int* tmpData = new int[mSources->Length];
				AdapterIMU^ Adapter = gcnew AdapterIMU();
				Type^ ImuType = Adapter->GetType();
				for (unsigned int kS = 0; kS < mSources->Length; kS++)
				{
					tmpData[kS] = 2; //initialize
					for (int m = 0; m < 2; m++)
					{
						if (mConnectors[kS * 2 + m] != nullptr && mConnectors[kS * 2 + m]->Adapter != nullptr)
						{
							if (mConnectors[kS * 2 + m]->Adapter->GetType() == ImuType)
							{
								tmpData[kS] = 9;
							}
						}
					}
				}

				mDataReaderInterfaceHelper->mCustomData = (LPARAM)tmpData;

				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoSerialEugenio,  // Thread Func
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
			finally
			{

				if (mDataReaderInterfaceHelper->mCustomData != NULL)
				{
					int* tmp = (int*)mDataReaderInterfaceHelper->mCustomData;
					delete(tmp);
					mDataReaderInterfaceHelper->mCustomData = NULL;
				}
			}

			LOG_EXITING;
		}

		/// <summary>
		/// Starts the acquisition of data from the device
		/// </summary>
		/// <returns>Returns -1 if there is an error, returns 0 if everything is fine </returns>
		void DataGeneratorDue::StartAcquisition(void)
		{
			LOG_ENTERING;
			{
				char buf[1] = { 128 }; // Decimal value to write to serial port
				OVERLAPPED *OverlappedStruc;
				OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
				if (OverlappedStruc == NULL)
				{
					OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
					FireOnError(ose);
					throw ose;
				}

				ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
				OverlappedStruc->Internal = STATUS_PENDING;

				SetLastError(0);

				//write 128 to start transmission
				BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, 1, NULL, OverlappedStruc);
				DWORD numberOfBytesTransferred = 0;
				OVERLAPPED *completedOv;
				ULONG_PTR  key;
				if (GetQueuedCompletionStatus(mCompletionPortHandle, &numberOfBytesTransferred, &key, &completedOv, 1000) == 0)
				{
					StartAcquisitionFailedException^ safe = gcnew StartAcquisitionFailedException(__FUNCTION__);
					CloseHandle(OverlappedStruc->hEvent);
					FireOnError(safe);
					throw safe;
				}
				DataGeneratorInterface::StartAcquisition();
				mManagedAcquisitionThread = gcnew Thread(gcnew ThreadStart(this, &DataGeneratorDue::OuternThread));
				mManagedAcquisitionThread->Start();
			}
			LOG_EXITING;
		}

		/// <summary>
		/// Stops the acquisition of data from the device
		/// </summary>
		void DataGeneratorDue::StopAcquisition(void)
		{
			LOG_ENTERING;
			DataGeneratorInterface::StopAcquisition();
			InternalStopSerialDataTransfer();
			LOG_EXITING;
		}

		/// <summary>
		/// Close the device communication
		/// </summary>
		void DataGeneratorDue::CloseDevice(void)
		{
			LOG_ENTERING;
			if (mDeviceHandle != INVALID_HANDLE_VALUE && mDeviceIsOpen)
			{
				CloseHandle(mDeviceHandle);
				mDeviceHandle = INVALID_HANDLE_VALUE;
			}
			if (mCompletionPortHandle != INVALID_HANDLE_VALUE && mDeviceIsOpen)
			{
				CloseHandle(mCompletionPortHandle);
				mCompletionPortHandle = INVALID_HANDLE_VALUE;
			}

			mDeviceIsOpen = false;
			DataGeneratorInterface::CloseDevice();
			LOG_EXITING;
		}

		/// <summary>
		/// Destroyer
		/// </summary>
		DataGeneratorDue::~DataGeneratorDue()
		{
			if (mDeviceIsOpen)
			{
				CloseDevice();
			}
			this->!DataGeneratorDue();
		}

		/// <summary>
		/// 
		/// </summary>
		DataGeneratorDue::!DataGeneratorDue()
		{

		}

		/// <summary>
		/// Stop Acquisition of device
		/// </summary>
		/// <returns> Returns -1 if there is an error, returns 0 if everything is fine </returns>
		void DataGeneratorDue::InternalStopSerialDataTransfer(void)
		{

			LOG_ENTERING;
			{
				char buf[1] = { 0 }; // Decimal value to write to serial port

				OVERLAPPED *OverlappedStruc;
				OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
				if (OverlappedStruc == NULL)
				{
					OverlappedStructException^ ose = gcnew OverlappedStructException(__FUNCTION__);
					FireOnError(ose);
					throw ose;
				}

				ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
				OverlappedStruc->Internal = STATUS_PENDING;

				SetLastError(0);

				//write 0 to stop transmission

				BOOL WriteFileRes = WriteFile(mDeviceHandle, buf, 1, NULL, OverlappedStruc);
				DWORD numberOfBytesTransferred = 0;
				OVERLAPPED *completedOv;
				ULONG_PTR  key;
				if (GetQueuedCompletionStatus(mCompletionPortHandle, &numberOfBytesTransferred, &key, &completedOv, 1000) == 0)
				{
					StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__);
					CloseHandle(OverlappedStruc->hEvent);
					FireOnError(safe);
					throw safe;
				}
				LOG_MESSAGE("Due: Data transfer stopped");
			}
			LOG_EXITING;
		}




#pragma unmanaged

		union SwapShort
		{
			unsigned char Bytes[2];
			unsigned short Short;
		};




		void ExpandIMUBuffer(unsigned short* DecodedBufferPosFireIMU,
			unsigned short* DecodedBufferPosFire, size_t NumberOfTracesToSend, size_t SamplesToCopyPerChannel)
		{
			unsigned short *dest = DecodedBufferPosFireIMU;
			unsigned short *source = DecodedBufferPosFire;
			unsigned short nP;
			unsigned short nT;
			if (NumberOfTracesToSend == 9)
			{
				for (size_t kch = 0; kch < 9; kch++)
				{
					dest = DecodedBufferPosFireIMU + kch;
					source = DecodedBufferPosFire + kch;

					for (size_t k = 0; k < SamplesToCopyPerChannel; k++)
					{
						//*dest = *source;
						//source += 16;
						//dest += 9;

						nP = k / 64;
						nT = (k % 64) / 22;
						source = DecodedBufferPosFire + 23 + nT * 40 + nP * 128 + 2 * kch;
						*dest = (*source) +32768;
						dest += 9;
					}
				}

			}
			else //10
			{
				//copy aux
				for (size_t k = 0; k < SamplesToCopyPerChannel; k++)
				{
					*dest = *source;
					source += 2;
					dest += 10;
				}
				for (size_t kch = 0; kch < 9; kch++)
				{
					dest = DecodedBufferPosFireIMU + 1 + kch;
					source = DecodedBufferPosFire + 1 + kch;

					for (size_t k = 0; k < SamplesToCopyPerChannel; k++)
					{
						//*dest = *source;
						//source += 2 * 16;
						//dest += 10;

						nP = k / 64;
						nT = (k % 64) / (22);
						source = DecodedBufferPosFire + 23 + nT * 40 + nP * 128 + 2 * kch;
						*dest = (*source) + 32768;
						dest += 10;
					}
				}


			}
		}

		ULONG _stdcall AsyncIoSerialEugenio(PVOID  ThreadParameter)
		{
			const int NUM_FRAMES_TO_SKIP_AT_BEGINNING = 0;
			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ACQUISITION_BUFFERS = 2;
			const int NUM_PROBES = 7;
			const int PACKET_LENGTH = 265;
			const int HEADER_SIZE = 4;
			short Header[HEADER_SIZE] = { 255,255,0,0 };
			const int FOOTER_SIZE = 5;
			const int MAX_RAMP = 255;
			const int DATA_PER_PACKET = (PACKET_LENGTH - HEADER_SIZE - FOOTER_SIZE);


			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;
			unsigned __int16 DeviceTotalSources = helper->mDeviceSources;

			unsigned short *DecodedBuffer[NUM_PROBES];
			memset(DecodedBuffer, 0, sizeof(unsigned short*)*NUM_PROBES);

			unsigned short *DecodedBufferPosWrite[NUM_PROBES];
			unsigned short *DecodedBufferPosFire[NUM_PROBES];
			unsigned short *DecodedBufferEnd[NUM_PROBES];

			unsigned short *DecodedBufferIMU[NUM_PROBES];
			memset(DecodedBufferIMU, 0, sizeof(unsigned short*)*NUM_PROBES);
			unsigned short *DecodedBufferPosFireIMU[NUM_PROBES];
			unsigned short *DecodedBufferEndIMU[NUM_PROBES];
			int NextExpectedRampIndex[NUM_PROBES];
			UCHAR *AcquisitionBuffer;
			UCHAR *AcquisitionBuffers[NUM_ACQUISITION_BUFFERS];
			int* VirtualNumChannelsPerSource = (int*)helper->mCustomData;

			//I select 10 times the size of a frame
			ULONG AcquisitionBufferSizeInBytes = (PACKET_LENGTH +10) * sizeof(char);

			//size for just one of the streams (one "channel" over the 2 or 9 allowed for due
			ULONG DecodedBufferSizeSingleSourceSingleChannel = NUM_DECODED_BUFFERS * helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSizeSingleSourceSingleChannelInBytes = DecodedBufferSizeSingleSourceSingleChannel * sizeof(__int16);

			ULONG numberOfBytesTransferred;
			OVERLAPPED *OverlappedStruc;
			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));

			if (OverlappedStruc == NULL)
			{
				return -1;
			}
			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			OverlappedStruc->Internal = STATUS_PENDING;


			AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, AcquisitionBufferSizeInBytes*NUM_ACQUISITION_BUFFERS);
			if (AcquisitionBuffer == NULL)
			{
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				return -1;
			}

			//for simplicity, I save the beginning of each acquisition buffer
			for (size_t kacq = 0; kacq < NUM_ACQUISITION_BUFFERS; kacq++)
			{
				AcquisitionBuffers[kacq] = AcquisitionBuffer + kacq*AcquisitionBufferSizeInBytes;
			}

			unsigned short NumberOfTracesToSend[NUM_PROBES];
			unsigned char ExpandIMU[NUM_PROBES];
			memset(ExpandIMU, 0, sizeof(ExpandIMU));
			unsigned char ChannelIsImu[NUM_PROBES][2];
			memset(ChannelIsImu, 0, sizeof(ChannelIsImu));
			for (size_t kSource = 0; kSource < DeviceTotalSources; kSource++)
			{
				NumberOfTracesToSend[kSource] = 0;
				if (VirtualNumChannelsPerSource[kSource] == 2)
					NumberOfTracesToSend[kSource] = helper->mSource[kSource].mAcquiredChannels;
				else//IMU
				{
					for (int km = 0; km < helper->mSource[kSource].mAcquiredChannels; km++)
					{
						if (helper->mSource[kSource].mMapping[km] == 0)
						{
							NumberOfTracesToSend[kSource] += 1;
						}
						else
						{
							ChannelIsImu[kSource][km] = 1;
							ExpandIMU[kSource] = true;
							NumberOfTracesToSend[kSource] += 9;
						}
					}
				}
				unsigned short NumberOfAcquiredChannels = helper->mSource[kSource].mAcquiredChannels;
				DecodedBuffer[kSource] = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeSingleSourceSingleChannelInBytes*NumberOfAcquiredChannels);
				if (ExpandIMU[kSource])
				{
					DecodedBufferIMU[kSource] =
						(unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeSingleSourceSingleChannelInBytes*NumberOfTracesToSend[kSource]);
				}
				if (DecodedBuffer[kSource] == NULL || (ExpandIMU[kSource] && DecodedBufferIMU[kSource] == NULL))
				{
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					LocalFree((void *)AcquisitionBuffer);
					AcquisitionBuffer = NULL;
					for (size_t kprev = 0; kprev <= kSource; kprev++)
					{
						if (DecodedBuffer[kprev] != NULL)
						{
							LocalFree((void *)DecodedBuffer[kprev]);
							DecodedBuffer[kprev] = NULL;
						}
						if (DecodedBufferIMU[kprev] != NULL)
						{
							LocalFree((void *)DecodedBuffer[kprev]);
							DecodedBuffer[kprev] = NULL;
						}
					}


					return -1;
				}
				//points to the beginning of the decoded buffer
				DecodedBufferPosFire[kSource] = DecodedBuffer[kSource];
				DecodedBufferPosWrite[kSource] = DecodedBuffer[kSource];
				DecodedBufferEnd[kSource] = DecodedBuffer[kSource] + DecodedBufferSizeSingleSourceSingleChannel*NumberOfAcquiredChannels;
				if (ExpandIMU[kSource])
				{
					DecodedBufferPosFireIMU[kSource] = DecodedBufferIMU[kSource];
					DecodedBufferEndIMU[kSource] =
						DecodedBufferIMU[kSource] + DecodedBufferSizeSingleSourceSingleChannel*NumberOfTracesToSend[kSource];
				}

			}

			for (size_t kdec = 0; kdec < DeviceTotalSources; kdec++)
			{
				NextExpectedRampIndex[kdec] = -(NUM_FRAMES_TO_SKIP_AT_BEGINNING + 1);
			}

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;
			int Iteration = 0;

			OVERLAPPED *completedOv;
			ULONG_PTR   key;

			size_t ValidFrame1 = 0;
			size_t ValidFrame2 = 0;


			unsigned short AcquisitionBufferIndex = 0;
			UCHAR* AcquisitionBufferCurrent = AcquisitionBuffers[AcquisitionBufferIndex];
			UCHAR* AcquisitionBufferEndPos = AcquisitionBufferCurrent + AcquisitionBufferSizeInBytes;
			UCHAR* FrameStartPos = AcquisitionBufferCurrent;
			UCHAR* FrameStopPos = AcquisitionBufferCurrent;
			size_t Source = 0;
			unsigned char SysID = 0;
			unsigned char AuxOut = 0;
			bool WaitForSecondPart = false;
			bool IsMultiPartFrame = false;
			int RampIndex = 0;
			size_t offsetRead = 0;
			int oldRampIndex = -100;

			DCB PortDCB;
			// Initialize the DCBlength member. 
			PortDCB.DCBlength = sizeof(DCB);
			SetLastError(0);
			// Get the default port setting information.
			if (GetCommState(helper->mDeviceHandle, &PortDCB))
			{

				WHILE(*mMustStopAcquisitionThread == false)
				{
					AcquisitionBufferIndex = (AcquisitionBufferIndex + 1) % 2;
					AcquisitionBufferCurrent = AcquisitionBuffers[AcquisitionBufferIndex];
					AcquisitionBufferEndPos = AcquisitionBufferCurrent + AcquisitionBufferSizeInBytes;
					FrameStartPos = AcquisitionBufferCurrent;



					SetLastError(0);
					//try an async read
					BOOL ReadFileRes = ReadFile(helper->mDeviceHandle, &AcquisitionBufferCurrent[offsetRead],
						AcquisitionBufferSizeInBytes - offsetRead, &numberOfBytesTransferred, OverlappedStruc);

					while (GetQueuedCompletionStatus(helper->mCompletionPortHandle, &numberOfBytesTransferred,
						&key, &completedOv, 1000) == 0)
					{
						if (*mMustStopAcquisitionThread == true)
						{
							exitMainLoop = true;
							break;
						}
						else
						{
							if (!SetCommState(helper->mDeviceHandle, &PortDCB))
							{
								exitMainLoop = true;
								break;
							}
						}
					}



					if (exitMainLoop)break;
					//I received the data.
					//first of all, search for valid data, 
					offsetRead = 0;
					ValidFrame1 = 0;

					// look for the first header
					while (FrameStartPos < AcquisitionBufferEndPos)
					{
						if (ValidFrame1 != 4)
						{
							FrameStartPos = AcquisitionBufferCurrent;
							while (FrameStartPos < AcquisitionBufferEndPos)
							{
								if (*FrameStartPos == Header[ValidFrame1])
								{
									ValidFrame1++;
									FrameStartPos++;

									if (ValidFrame1 == 4)
									{
										break;
									}
								}
								else
								{
									//non è l'header. 
									FrameStartPos -= (ValidFrame1 - 1);
									ValidFrame1 = 0;
								}
							}
						}
						else
							break;
					}

					if (ValidFrame1 != 4)
					{
						UCHAR* destination = AcquisitionBuffers[(AcquisitionBufferIndex + 1) % 2];
						offsetRead = 4;
						memcpy(destination, AcquisitionBufferEndPos - 4, offsetRead);

						/*helper->mNewDataAvailable(0, DecodedBufferPosFire[0],
							nullptr,
							helper->mSource[0].mAcquiredChannels,
							0,
							helper->mSource[0].mDataRegroupFactor, 10000);*/
						continue;
					}

					// look for the second header
					FrameStopPos = FrameStartPos;
					ValidFrame2 = 0;
					while (FrameStopPos < AcquisitionBufferEndPos)
					{
						if (ValidFrame2 != 4)
						{
							while (FrameStopPos < AcquisitionBufferEndPos)
							{
								if (*FrameStopPos == Header[ValidFrame2])
								{
									ValidFrame2++;
									FrameStopPos++;

									if (ValidFrame2 == 4)
									{
										break;
									}
								}
								else
								{
									//non è l'header. 
									FrameStopPos -= (ValidFrame2 - 1);
									ValidFrame2 = 0;
								}
							}
						}
						else
							break;
					}

					if (ValidFrame2 != 4 && FrameStopPos - FrameStartPos < PACKET_LENGTH)
					{
						UCHAR* destination = AcquisitionBuffers[(AcquisitionBufferIndex + 1) % 2];
						offsetRead = AcquisitionBufferEndPos - FrameStartPos + 4;
						memcpy(destination, FrameStartPos - 4, offsetRead);

						/*helper->mNewDataAvailable(0, DecodedBufferPosFire[0],
							nullptr,
							helper->mSource[0].mAcquiredChannels,
							0,
							helper->mSource[0].mDataRegroupFactor, 20000);*/

						continue;
					}


					if (ValidFrame2 != 4)
					{
						UCHAR* destination = AcquisitionBuffers[(AcquisitionBufferIndex + 1) % 2];
						offsetRead = ValidFrame2;
						memcpy(destination, FrameStartPos - ValidFrame2, offsetRead);
					}
					else
					{
						UCHAR* destination = AcquisitionBuffers[(AcquisitionBufferIndex + 1) % 2];
						offsetRead = AcquisitionBufferEndPos - (FrameStopPos - ValidFrame2);
						memcpy(destination, FrameStopPos - ValidFrame2, offsetRead);
					}

					UCHAR* PointerToHeader = FrameStartPos + (PACKET_LENGTH - HEADER_SIZE - FOOTER_SIZE);
					SysID = *PointerToHeader;
					PointerToHeader++;
					Source = *(PointerToHeader)-1;
					PointerToHeader++;
					RampIndex = *(PointerToHeader);
					AuxOut = SysID;

					if (FrameStopPos - FrameStartPos < PACKET_LENGTH)
					{
						/*helper->mNewDataAvailable(Source, DecodedBufferPosFire[0],
							nullptr,
							helper->mSource[0].mAcquiredChannels,
							0,
							helper->mSource[0].mDataRegroupFactor, 30000);*/
						continue;
					}
					/*helper->mNewDataAvailable(Source, DecodedBufferPosFire[0],
						nullptr,
						helper->mSource[0].mAcquiredChannels,
						0,
						helper->mSource[0].mDataRegroupFactor, 50000);*/
						/*helper->mNewDataAvailable(Source, DecodedBufferPosFire[0],
							nullptr,
							helper->mSource[0].mAcquiredChannels,
							0,
							helper->mSource[0].mDataRegroupFactor, AuxOut);*/



					bool ProcessFrame = true;
					if (Source >= 0 && Source < NUM_PROBES)
					{
						//unsigned short NumberOfTracesToSend = helper->mSource[Source].mAcquiredChannels;
						unsigned short* mapping = helper->mSource[Source].mMapping;
						unsigned short AcquiredChannels = helper->mSource[Source].mAcquiredChannels;
						if (AcquiredChannels > 0)
						{
							size_t DecodedBufferSize = DecodedBufferSizeSingleSourceSingleChannel*AcquiredChannels;
							unsigned short* DecodedBufferCurrentEndSend = DecodedBufferPosFire[Source] + DecodedBufferSize / NUM_DECODED_BUFFERS;
							//FrameStartPos += 4; //skip header
							if (NextExpectedRampIndex[Source] < -1)
							{
								NextExpectedRampIndex[Source]++;
								ProcessFrame = false;
							}
							else if (NextExpectedRampIndex[Source] != -1 && RampIndex != NextExpectedRampIndex[Source])
							{
								//copy empty buffers till done
								size_t FramesLost = ((MAX_RAMP + 1) - NextExpectedRampIndex[Source] + RampIndex) % (MAX_RAMP + 1);
								size_t SampleToCopy = FramesLost * AcquiredChannels * 64;
								while (SampleToCopy > 0)
								{
									//NOTA attenzione, DecodedBufferCurrentEndSend è l'indirizzo del primo byte DOPO il buffer
									size_t Space = DecodedBufferCurrentEndSend - DecodedBufferPosWrite[Source];
									if (SampleToCopy > Space)
									{
										memset(DecodedBufferPosWrite[Source], 0, Space * sizeof(__int16));
										if (ExpandIMU[Source] == true)
										{
											ExpandIMUBuffer(DecodedBufferPosFireIMU[Source],
												DecodedBufferPosFire[Source], NumberOfTracesToSend[Source],
												helper->mSource[0].mDataRegroupFactor);
											helper->mNewDataAvailable(Source, DataSize::SixteenBits, DecodedBufferPosFireIMU[Source],
												nullptr,
												NumberOfTracesToSend[Source],
												0,
												helper->mSource[Source].mDataRegroupFactor, AuxOut);
											DecodedBufferPosFireIMU[Source] +=
												NumberOfTracesToSend[Source] * DecodedBufferSizeSingleSourceSingleChannel;
											if (DecodedBufferPosFireIMU[Source] >= DecodedBufferEndIMU[Source])
												DecodedBufferPosFireIMU[Source] = DecodedBufferIMU[Source];
										}
										else
										{
											helper->mNewDataAvailable(Source, DataSize::SixteenBits, DecodedBufferPosFire[Source],
												nullptr,
												helper->mSource[Source].mAcquiredChannels,
												0,
												helper->mSource[Source].mDataRegroupFactor, ((unsigned int)SampleToCopy) / 128);
										}

										DecodedBufferPosWrite[Source] += Space;
										if (DecodedBufferPosWrite[Source] >= DecodedBufferEnd[Source])
										{
											DecodedBufferPosWrite[Source] = DecodedBuffer[Source];
										}
										DecodedBufferPosFire[Source] = DecodedBufferPosWrite[Source];
										DecodedBufferCurrentEndSend = DecodedBufferPosFire[Source] + DecodedBufferSize / NUM_DECODED_BUFFERS;
										SampleToCopy -= Space;
									}
									else //the buffer is big enough, copy everything and exit the loop
									{
										memset(DecodedBufferPosWrite[Source], 0, SampleToCopy * sizeof(__int16));
										DecodedBufferPosWrite[Source] += SampleToCopy;
										SampleToCopy = 0;
									}
								}
							}
							if (ProcessFrame)
							{
								NextExpectedRampIndex[Source] = (RampIndex + 1) % (MAX_RAMP + 1);
								unsigned short *acquireddatashort = (unsigned short *)FrameStartPos;
								size_t Sample = 0;


								while (Sample++ < DATA_PER_PACKET / 4)
								{
									for (int kc = 0; kc < AcquiredChannels; kc++)
									{
										SwapShort ss;
										ss.Short = acquireddatashort[mapping[kc]];
										unsigned char tmp = ss.Bytes[0];
										ss.Bytes[0] = ss.Bytes[1];
										//if (SysID==188 /*||
										//	SysID == 140 ||
										//	SysID == 156 ||
										//	SysID == 172)*/)
										//	ss.Bytes[1] = tmp;// +128;
										//else
										ss.Bytes[1] = tmp + 128;
										*DecodedBufferPosWrite[Source] = ss.Short;
										DecodedBufferPosWrite[Source]++;
									}
									//}
									acquireddatashort += 2;

									if (DecodedBufferPosWrite[Source] >= DecodedBufferCurrentEndSend)//decode buffer full
									{
										if (ExpandIMU[Source] == true)
										{
											ExpandIMUBuffer(DecodedBufferPosFireIMU[Source],
												DecodedBufferPosFire[Source], NumberOfTracesToSend[Source],
												helper->mSource[0].mDataRegroupFactor);
											helper->mNewDataAvailable(Source, DataSize::SixteenBits, DecodedBufferPosFireIMU[Source],
												nullptr,
												NumberOfTracesToSend[Source],
												0,
												helper->mSource[Source].mDataRegroupFactor, AuxOut);
											DecodedBufferPosFireIMU[Source] +=
												NumberOfTracesToSend[Source] * DecodedBufferSizeSingleSourceSingleChannel;
											if (DecodedBufferPosFireIMU[Source] >= DecodedBufferEndIMU[Source])
												DecodedBufferPosFireIMU[Source] = DecodedBufferIMU[Source];
										}
										else
										{
											helper->mNewDataAvailable(Source, DataSize::SixteenBits, DecodedBufferPosFire[Source],
												nullptr,
												helper->mSource[Source].mAcquiredChannels,
												0,
												helper->mSource[Source].mDataRegroupFactor, AuxOut);
										}






										if (DecodedBufferPosWrite[Source] >= DecodedBufferEnd[Source])
										{
											DecodedBufferPosWrite[Source] = DecodedBuffer[Source];
										}
										DecodedBufferPosFire[Source] = DecodedBufferPosWrite[Source];
										DecodedBufferCurrentEndSend = DecodedBufferPosFire[Source] + DecodedBufferSize / NUM_DECODED_BUFFERS;
									}

								}
							}
						}
						//////////////////////////////////////
					}



					if (*mMustStopAcquisitionThread) break;

				}//while
			}

			CancelIo(helper->mDeviceHandle);

			// We are not recording any more
			//InterlockedExchange(RecordingSamples, 0);


			if (!*mMustStopAcquisitionThread)
			{
				// Signal that acquisition ended before time
				//GlobalErrorDispatcher("An error occurred",helper->mError);
			}

			if (AcquisitionBuffer)
			{
				for (size_t kacq = 0; kacq < NUM_ACQUISITION_BUFFERS; kacq++)
				{
					LocalFree((void *)AcquisitionBuffer[kacq]);
					AcquisitionBuffer[kacq] = NULL;
				}
			}

			/*if (OverlappedStruc)
			{
			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;
			}*/

			if (DecodedBuffer)
			{
				for (size_t kprev = 0; kprev < DeviceTotalSources; kprev++)
				{
					LocalFree((void *)DecodedBuffer[kprev]);
					DecodedBuffer[kprev] = NULL;
				}
			}
			helper->mStopAcquisition();
			return 0;


		}

		ULONG _stdcall AsyncIoSerial(PVOID  ThreadParameter)
		{
			const int NUM_FRAMES_TO_SKIP_AT_BEGINNING = 0;
			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ACQUISITION_BUFFERS = 2;
			const int NUM_PROBES = 7;
			const int PACKET_LENGTH = 265;
			const int HEADER_SIZE = 4;
			short Header[HEADER_SIZE] = { 255,255,0,0 };
			const int FOOTER_SIZE = 5;
			const int MAX_RAMP = 255;
			const int DATA_PER_PACKET = (PACKET_LENGTH - HEADER_SIZE - FOOTER_SIZE);


			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;
			unsigned __int16 DeviceTotalSources = helper->mDeviceSources;

			unsigned short *DecodedBuffer[NUM_PROBES];
			memset(DecodedBuffer, 0, sizeof(unsigned short*)*NUM_PROBES);

			unsigned short *DecodedBufferPosWrite[NUM_PROBES];
			unsigned short *DecodedBufferPosFire[NUM_PROBES];
			unsigned short *DecodedBufferEnd[NUM_PROBES];

			unsigned short *DecodedBufferIMU[NUM_PROBES];
			memset(DecodedBufferIMU, 0, sizeof(unsigned short*)*NUM_PROBES);
			unsigned short *DecodedBufferPosFireIMU[NUM_PROBES];
			unsigned short *DecodedBufferEndIMU[NUM_PROBES];
			int NextExpectedRampIndex[NUM_PROBES];
			UCHAR *AcquisitionBuffer;
			UCHAR *AcquisitionBuffers[NUM_ACQUISITION_BUFFERS];
			int* VirtualNumChannelsPerSource = (int*)helper->mCustomData;

			//I select 10 times the size of a frame
			ULONG AcquisitionBufferSizeInBytes = PACKET_LENGTH * sizeof(char);

			//size for just one of the streams (one "channel" over the 2 or 9 allowed for due
			ULONG DecodedBufferSizeSingleSourceSingleChannel = NUM_DECODED_BUFFERS * helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSizeSingleSourceSingleChannelInBytes = DecodedBufferSizeSingleSourceSingleChannel * sizeof(__int16);

			ULONG numberOfBytesTransferred;
			OVERLAPPED *OverlappedStruc;
			OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));

			if (OverlappedStruc == NULL)
			{
				return -1;
			}
			ZeroMemory(OverlappedStruc, sizeof(OVERLAPPED));
			OverlappedStruc->Internal = STATUS_PENDING;


			AcquisitionBuffer = (UCHAR *)LocalAlloc(LPTR, AcquisitionBufferSizeInBytes*NUM_ACQUISITION_BUFFERS);
			if (AcquisitionBuffer == NULL)
			{
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				return -1;
			}

			//for simplicity, I save the beginning of each acquisition buffer
			for (size_t kacq = 0; kacq < NUM_ACQUISITION_BUFFERS; kacq++)
			{
				AcquisitionBuffers[kacq] = AcquisitionBuffer + kacq*AcquisitionBufferSizeInBytes;
			}

			unsigned short NumberOfTracesToSend[NUM_PROBES];
			unsigned char ExpandIMU[NUM_PROBES];
			memset(ExpandIMU, 0, sizeof(ExpandIMU));
			unsigned char ChannelIsImu[NUM_PROBES][2];
			memset(ChannelIsImu, 0, sizeof(ChannelIsImu));
			for (size_t kSource = 0; kSource < DeviceTotalSources; kSource++)
			{
				NumberOfTracesToSend[kSource] = 0;
				if (VirtualNumChannelsPerSource[kSource] == 2)
					NumberOfTracesToSend[kSource] = helper->mSource[kSource].mAcquiredChannels;
				else//IMU
				{
					for (int km = 0; km < helper->mSource[kSource].mAcquiredChannels; km++)
					{
						if (helper->mSource[kSource].mMapping[km] == 0)
						{
							NumberOfTracesToSend[kSource] += 1;
						}
						else
						{
							ChannelIsImu[kSource][km] = 1;
							ExpandIMU[kSource] = true;
							NumberOfTracesToSend[kSource] += 8;
						}
					}
				}
				unsigned short NumberOfAcquiredChannels = helper->mSource[kSource].mAcquiredChannels;
				DecodedBuffer[kSource] = (unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeSingleSourceSingleChannelInBytes*NumberOfAcquiredChannels);
				if (ExpandIMU[kSource])
				{
					DecodedBufferIMU[kSource] =
						(unsigned short*)LocalAlloc(LPTR, DecodedBufferSizeSingleSourceSingleChannelInBytes*NumberOfTracesToSend[kSource]);
				}
				if (DecodedBuffer[kSource] == NULL || (ExpandIMU[kSource] && DecodedBufferIMU[kSource] == NULL))
				{
					LocalFree((void *)OverlappedStruc);
					OverlappedStruc = NULL;
					LocalFree((void *)AcquisitionBuffer);
					AcquisitionBuffer = NULL;
					for (size_t kprev = 0; kprev <= kSource; kprev++)
					{
						if (DecodedBuffer[kprev] != NULL)
						{
							LocalFree((void *)DecodedBuffer[kprev]);
							DecodedBuffer[kprev] = NULL;
						}
						if (DecodedBufferIMU[kprev] != NULL)
						{
							LocalFree((void *)DecodedBuffer[kprev]);
							DecodedBuffer[kprev] = NULL;
						}
					}


					return -1;
				}
				//points to the beginning of the decoded buffer
				DecodedBufferPosFire[kSource] = DecodedBuffer[kSource];
				DecodedBufferPosWrite[kSource] = DecodedBuffer[kSource];
				DecodedBufferEnd[kSource] = DecodedBuffer[kSource] + DecodedBufferSizeSingleSourceSingleChannel*NumberOfAcquiredChannels;
				if (ExpandIMU[kSource])
				{
					DecodedBufferPosFireIMU[kSource] = DecodedBufferIMU[kSource];
					DecodedBufferEndIMU[kSource] =
						DecodedBufferIMU[kSource] + DecodedBufferSizeSingleSourceSingleChannel*NumberOfTracesToSend[kSource];
				}

			}

			for (size_t kdec = 0; kdec < DeviceTotalSources; kdec++)
			{
				NextExpectedRampIndex[kdec] = -(NUM_FRAMES_TO_SKIP_AT_BEGINNING + 1);
			}

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;
			int Iteration = 0;

			OVERLAPPED *completedOv;
			ULONG_PTR   key;

			size_t ValidFrame = 0;

			unsigned short AcquisitionBufferIndex = 0;
			UCHAR* AcquisitionBufferCurrent = AcquisitionBuffers[AcquisitionBufferIndex];
			UCHAR* AcquisitionBufferEndPos = AcquisitionBufferCurrent + AcquisitionBufferSizeInBytes;
			UCHAR* FrameStartPos = AcquisitionBufferCurrent;
			size_t Source = 0;
			unsigned char SysID = 0;
			bool WaitForSecondPart = false;
			bool IsMultiPartFrame = false;
			int RampIndex = 0;
			size_t offsetRead = 0;

			DCB PortDCB;
			// Initialize the DCBlength member. 
			PortDCB.DCBlength = sizeof(DCB);
			SetLastError(0);
			// Get the default port setting information.
			if (GetCommState(helper->mDeviceHandle, &PortDCB))
			{




				WHILE(*mMustStopAcquisitionThread == false)
				{
					SetLastError(0);
					//try an async read
					BOOL ReadFileRes = ReadFile(helper->mDeviceHandle, &AcquisitionBufferCurrent[offsetRead],
						AcquisitionBufferSizeInBytes - offsetRead, &numberOfBytesTransferred, OverlappedStruc);

					while (GetQueuedCompletionStatus(helper->mCompletionPortHandle, &numberOfBytesTransferred,
						&key, &completedOv, 1000) == 0)
					{
						if (*mMustStopAcquisitionThread == true)
						{
							exitMainLoop = true;
							break;
						}
						else
						{
							if (!SetCommState(helper->mDeviceHandle, &PortDCB))
							{
								exitMainLoop = true;
								break;
							}
						}
					}

					if (exitMainLoop)break;
					//I received the data.
					//first of all, search for valid data, 
					offsetRead = 0;
					while (FrameStartPos < AcquisitionBufferEndPos)
					{
						if (ValidFrame != 4)
						{
							FrameStartPos = AcquisitionBufferCurrent;
							while (FrameStartPos < AcquisitionBufferEndPos)
							{
								if (*FrameStartPos == Header[ValidFrame])
								{
									ValidFrame++;
									FrameStartPos++;
									if ((ValidFrame == 4 && FrameStartPos + (PACKET_LENGTH - HEADER_SIZE) > AcquisitionBufferEndPos)
										||
										(ValidFrame > 0 && FrameStartPos + (HEADER_SIZE - ValidFrame) > AcquisitionBufferEndPos))
									{
										//ho trovato l'inizio del frame PB2EB: FrameStartPosè la posizione del primo byte utile DOPO l'header
										//copy the received part at the beginning of the next buffer and read the remaining part
										WaitForSecondPart = true;
										UCHAR* destination = AcquisitionBuffers[(AcquisitionBufferIndex + 1) % 2];
										offsetRead = AcquisitionBufferEndPos - (FrameStartPos - ValidFrame);
										memcpy(destination, FrameStartPos - ValidFrame, offsetRead);
										FrameStartPos = destination + ValidFrame;
										break;
									}

									if (ValidFrame == 4)
									{
										break;
									}
								}
								else
								{
									//non è l'header. 
									FrameStartPos -= (ValidFrame - 1);
									ValidFrame = 0;
								}
							}
						}
						else //I have an "old" start from the previous acquisition
						{
							WaitForSecondPart = false;
						}

						if (WaitForSecondPart)
							break;

						if (ValidFrame == 4)
						{
							ValidFrame = 0;
							//UCHAR* PointerToSource = AcquisitionBuffer + (FrameStartPos - AcquisitionBuffer + 256 + 2) % AcquisitionBufferSizeInBytes;
							UCHAR* PointerToHeader = FrameStartPos + (PACKET_LENGTH - HEADER_SIZE - FOOTER_SIZE);
							SysID = *PointerToHeader;
							PointerToHeader++;
							Source = *(PointerToHeader)-1;
							SysID = RampIndex;
							PointerToHeader++;
							RampIndex = *(PointerToHeader);
							bool ProcessFrame = true;
							if (Source >= 0 && Source < NUM_PROBES)
							{
								//unsigned short NumberOfTracesToSend = helper->mSource[Source].mAcquiredChannels;
								unsigned short* mapping = helper->mSource[Source].mMapping;
								unsigned short AcquiredChannels = helper->mSource[Source].mAcquiredChannels;
								if (AcquiredChannels > 0)
								{
									size_t DecodedBufferSize = DecodedBufferSizeSingleSourceSingleChannel*AcquiredChannels;
									unsigned short* DecodedBufferCurrentEndSend = DecodedBufferPosFire[Source] + DecodedBufferSize / NUM_DECODED_BUFFERS;
									//FrameStartPos += 4; //skip header
									if (NextExpectedRampIndex[Source] < -1)
									{
										NextExpectedRampIndex[Source]++;
										ProcessFrame = false;
									}
									else if (NextExpectedRampIndex[Source] != -1 && RampIndex != NextExpectedRampIndex[Source])
									{
										//copy empty buffers till done
										size_t FramesLost = ((MAX_RAMP + 1) - NextExpectedRampIndex[Source] + RampIndex) % (MAX_RAMP + 1);
										size_t SampleToCopy = FramesLost * AcquiredChannels * 64;
										while (SampleToCopy > 0)
										{
											//NOTA attenzione, DecodedBufferCurrentEndSend è l'indirizzo del primo byte DOPO il buffer
											size_t Space = DecodedBufferCurrentEndSend - DecodedBufferPosWrite[Source];
											if (SampleToCopy > Space)
											{
												memset(DecodedBufferPosWrite[Source], 0, Space * sizeof(__int16));
												if (ExpandIMU[Source] == true)
												{
													ExpandIMUBuffer(DecodedBufferPosFireIMU[Source],
														DecodedBufferPosFire[Source], NumberOfTracesToSend[Source],
														helper->mSource[0].mDataRegroupFactor);
													helper->mNewDataAvailable(Source, DataSize::SixteenBits, DecodedBufferPosFireIMU[Source],
														nullptr,
														NumberOfTracesToSend[Source],
														0,
														helper->mSource[Source].mDataRegroupFactor, SysID);
													DecodedBufferPosFireIMU[Source] +=
														NumberOfTracesToSend[Source] * DecodedBufferSizeSingleSourceSingleChannel;
													if (DecodedBufferPosFireIMU[Source] >= DecodedBufferEndIMU[Source])
														DecodedBufferPosFireIMU[Source] = DecodedBufferIMU[Source];
												}
												else
												{
													helper->mNewDataAvailable(Source, DataSize::SixteenBits, DecodedBufferPosFire[Source],
														nullptr,
														helper->mSource[Source].mAcquiredChannels,
														0,
														helper->mSource[Source].mDataRegroupFactor, SysID);
												}

												DecodedBufferPosWrite[Source] += Space;
												if (DecodedBufferPosWrite[Source] >= DecodedBufferEnd[Source])
												{
													DecodedBufferPosWrite[Source] = DecodedBuffer[Source];
												}
												DecodedBufferPosFire[Source] = DecodedBufferPosWrite[Source];
												DecodedBufferCurrentEndSend = DecodedBufferPosFire[Source] + DecodedBufferSize / NUM_DECODED_BUFFERS;
												SampleToCopy -= Space;
											}
											else //the buffer is big enough, copy everything and exit the loop
											{
												memset(DecodedBufferPosWrite[Source], 0, SampleToCopy * sizeof(__int16));
												DecodedBufferPosWrite[Source] += SampleToCopy;
												SampleToCopy = 0;
											}
										}
									}
									if (ProcessFrame)
									{
										NextExpectedRampIndex[Source] = (RampIndex + 1) % (MAX_RAMP + 1);
										unsigned short *acquireddatashort = (unsigned short *)FrameStartPos;
										size_t Sample = 0;


										while (Sample++ < DATA_PER_PACKET / 4)
										{
											for (int kc = 0; kc < AcquiredChannels; kc++)
											{
												SwapShort ss;
												ss.Short = acquireddatashort[mapping[kc]];
												unsigned char tmp = ss.Bytes[0];
												ss.Bytes[0] = ss.Bytes[1];
												ss.Bytes[1] = tmp + 128;
												*DecodedBufferPosWrite[Source] = ss.Short;
												DecodedBufferPosWrite[Source]++;
											}
											//}
											acquireddatashort += AcquiredChannels;

											if (DecodedBufferPosWrite[Source] >= DecodedBufferCurrentEndSend)//decode buffer full
											{
												if (ExpandIMU[Source] == true)
												{
													ExpandIMUBuffer(DecodedBufferPosFireIMU[Source],
														DecodedBufferPosFire[Source], NumberOfTracesToSend[Source],
														helper->mSource[0].mDataRegroupFactor);
													helper->mNewDataAvailable(Source, DataSize::SixteenBits, DecodedBufferPosFireIMU[Source],
														nullptr,
														NumberOfTracesToSend[Source],
														0,
														helper->mSource[Source].mDataRegroupFactor, SysID);
													DecodedBufferPosFireIMU[Source] +=
														NumberOfTracesToSend[Source] * DecodedBufferSizeSingleSourceSingleChannel;
													if (DecodedBufferPosFireIMU[Source] >= DecodedBufferEndIMU[Source])
														DecodedBufferPosFireIMU[Source] = DecodedBufferIMU[Source];
												}
												else
												{
													helper->mNewDataAvailable(Source, DataSize::SixteenBits, DecodedBufferPosFire[Source],
														nullptr,
														helper->mSource[Source].mAcquiredChannels,
														0,
														helper->mSource[Source].mDataRegroupFactor, SysID);
												}






												if (DecodedBufferPosWrite[Source] >= DecodedBufferEnd[Source])
												{
													DecodedBufferPosWrite[Source] = DecodedBuffer[Source];
												}
												DecodedBufferPosFire[Source] = DecodedBufferPosWrite[Source];
												DecodedBufferCurrentEndSend = DecodedBufferPosFire[Source] + DecodedBufferSize / NUM_DECODED_BUFFERS;
											}

										}
									}
								}
								//////////////////////////////////////
							}
							FrameStartPos += PACKET_LENGTH - HEADER_SIZE; //footer
							if (*mMustStopAcquisitionThread) break;
						}
						else
						{
							//qualcosa non ha funzionato... ricomincio la ricerca						
							FrameStartPos = FrameStartPos++;
						}
					}
					if (*mMustStopAcquisitionThread) break;

					AcquisitionBufferIndex = (AcquisitionBufferIndex + 1) % 2;
					AcquisitionBufferCurrent = AcquisitionBuffers[AcquisitionBufferIndex];
					AcquisitionBufferEndPos = AcquisitionBufferCurrent + AcquisitionBufferSizeInBytes;
					if (ValidFrame == 0)
						FrameStartPos = AcquisitionBufferCurrent;


				}//while
			}

			CancelIo(helper->mDeviceHandle);

			// We are not recording any more
			//InterlockedExchange(RecordingSamples, 0);


			if (!*mMustStopAcquisitionThread)
			{
				// Signal that acquisition ended before time
				//GlobalErrorDispatcher("An error occurred",helper->mError);
			}

			if (AcquisitionBuffer)
			{
				for (size_t kacq = 0; kacq < NUM_ACQUISITION_BUFFERS; kacq++)
				{
					LocalFree((void *)AcquisitionBuffer[kacq]);
					AcquisitionBuffer[kacq] = NULL;
				}
			}

			/*if (OverlappedStruc)
			{
			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;
			}*/

			if (DecodedBuffer)
			{
				for (size_t kprev = 0; kprev < DeviceTotalSources; kprev++)
				{
					LocalFree((void *)DecodedBuffer[kprev]);
					DecodedBuffer[kprev] = NULL;
				}
			}
			helper->mStopAcquisition();
			return 0;


		}

	}

}