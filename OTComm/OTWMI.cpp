#include "OTWMI.h"
#using <system.management.dll>

using namespace System;
using namespace System::Management;

namespace OTComm
{
	OTWMI::OTWMI()
	{
		/*try
		{
			ManagementObjectSearcher ^searcher =
				gcnew ManagementObjectSearcher("root\\CIMV2",
				"SELECT Name, hardwareID FROM Win32_PnPEntity where Name like '%(COM%'");
			for each(ManagementObject ^queryObj in searcher->Get())
			{
				array<String^>^ a = (array<String^>^)queryObj["hardwareID"];
				Console::WriteLine("Name: {0}, description: {1}", queryObj["Name"], a[0]);
			}
		}
		catch (ManagementException^)
		{
		}*/
	}

	String^ OTWMI::WindowsVersion(void)
	{
		try
		{
			ManagementObjectSearcher ^searcher =
				gcnew ManagementObjectSearcher("root\\CIMV2",
					"SELECT Caption FROM Win32_OperatingSystem");
			for each(ManagementObject ^queryObj in searcher->Get())
			{
				String^ a = (String^)queryObj["Caption"];
				if (a != nullptr) return a;
			}
		}
		catch (ManagementException^)
		{
		}
		return "unknown";
	}

	array<String^>^ OTWMI::GetUSBSerialPort(String^ target)
	{
		int counterSerialConnection = 0;
		int counterAdded = 0;
		try
		{
			ManagementObjectSearcher ^searcher =
				gcnew ManagementObjectSearcher("root\\CIMV2",
					"SELECT PNPDeviceID,Caption FROM Win32_PnPEntity");
			for each(ManagementObject ^queryObj in searcher->Get())
			{
				String^ caption = (String^)queryObj["Caption"];
				String^ id = (String^)queryObj["PNPDeviceID"];
				int pos1 = id->IndexOf(target);
				if (pos1 != -1 && id->Length >= target->Length && id->Substring(pos1, 17) == target)
				{
					if (caption->Contains("(COM"))
					{
						counterSerialConnection++;
					}
				}
			}
			array<String^>^ listSerialConnection = gcnew array<String^>(counterSerialConnection);
			for each(ManagementObject ^queryObj in searcher->Get())
			{
				String^ caption = (String^)queryObj["Caption"];
				String^ id = (String^)queryObj["PNPDeviceID"];
				int pos1 = id->IndexOf(target);
				if (pos1 != -1 && id->Length >= target->Length && id->Substring(pos1, 17) == target)
				{
					if (caption->Contains("(COM"))
					{
						int pos3 = caption->IndexOf("COM");
						String^ afterCom = caption->Substring(pos3 + 3);
						int pos4 = afterCom->IndexOf(")");
						listSerialConnection[counterAdded]=("\\\\.\\COM" + afterCom->Substring(0, pos4));
						counterAdded++;
					}
				}
			}
			return listSerialConnection;
		}
		catch (ManagementException^)
		{
		}
		return gcnew array<String^>(0);
	}
	
	array<String^>^ OTWMI::GetUSBStandardSerialPort()
	{
		String^ target = "VID_0403+PID_6001";
		int counterSerialConnection = 0;
		int counterAdded = 0;
		try
		{
			ManagementObjectSearcher ^searcher =
				gcnew ManagementObjectSearcher("root\\CIMV2",
					"SELECT PNPDeviceID,Caption FROM Win32_PnPEntity");
			for each(ManagementObject ^queryObj in searcher->Get())
			{
				String^ caption = (String^)queryObj["Caption"];
				String^ id = (String^)queryObj["PNPDeviceID"];
				int pos1 = id->IndexOf(target);
				if (pos1 != -1 && id->Length >= target->Length && id->Substring(pos1, 17) == target)
				{
					if (caption->Contains("(COM"))
					{
						counterSerialConnection++;
					}
				}
			}
			array<String^>^ listSerialConnection = gcnew array<String^>(counterSerialConnection);
			for each(ManagementObject ^queryObj in searcher->Get())
			{
				String^ caption = (String^)queryObj["Caption"];
				String^ id = (String^)queryObj["PNPDeviceID"];
				int pos1 = id->IndexOf(target);
				if (pos1 != -1 && id->Length >= target->Length && id->Substring(pos1, 17) == target)
				{
					if (caption->Contains("(COM"))
					{
						int pos3 = caption->IndexOf("COM");
						String^ afterCom = caption->Substring(pos3 + 3);
						int pos4 = afterCom->IndexOf(")");
						listSerialConnection[counterAdded] = ("\\\\.\\COM" + afterCom->Substring(0, pos4));
						counterAdded++;
					}
				}
			}
			return listSerialConnection;
		}
		catch (ManagementException^)
		{
		}
		return gcnew array<String^>(0);
	}

