#pragma once
#include <Windows.h>
#include "DebugLog.h"
#include "SocketGroupInterface.h"
#include "DataConsumerInterface.h"


using namespace System;
using namespace System::Net;
using namespace System::Net::Sockets; 


namespace OTComm {

	ref class DataConsumerSocket;	

	///<summary>
	/// Socket group rapresentation
	///</summary>
	private ref class SocketGroup:public SocketGroupInterface
	{
	protected:
		///<summary>
		/// parent of socket group
		///</summary>
		DataConsumerSocket^ mParent;
	public:
		///<summary>
		/// Constructor
		///</summary>
		///<param name="Parent">DataConsumer of the socket</param>
		SocketGroup(DataConsumerSocket^ Parent);
		///<summary>
		/// Method to manage the socket connection when a command is received
		///</summary>
		///<param name="ar">The result of asynchronous operation</param>
		virtual void OnCommandReceived(IAsyncResult^ ar) override;
	};

	///<summary>
	/// DataConsumerInterface for sharing data on the socket
	///</summary>
	public ref  class DataConsumerSocket : public DataConsumerInterface
	{
	public:
		static System::Threading::ManualResetEvent^ allDone = gcnew System::Threading::ManualResetEvent(false);
		///<summary>
		/// Constructor
		///</summary>
		///<param name="Port">Port of the socket</param>
		DataConsumerSocket(int Port);
		~DataConsumerSocket();
		///<summary>
		/// Get Description of consumer
		///</summary>
		///<returns>Returns the description of consumer</returns>
		virtual String^ GetConsumerDescription() override;
		///<summary>
		/// Get XML configuration of consumer
		///</summary>
		///<returns>Returns the XML configuration of consumer</returns>
		virtual String^ GetConsumerXMLConfiguration() override;
		///<summary>
		/// Associate the DataGenerator to the DataConsumer
		///</summary>
		///<param name="gen">DataGeneratorInterface to associate to the DataConsumer</param>
		virtual void Register(DataGeneratorInterface^ gen, __int16 sourceindex) override;
		///<summary>
		/// Remove the DataGenerator from the DataConsumer
		///</summary>
		///<param name="gen">DataGeneratorInterface to remove from the DataConsumer</param>
		///<returns>Returns -1 if error, 0 if everything is fine</returns>
		virtual void Unregister(DataGeneratorInterface^ gen) override;

	private:
		///<summary>
		/// Method to set a listening socket for establish a connection
		///</summary>
		void StartListening(void);

		///<summary>
		/// Method to evaluate when client has accepted the connection
		///</summary>
		void ConnectionAccepted(IAsyncResult^ ar);

		///<summary>
		/// Method to close the connections with all the clients
		///</summary>
		void CloseAllConnections();
		//array< Byte >^ ReceiveData(int length);

		///<summary>
		/// Buffer filled for data transmission
		///</summary>
		array<unsigned char>^ mBuffer;

		///<summary>
		/// Socket to listen on for incoming connection (after client has connected, this socket is closed)
		///</summary>
		Socket^	mListeningSocket;

		///<summary>
		/// Endpoint for communcation: IPAdress:Port
		///</summary>
		IPEndPoint^	mListeningEndPoint;

		///<summary>
		/// indicates if waiting for connection. Is set to true after successful connection has been established
		///</summary>
		bool mAwaitingConnection;

		///<summary>
		/// List of all the accepted connection.
		///</summary>
		List<SocketGroup^>^ mSocketGroups;		

		///<summary>
		/// method raised when a new data packet is available
		///</summary>
		virtual void OnNewDataAvailable(System::Object^ sender, System::EventArgs^ e) sealed;
	};
}

