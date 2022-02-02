#pragma once
using namespace System;
namespace OTComm 
{
	//!  A class to easily convert between byte arrays and Strings
	/*!
	The class can be used when passing data to, e.g., sockets, serial ports, etc.
	*/
	///<summary>
	/// A class to easily convert between byte arrays and Strings
	///</summary>
	public ref class DataConverter
	{
	public:
		//! Static member to convert from Byte array to String^
		/*!
		\param input Byte array containing the data to convert
		\param length Number of bytes to convert. It can be sometimes convenient to just convert the first bytes of the array
		\return A String^ object containing the converted data
		\sa StringToByteArray(), ByteArrayToHexString()
		*/
		///<summary>Convert from ByteArray to String</summary>
		///<param name="input">ByteArray to convert</param>
		///<param name="length">how many bytes to convert</param>
		///<returns>Converted string</returns>
		static String^ ByteArrayToString(array<Byte>^ input, const unsigned int length);
		///<summary>Convert from String to ByteArray</summary>
		///<param name="input">String to convert</param>
		///<returns>Converted ByteArray </returns>
		static array<Byte>^ StringToByteArray(String^ input);
		///<summary>Convert from ByteArray to Hex String</summary>
		///<param name="input">ByteArray to convert</param>
		///<returns>Converted Hex String</returns>
		static String^ ByteArrayToHexString(array<Byte>^ input);
	};
}
