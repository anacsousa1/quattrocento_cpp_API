#include "Utilities.h"
#include "DebugLog.h"
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Drawing;

namespace OTComm
{
#pragma managed

	Bitmap^ Utilities::GetDefaultImage()
	{
		return nullptr;
	}

	Bitmap^ Utilities::GetImageFromEmbeddedResource(String^ ResourceName)
	{
		
		return GetImageFromEmbeddedResource(ResourceName, false);
		
	}

	Bitmap^ Utilities::GetImageFromEmbeddedResource(String^ ResourceName, bool LoadDefault)
	{
		//LOG_ENTERING;
		Bitmap^ bitmap;
		try
		{
			String^ FileName = System::IO::Path::GetTempFileName();
			Assembly ^myAssembly = Assembly::GetExecutingAssembly();
			Stream ^myStream = myAssembly->GetManifestResourceStream(ResourceName);
			bitmap = gcnew Bitmap(myStream);
		}
		catch (Exception^)
		{
			if (LoadDefault)
				bitmap = GetImageFromEmbeddedResource("Default.png", false);//always use false here to avoid infinite recursion
			else
				bitmap = nullptr;
		}
		//LOG_EXITING;
		return bitmap;
	}
		
	bool Utilities::WriteToFile(String^ _Text, String^ _Filename)
	{
		try
		{
			DebugLog::Print(_Filename);
			StreamWriter^ sw = gcnew StreamWriter(_Filename);
			DebugLog::Print(_Text);
			sw->Write(_Text);
			sw->Close();
		}
		catch (Exception^)
		{
			//TODO completare
			return false;
		}
		return true;
		
	}

}