

#include <windows.h>
#include "DataConverter.h"
#include "DataGeneratorAnEMG.h"
#include <guiddef.h>
#include <initguid.h>
#include <strsafe.h>
#include <setupapi.h>
#include <basetyps.h>

using namespace System::IO;
using namespace System::Reflection;
using namespace System::Xml;

namespace OTComm
{
	namespace AnEMG
	{

DEFINE_GUID(GUID_DEVINTERFACE_EMGUSB2, 0x9d57f99f, 0x33cb, 0x488d, 0xa5, 0x57, 0x2c, 0x5d, 0x0f, 0xd5, 0x1d, 0x85);
#define IOCTL_INDEX             0x800
#define FILE_DEVICE_EMGUSB2   0x65500
#define IOCTL_EMGUSB2_START_ACQUISITION CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 6, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_STOP_ACQUISITION	CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 7, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_GET_DEVICE_ID		CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 4, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_EMGUSB2_PROGRAM_FPGA	    CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 5, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_PROGRAM_PIC       CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 11,METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_EMGUSB2_GET_SETTINGS	    CTL_CODE(FILE_DEVICE_EMGUSB2, IOCTL_INDEX + 8, METHOD_BUFFERED, FILE_READ_ACCESS)



#pragma managed

#pragma region Enums
		private enum EMGUSBFamilies { EMGUSB1 = 1, EMGUSB2 = 2, EMGUSB2p = 3, MEBA = 81, WIRELESS = 82 };
#pragma endregion 

#pragma region Constructors and destructors
		DataGeneratorAnEMG::DataGeneratorAnEMG(Frequency SamplingFrequency) :
			DataGeneratorInterface(),
			mDeviceIsOpen(false), mDataReaderInterfaceHelper(NULL),
			mTestMode(false), mModel(AnEMG::Models::AnEMG_16), mManagedAcquisitionThreadHandle(nullptr)
		{
			mConnectors = gcnew array<GenericConnector^>(GetNumberConnectorsMultipleIn() + GetNumberConnectorsIn() + GetNumberConnectorsAux());
			unsigned int k = 0;
			for (; k < GetNumberConnectorsIn(); k++)
				mConnectors[k] = gcnew GenericConnector(16);
			for (; k < GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn(); k++)
				mConnectors[k] = gcnew GenericConnector(64);
			for (; k < (unsigned int)mConnectors->Length; k++)
				mConnectors[k] = gcnew GenericConnector(1);
			mSources = gcnew array<DataGeneratorSingleSource^>(1);
			mSources[0] = gcnew DataGeneratorSingleSource(SamplingFrequency, 2, GetChannelsMapping());
			mDeviceTotalChannels = GetNumberTotalChannels();
		}

		DataGeneratorAnEMG::~DataGeneratorAnEMG()
		{
			try
			{
				if (mDeviceIsOpen)
				{
					CloseDevice();
				}
				this->!DataGeneratorAnEMG();
			}
			catch(Exception^)
			{

			}
		}

		DataGeneratorAnEMG::!DataGeneratorAnEMG()
		{

		}

#pragma endregion
		
#pragma region XML files 

		String^ DataGeneratorAnEMG::GetDeviceXMLConfiguration()
		{
			LOG_ENTERING;
				bool IsUSB23 = DEVICE(mModel) == EMGUSBFamilies::EMGUSB2 || DEVICE(mModel) == EMGUSBFamilies::EMGUSB2p;
			// Decode settings
			const int gainvalues[] = { 0, 100, 200, 500, 1000, 2000, 5000, 10000 };
			const int highpassvalues[] = { 3, 10, 100, 200 };
			const int lowpassvalues[] = { 130, 500, 900, 4400 };

			
			OpenXMLConfigStream(false);

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			try
			{
				mXMLTextStream->WriteAttributeString("ID", GetDeviceID().ToString());
			}
			catch (Exception^)
			{
			}
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetNumberTotalChannels()).ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}
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
				startIndex = k < GetNumberConnectorsIn() ? 16 * k : (k < (GetNumberConnectorsIn() + GetNumberConnectorsMultipleIn()) ? GetNumberConnectorsIn() * 16 + (k - GetNumberConnectorsIn()) * 64 : (k - GetNumberConnectorsIn() - GetNumberConnectorsMultipleIn()) + GetNumberConnectorsIn() * 16 + GetNumberConnectorsMultipleIn() * 64);

