//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Timer Class Code
//
//****************************************************************************************


#ifndef TIMER_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define TIMER_CPP_//define one time and let everyone share, so VS doesn't give annoying errors

#include "Timer.h"

// ----------------------------------------------------------------
//  Name:           Timer
//	Class:			Timer
//  Description:    class constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

Timer::Timer(void)
{
	timer_start = timeGetTime();// sets the starter var to the system time
	reset();//reset stopwatch_start var
}

// ----------------------------------------------------------------
//  Name:           Timer
//	Class:			Timer
//  Description:    class deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

Timer::~Timer(void)
{
}

// ----------------------------------------------------------------
//  Name:           getTimer
//	Class:			Timer
//  Description:    get current time
//  Arguments:      None.
//  Return Value:   DWORD.
// ----------------------------------------------------------------

DWORD Timer::getTimer()
{
	return (DWORD) (timeGetTime());//return the current time
}

// ----------------------------------------------------------------
//  Name:           getStartTimeMillis
//	Class:			Timer
//  Description:    elapse time
//  Arguments:      None.
//  Return Value:   DWORD
// ----------------------------------------------------------------

DWORD Timer::getStartTimeMillis()
{
	return (DWORD) (timeGetTime() - timer_start);
}

// ----------------------------------------------------------------
//  Name:           sleep
//	Class:			Timer
//  Description:    sleep function
//  Arguments:      ms.
//  Return Value:   None.
// ----------------------------------------------------------------


void Timer::sleep(int ms)
{
	DWORD start = getTimer(); //set start to the current time 
	while (start + ms > getTimer());//
}

// ----------------------------------------------------------------
//  Name:           reset
//	Class:			Timer
//  Description:    set the current time to the stopwatch_start
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

void Timer::reset()
{
	stopwatch_start = getTimer();//reset stopwatch start var to the current time.
}

// ----------------------------------------------------------------
//  Name:           stopwatch
//  Class:			Timer
//  Description:    stop watch at a certain amount of time and reset to current time
//  Arguments:      ms
//  Return Value:   true/false
// ----------------------------------------------------------------

bool Timer::stopwatch(int ms)
{
	if ( timeGetTime() > stopwatch_start + ms ) {//if the current time is greater than the starting time plus the time past
		stopwatch_start = getTimer();//reset the stopwatch_start var to the current time
		return true;
	}
	else return false;
}




#endif


