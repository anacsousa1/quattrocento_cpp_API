#pragma once
/*
namespace OTComm
{
	ref class RemoteDevice
	{
	public:
		String^ Description;
		bool IsAvailableForReading;
		bool IsAvailableForControl;
		String^ Name;
		virtual String^ ToString() override;
	};

	ref class DataGeneratorRemote : public OTComm::DataGeneratorInterface
	{
	public:
		static List<RemoteDevice^> GetAvailableDevices(IPEndPoint^e)
		{
			List<RemoteDevice^> ^rd = gcnew List<RemoteDevice^>();
			//interroga il server
			//se tutto ok, leggi i risultati
			//
		}

		DataGeneratorRemote(IPEndPoint ^e);
		~DataGeneratorRemote();
		!DataGeneratorRemote();
		virtual String^ GetDeviceDescription() override;
		virtual String^ GetDeviceXMLConfiguration() override;
		virtual int OpenDevice(void) override;
		virtual int InitializeDevice(void) override;
		virtual int StartAcquisition(int DeviceTotalChannels, int AcquiredChannels, array<unsigned int>^ Mapping) override;
		virtual int StopAcquisition(void) override;
		virtual int CloseDevice(void) override;
	protected:
		virtual int OnNewData(void) override;
		bool mDeviceIsOpen;
		Socket^ mSocket;
	};
}*/