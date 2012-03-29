//
// Name :         GrTransform.h
// Description :  Header file for CGrTransform.  This class implements
//                a 4x4 transformation matrix.  
// Notice :       Angles are in degrees.
//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//
#pragma once

#if !defined(_GrTransform_h)
#define _GrTransform_h

#if !defined(LibGrafx)
#define LibGrafx
#endif

#include "GrVector.h"

//! Class that contains a 4 by 4 homogeneous transformation matrix.

/*! The CGrTransform class is used to store a 3D homogeneous transformation
    (4 by 4) and contains functions to support normal vector and matrix
    mathematical operations.

    All angles are degrees for the rotation functions.
 
    See also \ref VecTranGlobals for global functions that operate
    on CGrVector and CGrTransform objects.

\author Charles B. Owen
\version See CGrVector.
*/

class LibGrafx CGrTransform  
{
public:
    //! Default constructor. 
    /*! This function does initialize the matrix to an identity matrix. */
    CGrTransform() {SetIdentity();}

    //! Destructor
    ~CGrTransform() {}

    //! Set the matrix to a zero matrix.
    void SetZero();

    //! Set the matrix to an identity matrix.
    void SetIdentity();

    //! Set the matrix to a translation matrix
    /*! The matrix is set to a 4x4 transformation matrix.
        \param x X
        \param y Y
        \param z Z */
    CGrTransform &SetTranslate(double x, double y, double z) {SetIdentity(); m[0][3]=x; m[1][3]=y; m[2][3]=z;  return *this;}

    //! Set the matrix to a translation matrix
    /*! The matrix is set to a 4x4 transformation matrix.
        \param p A CGrVector object representing the X,Y,Z translation. */
    CGrTransform &SetTranslate(const CGrVector &p) {SetIdentity(); m[0][3]=p.X(); m[1][3]=p.Y(); m[2][3]=p.Z(); return *this;}

    //! Set the translation component of the matrix
    /*! This matrix is unchanged other than the right column, which is set to
        the x, y, z translation amount in p.
        \param p The X, Y, Z, translation amount to set. */
    CGrTransform &SetTranslation(const CGrVector &p) {m[0][3]=p.X(); m[1][3]=p.Y(); m[2][3]=p.Z(); return *this;}

    //! Set the matrix to a rotation matrix around an arbitrary vector.
    /*! \param r Rotation angle (radians).
        \param v Vector to rotate around. Must be normalized! */
    CGrTransform &SetRotate(double r, const CGrVector v);

    //! Set the matrix to a rotation matrix around the X axis.
    /*! \param r Angle for the rotation (radians). */
    CGrTransform &SetRotateX(double r);

    //! Set the matrix to a rotation matrix around the X axis.
    /*! This member function creates a rotation matrix around the 
        X axis where the sine and cosine of the angle are supplied.
        \param cr Cosine of the angle to rotate.
        \param sr Sine of the angle to rotate. */
    CGrTransform &SetRotateX(double cr, double sr);

    //! Set the matrix to a rotation matrix around the Y axis.
    /*! \param r Angle for the rotation (radians). */
    CGrTransform &SetRotateY(double r);

    //! Set the matrix to a rotation matrix around the Y axis.
    /*! This member function creates a rotation matrix around the 
        Y axis where the sine and cosine of the angle are supplied.
        \param cr Cosine of the angle to rotate.
        \param sr Sine of the angle to rotate. */
    CGrTransform &SetRotateY(double cr, double sr);

    //! Set the matrix to a rotation matrix around the Z axis.
    /*! \param r Angle for the rotation (radians). */
    CGrTransform &SetRotateZ(double r);

    //! Set the matrix to a rotation matrix around the Z axis.
    /*! This member function creates a rotation matrix around the 
        Z axis where the sine and cosine of the angle are supplied.
        \param cr Cosine of the angle to rotate.
        \param sr Sine of the angle to rotate. */
    CGrTransform &SetRotateZ(double cr, double sr);

    //! Set the matrix to a rotation matrix where the rows of the matrix are supplied.
    /*! This function creates a rotation matrix that will rotate a frame
        to the origin when x, y, z are valid orthonormal frame coordinate axis.
        \param x Values to set the first row of the matrix.
        \param y Values to set the second row of the matrix.
        \param z Values to set the third row of the matrix. */
    CGrTransform &SetRotate(const CGrVector &x, const CGrVector &y, const CGrVector &z);

    //! Set the matrix to a uniform scaling matrix.
    /*! \param s Scale factor to set. */
    CGrTransform &SetScale(double s) {SetIdentity();  m[0][0]=s;  m[1][1]=s;  m[2][2]=s;  return *this;}

