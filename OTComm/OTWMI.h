#pragma once
using namespace System;

namespace OTComm {
	///<summary>Functions helpfull for windows management instrumentation</summary>
	public ref class OTWMI
	{
	public:
		OTWMI();
		///<summary>Obtain Windows version</summary>
		///<returns>Windows version</returns>
		static String^ WindowsVersion();
		///<summary>Obtain COMPORT connected to target devices connected via USB</summary>
		///<param name="target">Identity of device to discover</param>
		///<returns>list of COMPORT connected to devices specified by target</returns>
		static array<String^>^ GetUSBSerialPort(String^ target);
		///<summary>Obtain COMPORT connected to Forza or Quattro devices connected via USB</summary>
		///<returns>list of COMPORT connected to devices Quattro or Forza</returns>
		static array<String^>^ GetUSBStandardSerialPort();
		///<summary>Obtain COMPORT connected to DUEPRO devices connected via USB</summary>
		///<returns>list of COMPORT connected to devices DUEPRO</returns>
		static array<String^>^ GetUSBDueProSerialPort();
		///<summary>Obtain COMPORT or name of Quattro devices paired via BT</summary>
		///<returns>list of COMPORT or name of Quattro devices paired via BT</returns>
		static array<String^>^ GetPairedQuattro();
		///<summary>Obtain COMPORT or name of Forza devices paired via BT</summary>
		///<returns>list of COMPORT or name of Forza devices paired via BT</returns>
		static array<String^>^ GetPairedForza();
		///<summary>Obtain COMPORT or name of NOD devices paired via BT</summary>
		///<returns>list of COMPORT or name of NOD devices paired via BT</returns>
		static array<String^>^ GetPairedNOD();
		///<summary>Get device name from COMPORT</summary>
		///<param name="port">COMPORT of device</param>
		///<returns>caption associated to COMPORT</returns>
		static String^ GetCaptionFromPort(String^ port);
		///<summary>Get device COMPORT from name</summary>
		///<param name="caption">name of device</param>
		///<returns>COMPORT associated to name</returns>
		static String^ GetPortFromCaption(String^ caption);
	};

}