
#include "DataConsumerLogMultiSource.h"
#include "DebugLog.h"
#include "DataConsumerLog16.h"
#include "GenericException.h"
namespace OTComm {
	  
		DataConsumerLogMultiSource::DataConsumerLogMultiSource(String^ fileNameTemplate):
		mDataConsumers(nullptr), DataConsumerLog()
	{			
			FileTemplateName = fileNameTemplate;
	}

	DataConsumerLogMultiSource::~DataConsumerLogMultiSource()
	{
	}


	String^ DataConsumerLogMultiSource::GetConsumerXMLConfiguration()
	{
		String^ XmlResult = String::Empty;
		//TODO: call GetConsumerXMLConfiguration for each DataConsumer
		return XmlResult;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="gen"></param>
	/// <returns></returns>
	void DataConsumerLogMultiSource::Register(DataGeneratorInterface^ gen, __int16 source)
	{
		try
		{
			if(mDataConsumers == nullptr)
				mDataConsumers= gcnew List<DataConsumerLog^>;
			if (source<0)				
			{
				if (FileTemplateName->IndexOf("{0}") < 0)
				{
					throw gcnew InvalidFileTemplateName(__FUNCTION__);
				}
				unsigned __int16 sources = gen->GetNumberSources();
				for (unsigned __int16 k = 0; k < sources; k++)
				{	
					String^ tmpFileName = String::Format(FileTemplateName,k);
					DataConsumerLog^ dcl = gcnew DataConsumerLog16(tmpFileName);
					mDataConsumers->Add(dcl);
					gen->AddDataConsumer(dcl, k);
				}
			}
			else
			{
				String^ tmpFileName = String::Format(FileTemplateName, 0);
				DataConsumerLog^ dcl = gcnew DataConsumerLog16(tmpFileName); //assicurarsi che sia univoco
				mDataConsumers->Add(dcl);
				gen->AddDataConsumer(dcl, source);
			}
		}
		catch (Exception^)
		{
			throw gcnew RegistrationFailedException(__FUNCTION__);
		}
	}

	void DataConsumerLogMultiSource::Unregister(DataGeneratorInterface^ gen)
	{
		try
		{
			for (int k = 0; k < mDataConsumers->Count; k++)
			{
				mDataConsumers[k]->Unregister(gen);
			}
			gen->OnOpenDevice -= gcnew OTComm::OpenDevice(this, &OTComm::DataConsumerLogMultiSource::OnOpenDevice);
			gen->OnNewDataAvailable -= gcnew OTComm::NewDataAvailable(this, &OTComm::DataConsumerLogMultiSource::OnNewDataAvailable);
			gen->OnCloseDevice -= gcnew OTComm::CloseDevice(this, &OTComm::DataConsumerLogMultiSource::OnCloseDevice);
		}
		catch (Exception^)
		{
			throw gcnew RegistrationFailedException(__FUNCTION__);
		}
	}

	/// <summary>
	/// Return a synthetic description of the DataConsumer.
	/// </summary>
	/// <returns>A string similar to "Data logger saving data to " followed by the name of the currently set 
	/// file where the received data is stored</returns>
	String^ DataConsumerLogMultiSource::GetConsumerDescription()
	{
		return "Data logger saving data to " ;//TODO complete
	}
		
	void DataConsumerLogMultiSource::StartRecording()
	{
		if (mDataConsumers!=nullptr)
		for (int k = 0; k < mDataConsumers->Count; k++)
		{
			mDataConsumers[k]->StartRecording();
		}
	}

	void DataConsumerLogMultiSource::StopRecording()
	{
		if (mDataConsumers != nullptr)
		for (int k = 0; k < mDataConsumers->Count; k++)
		{
			mDataConsumers[k]->StopRecording();
		}
	}

	void DataConsumerLogMultiSource::OnNewDataAvailable(Object^ dri, EventArgs^ e)
	{
		//do nothing. Required for the derivation from abstract class.
	}


}