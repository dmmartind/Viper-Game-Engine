//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: State Manager Class Code 
//
//****************************************************************************************
#include "StateMgr.h"


namespace Viper2D
{

// ----------------------------------------------------------------
//  Name:           eStateManager
//	Class:			eStateManager
//  Description:    constructor to clear the state list
//  Arguments:      None.
//  Return Value:   eStateManager
// ----------------------------------------------------------------	
	eStateManager::eStateManager()
	{
		state_list.clear();
	}
// ----------------------------------------------------------------
//  Name:           ~eStateManager
//	Class:			eStateManager
//  Description:    deconstructor for clearing the list, if its empty
//  Arguments:      None.
//  Return Value:   eStateManager
// ----------------------------------------------------------------
	eStateManager::~eStateManager()
	{
		if(state_list.empty() == false)
			state_list.clear();
	}
// ----------------------------------------------------------------
//  Name:           getState
//	Class:			eStateManager
//  Description:    return the current state
//  Arguments:      None.
//  Return Value:   cState
// ----------------------------------------------------------------
	cState eStateManager::getState()
	{
		return state_list.back();


	}
// ----------------------------------------------------------------
//  Name:           setState
//	Class:			eStateManager
//  Description:    set the current state
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------
	void eStateManager::setState(cState input)
	{

		state_list.push_back(input);

	}

// ----------------------------------------------------------------
//  Name:           popState
//	Class:			eStateManager
//  Description:    delete the current state
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
	void eStateManager::popState()
	{

		state_list.pop_back();
	}

};