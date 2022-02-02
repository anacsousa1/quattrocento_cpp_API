#pragma once
using namespace System;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Reflection;
namespace OTComm
{
	///<summary>Class to help user to get info from OTComm</summary>
	
	public ref class Utilities
	{
	public:
		///<summary>Get image from embedded resources</summary>
		///<param name="ResourceName">name of resource</param>
		///<returns>image associated to ResourceName</returns>
		static Bitmap^ GetImageFromEmbeddedResource(String^ ResourceName);
		///<summary>Get image from embedded resources</summary>
		///<param name="ResourceName">name of resource</param>
		///<param name="LoadDefault">set to true if load default image when load ResourceNAme fails</param>
		///<returns>image associated to ResourceName</returns>
		static Bitmap^ GetImageFromEmbeddedResource(String^ ResourceName, bool LoadDefault);
		///<summary>Get default image from embedded resources</summary>
		///<returns>default image</returns>
		static Bitmap^ GetDefaultImage();
		///<summary>Write text to file</summary>
		///<param name="Text">text to write</param>
		///<param name="FileName">path to file to overwrite</param>
		///<returns>-1 if fails, 0 if everything is fine</returns>
		static 	bool WriteToFile(String^ Text, String^ FileName);
		///<summary>Get embedded resources</summary>
		///<returns>list of embedded resources</returns>
		static array<String^>^ GetEmbeddedResourceNames()
		{
			return Assembly::GetExecutingAssembly()->GetManifestResourceNames();
		}
	};
}