    //! Set the matrix to a scaling matrix.
    /*! \param x X scale factor to set. 
        \param y Y scale factor to set.
        \param z Z scale factor to set.*/
    CGrTransform &SetScale(double x, double y, double z) {SetIdentity();  m[0][0]=x;  m[1][1]=y;  m[2][2]=z;  return *this;}

    //! Transpose this matrix.
    CGrTransform &Transpose();

    //! Set the matrix to a rotation matrix assuming x, y, z are Euler angles
    //! applied in XYZ order.
    /*! \param x X rotation angle (radians). This is the first rotation.
        \param y Y rotation angle (radians). This is the second rotation.
        \param z Z rotation angle (radians). This is the third rotation. */
    CGrTransform &SetEulerXYZ(double x, double y, double z);

    //! Create a uniform scaling matrix.
    /*! \param s Scale factor to set. 
        \code
        CGrTransform t = CGrTransform::GetScale(0.5);
        \endcode */
    static CGrTransform GetScale(double s) {CGrTransform t; t.SetScale(s); return t;}

    //! Create a translation matrix
    /*! A 4x4 transformation matrix is returned.
        \param x X
        \param y Y
        \param z Z 
        \code
        CGrTransform t = CGrTransform::GetTranslate(x, y, z); 
        \endcode */
    static CGrTransform GetTranslate(double x, double y, double z) {CGrTransform t; t.SetTranslate(x, y, z); return t;}

    //! Create a translation matrix
    /*! A 4x4 transformation matrix is returned.
        \param v A vector specifying the x,y,z translation amount.
        \code
        CGrVector v(x, y, z);
        CGrTransform t = CGrTransform::GetTranslate(v); 
        \endcode */
    static CGrTransform GetTranslate(const CGrVector &v) {CGrTransform t; t.SetTranslate(v.X(), v.Y(), v.Z()); return t;}

    //! Get a rotation matrix around the X axis.
    /*! \param r Angle for the rotation (radians). 
        \code 
        CGrTransform t = CGrTransform::GetRotateX(3.1415);
        \endcode */
    static CGrTransform GetRotateX(double r) {CGrTransform t; t.SetRotateX(r); return t;}
    
    //! Get a rotation matrix around the Y axis.
    /*! \param r Angle for the rotation (degrees). 
        \code 
        CGrTransform t = CGrTransform::GetRotateY(32);
        \endcode */
    static CGrTransform GetRotateY(double r) {CGrTransform t; t.SetRotateY(r); return t;}
    
    //! Get a rotation matrix around the Z axis.
    /*! \param r Angle for the rotation (degrees). 
        \code 
        CGrTransform t = CGrTransform::GetRotateZ(45);
        \endcode */
    static CGrTransform GetRotateZ(double r) {CGrTransform t; t.SetRotateZ(r); return t;}

    //! Get a rotation matrix around an arbitrary vector.
    /*! \param r Rotation angle (radians).
        \param v Vector to rotate around. Must be normalized!
        \code
        CGrTransform t = CGrTransform::GetRotate(1.52, CGrVector(0, 1, 0); 
        \endcode */
    static CGrTransform GetRotate(double r, const CGrVector v) {CGrTransform t; t.SetRotate(r, v); return t;}

    //! Get Euler angles for the current matrix rotation component in X,Y,Z order.
    /*! \param x [out] Destination for X.
        \param y [out] Destination for Y.
        \param z [out] Destination for Z. */
    void GetEulerXYZ(double &x, double &y, double &z) const;

    //! Get the translation component of the matrix.
    /*! The translation component is the right-most column first three values. */
    CGrVector GetTranslation() const {return CGrVector(m[0][3], m[1][3], m[2][3], 1);}

    //! This function allows users to access locations in the matrix.
    /*! \param r Row (0-3).
        \param c Column (0-3).
        \code
        CGrTransform t;
        t.SetIdentity();
        double m23 = t.M(2, 3);
        t.M(1, 2) = 13.0;
        \endcode */
    double &M(int r, int c) {return m[r][c];}

    //! This function allows users to access locations in the matrix when the object is const.
    /*! \param r Row (0-3).
        \param c Column (0-3).
        \code
        CGrTransform t;
        t.SetIdentity();
        double m23 = t.M(2, 3);
        \endcode */
    const double &M(int r, int c) const {return m[r][c];}

    //! *= operator. Results in this = this * b
    /*! \param b Matrix to compose (post-multiply) this matrix by. */
    CGrTransform &operator*=(const CGrTransform &b) {return Compose(b);}

