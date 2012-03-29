#pragma once
//
// Name :         CGrSphere.h
// Description :  Simple description of a sphere. Used for bounding spheres.
//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//

#if !defined(LibGrafx)
#define LibGrafx
#endif


#include "GrVector.h"
#include "GrTransform.h"

class LibGrafx CGrSphere
{
public:
    CGrSphere();
    CGrSphere(const CGrVector &o, double r) : mOrigin(o), mRadius(r) {}
    CGrSphere(const CGrSphere &o) {mOrigin =o.mOrigin; mRadius = o.mRadius;}

    CGrVector GetOrigin() const {return mOrigin;}
    void SetOrigin(const CGrVector &o) {mOrigin = o;}
    double GetRadius() const {return mRadius;}
    void SetRadius(double r) {mRadius = r;}

    bool IntersectionTest(const CGrSphere &s) const {return (mOrigin - s.mOrigin).LengthSquared3() <=
        (s.mRadius + mRadius) * (s.mRadius + mRadius);} 
    bool IntersectionTest(const CGrVector &v) const
        {return (v - mOrigin).LengthSquared3() < (mRadius * mRadius);}

private:
    CGrVector mOrigin;
    double  mRadius;
};


inline CGrSphere operator*(const CGrTransform &t, const CGrSphere &s)
{
    CGrSphere sphere;
    sphere.SetOrigin(t * s.GetOrigin());
    sphere.SetRadius(s.GetRadius() * sqrt(t[0][0] * t[0][0] +
        t[1][0] * t[1][0] + t[2][0] * t[2][0]));

    return sphere;
}
