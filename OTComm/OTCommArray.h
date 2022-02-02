/*#pragma once
using namespace System;
using namespace System::Collections::Generic;
namespace OTComm
{
	generic <class T> public ref class OTCommArray
		{
		private :
			
		public:
			List<T> ^ mObjects;
			OTCommArray()
			{
				mObjects = gcnew List<T>^;
			}

			OTCommArray()
			{
				mObjects = gcnew List<T>^;
			}
			
			virtual property int Count
			{
				int get() { return mObjects !=nullptr? mObjects->Count:0; }
			}

			void Set(const int nIndexStart, T c)
			{
				Set(nIndexStart, nIndexStart, c);
			}

			void Set(const int nIndexStart, const int nIndexEnd, T c)
			{
				for (int k = nIndexStart; k < nIndexEnd; k++)
				{
					mObjects[k] = c;
				}
			}
		};

}*/