	array<String^>^ OTWMI::GetUSBDueProSerialPort()
	{
		String^ target = "VID_0483&PID_5740";
		int counterSerialConnection = 0;
		int counterAdded = 0;
		try
		{
			ManagementObjectSearcher ^searcher =
				gcnew ManagementObjectSearcher("root\\CIMV2",
					"SELECT PNPDeviceID,Caption FROM Win32_PnPEntity");
			for each(ManagementObject ^queryObj in searcher->Get())
			{
				String^ caption = (String^)queryObj["Caption"];
				String^ id = (String^)queryObj["PNPDeviceID"];
				
				int pos1 = id->IndexOf(target);
				if (pos1 != -1 && id->Length >= target->Length && id->Substring(pos1, 17) == target)
				{
					if (caption->Contains("(COM"))
					{
						counterSerialConnection++;
					}
				}
			}
			array<String^>^ listSerialConnection = gcnew array<String^>(counterSerialConnection);
			for each(ManagementObject ^queryObj in searcher->Get())
			{
				String^ caption = (String^)queryObj["Caption"];
				String^ id = (String^)queryObj["PNPDeviceID"];
				int pos1 = id->IndexOf(target);
				if (pos1 != -1 && id->Length >= target->Length && id->Substring(pos1, 17) == target)
				{
					if (caption->Contains("(COM"))
					{
						int pos3 = caption->IndexOf("(COM");
						String^ afterCom = caption->Substring(pos3 + 4);
						int pos4 = afterCom->IndexOf(")");
						listSerialConnection[counterAdded] = ("\\\\.\\COM" + afterCom->Substring(0, pos4));
						counterAdded++;
					}
				}
			}
			return listSerialConnection;
		}
		catch (ManagementException^)
		{
		}
		return gcnew array<String^>(0);
	}

