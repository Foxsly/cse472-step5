//
// Name :         GrTransform.cpp
// Description :  Implementation file for CGrTransform.  This class implements
//                a 4x4 transformation matrix.  
//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//
#include "stdafx.h"
#include <cmath>
#include "GrTransform.h"

#ifndef NOOPENGL
#include <GL/gl.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef NOOPENGL
//! Applies this matrix to the OpenGL matrix stack using the glMultMatrixd() function.
void CGrTransform::glMultMatrix() const
{
    double mm[16];
    for(int i=0;  i<4;  i++)
        for(int j=0;  j<4;  j++)
        {
            mm[i * 4 + j] = m[j][i];
        }

        ::glMultMatrixd(mm);
}
#endif

//
// Name :         glTransform()
// Description :  Utilize a matrix to transform on the OpenGL stack.
//
void glTransform(const CGrTransform &transform)
{
    GLdouble mat[16];
    GLdouble *m = mat;
    for(int c=0;  c<4;  c++)
    {
        for(int r=0;  r<4;  r++)
        {
            *m++ = transform.M(r, c);
        }
    }

    glMultMatrixd(mat);
}


CGrTransform CGrTransform::GetLookAt(double ex, double ey, double ez, double cx, double cy, double cz, double ux, double uy, double uz)
{
    CGrTransform t;
    t.SetLookAt(ex, ey, ez, cx, cy, cz, ux, uy, uz);
    return t;
}

CGrTransform CGrTransform::GetLookAt(const double *eye, const double *center, const double *up)
{
    CGrTransform t;
    t.SetLookAt(eye[0], eye[1], eye[2],
        center[0], center[1], center[2], 
        up[0], up[1], up[2]);
    return t;
}


CGrTransform CGrTransform::GetPerspective(double fov, double aspect, double zNear, double zFar)
{
    CGrTransform t;
    t.SetPerspective(fov, aspect, zNear, zFar);\
    return t;
}

void CGrTransform::SetIdentity()
{
   m[0][0] = 1;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = 1;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = 0;  m[2][2] = 1;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;
}

void CGrTransform::SetZero()
{
   m[0][0] = 0;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = 0;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = 0;  m[2][2] = 0;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 0;
}

//
// Rotation matrices.  There are two versions of each
// of the standard matrices, a version that accepts
// an angle IN DEGREES and a version that accepts
// the cosine and sine of the rotation angle.
//

