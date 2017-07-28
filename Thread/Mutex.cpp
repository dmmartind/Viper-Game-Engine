//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Mutex Class Code 
//
//****************************************************************************************

#include "Mutex.h"


// ----------------------------------------------------------------
//  Name:           cMutexImpl
//	Class:			cMutexImpl
//  Description:    constructor to initializes a critical section object
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------
cMutexImpl::cMutexImpl()
{
	InitializeCriticalSection(&obj);

}

// ----------------------------------------------------------------
//  Name:           ~cMutexImpl
//	Class:			cMutexImpl
//  Description:    deconstructor to delete a critical section object
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------

cMutexImpl::~cMutexImpl()
{
	
	DeleteCriticalSection(&obj);


}

// ----------------------------------------------------------------
//  Name:           lock
//	Class:			cMutexImpl
//  Description:    enter critical section object
//  Arguments:      None
//  Return Value:   void
// ----------------------------------------------------------------

void cMutexImpl::lock()
{
	EnterCriticalSection(&obj);



}

// ----------------------------------------------------------------
//  Name:           unlock
//	Class:			cMutexImpl
//  Description:    leave critical section object
//  Arguments:      None
//  Return Value:	void
// ----------------------------------------------------------------

void cMutexImpl::unlock()
{
	LeaveCriticalSection(&obj);
	
}

// ----------------------------------------------------------------
//  Name:           cMutex
//	Class:			cMutex
//  Description:    constructor for new critical section class
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------

cMutex::cMutex()
{

	m_this = new cMutexImpl;
	
}

// ----------------------------------------------------------------
//  Name:           ~cMutex
//	Class:			cMutex
//  Description:    deconstructor to delete critical section
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------

cMutex::~cMutex()
{
	
delete m_this;
}

// ----------------------------------------------------------------
//  Name:           lock
//	Class:			cMutex
//  Description:    enable the lock flag for a critical section to keep out other threads
//  Arguments:      None
//  Return Value:   void
// ----------------------------------------------------------------

void cMutex::lock()
{

m_this->lock();

}

// ----------------------------------------------------------------
//  Name:           unlock
//	Class:			cMutex
//  Description:    disable the lock flag for a critical section to open it out for another thread
//  Arguments:      None
//  Return Value:   void
// ----------------------------------------------------------------

void cMutex::unlock()
{
	
m_this->unlock();


}

// ----------------------------------------------------------------
//  Name:           incrementRC
//	Class:			cMutex
//  Description:    increament a protected var
//  Arguments:      long *value
//  Return Value:   void
// ----------------------------------------------------------------

void cMutex::incrementRC(long *value)
{
	InterlockedIncrement(value);//safely increment shared var 

}

// ----------------------------------------------------------------
//  Name:           decrementRC
//	Class:			cMutex
//  Description:    decrement a protected var
//  Arguments:      long *value
//  Return Value:   long
// ----------------------------------------------------------------

long cMutex::decrementRC(long *value)
{
	return InterlockedDecrement(value);//safely decrement shared var

}

// ----------------------------------------------------------------
//  Name:           test_and_set
//	Class:			cMutex
//  Description:    assigns value the a protected var
//  Arguments:      long* value, long newValue
//  Return Value:   long
// ----------------------------------------------------------------

long cMutex::test_and_set(long* value, long newValue)
{
	return InterlockedExchange(value,newValue);//sets value to a shared var
}

