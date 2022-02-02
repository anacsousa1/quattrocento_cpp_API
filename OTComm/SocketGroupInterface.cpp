#include "DataConsumerSocket.h"
namespace OTComm {
#pragma managed
	SocketGroupInterface::SocketGroupInterface() :mTransmitData(false), mID(IDCounter)
	{
		LOG_MESSAGE(String::Format("Connection {0} opened", mID));
		IDCounter++;
	}

	bool SocketGroupInterface::SendData(array< Byte >^ data)
	{
		// Receive a byte array of arbitrary size and send this through the socket
		try
		{
			mServerNetworkStream->BeginWrite(data, 0, data->Length, nullptr, this);
			return true;
		}
		catch (Exception^)
		{
			// NetworkStream failed -> Socket was closed -> ResetSocket
			//onError(e->Message);
			if (mServerSocket != nullptr && !mServerSocket->Connected)
			{
				CloseConnection();
			}
			return false;
		}

		return false;
	}

	bool SocketGroupInterface::SendData(String^ data)
	{
		try
		{
			array<Byte>^ ByteArray = gcnew array<Byte>(data->Length);
			for (int i = 0; i < data->Length; i++)
				ByteArray[i] = (Byte)data[i];
			return this->SendData(ByteArray);
		}
		catch(Exception^)
		{

		}
	}

	void SocketGroupInterface::CloseConnection()
	{
		LOG_MESSAGE(String::Format("Connection {0} closed", mID));
		try
		{
			mTransmitData = false;
			if (mServerNetworkStream != nullptr)
			{
				mServerNetworkStream->Close();
				mServerNetworkStream = nullptr;
			}
			if (mServerSocket != nullptr)
			{
				mServerSocket->Close();
				mServerSocket = nullptr;
			}
		}
		catch (Exception^)
		{

		}
	}



	void SocketGroupInterface::WaitForCommand()
	{
		// The ServerNetworkStream can be used to send data, but also to receive. To receive asynchronously, 
		// begin reading from this port and if something is received, put this into the CommandArray, up to 15 bytes
		try
		{
			mCommandArray = gcnew array<Byte>(128);
			mServerNetworkStream->BeginRead(mCommandArray, 0, 128,
				gcnew System::AsyncCallback(this, &SocketGroup::CommandReceived), mServerNetworkStream);
		}
		catch (System::ObjectDisposedException^)
		{
		}
		catch (Exception^)
		{
			if (mServerSocket != nullptr && !mServerSocket->Connected)
			{
				CloseConnection();
			}
		}
	}

	bool SocketGroupInterface::SendTestData()
	{
		array< Byte >^ data = gcnew array< Byte >(16);
		for (int i = 0; i < 16; i++)
			data[i] = 'A' + i;
		return SendData(data);
	}

	void SocketGroupInterface::CommandReceived(IAsyncResult^ ar)
	{
		OnCommandReceived(ar);
	}
}