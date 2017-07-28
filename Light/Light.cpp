//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: Light Class Code

//****************************************************************************************

#ifndef _LIGHT_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define _LIGHT_CPP_//define one time and let everyone share, so VS doesn't give annoying errors

#include "Light.h"

namespace Viper2D
{

	// ----------------------------------------------------------------
//  Name:           Light
//	Class:			Light
//  Description:    class constructor
//  Arguments:      index, light type, position, direction of light, range of light
//  Return Value:   None.
// ----------------------------------------------------------------

	Light::Light(int lightNum, D3DLIGHTTYPE type, D3DXVECTOR3 position, D3DXVECTOR3 direction, double range)
	{
		this->p_lightNum= lightNum;//set light index number of the Light class
		ZeroMemory(&p_light, sizeof(D3DLIGHT9));//clear memeory for the light structure

		p_light.Diffuse.r=p_light.Ambient.r=1.0f;//set the object reflection and ambient light to 1 for red
		p_light.Diffuse.b=p_light.Ambient.b=1.0f;//set the object reflection and ambient light to 1 for blue
		p_light.Diffuse.g=p_light.Ambient.g=1.0f;//set the object reflection and ambient light to 1 for green
		p_light.Diffuse.a=p_light.Ambient.a=1.0f;//set the object reflection and ambient light to 1 for alpha

		switch(type)//find out what type of light is this.
		{
		case D3DLIGHT_POINT://light bulb or sun type light
			p_light.Type=D3DLIGHT_POINT;//set light type
			p_light.Position=position;//set the position of the light
			p_light.Attenuation0=0.1f;//set the light decrease rate to the maximum range
			p_light.Range=(float)range;//set the range of the light
			break;
		case D3DLIGHT_SPOT://stage spot light
			p_light.Type=D3DLIGHT_SPOT;//set the light type
			p_light.Position=position;//set the position of the light
			p_light.Direction=direction;//set the direction of the light
			p_light.Range=(float)range;//set the range of the light
			p_light.Theta=0.5f;//controls the diameter of the inner cone of the spot light
			p_light.Phi=1.0f;//controls the diameter of the outer cone of the spot light
			p_light.Falloff=1.0f;//set the light intensity variation between the cones
			p_light.Attenuation0=1.0f;//set the light decrease rate to the maximum range
			break;
		case D3DLIGHT_DIRECTIONAL://light like the sun
		default:
			p_light.Type=D3DLIGHT_DIRECTIONAL;//set the type
			p_light.Range=(float)range;//set the light range
			D3DXVec3Normalize((D3DXVECTOR3*)&p_light.Direction, &direction);//convert the cordinates to the value 
			//between 0 and 1
			break;

		}

	Show();//show the light
	Update();//update the current light settings

	}

	// ----------------------------------------------------------------
//  Name:           Light
//	Class:			Light
//  Description:    class deconstructor
//  Arguments:      void
//  Return Value:   None.
// ----------------------------------------------------------------
	Light::~Light(void)
	{
		
		Free();
	}

	void Light::Free()
	{
		ZeroMemory(&p_light,sizeof(p_light));
		ZeroMemory(&p_type,sizeof(p_type));
		
	}


	// ----------------------------------------------------------------
//  Name:           Update
//	Class:			Light
//  Description:    re-applies settings to a light
//  Arguments:      none
//  Return Value:   void
// ----------------------------------------------------------------

	void Light::Update()
	{
		g_engine->getDevice()->SetLight(p_lightNum, &p_light);//set the light with the new settings
	}

		// ----------------------------------------------------------------
//  Name:           Show
//	Class:			Light
//  Description:    turn the light on
//  Arguments:      none
//  Return Value:   void
// ----------------------------------------------------------------

	void Light::Show()//
	{
		g_engine->getDevice()->LightEnable(p_lightNum,true);//turn the light on
	}

		// ----------------------------------------------------------------
//  Name:           Hide
//	Class:			Light
//  Description:    //turn the light off
//  Arguments:      none
//  Return Value:   void
// ----------------------------------------------------------------

	void Light::Hide()
	{
		g_engine->getDevice()->LightEnable(p_lightNum,false);//turn the light off
	}

		// ----------------------------------------------------------------
//  Name:           setX
//	Class:			Light
//  Description:    set the x cordinate of the position
//  Arguments:      x cordinate
//  Return Value:   void
// ----------------------------------------------------------------

	void Light::setX(double value)
	{ 
		p_light.Position.x = (float)value;//set the x value of the position
	}


		// ----------------------------------------------------------------
//  Name:           getX
//	Class:			Light
//  Description:    get the x cordinate of the position
//  Arguments:      NONE
//  Return Value:   double
// ----------------------------------------------------------------