				if (mConnectors[k]->Adapter != nullptr)
				{
					mConnectors[k]->Adapter->WriteXMLConfiguration(mXMLTextStream, k, startIndex, 0);
				}

				//ChannelNum += mConnectors[k]->NumConnectableChannels;
			}

			mXMLTextStream->WriteEndElement();//"Channels"
			mXMLTextStream->WriteEndElement();//"Device"
			String^ res = CloseXMLConfigStream();
			LOG_EXITING;
			return res;
		}

		String^ DataGeneratorAnEMG::GetSavingXMLConfiguration()
		{
			LOG_ENTERING;
			// Raw settings (for the abstract)
			array<byte>^Firmware = gcnew array<byte>(3);
			array<float>^ m_Gain = gcnew array<float>(mConnectors->Length);
			array<float>^ m_LowPassFilter = gcnew array<float>(mConnectors->Length);
			array<float>^ m_HighPassFilter = gcnew array<float>(mConnectors->Length);
			array<int>^ m_DetectionMode = gcnew array<int>(mConnectors->Length);
			for (int ii = 0; ii < 16; ii++)
			{
				if (m_Gain->Length - ii - 1 > 0)
				{
					m_Gain[m_Gain->Length - ii - 1] = 0.5;
					m_LowPassFilter[m_LowPassFilter->Length - ii - 1] = 100000;
					m_DetectionMode[m_DetectionMode->Length - ii - 1] = 3;
				}
			}


			OpenXMLConfigStream(false);			

			mXMLTextStream->WriteStartElement("Device");
			mXMLTextStream->WriteAttributeString("Name", GetDeviceDescription());
			try
			{
				mXMLTextStream->WriteAttributeString("ID", GetDeviceID().ToString());
			}
			catch(Exception^)
			{ }
			mXMLTextStream->WriteAttributeString("DeviceTotalChannels", (GetAcquiredChannels())->Length.ToString());
			mXMLTextStream->WriteAttributeString("Model", ((int)mModel).ToString());
			mXMLTextStream->WriteAttributeString("Sources", mSources->Length.ToString());
			//for (int ii = 0; ii < mSources->Length; ii++) //PB2EB: sistemare aprendo una sezione <Source></Source>
			//{
			//	mXMLTextStream->WriteAttributeString("SampleFrequency", mSources[ii]->mSamplingFrequency.ToString());
			//}

			mXMLTextStream->WriteStartElement("Channels");

			unsigned int ChannelNum = 0;
			int startIndex = 0;
			for (unsigned int k = 0; k < (unsigned int)mConnectors->Length; k++)
			{
				if (mConnectors[k]->Adapter != nullptr)
				{
					mConnectors[k]->Adapter->WriteSavingXMLConfiguration(
						mXMLTextStream,
						k,
						startIndex,
						0,
						m_Gain[k],
						m_LowPassFilter[k],
						m_HighPassFilter[k],
						m_DetectionMode[k]);

					for (int zz = 0; zz < mConnectors[k]->Adapter->Channels->Length; zz++)
						if (mConnectors[k]->Adapter->Channels[zz] != nullptr)
							startIndex++;
				}
			}

			mXMLTextStream->WriteEndElement();//"Channels"

			mXMLTextStream->WriteEndElement();//"Device"

			String^ res = CloseXMLConfigStream();
			LOG_EXITING;
			return res;
		}

		DataGeneratorAnEMG^ DataGeneratorAnEMG::FromXMLConfiguration(String^ Filename)
		{
			DataGeneratorAnEMG^ gdr = nullptr;
			XmlTextReader^ textReader = gcnew XmlTextReader(Filename);
			XmlDocument^ xmlDoc = gcnew XmlDocument();
			xmlDoc->Load(textReader);
			System::Xml::XmlNode^ DeviceNode = xmlDoc->ChildNodes[1];
			try{
				String^ deviceName = DeviceNode->Attributes["Name"]->Value;
				unsigned int deviceModel = System::Convert::ToUInt16(DeviceNode->Attributes["Model"]->Value);
				unsigned int deviceFrequency = System::Convert::ToUInt16(DeviceNode->Attributes["SampleFrequency"]->Value);
				if (deviceName->Contains("AnEMG"))
				{
					gdr = gcnew OTComm::AnEMG::DataGeneratorAnEMG(deviceFrequency);
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
								DebugLog::Print(ChannelNode->Attributes["ID"]->Value->ToString());
								DebugLog::Print(ChannelNode->Attributes["Muscle"]->Value->ToString());
								DebugLog::Print(ChannelNode->Attributes["Side"]->Value->ToString());
								DebugLog::Print(ChannelNode->Attributes["Index"]->Value->ToString());

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
						gdr->ConnectAux(adPosition - (unsigned int)gdr->GetNumberConnectorsIn() - (unsigned int)gdr->GetNumberConnectorsMultipleIn(), (AdapterAux^)ad);
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


		unsigned int DataGeneratorAnEMG::GetNumberTotalChannels()
		{
			return GetNumberChannelsNonAux() + GetNumberChannelsAux();
		}

		unsigned int DataGeneratorAnEMG::GetNumberChannelsNonAux()
		{
			return 64;
		}

		unsigned int DataGeneratorAnEMG::GetNumberChannelsAux()
		{
			return 16;
		}

		array<unsigned int>^ DataGeneratorAnEMG::CreateChannelsMapping()
		{
			return gcnew array<unsigned int>(80) {
				0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70,
					75, 1, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 71, 76, 2, 7,
					12, 17, 22, 27, 32, 37, 42, 47, 52, 57, 62, 67, 72, 77, 3, 8, 13, 18,
					23, 28, 33, 38, 43, 48, 53, 58, 63, 68, 73, 78, 4, 9, 14, 19, 24, 29,
					34, 39, 44, 49, 54, 59, 64, 69, 74, 79
			};
		}

		array<unsigned int>^ DataGeneratorAnEMG::GetAcquiredChannels()
		{
			return mAcquiredChannelsIndices;
		}
		array<unsigned int>^ DataGeneratorAnEMG::GetChannelsMapping()
		{
			return DataGeneratorAnEMG::CreateChannelsMapping();
		}

		unsigned int DataGeneratorAnEMG::GetNumberConnectorsAux()
		{
			return GetNumberChannelsAux();
		}

		unsigned int DataGeneratorAnEMG::GetNumberConnectorsMultipleIn()
		{
			return 0;
		}

		unsigned int DataGeneratorAnEMG::GetNumberConnectorsIn()
		{
			return 4;//TODO
		}

		void DataGeneratorAnEMG::ConnectAux(const unsigned int port, AdapterAux^ adapter)
		{
			if (port >= GetNumberConnectorsAux())
			{
				InvalidConnectorNumberException^ icne = gcnew InvalidConnectorNumberException(__FUNCTION__);
				FireOnError(icne);
				throw icne;
			}

			mConnectors[GetNumberConnectorsMultipleIn() + GetNumberConnectorsIn() + port]->Adapter = adapter;
		}

		GenericAdapter^ DataGeneratorAnEMG::GetAdapterFromID(String^ adapterID)
		{
			if (adapterID == "Direct connection to Auxiliary Input")
				return gcnew OTComm::AnEMG::AdapterAux();
			return nullptr;
		}

		GenericChannel^ DataGeneratorAnEMG::GetChannelFromID(String^ channelID, String^ muscleDescription, String^ sideDescription)
		{
			if (channelID == "AUX  Acceleration")
				return gcnew OTComm::Sensors::AUXAcc(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == "AUX  Torque")
				return gcnew OTComm::Sensors::AUXTor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == "AUX  Force")
				return gcnew OTComm::Sensors::AUXFor(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			if (channelID == "AUX  Trigger")
				return gcnew OTComm::Sensors::AUXTrig(Muscles::ID(muscleDescription), MuscleSide::ID(sideDescription));
			return nullptr;
		}
#pragma endregion

#pragma region Device properties 

		unsigned int DataGeneratorAnEMG::GetDeviceModel()
		{
			return (unsigned int)mModel;
		}

		bool DataGeneratorAnEMG::IsOpen()
		{
			return mDeviceIsOpen;
		}

		
		Bitmap^ DataGeneratorAnEMG::Image::get()
		{
			return Utilities::GetImageFromEmbeddedResource("AnEMG12_2.png", true);
		}		

		String^ DataGeneratorAnEMG::GetDeviceDescription(void)
		{
			String^ res = gcnew String("AnEMG");
			return res;
		}
		
		int DataGeneratorAnEMG::GetDeviceID()
		{
			LOG_ENTERING;
			try
			{
				return(int)ReadDeviceID();
			}
			catch (Exception^e)
			{
				GetDeviceIDFailedException^ gdIDfe = gcnew GetDeviceIDFailedException(__FUNCTION__, e);
				FireOnError(gdIDfe);
				throw gdIDfe;
			}
			LOG_EXITING;			
		}				

#pragma endregion

#pragma region Public device operations
		void DataGeneratorAnEMG::OpenDevice(void)
		{
			LOG_ENTERING;
			try
			{
				WCHAR completeDeviceName[MAX_DEVPATH_LENGTH];
				if (!GetDevicePath((LPGUID)&GUID_DEVINTERFACE_EMGUSB2, completeDeviceName, countof(completeDeviceName), 0))
				{
					throw gcnew InvalidDevicePath(__FUNCTION__);
				}

				mPortName = gcnew System::String(completeDeviceName);
				//IntPtr  ptrToNativeString  = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(mPortName);
				mDeviceHandle = CreateFile(completeDeviceName,
					GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_WRITE | FILE_SHARE_READ,
					NULL, // default security
					OPEN_EXISTING,
					(FILE_FLAG_OVERLAPPED | SECURITY_IMPERSONATION),
					NULL);
				//System::Runtime::InteropServices::Marshal::FreeHGlobal(ptrToNativeString);
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}

				// Stop acquisition
				InternalStopUSBDataTransfer();

				mDeviceIsOpen = true;
				DataGeneratorInterface::OpenDevice();
			}
			catch(Exception ^e)
			{
				OpenDeviceFailedException^ odfe = gcnew OpenDeviceFailedException(__FUNCTION__, e);
				FireOnError(odfe);
				throw odfe;
			}
			LOG_EXITING;
		}
		
		void DataGeneratorAnEMG::InitializeDevice()
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				InternalStopUSBDataTransfer();
				ProgramFPGA();
				DataGeneratorInterface::InitializeDevice();
			}
			catch (Exception^e)
			{
				InitializeDeviceFailedException^ idfe = gcnew InitializeDeviceFailedException(__FUNCTION__, e);
				FireOnError(idfe);
				throw idfe;
			}
			LOG_EXITING;			
		}

		void DataGeneratorAnEMG::StartAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				OVERLAPPED *OverlappedStruc;
				OverlappedStruc = (OVERLAPPED *)LocalAlloc(LPTR, sizeof(OVERLAPPED));
				if (OverlappedStruc == NULL)
				{
					throw gcnew OverlappedStructException(__FUNCTION__);
				}
				else
				{
					DataGeneratorInterface::StartAcquisition();
					mManagedAcquisitionThreadHandle =
						gcnew Thread(gcnew ThreadStart(this, &DataGeneratorAnEMG::OuternThread));
					mManagedAcquisitionThreadHandle->Start();
					LOG_MESSAGE("AnEMG Port: thread started");
				}
			}
			catch (Exception^e)
			{
				StartAcquisitionFailedException^ safe = gcnew StartAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}

		void DataGeneratorAnEMG::StopAcquisition(void)
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				//TODO CAPIRE PERCHé JOIN
					DataGeneratorInterface::StopAcquisition();
					if (mManagedAcquisitionThreadHandle != nullptr)
						try
					{
						mManagedAcquisitionThreadHandle->Join(1000);
						InternalStopUSBDataTransfer();
					}
					catch (Exception^)
					{
						
					}				
				InternalStopUSBDataTransfer();			
			}
			catch (Exception^e)
			{
				StopAcquisitionFailedException^ safe = gcnew StopAcquisitionFailedException(__FUNCTION__, e);
				FireOnError(safe);
				throw safe;
			}
			LOG_EXITING;
		}

		void DataGeneratorAnEMG::CloseDevice(void)
		{
			LOG_ENTERING;
			try
			{
				if (!mDeviceIsOpen)
				{
					throw gcnew DeviceIsClosedException(__FUNCTION__);
				}
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				InternalStopUSBDataTransfer();
				CloseHandle(mDeviceHandle);
				mDeviceHandle = INVALID_HANDLE_VALUE;
				mDeviceIsOpen = false;
				DataGeneratorInterface::CloseDevice();
			}
			catch (Exception^e)
			{
				CloseDeviceFailedException^ cdfe = gcnew CloseDeviceFailedException(__FUNCTION__, e);
				FireOnError(cdfe);
				throw cdfe;
			}
			LOG_EXITING;
		}
