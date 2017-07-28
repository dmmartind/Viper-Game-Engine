//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: Sound Class Code

//**********************************************vk)****************************************


#ifndef _SOUND_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define _SOUND_CPP_//define one time and let everyone share, so VS doesn't give annoying errors


#include "Sound.h"  //sound header


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           cData
//	Class:			cData
//  Description:    constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


cData::cData()
{
	VolumePercent=0;  //set volume to 0
	Loop=false;			//set loop to 0
	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           cData
//	Class:			cData
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

cData::~cData()
{
	Desc->Free();//free sound object
	sChannel->Free();//free channel object
	long VolumePercent=0;//set volume to 0
	bool Loop=false;//set the loop flag to 0

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getSoundData
//	Class:			cData
//  Description:    return the sound data object
//  Arguments:      None.
//  Return Value:   SoundData*
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SoundData* cData::getSoundData()
	{

		return Desc;//sound data object
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setSoundData
//	Class:			cData
//  Description:    set the sound data object
//  Arguments:      SoundData*
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void cData::setSoundData(SoundData* input)
	{

		Desc=input;

	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getLoop
//	Class:			cData
//  Description:    retun the value of the loop flag
//  Arguments:      None.
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool cData::getLoop()
	{
		
	return Loop;
	

	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setLoop
//	Class:			cData
//  Description:    set the value of the loop flag
//  Arguments:      bool
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void cData::setLoop(bool input)
	{

		Loop=input;




	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getSoundChannel
//	Class:			cData
//  Description:    return sound channel
//  Arguments:      None.
//  Return Value:   SoundChannel
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SoundChannel* cData::getSoundChannel()
	{
		return sChannel;

	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setSoundChannel
//	Class:			cData
//  Description:    set sound channel
//  Arguments:      SoundChannel
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void cData::setSoundChannel(SoundChannel* input)
	{


		sChannel=input;



	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           getVolume
//	Class:			cData
//  Description:    return volume
//  Arguments:      None.
//  Return Value:   long
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	long cData::getVolume()
	{

		return VolumePercent;//volume var




	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           setVolume
//	Class:			cData
//  Description:    set the volume of the channel
//  Arguments:      long
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void cData::setVolume(long input)
	{


		VolumePercent=input;//volume var

	}

//////////////////////////////////////////////////////////////////////
//
// Sound Functions
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Sound
//	Class:			Sound
//  Description:    constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
Sound::Sound()
{
  short i;//loop counter
  
  // Initialize COM
  CoInitialize(NULL);//initialize COM

  m_hWnd   = NULL;//set window handle to null
  m_Volume = 0;//set volume to 0

  m_hThread = NULL;//set thread handle to null
  m_ThreadID = NULL;//set thread id to null
  m_ThreadActive = FALSE;//set the thread active to false
  m_CooperativeLevel=0;//set the level of priority on the buffer
  m_Frequency=0;//set the frequency of the buffer to 0
  m_Channels=0;//set the channels to 0
  m_BitsPerSample=0;//set the bit resolution to 0

  m_pDS = NULL;		//set directx sound object to null
  m_pDSBPrimary = NULL;//set the directx primary sound buffer to null



  for(i=0;i<32;i++)
    m_EventChannel[i] = NULL;//initialize the event channel array

  for(i=0;i<33;i++)
    m_Events[i] = NULL;//initialize the event array
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ~Sound
//	Class:			Sound
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Sound::~Sound()
{
  Shutdown();//call shutdown

  // Uninitialize COM
  CoUninitialize();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Init
//	Class:			Sound
//  Description:    initializes the directx sound object and primary sound buffer
//  Arguments:      window handle, frequency, channels, bit resolution,priority level
//  Return Value:   Bool
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Sound::Init(HWND hWnd, long Frequency, short Channels, short BitsPerSample, long CooperativeLevel)
{
  DSBUFFERDESC dsbd; //buffer description structure
  WAVEFORMATEX wfex; //buffer format structure
  
  short        i;	//loop iterater
  
  // Shutdown system in case of prior install
  Shutdown();

  // Save parent window handle
  if((m_hWnd = hWnd) == NULL)
    return FALSE;

  ///////////////////////////////////////////////////////////////////
  // Initialize DirectSound
  ///////////////////////////////////////////////////////////////////

  // Save settings of sound setup
  if(CooperativeLevel == DSSCL_NORMAL)
    CooperativeLevel = DSSCL_PRIORITY;//set buffer priority
  
m_CooperativeLevel = CooperativeLevel;//set buffer priority to the sound object
  m_Frequency        = Frequency;//set frequency to the sound object
  m_Channels         = Channels;//set number of channels to the sound object
  m_BitsPerSample    = BitsPerSample;//set the bit resolution to the sound object

  // create an IDirectSound8 object
  if(FAILED(DirectSoundCreate8(NULL, &m_pDS, NULL)))
  {
	  //return failed and set values to NULL or 0
		m_hWnd=NULL;
		m_CooperativeLevel = 0;
		m_Frequency= 0;
		m_Channels= 0;
		m_BitsPerSample=0; 

    return FALSE;//failed
  }

  // Set cooperative mode
  if(FAILED(m_pDS->SetCooperativeLevel(m_hWnd, m_CooperativeLevel)))
  {
	  //return failed and set values to NULL or 0
		m_hWnd=NULL;
		m_CooperativeLevel = 0;
		m_Frequency= 0;
		m_Channels= 0;
		m_BitsPerSample=0;


    return FALSE;
  }
  // Get primary buffer control
  ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));//clear memeory for structure
  dsbd.dwSize        = sizeof(DSBUFFERDESC);//set the memory size to default
  //set the flags to create a primary buffer that has volume control and can reports back it's current position
  dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2;
  dsbd.dwBufferBytes = 0;//set the buffer to 0 because it's just the primary buffer
  dsbd.lpwfxFormat   = NULL;//set the buffer to NULL for primary buffer
  if(FAILED(m_pDS->CreateSoundBuffer(&dsbd, &m_pDSBPrimary, NULL)))//create sound buffer 
  {		//return failed and set values to NULL or 0
		m_hWnd=NULL;
		m_CooperativeLevel = 0;
		m_Frequency= 0;
		m_Channels= 0;
		m_BitsPerSample=0;
		ZeroMemory(&dsbd, sizeof(dsbd));//clear memory for structure   
	  return FALSE;//false
  }
  // Set the primary buffer format
  ZeroMemory(&wfex, sizeof(WAVEFORMATEX));//zero out the memory for the structure 
  wfex.wFormatTag      = WAVE_FORMAT_PCM; //set the format to wave format
  wfex.nChannels       = (WORD)m_Channels;//set the number of channels
  wfex.nSamplesPerSec  = m_Frequency;//set the frequency
  wfex.wBitsPerSample  = (WORD)m_BitsPerSample;//set the bit resolution
  wfex.nBlockAlign     = wfex.wBitsPerSample / 8 * wfex.nChannels;//amount of bytes per block
  wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;//bytes per second
  if(FAILED(m_pDSBPrimary->SetFormat(&wfex)))//set the format for the primary buffer
  {
	  //return failed and set values to NULL or 0
	m_hWnd=NULL;
	m_CooperativeLevel = 0;
	m_Frequency= 0;
	m_Channels= 0;
	m_BitsPerSample=0;
	ZeroMemory(&dsbd, sizeof(dsbd));//clear memory for structure 
	ZeroMemory(&wfex, sizeof(wfex));//zero out the memory for the structure 
	return FALSE;//
  }
  // Create the events, plus 
  // an extra one for thread termination
  for(i=0;i<33;i++) {
    if((m_Events[i] = CreateEvent(NULL,FALSE,FALSE,NULL)) == NULL)//creates a non-signaled auto rest object and returns it
		//to the array
	{	//return failed and set values to NULL or 0
		m_hWnd=NULL;
		m_CooperativeLevel = 0;
		m_Frequency= 0;
		m_Channels= 0;
		m_BitsPerSample=0;
		ZeroMemory(&dsbd, sizeof(dsbd));//clear memory for structure 
		ZeroMemory(&wfex, sizeof(wfex));//zero out the memory for the structure 
		
		return FALSE;//return failed
	}
  }

  // Create a thread for handling notifications
  if((m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleNotifications, (LPVOID)this, 0, &m_ThreadID)) == NULL)
  {
		//return failed and set values to NULL or 0
		m_hWnd=NULL;
		m_CooperativeLevel = 0;
		m_Frequency= 0;
		m_Channels= 0;
		m_BitsPerSample=0;
	  	ZeroMemory(&dsbd, sizeof(dsbd));//clear memory for structure 
		ZeroMemory(&wfex, sizeof(wfex));//zero out the memory for the structure    
	  return FALSE;//return failed
  }
  // Start main buffer playing
  if(FAILED(m_pDSBPrimary->Play(0, 0, DSBPLAY_LOOPING)))
  {     //return failed and set values to NULL or 0
		m_hWnd=NULL;
		m_CooperativeLevel = 0;
		m_Frequency= 0;
		m_Channels= 0;
		m_BitsPerSample=0;
		ZeroMemory(&dsbd, sizeof(dsbd));//clear memory for structure 
		ZeroMemory(&wfex, sizeof(wfex));//zero out the memory for the structure 
    return FALSE;//return fail
  }
  

  // Set default volume to full
	SetVolume(100);
	ZeroMemory(&dsbd, sizeof(dsbd));//clear memory for structure 
	ZeroMemory(&wfex, sizeof(wfex));//zero out the memory for the structure 
  
  return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Shutdown
//	Class:			Sound
//  Description:    garbage collector for the class
//  Arguments:      None
//  Return Value:   Bool
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Sound::Shutdown()
{
  short i;//loop iterator

  

  // Go through all used sound channels and free them
  for(i=0;i<32;i++) {
    if(m_EventChannel[i] != NULL) {//if the channel has something
      m_EventChannel[i]->Free();//free it
      m_EventChannel[i] = NULL;//and set it to NULL
    }

    // Clear the event status
    if(m_Events[i] != NULL)//if an event exist
      ResetEvent(m_Events[i]);//reset the event
  }

  // Stop the primary channel from playing
  if(m_pDSBPrimary != NULL)//if primary buffer exist,
    m_pDSBPrimary->Stop();//then stop it

  // Release the DirectSound objects
  ReleaseCOM(m_pDSBPrimary);//release primary buffer
  ReleaseCOM(m_pDS);//release sound buffer

  // Force a closure of the thread by triggering the last event
  // and waiting for it to terminate
  if(m_hThread != NULL) {//if thread exist
    if(m_Events[32] != NULL) {//and an event exist
      while(m_ThreadActive == TRUE)//and while thread is still active
        SetEvent(m_Events[32]);//set event to continue 
    } else {
      // getting here means no event assigned on thread
      // and need to terminate it - not desirable
      TerminateThread(m_hThread, 0);
    }
  }

  // Close all event handles
  for(i=0;i<33;i++) {
    if(m_Events[i] != NULL) {//if event exist
      CloseHandle(m_Events[i]);//close handle
      m_Events[i] = NULL;//and set it to null
    }
  }

  // Free the thread handle
  if(m_hThread != NULL) {//if thread exist
    CloseHandle(m_hThread);//close handle
    m_hThread = NULL;//set the thread to null
  }
  m_ThreadID = NULL;//set thread id to null

  //set all to 0
m_Volume=0;
m_ThreadActive=false;
m_CooperativeLevel=0;
m_Frequency=0;
m_Channels=0;
m_BitsPerSample=0;
 m_hWnd   = NULL;//set window handle to null
  return TRUE;//return true
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetDirectSoundCOM
//	Class:			Sound
//  Description:    returns directx sound object
//  Arguments:      None
//  Return Value:   IDirectSound8
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
IDirectSound8 *Sound::GetDirectSoundCOM()
{
  return m_pDS;//return  directx object
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetPrimaryBufferCOM
//	Class:			Sound
//  Description:    returns directx sound buffer
//  Arguments:      None
//  Return Value:   IDirectSoundBuffer
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
IDirectSoundBuffer *Sound::GetPrimaryBufferCOM()
{
  return m_pDSBPrimary;//returns directx sound buffer
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           AssignEvent
//	Class:			Sound
//  Description:    copies the event address from the event array to the event handle; sets the event array to it's counter
//  Arguments:      sound channel, eventNum, EventHandle
//  Return Value:   Bool
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Sound::AssignEvent(SoundChannel *Channel, short *EventNum, HANDLE *EventHandle)
{
	//AssignEvent(this, &m_Event, &hEvent)
  short i;//loop iterator

  for(i=0;i<32;i++) {
    if(m_Events[i] != NULL && m_EventChannel[i] == NULL) {//if the event exist and channel doesn't exits then put one
		//otherwise go to the next interation
      ResetEvent(m_Events[i]);//resets state of event object
      m_EventChannel[i] = Channel;//set the event channel to the current channel
      *EventNum = i;//keeps the last entry of the event object array
      *EventHandle = m_Events[i];//copies the event object to a pointer to be copied to attributes at the return of this
      return TRUE;//return true for success
    }
  }

  return FALSE;//return false for unsuccessful
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ReleaseEvent
//	Class:			Sound
//  Description:    release event from the array
//  Arguments:      sound channel, eventNum
//  Return Value:   Bool
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL Sound::ReleaseEvent(SoundChannel *Channel, short *EventNum)
{
	
  if((unsigned short)*EventNum < 33 && m_EventChannel[*EventNum] == Channel) {// for each event in existance and
	  //if the sound channel exist
    ResetEvent(m_Events[*EventNum]);//reset the event address if changed
    m_EventChannel[*EventNum] = NULL;//set the event channel array to NULL
    *EventNum = -1;//set the event num var to -1 for no count
    return TRUE;//return successful
  } 

  return FALSE;//return unsuccessful
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           GetVolume
//	Class:			Sound
//  Description:    return the volume level
//  Arguments:      NONE
//  Return Value:   LONG
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
long Sound::GetVolume()
{
  return m_Volume;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SetVolume
//	Class:			Sound
//  Description:    set the volume and return the bool value of success or fail
//  Arguments:      percent level of volume
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL Sound::SetVolume(long Percent)
{
  long Volume;//create a volume var

  // Set the sound main volume
  if(m_pDSBPrimary == NULL)//if the buffer is not valid
    return FALSE;//return  failed

  // calculate a usable volume level
  if(!Percent)//if percent = 0
    Volume = DSBVOLUME_MIN;//set the minimal volume level
  else 
    Volume = -20 * (100 - (Percent % 101));//set volume

  if(FAILED(m_pDSBPrimary->SetVolume(Volume)))//set the volume level
    return FALSE;//if failed, return failed

  m_Volume = Percent % 101;//set the volume attribute with the new info

  return TRUE;//return successful
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           HandleNotifications
//	Class:			Sound
//  Description:    notification manager
//  Arguments:      lpvoid object
//  Return Value:   DWORD
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD Sound::HandleNotifications(LPVOID lpvoid)
{
  DWORD   dwResult, Channel;//vars to hold result from current threads and holds the channel number of the threads
  //respectfully.
  Sound *SoundPtr;//sound class object
  BOOL    Complete;//flag to terminate the notification manager thread
  MSG     Msg;//message structure

  SoundPtr = (Sound*)lpvoid;//get the sound class object from lpvoid

  SoundPtr->m_ThreadActive = TRUE;//set the thread active attribute

  Complete = FALSE;//set flag that the process has not completed

  while(Complete == FALSE) {//while not completed
    // Wait for a message
    dwResult = MsgWaitForMultipleObjects(33, SoundPtr->m_Events,
                                         FALSE, INFINITE, 
                                         QS_ALLEVENTS);//waits for any of the threads to signal a return

    // Get channel # to update
    Channel = dwResult - WAIT_OBJECT_0;//get the channel number of the signaled thread

    // Check for channel update
    if(Channel >=0 && Channel < 32) {//if channel is within bounds
      if(SoundPtr->m_EventChannel[Channel] != NULL)//if event channel exist
        SoundPtr->m_EventChannel[Channel]->Update();//then update the buffer
    } else

    // Check for thread closure
    if(Channel == 32) {//reserved channel for closing notification manager thread
      Complete = TRUE;//set the completed flag to true
    } else

    // Check for waiting messages
    if(Channel > 32) {//if the current channel is > 32
      while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {//get message and extract data from it
        if(Msg.message == WM_QUIT) {//if app quits, then set complete flag to terminate
          Complete = TRUE;//set the complete flag to true
          break;
        }
      }
    }
  }

  SoundPtr->m_ThreadActive = FALSE;//set the thread active to false

  return 0L;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Restore
//	Class:			Sound
//  Description:    restore all the sound buffers
//  Arguments:      NONE
//  Return Value:   BOOL
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL Sound::Restore()
{
  short i;//interator counter

  // Handle primary 
  if(m_pDSBPrimary != NULL)
    m_pDSBPrimary->Restore();//reallocates primary buffer

  // Handle all used sound channels
  for(i=0;i<32;i++) {//iterate through the event pointers
    if(m_EventChannel[i] != NULL)//if the channel is valid
      m_EventChannel[i]->m_pDSBuffer->Restore();//reallocates secondary buffers.
  }

  return TRUE;//return true

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ReleaseCOM
//	Class:			Sound
//  Description:    free up DirectX COM objects
//  Arguments:      class T
//  Return Value:   Void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
	void Sound::ReleaseCOM(T& x)
	{
		if(x)//if COM object is valid
		{
			x->Release();//release COM object
			ZeroMemory(&x,sizeof(x));//zero out the memory
			x = NULL;//set the pointer to null
		}
}


#endif
