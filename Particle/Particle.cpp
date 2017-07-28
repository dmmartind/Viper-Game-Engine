//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Particle Class Code 
//
//****************************************************************************************
#include "Particle.h"
#include <string.h>


namespace Viper2D
{
	// ----------------------------------------------------------------
//  Name:           Particle
//	Class:			Particle
//  Description:    constructor
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------
	Particle::Particle()
	{

		image=NULL;//null image object
		direction=0;//set particle direction 0
		length=100;//set particle length to 100
		max=100;//set max count of particles to 100
		alpha_min=254;//set alpha minimum 254
		alpha_max=255;//set alpha maximum 255
		minRed=0;//minimum red to 0
		minGreen=0;//set minimum green to 0
		minBlue=0;//set minimum blue to 0
		maxRed=255;//set the maximum red at 255
		maxGreen=255;//set the maximum green at 255
		maxBlue=255;//set the maximum blue at 255
		dither=10;//set particle dither to 10
		velocity=1.0f;//set particle velocity to 1
		scale=1.0f;//set particle scale to 1
		isLoaded=false;//set loaded to false

	}


// ----------------------------------------------------------------
//  Name:           ~Particle
//	Class:			Particle
//  Description:    deconstructor
//  Arguments:      None
//  Return Value:   None.
// ----------------------------------------------------------------

	Particle::~Particle()
	{
		

		Free();//free everything




	}

// ----------------------------------------------------------------
//  Name:           Free
//	Class:			Particle
//  Description:    free up all the particles,textures, and positions
//  Arguments:      None
//  Return Value:   Void
// ----------------------------------------------------------------


	void Particle::Free()
	{
		int counter=0;//set counter
		for(iter i = particles.begin(); i != particles.end(); ++i)//loop through the particle list
		{
			(*i)->Free();//free each node
			(*i) = NULL;//set the node to null
			counter++;//increment counter
			
		}
		for(int x=0; x<counter; x++)//loop through the particle max
		{
			particles.pop_back();//delete each node from the list
		}
		
		if(isLoaded && image != NULL)//check the image is loaded and the image exist
		{
			delete image;//destroy image
			isLoaded=false;//set isloaded flag to false
		}

		ZeroMemory(&position,sizeof(position));//zero memeory of the position vector
		
		
		
		direction=0;//set the particle direction to 0
		length=0;//set the particle length to 0
		max=0;//set the max particles to 0
		alpha_min=0;//set alpha minimum to 0
		alpha_max=0;//set alpha maximum to 0
		minRed=0;//set the minimum red to 0
		minGreen=0;//set the minimum green to 0
		minBlue=0;//set the minimum blue to 0
		maxRed=0;//set the maxiumum red to 0
		maxGreen=0;//set the maximum green to 0
		maxBlue=0;//set the maxnium blue to 0
		dither=0;//set the particle dither to 0
		velocity=0.0;//set the particle velocity to 0
		scale=0.0;//set the scale to 0



	}


// ----------------------------------------------------------------
//  Name:           loadImage
//	Class:			Particle
//  Description:    load image for base to create the particle from
//  Arguments:      char *filename
//  Return Value:   bool
// ----------------------------------------------------------------

	bool Particle::loadImage(char *filename)
	{
		image= new Texture();//set the texture image
		isLoaded=true;//set isloaded flag is true
		return image->Load(filename);//load file and return true, if successfull
		
	}


// ----------------------------------------------------------------
//  Name:           Add
//	Class:			Particle
//  Description:    create the particle after setting the image,position,velocity,and scale. Then set it to the particle vector
//  Arguments:      None
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::Add()
	{
		static double DEGREE=3.1415926535 / 180.0f;//degree definition
		double vx,vy;//create cordinates
		_2DEngine *p = new _2DEngine();//create new sprite
		p->setImage(image);//set image
		p->setPosition(position.getx(),position.gety());//set position of particle

		double dilute= (rand() % dither - dither/2) / 100.0f;//set some extra variation

		double dir= direction - 90.0;//set the direction of the particle
		vx = cos(dir * DEGREE) + dilute;//cos scaler x
		vy= sin(dir * DEGREE) + dilute;//sin scaler y
		p->setVelocity(vx * velocity, vy * velocity);//normal velocity vector
		
		int r= rand()%(maxRed-minRed)+minRed;//random shade of red
		int g= rand()%(maxGreen-minGreen)+minGreen;//random shade of green
		int b= rand()%(maxBlue-minBlue)+minBlue;//random shade of blue
		int a= rand()%(alpha_max-alpha_min)+alpha_min;//random alpha shade
		p->setColor(D3DCOLOR_RGBA(r,g,b,a));//set color of particle

		p->setScale(scale);//set scale of particle

		particles.push_back(p);//add to particle vector
	}


// ----------------------------------------------------------------
//  Name:           draw
//	Class:			Particle
//  Description:    draw the particle from every node
//  Arguments:      None
//  Return Value:   void
// ------------------------------------------------------------------

