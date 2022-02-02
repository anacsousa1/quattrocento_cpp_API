#pragma once
#include <setupapi.h>

namespace OTComm 
{	
	BOOL GetDevicePath(IN  LPGUID InterfaceGuid, __out_ecount(BufLen) PWSTR DevicePath, __in size_t BufLen, int DeviceIndex);
}
