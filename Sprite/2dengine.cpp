//*********** (C) Copyright 2009 David Martin All rights reserved. **********
// Purpose: 2D Class Code
//
//**********************************************vk)****************************************

#include "2dengine.h"


namespace Viper2D
{
// ----------------------------------------------------------------
//  Name:           _2DEngine
//	Class:			_2DEngine
//  Description:    constructor to null everything
//  Arguments:      NONE
//  Return Value:   NONE
// ----------------------------------------------------------------	
	_2DEngine::_2DEngine():cBaseEntity(RENDER2D)
	{

	width=1;//texture width
	height=1;//texture height
	
	state=0;//texture state
	isLoaded=false;//texture isLoaded flag
	image=NULL;//image object
	setPosition(0.0f,0.0f);//set position to cordinate 0,0
	setVelocity(0.0f,0.0f);//set the velocity to x+0,y+0

	moveClock=16;//set the move clock to 16
		color=0xFFFFFFFF;//set the color to black
		scale_k=1.0F;//set the scale to 1
		rotation=0;//set the rotation to 0
		frameControl=1;//set the animation direction to 1
		totalFrames=1;//set the total frames to 1
		currentFrame=0;//set the current frame to 0
		frameTimer=0;//set the frame timer to 0
		columns=1;//set the number of columns to 0 (sprite sheet)
		direction=0;//set the direction of the texture
		movestart=0;//set the move start to 0
		moveAngle=0;//set the move angle to 0
		
		animstarty=0;//animation start y cordinate
		animstartx=0;//animation start x cordinate
		framestart=0;//set the frame start to 0
	

	}	

// ----------------------------------------------------------------
//  Name:           ~_2DEngine
//	Class:			_2DEngine
//  Description:    deconstructor
//  Arguments:      None.
//  Return Value:   NONE
// ----------------------------------------------------------------	
	_2DEngine::~_2DEngine()
	{
		Free();//
		Free_2DEngine_Vector();//free object list

		
		

		

	}	


// ----------------------------------------------------------------
//  Name:           Free
//	Class:			_2DEngine
//  Description:    free 
//  Arguments:      NONE
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::Free()
		{
			width=1;//set width
			height=1;
			
			state=0;

			collisionType=COLLISION_RECT;//set default collision type

		moveClock=16;//set the move clock to 16
		D3DCOLOR color=0xFFFFFFFF;//set the key color to black
		scale_k=1.0F;//set the scale to 1
		rotation=0;//set the rotation to 0
		frameControl=1;//set the animation direction to 1
		totalFrames=1;//set the total frames equal 1
		currentFrame=0;//set the current frame to 0
		frameTimer=0;//set the frame timer to 0
		columns=1;//set the coloumns to 1 (sprite sheet column)
		direction=0;//set direction to 0
		movestart=0;//set move start to 0
		moveAngle=0;//set move andgle to 0
		animstarty=0;//set the animation start y cordinate
		animstartx=0;//set the animation start x cordinate
		framestart=0;//set the frame start to 0
			

			
			
			 if (isLoaded)
			 {
				delete image;
				isLoaded=false;
				image=NULL;
			 }
				
		}

// ----------------------------------------------------------------
//  Name:           setSize
//	Class:			_2DEngine
//  Description:    set size of sprite
//  Arguments:      int width,int height
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setSize(int width,int height)
	{


		this->width=width;
		this->height=height;
	}


// ----------------------------------------------------------------
//  Name:           getWidth
//	Class:			_2DEngine
//  Description:    get width function
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	
	int _2DEngine::getWidth()
	{

		return this->width;



	}

// ----------------------------------------------------------------
//  Name:           getHeight
//	Class:			_2DEngine
//  Description:    get height function
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	
	int _2DEngine::getHeight()
	{

		return this->height;




	}

// ----------------------------------------------------------------
//  Name:           setWidth
//	Class:			_2DEngine
//  Description:    set width function
//  Arguments:      int width
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setWidth(int width)
	{



		this->width=width;


	}


// ----------------------------------------------------------------
//  Name:           setHeight
//	Class:			_2DEngine
//  Description:    set height function
//  Arguments:      int height
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setHeight(int height)
	{


		this->height=height;


	}

// ----------------------------------------------------------------
//  Name:           checkState
//	Class:			_2DEngine
//  Description:    get state of an object(not implemented yet)
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	
	int _2DEngine::checkState()
	{

		return this->state;



	}

// ----------------------------------------------------------------
//  Name:           setState
//	Class:			_2DEngine
//  Description:    set state of an object(not implemented yet)
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setState(int value)
	{


		this->state=value;



	}

// ----------------------------------------------------------------
//  Name:           loadImage
//	Class:			_2DEngine
//  Description:    load image with transparency key
//  Arguments:      char *filename, D3DCOLOR color
//  Return Value:   bool
// ----------------------------------------------------------------	
	bool _2DEngine::loadImage(char *filename, D3DCOLOR color)
	{
		if(isLoaded && image != NULL)//check to see if an image is loaded and image exist
		{
			if(image)//if image exist
				image->Release();	//release image	
			delete image;//delete image
		}
		image= new Texture();//create new allocation for texture
		if(image->Load(filename, color))//load picture with transparency key
		{
			this->setSize(image->getWidth(), image->getHeight());//if success then set the size to the image attributes for height and width
			isLoaded=true;//set that the image is loaded
			return true;//return success
		}
		else
			return false;//otherwise return false
	}


// ----------------------------------------------------------------
//  Name:           setImage
//	Class:			_2DEngine
//  Description:    set image to an object
//  Arguments:      Texture *image
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setImage(Texture *image)
	{
		this->image = image;//set the image to the texture
		this->setWidth(image->getWidth());//set the width attribute
		this->setHeight(image->getHeight());//set the height sttribute
		this->isLoaded = true;//set that the image is loaded
	}


// ----------------------------------------------------------------
//  Name:           transform
//	Class:			_2DEngine
//  Description:    2D tranformation function
//  Arguments:      None.
//  Return Value:   void
//	Note:			The rotation is only the angle and not the actual matrix. This will be implemented in a later release.
// ----------------------------------------------------------------	
	void _2DEngine::transform()
	{
		D3DXMATRIX outputMatrix;//create an output matrix
		D3DXVECTOR2 scaleMatrix((float)this->getScale(),(float)this->getScale());//create scale matrix
		D3DXVECTOR2 rotationCenterMatrix((float)(width * this->getScale())/2, (float)(height * this->getScale())/2);//matrix to hold the center cordinate of the sprite
		D3DXVECTOR2 translationMatrix((float)this->getx(),(float)this->gety());//translation matrix
		D3DXMatrixTransformation2D(&outputMatrix, NULL, 0, &scaleMatrix,&rotationCenterMatrix,(float)this->getRotation(),&translationMatrix);//translation matrix
		g_engine->getSpriteHandler()->SetTransform(&outputMatrix);//set transform matrix to the sprite handle
	}


// ----------------------------------------------------------------
//  Name:           draw
//	Class:			_2DEngine
//  Description:    function to draw sprite
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::draw()
	{
		int fx= (this->getCurrentFrame() % this->getColumns())*this->width;//get the x cordinate of the sprite sheet or texture
		int fy= (this->getCurrentFrame() / this->getColumns())*this->height;//get the y cordinate of the sprite sheet or texture
		RECT srcRect= {fx,fy,fx+this->width,fy+this->height};//create a bounding rectangle
		this->transform();//calculate the trasformation matrix
		g_engine->getSpriteHandler()->Draw(this->image->GetTexture(), &srcRect,NULL,NULL,this->getColor());//draw the sprite
	}


// ----------------------------------------------------------------
//  Name:           move
//	Class:			_2DEngine
//  Description:    sprite move function
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::move()
	{
		if (this->getMoveClock() > 0) {//for consistant movement
				
			if (timeGetTime() > (DWORD)(this->getMoveStart() + this->getMoveClock())) {//if the current time is  > (starting time + move clock)
                //reset move timer
				this->setMoveStart(timeGetTime());//set move start

                //move sprite by velocity amount
				this->setx(this->getx() + this->getVelocity().getx());
				this->sety(this->gety() + this->getVelocity().gety());
            }
        }
        else {//fast
            //move sprite by velocity amount
			this->setx(this->getx() + this->getVelocity().getx());
			this->sety(this->gety() + this->getVelocity().gety());
        }



	}



// ----------------------------------------------------------------
//  Name:           animate
//	Class:			_2DEngine
//  Description:    animation function using a sprite sheet
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::animate() 
    {
        //update frame based on animdir
		if (this->getFrameTimer() > 0) {//consistant animation// frame time is > 0
				if (timeGetTime() > (DWORD)(this->getFrameStart() + this->getFrameTimer()))  {//current time > (frame start time + frame time)
                //reset animation timer
				this->setFrameStart(timeGetTime());//set frame start to the current time
				this->setCurrentFrameplus(this->getFrameControl());//set the current frame + the frame control

                //keep frame within bounds on the sprite sheet
				if (this->getCurrentFrame() < 0)//if the current frame is < 0
					this->setCurrentFrame(this->getTotalFrames()-1);//set current frame to the end
				if (this->getCurrentFrame() > this->getTotalFrames()-1) this->setCurrentFrame(0);//if the current frame is > than the last frame,
																									//set the current frame to the beginning
            }
        }
        else {

            //no animation timer--update at cpu clock speed
			int cf=this->getCurrentFrame();//get current frame
			cf += this->getFrameControl();//current frame + the frame control
			this->setCurrentFrame(cf);//set the current frme
			if (this->getCurrentFrame() < 0) this->setCurrentFrame(this->getTotalFrames()-1);//if the current frame is less that 0,
																								//set the current frame to the end
            if (this->getCurrentFrame() > this->getTotalFrames()-1) this->setCurrentFrame(0);//if the current frame is > than the last frame,
																								//set the current frame to the first frame
        }
    }

// ----------------------------------------------------------------
//  Name:           Free_2DEngine_Vector
//	Class:			_2DEngine
//  Description:    free object structs
//  Arguments:      None.
//  Return Value:   void
// ----------------------------------------------------------------	

void _2DEngine::Free_2DEngine_Vector()
{

setPosition(0.0f,0.0f);
	setVelocity(0.0f,0.0f);
	ZeroMemory(&position,sizeof(position));
	ZeroMemory(&velocity,sizeof(velocity));
	
}


// ----------------------------------------------------------------
//  Name:           getPosition
//	Class:			_2DEngine
//  Description:    get position 
//  Arguments:      None.
//  Return Value:   _vector
// ----------------------------------------------------------------	
	_vector _2DEngine::getPosition()
	{

		return position;



	}



// ----------------------------------------------------------------
//  Name:           setPosition
//	Class:			_2DEngine
//  Description:    set the position using a vector
//  Arguments:      _vector position
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setPosition(_vector position)
	{


		this->position=position;

	}




// ----------------------------------------------------------------
//  Name:           setPosition
//	Class:			_2DEngine
//  Description:    set the position of a sprite 
//  Arguments:      double x, double y
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setPosition(double x, double y)
	{

		this->position.set((float)x,(float)y,0.0f);
		
	}





// ----------------------------------------------------------------
//  Name:           getx
//	Class:			_2DEngine
//  Description:    get the position x
//  Arguments:      None.
//  Return Value:   double
// ----------------------------------------------------------------	
	double _2DEngine::getx()
	{

		return this->position.getx();



	}



// ----------------------------------------------------------------
//  Name:           gety
//	Class:			_2DEngine
//  Description:    get the position y
//  Arguments:      None.
//  Return Value:   double
// ----------------------------------------------------------------	
	double _2DEngine::gety()
	{

		return this->position.gety();
		

	}

	
	
// ----------------------------------------------------------------
//  Name:           setx
//	Class:			_2DEngine
//  Description:    set position x
//  Arguments:      double x
//  Return Value:   void
// ----------------------------------------------------------------	
	
