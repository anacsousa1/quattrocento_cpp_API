#include "DataConverter.h"

namespace OTComm {

	String^ DataConverter::ByteArrayToString(array<Byte>^ input, const unsigned int length)
	{
		String^ output;
		System::Text::ASCIIEncoding^ ascii = gcnew Text::ASCIIEncoding;
		output = ascii->GetString(input, 0, length);
		return output;
	}

	array<Byte>^ DataConverter::StringToByteArray(String^ input)
	{
		array<Byte>^ data = gcnew array<Byte>(input->Length);
		System::Runtime::InteropServices::Marshal::Copy(IntPtr(&input), data, 0, input->Length);
		return data;
	}

	System::String^ DataConverter::ByteArrayToHexString(array<Byte>^ input)
	{
		if (input != nullptr)
		{
			System::Runtime::Remoting::Metadata::W3cXsd2001::SoapHexBinary^ shb =
				gcnew System::Runtime::Remoting::Metadata::W3cXsd2001::SoapHexBinary(input);
			return shb->ToString();
		}
		else
			return "";
	}
}