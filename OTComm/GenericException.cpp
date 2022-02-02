#pragma once
#include "windows.h"
#include "GenericException.h"

using namespace System;

namespace OTComm {


	OTCommException::OTCommException(String^ source) :Exception(source)
	{
		Source = source;
		HResult = GetLastError();
		LOG_EXCEPTION(this);
	}

	OTCommException::OTCommException(String^ source, String^ message) :Exception(message)
	{
		Source = source;
		HResult = GetLastError();
		LOG_EXCEPTION(this);
	}

	OTCommException::OTCommException(String^ source, Exception^ innerException) :Exception("", InnerException)
	{
		Source = source;
		HResult = GetLastError();
		LOG_EXCEPTION(this);
	}

	OTCommException::OTCommException(String^ source, String^ message, Exception^ innerException) :Exception(message, InnerException)
	{
		Source = source;
		HResult = GetLastError();
		LOG_EXCEPTION(this);
	}
	
	WSAException::WSAException(String^ source, String^ message) :OTCommException(source, mCustomMessage + message)
	{
		HResult = WSAGetLastError();
	}
	WSAException::WSAException(String^ source, String^ message, Exception^ inner) :OTCommException(source, mCustomMessage + message, inner)
	{
		HResult = WSAGetLastError();
	}

}