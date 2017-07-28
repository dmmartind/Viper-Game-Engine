//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: Audio Interface Code
//
//**********************************************vk)****************************************




#ifndef _AUDIO_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define _AUDIO_CPP_//define one time and let everyone share, so VS doesn't give annoying errors

    
#include "Audio.h"//audio header



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Sample
//	Class:			Sample
//  Description:    constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Sample::Sample()
    {
		index_count=0;//set the index count to 0
		
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ~Sample
//	Class:			Sample
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    Sample::~Sample()
    {
		//data.Free(); //free sound data object       
		//channel.Free();//free sound channel object
		
		index_count=0;//set index to 0
		name=NULL;//set audio name to null
        
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Audio
//	Class:			Audio
//  Description:    constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Audio::Audio()//
    {
        //nothing for now
    }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Audio
//	Class:			Audio
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    Audio::~Audio()
    {
		StopAll();//stops all play
		
        //release all samples
        for (Iterator i = samples_vec.begin(); i != samples_vec.end(); ++i)//empty out all references
        {
			(*i)->channel.turnon(false);//set the audio playback to false
			TerminateThread((*i)->p_Thread, 0);//terminate the thread
			(*i)->~Sample();//call the deconstructor for the audio sample class
			(*i) = NULL;//set node to null
        }
        
		//samples_vec.clear();//clear out vector
		
			
    }

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Init
//	Class:			Audio
//  Description:	Audio interface initialization
//  Arguments:      hwnd
//  Return Value:   Bool
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    bool Audio::Init(HWND hwnd)
    {					//app handle,frequency,channel,Bit resolution,sound card priority
		if ((system.Init(hwnd, 44100,2,16,DSSCL_PRIORITY)) == false)//initialize audio interface with default attributes
            return false;//return if failed
            
        return true;//otherwise return true for success
    }





/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Load
//	Class:			Audio
//  Description:	WAV:fill the attributes of the RIFF header of the WAV file and set the reference to the wav file
	//				to the data channel.
	//				OGG: fill the attributes of the RIFF header of the OGG file and decode the OGG to WAV and copy 
	//				the WAV to a buffer in its own class to hold until the channel buffer is created.
//  Arguments:      filename, reference name
//  Return Value:   bool
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


    
    bool Audio::Load(char *filename, char *name)
	{
        if (strlen(filename) == 0 || strlen(name) == 0) return false;//make sure that bot arguments have something

		Sample *new_sample = new Sample();//create new sample object
		
		if(!SampleExists(filename))//if file exist
			new_sample->setName(name);//set reference name to the object
		else//otherwise cleanup
		{
			if(new_sample != NULL)//if sample is valid
				delete new_sample;//delete it
			new_sample=NULL;//otherwise set it to NULL
			return false;//return fail
		}

		FILE *fp= new FILE();//create file pointer
		


		if((strstr(filename,".ogg")) != NULL)//if ogg file
			{
				
				CWAVFile *wav_data;//create wave file object
				wav_data= new CWAVFile;//create new instance of wav file object
				wav_data=new_sample->data.LoadOGG(filename);//get RIFF attributes and read into buffer and
				//put into the object
				
				new_sample->data.Load_OGG_Wav_Header(wav_data);//set the RIFF header to the datachannel
				
				new_sample->data.SetSource_WAV_OGG(wav_data);//set the buffer to the data channel
				
				//ZeroMemory(&wav_data,sizeof(wav_data));
				wav_data=NULL;//set wav data class to null
				//fp=NULL;
				
			}
		else if ((strstr(filename,".wav")) != NULL)//if wav file
		{
			if((fp=new_sample->data.LoadWAVHeader(filename,fp)) == 0)//get the RIFF header and set that into the
			{	//data channel
				if(new_sample != NULL)//if failed,check to see if the sample exist
					delete new_sample;//delete sample
				new_sample=NULL;//set the sample to null
				if(fp != NULL)//if the file pointer to exist
					fclose(fp);//close file
				//fp=NULL;			
				return false;//return failed
			}
			if((new_sample->data.SetSource(fp)) == false)//set the reference to the wav file
			{	//if failed
				if(new_sample != NULL)//if sample exist
					delete new_sample;//delete sample
				new_sample=NULL;//set the sample to null
				if(fp != NULL)//if file pointer exist
					fclose(fp);//close it
				//fp=NULL;
				return false;//return failed

			}
		}
			
		else//file not recognized as a playable format
		{
			if(new_sample != NULL)//check to see if the sample exist
					delete new_sample;//delete sample
				new_sample=NULL;//set the sample to null
				
				//fp=NULL;		
			return 0;//wrong file type
		}

/*
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if((new_sample->channel.Create(&system,&new_sample->data)) == false)
			{////////////////////////////////////clean up
					delete new_sample;
					new_sample = NULL;
					return false;
			}////////////////////////////////////////////////

			if(new_sample->data.getIsOGG())
				{
					new_sample->channel.LoadSoundData(0,new_sample->data.getPCMBUFFER()->getData(),new_sample->data.getSize());
				}
*/					








		/////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        samples_vec.push_back(new_sample);//push this new object into the queue
//////////////////////////////////////////////clean up

				
				new_sample=NULL;//set the sample to null
			//	if(fp != NULL)
			//		fclose(fp);
				//fp=NULL;
		
		//new_sample=NULL;
		fp=NULL; //set the file pointer to NULL@@@@@@@@@@@@@@
////////////////////////////////////////////////////////

    
        return true;    //success
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SampleExists
//	Class:			Audio
//  Description:	Does a particular reference exist
//  Arguments:      reference name
//  Return Value:   bool
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    bool Audio::SampleExists(char *name)
    {
        for (Iterator i = samples_vec.begin(); i != samples_vec.end(); ++i)//go through the queue
        {
            if ((*i)->getName() == name)//return true if reference is found
            {
                return true;//return success
            }         
        }
    
        return false;//return false if not found
    }

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           IsPlaying
//	Class:			Audio
//  Description:	check to see if the reference is still playing
//  Arguments:      reference name
//  Return Value:   bool
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    bool Audio::IsPlaying(char *name)
    {
		Sample *samp = FindSample(name);//fine the reference and return it to a new sample object
        
		if(samp->channel.IsPlaying())//check to see if reference is playing
		{
			samp=NULL;//set the sample to null
			return true;//true if it is
		}
		else
		{
			samp=NULL;//set the sample to null
			return false;//false if it isn't
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           FindSample
//	Class:			Audio
//  Description:	check to see if reference is in the queue and return reference
//  Arguments:      reference name
//  Return Value:   sample
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Sample *Audio::FindSample(char* name)//out of bounds check
    {
		int counter=0;//set the index counter to 0
        Sample *sample = NULL;//create new sample object, but don't initialize it to use it as a sample pointer
        for (Iterator i = samples_vec.begin(); i != samples_vec.end(); ++i)//interate through the queue
        {	counter++;//increment counter
		if ((*i)->getName() == name)//find reference
            {
                sample = (*i);//set the sample to the object
				sample->index_count=counter;//set the index counter to the current reference counter
                break;//exit loop
            }
        }

		
        return sample;//return object
    }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Play
//	Class:			Audio
//  Description:	play a the file via reference
//  Arguments:      reference name, volume, loop flag
//  Return Value:   sample
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    bool Audio::Play(char *name, int vol, bool loop)
    {
		cData *pdata;//data object for the play thread
		pdata= new cData;//create a new instance as a cData object for the play thread
        Sample *sample = FindSample(name);//find the audio
		
    
        if (sample != NULL)//if the sample exist
		{
			if(sample->channel.IsPlaying() == true || sample->channel.IsPaused())//check audio for playing state or paused state
			{/////////////////////////////////////////clean up
				//clean up
				sample=NULL;//set the sample to null
				pdata= NULL;//set the thread data to null
				
				return false;//return fail
			}////////////////////////////////////////////////// 

			if(sample->channel.getAlive() == true)//check to see the channel is alive
			{/////////////////////////////////////////clean up
				//if so
				sample->channel.turnon(true);//turn on channel
				
				sample=NULL;//set the sample to null
				pdata= NULL;//set the thread data to null
				
				return false;//return false
			}////////////////////////////////////////////////// 



			if((sample->channel.Create(&system,&sample->data)) == false)//create channel
			{////////////////////////////////////if failed, clean up
					if(sample)//if sample exist, 
						delete sample;//delete sample
					sample=NULL;//set the sample to null
					pdata=NULL;	//set the thread data to null
					return false;//return failed
			}////////////////////////////////////////////////

			if(sample->data.getIsOGG())//if the data node is an OGG format
				{
					sample->channel.LoadSoundData(0,sample->data.getPCMBUFFER()->getData(),sample->data.getSize());//load data to channel
				}

				
				
				//fill the thread data struct
				pdata->setSoundData(&sample->data);//set the pointer to the sounddata
				pdata->setVolume(vol);//set the volume
				pdata->setLoop(loop);//set the loop flag
				pdata->setSoundChannel(&sample->channel);//set the channel pointer

				

				
				if((sample->p_Thread = CreateThread(NULL, 0,sample->channel.Play,pdata, 0, &sample->p_ThreadID)) == NULL)//create the channel thread and play file
				{/////////////////////////////////////////////if failed, clean up
					if(pdata)//if pdata exist
						delete pdata;//delete thread data object
					if(sample)//if sample exist
						delete sample;//delete sample
					sample=NULL;//set the sample to null
					pdata=NULL;//set the thread data to null
					return false;//return false
				}////////////////////////////////////////////////////////
					
					sample=NULL;//set sample to null
					pdata=NULL;//set thread data to null
			return true;	//return success
		}  
		////////////////////////////////////////////clean up
					if(sample)//if sample exist
						delete sample;//delete sample
					sample=NULL;//set sample to null
					pdata=NULL;//set data to null
					return false;//return failed
		/////////////////////////////////////////////////////

        
    }//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@





/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Stop
//	Class:			Audio
//  Description:	stop playing sound via reference name
//  Arguments:      reference name
//  Return Value:   BOOL 
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    BOOL Audio::Stop(char *name)
    {
        if (!IsPlaying(name)) //check reference to see if it's playing
			return false;//return failed
    
        Sample *sample = FindSample(name); //find the node
        if (sample == NULL) //if sample is null
					return false;//return fail
		
		sample->channel.turnon(true);//turn on the channel
		sample->channel.setLoop(false);//change the loop to 0
		sample->channel.Stop();//stop playing sound
		
		

		//TerminateThread(sample->p_Thread, 0);//terminate the thread

		
		sample=NULL;//set the sample to null
		return true;//return success
    }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           StopAll
//	Class:			Audio
//  Description:	stop playing all sounds
//  Arguments:      NONE
//  Return Value:   VOID 
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


    
    void Audio::StopAll()
    {
        for (Iterator i = samples_vec.begin(); i != samples_vec.end(); ++i)//iterate through the sound vector 
        {
			(*i)->channel.turnon(true);//turn on channel
			(*i)->channel.Stop();//stop sound
			
        }
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           StopAllExcept
//	Class:			Audio
//  Description:	stop all sound except for the reference of the sound sent to the function
//  Arguments:      sound reference name
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    void Audio::StopAllExcept(char *name)
    {
        for (Iterator i = samples_vec.begin(); i != samples_vec.end(); ++i)//iterate through the vector
        {
            if ((*i)->getName() != name)//if the current node doesn't match the name
            {	
				(*i)->channel.turnon(true);//turn on the channel
				(*i)->channel.Stop();//stop the playback
				
            }
        }
    }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           ChangeVolume
//	Class:			Audio
//  Description:	change the volume of the sound by the reference name
//  Arguments:      sound reference name, volume level
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Audio::ChangeVolume(char *name, int level)
	{

		Sample *sample = FindSample(name);//find the node
    
        sample->channel.SetVolume(level);//set the supplied volume 
		
		sample=NULL;//set the sample ptr to null

	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Pan
//	Class:			Audio
//  Description:	pan sound between -100 to 100 from left , center, right
//  Arguments:      sound reference name, level of pan
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void Audio::Pan(char *name, int level)
	{

		Sample *sample = FindSample(name);//find the node

		sample->channel.SetPan(level);//set the pan

		sample=NULL;//set the sample ptr to null
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Frequency
//	Class:			Audio
//  Description:	change the sample frequency that the playback will be sampled at
//  Arguments:      sound reference name, frequency
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Audio::Frequency(char *name, int level)
	{

		Sample *sample = FindSample(name);//find the node

		sample->channel.SetFrequency(level);//set the frequency

		sample=NULL;//set the sample ptr to null
		
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           SetLoop
//	Class:			Audio
//  Description:	change the looping state of the referenced sound
//  Arguments:      sound reference name
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Audio::SetLoop(char *name)
	{
		Sample *sample = FindSample(name);//find the node

		sample->channel.setLoop(true);//set the loop to true

		sample=NULL;//set the sample ptr to null
	}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           StopLoop
//	Class:			Audio
//  Description:	change the looping state of the referenced sound
//  Arguments:      sound reference name
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Audio::StopLoop(char *name)
	{
		Sample *sample = FindSample(name);//find the node

		sample->channel.setLoop(false);//set the loop to false

		sample=NULL;//set the sample ptr to null

	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------
//  Name:           Pause
//	Class:			Audio
//  Description:	pause the referenced sound
//  Arguments:      sound reference name
//  Return Value:   void
// ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Audio::Pause(char *name)
	{
		Sample *sample = FindSample(name);//find the node

		sample->channel.ChangePauseState();//set the pause flag to true, if true, false otherwise

		sample=NULL;//set the sample ptr to null
	}
	

#endif