	void _2DEngine::setx(double x)
	{

		this->position.setx((float)x);


	}



// ----------------------------------------------------------------
//  Name:           sety
//	Class:			_2DEngine
//  Description:    set the position y
//  Arguments:      double y
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::sety(double y)
	{

		this->position.sety((float)y);



	}



// ----------------------------------------------------------------
//  Name:           getVelocity
//	Class:			_2DEngine
//  Description:    get velocity vector of a sprite
//  Arguments:      None.
//  Return Value:   _vector
// ----------------------------------------------------------------	
	
	_vector _2DEngine::getVelocity()
	{

		return velocity;

	}


// ----------------------------------------------------------------
//  Name:           setVelocity
//	Class:			_2DEngine
//  Description:    set velocity vector for a sprite
//  Arguments:      _vector vel
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setVelocity(_vector vel)
	{

		this->velocity=vel;
		


	}


// ----------------------------------------------------------------
//  Name:           setVelocity
//	Class:			_2DEngine
//  Description:    set velocity of a sprite
//  Arguments:      double x, double y
//  Return Value:   void
// ----------------------------------------------------------------	
	
	void _2DEngine::setVelocity(double x, double y)
	{

		this->velocity.set((float)x, (float)y, 0.0f);
		

	}



// ----------------------------------------------------------------
//  Name:           getCollision
//	Class:			_2DEngine
//  Description:    get collision type of an object
//  Arguments:      None.
//  Return Value:   Collision
// ----------------------------------------------------------------	
Collision _2DEngine::getCollision()
{
	return collisionType;
}


// ----------------------------------------------------------------
//  Name:           setCollision
//	Class:			_2DEngine
//  Description:    set collision type
//  Arguments:      Collision value
//  Return Value:   void
// ----------------------------------------------------------------	
void _2DEngine::setCollision(Collision value)
{
	collisionType=value;
	
}



// ----------------------------------------------------------------
//  Name:           setMoveClock
//	Class:			_2DEngine
//  Description:    set the move timer
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	
	


