#pragma once
namespace OTComm {

	///<summary>Function to print messages to console during Debug</summary>
	public ref class DebugLog abstract sealed
	{
	public:
		///<summary>Function to print messages to console during Debug</summary>
		///<param name="str">Pointer to char to print</param>
		static void Print(char *str);
		///<summary>Function to print messages to console during Debug</summary>
		///<param name="str">String to print</param>
		static void Print(System::String^ str);
		///<summary>Function to print last error to console during Debug</summary>
		static void PrintLastError(void);
		///<summary>Function to print error to console during Debug</summary>
		///<param name="description">description</param>
		///<param name="function">function</param>
		static void PrintLastError(char* description, char* function);
		static void PrintException(System::Exception^ exc);
		//static String^ GetBuffer();
	};
	
	#define LOG_ENTERING	DebugLog::Print(__FUNCTION__ ":entering")
	#define LOG_EXITING		DebugLog::Print(__FUNCTION__ ":exiting")
	#define LOG_SUCCESS		DebugLog::Print(__FUNCTION__ ":success")
	#define LOG_MESSAGE(x)	DebugLog::Print(x)
	#define LOG_ERROR(x)	DebugLog::PrintLastError(x,__FUNCTION__)
	#define LOG_EXCEPTION(x)	DebugLog::PrintException(x)

	#define DO_NOTHING		{(void)0;}

}