#pragma endregion

#pragma region Non public device operations

		UCHAR DataGeneratorAnEMG::ReadDeviceID()
		{
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}

				ULONG index;
				UCHAR ch = 0;
				UCHAR dev_id = 0;
				if (!DeviceIoControl(mDeviceHandle,
					IOCTL_EMGUSB2_GET_DEVICE_ID,
					&ch,  // Ptr to InBuffer
					1,            // Length of InBuffer
					&dev_id,      // Ptr to OutBuffer
					1,            // Length of OutBuffer
					&index,       // BytesReturned
					0)) {         // Ptr to Overlapped structure
					dev_id = 0;
					throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_GET_DEVICE_ID");
				}
				return dev_id;
			}
			catch (Exception^)
			{

			}
		}
		void realpath(const char *filename, wchar_t *pathbuf, DWORD size)
		{
			OFSTRUCT of;
			HANDLE file = (HANDLE)OpenFile(filename, &of, OF_READ);
			GetFinalPathNameByHandle(file, pathbuf, size, FILE_NAME_OPENED);
			CloseHandle(file);
		}

		void DataGeneratorAnEMG::ProgramFPGA()
		{
			LOG_ENTERING;
			String^ FileName = String::Empty;
			IntPtr ptrtxt = IntPtr::Zero;

			if (mDeviceHandle == INVALID_HANDLE_VALUE)
			{
				throw gcnew InvalidHandleException(__FUNCTION__);
			}
			try
			{
				String^ ResourceName;
				String^ FileName = System::IO::Path::GetTempFileName();
				switch (DEVICE(mModel))
				{
				case EMGUSBFamilies::EMGUSB2:
					ResourceName = "emg256.hex";
					break;
				case EMGUSBFamilies::EMGUSB2p:
					ResourceName = "emg256p.hex";
					break;
				case EMGUSBFamilies::MEBA:
					ResourceName = "emg32.hex";
					break;
				case EMGUSBFamilies::WIRELESS:
					ResourceName = "wireless.hex";
					break;
				default:
					ResourceName = "emg128.hex";
					break;
				}

				Assembly ^myAssembly = Assembly::GetExecutingAssembly();
				Stream ^myStream = myAssembly->GetManifestResourceStream(ResourceName);
				FileStream^ fs = gcnew FileStream(FileName, FileMode::Append, FileAccess::Write, FileShare::Write);

				array<Byte>^ buffer = gcnew array<Byte>(32768);
				long long TempPos = myStream->Position;

				while (true)
				{
					int read = myStream->Read(buffer, 0, buffer->Length);
					if (read <= 0) break;
					fs->Write(buffer, 0, read);
				}
				myStream->Close();
				fs->Close();

				// Get a FileSecurity object that represents the 
				// current security settings.
				System::Security::AccessControl::FileSecurity ^fSecurity = File::GetAccessControl(FileName);

				//BUG FIX for language(PC DANESE)
				System::Security::Principal::SecurityIdentifier ^everyone =
					gcnew System::Security::Principal::SecurityIdentifier(
						System::Security::Principal::WellKnownSidType::WorldSid, nullptr);

				// Add the FileSystemAccessRule to the security settings.
				fSecurity->AddAccessRule(
					gcnew System::Security::AccessControl::FileSystemAccessRule(everyone,
						System::Security::AccessControl::FileSystemRights::FullControl,
						System::Security::AccessControl::AccessControlType::Allow));

				// Set the new access settings.
				File::SetAccessControl(FileName, fSecurity);


				ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(FileName);
				TCHAR buf[1024];
				realpath((LPCSTR)ptrtxt.ToPointer(), buf, 1024);
				ULONG index;
				if (!DeviceIoControl(mDeviceHandle,
					IOCTL_EMGUSB2_PROGRAM_FPGA,
					buf,  // Ptr to InBuffer
					1024,          // Length of InBuffer
					NULL,         // Ptr to OutBuffer
					0,            // Length of OutBuffer
					&index,       // BytesReturned
					0))
				{         // Ptr to Overlapped structure
					throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_PROGRAM_FPGA");
				}
				
			}
			catch (Exception^e)
			{
				if(ptrtxt!= IntPtr::Zero)
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
				if(!String::IsNullOrEmpty(FileName))
					File::Delete(FileName);
				throw e;
			}
			LOG_EXITING;
		}

		
		void DataGeneratorAnEMG::OuternThread()
		{
			LOG_ENTERING;
			try
			{
				DataGeneratorInterfaceHelper* mDataReaderInterfaceHelper = new DataGeneratorInterfaceHelper(this);
				pin_ptr<int> MustStopAcquisitionPinned = &mMustStopAcquisition; // pin pointer 
				mDataReaderInterfaceHelper->mMustStopAcquisitionPtr = MustStopAcquisitionPinned;
				HANDLE mAsyncIoThreadHandle = CreateThread(
					NULL,			// Default Security Attrib.
					0,				// Initial Stack Size,
					AsyncIoAnEMG,  // Thread Func
					mDataReaderInterfaceHelper, // Thread parameter
					0,             // Creation Flags
					NULL);        // Don't need the Thread Id.
				if (mAsyncIoThreadHandle == NULL)
				{
					ThreadCreationException^ tce = gcnew ThreadCreationException(__FUNCTION__);
					FireOnError(tce);
				}
				else
					WaitForSingleObject(mAsyncIoThreadHandle, INFINITE);			
			}
			catch (Exception ^e)
			{
				AcquisitionThreadException^ ate = gcnew AcquisitionThreadException(__FUNCTION__, e);
				FireOnError(ate);
			}
			LOG_EXITING;
		}		

		void DataGeneratorAnEMG::InternalStopUSBDataTransfer(void)
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle == INVALID_HANDLE_VALUE)
					throw gcnew InvalidHandleException(__FUNCTION__);

				// Stop acquisition
				DWORD read;
				if (!DeviceIoControl(mDeviceHandle,
					IOCTL_EMGUSB2_STOP_ACQUISITION,
					NULL,		// Ptr to InBuffer
					0,          // Length of InBuffer
					NULL,         // Ptr to OutBuffer
					0,            // Length of OutBuffer
					&read,       // BytesReturned
					0)) // Ptr to Overlapped structure
				{
					FireOnError("AnEMG failed stop acquisition", GetLastError(), __FUNCTION__);
					LOG_ERROR("AnEMG failed stop acquisition");
					throw gcnew DeviceIoControlException(__FUNCTION__,"IOCTL_EMGUSB2_STOP_ACQUISITION");
				}
			}
			catch(Exception^e)
			{
				throw e;
			}
			LOG_EXITING;
		}

		

		void DataGeneratorAnEMG::ProgramPIC(String^ hexfile)
		{
			LOG_ENTERING;
			try
			{
				if (mDeviceHandle != INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}

				IntPtr ptrtxt = System::Runtime::InteropServices::Marshal::StringToCoTaskMemAnsi(hexfile);
				TCHAR buf[1024];
				realpath((LPCSTR)ptrtxt.ToPointer(), buf, 1024);
				ULONG index;
				if (!DeviceIoControl(mDeviceHandle,
					IOCTL_EMGUSB2_PROGRAM_PIC,
					(LPVOID)ptrtxt.ToPointer(),  // Ptr to InBuffer
					1024,          // Length of InBuffer
					NULL,         // Ptr to OutBuffer
					0,            // Length of OutBuffer
					&index,       // BytesReturned
					0)) {         // Ptr to Overlapped structure
					System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
					throw gcnew DeviceIoControlException(__FUNCTION__,"IOCTL_EMGUSB2_PROGRAM_PIC");
				}
				System::Runtime::InteropServices::Marshal::FreeCoTaskMem(ptrtxt);
			}
			catch (Exception^e)
			{
				ProgramPICFailedException^ ppfe = gcnew ProgramPICFailedException(__FUNCTION__,e);
				FireOnError(ppfe);
				throw ppfe;
			}
			LOG_EXITING;
		}

		String^ DataGeneratorAnEMG::GetFirmwareVersion()
		{
			try
			{
				if (mDeviceHandle != INVALID_HANDLE_VALUE)
				{
					throw gcnew InvalidHandleException(__FUNCTION__);
				}
				ULONG index;
				UCHAR ch, buf[128];
				if (mDeviceHandle != INVALID_HANDLE_VALUE)
				{
					ZeroMemory(buf, sizeof(buf));
					if (!DeviceIoControl(mDeviceHandle,
						IOCTL_EMGUSB2_GET_SETTINGS,
						&ch,      // Ptr to InBuffer
						1,            // Length of InBuffer
						buf,          // Ptr to OutBuffer
						sizeof(buf), // Length of OutBuffer
						&index,       // BytesReturned
						0)) {         // Ptr to Overlapped structure
						throw gcnew DeviceIoControlException(__FUNCTION__, "IOCTL_EMGUSB2_GET_SETTINGS");
					}
					char m_FirmwareVersion[3];
					if (DEVICE(mModel) != EMGUSBFamilies::EMGUSB2 && DEVICE(mModel) != EMGUSBFamilies::EMGUSB2p)
					{
						// EMG-USB			
						m_FirmwareVersion[0] = buf[23];
						m_FirmwareVersion[1] = buf[24];
						m_FirmwareVersion[2] = buf[25];
					}
					else
					{
						// EMG-USB2		
						m_FirmwareVersion[0] = buf[22];
						m_FirmwareVersion[1] = buf[23];
						m_FirmwareVersion[2] = buf[24];
					}
					return gcnew String(m_FirmwareVersion);
				}
				return gcnew String("");
			}
			catch(Exception^e)
			{
				GetFirmwareFailedException^ gffe = gcnew GetFirmwareFailedException(__FUNCTION__, e);
				FireOnError(gffe);
				throw gffe;
			}
		}

		

