//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Thread Class Code 
//
//****************************************************************************************
#include "Thread.h"


typedef void* (*start)(void*);//set void* to extract the start struct from a void* object


// ----------------------------------------------------------------
//  Name:           
//	Struct:			ThreadParam
//  Description:    thread parameter object
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------
struct ThreadParam
{
	start funct;
	void* arg;
};

// ----------------------------------------------------------------
//  Name:           cThread
//	Class:			cThread
//  Description:    constructor to create new thread object,get the handle of the thread, and the running state to false
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------
cThread::cThread()
{

	handle= new Thread_t();//thread creation
	running = false;//running state

}

// ----------------------------------------------------------------
//  Name:           ~cThread
//	Class:			cThread
//  Description:    deconstructor to set the handle to null and the running state to false
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------
cThread::~cThread()
{

	handle=NULL;//set the thread handle to null
	running = false;//set the running flag to false


}

// ----------------------------------------------------------------
//  Name:           threadProc
//	Class:			cThread
//  Description:    thread callback function the function inside a created thread
//  Arguments:      void *a
//  Return Value:   unsigned __stdcall
// ----------------------------------------------------------------
unsigned __stdcall cThread::threadProc(void *a)
{
	ThreadParam* param = (ThreadParam*)a;//create an instance of the ThreadParam struct and set to the parameter
	start func = param->funct;//create a start function funct and set it to the provided function pointer from the parameter
	void* arg = param->arg;//set the argument to the provided argument

	delete param;//delete parameter
	param =0;//set the param to 0

	unsigned rv = (unsigned)func(arg);//call the function and when the function ends return the handle
	_endthreadex(rv);//end thread
	return rv;//return handle

}


// ----------------------------------------------------------------
//  Name:           Thread_Create
//	Class:			cThread
//  Description:    creates the thread and calls its callback function to run the function within the thread
//  Arguments:      void* (*start)(void*), void* arg
//  Return Value:   int
// ----------------------------------------------------------------
int cThread::Thread_Create(void* (*start)(void*), void* arg)
{
	ThreadParam* param= new ThreadParam;//create new ThreadParam struct 
	param->funct = start;//set the function to the provided function
	param->arg = arg;//set the fnction to the provided argument
	unsigned id;//thread id 
	HANDLE h = (HANDLE)_beginthreadex(0,0,threadProc,param,0,&id);//create with 0 address and 0 stack, thread callback function,paramter object,running state,thread address
	if( 0 == h)//if the thread returned a 0
	{
		*handle = 0;//set the handle to 0
		delete param;//delete parameter
		return 0;//return failed
	}
	else
	{
		*handle = (Thread_t)h;//get the handle of the thread
		//Debug::println("The thread 0x{0,x} (handle=0x{0,x}) started.", (int)id, (int)h);
		return 1;//return success
	}

}

// ----------------------------------------------------------------
//  Name:           Thread_Detach
//	Class:			cThread
//  Description:    terminate thread
//  Arguments:      None
//  Return Value:   int
// ----------------------------------------------------------------

int cThread::Thread_Detach()
{
	HANDLE h = (HANDLE)handle;
	if(h)
	{
		//Debug::println("Detaching thread (handle=0x{0,x}).", (int)h);
		CloseHandle(h);
		return 0;
	}

	return 1;
}

// ----------------------------------------------------------------
//  Name:           Thread_Join
//	Class:			cThread
//  Description:    waits for a thread to terminate for an infinite amount of time
//  Arguments:      None
//  Return Value:   int
// ----------------------------------------------------------------

int cThread::Thread_Join()
{
	HANDLE h = (HANDLE)handle;//get the thread handle
	if(h)//if the handle exist
	{
		//Debug::println("Joining thread (handle=0x{0,x})...",(int)h);
		DWORD rv = WaitForSingleObject(h,INFINITE);//wait for thread
		if(WAIT_OBJECT_0 == rv)//if the thread returned
		{
			CloseHandle(h);//close the handle
			return 1;//return 0
		
		}
	}
	return 0;//if the thead didn't exist
}

// ----------------------------------------------------------------
//  Name:           Thread_Sleep
//	Class:			cThread
//  Description:    make the thread sleep for a number of milliseconds
//  Arguments:      int millis
//  Return Value:   int
// ----------------------------------------------------------------
int cThread::Thread_Sleep(int millis)
{
	Sleep(millis);
	return 0;
}
