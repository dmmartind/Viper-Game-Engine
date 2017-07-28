//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: Vector Framework 
//
//****************************************************************************************




#include "vector.h"

namespace Viper2D
{

// ----------------------------------------------------------------
//  Name:           _vector
//	Class:			_vector
//  Description:    constructor
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------


_vector::_vector()
{
	x=0.0f;
	y=0.0f;
	z=0.0f;
	rotation=0;
}












// ----------------------------------------------------------------
//  Name:           _vector
//	Class:			_vector
//  Description:    constructor
//  Arguments:      float input1, float input2, float input3
//  Return Value:   None.
// ----------------------------------------------------------------


_vector::_vector(float input1, float input2, float input3)
{
	x=input1;
	y=input2;
	z=input3;
}

// ----------------------------------------------------------------
//  Name:           ~_vector
//	Class:			_vector
//  Description:    deconstructor
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------

_vector::~_vector()
{
	x=y=z=0;
}

// ----------------------------------------------------------------
//  Name:           getx
//	Class:			_vector
//  Description:    returns the x cordinate
//  Arguments:      None
//  Return Value:   float
// ----------------------------------------------------------------

float _vector::getx()
{
	return x;
}

// ----------------------------------------------------------------
//  Name:           gety
//	Class:			_vector
//  Description:    get the y value of the current 
//  Arguments:      None
//  Return Value:   float
// ----------------------------------------------------------------

float _vector::gety()
{
	return y;
}

// ----------------------------------------------------------------
//  Name:           getz
//	Class:			_vector
//  Description:    get the z value of the current cordinate
//  Arguments:      None
//  Return Value:   float
// ----------------------------------------------------------------

float _vector::getz()
{
	return z;
}

// ----------------------------------------------------------------
//  Name:           setx
//	Class:			_vector
//  Description:    inject value for the x cordinate
//  Arguments:      float value
//  Return Value:   void
// ----------------------------------------------------------------

void _vector::setx(float value)
{
	x=value;
}

// ----------------------------------------------------------------
//  Name:           sety
//	Class:			_vector
//  Description:    inject value for the y-cordinate
//  Arguments:		float value
//  Return Value:   void
// ----------------------------------------------------------------

void _vector::sety(float value)
{

	y=value;
}

// ----------------------------------------------------------------
//  Name:           setz
//	Class:			_vector
//  Description:    inject value for the z-cordinate
//  Arguments:      float value
//  Return Value:   void
// ----------------------------------------------------------------

void _vector::setz(float value)
{

	z=value;
}

// ----------------------------------------------------------------
//  Name:           distance
//	Class:			_vector
//  Description:    vector distance operation
//  Arguments:      _vector second
//  Return Value:   float
// ----------------------------------------------------------------

float _vector::distance(_vector second)
{
	return sqrt(pow((second.x-x),2)+pow((second.y-y),2)+ pow((second.z-z),2));
}

// ----------------------------------------------------------------
//  Name:           length
//	Class:			_vector
//  Description:    vector length operation
//  Arguments:      None
//  Return Value:   float
// ----------------------------------------------------------------

float _vector::length()
{
	return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}


// ----------------------------------------------------------------
//  Name:           move
//	Class:			_vector
//  Description:    add or subtract values as a position move
//  Arguments:      float input1,float input2, float input3
//  Return Value:   void
// ----------------------------------------------------------------

void _vector::move(float input1,float input2, float input3)
{
	x+=input1;
	y+=input2;
	z+=input3;
}

// ----------------------------------------------------------------
//  Name:           operator=
//	Class:			_vector
//  Description:    vector copy operation
//  Arguments:      _vector input
//  Return Value:   void
// ----------------------------------------------------------------



void _vector::operator=(_vector input)
{
	x=input.x;
	y=input.y;
	z=input.z;
}

// ----------------------------------------------------------------
//  Name:           dot_product
//	Class:			_vector
//  Description:    vector dot product operation
//  Arguments:      _vector input
//  Return Value:   float
// ----------------------------------------------------------------

float _vector::dot_product(_vector input)
{

	return (x*input.x + y*input.y + z*input.z);
}


// ----------------------------------------------------------------
//  Name:           cross_product
//	Class:			_vector
//  Description:    vector cross product operation
//  Arguments:      _vector input
//  Return Value:   _vector
// ----------------------------------------------------------------

_vector _vector::cross_product(_vector input)
{
	_vector temp;
	temp.x=y*input.z-z*input.y;
	temp.y=z*input.x-x*input.z;
	temp.z=x*input.y-y*input.x;

	return temp;
}


// ----------------------------------------------------------------
//  Name:           normal
//	Class:			_vector
//  Description:    vector normal operation
//  Arguments:      None
//  Return Value:   _vector
// ----------------------------------------------------------------

_vector _vector::normal()
{
	_vector temp;
	temp.x = x/(sqrt(pow(x,2)+pow(y,2)+pow(z,2)));
	temp.y = y/(sqrt(pow(x,2)+pow(y,2)+pow(z,2)));
	temp.z = z/(sqrt(pow(x,2)+pow(y,2)+pow(z,2)));
	return temp;
}

// ----------------------------------------------------------------
//  Name:           set
//	Class:			_vector
//  Description:    set operation to inject new values into the vector
//  Arguments:      float input1, float input2, float input3
//  Return Value:   void
// ----------------------------------------------------------------

void _vector::set(float input1, float input2, float input3)
{
	x=input1;
	y=input2;
	z=input3;
}

// ----------------------------------------------------------------
//  Name:           operator+
//	Class:			_vector
//  Description:    vector addition operation
//  Arguments:      _vector input
//  Return Value:   _vector
// ----------------------------------------------------------------


_vector _vector::operator+(_vector input)
{
	_vector temp;
	temp.x=x + input.x;
	temp.y=y + input.y;
	temp.z=z + input.z;
	return temp;
}


// ----------------------------------------------------------------
//  Name:           operator+=
//	Class:			_vector
//  Description:    vector addition operation
//  Arguments:      _vector input
//  Return Value:   void
// ----------------------------------------------------------------


void _vector::operator+=(_vector input)
{
	x+=input.x;
	y+=input.y;
	z+=input.z;
}


// ----------------------------------------------------------------
//  Name:           operator-
//	Class:			_vector
//  Description:    vector subtraction operation
//  Arguments:      _vector input
//  Return Value:   _vector
// ----------------------------------------------------------------

_vector _vector::operator-(_vector input)
{
	_vector temp;
	temp.x=x - input.x;
	temp.y=y - input.y;
	temp.z=z - input.z;

	return temp;
}


// ----------------------------------------------------------------
//  Name:           operator-=
//	Class:			_vector
//  Description:    vector subtraction operation
//  Arguments:      _vector input
//  Return Value:   void
// ----------------------------------------------------------------

void _vector::operator-=(_vector input)
{
	x-=input.x;
	y-=input.y;
	z-=input.z;
}

// ----------------------------------------------------------------
//  Name:           operator*
//	Class:			_vector
//  Description:    vector multiplication operation
//  Arguments:      _vector input
//  Return Value:   _vector
// ----------------------------------------------------------------

_vector _vector::operator*(_vector input)
{
	_vector temp;
	temp.x=x * input.x;
	temp.y=y * input.y;
	temp.z=z * input.z;

	return temp;
}


// ----------------------------------------------------------------
//  Name:           operator*=
//	Class:			_vector
//  Description:    vector multiplication operation
//  Arguments:      N_vector input
//  Return Value:   void
// ----------------------------------------------------------------

void _vector::operator*=(_vector input)
{
	x*=input.x;
	y*=input.y;
	z*=input.z;
}



// ----------------------------------------------------------------
//  Name:			operator/
//	Class:			_vector
//  Description:    vector divide operation
//  Arguments:      _vector input
//  Return Value:   _vector
// ----------------------------------------------------------------

_vector _vector::operator/(_vector input)
{
	_vector temp;
	temp.x=x / input.x;
	temp.y=y / input.y;
	temp.z=z / input.z;

	return temp;
}


// ----------------------------------------------------------------
//  Name:			operator/=
//	Class:			_vector
//  Description:    vector divide operation
//  Arguments:      _vector input
//  Return Value:   void
// ----------------------------------------------------------------

void _vector::operator/=(_vector input)
{
	x/=input.x;
	y/=input.y;
	z/=input.z;
}


// ----------------------------------------------------------------
//  Name:           operator==
//	Class:			_vector
//  Description:	check to see if two vectors are equal
//  Arguments:      _vector input
//  Return Value:   bool
// ----------------------------------------------------------------

bool _vector::operator==(_vector input)
{
	if(x!=input.x)
		return false;
	if(y!=input.y)
		return false;
	if(z!=input.z)
		return false;
return true;
}




// ----------------------------------------------------------------
//  Name:           operator<<
//	Class:			_vector
//  Description:	check to see if two vectors are equal
//  Arguments:      vector object
//  Return Value:   void
// ----------------------------------------------------------------

//void _vector::operator<<(_vector input)
//{
	


// ----------------------------------------------------------------
//  Name:           setRotation
//	Class:			_vector
//  Description:	set the rotation
//  Arguments:      float input
//  Return Value:   void
// ----------------------------------------------------------------

void _vector::setRotation(float input)
{
	rotation = input;

}

// ----------------------------------------------------------------
//  Name:           getRotation
//	Class:			_vector
//  Description:	get the rotation
//  Arguments:      NONE
//  Return Value:   float
// ----------------------------------------------------------------

float _vector::getRotation()
{
	return rotation;

}

};