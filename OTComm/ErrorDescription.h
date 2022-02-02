#pragma once

using namespace System;

namespace OTComm {
	///<summary>Description of Error</summary>
	public ref class ErrorDescription : public System::EventArgs
	{
	protected:
		Exception^ e;
	public:
		ErrorDescription(Exception^ e);
		///<summary>Gets message of error</summary>
		///<returns>Return message of error</returns>
		String^ GetMessage();
		///<summary>Gets function of error</summary>
		///<returns>Return function of error</returns>
		String^ GetFunction();
		///<summary>Gets code of error</summary>
		///<returns>Return code of error</returns>
		int GetCode();
		///<summary>Gets message of error</summary>
		///<returns>Return message of error and code</returns>
		virtual String ^ ToString() override;
	};
}