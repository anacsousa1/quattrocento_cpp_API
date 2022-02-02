#include <Windows.h>
#include "DataConverter.h"
#include "DataConsumerSocket.h"
#include "DebugLog.h"
#include "SocketGroupInterface.h"
#include "DataConsumerInterface.h"


using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;

using namespace System::Collections::Generic;
namespace OTComm {

	

	SocketGroup::SocketGroup(DataConsumerSocket^ Parent) : mParent(Parent)
	{

	}


	void SocketGroup::OnCommandReceived(IAsyncResult^ ar)
	{
		
		NetworkStream^ ns = safe_cast<NetworkStream^>(ar->AsyncState);
		
		// is invoked in case of data (command) received.
		try
		{
			int length = ns->EndRead(ar);
			if (length == 0)
			{
				DebugLog::Print(String::Format("Connection {0} received 0 length command ", mID));
				CloseConnection();
					return;
			}

			// we received a byte array of size length, so convert this into a string for further processing
			String^ command = DataConverter::ByteArrayToString(mCommandArray, length);
			DebugLog::Print(String::Format("Connection {0} received command '{1}' with length {2}", mID, command, length));

			// next first put the server again in listening mode for new commands
			// and now check what was asked for
			if (String::Compare(command, "startTX") == 0)
			{
				mTransmitData = true;
				WaitForCommand();
			}
			else if (String::Compare(command, "getdevXMLconfig") == 0)
			{
				if (!mTransmitData)
					SendData(mParent->GetDeviceXMLConfiguration());
				WaitForCommand();
			}
			else if (String::Compare(command, "getdevdescription") == 0)
			{
				if (!mTransmitData)
					SendData(mParent->GetDeviceDescription());
				WaitForCommand();
			}
			else if (String::Compare(command, "getsocXMLconfig") == 0)
			{
				if (!mTransmitData)
					SendData(mParent->GetConsumerXMLConfiguration());
				WaitForCommand();
			}
			else if (String::Compare(command, "getsocdescription") == 0)
			{
				if (!mTransmitData)
					SendData(mParent->GetConsumerDescription());
				WaitForCommand();
			}
			else if (String::Compare(command, "stopTX") == 0)
			{
				mTransmitData = false;
				WaitForCommand();
			}

			else if (String::Compare(command, "quit") == 0)
			{
					CloseConnection();
			}
			else if (String::Compare(command, "testdata") == 0)
			{
				WaitForCommand();
				if (!mTransmitData)
					SendTestData();
			}
			else
				WaitForCommand();
		}
		catch (System::ObjectDisposedException^)
		{
		}
		catch (Exception^)
		{
			//onError(e->Message);

			// reset socket in case of error into listening mode
			if (mServerSocket!=nullptr && !mServerSocket->Connected)
			{
				CloseConnection();
			}
		}
	}

	DataConsumerSocket::DataConsumerSocket(int Port) : mBuffer(nullptr)
	{
		// Configure the ListeningEndPoint to all local interfaces and specified port
		mSocketGroups = gcnew 	List < SocketGroup^ > ();
		mListeningEndPoint = gcnew IPEndPoint(IPAddress::Any, Port); // IPAddress::Parse(IP)
		// disable transmission
		mAwaitingConnection = false;
		// Set server in listening mode to wait for incoming connections
		StartListening();
	}

	DataConsumerSocket::~DataConsumerSocket()
	{
		CloseAllConnections();
	}

	String^ DataConsumerSocket::GetConsumerDescription()
	{
		return "Socket dispatcher";
	}

	String^ DataConsumerSocket::GetConsumerXMLConfiguration()
	{
		return "";
	}

	void DataConsumerSocket::Register(DataGeneratorInterface^ gen, __int16 sourceindex)
	{
		try
		{
			this->SetDataGeneratorInterface(gen, sourceindex);
			gen->OnNewDataAvailable += gcnew OTComm::NewDataAvailable(this, &OTComm::DataConsumerSocket::OnNewDataAvailable);
		}
		catch (Exception^)
		{
			throw gcnew RegistrationFailedException(__FUNCTION__);
		}

	}

