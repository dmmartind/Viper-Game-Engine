//*********** (C) Copyright 2009 David Martin All rights reserved. **********
//
// Purpose: Math Class Code 
//
//****************************************************************************************
#include "math.h"

namespace Viper2D {

    double cMath::toDegrees(double radians)
    {
        return radians * PI_under_180;
    }
    
    double cMath::toRadians(double degrees)
    {
        return degrees * PI_over_180;
    }
    
    double cMath::wrapAngleDegs(double degs)
    {
        double result = fmod(degs, 360.0);
        if (result < 0) result += 360.0f;
        return result;
    }
    
    double cMath::wrapAngleRads(double rads)
    {
        double result = fmod(rads, PI);
        if (result < 0) result += PI;
        return result;
    }

    double cMath::LinearVelocityX(double angle)
    {
        angle -= 90;//
        if (angle < 0) angle = 360 + angle;    
        return cos( angle * PI_over_180);
    }
    
    double cMath::LinearVelocityY(double angle)
    {
        angle -= 90;
        if (angle < 0) angle = 360 + angle;    
        return sin( angle * PI_over_180);
    }
    
    _vector cMath::LinearVelocity(double angle)
    {
        double vx = LinearVelocityX(angle);
        double vy = LinearVelocityY(angle);
        return _vector((float)vx,(float)vy,(float)0.0f);
    }
    
    double cMath::AngleToTarget(double x1,double y1,double x2,double y2)
    {
        double deltaX = (x2-x1);
        double deltaY = (y2-y1);
        return atan2(deltaY,deltaX);
    }
    
    double cMath::AngleToTarget(_vector& source,_vector& target)
    {
        return AngleToTarget(source.getx(),source.gety(),target.getx(),target.gety());
    }
    
    double cMath::Distance( double x1,double y1,double x2,double y2 )
    {
        double deltaX = (x2-x1);
        double deltaY = (y2-y1);
        return sqrt(deltaX*deltaX + deltaY*deltaY);
    }
    
    double cMath::Distance( _vector& vec1, _vector& vec2 ) 
    {
        return Distance(vec1.getx(),vec1.gety(),vec2.getx(),vec2.gety());
    }
    
    double cMath::Length(double x,double y,double z)
    {
        return sqrt(x*x + y*y + z*z);
    }
    
    double cMath::Length(_vector& vec)
    {
        return Length(vec.getx(),vec.gety(),vec.getz());
    }
    
    double cMath::DotProduct(double x1,double y1,double z1,double x2,double y2,double z2)
    {
        return (x1*x2 + y1*y2 + z1*z2);
    }
    
    double cMath::DotProduct( _vector& vec1, _vector& vec2 )
    {
        return DotProduct(vec1.getx(),vec1.gety(),vec1.getz(),vec2.getx(),vec2.gety(),vec2.getz());
    }
    
    _vector cMath::CrossProduct( double x1,double y1,double z1,double x2,double y2,double z2)
    {
        double nx = (y1*z2)-(z1*y2);
        double ny = (z1*y2)-(x1*z2);
        double nz = (x1*y2)-(y1*x2);
        return _vector((float)nx,(float)ny,(float)nz);
    }
    
    _vector cMath::CrossProduct( _vector& vec1, _vector& vec2 )
    {
        return CrossProduct(vec1.getx(),vec1.gety(),vec1.getz(),vec2.getx(),vec2.gety(),vec2.getz());
    }
    
    _vector    cMath::Normal(double x,double y,double z)
    {
        double length = Length(x,y,z);
        if (length != 0) length = 1 / length;
        double nx = x*length;
        double ny = y*length;
        double nz = z*length;
        return _vector((float)nx,(float)ny,(float)nz);
    }
    
    _vector cMath::Normal(_vector& vec)
    {
        return Normal(vec.getx(),vec.gety(),vec.getz());
    }
    
};