	void _2DEngine::setMoveClock(int value)
	{

		moveClock=value;

	}

// ----------------------------------------------------------------
//  Name:           getMoveClock
//	Class:			_2DEngine
//  Description:    get move timer
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	
	int _2DEngine::getMoveClock()
	{
		return moveClock;
	}


// ----------------------------------------------------------------
//  Name:           setColor
//	Class:			_2DEngine
//  Description:    set color key for transparency
//  Arguments:      D3DCOLOR color
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setColor(D3DCOLOR color)
	{
		this->color=color;//
	}


// ----------------------------------------------------------------
//  Name:           setScale
//	Class:			_2DEngine
//  Description:    set scale for a sprite
//  Arguments:      double value
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setScale(double value)
	{

		scale_k=value;

	}


// ----------------------------------------------------------------
//  Name:           getScale
//	Class:			_2DEngine
//  Description:    get scale of a sprite
//  Arguments:      None.
//  Return Value:   double
// ----------------------------------------------------------------	
	double _2DEngine::getScale()
	{
		return scale_k;

	}

// ----------------------------------------------------------------
//  Name:           setRotation
//	Class:			_2DEngine
//  Description:    set rotation matrix(not implemented correcly for this release)
//  Arguments:      double value
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setRotation(double value)
	{
		rotation=value;
	}

// ----------------------------------------------------------------
//  Name:          getRotation
//	Class:			_2DEngine
//  Description:    get rotation matrix(not implemented correcly for this release)
//  Arguments:      None.
//  Return Value:   double
// ----------------------------------------------------------------?????????????????/	