	void DataConsumerSocket::Unregister(DataGeneratorInterface^ gen)
	{
		try
		{
			this->SetDataGeneratorInterface(nullptr,-1);
			gen->OnNewDataAvailable -= gcnew OTComm::NewDataAvailable(this, &OTComm::DataConsumerSocket::OnNewDataAvailable);
		}
		catch (Exception^)
		{
			throw gcnew RegistrationFailedException(__FUNCTION__);
		}
	}

	void DataConsumerSocket::StartListening()
	{	
		// Setup a listening Socket to await a connection from a peer socket.
		mListeningSocket = gcnew Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::IP);
		mListeningSocket->Bind(mListeningEndPoint);
		mListeningSocket->Listen(5);
		mListeningSocket->BeginAccept(gcnew System::AsyncCallback(this, &DataConsumerSocket::ConnectionAccepted), mListeningSocket);
	}

	void DataConsumerSocket::ConnectionAccepted(IAsyncResult^ ar)
	{
		// Method will be evaluated when a client has connected. Therefore accept the 
		// connection and by this bind to ServerSocket
		try
		{
			DebugLog::Print("DataConsumerSocket: Accepting connection");
			SocketGroup^ sg = gcnew SocketGroup(this);
			
			sg->mServerSocket = mListeningSocket->EndAccept(ar);
			sg->mServerSocket->NoDelay = true;
			mListeningSocket->BeginAccept(gcnew System::AsyncCallback(this, &DataConsumerSocket::ConnectionAccepted), mListeningSocket);
			// Setup a network stream on the server Socket
			sg->mServerNetworkStream = gcnew NetworkStream(sg->mServerSocket, true);
			// this->onStatus("Connection Established, waiting for handshake");
			mSocketGroups->Add(sg);
			sg->SendData("OTBioLab");
			sg->WaitForCommand();
		}
		catch (System::ObjectDisposedException^)
		{
			//onStatus(e->Message);
			// Listening Socket has been disposed, no further handling
			return;
		}
		catch (Exception^)
		{
			// another error occured
			//onStatus(e->Message);
			return;
		}
		// onStatus("Socket Connected");
		//this->mAwaitingConnection = false;
		// Stop listening, to avoid other clients to connect
		//mListeningSocket->Disconnect(true);
		//mListeningSocket->Close();		
	}

	void DataConsumerSocket::OnNewDataAvailable(System::Object^ dri, System::EventArgs^ e)
	{
		if (e == nullptr)
			return;
		array<unsigned __int16>^data = ((NewDataAvailableEventArgs^)e)->Data->data16;
		if (data == nullptr)
			return;
		int length = ((NewDataAvailableEventArgs^)e)->Data->length;
		bool AtLeastOneTransmission = false;
		for (int k = mSocketGroups->Count-1; k >= 0; k--)
		{
			if (mSocketGroups[k]->mServerNetworkStream==nullptr)
				mSocketGroups->RemoveAt(k);
			else if (mSocketGroups[k]->mTransmitData)
				AtLeastOneTransmission = true;
		}

		if (AtLeastOneTransmission)
		{
			if (mBuffer == nullptr || mBuffer->Length != length*sizeof(short))
				mBuffer = gcnew array < unsigned char >(length*sizeof(short));

			Buffer::BlockCopy(data, 0, mBuffer, 0, length*sizeof(short));
			try
			{
				for each (SocketGroup^ sg in mSocketGroups)
				{
					if (sg->mTransmitData)
					{
						sg->SendData(mBuffer);
					}
				}
			}
			catch (Exception^)
			{

			}
		}
	}

	void DataConsumerSocket::CloseAllConnections()
	{
		for each (SocketGroup^ sg in mSocketGroups)
		{
			sg->CloseConnection();
		}
		
		if (mListeningSocket != nullptr)
		{
		mListeningSocket->Disconnect(true);
		mListeningSocket->Close();
		mListeningSocket = nullptr;
		}
	}
	

	

	
}