//========= Copyright © 2009, David Martin, All rights reserved. ============
//
//Purpose: Entity Class Code
//
//=============================================================================
#include "Entity.h"

namespace Viper2D
{

// ----------------------------------------------------------------
//  Class:			cBaseEntity
//  Description:    constructor
//  Arguments:      enum RenderType renderType.
//  Return Value:   None.
// ----------------------------------------------------------------
cBaseEntity::cBaseEntity(enum RenderType renderType)
{
	
		this->renderType = renderType;
		this->SetModelIndex(-1);
		this->SetModelName("");
		this->setVisible(false);
		this->setAlive(false);
		this->setCollidable(false);
		this->objectType = ISBASEOBJECT;
		this->lifeLength = 0;
		this->lifeTimer.reset();
	

	
}

// ----------------------------------------------------------------
//  Class:			~cBaseEntity
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
cBaseEntity::~cBaseEntity()
{

}

// ----------------------------------------------------------------
//  Name:			GetModelIndex
//	Class:			cBaseEntity
//  Description:    Get model index for this entity
//  Arguments:      None.
//  Return Value:   INT.
// ----------------------------------------------------------------
int cBaseEntity::GetModelIndex()
{
	return m_nModelIndex;
}

// ----------------------------------------------------------------
//	Name:			SetModelIndex
//  Class:			cBaseEntity
//  Description:    set the model index for the current node
//  Arguments:      int index
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::SetModelIndex( int index )
{
	m_nModelIndex = index;
	
}

// ----------------------------------------------------------------
//	Name:			SetModelName
//  Class:			cBaseEntity
//  Description:    set the model name for the current node
//  Arguments:      std::string name
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::SetModelName(std::string name )
{
	m_ModelName = name;
}

// ----------------------------------------------------------------
//  Name:           GetModelName
//  Class:			cBaseEntity
//  Description:    get the model name of the current node
//  Arguments:      None.
//  Return Value:   std::string
// ----------------------------------------------------------------
std::string cBaseEntity::GetModelName()
{
	return m_ModelName;
}


// ----------------------------------------------------------------
//	Name:           setObjectType
//  Class:			cBaseEntity
//  Description:    set the object type of the current node
//  Arguments:      None.
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::setObjectType(ObjectType input)
{
	objectType=input;
}

// ----------------------------------------------------------------
//  Name:			getObjectType
//  Class:			cBaseEntity
//  Description:    get the object type of the current node
//  Arguments:      None.
//  Return Value:   ObjectType
// ----------------------------------------------------------------
ObjectType cBaseEntity::getObjectType()
{

return objectType;

}

// ----------------------------------------------------------------
//	Name:           lifetimeExpired
//  Class:			cBaseEntity
//  Description:    find out whether or not an node has expired
//  Arguments:      None.
//  Return Value:   bool
// ----------------------------------------------------------------
bool cBaseEntity::lifetimeExpired()
{

	return lifeTimer.stopwatch(getLifeExpect());

}

// ----------------------------------------------------------------
//  Name:			getLifeExpect
//  Class:			cBaseEntity
//  Description:    get the length of the life of a node
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
int cBaseEntity::getLifeExpect()
{

	return lifeLength;

}

// ----------------------------------------------------------------
//  Name:			getRenderType
//  Class:			cBaseEntity
//  Description:    get the render type of a node(3D or 2D)
//  Arguments:      None.
//  Return Value:   RenderType
// ----------------------------------------------------------------

RenderType cBaseEntity::getRenderType()
{

	return renderType;


}
// ----------------------------------------------------------------
//	Name:           setRenderType
//  Class:			cBaseEntity
//  Description:    set the render type of a node (3D or 2D)
//  Arguments:      RenderType input
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::setRenderType(RenderType input)
{

	renderType=input;



}

// ----------------------------------------------------------------
//	Name:			GetHealth
//  Class:			cBaseEntity
//  Description:    return the health of an node
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
int	cBaseEntity::GetHealth()
{

	return iHealth;

}

// ----------------------------------------------------------------
//	Name:			GetMaxHealth
//  Class:			cBaseEntity
//  Description:    get he maximum health of an node
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
int	cBaseEntity::GetMaxHealth()
{

	return m_iHealth;
}


// ----------------------------------------------------------------
//	Name:			setHealth
//  Class:			cBaseEntity
//  Description:    set the current health of a node
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::setHealth(int input)
{

	iHealth=input;

}

// ----------------------------------------------------------------
//	Name:			setMaxHealth
//  Class:			cBaseEntity
//  Description:    set the maximum health for an node
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::setMaxHealth(int input)
{

	m_iHealth=input;
}

// ----------------------------------------------------------------
//  Name:			GetShield
//  Class:			cBaseEntity
//  Description:    Get the shield of an node
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
int cBaseEntity::GetShield()
{

	return isheild;
	


}

// ----------------------------------------------------------------
//	Name:			GetMaxShield
//  Class:			cBaseEntity
//  Description:    get the max shield for an node
//  Arguments:      None.
//  Return Value:   INT
// ----------------------------------------------------------------
int cBaseEntity::GetMaxShield()
{

	return m_isheild;


}

// ----------------------------------------------------------------
//	Name:			setShield
//  Class:			cBaseEntity
//  Description:    set the shield of an node
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::setShield(int input)
{

	isheild=input;
	


}

// ----------------------------------------------------------------
//	Name:			setMaxShield
//  Class:			cBaseEntity
//  Description:    set the max shieled for an node
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::setMaxShield(int input)
{

	m_isheild = input;


}

// ----------------------------------------------------------------
//	Name:			setLifeExpect
//  Class:			cBaseEntity
//  Description:    set new life length and reset the timer
//  Arguments:      int input
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::setLifeExpect(int input)
{

	lifeLength = input;
	lifeTimer.reset();


}


// ----------------------------------------------------------------
//	Name:			setAlive
//  Class:			cBaseEntity
//  Description:    set the alive flag for the node
//  Arguments:      bool input
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::setAlive(bool input)
{

	this->alive=input;


}



// ----------------------------------------------------------------
//	Name:			IsAlive
//  Class:			cBaseEntity
//  Description:    get the T/F on whether a node is alive
//  Arguments:      None.
//  Return Value:   BOOL
// ----------------------------------------------------------------
bool cBaseEntity::IsAlive()
{

return alive;

}


// ----------------------------------------------------------------
//	Name:			IsVisible
//  Class:			cBaseEntity
//  Description:    get the visible flag for a node
//  Arguments:      None.
//  Return Value:   BOOL
// ----------------------------------------------------------------
bool cBaseEntity::IsVisible()
{


return visible;


}



// ----------------------------------------------------------------
//  Name:			setVisible
//  Class:			cBaseEntity
//  Description:    set visible flag
//  Arguments:      bool input
//  Return Value:   VOID
// ----------------------------------------------------------------
void cBaseEntity::setVisible(bool input)
{

visible=input;


}


// ----------------------------------------------------------------
//	Name:			setCollidable
//  Class:			cBaseEntity
//  Description:    set whether or not the node is collidable
//  Arguments:      bool value
//  Return Value:   void
// ----------------------------------------------------------------
void cBaseEntity::setCollidable(bool value)
{

 isCollidable=value;


}

// ----------------------------------------------------------------
//	Name:			checkCollidable
//  Class:			cBaseEntity
//  Description:    get whether a node is collidable
//  Arguments:      None.
//  Return Value:   BOOL.
// ----------------------------------------------------------------
bool cBaseEntity::checkCollidable()
{


return isCollidable;

}


};