	double _2DEngine::getRotation()
	{
		return rotation;
	}

// ----------------------------------------------------------------
//  Name:           setFrameControl
//	Class:			_2DEngine
//  Description:    set animation direction on a sprite sheet
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setFrameControl(int value)
	{
		frameControl=value;
	}

// ----------------------------------------------------------------
//  Name:           getFrameControl
//	Class:			_2DEngine
//  Description:    get the animation direction on the sprite sheet
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	

	int _2DEngine::getFrameControl()
	{
		return frameControl;

	}

// ----------------------------------------------------------------
//  Name:           setTotalFrames
//	Class:			_2DEngine
//  Description:    set the total frames on a sprite sheet
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setTotalFrames(int value)
	{

		totalFrames=value;
	

	}

// ----------------------------------------------------------------
//  Name:           getTotalFrames
//	Class:			_2DEngine
//  Description:    get the total frames on the sprite sheet
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	

	int _2DEngine::getTotalFrames()
	{
		return totalFrames;
	}

// ----------------------------------------------------------------
//  Name:           setCurrentFrame
//	Class:			_2DEngine
//  Description:    set the current frame on a sprite sheet
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setCurrentFrame(int value)
	{
			currentFrame=value;
	}

// ----------------------------------------------------------------
//  Name:           setCurrentFrameplus
//	Class:			_2DEngine
//  Description:    set current frame to the current frame + the provided value on a sprite sheet
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setCurrentFrameplus(int value)
	{
			currentFrame+=value;
	}

// ----------------------------------------------------------------
//  Name:           getCurrentFrame
//	Class:			_2DEngine
//  Description:    get current frame on a sprite sheet
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	
	int _2DEngine::getCurrentFrame()
	{
		return currentFrame;
	}

// ----------------------------------------------------------------
//  Name:           setFrameTimer
//	Class:			_2DEngine
//  Description:    set frame timer for the animation
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setFrameTimer(int value)
	{
		frameTimer=value;

	}

// ----------------------------------------------------------------
//  Name:           getFrameTimer
//	Class:			_2DEngine
//  Description:    get frame timer for a animation from a sprite sheet
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	

