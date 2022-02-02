#pragma once
using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets;

namespace OTComm
{
	private ref class SocketGroupInterface abstract
	{
	protected:
		Int32 mID;
		static Int32 IDCounter;
	public:
		SocketGroupInterface();
		Socket^			mServerSocket;			// socket for communication and data exchange, takes over 
												// from ListeningSocket once the connection is established
		NetworkStream^	mServerNetworkStream;	// NetworkStream through ServerSocket to send/receive data
		array<Byte>^	mCommandArray;			// Array to store last command received
		bool			mTransmitData;			
		void WaitForCommand();
		void CommandReceived(IAsyncResult^ ar);
		virtual void CloseConnection();
		virtual void OnCommandReceived(IAsyncResult^ ar) = 0;
		bool SendTestData();
		bool SendData(array< Byte >^ data);
		bool SendData(String^ data);
	};
}