CGrTransform &CGrTransform::SetRotateX(double r)
{
   double rr = r * GR_DTOR;
   double cr = cos(rr);
   double sr = sin(rr);

   m[0][0] = 1;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = cr;  m[1][2] = -sr;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = sr;  m[2][2] = cr;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

CGrTransform &CGrTransform::SetRotateX(double cr, double sr)
{
   m[0][0] = 1;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = cr;  m[1][2] = -sr;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = sr;  m[2][2] = cr;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

CGrTransform &CGrTransform::SetRotateY(double r)
{
   double rr = r * GR_DTOR;
   double cr = cos(rr);
   double sr = sin(rr);

   m[0][0] = cr;  m[0][1] = 0;  m[0][2] = sr;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = 1;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = -sr;  m[2][1] = 0;  m[2][2] = cr;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

CGrTransform & CGrTransform::SetRotateY(double cr, double sr)
{
   m[0][0] = cr;  m[0][1] = 0;  m[0][2] = sr;  m[0][3] = 0;
   m[1][0] = 0;  m[1][1] = 1;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = -sr;  m[2][1] = 0;  m[2][2] = cr;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}


CGrTransform &CGrTransform::SetRotateZ(double r)
{
   double rr = r * GR_DTOR;
   double cr = cos(rr);
   double sr = sin(rr);

   m[0][0] = cr;  m[0][1] = -sr;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = sr;  m[1][1] = cr;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = 0;  m[2][2] = 1;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

CGrTransform & CGrTransform::SetRotateZ(double cr, double sr)
{
   m[0][0] = cr;  m[0][1] = -sr;  m[0][2] = 0;  m[0][3] = 0;
   m[1][0] = sr;  m[1][1] = cr;  m[1][2] = 0;  m[1][3] = 0;
   m[2][0] = 0;  m[2][1] = 0;  m[2][2] = 1;  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}

//
// Name :         CGrTransform::SetRotate()
// Description :  Create a rotation matrix that will rotate
//                by angle r around vector v.  Note that
//                r is in degrees.
//

CGrTransform &CGrTransform::SetRotate(double r, const CGrVector v)
{
	double rr = r * GR_DTOR;
	double c = cos(rr);
    double s = sin(rr);
    double t = 1. - c;

	double l = v.Length3();
	double x = v.X() / l;
	double y = v.Y() / l;
	double z = v.Z() / l;

    m[0][0] = t * x * x + c;
    m[0][1] = t * x * y - s * z;
    m[0][2] = t * x * z + s * y;
    m[0][3] = 0;

    m[1][0] = t * x * y + s * z;
    m[1][1] = t * y * y + c;
    m[1][2] = t * y * z - s * x;
    m[1][3] = 0;

    m[2][0] = t * x * z - s * y;
    m[2][1] = t * y * z + s * x;
    m[2][2] = t * z * z + c;
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;

	return *this;
}



CGrTransform & CGrTransform::SetRotate(const CGrVector &x, const CGrVector &y, const CGrVector &z)
{
   m[0][0] = x.X();  m[0][1] = x.Y();  m[0][2] = x.Z();  m[0][3] = 0;
   m[1][0] = y.X();  m[1][1] = y.Y();  m[1][2] = y.Z();  m[1][3] = 0;
   m[2][0] = z.X();  m[2][1] = z.Y();  m[2][2] = z.Z();  m[2][3] = 0;
   m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;

   return *this;
}


//
// Name :        CGrTransform::SetEulerXYZ()
// Description : Create a rotation matrix from Euler angles in the rotation
//               order XYZ
//

CGrTransform &CGrTransform::SetEulerXYZ(double x, double y, double z)
{
    CGrTransform rx, ry, rz;
    rx.SetRotateX(x);
    ry.SetRotateY(y);
    rz.SetRotateZ(z);
    
    *this = rz * ry * rx;
    return *this;
}


//
// Name :         CGrTransform::GetEulerXYZ()
// Description :  Extract Euler angles for the current rotation matrix.
//                Based on the order XYZ.
//

void CGrTransform::GetEulerXYZ(double &x, double &y, double &z) const
{
        // What's the angle the X vector rotates to?
        double xx = m[0][0];
        double xy = m[1][0];
        double xz = m[2][0];

        // Determine the Z angle first
        if(fabs(xx) < 0.000001 && fabs(xy) < 0.00001)
        {
            // Gimble lock condition.  
            y = -90;
            z = 0;
        }
        else
        {
            z = atan2(xy, xx) * GR_RTOD;
            y = asin(-xz) * GR_RTOD;
        }

        // Now that we have these, we create a rotation matrix
        // that cancels them and figure out what the rotation is.
        CGrTransform rzinv, ryinv;
        rzinv.SetRotateZ(-z);
        ryinv.SetRotateY(-y);

        CGrTransform rxfor = ryinv * rzinv * *this;
        x = atan2(rxfor[2][1], rxfor[1][1]) * GR_RTOD;
}


inline void _swap(double &a, double &b)
{
   double t = a;
   a = b;
   b = t;
}

CGrTransform & CGrTransform::Transpose()
{
   _swap(m[0][1], m[1][0]);
   _swap(m[0][2], m[2][0]);
   _swap(m[0][3], m[3][0]);
   _swap(m[1][2], m[2][1]);
   _swap(m[1][3], m[3][1]);
   _swap(m[2][3], m[3][2]);

   return *this;
}

//
// Name :         CGrTransform::SetLookAt() 
// Description :  Construct a LookAt matrix.  This is a rotation and
//                translation that will put the eye at 0,0,0, up in the
//                direction of the Y axis and looking down the -Z axis.
//

void CGrTransform::SetLookAt(double ex, double ey, double ez, 
                          double cx, double cy, double cz, 
                          double ux, double uy, double uz)
{
   CGrVector eye(ex, ey, ez);
   CGrVector center(cx, cy, cz);
   CGrVector up(ux, uy, uz);
   
   CGrVector cameraz = Normalize3(eye - center);
   CGrVector camerax = Normalize3(Cross(up, cameraz));
   CGrVector cameray = Cross(cameraz, camerax);

   CGrTransform r;
   r[0][0] = camerax.X();  r[0][1]= camerax.Y();  r[0][2] = camerax.Z();  r[0][3] = 0;
   r[1][0] = cameray.X();  r[1][1] = cameray.Y();  r[1][2] = cameray.Z();  r[1][3] = 0;
   r[2][0] = cameraz.X();  r[2][1] = cameraz.Y();  r[2][2] = cameraz.Z();  r[2][3] = 0;
   r[3][0] = r[3][1] = r[3][2] = 0.;  r[3][3] = 1.0;

   CGrTransform t;
   t.SetTranslate(-ex, -ey, -ez);

   *this = r * t;
}


//
// Name :         CGrTransform::SetPerspective()
// Description :  Set this matrix as a perspective projection matrix.
//
void CGrTransform::SetPerspective(double fov, double aspect, double zNear, double zFar)
{
    SetZero();

    double range = tan(fov * GR_DTOR / 2) * zNear;
    m[0][0] = (2 * zNear) / ((range * aspect) - (-range * aspect));
    m[1][1] = (2 * zNear) / (2 * range);
    m[2][2] = -(zFar + zNear) / (zFar - zNear);
    m[3][2] = -1;
    m[2][3] = -(2 * zFar * zNear) / (zFar - zNear);
}



//
// Name :         CGrTransform::SetAffineInverse()
// Description :  Set this matrix to the inverse of another matrix,
//                assuming the other matrix is an affine transform
//

CGrTransform &CGrTransform::SetAffineInverse(const CGrTransform &fm)
{
    // First compute the inverse of the upper left 3x3 submatrix
    double adjoint[3][3];

    adjoint[0][0] =  (fm.M(1, 1) * fm.M(2, 2) - fm.M(1, 2) * fm.M(2, 1));
    adjoint[1][0] = -(fm.M(1, 0) * fm.M(2, 2) - fm.M(1, 2) * fm.M(2, 0));
    adjoint[2][0] =  (fm.M(1, 0) * fm.M(2, 1) - fm.M(1, 1) * fm.M(2, 0));
    adjoint[0][1] = -(fm.M(0, 1) * fm.M(2, 2) - fm.M(0, 2) * fm.M(2, 1));
    adjoint[1][1] =  (fm.M(0, 0) * fm.M(2, 2) - fm.M(0, 2) * fm.M(2, 0));
    adjoint[2][1] = -(fm.M(0, 0) * fm.M(2, 1) - fm.M(0, 1) * fm.M(2, 0));
    adjoint[0][2] =  (fm.M(0, 1) * fm.M(1, 2) - fm.M(0, 2) * fm.M(1, 1));
    adjoint[1][2] = -(fm.M(0, 0) * fm.M(1, 2) - fm.M(0, 2) * fm.M(1, 0));
    adjoint[2][2] =  (fm.M(0, 0) * fm.M(1, 1) - fm.M(0, 1) * fm.M(1, 0));

    // Now compute the determinate
    // It is the sum of the products of the cofactors and the elements of one 
    // row of the matrix:

    double det = fm.M(0, 0) * adjoint[0][0] + fm.M(0, 1) * adjoint[1][0] + fm.M(0, 2) * adjoint[2][0];
    if(det == 0)
        det = 0.000001;

    // Put in as the rotation part:

    M(0, 0) = adjoint[0][0] / det;
    M(0, 1) = adjoint[0][1] / det;
    M(0, 2) = adjoint[0][2] / det;
    M(1, 0) = adjoint[1][0] / det;
    M(1, 1) = adjoint[1][1] / det;
    M(1, 2) = adjoint[1][2] / det;
    M(2, 0) = adjoint[2][0] / det;
    M(2, 1) = adjoint[2][1] / det;
    M(2, 2) = adjoint[2][2] / det;
    M(3, 0) = M(3, 1) = M(3, 2) = 0.;
    M(3, 3) = fm.M(3, 3);

    double x = -fm.M(0, 3);
    double y = -fm.M(1, 3);
    double z = -fm.M(2, 3);

    M(0, 3) = x * M(0, 0) + y * M(0, 1) + z * M(0, 2);
    M(1, 3) = x * M(1, 0) + y * M(1, 1) + z * M(1, 2);
    M(2, 3) = x * M(2, 0) + y * M(2, 1) + z * M(2, 2);

    return *this;
}




CGrTransform &CGrTransform::SetInverse(const CGrTransform &b)
{
    // First column
    m[0][0] = b[1][1]  * b[2][2] * b[3][3] - 
             b[1][1]  * b[3][2] * b[2][3] - 
             b[1][2]  * b[2][1]  * b[3][3] + 
             b[1][2]  * b[3][1]  * b[2][3] +
             b[1][3] * b[2][1]  * b[3][2] - 
             b[1][3] * b[3][1]  * b[2][2];

    m[0][1] = -b[0][1]  * b[2][2] * b[3][3] + 
              b[0][1]  * b[3][2] * b[2][3] + 
              b[0][2]  * b[2][1]  * b[3][3] - 
              b[0][2]  * b[3][1]  * b[2][3] - 
              b[0][3] * b[2][1]  * b[3][2] + 
              b[0][3] * b[3][1]  * b[2][2];

    m[0][2] = b[0][1]  * b[1][2] * b[3][3] - 
             b[0][1]  * b[3][2] * b[1][3] - 
             b[0][2]  * b[1][1] * b[3][3] + 
             b[0][2]  * b[3][1] * b[1][3] + 
             b[0][3] * b[1][1] * b[3][2] - 
             b[0][3] * b[3][1] * b[1][2];

    m[0][3] = -b[0][1]  * b[1][2] * b[2][3] + 
               b[0][1]  * b[2][2] * b[1][3] +
               b[0][2]  * b[1][1] * b[2][3] - 
               b[0][2]  * b[2][1] * b[1][3] - 
               b[0][3] * b[1][1] * b[2][2] + 
               b[0][3] * b[2][1] * b[1][2];

    m[1][0] = -b[1][0]  * b[2][2] * b[3][3] + 
              b[1][0]  * b[3][2] * b[2][3] + 
              b[1][2]  * b[2][0] * b[3][3] - 
              b[1][2]  * b[3][0] * b[2][3] - 
              b[1][3] * b[2][0] * b[3][2] + 
              b[1][3] * b[3][0] * b[2][2];

    m[1][1] = b[0][0]  * b[2][2] * b[3][3] - 
             b[0][0]  * b[3][2] * b[2][3] - 
             b[0][2]  * b[2][0] * b[3][3] + 
             b[0][2]  * b[3][0] * b[2][3] + 
             b[0][3] * b[2][0] * b[3][2] - 
             b[0][3] * b[3][0] * b[2][2];

    m[1][2] = -b[0][0]  * b[1][2] * b[3][3] + 
              b[0][0]  * b[3][2] * b[1][3] + 
              b[0][2]  * b[1][0] * b[3][3] - 
              b[0][2]  * b[3][0] * b[1][3] - 
              b[0][3] * b[1][0] * b[3][2] + 
              b[0][3] * b[3][0] * b[1][2];

    m[1][3] = b[0][0]  * b[1][2] * b[2][3] - 
              b[0][0]  * b[2][2] * b[1][3] - 
              b[0][2]  * b[1][0] * b[2][3] + 
              b[0][2]  * b[2][0] * b[1][3] + 
              b[0][3] * b[1][0] * b[2][2] - 
              b[0][3] * b[2][0] * b[1][2];

    m[2][0] = b[1][0]  * b[2][1] * b[3][3] - 
             b[1][0]  * b[3][1] * b[2][3] - 
             b[1][1]  * b[2][0] * b[3][3] + 
             b[1][1]  * b[3][0] * b[2][3] + 
             b[1][3] * b[2][0] * b[3][1] - 
             b[1][3] * b[3][0] * b[2][1];

    m[2][1] = -b[0][0]  * b[2][1] * b[3][3] + 
              b[0][0]  * b[3][1] * b[2][3] + 
              b[0][1]  * b[2][0] * b[3][3] - 
              b[0][1]  * b[3][0] * b[2][3] - 
              b[0][3] * b[2][0] * b[3][1] + 
              b[0][3] * b[3][0] * b[2][1];

    m[2][2] = b[0][0]  * b[1][1] * b[3][3] - 
              b[0][0]  * b[3][1] * b[1][3] - 
              b[0][1]  * b[1][0] * b[3][3] + 
              b[0][1]  * b[3][0] * b[1][3] + 
              b[0][3] * b[1][0] * b[3][1] - 
              b[0][3] * b[3][0] * b[1][1];

    m[2][3] = -b[0][0]  * b[1][1] * b[2][3] + 
               b[0][0]  * b[2][1] * b[1][3] + 
               b[0][1]  * b[1][0] * b[2][3] - 
               b[0][1]  * b[2][0] * b[1][3] - 
               b[0][3] * b[1][0] * b[2][1] + 
               b[0][3] * b[2][0] * b[1][1];

    m[3][0] = -b[1][0] * b[2][1] * b[3][2] + 
              b[1][0] * b[3][1] * b[2][2] + 
              b[1][1] * b[2][0] * b[3][2] - 
              b[1][1] * b[3][0] * b[2][2] - 
              b[1][2] * b[2][0] * b[3][1] + 
              b[1][2] * b[3][0] * b[2][1];

    m[3][1] = b[0][0] * b[2][1] * b[3][2] - 
             b[0][0] * b[3][1] * b[2][2] - 
             b[0][1] * b[2][0] * b[3][2] + 
             b[0][1] * b[3][0] * b[2][2] + 
             b[0][2] * b[2][0] * b[3][1] - 
             b[0][2] * b[3][0] * b[2][1];

    m[3][2] = -b[0][0] * b[1][1] * b[3][2] + 
               b[0][0] * b[3][1] * b[1][2] + 
               b[0][1] * b[1][0] * b[3][2] - 
               b[0][1] * b[3][0] * b[1][2] - 
               b[0][2] * b[1][0] * b[3][1] + 
               b[0][2] * b[3][0] * b[1][1];

    m[3][3] = b[0][0] * b[1][1] * b[2][2] - 
              b[0][0] * b[2][1] * b[1][2] - 
              b[0][1] * b[1][0] * b[2][2] + 
              b[0][1] * b[2][0] * b[1][2] + 
              b[0][2] * b[1][0] * b[2][1] - 
              b[0][2] * b[2][0] * b[1][1];

    double det = b[0][0] * m[0][0] + b[1][0] * m[0][1] + b[2][0] * m[0][2] + b[3][0] * m[0][3];

    if(det == 0)
        det = 0.000001;

    det = 1.0 / det;

    for(int r=0;  r<4;  r++)
    {
        for(int c=0;  c<4;  c++)
        {
            m[r][c] *= det;
        }
    }

    return *this;
}