	array<String^>^ OTWMI::GetPairedQuattro()
	{
		int counterQuattro = 0;
		int counterAdded = 0;
		if (OTWMI::WindowsVersion()->Contains("Microsoft Windows 8") || OTWMI::WindowsVersion()->Contains("Microsoft Windows 10"))
		{
			try
			{
				ManagementObjectSearcher ^searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT Caption FROM Win32_PnPEntity where Caption like 'QU%' ");
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ aa = (String^)queryObj["Caption"];
					if (aa->Length == 5)
					{
						counterQuattro++;
					}
				}
				array<String^>^ listQuattro = gcnew array<String^>(counterQuattro);
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ aa = (String^)queryObj["Caption"];
					if (aa->Length == 5)
					{
						listQuattro[counterAdded] = aa;
						counterAdded++;
					}
				}
				return listQuattro;
			}
			catch (ManagementException^)
			{

			}

		}
		else if (OTWMI::WindowsVersion()->Contains("Microsoft Windows 7"))
		{
			try
			{
				ManagementObjectSearcher ^searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT PNPDeviceID,Caption FROM Win32_PnPEntity where Caption like '%(COM%' ");
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ appoggioID = (String^)queryObj["PNPDeviceID"];
					int appoggio_count = appoggioID->Split('&')->Length;
					if (appoggio_count < 4)
						continue;
					appoggioID = appoggioID->Split('&')[4]->Split('_')[0];
					if (appoggioID->Length == 12 && !(appoggioID->ToString() == "000000000000"))
					{
						counterQuattro++;
					}
				}
				array<String^>^ listQuattro = gcnew array<String^>(counterQuattro);
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ appoggioID = (String^)queryObj["PNPDeviceID"];
					int appoggio_count = appoggioID->Split('&')->Length;
					if (appoggio_count < 4)
						continue;
					appoggioID = appoggioID->Split('&')[4]->Split('_')[0];
					if (appoggioID->Length == 12 && !(appoggioID->ToString() == "000000000000"))
					{
						String^ appoggioCaption = (String^)queryObj["Caption"];
						appoggioCaption = appoggioCaption->Substring(appoggioCaption->IndexOf("(COM"));
						appoggioCaption = appoggioCaption->Replace("(", "");
						appoggioCaption = appoggioCaption->Replace(")", "");
						listQuattro[counterAdded] = appoggioCaption;
						counterAdded++;
					}
				}
				return listQuattro;
			}
			catch (ManagementException^)
			{

			}

		}

		return gcnew array<String^>(0);
	}

	array<String^>^ OTWMI::GetPairedForza()
	{
		int counterQuattro = 0;
		int counterAdded = 0;
		if (OTWMI::WindowsVersion()->Contains("Microsoft Windows 8") || OTWMI::WindowsVersion()->Contains("Microsoft Windows 10"))
		{
			try
			{
				ManagementObjectSearcher ^searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT Caption FROM Win32_PnPEntity where Caption like 'FZ%' ");
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ aa = (String^)queryObj["Caption"];
					if (aa->Length == 5)
					{
						counterQuattro++;
					}
				}
				array<String^>^ listQuattro = gcnew array<String^>(counterQuattro);
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ aa = (String^)queryObj["Caption"];
					if (aa->Length == 5)
					{
						listQuattro[counterAdded] = aa;
						counterAdded++;
					}
				}
				return listQuattro;
			}
			catch (ManagementException^)
			{

			}

		}
		else if (OTWMI::WindowsVersion()->Contains("Microsoft Windows 7"))
		{
			try
			{
				ManagementObjectSearcher ^searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT PNPDeviceID,Caption FROM Win32_PnPEntity where Caption like '%(COM%' ");
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ appoggioID = (String^)queryObj["PNPDeviceID"];
					int appoggio_count = appoggioID->Split('&')->Length;
					if (appoggio_count < 4)
						continue;
					appoggioID = appoggioID->Split('&')[4]->Split('_')[0];
					if (appoggioID->Length == 12 && !(appoggioID->ToString() == "000000000000"))
					{
						counterQuattro++;
					}
				}
				array<String^>^ listQuattro = gcnew array<String^>(counterQuattro);
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ appoggioID = (String^)queryObj["PNPDeviceID"];
					int appoggio_count = appoggioID->Split('&')->Length;
					if (appoggio_count < 4)
						continue;
					appoggioID = appoggioID->Split('&')[4]->Split('_')[0];
					if (appoggioID->Length == 12 && !(appoggioID->ToString() == "000000000000"))
					{
						String^ appoggioCaption = (String^)queryObj["Caption"];
						appoggioCaption = appoggioCaption->Substring(appoggioCaption->IndexOf("(COM"));
						appoggioCaption = appoggioCaption->Replace("(", "");
						appoggioCaption = appoggioCaption->Replace(")", "");
						listQuattro[counterAdded] = appoggioCaption;
						counterAdded++;
					}
				}
				return listQuattro;
			}
			catch (ManagementException^)
			{

			}

		}

		return gcnew array<String^>(0);
	}

	array<String^>^ OTWMI::GetPairedNOD()
	{
		int counterQuattro = 0;
		int counterAdded = 0;
		if (OTWMI::WindowsVersion()->Contains("Microsoft Windows 8") || OTWMI::WindowsVersion()->Contains("Microsoft Windows 10"))
		{
			try
			{
				ManagementObjectSearcher ^searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT Caption FROM Win32_PnPEntity where Caption like 'NOD%' ");
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ aa = (String^)queryObj["Caption"];
					if (aa->Length == 6)
					{
						counterQuattro++;
					}
				}
				array<String^>^ listQuattro = gcnew array<String^>(counterQuattro);
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ aa = (String^)queryObj["Caption"];
					if (aa->Length == 6)
					{
						listQuattro[counterAdded] = aa;
						counterAdded++;
					}
				}
				return listQuattro;
			}
			catch (ManagementException^)
			{

			}

		}
		else if (OTWMI::WindowsVersion()->Contains("Microsoft Windows 7"))
		{
			try
			{
				ManagementObjectSearcher ^searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT PNPDeviceID,Caption FROM Win32_PnPEntity where Caption like '%(COM%' ");
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ appoggioID = (String^)queryObj["PNPDeviceID"];
					int appoggio_count = appoggioID->Split('&')->Length;
					if (appoggio_count < 4)
						continue;
					appoggioID = appoggioID->Split('&')[4]->Split('_')[0];
					if (appoggioID->Length == 12 && !(appoggioID->ToString() == "000000000000"))
					{
						counterQuattro++;
					}
				}
				array<String^>^ listQuattro = gcnew array<String^>(counterQuattro);
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					String^ appoggioID = (String^)queryObj["PNPDeviceID"];
					int appoggio_count = appoggioID->Split('&')->Length;
					if (appoggio_count < 4)
						continue;
					appoggioID = appoggioID->Split('&')[4]->Split('_')[0];
					if (appoggioID->Length == 12 && !(appoggioID->ToString() == "000000000000"))
					{
						String^ appoggioCaption = (String^)queryObj["Caption"];
						appoggioCaption = appoggioCaption->Substring(appoggioCaption->IndexOf("(COM"));
						appoggioCaption = appoggioCaption->Replace("(", "");
						appoggioCaption = appoggioCaption->Replace(")", "");
						listQuattro[counterAdded] = appoggioCaption;
						counterAdded++;
					}
				}
				return listQuattro;
			}
			catch (ManagementException^)
			{

			}

		}

		return gcnew array<String^>(0);
	}

	String^ OTWMI::GetCaptionFromPort(String ^ port)
	{
		String^ DeviceName = port;
		DeviceName = DeviceName->Replace("\\\\.\\", "");

		if (OTWMI::WindowsVersion()->Contains("Microsoft Windows 8") || OTWMI::WindowsVersion()->Contains("Microsoft Windows 10"))
		{
			try
			{
				ManagementObjectSearcher ^searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT DeviceID FROM Win32_PnPEntity where Name like '%(" + DeviceName + "%' and DeviceID like '%THENUM%' ");//Name like '%(COM%' and
				String^ DeviceID = "";
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					DeviceID = (String^)queryObj["DeviceID"];
					DeviceID = DeviceID->Substring(DeviceID->IndexOf("_LOCALMFG") + 1);
					DeviceID = DeviceID->Substring(0, DeviceID->IndexOf("_"));
					DeviceID = DeviceID->Substring(DeviceID->Length - 12);
				}
				searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT Caption FROM Win32_PnPEntity where DeviceID like '%" + DeviceID + "' ");
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					DeviceName = (String^)queryObj["Caption"];
					return DeviceName;
				}
			}
			catch (ManagementException^)
			{

			}


			
		}

		return DeviceName;
	}
	
	String ^ OTWMI::GetPortFromCaption(String ^ caption)
	{
		String^ serialPort = "COM0";

		if (OTWMI::WindowsVersion()->Contains("Microsoft Windows 8") || OTWMI::WindowsVersion()->Contains("Microsoft Windows 10"))
		{
			try
			{
				ManagementObjectSearcher ^searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT PNPDeviceID FROM Win32_PnPEntity where Caption like '" + caption + "' ");
				String^ DeviceID = "";
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					DeviceID = (String^)queryObj["PNPDeviceID"];
					DeviceID = DeviceID->Substring(DeviceID->IndexOf("BLUETOOTHDEVICE"));
					DeviceID = DeviceID->Replace("BLUETOOTHDEVICE_", "");
				}
				searcher =
					gcnew ManagementObjectSearcher("root\\CIMV2",
						"SELECT Name FROM Win32_PnPEntity where Name like '%(COM%' and DeviceID like '%THENUM%' and  not DeviceID like '%LOCALMFG&0000%' and DeviceID like '%" + DeviceID + "%' ");
				String^ serialPortName = "";
				for each(ManagementObject ^queryObj in searcher->Get())
				{
					serialPortName = (String^)queryObj["Name"];
					serialPortName = serialPortName->Substring(serialPortName->IndexOf("(COM"));
					serialPortName = serialPortName->Replace("(", "");
					serialPortName = serialPortName->Replace(")", "");
					return "\\\\.\\" + serialPortName;
				}
			}
			catch (ManagementException^)
			{
				return "\\\\.\\COM0";
			}

			return serialPort;
		}
		else return caption;

	}
}