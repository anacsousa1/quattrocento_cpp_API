#include "ErrorDescription.h"
using namespace System;
namespace OTComm {

	ErrorDescription::ErrorDescription(Exception^ exception)
	{
		e = exception;
	}

	String^ ErrorDescription::GetMessage()
	{
		return e->Message;
	}

	String^ ErrorDescription::GetFunction()
	{
		return e->Source;
	}

	int ErrorDescription::GetCode()
	{
		//TODO 
		return 0;
	}

	String ^ ErrorDescription::ToString()
	{
		return String::Format("{0} ({1}) in {2}", GetMessage(), GetCode(), GetFunction());
	}
}