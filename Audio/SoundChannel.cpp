//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: Audio SoundChannel Code

//**********************************************vk)****************************************


#ifndef _SOUNDCHANNEL_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define _SOUNDCHANNEL_CPP_//define one time and let everyone share, so VS doesn't give annoying errors


#include "Sound.h"

//////////////////////////////////////////////////////////////////////
//
// SoundChannel Functions
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SoundChannel
//	Class:			SoundChannel
//  Description:    constructor
//  Arguments:      NONE
//  Return Value:   NONE
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
SoundChannel::SoundChannel()
{
  m_Sound     = NULL;//sound object
  m_pDSBuffer = NULL;//sound buffer
  m_pDSNotify = NULL;//sound notification
  m_Event     = -1;//Event iterater
  m_Volume    = 0;//set the volume attribute to 0
  m_Pan       = 0;//set the pan attribute to 0
  m_Frequency = 0;//set the frequency to 0
  m_Playing   = FALSE;//set the playing flag to false
  m_Pause		=FALSE;//set the pause flag to false
  m_flag      =0;//set the flag to 0
  is_ogg      =false;//set the ogg flag
  m_Loop	= false;//set the loop flag to 0
  m_BitsPerSample =0;//set the sample bit rate to 0
  m_Channels=  0;//set the channels to 0
  m_LoadSection =  0;//set the load position var to 0
  m_StopSection   =0;//set the stop position var to 0
  m_NextNotify    =0;//set the next block var to 0
  dwCurrentPlayPos =0;//set the current play position to 0
  firstPlay=false;//set the first play flag to false
  startPlayLoop=false;//set the start to play loop flag to false

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ~SoundChannel
//	Class:			SoundChannel
//  Description:    deconstructor
//  Arguments:      NONE
//  Return Value:   NONE
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
SoundChannel::~SoundChannel()
{
  Free();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetSoundBufferCOM
//	Class:			SoundChannel
//  Description:    return sound buffer
//  Arguments:      NONE
//  Return Value:   IDirectSoundBuffer8
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

IDirectSoundBuffer8 *SoundChannel::GetSoundBufferCOM()
{
  return m_pDSBuffer;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetNotifyCOM
//	Class:			SoundChannel
//  Description:    return notify object
//  Arguments:      NONE
//  Return Value:   IDirectSoundNotify8
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

IDirectSoundNotify8 *SoundChannel::GetNotifyCOM()
{
  return m_pDSNotify;
}  



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Create
//	Class:			SoundChannel
//  Description:    create sound channel
//  Arguments:      sound object, frequency, channels, bit resolution, ogg flag, size of buffer
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundChannel::Create(Sound *Sound, long Frequency, short Channels, short BitsPerSample, bool is_ogg, long Size,long wave_chunk, long wave_size)
{
  DSBUFFERDESC dsbd;//buffer description
  WAVEFORMATEX wfex;//wave format structure
  HANDLE       hEvent;//event index
  DSBPOSITIONNOTIFY   dspn[4];//notify structure
  IDirectSoundBuffer *pDSBuffer;//sound buffer

  // Free a prior channel
  Free();

  if((m_Sound = Sound) == NULL)//if the sound object doesn't exist
  {
		return FALSE;//failed
  }
	if(m_Sound->GetDirectSoundCOM() == NULL)//get sound COM
	{
		return FALSE;//failed
	}
  // Save playback format
  m_Frequency     = Frequency;//set the frequency attribute
  m_BitsPerSample = BitsPerSample;//set the bit resolution attribute
  m_Channels      = Channels;//set the channel attribute

  // Create a new sound buffer for this channel
  // Using specified format
  ZeroMemory(&wfex, sizeof(WAVEFORMATEX)); //zero the structure
  wfex.wFormatTag      = WAVE_FORMAT_PCM; //set the wave format
  wfex.nChannels       = (WORD)m_Channels;//set channels
  wfex.nSamplesPerSec  = m_Frequency;//set the frequency
  wfex.wBitsPerSample  = (WORD)m_BitsPerSample;//set the bit resolution
  wfex.nBlockAlign     = wfex.wBitsPerSample / 8 * wfex.nChannels;//set the data block
  wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;//set the avereage bytes/sec

  //| DSBCAPS_GLOBALFOCUS
  ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));//zero out memory
  dsbd.dwSize        = sizeof(DSBUFFERDESC);//set the default size
  if(is_ogg)//is ogg
  {
	dsbd.dwFlags       = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE | DSBCAPS_GETCURRENTPOSITION2 ;
	//set the buffer with volume control, set the panning, set the frequency control,software buffer lock, and position marker
	dsbd.dwBufferBytes=Size;//buffer size
  }
  else
  {
	  dsbd.dwFlags     = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_LOCSOFTWARE | DSBCAPS_GETCURRENTPOSITION2 ;
	  //set the buffer with volume control, set the panning, set the frequency control,software buffer lock, position marker, and notification
	
	  dsbd.dwBufferBytes = wave_size;//set buffer size to default

  }
	
  dsbd.lpwfxFormat   = &wfex;//format structure
  if(FAILED(m_Sound->GetDirectSoundCOM()->CreateSoundBuffer(&dsbd, &pDSBuffer, NULL)))//create sound buffer
  {
	  //clean up data structures
	  ZeroMemory(&wfex, sizeof(wfex));
	  ZeroMemory(&dsbd, sizeof(dsbd));
		//release buffer
	  pDSBuffer->Release();
	  pDSBuffer=NULL;
	  return FALSE;//return failed
  }
  // query for newer interface
  if(FAILED(pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pDSBuffer)))
  {
	  //clean up data structures
	ZeroMemory(&wfex, sizeof(wfex));
	  ZeroMemory(&dsbd, sizeof(dsbd));
		//clean up sound buffer
	  pDSBuffer->Release();
	  pDSBuffer=NULL;
	return FALSE;//failed
  }

  // Release old object - we have the newer one now
  pDSBuffer->Release();

  if(!is_ogg)
  {
  // Create the notification interface
  if(FAILED(m_pDSBuffer->QueryInterface(IID_IDirectSoundNotify8, (void**)&m_pDSNotify)))
  {
	  //clean up data structures
	ZeroMemory(&wfex, sizeof(wfex));
	  ZeroMemory(&dsbd, sizeof(dsbd));
		//clean up sound buffer
	  pDSBuffer->Release();
	  pDSBuffer=NULL;
	return FALSE;
  }
  // Get an event for this
  if(m_Sound->AssignEvent(this, &m_Event, &hEvent) == FALSE)
  {
	  //clean up data structures
	 ZeroMemory(&wfex, sizeof(wfex));
	  ZeroMemory(&dsbd, sizeof(dsbd));
		hEvent=NULL;//set current event to null
	  //release buffer
		pDSBuffer->Release();
	  pDSBuffer=NULL;
		return FALSE;
  }
  // Setup the 4 notification positions for each sound block
  dspn[0].dwOffset = wave_chunk - 1;//the buffer size of the first block
  dspn[0].hEventNotify = hEvent;//set an event to it
  dspn[1].dwOffset = wave_chunk * 2 - 1;//take the second buffer size of the second block
  dspn[1].hEventNotify = hEvent;//set an event to it
  dspn[2].dwOffset = wave_chunk * 3 - 1;//take the third buffer size of the third block
  dspn[2].hEventNotify = hEvent;//set an event to it
  dspn[3].dwOffset = wave_size - 1;//take a fourth buffer size of the fourth block
  dspn[3].hEventNotify = hEvent;//set an event to it.
  if(FAILED(m_pDSNotify->SetNotificationPositions(4, dspn)))//set notification positions and events
  {
	  //clean up data structure 
		ZeroMemory(&wfex, sizeof(wfex));
	  ZeroMemory(&dsbd, sizeof(dsbd));

		hEvent=NULL;//set the current event to null
	//clear the sound buffer  
	pDSBuffer->Release();
	  pDSBuffer=NULL;

	  for(int x=0; x<4; x++)//for each block
	  {
		  dspn[x].dwOffset=NULL;//clear the buffer offset
		  dspn[x].hEventNotify=NULL;//clear the notify pointer
	  }
		ZeroMemory(&dspn, sizeof(dspn));//zero memory
	  return FALSE;//return fail
  }
  }
  // set the pan and default volume
  SetVolume(100);//set volume to 100 percent
  SetPan(0);//set pan to center

  //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@garbage collection
  //ZeroMemory(&wfex, sizeof(wfex));
	//ZeroMemory(&dsbd, sizeof(dsbd));
		hEvent=NULL;//set event pointer to null
//	  pDSBuffer->Release();
//	  pDSBuffer=NULL;

	  //for(int x=0; x<4; x++)
	  //{
	//	  dspn[x].dwOffset=NULL;
	//	  dspn[x].hEventNotify=NULL;
	//  }
	//	ZeroMemory(&dspn, sizeof(dspn));
	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

  return TRUE;//return success
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Create
//	Class:			SoundChannel
//  Description:    create secondary buffer with minimum arguments
//  Arguments:      Sound object, SoundData object
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundChannel::Create(Sound *Sound, SoundData *SoundDesc)
{
	return Create(Sound, SoundDesc->m_Frequency, SoundDesc->m_Channels, SoundDesc->m_BitsPerSample, SoundDesc->is_ogg, SoundDesc->m_Size, SoundDesc->g_SoundBufferChunk, SoundDesc->g_SoundBufferSize);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Free
//	Class:			SoundChannel
//  Description:    garbage collector
//  Arguments:      NONE
//  Return Value:   NONE
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundChannel::Free()
{
  // Stop any playback
  Stop();

  // Release the notification
  ReleaseCOM(m_pDSNotify);

  // Release the buffer
  ReleaseCOM(m_pDSBuffer);

  // Release event from parent Sound class
  m_Sound->ReleaseEvent(this, &m_Event);

  m_Event     = -1;//Event iterater
  m_Volume    = 0;//set the volume attribute to 0
  m_Pan       = 0;//set the pan attribute to 0
  m_Frequency = 0;//set the frequency to 0
  m_Playing   = FALSE;//set the playing flag to false
  m_Pause		=FALSE;//set the pause flag to false
  m_flag      =0;//set the flag to 0
  is_ogg      =false;//set the ogg flag
  m_Loop	= false;//set the loop flag to 0
  m_BitsPerSample =0;//set the sample bit rate to 0
  m_Channels=  0;//set the audio channels to 0
  m_LoadSection =  0;//set the load block position  to 0
  m_StopSection   =0;//set the stop block position to 0
  m_NextNotify    =0;//set the next block var to 0
  dwCurrentPlayPos =0;//set the current play position to 0
  startPlayLoop=false;//set the start play loop flag to false


  return TRUE;//return success
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Read
//	Class:			SoundChannel
//  Description:    Copy input buffer to output buffer with a specific size
//  Arguments:      BYTE output,BYTE input,DWORD old size,DWORD new size
//  Return Value:   int
// ----------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////-----------------------------------------------------------------------------
int SoundChannel::Read(BYTE* pBuffer,BYTE* input, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
    
        if( input == NULL )//if input buffer is not valid
            return 0;//return failed
        if( pdwSizeRead != NULL )//if current size is > 0
            *pdwSizeRead = 0;//set it to 0

        CopyMemory( pBuffer, input, dwSizeToRead );//copy input to the pbuffer with the current size from dwSizeToRead
        
        if( pdwSizeRead != NULL )//if new size is valid
            *pdwSizeRead = dwSizeToRead;//set it to the new size
		return 1;//return success

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           LoadSoundData
//	Class:			SoundChannel
//  Description:    copy pcm data into secondary buffer
//  Arguments:      LockPos, fp, Size
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL SoundChannel::LoadSoundData(long LockPos, BYTE *fp, long Size)
{
  BYTE *Ptr1, *Ptr2;//init pointer vars
  DWORD Size1, Size2,newSize,newSize2;//init buffer size vars
  
  

  if(!Size)//if the buffer size doesn't exist
    return FALSE;//fail

// lock the sound buffer at position specified
//lock the secondary buffer for input
  if(FAILED(m_pDSBuffer->Lock(LockPos, Size,(void**)&Ptr1, &Size1,(void**)&Ptr2, &Size2, DSBLOCK_ENTIREBUFFER)))
    return FALSE;

	


  // read in the data
  //Ptr1=buffer_read(Ptr1, 1, Size1, fp);//add the pcm data into the buffer.
  //memcpy(&Ptr1,fp,Size1+10);
  Read(Ptr1,fp,Size1,&newSize);////////////////////////
  if(Ptr2 != NULL)//if the second ptr has an address
    //buffer_read(Ptr2, 1, Size2, fp);//read the pcm data into the buffer
  Read(Ptr2,fp,Size2,&newSize2);
  // unlock it
  m_pDSBuffer->Unlock(Ptr1, Size1, Ptr2, Size2);//unlock the buffer

  // return a success
  return TRUE;//return true 
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Play
//	Class:			SoundChannel
//  Description:    play function that's a thread
//  Arguments:      lpvoid
//  Return Value:   DWORD
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD WINAPI SoundChannel::Play(LPVOID lpvoid)
{//--1
	bool loop=false;//set loop = 0
	int volume=0;//set the volume to 0
	SoundData *data;//sound data object

	SoundChannel *sc;//sound channel object
	
	
	cData *test=new cData;//create a new cData object 
	test=(cData*)lpvoid;//get the data from lpvoid

	
	volume=test->getVolume();//set the volume
	data=test->getSoundData();//copy the sound data object
	sc=test->getSoundChannel();//copy the sound channel object
	sc->setLoop(test->getLoop());//set the loop flag attribute

  if(data == NULL)//if sound data object doesn't exist
    return 0;//return fail
  if(sc->m_pDSBuffer == NULL)//if sound channel object doesn't exist
    return 0;//return fail
if(!data->is_ogg)//if the sound data object doesn't have OGG pcm data
{//--2
  if(sc->m_pDSNotify == NULL)//check to make sure that the directx notification exist
    return 0;//if not fail
}//--2

  // Stop any playback
  sc->Stop();//stop all playback, if there is any on this channel

  // Restore a lost buffer just in case
  //sc->m_pDSBuffer->Restore();

  // Setup playing information
  sc->m_Desc.Copy(data);
  
  // Set looping data
  //if(loop==0)//if looping is turned off
	//  sc->setLoop(true);//set loop to 1
  //else
//	  sc->setLoop(false);//othersize set it to 0

  sc->is_ogg=data->is_ogg;//so the ogg flag to the data channel 
  if(!data->is_ogg)//if data channel is not ogg
  {//--2
  // Calculate stop section position
  // Calculate stop section position
	  if(!sc->isLoop())//if no looping
		{//--3
			sc->m_StopSection = -1;//set the end section to end at the end of the pcm data
		}//--3
	else
	{//--3
		 sc->m_StopSection = (short)(((sc->m_Desc.m_Size * sc->isLoop()) % sc->m_Desc.g_SoundBufferSize) / sc->m_Desc.g_SoundBufferChunk);
	//calculate the looping by taking the size of the data buffer * by the loop flag mod the (soundbuffersize/soundbufferchunck)
	}//--3


  
  
  
  // Buffer in data
  sc->m_LoadSection = 0;//start at index 0 of the secondary sound buffer
  
  
  for(int x=0;x<4;x++)
  {//--3
	  sc->BufferData();//read buffer for each of the four events.
  }//--3
  // Set the volume
  sc->SetVolume(volume);//set volume of playback

  // Set position and begin play
  sc->m_NextNotify = 0;//set the notify event stage to 0
	
}
  sc->setAlive(true);//set the channel alive
  sc->startPlayLoop=true;//set start play loop flag

  while(sc->getAlive() == true)//while channel is alive
  {
	
  
	while(((sc->IsPlaying()== true) || (sc->startPlayLoop == true)) )//whicle channel is playing and has started the play loop
	{
	  sc->startPlayLoop=false;//set the start loop flag to false, since it's in the loop

	  if(sc->isLoop()==false)//if the loop flag is false
		  sc->nonLoopPlay();//call for non looped play

	  if(sc->isLoop()==true){//if the loop flag is true
		sc->startPlayLoop=true;//set the start play loop to true
		
		if((sc->IsPlaying()== false))//if play flag is false
		{
			sc->setFirstPlay(false);//set the first play to false
		}
		  sc->LoopPlay();//call the loop play to cover the other functions besides looping
	  }Sleep(500);//sleep to wait for update

		
		


		 
	}//--2

  

 Sleep(500);//sleep to wait for update
  }

 
 ///////////////////set flags to false//////////////////////////////////////
 sc->setLoop(false);
 sc->startPlayLoop=false;
 sc->setFirstPlay(false);
 
  ExitThread(0);//kill thread
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Stop
//	Class:			SoundChannel
//  Description:    stop playback
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundChannel::Stop()
{
  if(m_pDSBuffer)//check to see if the secondary buffer exist
    m_pDSBuffer->Stop();//stop it 

  m_Playing = FALSE;//set the playback flag to false
  

  return TRUE;//return true
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetVolume
//	Class:			SoundChannel
//  Description:    get the volume
//  Arguments:      NONE
//  Return Value:   LONG
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

long SoundChannel::GetVolume()
{
  return m_Volume;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setvolume
//	Class:			SoundChannel
//  Description:    set volume
//  Arguments:      LONG
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundChannel::SetVolume(long Percent)
{
  long Volume;//init volume var

  if(m_pDSBuffer == NULL)//check to see if secondary buffer exist
    return FALSE;//return failed

  // calculate a usable volume level
  if(!Percent)//if percent == 0
    Volume = DSBVOLUME_MIN;//set the minimum volume level
  else 
    Volume = -20 * (100 - (Percent % 101));//calculate the new volume level
  if(FAILED(m_pDSBuffer->SetVolume(Volume)))//set the volume level
    return FALSE;//return failed

  m_Volume = Percent % 101;//set the volume level attribute for the sound class

  return TRUE;//return successful
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetPan
//	Class:			SoundChannel
//  Description:    get the current pan setting
//  Arguments:      NONE
//  Return Value:   signed long
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

signed long SoundChannel::GetPan()//
{
  return m_Pan;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SetPan
//	Class:			SoundChannel
//  Description:    Set the current Pan setting
//  Arguments:      signed long
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundChannel::SetPan(signed long Level)
{
  signed long Pan;//init var

  if(m_pDSBuffer == NULL)//if secondary buffer exist
    return FALSE;//return failed

  // calculate a usable setting
  if(Level < 0) //if the level is less that 0
    Pan = DSBPAN_LEFT / 100 * ((-Level) % 101);//calculate the pan level for left panning
  else
    Pan = DSBPAN_RIGHT / 100 * (Level % 101);//calculate the pan level for right panning

  if(FAILED(m_pDSBuffer->SetPan(Pan)))//set pan
    return FALSE;//return failed

  m_Pan = Level % 101;//set the attribute panning level

  return TRUE;  //return true
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetFrequency
//	Class:			SoundChannel
//  Description:    returns the playback frequency
//  Arguments:      NONE
//  Return Value:   LONG
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

long SoundChannel::GetFrequency()
{
  return m_Frequency;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SetFrequency
//	Class:			SoundChannel
//  Description:    set the frequency of the playback
//  Arguments:      long
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundChannel::SetFrequency(long Level)
{
  if(m_pDSBuffer == NULL)//if secondary buffer exist
    return FALSE;

  if(FAILED(m_pDSBuffer->SetFrequency(Level)))//set frequency with given level
    return FALSE;//return failed

  m_Frequency = Level;//set the class attribute

  return TRUE;//return successful
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           IsPlaying
//	Class:			SoundChannel
//  Description:    check to see if channel is playing
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
bool SoundChannel::IsPlaying()
{
	
	  if(this->GetSoundBufferCOM() == NULL)
		  return false;
	  
		  if(FAILED(this->GetSoundBufferCOM()->GetCurrentPosition(&this->dwCurrentPlayPos,NULL)))//get the current position
				return false;//return if this failed
		
		  if((this->dwCurrentPlayPos > 0) && (this->firstPlay == true))
		  {
					
			  return true;//return failed
		  }
		  else if((this->dwCurrentPlayPos == 0) && (this->firstPlay == true))
			  return false;
	return false;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SoundChannel::IsPlaying()
{
	
	  if(this->GetSoundBufferCOM() == NULL)//if the sound buffer COM exist
		  return false;//return fail
	  
		  if(FAILED(this->GetSoundBufferCOM()->GetCurrentPosition(&this->dwCurrentPlayPos,NULL)))//get the current position
				return false;//return if this failed
		
		  if((this->dwCurrentPlayPos > 0))//if the current position is > 0
		  {
					
			  return true;//return true is playing
		  }
		  else if((this->dwCurrentPlayPos == 0))//if it equals 0
			  return false;//return not playing
	return false;//otherwise return false for not playing
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           IsPaused
//	Class:			SoundChannel
//  Description:    check for the whether current playback is paused or not
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SoundChannel::IsPaused()
{
	return m_Pause;//return pause flag
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           BufferData
//	Class:			SoundChannel
//  Description:    handles the copying from the sound file to the buffer for wav streaming
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL SoundChannel::BufferData()
{
  long Pos, Size;//init position and size vars
  long ToLoad, LoadPos;//intit vars
  char *Ptr;//set pointers

  if(m_pDSBuffer == NULL)//if secondary buffer doesn't exist
    return FALSE;

  // Setup position to load in
  Pos  = (m_LoadSection % 4) * m_Desc.g_SoundBufferChunk;//calculate the new position
  if(FAILED(m_pDSBuffer->Lock(Pos, m_Desc.g_SoundBufferChunk, (void**)&Ptr, (DWORD*)&Size, NULL, NULL, 0)))
	  //lock buffer at that position
    return FALSE;//return failed
    
  // Clear out buffer if nothing left to load
  if(!m_Desc.m_Left)//if no data at all
    ZeroMemory(Ptr, Size);//zero out memory to new size 
  else {
    // Load in the data - take looping into account
    ToLoad = Size;//set the new size of buffer
    LoadPos = 0;//set the load position
    for(;;) {//infinit loop
      if(m_Desc.m_Left > ToLoad) 
	  {//if new size is less than the total buffer size
        if(m_Desc.m_fp != NULL) 
		{//check if file pointer exist
          fseek(m_Desc.m_fp, m_Desc.m_Pos, SEEK_SET);//move file marker to new position
          fread(&Ptr[LoadPos], 1, ToLoad, m_Desc.m_fp);//read in data from file up to the new size into buffer
        }
		else //if no file pointet(future ogg streaming code)
          memcpy(&Ptr[LoadPos], &m_Desc.m_Ptr[m_Desc.m_Pos], ToLoad);//copy buffer to seconday buffer
        m_Desc.m_Left -= ToLoad;//subtract the size from the buffer to move the marker to the beginning 
        m_Desc.m_Pos += ToLoad;//subtract the size from the buffer to move the marker towards the end.
        break;//break 
      } else {//if left marker is less than new size
        if(m_Desc.m_fp != NULL)
		{//if file pointer does exist
          fseek(m_Desc.m_fp, m_Desc.m_Pos, SEEK_SET);//move the file pointer to the new position
          fread(&Ptr[LoadPos], 1, m_Desc.m_Left, m_Desc.m_fp);//read from left to position into the buffer
        } else //if it does not exist
          memcpy(&Ptr[LoadPos], &m_Desc.m_Ptr[m_Desc.m_Pos], m_Desc.m_Left);// copy from memory
        ToLoad -= m_Desc.m_Left;//move marker further left
        LoadPos += m_Desc.m_Left;//move position to the right

        // Check if we need to stop loop
        //if(isLoop() == false) {//if looping is set
        //  setLoop(false);//turn it off
          
            // Clear out remaining buffer space
           // if(ToLoad)//new size > 0
           //   ZeroMemory(&Ptr[LoadPos], ToLoad);//zero out buffer with the new size
           // m_Desc.m_Left = 0L;//set the left marker to far left
           // break;//break
          
        //}

        m_Desc.m_Pos = m_Desc.m_StartPos;//set the current position to the position marker
        m_Desc.m_Left = m_Desc.m_Size;//set the left marker to the size

        // See if we need to stop loading data
        if(!ToLoad)//if size is 0 
          break;//break
      }
    }
  }

  // Unlock the buffer
  m_pDSBuffer->Unlock(Ptr, Size, NULL, 0);//unlock the secondary buffer

  // Mark next section to load
  if((m_LoadSection+=1) > 3)//set the next notify event
    m_LoadSection = 0;//set the loadsection to 0

  return TRUE;//set to true for success
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Update
//	Class:			SoundChannel
//  Description:    sets the next section to be uploaded into the secondary buffer
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL SoundChannel::Update()
{
  // Check for end of sound
	if(m_NextNotify == m_StopSection && !m_Desc.m_Left) {
    Stop();//if so, stop
  } else {
    // Buffer in more data
    BufferData();
    if((m_NextNotify+=1) > 3)//if the next event index is 4
      m_NextNotify = 0;//reset it
  }

  return TRUE;//return successful
}







//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ChangePauseState
//	Class:			SoundChannel
//  Description:    change the playback pause/not pause state
//  Arguments:      NONE
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SoundChannel::ChangePauseState()
{

	if(m_Pause==1)//if the pause flag is true 
		m_Pause=0;//set it to false 
	else
		m_Pause=1;//set it to true
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setLoop
//	Class:			SoundChannel
//  Description:    sets the playback loop flag
//  Arguments:      BOOL
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SoundChannel::setLoop(bool input)
{
	m_Loop=input;
	



}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           isLoop
//	Class:			SoundChannel
//  Description:    chack to see if the current playback is looped
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SoundChannel::isLoop()
{
	return m_Loop;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           isOgg
//	Class:			SoundChannel
//  Description:    flagto check to see if the current channel is OGG file
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SoundChannel::isOgg()
{
	return is_ogg;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setOgg
//	Class:			SoundChannel
//  Description:    set the current channel as an ogg file
//  Arguments:      BOOL
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void SoundChannel::setOgg(bool input)
	{


		is_ogg=input;


	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           nonLoopPlay
//	Class:			SoundChannel
//  Description:    handle pause/not pause states during non-looped playback
//  Arguments:      NONE
//  Return Value:   INT
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
int SoundChannel::nonLoopPlay()
{
	
	
	if(this->firstPlay == false)
	{
		if(this->isOgg() == false)
		{
			if(FAILED(this->m_pDSBuffer->SetCurrentPosition(0)))//set the current position of the seconday buffer
				return 0;//return if failed
			if(FAILED(this->m_pDSBuffer->Play(0,0,NULL)))//play sound 
				return 0;//return if failed
			
			

			this->firstPlay=true;
		}
		else if(this->isOgg() == true)
		{

			if(FAILED(this->m_pDSBuffer->SetCurrentPosition(0)))//set the current position of the seconday buffer
				return 0;//return if failed
			if(FAILED(this->m_pDSBuffer->Play(0,0,NULL)))//play sound 
				return 0;//return if failed
			
			this->firstPlay=true;
		}
	}
	if((this->IsPaused()== true) && (this->m_flag==0))//if the pause flag is set to true
	 {//--3
		 if(FAILED(this->m_pDSBuffer->GetCurrentPosition(&this->dwCurrentPlayPos,NULL)))//get the current position
			return 0;//return if this failed
		
		 if(FAILED(this->m_pDSBuffer->Stop()))//stop playback
			return 0;//return if failed
		 this->m_flag=1;//set the pause flag to true
		 
	 }//--3
	if((this->IsPaused()== false) && (this->m_flag==1))//if the pause flag is set to true
	{
		if(this->isOgg() == false)
		{
			if(FAILED(this->m_pDSBuffer->SetCurrentPosition(this->dwCurrentPlayPos)))//set the current position 
				return 0;//return failed
			if(FAILED(this->m_pDSBuffer->Play(0,0,DSBPLAY_LOOPING)))//play from that position
				return 0;//return failed
			this->m_flag=0;//set the pause flag to 0.
		}
		else if(this->isOgg() == true)
		{
			if(FAILED(this->m_pDSBuffer->SetCurrentPosition(this->dwCurrentPlayPos)))//set the current position 
				return 0;//return failed
			if(FAILED(this->m_pDSBuffer->Play(0,0,NULL)))//play from that position
				return 0;//return failed
			this->m_flag=0;//set the pause flag to 0.
		}


	}
	if(this->isLoop() == false && this->gethasLooped() == true)
	{
		
		if(FAILED(this->m_pDSBuffer->GetCurrentPosition(&this->dwCurrentPlayPos,NULL)))//get the current position
			return 0;//return if this failed
		
		 if(FAILED(this->m_pDSBuffer->Stop()))//stop playback
			 if(FAILED(this->m_pDSBuffer->SetCurrentPosition(this->dwCurrentPlayPos)))//set the current position 
				return 0;//return failed
			if(FAILED(this->m_pDSBuffer->Play(0,0,NULL)))//play from that position
				return 0;//return failed
			this->m_flag=0;//set the pause flag to 0.
			this->firstPlay=true;
			this->sethasLooped(false);
	}
return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           LoopPlay
//	Class:			SoundChannel
//  Description:    handle pause/non-paused states during looped playback
//  Arguments:      NONE
//  Return Value:   INT
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

int SoundChannel::LoopPlay()
{
	
	if(this->firstPlay == false)
	{
		if(this->isOgg() == false)
		{
			if(FAILED(this->m_pDSBuffer->SetCurrentPosition(0)))//set the current position of the seconday buffer
				return 0;//return if failed
			if(FAILED(this->m_pDSBuffer->Play(0,0,DSBPLAY_LOOPING)))//play sound 
				return 0;//return if failed
			
			this->firstPlay=true;
			this->sethasLooped(true);
		}
		else if(this->isOgg() == true)
		{

			if(FAILED(this->m_pDSBuffer->SetCurrentPosition(0)))//set the current position of the seconday buffer
				return 0;//return if failed
			if(FAILED(this->m_pDSBuffer->Play(0,0,DSBPLAY_LOOPING)))//play sound 
				return 0;//return if failed
			
			this->firstPlay=true;
			this->sethasLooped(true);
		}
	}
	if((this->IsPaused()== true) && (this->m_flag==0))//if the pause flag is set to true
	 {//--3
		 if(FAILED(this->m_pDSBuffer->GetCurrentPosition(&this->dwCurrentPlayPos,NULL)))//get the current position
			return 0;//return if this failed
		
		 if(FAILED(this->m_pDSBuffer->Stop()))//stop playback
			return 0;//return if failed
		 this->m_flag=1;//set the pause flag to true
		 this->sethasLooped(true);
		 
	 }//--3
	if((this->IsPaused()== false) && (this->m_flag==1))//if the pause flag is set to true
	{
		if(this->isOgg() == false)
		{
			if(FAILED(this->m_pDSBuffer->SetCurrentPosition(this->dwCurrentPlayPos)))//set the current position 
				return 0;//return failed
			if(FAILED(this->m_pDSBuffer->Play(0,0,DSBPLAY_LOOPING)))//play from that position
				return 0;//return failed
			this->m_flag=0;//set the pause flag to 0.
			this->sethasLooped(true);
		}
		else if(this->isOgg() == true)
		{
			if(FAILED(this->m_pDSBuffer->SetCurrentPosition(this->dwCurrentPlayPos)))//set the current position 
				return 0;//return failed
			if(FAILED(this->m_pDSBuffer->Play(0,0,DSBPLAY_LOOPING)))//play from that position
				return 0;//return failed
			this->m_flag=0;//set the pause flag to 0.
			this->sethasLooped(true);
		}


	}
return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setFirstPlay
//	Class:			SoundChannel
//  Description:    set the state for the playback flag to give the ability to turn playback on/off
//  Arguments:      BOOL
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void SoundChannel::setFirstPlay(bool input)
{
	this->firstPlay=input;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getFirstPlay
//	Class:			SoundChannel
//  Description:    get the current state form the playback switch
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool SoundChannel::getFirstPlay()
{

	return this->firstPlay;



}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           sethasLooped
//	Class:			SoundChannel
//  Description:    flag to state whether a audio file has looped at least once
//  Arguments:      BOOL
//  Return Value:   VOID
// ----------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


void SoundChannel::sethasLooped(bool input)
{

	hasLooped=input;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           gethasLooped
//	Class:			SoundChannel
//  Description:    change the playback pause/not pause state
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SoundChannel::gethasLooped()
{

	return hasLooped;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setAlive
//	Class:			SoundChannel
//  Description:    set the flag to state whether a channel/thread is alive or dead
//  Arguments:      BOOL
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SoundChannel::setAlive(bool input)
{
	isAlive=input;


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getAlive
//	Class:			SoundChannel
//  Description:    get the flag to state whether a channel/thread is alive or dead
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SoundChannel::getAlive()
{
	return isAlive;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           turnon
//	Class:			SoundChannel
//  Description:    set flag to turn on/off playback
//  Arguments:      BOOL
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SoundChannel::turnon(bool input)
{
	startPlayLoop=input;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ReleaseCOM
//	Class:			SoundChannel
//  Description:    release all COM objects
//  Arguments:      class T
//  Return Value:   VOID
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<class T>
	void SoundChannel::ReleaseCOM(T& x)
	{
		if(x)
		{
			x->Release();
			ZeroMemory(&x,sizeof(x));
			x = NULL;
		}
}






#endif
 

