//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Rectangle Class Code 
//
//****************************************************************************************
#include "Rect.h"

namespace Viper2D
{

// ----------------------------------------------------------------
//  Name:           Rect
//	Class:			Rect
//  Description:    constructor to create rectangle from integer arguments
//  Arguments:      int left, int top, int right, int bottom
//  Return Value:   NONE
// ----------------------------------------------------------------	
	Rect::Rect(int left, int top, int right, int bottom)
	{
		this->setLeft((double)left);
		this->setTop((double)top);
		this->setRight((double)right);
		this->setBottom((double)bottom);
	}

// ----------------------------------------------------------------
//  Name:           Rect
//	Class:			Rect
//  Description:    constructor to create rectangle from double arguments
//  Arguments:      double left, double top, double right, double bottom
//  Return Value:   NONE
// ----------------------------------------------------------------	
	Rect::Rect(double left, double top, double right, double bottom)
	{
		this->setLeft(left);
		this->setTop(top);
		this->setRight(right);
		this->setBottom(bottom);
	}

// ----------------------------------------------------------------
//  Name:           getTop
//	Class:			Rect
//  Description:    get top of rectangle
//  Arguments:      NONE
//  Return Value:   double
// ----------------------------------------------------------------	
	double Rect::getTop()
	{
		return top;
	}

// ----------------------------------------------------------------
//  Name:           getLeft
//	Class:			Rect
//  Description:    get left of rectangle
//  Arguments:      NONE
//  Return Value:   double
// ----------------------------------------------------------------	
	double Rect::getLeft()
	{

		return left;


	}

// ----------------------------------------------------------------
//  Name:           getRight
//	Class:			Rect
//  Description:    get right of rectangle
//  Arguments:      NONE
//  Return Value:   double
// ----------------------------------------------------------------	
	double Rect::getRight()
	{

		return right;

	}

// ----------------------------------------------------------------
//  Name:           getBottom
//	Class:			Rect
//  Description:    get bottom of rectangle
//  Arguments:      NONE
//  Return Value:   double
// ----------------------------------------------------------------	
	double Rect::getBottom()
	{

		return bottom;

	}

// ----------------------------------------------------------------
//  Name:           getWidth
//	Class:			Rect
//  Description:    get width of rectangle
//  Arguments:      NONE
//  Return Value:   double
// ----------------------------------------------------------------	
	double Rect::getWidth()
	{

		return right-left;


	}

// ----------------------------------------------------------------
//  Name:           getHeight
//	Class:			Rect
//  Description:    get height of rectangle
//  Arguments:      NONE
//  Return Value:  double
// ----------------------------------------------------------------	
	double Rect::getHeight()
	{

		return bottom-top;


	}

// ----------------------------------------------------------------
//  Name:           setTop
//	Class:			Rect
//  Description:    set top of rectangle
//  Arguments:      double input
//  Return Value:   void
// ----------------------------------------------------------------	
	void Rect::setTop(double input)
	{

		top=input;


	}

// ----------------------------------------------------------------
//  Name:           setLeft
//	Class:			Rect
//  Description:    set left of rectangle
//  Arguments:      double input
//  Return Value:   void
// ----------------------------------------------------------------	

	void Rect::setLeft(double input)
	{
		left=input;



	}

// ----------------------------------------------------------------
//  Name:           setRight
//	Class:			Rect
//  Description:    set right of rectangle
//  Arguments:      double input
//  Return Value:   void
// ----------------------------------------------------------------	
	void Rect::setRight(double input)
	{

		right=input;

	}

// ----------------------------------------------------------------
//  Name:           setBottom
//	Class:			Rect
//  Description:    set bottom of rectangle
//  Arguments:      double input
//  Return Value:   void
// ----------------------------------------------------------------	
	void Rect::setBottom(double input)
	{

		bottom=input;

	}

// ----------------------------------------------------------------
//  Name:           isInside
//	Class:			Rect
//  Description:    cast vector to x,y cordinates for the arguments to the overlap calculation
//  Arguments:      _vector point
//  Return Value:   bool
// ----------------------------------------------------------------	
	bool Rect::isInside(_vector point)
	{
		return this->isInside(point.getx(), point.gety());

	}

// ----------------------------------------------------------------
//  Name:           isInside
//	Class:			Rect
//  Description:    cast integer to float for the arguments to the overlap calculation
//  Arguments:      int x, int y
//  Return Value:   bool
// ----------------------------------------------------------------	
	bool Rect::isInside(int x, int y)
	{
		return this->isInside((float)x,(float)y);


	}

// ----------------------------------------------------------------
//  Name:           isInside
//	Class:			Rect
//  Description:    overlap calculation for two object vectors
//  Arguments:      double x, double y
//  Return Value:   bool
// ----------------------------------------------------------------	
	bool Rect::isInside(double x, double y)
	{

		return(x > getLeft() && x < getRight() && y > getTop() && y < getBottom());

	}


};