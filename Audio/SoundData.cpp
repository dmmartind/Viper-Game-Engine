//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: Audio: SoundData Class Code

//**********************************************vk)****************************************


#ifndef _SOUNDDATA_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define _SOUNDDATA_CPP_//define one time and let everyone share, so VS doesn't give annoying errors


#include "Sound.h"
#include "..\ogg\vorbisfile.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           CWAVFile
//	Class:			CWAVFile
//  Description:    constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWAVFile::CWAVFile()
{
	Init();//initialize
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Init
//	Class:			CWAVFile
//  Description:    initialize everything
//  Arguments:      None.
//  Return Value:   Void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
 void CWAVFile::Init()   
 {   
   m_Data = NULL;   
   m_AudioFormat = 0;   
   m_BitsPerSample = 0;   
   m_BlockAlign = 0;   
   m_ByteRate = 0;   
   m_NumberofChannels =0;
   m_SampleRate = 0;   
   m_DataLen = 0; 
   index=0;
   
   
 }  

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ~CWAVFile
//	Class:			CWAVFile
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   NONE
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 CWAVFile::~CWAVFile()   
 {   
   if (NULL != m_Data)//if buffer exist
   {
	   delete[] m_Data;//delete buffer
	   m_Data = NULL;//set the buffer to null
	   m_DataLen = 0;//set the data length to null
   }   
    
 } 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getData
//	Class:			CWAVFile
//  Description:    get the data at the provided index from the data object
//  Arguments:      long index
//  Return Value:   BYTE
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
 BYTE CWAVFile::getData(long index)
{
	return m_Data[index];




}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getData
//	Class:			CWAVFile
//  Description:    get the data from the data object
//  Arguments:      None.
//  Return Value:   BYTE*
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

 BYTE *CWAVFile::getData()
 {

	return m_Data;


 }

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setData
//	Class:			CWAVFile
//  Description:    set the data to the data object
//  Arguments:      BYTE* input
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWAVFile::setData(BYTE* input)
	{
		
		m_Data=input;




	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getIndex
//	Class:			CWAVFile
//  Description:    get the index of the data in the data object
//  Arguments:      None.
//  Return Value:   LONG
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


	long CWAVFile::getIndex()
	{

		return index;


	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setIndex
//	Class:			CWAVFile
//  Description:    set current index of the data in the data object
//  Arguments:      long input
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void CWAVFile::setIndex(long input)
	{

		index=input;


	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getDataLen
//	Class:			CWAVFile
//  Description:    get data length
//  Arguments:      None.
//  Return Value:   unsigned int
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	unsigned int CWAVFile::getDataLen()
	{
		return m_DataLen;



	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setDataLen
//	Class:			CWAVFile
//  Description:    set data length
//  Arguments:      unsigned int input
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWAVFile::setDataLen(unsigned int input)
	{
		m_DataLen=input;

	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getAudioFormat
//	Class:			CWAVFile
//  Description:    get audio format
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	int CWAVFile::getAudioFormat()
	{

		return m_AudioFormat;


	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setAudioFormat
//	Class:			CWAVFile
//  Description:    set audio format
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWAVFile::setAudioFormat(int input)
	{

		m_AudioFormat=input;



	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getSampleRate
//	Class:			CWAVFile
//  Description:    get sample rate
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


	int CWAVFile::getSampleRate()
	{


		return m_SampleRate;




	}

	 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setSampleRate
//	Class:			CWAVFile
//  Description:    set sample rate
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void CWAVFile::setSampleRate(int input)
	{


		m_SampleRate=input;


	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getNumberofChannels
//	Class:			CWAVFile
//  Description:    get the number of channels
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int CWAVFile::getNumberofChannels()
	{


		return m_NumberofChannels;


	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setNumberofChannels
//	Class:			CWAVFile
//  Description:    set the number of channels
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void CWAVFile::setNumberofChannels(int input)
	{

		m_NumberofChannels=input;




	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getBitsPerSample
//	Class:			CWAVFile
//  Description:    get bits per sample for an audio file
//  Arguments:      None.
//  Return Value:   SHORT
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	short CWAVFile::getBitsPerSample()
	{

		return m_BitsPerSample;



	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setBitsPerSample
//	Class:			CWAVFile
//  Description:    set bits per sample for an audio file
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWAVFile::setBitsPerSample(int input)
	{



		m_BitsPerSample=input;

	}


 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getByteRate
//	Class:			CWAVFile
//  Description:    get the byte rate of sound file
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int CWAVFile::getByteRate()
	{


		return m_ByteRate;


	}

	 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setByteRate
//	Class:			CWAVFile
//  Description:    set the byte rate of sound file
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CWAVFile::setByteRate(int input)
	{


		m_ByteRate=input;


	}

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getBlockAlign
//	Class:			CWAVFile
//  Description:    get the number blocks that a buffer can be divided into when streamed
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int CWAVFile::getBlockAlign()
	{


		return m_BlockAlign;

	}

	 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setBlockAlign
//	Class:			CWAVFile
//  Description:    set the number blocks that a buffer can be divided into when streamed
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void CWAVFile::setBlockAlign(int input)

	{

		m_BlockAlign=input;



	}


//////////////////////////////////////////////////////////////////////
//
// SoundData Functions
//
//////////////////////////////////////////////////////////////////////

 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SoundData
//	Class:			SoundData
//  Description:    constructor
//  Arguments:      None.
//  Return Value:   NONE
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
SoundData::SoundData()
{
  m_Frequency     = 22050;//set the frequency
  m_Channels      = 1;//set the channels
  m_BitsPerSample = 16;//set the bits resolution

  m_fp       = NULL;//set the file pointer to null
  m_Ptr      = NULL;//set the pointer to null
  m_Buf		 = NULL;//set the buffer to null
  m_StartPos = m_Pos = 0;//set the start marker position to 0
  m_Size     = m_Left = 0;//set the size to 0
  is_ogg=false;//set the ogg flagg to false
  m_Pcm      = NULL;
  g_SoundBufferSize=0; 
g_SoundBufferChunk=0;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ~SoundData
//	Class:			SoundData
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   NONE
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

SoundData::~SoundData()
{
  Free();//free everything
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Create
//	Class:			SoundData
//  Description:    creates a new buffer with the size that is already set in the class
//  Arguments:      None.
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//BOOL SoundData::Create()
//{
//  return Create(m_Size);
//}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Create
//	Class:			SoundData
//  Description:    creates a new data buffer using the size set by the user
//  Arguments:      None.
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//BOOL SoundData::Create(long Size)
//{
  // Free prior allocated data
//  Free();//free all pointers and buffers

  // Check for valid size
//  if((m_Size = Size) == 0)//set current size to 0
//    return FALSE;//otherwize return failed

  // Create a new buffer
//  m_Buf = new char[m_Size];//set new buffer with the new size
//  if(m_Buf == NULL)//if buffer creation failed,
 //   return FALSE;//return failed

  // Point to new buffer
//  m_Ptr = m_Buf;//set the pointer to the new buffer
//  m_fp  = NULL;//set the file pointer to null, since we are using buffered data instead of file data

  // Clear out new buffer
//  ZeroMemory(m_Buf, m_Size);//zero out the current buffer.

//  return TRUE;//return true
//}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Free
//	Class:			SoundData
//  Description:    frees all buffers and pointers
//  Arguments:      None.
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundData::Free()
{
  if(m_Buf != NULL) {//if buffer exits,
    delete[] m_Buf;//delete it
    m_Buf = NULL;//and set the pointer to null
  }

  if(m_Ptr != NULL) {//if buffer exits,
    delete[] m_Ptr;//delete it
    m_Ptr = NULL;//and set the pointer to null
  }

  if(m_fp != NULL)
  {
	  fclose(m_fp);
		m_fp=NULL;
  }

  if(m_Pcm != NULL) {//if buffer exits,
    delete[] m_Pcm;//delete it
    m_Pcm = NULL;//and set the pointer to null
  }
  


  m_Size = 0;//set the buffer size to 0
  m_Frequency     = 0;//set the frequency
  m_Channels      = 0;//set the channels
  m_BitsPerSample = 0;//set the bits resolution
  m_StartPos = m_Pos = 0;//set the start marker position to 0
  m_Size     = m_Left = 0;//set the size to 0
  is_ogg=false;//set the ogg flagg to false
  g_SoundBufferSize=0; 
	g_SoundBufferChunk=0;
  


  
  return TRUE;//return true for success
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetPtr
//	Class:			SoundData
//  Description:    return buffer
//  Arguments:      None.
//  Return Value:   char*
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *SoundData::GetPtr()
{
  return m_Buf;
}








/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SetFormat
//	Class:			SoundData
//  Description:    set new format for secondary sound buffer
//  Arguments:      long Frequency, short Channels, short BitsPerSample
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundData::SetFormat(long Frequency, short Channels, short BitsPerSample)
{
  m_Frequency = Frequency;//set the new frequency
  m_Channels = Channels;//set the channels
  m_BitsPerSample = BitsPerSample;//set the bit resolution

  return TRUE;//return true
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SetSource
//	Class:			SoundData
//  Description:    set the file pointer to the sound channel
//  Arguments:      FILE *fp, long Pos, long Size
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundData::SetSource(FILE *fp, long Pos, long Size)
{
  m_fp = fp;//set the new file pointer
  m_Ptr = NULL;//the data file pointer to null

  //if(Pos != -1)//if the position was already set
  //  m_StartPos = m_Pos = Pos;//set the start position to it
  //if(Size != -1)//if the size was already set
  //  m_Size = m_Left = Size;//set the size attribute to the size

  return TRUE;//return success
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SetSource_WAV_OGG
//	Class:			SoundData
//  Description:    sets the object pointer to the pcm data object
//  Arguments:      CWAVFile *ptr
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundData::SetSource_WAV_OGG(CWAVFile *ptr)
{
m_fp = NULL;//set the file ptr to null, since we are not using it
  m_Pcm = ptr;//set the ptr to the buffer

  long Pos=0;//set the marker position to 0
  long Size=0;//set the size to 0
	
  Pos=ptr->getIndex();//set the position to the current position of the marker of the pcm buffer
  
  Size= ptr->getDataLen();//set the size to the size of the pcm buffer

  if(Pos != -1)//if the position exist
	  m_StartPos = m_Pos = Pos;//then the se the position attributes to it.
  else
	  return false;//return unsuccessful
  if(Size != -1)//if the size exist 
	  m_Size = m_Left = Size;//set the the size attributes to the current size of the pcm buffer
  else
	  return false;//return unsuccessful
  return TRUE;//return successful

 }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Load_OGG_Wav_Header
//	Class:			SoundData
//  Description:    set the attributes from the RIFF data of the OGG file
//  Arguments:      CWAVFile *data
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL SoundData::Load_OGG_Wav_Header(CWAVFile *data)
{
	BOOL ReturnVal;//init return value flag
	

	if(data == NULL)//if pcm buffer doesn't exist
		return false;//return failed

	ReturnVal=false;//set the return value

	if(data->getAudioFormat() == WAVE_FORMAT_PCM)//make sure that the audio format of from the RIFF header is in WAV-PCM format
	{
		m_Frequency = data->getSampleRate();//set the frequency attributes
		m_Channels = data->getNumberofChannels();//set the channel attributes
		m_BitsPerSample = data->getBitsPerSample();//set the bit resolution attributes

		m_Size = m_Left = data->getDataLen();//set the buffer size attributes
		m_StartPos = m_Pos = data->getIndex();//set the position attributes
		is_ogg=true;//set the ogg flag to true
		g_SoundBufferSize=data->getDataLen();
		g_SoundBufferChunk=data->getDataLen()/4;
		

    ReturnVal = true;//set the return value to true
	}
return ReturnVal;//return success or fail
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           LoadWAVHeader
//	Class:			SoundData
//  Description:    set the attributes from the RIFF data of the wave file
//  Arguments:      char *Filename, FILE *fp
//  Return Value:   FILE*
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
FILE* SoundData::LoadWAVHeader(char *Filename, FILE *fp)
{
	errno_t err;//set error return var

  sWaveHeader Hdr;//init RIFF Header structure
  //BOOL ReturnVal;//init the return flag var
  long Pos;//init the position var

  if(Filename == NULL && fp == NULL)//check to see if both he filename and the file pointer exist
    return 0;//if not, fail

  if(Filename != NULL) {//if the filename exist
    if((err=fopen_s(&fp,Filename, "rb")) != 0)//open wave file
		return 0;//return failed if can't
	}

  // Save position in file
  Pos = ftell(fp);

   // Read in header and parse
  
  fread(&Hdr, 1, sizeof(sWaveHeader), fp);//read the RIFF Header
  if(!memcmp(Hdr.RiffSig, "RIFF", 4) && !memcmp(Hdr.WaveSig, "WAVE", 4) && !memcmp(Hdr.FormatSig, "fmt ", 4) && !memcmp(Hdr.DataSig, "data", 4)) {
    //check to make sure all the RIFF elements are correct
	m_Frequency     = Hdr.SampleRate;//set the frequency attributes
    m_Channels      = Hdr.Channels;//set the channel attributes
    m_BitsPerSample = Hdr.BitsPerSample;//set the bit resolution attributes

    m_Size = m_Left = Hdr.DataSize;//set the size attribute
    m_StartPos = m_Pos = ftell(fp);//set the current position attributes
	g_SoundBufferSize=Hdr.WaveformChunkSize;
	g_SoundBufferChunk=Hdr.WaveformChunkSize/4;
}

  // Close if we opened file
  // otherwise return to original position
  if(Filename == NULL)
    fclose(fp);
  else 
    fseek(fp, Pos, SEEK_SET);

  return fp;//return back the file ptr
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Copy
//	Class:			SoundData
//  Description:    copies attributes of one class object to another
//  Arguments:      SoundData *Source
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL SoundData::Copy(SoundData *Source)
{
  if(Source == NULL)//if sounddata object doesn't exist
    return FALSE;//fail
  
  m_Frequency     = Source->m_Frequency;//copy the frequency attribute
  m_Channels      = Source->m_Channels;//copy the channels attribute
  m_BitsPerSample = Source->m_BitsPerSample;//copy the bit resolution

  m_fp       = Source->m_fp;//copy the file pointer
  m_Ptr      = Source->m_Ptr;//copy ptr
  m_Size     = Source->m_Size;//copy size attribute
  m_Left     = Source->m_Left;//copy current position attribute
  m_Pos      = Source->m_Pos;//copy current position
  m_StartPos = Source->m_StartPos;//copy starting position
  g_SoundBufferSize = Source->g_SoundBufferSize; 
  g_SoundBufferChunk = Source->g_SoundBufferChunk;
  

  return TRUE;//return success


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           LoadOGG
//	Class:			SoundData
//  Description:    load ogg file pointer to the sound vector
//  Arguments:      char *Filename
//  Return Value:   CWAVFile*
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWAVFile *SoundData::LoadOGG(char *Filename)
{
		int pcm_size=0;//size of pcm block buffer
		int counter=0;//init buffer index counter
		
		BYTE *pcmdata;//pcm data buffer
		char pc_mount[4096]; //pcm block buffer
		OggVorbis_File vf;//ogg decoder object
		int eof=0; //init end of file flag
		int current_section;//init current position
		FILE *fp;//file ptr
		fopen_s(&fp,Filename, "rb");//open ogg file
		if(fp == NULL)//check for open file
			return NULL;//return fail
		if((ov_open(fp,&vf,NULL,0)) < 0)//open file and set data stream
		{
			return NULL;//return failed
		}
		vorbis_info *vi=ov_info(&vf,-1);//get header info
		int samples=(int)ov_pcm_total(&vf,-1);//get the amount of samples
		int channels=vi->channels;//get the channel attribute
		int samplerate=vi->rate;//get the frequency
		

		
		
		
		if(samples > 0)//if samples exist
		{
			//pcmdata.reserve(channels*2*samples); //resize the buffer to the size of the ogg pcm data
			pcm_size=channels*2*samples;//set the pc size
			pcmdata=(BYTE*)malloc(pcm_size);//create pcm buffer with the new size
		}
		while(!eof)//while not end of the ogg file
		{
			long ret=ov_read(&vf,pc_mount,sizeof(pc_mount),0,2,1,&current_section);
			//grab data block and hold the position of the marker
			if(ret==0)//if eof
				eof=1;//set the eof flag to 0
			else if(ret <0)//if failed
			{
				return NULL;//error in stream
			}
			else
			{
				for(int q=0; q< ret; q++)//if each index
				{
					//pcmdata.push_back(pc_mount[q]);//move data from ogg buffer to wav buffer
					*(pcmdata+counter)=*(pc_mount+q);//copy the pcm block to the pcm buffer
					counter++;//increament counter
					
				}
			}
		}

		ov_clear(&vf);//clear the ogg object

		CWAVFile *wavefile;//create wave object
		wavefile= new CWAVFile;//init wave object
		
		
		wavefile->setAudioFormat(WAVE_FORMAT_PCM);//set the format attribute
		wavefile->setBitsPerSample(16);//set the bit resolution attribute
		wavefile->setBlockAlign(2*channels);//set the block align attribute
		wavefile->setByteRate(samplerate*channels*2);//set the byte rate attributes
		wavefile->setDataLen(channels*2*samples);//size of pcm buffer attributes
		wavefile->setNumberofChannels(channels);//set the channel attributes
		wavefile->setSampleRate(samplerate);//sample rate attribute
		wavefile->setData(pcmdata); //set the pointer to the buffer
		
		

		//delete pcmdata;
		pcmdata=NULL;
		fclose(fp);
		fp=NULL;
		ZeroMemory(&vi, sizeof(vi));
		vi=NULL;
		ZeroMemory(&pc_mount, sizeof(char));
					
		return wavefile;//return wave object
	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getIsOGG
//	Class:			SoundData
//  Description:    return OGG flag
//  Arguments:      None.
//  Return Value:   bool
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SoundData::getIsOGG()
{
	return is_ogg;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getPCM
//	Class:			SoundData
//  Description:    return PCM buffer
//  Arguments:      None.
//  Return Value:   CWAVFile*
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWAVFile *SoundData::getPCM()
	{
	return m_Pcm;
	
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getSize
//	Class:			SoundData
//  Description:    return size of buffer
//  Arguments:      None.
//  Return Value:   long
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

long SoundData::getSize()
	{
		return m_Size;
	}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ReleaseCOM
//	Class:			SoundData
//  Description:    release COM object
//  Arguments:      class T
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
	void SoundData::ReleaseCOM(T& x)
	{
		if(x)
		{
			x->Release();
			ZeroMemory(&x,sizeof(x));
			x = NULL;
		}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getPCMBUFFER
//	Class:			SoundData
//  Description:    return PCM buffer
//  Arguments:      None.
//  Return Value:   CWAVFile*
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CWAVFile *SoundData::getPCMBUFFER()
	{

		return m_Pcm;
		
	}





#endif