#pragma endregion

#pragma region Managed code
#pragma unmanaged


		ULONG _stdcall AsyncIoAnEMG(PVOID  ThreadParameter)
		{

			const int NUM_DECODED_BUFFERS = 10;
			const int NUM_ASYNCH_IO = 10;
			DataGeneratorInterfaceHelper* helper = (DataGeneratorInterfaceHelper*)ThreadParameter;
			unsigned __int16 *ChannelMappings = helper->mSource[0].mMapping;

			int *mMustStopAcquisitionThread = helper->mMustStopAcquisitionPtr;

			ULONG DecodedBufferSize = helper->mSource[0].mAcquiredChannels * NUM_DECODED_BUFFERS * helper->mSource[0].mDataRegroupFactor;
			ULONG DecodedBufferSizeInBytes = DecodedBufferSize * sizeof(__int16);
			//volatile LONG *RecordingSamples;
			unsigned __int16 *DecodedBuffer;
			unsigned __int16  *DecodedBufferPos;
			unsigned __int16  *DecodedBufferEnd;
			UCHAR * AcquisitionBuffer;

			ULONG numberOfBytesTransferred;

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
			ULONG SingleAcquisitionBufferSizeInBytes = helper->mDeviceTotalChannels*helper->mSource[0].mDataRegroupFactor * sizeof(char)*helper->mSource[0].mBytesPerChannel;

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



			DecodedBuffer = (unsigned __int16 *)LocalAlloc(LPTR, DecodedBufferSizeInBytes);
			if (DecodedBuffer == NULL)
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				//DebugLog::Print("DecodedBuffer is null");
				return -1;
			}
			else
				;//DebugLog::Print("DecodedBuffer is OK");

				 //points to the beginning of the decoded buffer
			DecodedBufferPos = DecodedBuffer;
			DecodedBufferEnd = DecodedBuffer + DecodedBufferSize;

			//this is the main acquisition loop
			bool exitMainLoop = false;
			DWORD lettitot = 0;

			char OptionsByte = 0;/////////////////FIX
			switch (helper->mDeviceTotalChannels)
			{
			case 72: //64 non aux
			case 80:
				break;
				//case 128:
			default:
				OptionsByte = OptionsByte | 0x04;
				break;
				//case 192:			OptionsByte = OptionsByte | 0x08;			break;
			case 272:// 256:
				OptionsByte = OptionsByte | 0x0c;
				break;
			}
			switch (helper->mSource[0].mSamplingFrequency)
			{
			case 512:
				break;
			case 2048:
			default:
				OptionsByte = OptionsByte | 0x10;
				break;
			case 4096:
			case 5012: // Correzione bug 5012 => 5120
			case 5120:
				OptionsByte = OptionsByte | 0x20;
				break;
			case 10240:
				OptionsByte = OptionsByte | 0x30;
				break;
			}

			OptionsByte = OptionsByte & 0xFC;
			if (!DeviceIoControl(helper->mDeviceHandle,
				IOCTL_EMGUSB2_START_ACQUISITION,
				&OptionsByte,			// Ptr to InBuffer
				1,				// Length of InBuffer
				NULL,			// Ptr to OutBuffer
				0,				// Length of OutBuffer
				&lettitot,      // BytesReturned
				0))  // Ptr to Overlapped structure
			{
				CloseHandle(hCompletionPort);
				hCompletionPort = NULL;
				LocalFree((void *)OverlappedStruc);
				OverlappedStruc = NULL;
				LocalFree((void *)AcquisitionBuffer);
				AcquisitionBuffer = NULL;
				//DebugLog::Print("DecodedBuffer is null");
				return -1;
			}

			for (int i = 0; i < NUM_ASYNCH_IO; i++)
			{
				BOOL ReadFileRes = ReadFile(helper->mDeviceHandle,
					AcquisitionBuffer + (i * SingleAcquisitionBufferSizeInBytes),
					SingleAcquisitionBufferSizeInBytes, NULL, &OverlappedStruc[i]);

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
					for (unsigned int channel = 0; channel < helper->mSource[0].mAcquiredChannels; channel++)
					{
						//add control for buffer overflow
						*DecodedBufferPos = AcquisitionBufferCurrentReadStart[ChannelMappings[channel]];						
						DecodedBufferPos++;
					}
					AcquisitionBufferCurrentReadStart += helper->mDeviceTotalChannels;
				}

				if (*mMustStopAcquisitionThread) break;

				helper->mNewDataAvailable(0, DataSize::SixteenBits,DecodedBufferCurrentStart,nullptr,  helper->mSource[0].mAcquiredChannels, 0,helper->mSource[0].mDataRegroupFactor,0);

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


			// We are not recording any more
			//InterlockedExchange(RecordingSamples, 0);


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

			/*if (OverlappedStruc)
			{
			LocalFree((void *)OverlappedStruc);
			OverlappedStruc = NULL;
			}*/

			if (DecodedBuffer)
			{
				LocalFree((void *)DecodedBuffer);
				DecodedBuffer = NULL;
			}
			helper->mStopAcquisition();
			return 0;
		}


		static BOOL GetDevicePath(IN  LPGUID InterfaceGuid, __out_ecount(BufLen) PWSTR DevicePath, __in size_t BufLen, int DeviceIndex)
		{
			HDEVINFO HardwareDeviceInfo;
			SP_DEVICE_INTERFACE_DATA DeviceInterfaceData;
			PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData = NULL;
			ULONG Length, RequiredLength = 0;
			BOOL bResult;
			HRESULT hr;

			HardwareDeviceInfo = SetupDiGetClassDevs(InterfaceGuid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));

			if (HardwareDeviceInfo == INVALID_HANDLE_VALUE) {
				//wprintf(L"SetupDiGetClassDevs failed!\n");
				return FALSE;
			}
			DeviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

			bResult = SetupDiEnumDeviceInterfaces(HardwareDeviceInfo, 0, InterfaceGuid, DeviceIndex, &DeviceInterfaceData);
			if (bResult == FALSE)
			{
				SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
				return FALSE;
			}

			SetupDiGetDeviceInterfaceDetail(
				HardwareDeviceInfo,
				&DeviceInterfaceData,
				NULL,
				0,
				&RequiredLength,
				NULL
			);

			DeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, RequiredLength);

			if (DeviceInterfaceDetailData == NULL)
			{
				SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
				return FALSE;
			}

			DeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			Length = RequiredLength;
			bResult = SetupDiGetDeviceInterfaceDetail(
				HardwareDeviceInfo,
				&DeviceInterfaceData,
				DeviceInterfaceDetailData,
				Length,
				&RequiredLength,
				NULL);

			if (bResult == FALSE)
			{
				SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
				LocalFree(DeviceInterfaceDetailData);
				return FALSE;
			}

			hr = StringCchCopy(DevicePath, BufLen, DeviceInterfaceDetailData->DevicePath);
			if (FAILED(hr))
			{
				SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
				LocalFree(DeviceInterfaceDetailData);
				return FALSE;
			}

			SetupDiDestroyDeviceInfoList(HardwareDeviceInfo);
			LocalFree(DeviceInterfaceDetailData);
			return TRUE;
		}
#pragma endregion
	}

}

