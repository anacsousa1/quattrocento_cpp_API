#include "DebugLog.h"
#include <ctime>
#include <stdio.h>
#include <Windows.h>

using namespace System;
using namespace System::Runtime::InteropServices;
namespace OTComm {
	
	inline void DebugLog::Print(char *str)
	{
#ifdef _DEBUG
		char TmpBuffer[256];
		time_t rawtime;
		time(&rawtime);
		char TimeBuffer[256];
		ctime_s(TimeBuffer, sizeof(TimeBuffer), &rawtime);
		TimeBuffer[strlen(TimeBuffer) - 1] = 0;
		sprintf_s(TmpBuffer, sizeof(TmpBuffer), " %s- %s", TimeBuffer, str);
		OutputDebugStringA(TmpBuffer);
#endif
	}

	inline void DebugLog::Print(String^ str)
	{
#ifdef _DEBUG
		System::Diagnostics::Debug::Print(str);
#endif
	}
	inline void DebugLog::PrintLastError()
	{
#ifdef _DEBUG
		char TmpBuffer[512];
		int lasterr = GetLastError();
		sprintf_s(TmpBuffer, sizeof(TmpBuffer), " LastError is %d (0x%x): ", lasterr, lasterr);
		DebugLog::Print(TmpBuffer);
#endif
	}

	inline void DebugLog::PrintLastError(char* description, char* function)
	{
#ifdef _DEBUG
		char TmpBuffer[256];
		int lasterr = GetLastError();
		sprintf_s(TmpBuffer, sizeof(TmpBuffer), "%s:error:%s [%d(0x%x)]", function, description, lasterr, lasterr);
		DebugLog::Print(TmpBuffer);
#endif
	}

	inline void DebugLog::PrintException(System::Exception^ exc)
	{
#ifdef _DEBUG
		//TODO test this code, rewritten to avoid the following warning
		//Warning	C4477	'sprintf_s' : format string '%s' requires an argument of type 'char *', but variadic argument 1 has type 'System::String ^'
		char TmpBuffer[1025];
		int i,j;
		for (i = 0; i < exc->Source->Length && i<1024-7; i++)
			TmpBuffer[i] = static_cast<char>(exc->Source[i]);
		strcat_s(&TmpBuffer[i], 1025-i, ":error:");
		i += 7;
		for (j = 0;  j<exc->Message->Length &&i+j<1024; j++)
			TmpBuffer[i+j] = static_cast<char>(exc->Message[j]);
		TmpBuffer[i + j] = 0;
		DebugLog::Print(TmpBuffer);
		int lasterr = GetLastError();
		sprintf_s(TmpBuffer, sizeof(TmpBuffer), "LastError: %d(0x%x)", lasterr, lasterr);
		DebugLog::Print(TmpBuffer);

#endif
	}
}