	void Particle::draw()
	{//int count=0;
		//char *buffer=(char*)malloc(80);
		for(iter i = particles.begin(); i != particles.end(); ++i)
		{
			(*i)->draw();//draw particle for each node
			//count++;
		}
		//_itoa_s(count,buffer,sizeof(buffer),2);
		//MessageBox(NULL,buffer,"TEST",MB_OK);
	}


// ----------------------------------------------------------------
//  Name:           update
//	Class:			Particle
//  Description:    add particles as long as there are particle left in the vector, move them, and set the new position
//  Arguments:      None
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::update()
	{
		static Timer timer;//timer object

		if((int)particles.size() < max)//if the vector size if < max
		{
			if(timer.stopwatch(1))//wait
				Add();//add particle
		}

		for(iter i = particles.begin(); i != particles.end(); ++i)//for each node
		{
			(*i)->move();//move
			if((*i)->getPosition().distance(this->getPosition()) > this->length)//set distance to the position as long as its greater than length
			{
				(*i)->setx(position.getx());//set cordinate x
				(*i)->sety(position.gety());//set cordinate y
			}
		}
	}


// ----------------------------------------------------------------
//  Name:           setAlphaRange
//	Class:			Particle
//  Description:    set the alpha levels
//  Arguments:      int min, int max
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::setAlphaRange(int min, int max)
	{
		alpha_min=min;//set the minimal alpha shade
		alpha_max=max;//set the maximal alpha shade
	}


// ----------------------------------------------------------------
//  Name:           setColorRange
//	Class:			Particle
//  Description:	set the color range for each color RBG
//  Arguments:      int red1, int green1, int blue1, int red2, int green2, int blue2
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::setColorRange(int red1, int green1, int blue1, int red2, int green2, int blue2)
	{
		minRed = red1;//set minimal shade of red
		minGreen = green1;//set minimal shade of green
		minBlue = blue1;//set minimal shade of blue
		maxRed = red2;//set maximum shade of red
		maxGreen = green2;//set maximum shade of green
		maxBlue = blue2;//set maximum shade of blue
	}


// ----------------------------------------------------------------
//  Name:           setPosition
//	Class:			Particle
//  Description:    set position of the particle
//  Arguments:      double x, double y
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::setPosition(double x, double y)
		{

			position.set((float)x,(float)y,0.0f);


		}


// ----------------------------------------------------------------
//  Name:           setPosition
//	Class:			Particle
//  Description:    set the position of the particle
//  Arguments:      _vector vec
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::setPosition(_vector vec)
		{

			position = vec;
		}



// ----------------------------------------------------------------
//  Name:           getPosition
//	Class:			Particle
//  Description:    get the position of the particle
//  Arguments:      None
//  Return Value:   _vector
// ----------------------------------------------------------------

	_vector Particle::getPosition()
		{

			
			return position;



		}



// ----------------------------------------------------------------
//  Name:           setDirection
//	Class:			Particle
//  Description:    set the particle direction
//  Arguments:      double angle
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::setDirection(double angle)
		{

			direction=angle;


		}
			
// ----------------------------------------------------------------
//  Name:           getDirection
//	Class:			Particle
//  Description:    get the direction of a particle
//  Arguments:      None
//  Return Value:   double
// ----------------------------------------------------------------
	
			
	double Particle::getDirection()
		{


			return direction;


		}


// ----------------------------------------------------------------
//  Name:           setMax
//	Class:			Particle
//  Description:    set the maximum amount of particles to use
//  Arguments:		int num
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::setMax(int num)
		{

			max = num;




		}


		


	// ----------------------------------------------------------------
//  Name:           setDither
//	Class:			Particle
//  Description:    set variation
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------
	void Particle::setDither(int value)
		{


			dither=value;


		}


// ----------------------------------------------------------------
//  Name:           setLength
//	Class:			Particle
//  Description:    set length of the particle
//  Arguments:      double value
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::setLength(double value)
		{
			length=value;
		}


// ----------------------------------------------------------------
//  Name:           setVelocity
//	Class:			Particle
//  Description:    set the velocity of the particle
//  Arguments:      double value
//  Return Value:   void
// ----------------------------------------------------------------

	void Particle::setVelocity(double value)
		{


			velocity=value;


		}
		

		
// ----------------------------------------------------------------
//  Name:           setScale
//	Class:			Particle
//  Description:    set scale 
//  Arguments:      double value
//  Return Value:   void
// ----------------------------------------------------------------

		void Particle::setScale(double value)
		{

			scale=value;

		}

};