    //! = (assignment) operator. Sets the matrix to the values in b.
    /*! \param b Matrix to set the values from. */
    CGrTransform &operator=(const CGrTransform &b) {for(int r=0; r<4;  r++) for(int c=0;  c<4;  c++) m[r][c] = b.m[r][c]; return *this;}

    //! This operator allows access to the values in the matrix using normal array notation.
    /*! \code
        CGrTransform t;
        t.SetIdentity();
        double m23 = t[2][3];
        t[1][2] = 13.0;
        \endcode */
    double *operator[](int r) {return m[r];}

    //! This operator allows access to the values in the matrix using normal array notation
    //! where the transform may be const.
    /*! \code
        CGrTransform t;
        t.SetIdentity();
        double m23 = t[2][3];
        \endcode */
    const double * operator[](int r) const {return m[r];}

    //! Duplicates the functionality of gluSetLookAt() with this matrix as the destination.
    void SetLookAt(double ex, double ey, double ez, double cx, double cy, double cz, double ux, double uy, double uz);

    //! Obtain a look at matrix just as created by gluSetLookAt()
    static CGrTransform GetLookAt(double ex, double ey, double ez, double cx, double cy, double cz, double ux, double uy, double uz);

    //! Obtain a look at matrix just as created by gluSetLookAt()
    static CGrTransform GetLookAt(const double *eye, const double *center, const double *up);

    //! Create a perspective projection matrix
    void SetPerspective(double fov, double aspect, double zNear, double zFar);

    //! Get a perspective projection matrix
    static CGrTransform GetPerspective(double fov, double aspect, double zNear, double zFar);

    //! Set this matrix from a linear array in column major form.
    /*! OpenGL stores matrices in column major format. This function
        can be used to convert a matrix in OpenGL format to CGrTransform 
        format. 
        \parm mat The source matrix, a 16 element linear array of floats
        in column major order. */
    void SetFromColumnMajor(const float *mat)
    {
        for(int i=0;  i<4;  i++)
            for(int j=0;  j<4;  j++)
            {
                m[j][i] = *mat++;
            }
    }

    //! Set this matrix from a linear array in column major form.
    /*! OpenGL stores matrices in column major format. This function
        can be used to convert a matrix in OpenGL format to CGrTransform 
        format. 
        \parm mat The source matrix, a 16 element linear array of doubles
        in column major order. */
    void SetFromColumnMajor(const double *mat)
    {
        for(int i=0;  i<4;  i++)
            for(int j=0;  j<4;  j++)
            {
                m[j][i] = *mat++;
            }
    }

    //! Obtain a copy of this matrix as a linear array in column major form.
    /*! OpenGL stores matrices in column major format. This function can be used
        to convert a matrix in a CGrTransform object to OpenGL format. 
        \paraqm mat The destination matrix, a 16 element linear array of doubles
        in column major order. */
    void SetToColumnMajor(double *mat)
    {
        for(int i=0;  i<4;  i++)
            for(int j=0;  j<4;  j++)
            {
                *mat++ = m[j][i];
            }
    }

    //! Obtain a copy of this matrix as a linear array in column major form.
    /*! OpenGL stores matrices in column major format. This function can be used
        to convert a matrix in a CGrTransform object to OpenGL format. 
        \paraqm mat The destination matrix, a 16 element linear array of floats
        in column major order. */
    void SetToColumnMajor(float *mat)
    {
        for(int i=0;  i<4;  i++)
            for(int j=0;  j<4;  j++)
            {
                *mat++ = (float)m[j][i];
            }
    }


#ifndef NOOPENGL
    //! Applies this matrix to the OpenGL matrix stack using the glMultMatrixd() function.
    void glMultMatrix() const;
#endif

    //! Set the matrix from a quaternion.
    /*! Quaterions are assumed to be in the form a + bi + cj + dk
        \param a Real part of the quaternion.
        \param b i element.
        \param c j element.
        \param d k element. */
    CGrTransform &SetFromQuaternion(double a, double b, double c, double d)
    {
        m[0][3] = m[1][3] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.;
        m[3][3] = 1.;

        m[0][0] = a * a + b * b - c * c - d * d;
        m[0][1] = 2. * b * c - 2. * a * d;
        m[0][2] = 2. * a * c + 2. * b * d;
        m[1][0] = 2. * a * d + 2. * b * c;
        m[1][1] = a * a - b * b + c * c - d * d;
        m[1][2] = 2. * c * d - 2. * a * b;
        m[2][0] = (2. * b * d - 2. * a * c);
        m[2][1] = (2. * a * b + 2. * c * d);
        m[2][2] = (a * a - b * b - c * c + d * d);
        return *this;
    }