	int _2DEngine::getFrameTimer()
	{

		return frameTimer;

	}

// ----------------------------------------------------------------
//  Name:           setColumns
//	Class:			_2DEngine
//  Description:    set the column for a sprite sheet
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setColumns(int value)
	{

		columns=value;


	}

// ----------------------------------------------------------------
//  Name:           getColumns
//	Class:			_2DEngine
//  Description:    get columns for the sprite sheet
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	

	int _2DEngine::getColumns()
	{

		return columns;
	}

// ----------------------------------------------------------------
//  Name:           setDirection
//	Class:			_2DEngine
//  Description:    set direction for velocity calculations
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setDirection(int value)
	{

		direction=value;
	
	}

// ----------------------------------------------------------------
//  Name:           getDirection
//	Class:			_2DEngine
//  Description:    get direction for velocity calculations
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	

	int _2DEngine::getDirection()
	{
		return direction;

	}

// ----------------------------------------------------------------
//  Name:           getColor
//	Class:			_2DEngine
//  Description:    get the alpha key
//  Arguments:      None.
//  Return Value:   D3DCOLOR
// ----------------------------------------------------------------	


	D3DCOLOR _2DEngine::getColor()
	{

		return color;



	}

// ----------------------------------------------------------------
//  Name:           getMoveStart
//	Class:			_2DEngine
//  Description:    get move start time for the clock used for sprite movement
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	

	int _2DEngine::getMoveStart()
	{

		return movestart;
	}


// ----------------------------------------------------------------
//  Name:           setMoveStart
//	Class:			_2DEngine
//  Description:    set the move start time for the clock used for sprite movement
//  Arguments:      int time
//  Return Value:   void
// ----------------------------------------------------------------	
	void _2DEngine::setMoveStart(int time)
	{

		this->movestart=time;

	}

// ----------------------------------------------------------------
//  Name:           getFrameStart
//	Class:			_2DEngine
//  Description:    get the starting frame from the sprite sheet
//  Arguments:      None.
//  Return Value:   int
// ----------------------------------------------------------------	


	int  _2DEngine::getFrameStart()
	{
		return framestart;



	}

// ----------------------------------------------------------------
//  Name:           setFrameStart
//	Class:			_2DEngine
//  Description:    set the starting frame on the sprite sheet
//  Arguments:      int value
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setFrameStart(int value)
	{

		framestart=value;


	}

// ----------------------------------------------------------------
//  Name:           getAngle
//	Class:			_2DEngine
//  Description:    get the rotation angle for the rotation matrix
//  Arguments:      None.
//  Return Value:   float
// ----------------------------------------------------------------	

	float _2DEngine::getAngle()
	{
		
		return moveAngle;


	}

// ----------------------------------------------------------------
//  Name:           setAngle
//	Class:			_2DEngine
//  Description:    set angle for the rotation matrix(not impleamented in this release)
//  Arguments:      float input
//  Return Value:   void
// ----------------------------------------------------------------	

	void _2DEngine::setAngle(float input)
	{

		moveAngle=input;
		


	}










};
