//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Physics Class Code 
//
//****************************************************************************************
#include "..\Physics\Physics.h"


namespace Viper2D
{

// ----------------------------------------------------------------
//  Name:           cPhysicsObject
//	Class:			cPhysicsObject
//  Description:    constructor to set the collision count to 0
//  Arguments:      NONE
//  Return Value:   NONE
// ----------------------------------------------------------------	
	cPhysicsObject::cPhysicsObject()
	{
		collision_count=0;

	}

// ----------------------------------------------------------------
//  Name:           ~cPhysicsObject
//	Class:			cPhysicsObject
//  Description:    deconstructor
//  Arguments:      NONE
//  Return Value:   NONE
// ----------------------------------------------------------------	
	cPhysicsObject::~cPhysicsObject()
	{



	}

// ----------------------------------------------------------------
//  Name:           TestForCollisions
//	Class:			cPhysicsObject
//  Description:    test each entity against all the other entities for collision
//  Arguments:      NONE
//  Return Value:   void
// ----------------------------------------------------------------	
	void cPhysicsObject::TestForCollisions()
        {//0
            std::list<cBaseEntity*>::iterator first;//create first iterator
            std::list<cBaseEntity*>::iterator second;//create second iterator
			std::list<cBaseEntity*>::iterator end;//create end iterator
            _2DEngine *sprite1;//create sprite1 object
            _2DEngine *sprite2;//create sprite2 object
			std::list<cBaseEntity*> new_object;//create new list
			new_object= g_engine->getObject();//point list to engine list
			first = new_object.begin();//set the first to the beginning of the list
			end = new_object.end();//set the end to the end of the list
			end--;//decrement list
			while (first != end )//while the beginning is not the end
            {//1
                
                if ( (*first)->getRenderType() == RENDER2D )//if the object is 2D
                {//2
                    
                    sprite1 = (_2DEngine*) *first;//get the first sprite from the current node
                    
					if ( sprite1->IsAlive() && sprite1->IsVisible() && (sprite1->checkCollidable()) )//if sprite is alive, visible, and colliable
                    {//3
                        
                        second = new_object.begin();//set second iterator to the beginning of the list
                        while (second != end )//while the second list is not the end of the list
                        {//4
							
                            //point local sprite to sprite contained in the list
                            sprite2 = (_2DEngine*) *second;//get the sprite of the current node of the second list
                            
                            //]sprite2 is alive,visible,and colliable and sprite1 not equal sprite2 and object types are not equal
							if ( sprite2->IsAlive() && sprite2->IsVisible() && (sprite2->checkCollidable()) && (sprite1 != sprite2) && (sprite1->getObjectType() != sprite2->getObjectType()) )
                            {//5
                                //test for collision
                                if ( collision(first, second ) ) {//6
                                
                                    //notify game of collision
                                    game_entityCollision( first, second );
									//collision_count++;
                                }//6
    
                            }//5
                            //go to the next sprite in the list
                            second++;
                        }//4
            
                    } //3
                    //go to the next sprite in the list
                    first++;
                }//render2d//2
            } //while//1
        } //0

// ----------------------------------------------------------------
//  Name:           collision
//	Class:			cPhysicsObject
//  Description:    check waht type of collision
//  Arguments:      std::list<cBaseEntity*>::iterator test1,std::list<cBaseEntity*>::iterator test2
//  Return Value:   BOOL
// ----------------------------------------------------------------	
	bool cPhysicsObject::collision(std::list<cBaseEntity*>::iterator test1,std::list<cBaseEntity*>::iterator test2)
        {
            _2DEngine *sprite1;//create local sprite
            //_2DEngine *sprite2;

			sprite1 = (_2DEngine*) *test1;//set it to the first iterator
			//sprite2 = (_2DEngine*) *test2;
			
			
			switch (sprite1->getCollision()) {//get the collision of the sprite
                case COLLISION_RECT://if the bounding box collision
                    return collisionBR(test1,test2);
					break;
                case COLLISION_DIST://sphere collision
                    return collisionD(test1,test2);
                    break;
                case COLLISION_NONE://no collision
                default:
                    return false;
            }
   }


// ----------------------------------------------------------------
//  Name:           collisionD
//	Class:			cPhysicsObject
//  Description:    collision with the distance formula for sphere bounding
//  Arguments:      std::list<cBaseEntity*>::iterator test1,std::list<cBaseEntity*>::iterator test2
//  Return Value:   BOOL
// ----------------------------------------------------------------	
   bool cPhysicsObject::collisionD(std::list<cBaseEntity*>::iterator test1,std::list<cBaseEntity*>::iterator test2)
        {
             _2DEngine *sprite1;//create local sprite1
            _2DEngine *sprite2;//create local sprite2

			sprite1 = (_2DEngine*) *test1;//set sprite1 to the first argument
			sprite2 = (_2DEngine*) *test2;//set sprite2 to the second argument
			
			
			
			
			double radius1, radius2;//create radius1 and radius2

            //calculate radius 1
            if (sprite1->getWidth() > sprite1->getHeight())//if the width > height
                radius1 = (sprite1->getWidth()*sprite1->getScale())/2;//(width * scale)/2 for the radius
            else//if height >= width
                radius1 = (sprite1->getHeight()*sprite1->getScale())/2;//(height * scale)/2 for the radius

            //point = center of sprite 1
            double x1 = sprite1->getx() + radius1;//x cordinate + radius1
            double y1 = sprite1->gety() + radius1;//y cordinate + radius1
            _vector vector1((float)x1, (float)y1, (float)0.0);//create vector

            //calculate radius 2
            if (sprite2->getWidth() > sprite2->getHeight())//if the width > height
                radius2 = (sprite2->getWidth()*sprite2->getScale())/2;//(width * scale)/2 for the radius
            else
                radius2 = (sprite2->getHeight()*sprite2->getScale())/2;//(height * scale)/2 for the radius

            //point = center of sprite 2
            double x2 = sprite2->getx() + radius2;//x cordinate + radius1
            double y2 = sprite2->gety() + radius2;//y cordinate + radius1
            _vector vector2((float)x2, (float)y2, (float)0.0);//create vector

            //calculate distance
			double dist = vector1.distance( vector2 );//calculate the distance

            //return distance comparison
            return (dist < radius1 + radius2);//return dist < (r1 + r2)
        }



// ----------------------------------------------------------------
//  Name:           collisionD
//	Class:			cPhysicsObject
//  Description:    collision with the bounding box
//  Arguments:      std::list<cBaseEntity*>::iterator test1,std::list<cBaseEntity*>::iterator test2
//  Return Value:   BOOL
// ----------------------------------------------------------------	
   bool cPhysicsObject::collisionBR(std::list<cBaseEntity*>::iterator test1,std::list<cBaseEntity*>::iterator test2) 
        {
            bool ret = false;//set ret to false
			 _2DEngine *sprite1;//create sprite1
            _2DEngine *sprite2;//create sprite2

			sprite1 = (_2DEngine*) *test1;//set sprite1 to first argument
			sprite2 = (_2DEngine*) *test2;//set sprite2 to second argument


            Rect *ra = new Rect(//create rectangle with sprite 1 attributes
                sprite1->getx(),
                sprite1->gety(),
                sprite1->getx() + sprite1->getWidth() * sprite1->getScale(), 
                sprite1->gety() + sprite1->getHeight() * sprite1->getScale() );
            
            Rect *rb = new Rect(//create rectangle with sprite 1 attributes
                sprite2->getx(), 
                sprite2->gety(),
                sprite2->getx() + sprite2->getWidth() * sprite2->getScale(), 
                sprite2->gety() + sprite2->getHeight() * sprite2->getScale() );

            //looking for intersection
            if (ra->isInside( rb->getLeft(), rb->getTop() ) ||
                ra->isInside( rb->getRight(), rb->getTop() ) ||
                ra->isInside( rb->getLeft(), rb->getBottom() ) ||
                ra->isInside( rb->getRight(), rb->getBottom() )) 
                    ret = true;//set true if intersect

            delete ra;//destroy rectngle 1
            delete rb;//destroy rectangle 1        
            return ret;//return ret
        }


};