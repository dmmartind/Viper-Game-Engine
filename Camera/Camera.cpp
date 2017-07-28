//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: Camera Class Code

//****************************************************************************************

#ifndef _CAMERA_CPP_//define one time and let everyone share, so VS doesn't give annoying errors
#define _CAMERA_CPP_//define one time and let everyone share, so VS doesn't give annoying errors

#include "Camera.h"

namespace Viper2D
{
	// ----------------------------------------------------------------
//  Name:           Camera
//	Class:			Camera
//  Description:    class constructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------

	Camera::Camera(void)
	{
		p_position=D3DXVECTOR3(0.0f,0.0f,10.0f);//set the vector used for the camera poision cordinates
		p_updir=D3DXVECTOR3(0.0f,1.0f,0.0f);//set the vector used for the camera rotaion cordinates
		float ratio=640/480;//screen ratio
		setPerspective(3.14159f/4,ratio,1.0f,2000.0f);

	}

	// ----------------------------------------------------------------
//  Name:           Camera
//	Class:			Camera
//  Description:    class destructor
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
	Camera::~Camera(void) {}


	// ----------------------------------------------------------------
//  Name:           setPerspective
//	Class:			Camera
//  Description:    set the projection matrix
//  Arguments:      //fov::field of view::width of the projected view, so the higher the number,
					//the more you see
					//aspect ratio: size of the viewing screen
					//near range: clipping plane for objects near the camera
					//far range: clipping plane for objects at the maxmimum range of the camera
//  Return Value:   None.
	
// ----------------------------------------------------------------
	
	void Camera::setPerspective(float fov, float aspectRatio, float nearRange, float farRange)
	{

		this->setFOV(fov);
		this->setAspectRatio(aspectRatio);
		this->setNearRange(nearRange);
		this->setFarRange(farRange);

	}


	// ----------------------------------------------------------------
//  Name:           Update
//	Class:			Camera
//  Description:    set the projection matrix
//  Arguments:      //fov::field of view::width of the projected view, so the higher the number,
					//the more you see
					//aspect ratio: size of the viewing screen
					//near range: clipping plane for objects near the camera
					//far range: clipping plane for objects at the maxmimum range of the camera
//  Return Value:   None.
	
// ----------------------------------------------------------------
	void Camera::Update()
	{
		//set the camera's perspective matrix (clipping regeon)
		D3DXMatrixPerspectiveFovLH(&this->p_matrixProj, this->p_fov, this->p_aspectRatio, this->p_nearRange, this->p_farRange);
		g_engine->getDevice()->SetTransform(D3DTS_PROJECTION, &this->p_matrixProj);
		//set the camera's view matrix
		D3DXMatrixLookAtLH(&this->p_matrixView, &this->p_position, &this->p_target, &this->p_updir);
		g_engine->getDevice()->SetTransform(D3DTS_VIEW,&this->p_matrixView);
	}

	// ----------------------------------------------------------------
//  Name:           setPosition
//	Class:			Camera
//  Description:    set the position of the camera
//  Arguments:      //x,y,z
//  Return Value:   None.
	
// ----------------------------------------------------------------
	void Camera::setPosition(float x, float y, float z)
	{
		this->p_position.x=x;
		this->p_position.y=y;
		this->p_position.z=z;
	}


	// ----------------------------------------------------------------
//  Name:           setPosition
//	Class:			Camera
//  Description:    set the position of the camera
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------


	void Camera::setPosition(D3DXVECTOR3 position)
	{
		this->setPosition(position.x, position.y, position.z);

	}

		// ----------------------------------------------------------------
//  Name:           getNearRange
//	Class:			Camera
//  Description:    //get the near clipping range
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------



	float Camera::getNearRange() 
	{
		return p_nearRange;
	}

		// ----------------------------------------------------------------
//  Name:           setNearRange
//	Class:			Camera
//  Description:    //set the near clipping range
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------
	
	float Camera::setNearRange(float value) 
	{
		return p_nearRange = value;
	}

		// ----------------------------------------------------------------
//  Name:           getFarRange
//	Class:			Camera
//  Description:   //get the far clipping range
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::getFarRange()
	{
		return p_farRange;
	}

		// ----------------------------------------------------------------
//  Name:           setFarRange
//	Class:			Camera
//  Description:    //set the far clipping range
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::setFarRange(float value) 
	{
		return p_farRange = value;
	}

		// ----------------------------------------------------------------
//  Name:           getAspectRatio
//	Class:			Camera
//  Description:    //get the aspect ratio
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::getAspectRatio() 
	{
		return p_aspectRatio;
	}

		// ----------------------------------------------------------------
//  Name:           setAspectRatio
//	Class:			Camera
//  Description:   //set the aspect ratio
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::setAspectRatio(float value) 
	{
		return p_aspectRatio = value;
	}

		// ----------------------------------------------------------------
//  Name:           getFOV
//	Class:			Camera
//  Description:    //get the field of view
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::getFOV() 
	{
		return p_fov;
	}

		// ----------------------------------------------------------------
//  Name:           setFOV
//	Class:			Camera
//  Description:    //set the field of view
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	void Camera::setFOV(float value)
	{
		p_fov = value;
	}

		// ----------------------------------------------------------------
//  Name:           getPosition
//	Class:			Camera
//  Description:    //get the position of the camera
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	D3DXVECTOR3 Camera::getPosition()
	{
		return p_position;
	}

		// ----------------------------------------------------------------
//  Name:           getX
//	Class:			Camera
//  Description:    //get the position x cordinate
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::getX() 
	{
		return p_position.x;
	}

		// ----------------------------------------------------------------
//  Name:           setX
//	Class:			Camera
//  Description:    //set the position x cordinate
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::setX(float value) 
	{
		return p_position.x = value;
	}

		// ----------------------------------------------------------------
//  Name:           getY
//	Class:			Camera
//  Description:    //get the position y cordinate
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::getY() 
	{
		return p_position.y;
	}

		// ----------------------------------------------------------------
//  Name:           setY
//	Class:			Camera
//  Description:    //set the position y cordinate
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::setY(float value) 
	{
		return p_position.y = value;
	}

		// ----------------------------------------------------------------
//  Name:           getZ
//	Class:			Camera
//  Description:    //get the position z cordinate
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::getZ() 
	{
		return p_position.z;
	}

		// ----------------------------------------------------------------
//  Name:           setZ
//	Class:			Camera
//  Description:    //set the z cordinate
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	float Camera::setZ(float value) 
	{
		return p_position.z = value;
	}

		// ----------------------------------------------------------------
//  Name:           getTarget
//	Class:			Camera
//  Description:    //get the target of the camera
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------


	
	D3DVECTOR Camera::getTarget()
	{
		return p_target;
	}

			// ----------------------------------------------------------------
//  Name:           setTarget
//	Class:			Camera
//  Description:    //set the target of the camera
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	void Camera::setTarget(D3DXVECTOR3 value) 
	{
		p_target=value;
	}

			// ----------------------------------------------------------------
//  Name:           setTarget
//	Class:			Camera
//  Description:    //set the target of the camera
//  Arguments:      //vector
//  Return Value:   None.
	
// ----------------------------------------------------------------

	void Camera::setTarget(float x,float y,float z) 
	{
		p_target.x=x; p_target.y=y; p_target.z=z;
	}



};

#endif