	double Light::getX() //get the x value of the position
	{ 
		return p_light.Position.x; 
	}

		// ----------------------------------------------------------------
//  Name:           setY
//	Class:			Light
//  Description:    set the y cordinate of the position of the light
//  Arguments:      y cordinate
//  Return Value:   void
// ----------------------------------------------------------------

	void Light::setY(double value) //set the y position of the 
	{ 
		p_light.Position.y = (float)value;
	}

		// ----------------------------------------------------------------
//  Name:           getY
//	Class:			Light
//  Description:    get the y cordinate of the position
//  Arguments:      NONE
//  Return Value:   double
// ----------------------------------------------------------------
	
	double Light::getY() 
	{ 
		return p_light.Position.y; 
	}

		// ----------------------------------------------------------------
//  Name:           setZ
//	Class:			Light
//  Description:    set the z cordinate of the light
//  Arguments:      z cordinate
//  Return Value:   void
// ----------------------------------------------------------------

	void Light::setZ(double value) 
	{ 
		p_light.Position.z = (float)value; 
	}

		// ----------------------------------------------------------------
//  Name:           getZ
//	Class:			Light
//  Description:    get the z cordinate of the light
//  Arguments:      none
//  Return Value:   double
// ----------------------------------------------------------------

	double Light::getZ() 
	{ 
		return p_light.Position.z; 
	}

		// ----------------------------------------------------------------
//  Name:           getType
//	Class:			Light
//  Description:    get the type of light
//  Arguments:      none
//  Return Value:   D3DLIGHTTYPE
// ----------------------------------------------------------------

	D3DLIGHTTYPE Light::getType() 
	{ 
		return p_type; 
	}

		// ----------------------------------------------------------------
//  Name:           setColor
//	Class:			Light
//  Description:    set the color of light
//  Arguments:      color vector
//  Return Value:   void
// ----------------------------------------------------------------

	void Light::setColor(D3DCOLORVALUE color) 
	{ 
		p_light.Diffuse = color; 
	}

		// ----------------------------------------------------------------
//  Name:           getColor
//	Class:			Light
//  Description:    set the diffuse or light reflection on an object
//  Arguments:      none
//  Return Value:   D3DCOLORVALUE
// ----------------------------------------------------------------

	D3DCOLORVALUE Light::getColor() 
	{
		return p_light.Diffuse; 
	}

		// ----------------------------------------------------------------
//  Name:           setDirection
//	Class:			Light
//  Description:    set the direction using the direction vector
//  Arguments:      direction vector
//  Return Value:   void
// ----------------------------------------------------------------

	void Light::setDirection(D3DXVECTOR3 direction) 
	{ 
		this->p_light.Direction = direction; 
	}

		// ----------------------------------------------------------------
//  Name:           setDirection
//	Class:			Light
//  Description:    set the vector with the x,y,z cordinates
//  Arguments:      x,y,z
//  Return Value:   void
// ----------------------------------------------------------------
	
	void Light::setDirection(double x,double y,double z) 
	{ 
		setDirection(D3DXVECTOR3((float)x,(float)y,(float)z)); 
	}

		// ----------------------------------------------------------------
//  Name:           getDirection
//	Class:			Light
//  Description:    get the direction vector
//  Arguments:      NONE
//  Return Value:   D3DXVECTOR3
// ----------------------------------------------------------------

	D3DXVECTOR3 Light::getDirection() 
	{ 
		return this->p_light.Direction; 
	}

		// ----------------------------------------------------------------
//  Name:           setPosition
//	Class:			Light
//  Description:    set the position vector for the light
//  Arguments:      position vector
//  Return Value:   void
// ----------------------------------------------------------------

	void Light::setPosition(D3DXVECTOR3 pos) 
	{ 
		p_light.Position = pos; 
	}

		// ----------------------------------------------------------------
//  Name:           setPosition
//	Class:			Light
//  Description:    set the position the light through x,y,z cordinates
//  Arguments:      X,Y,Z
//  Return Value:   void
// ----------------------------------------------------------------
	
	void Light::setPosition(double x,double y,double z) 
	{ 
		setPosition(D3DXVECTOR3((float)x,(float)y,(float)z)); 
	}

		// ----------------------------------------------------------------
//  Name:           getPosition
//	Class:			Light
//  Description:    get the position vector of the light
//  Arguments:      NONE
//  Return Value:   D3DXVECTOR3
// ----------------------------------------------------------------

	D3DXVECTOR3 Light::getPosition() 
	{ 
		return p_light.Position; 
	}


	template<class T> void Light::ReleaseCOM(T& x)
	{
		if(x)
		{
			x->Release();
			ZeroMemory(&x,sizeof(x));
			x = NULL;
		}
}

};






#endif