    //! Set the matrix from a quaternion.
    /*! Quaterions are assumed to be in the form a + bi + cj + dk
        \param q Array of four doubles in the form a, b, c, d sequentially in memory. */
    CGrTransform &SetFromQuaternion(const double *q) {SetFromQuaternion(q[0], q[1], q[2], q[3]);  return *this;}

    
    //! Set this matrix to the inverse of another matrix, assuming the matrix
    //! is affine.
    /*! This will work for matrices where the last row is (0, 0, 0, 1).
        \param fm Matrix to invert. 
        \return *this. */
    CGrTransform &SetAffineInverse(const CGrTransform &fm);

    //! Get the inverse of a matrix, assuming the matrix
    //! is affine.
    /*! This will work for matrices where the last row is (0, 0, 0, 1).
        \param fm Matrix to invert. 
        \return Inverse of affine matrix fm. */
    static CGrTransform GetAffineInverse(const CGrTransform &fm) {CGrTransform r;  r.SetAffineInverse(fm);  return r;}

    //! Invert this matrix
    /*! This will work for matrices where the last row is (0, 0, 0, 1). 
        \return *this. */
    CGrTransform &AffineInverse(void) {CGrTransform b; b.SetAffineInverse(*this);  *this = b; return *this;}

    //! Set this matrix to the inverse of another matrix.
    /*! This will work for any non-singular matrix. This is not as fast as SetAffineInverse
        and that function is recommended when a strict inverse is not necessary.
        \param fm Matrix to invert. 
        \return *this. */
    CGrTransform &SetInverse(const CGrTransform &fm);

    //! Get the inverse of a matrix.
    /*! This will work for any non-singular matrix. This is not as fast as GetAffineInverse
        and that function is recommended when a strict inverse is not necessary.
        \param fm Matrix to invert. 
        \return Inverse of matrix fm. */
    static CGrTransform GetInverse(const CGrTransform &fm) {CGrTransform r;  r.SetInverse(fm);  return r;}

    //! Invert this matrix
    /*! This will work for matrices where the last row is (0, 0, 0, 1). 
        \return *this. */
    CGrTransform Invert(void) {CGrTransform b; b.SetInverse(*this);  *this = b; return *this;}

private:
    CGrTransform &Compose(const CGrTransform &b);		// Exported use is discouraged, use *=

    double m[4][4];
};

//! Normal * operator. Computes the product of two matrices.
/*! Results is a * b.
    \ingroup VecTranGlobals
    \param a First matrix
    \param b Second matrix */
inline CGrTransform operator *(const CGrTransform &a, const CGrTransform &b)
{
   CGrTransform x;
   for(int r=0;  r<4;  r++)
      for(int c=0;  c<4;  c++)
      {
         x[r][c] = a[r][0] * b[0][c] + a[r][1] * b[1][c] + a[r][2] * b[2][c] + a[r][3] * b[3][c];
      }

   return x;
}

inline CGrTransform &CGrTransform::Compose(const CGrTransform &b)
{
   *this = *this * b;
   return *this;
}

//! Normal * operator. Computes the product of a matrix and a vector.
/*! Results is a * p.
    \ingroup VecTranGlobals
    \param a The matrix
    \param p The vector */
inline CGrVector operator *(const CGrTransform &a, const CGrVector &p)
{
   return CGrVector(a[0][0] * p.X() + a[0][1] * p.Y() + a[0][2] * p.Z() + a[0][3] * p.W(),
               a[1][0] * p.X() + a[1][1] * p.Y() + a[1][2] * p.Z() + a[1][3] * p.W(),
               a[2][0] * p.X() + a[2][1] * p.Y() + a[2][2] * p.Z() + a[2][3] * p.W(),
               a[3][0] * p.X() + a[3][1] * p.Y() + a[3][2] * p.Z() + a[3][3] * p.W());
}

//! Computes and returns the transpose of a matrix.
/*! Results is a * p.
    \ingroup VecTranGlobals
    \param t The matrix to transpose.
    \code
    CGrTransform t;
    t.SetRotateX(0.12);
    CGrTransform u = Transpose(t);
    \endcode */
inline CGrTransform Transpose(const CGrTransform &t)
{
   CGrTransform r(t);
   return r.Transpose();
}


//
// This header contains extensions to CTransform for the LibRealityGL 
// library.
//

#ifndef NOOPENGL

//! Utilize a matrix to transform on the OpenGL stack.
LibGrafx void glTransform(const CGrTransform &transform);

#endif

#endif
