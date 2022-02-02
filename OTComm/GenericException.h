#pragma once
#include "DebugLog.h"

using namespace System;

namespace OTComm {

	public ref class OTCommException : Exception
	{
	public:
		OTCommException(String^ source) ;
		OTCommException(String^ source, String^ message) ;
		OTCommException(String^ source, Exception^ innerException) ;
		OTCommException(String^ source, String^ message, Exception^ innerException) ;
	};
	
	public ref class ConstructorFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Constructor failed.";
	public:
		ConstructorFailedException(String^ source) :OTCommException(source, mCustomMessage) {}
		ConstructorFailedException(String^ source, String^ message) : OTCommException(source, message) {}
		ConstructorFailedException(String^ source, Exception^ inner) : OTCommException(source, mCustomMessage, inner) {}
		ConstructorFailedException(String^ source, String^ message, Exception^ inner) : OTCommException(source, message, inner) {}
	};

	public ref class OpenDeviceFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Oped device failed.";
	public:
		OpenDeviceFailedException(String^ source) :OTCommException(source, mCustomMessage) {}
		OpenDeviceFailedException(String^ source, String^ message) : OTCommException(source, message) {}
		OpenDeviceFailedException(String^ source, Exception^ inner) : OTCommException(source, mCustomMessage, inner) {}
		OpenDeviceFailedException(String^ source, String^ message, Exception^ inner) : OTCommException(source, message, inner) {}
	};
	
	public ref class CloseDeviceFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Close device failed.";
	public:
		CloseDeviceFailedException(String^ source):OTCommException(source, mCustomMessage){}
		CloseDeviceFailedException(String^ source, String^ message): OTCommException(source, message){}
		CloseDeviceFailedException(String^ source, Exception^ inner): OTCommException(source, mCustomMessage, inner){}
		CloseDeviceFailedException(String^ source, String^ message, Exception^ inner): OTCommException(source, message, inner){}
	};

	public ref class InitializeDeviceFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Initialize device failed.";
	public:
		InitializeDeviceFailedException(String^ source):OTCommException(source, mCustomMessage){}
		InitializeDeviceFailedException(String^ source, String^ message):OTCommException(source, message){}
		InitializeDeviceFailedException(String^ source, Exception^ inner):OTCommException(source, mCustomMessage, inner){}
		InitializeDeviceFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class NoAdaptersConnectedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "At least one adapter must be connected to the generator trough a connector.";
	public:
		NoAdaptersConnectedException(String^ source):OTCommException(source, mCustomMessage){}
		NoAdaptersConnectedException(String^ source, String^ message):OTCommException(source, message){}
		NoAdaptersConnectedException(String^ source, Exception^ inner):OTCommException(source, mCustomMessage, inner){}
		NoAdaptersConnectedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class StartAcquisitionFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Start acquisition failed.";
	public:
		StartAcquisitionFailedException(String^ source):OTCommException(source, mCustomMessage){}
		StartAcquisitionFailedException(String^ source, String^ message):OTCommException(source, message){}
		StartAcquisitionFailedException(String^ source, Exception^ inner):OTCommException(source, mCustomMessage, inner){}
		StartAcquisitionFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class StopAcquisitionFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Stop acquisition failed.";
	public:
		StopAcquisitionFailedException(String^ source):OTCommException(source, mCustomMessage){}
		StopAcquisitionFailedException(String^ source, String^ message):OTCommException(source, message){}
		StopAcquisitionFailedException(String^ source, Exception^ inner):OTCommException(source, inner){}
		StopAcquisitionFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class WSAException : OTCommException
	{
	private:
		static String^ mCustomMessage = "A WSA error was returned by function ";
	public:
		WSAException(String^ source, String^ message);
		WSAException(String^ source, String^ message, Exception^ inner);
	};

	public ref class StartRecordingFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Start recording failed.";
	public:
		StartRecordingFailedException(String^ source):OTCommException(source, mCustomMessage){}
		StartRecordingFailedException(String^ source, String^ message):OTCommException(source, message){}
		StartRecordingFailedException(String^ source, Exception^ inner):OTCommException(source, mCustomMessage, inner){}
		StartRecordingFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class StopRecordingFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Stop recording failed.";
	public:
		StopRecordingFailedException(String^ source):OTCommException(source, mCustomMessage){}
		StopRecordingFailedException(String^ source, String^ message):OTCommException(source, message){}
		StopRecordingFailedException(String^ source,  Exception^ inner):OTCommException(source, inner){}
		StopRecordingFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class InvalidFileException :OTCommException
	{
	private:
		static String^ mCustomMessage = "An operation was tried on an invalid file.";
	public:
		InvalidFileException(String^ source):OTCommException(source, mCustomMessage){}
		InvalidFileException(String^ source, String^ message): OTCommException(source, message){}
		InvalidFileException(String^ source, String^ message, Exception^ inner): OTCommException(source, message, inner){}
	};

	public ref class RecordInProgressException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Recording is already in progress.";
	public:
		RecordInProgressException(String^ source):OTCommException(source, mCustomMessage){}
		RecordInProgressException(String^ source, String^ message):OTCommException(source, message){}
		RecordInProgressException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class InvalidHandleException : OTCommException
	{
	private:
		static String^ mCustomMessage = "An operation on an invalid handle was tried.";
	public:
		InvalidHandleException(String^ source):OTCommException(source, mCustomMessage){}
		InvalidHandleException(String^ source, String^ message):OTCommException(source, message){}
		InvalidHandleException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class InvalidConnectorException : OTCommException
	{
	private:
		static String^ mCustomMessage = "The connector object was not initialized.";
	public:
		InvalidConnectorException(String^ source) :OTCommException(source, mCustomMessage) {}
		InvalidConnectorException(String^ source, String^ message):OTCommException(source, message){}
		InvalidConnectorException(String^ source, String^ message, Exception^ inner) :OTCommException(source, message, inner) {}
	};

	public ref class DeviceIoControlException : OTCommException
	{
	private:
		static String^ mCustomMessage = "A DeviceIOControl operation returned an error.";
	public:
		DeviceIoControlException(String^ source):OTCommException(source, mCustomMessage){}
		DeviceIoControlException(String^ source, String^ message):OTCommException(source, mCustomMessage + message){}
		DeviceIoControlException(String^ source, String^ message, Exception^ inner):OTCommException(source, mCustomMessage + message, inner){}
	};

	public ref class InvalidDevicePath : OTCommException
	{
	private:
		static String^ mCustomMessage = "The supplied device path is invalid.";
	public:
		InvalidDevicePath(String^ source):OTCommException(source, mCustomMessage){}
		InvalidDevicePath(String^ source, String^ message):OTCommException(source, message){}
		InvalidDevicePath(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class DeviceIsClosedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "An operation on a closed device was tried.";
	public:
		DeviceIsClosedException(String^ source) :OTCommException(source, mCustomMessage) {}
		DeviceIsClosedException(String^ source, String^ message):OTCommException(source, message){}
		DeviceIsClosedException(String^ source, String^ message, Exception^ inner) :OTCommException(source, message, inner) {}
	};

	public ref class InvalidConnectorNumberException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Invalid port number";
	public:
		InvalidConnectorNumberException(String^ source):OTCommException(source, mCustomMessage){}
		InvalidConnectorNumberException(String^ source, String^ message):OTCommException(source, message){}
		InvalidConnectorNumberException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class InvalidOddChannelsNumberException : OTCommException
	{
	private:
		static String^ mCustomMessage = "The number of channels must be an odd number";
	public:
		InvalidOddChannelsNumberException(String^ source):OTCommException(source, mCustomMessage){}
		InvalidOddChannelsNumberException(String^ source, String^ message):OTCommException(source, message){}
		InvalidOddChannelsNumberException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class InvalidChannelsNumberException : OTCommException
	{
	private:
		static String^ mCustomMessage = "An invalid channel number was provided.";
	public:
		InvalidChannelsNumberException(String^ source):OTCommException(source,mCustomMessage){}
		InvalidChannelsNumberException(String^ source, String^ message):OTCommException(source, message){}
		InvalidChannelsNumberException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};
	
	public ref class OverlappedCommunicationException : OTCommException
	{
	private:
		static String^ mCustomMessage = "An error occurred during an overlappedIO communication.";
	public:
		OverlappedCommunicationException(String^ source):OTCommException(source, mCustomMessage){}
		OverlappedCommunicationException(String^ source, String^ message):OTCommException(source, message){}
		OverlappedCommunicationException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class OverlappedStructException : OTCommException
	{
	private:
		static String^ mCustomMessage = "An error occurred creating the overlappedIO structure.";
	public:
		OverlappedStructException(String^ source):OTCommException(source, mCustomMessage){}
		OverlappedStructException(String^ source, String^ message):OTCommException(source, message){}
		OverlappedStructException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};
	
	public ref class CompletionPortException : OTCommException
	{
	private:
		static String^ mCustomMessage = "An error occurred creating the completion port.";
	public:
		CompletionPortException(String^ source):CompletionPortException(source, mCustomMessage){}
		CompletionPortException(String^ source, String^ message):OTCommException(source, message){}
		CompletionPortException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};



	public ref class SetCommFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "A call to SetCommXXX function returned an error.";
	public:
		SetCommFailedException(String^ source):OTCommException(source, mCustomMessage){}
		SetCommFailedException(String^ source, String^ message):OTCommException(source, message){}
		SetCommFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class GetCommFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "A call to GetCommXXX function returned an error.";
	public:
		GetCommFailedException(String^ source):OTCommException(source, mCustomMessage){}
		GetCommFailedException(String^ source, String^ message):OTCommException(source, message){}
		GetCommFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class GetDeviceIDFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Get Device ID failed.";
	public:
		GetDeviceIDFailedException(String^ source):OTCommException(source, mCustomMessage){}
		GetDeviceIDFailedException(String^ source, String^ message):OTCommException(source, message){}
		GetDeviceIDFailedException(String^ source, Exception^ inner):OTCommException(source, mCustomMessage, inner){}
		GetDeviceIDFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};
	
	public ref class DeviceNotFoundException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Get Device ID failed.";
	public:
		DeviceNotFoundException(String^ source) :OTCommException(source, mCustomMessage) {}
		DeviceNotFoundException(String^ source, String^ message) :OTCommException(source, message) {}
		DeviceNotFoundException(String^ source, Exception^ inner) :OTCommException(source, mCustomMessage, inner) {}
		DeviceNotFoundException(String^ source, String^ message, Exception^ inner) :OTCommException(source, message, inner) {}

	};

	public ref class AcquisitionThreadException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Acquisition thread creation failed.";
	public:
		AcquisitionThreadException(String^ source):OTCommException(source, mCustomMessage){}
		AcquisitionThreadException(String^ source, String^ message):OTCommException(source, message){}
		AcquisitionThreadException(String^ source,  Exception^ inner):OTCommException(source, mCustomMessage, inner){}
		AcquisitionThreadException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class ThreadCreationException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Thread creation failed.";
	public:
		ThreadCreationException(String^ source):OTCommException(source, mCustomMessage){}
		ThreadCreationException(String^ source, String^ message):OTCommException(source, message){}
		ThreadCreationException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class ProgramFPGAFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage="FPGA program failed.";
	public:
		ProgramFPGAFailedException(String^ source):OTCommException(source, mCustomMessage){}
		ProgramFPGAFailedException(String^ source, String^ message):OTCommException(source, message){}
		ProgramFPGAFailedException(String^ source, Exception^ inner):OTCommException(source, mCustomMessage, inner){}
		ProgramFPGAFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};
	
	public ref class GetFirmwareFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "Firmware extraction failed.";
	public:
		GetFirmwareFailedException(String^ source):OTCommException(source, mCustomMessage){}
		GetFirmwareFailedException(String^ source, String^ message):OTCommException(source, message){}
		GetFirmwareFailedException(String^ source,  Exception^ inner):OTCommException(source, mCustomMessage, inner){}
		GetFirmwareFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class ProgramPICFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "PIC program failed.";
	public:
		ProgramPICFailedException(String^ source):OTCommException(source, mCustomMessage){}
		ProgramPICFailedException(String^ source, String^ message):OTCommException(source, message){}
		ProgramPICFailedException(String^ source, Exception^ inner):OTCommException(source, mCustomMessage, inner){}
		ProgramPICFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class RegistrationFailedException : OTCommException
	{
	private:
		static String^ mCustomMessage = "The (un)registration to the generator events failed.";
	public:
		RegistrationFailedException(String^ source):OTCommException(source,mCustomMessage){}
		RegistrationFailedException(String^ source, String^ message):OTCommException(source, message){}
		RegistrationFailedException(String^ source, String^ message, Exception^ inner):OTCommException(source, message, inner){}
	};

	public ref class XMLOpeningException : OTCommException
	{
	private:
		static String^ mCustomMessage = "An error occurred opening an XML file.";
	public:
		XMLOpeningException(String^ source) :OTCommException(source, mCustomMessage) {}
		XMLOpeningException(String^ source, String^ message) :OTCommException(source, message) {}
		XMLOpeningException(String^ source, String^ message, Exception^ inner) :OTCommException(source, message, inner) {}
		XMLOpeningException(String^ source, Exception^ inner) :OTCommException(source, mCustomMessage, inner) {}
	};

	public ref class InvalidSourceIndex : OTCommException
	{
	private:
		static String^ mCustomMessage = "The required source index exceeds the number of sources for this DataGenerator.";
	public:
		InvalidSourceIndex(String^ source) :OTCommException(source, mCustomMessage) {}
		InvalidSourceIndex(String^ source, String^ message) :OTCommException(source, message) {}
		InvalidSourceIndex(String^ source, String^ message, Exception^ inner) :OTCommException(source, message, inner) {}
		InvalidSourceIndex(String^ source, Exception^ inner) :OTCommException(source, mCustomMessage, inner) {}
	};

	public ref class InvalidChannelNumber : OTCommException
	{
	private:
		static String^ mCustomMessage = "The provided channel index exceeds the number of channels for this Adapter.";
	public:
		InvalidChannelNumber(String^ source) :OTCommException(source, mCustomMessage) {}
		InvalidChannelNumber(String^ source, String^ message) :OTCommException(source, message) {}
		InvalidChannelNumber(String^ source, String^ message, Exception^ inner) :OTCommException(source, message, inner) {}
		InvalidChannelNumber(String^ source, Exception^ inner) :OTCommException(source, mCustomMessage, inner) {}
	};
	
	public ref class InvalidFileTemplateName : OTCommException
	{
	private:
		static String^ mCustomMessage = "The provided file template does not contain '{0}'.";
	public:
		InvalidFileTemplateName(String^ source) :OTCommException(source, mCustomMessage) {}
		InvalidFileTemplateName(String^ source, String^ message) :OTCommException(source, message) {}
		InvalidFileTemplateName(String^ source, String^ message, Exception^ inner) :OTCommException(source, message, inner) {}
		InvalidFileTemplateName(String^ source, Exception^ inner) :OTCommException(source, mCustomMessage, inner) {}
	};
}