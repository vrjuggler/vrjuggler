#ifndef _VJ_MATRIX_
#define _VJ_MATRIX_
#pragma once

#include <vjConfig.h>
#include <math.h>
#include <string.h>

#include <Math/vjVec3.h>

class vjQuat;
class vjVec3;
class vjCoord;

#ifdef VJ_API_PERFORMER
   #include <Performer/pr/pfLinMath.h>
#endif

// Andy: M_PI not defined in win32
#ifndef M_PI
#define M_PI 3.14159265358979323846
#define M_PI_2   1.57079632679489661923
#endif


//-----------------------------------------------------------------------------
//: vjMatrix: Scene graph matrix optimized for OpenGL.
//
// C/C++ uses matrices in row major order.  In other words the access
// indices look like: <br>
// (0,0) (0,1) (0,2) (0,3)   <=== Array      <br>
// (1,0) (1,1) (1,2) (1,3)   <=== Array      <br>
// (2,0) (2,1) (2,2) (2,3)   <=== Array      <br>
// (3,0) (3,1) (3,2) (3,3)   <=== Array      <br>
// <br>
// For OpenGL, the matrix has to be column major in memory, so we need to
// access it more like: <br>
//    NOTE: The given indexes are what the cells have to be called in C/C++ notation
//	    Since we are putting the columns into memory back-to-back
// (0,0) (1,0) (2,0) (3,0)       <br>
// (0,1) (1,1) (2,1) (3,1)       <br>
// (0,2) (1,2) (2,2) (3,2)       <br>
// (0,3) (1,3) (2,3) (3,3)       <br>
//   ^     ^     ^     ^         <br>
//   ====================== Arrays     <br>
//
// This is VERY important!!!
//--------------------------------------------------------------------------------
class vjMatrix
{
public:
   // Column major.  In other words {Column1, Column2, Column3, Column4} in memory
   // Access element mat[column][row]
   float mat[4][4];

public:
   //: Constructor: Identity constructor
   vjMatrix() { makeIdent();};

   //: Constructor: Specify each element
   vjMatrix(float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33)
   {
      mat[0][0] = a00; mat[1][0] = a01; mat[2][0] = a02; mat[3][0] = a03;
      mat[0][1] = a10; mat[1][1] = a11; mat[2][1] = a12; mat[3][1] = a13;
      mat[0][2] = a10; mat[1][2] = a21; mat[2][2] = a22; mat[3][2] = a23;
      mat[0][3] = a30; mat[1][3] = a31; mat[2][3] = a32; mat[3][3] = a33;
   }

   //: Build matrix from coord
   vjMatrix(vjCoord coord);

public:
   //: Set from a memory region
   //! PRE: m refers to an array of 16 floats
   //! POST: this' = *m
   void    set(float *m) {
      memcpy(mat, m, sizeof(float) * 16);
   }

   //: Set matrix elementwise
   //!POST: this' is set to pass args
   //!ARGS: a00 - Upper left
   //!ARGS: a33 - Lower left
   void set(float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33)
   {
      mat[0][0] = a00; mat[1][0] = a01; mat[2][0] = a02; mat[3][0] = a03;
      mat[0][1] = a10; mat[1][1] = a11; mat[2][1] = a12; mat[3][1] = a13;
      mat[0][2] = a20; mat[1][2] = a21; mat[2][2] = a22; mat[3][2] = a23;
      mat[0][3] = a30; mat[1][3] = a31; mat[2][3] = a32; mat[3][3] = a33;
   }

public:
   //: Create Identity Matrix
   //! POST: this' is identity
   void makeIdent()
   { zero(); mat[0][0] = mat[1][1] = mat[2][2] = mat[3][3] = 1.0f;}

   //: Zero out the matrix
   //! POST: this' is all zeros
   void zero()
   {
      mat[0][0] = mat[1][0] = mat[2][0] = mat[3][0] = 0.0f;
      mat[0][1] = mat[1][1] = mat[2][1] = mat[3][1] = 0.0f;
      mat[0][2] = mat[1][2] = mat[2][2] = mat[3][2] = 0.0f;
      mat[0][3] = mat[1][3] = mat[2][3] = mat[3][3] = 0.0f;
   }

   //: Make XYZEuler matrix
   //!ARGS: rotX - Rotation about the x-axis
   //!ARGS: rotY - Rotation about the y-axis
   //!ARGS: rotZ - Rotation about the z-axis
   //
   //!POST: mat = rX*rY*rZ
   void makeXYZEuler(float xRot, float yRot, float zRot);
   void getXYZEuler(float& xRot, float& yRot, float& zRot);

   //: Make ZYX Euler matrix
   //!POST: mat = rZ*rY*rX
   //!ARGS: rotZ - Rotation about the z-axis
   //!ARGS: rotY - Rotation about the y-axis
   //!ARGS: rotX - Rotation about the x-axis
   void makeZYXEuler(float zRot, float yRot, float xRot);
   void getZYXEuler(float& zRot, float& yRot, float& xRot);

   //!POST: mat = rZ*rX*rY
   void makeZXYEuler(float zRot, float xRot, float yRot);
   void getZXYEuler(float& zRot, float& xRot, float& yRot);

   //: Make a matrix using direction cosines
   //
   //!ARGS: secXAxis - The x-axis of the secondary coord system in terms of the first
   //+      secYAxis - The y-axis of the secondary coord system in terms of the first
   //+      secZAxis - The z-axis of the secondary coord system in terms of the first
   //
   //!POST: mat = direction cosine matrix
   //!NOTE: Based on "Virtual Reality Sytems" pg. 26
   //+      The matrix created is able to transform a pt in the first coord
   //+      system to the second coord system. pt'= Mat*pt
   void makeDirCos(vjVec3 secXAxis, vjVec3 secYAxis, vjVec3 secZAxis);

   //: Make matrix from given quaternion
   //!POST: mat = Matrix specified by Quaternion _quat
   void    makeQuaternion(float* q);
   void    makeQuaternion(vjQuat& q);


   //: Make rotation matrix around _axis
   void makeRot(float _degrees, vjVec3 _axis);

   //: Make translation matrix
   void makeTrans(float _x, float _y, float _z);

   //: Sets given translation to current matrix
   //!POST: mat' = mat with the tranlation portion changed to the parameters passed
   void setTrans(float _x, float _y, float _z);

   //: Get the translation portion of the matrix
   //!POST: _x, _y, and _z contain the translation portions of the matrix
   void getTrans(float& _x, float& _y, float& _z);

   //: Make scale matrix
   void makeScale(float _x, float _y, float _z);

//    void	makeVecRotVec(const vjVec3&  _v1, const vjVec3&  _v2)
//	{;}
//    void	makeCoord(const sgCoord* _c)
//	{;}

   //: Copy matrix
   //!POST: this' = _v
   void copy(const vjMatrix&  _v) { *this = _v;}

   //: Compare matrix for equality
   //!RETVAL: 1 - this == _m
   //!RETVAL: 0 - this != _m
   int     equal(const vjMatrix&  _m) const  {
      return (((mat)[0][0] == (_m)[0][0]) &&
              ((mat)[0][1] == (_m)[0][1]) &&
              ((mat)[0][2] == (_m)[0][2]) &&
              ((mat)[0][3] == (_m)[0][3]) &&
              ((mat)[1][0] == (_m)[1][0]) &&
              ((mat)[1][1] == (_m)[1][1]) &&
              ((mat)[1][2] == (_m)[1][2]) &&
              ((mat)[1][3] == (_m)[1][3]) &&
              ((mat)[2][0] == (_m)[2][0]) &&
              ((mat)[2][1] == (_m)[2][1]) &&
              ((mat)[2][2] == (_m)[2][2]) &&
              ((mat)[2][3] == (_m)[2][3]) &&
              ((mat)[3][0] == (_m)[3][0]) &&
              ((mat)[3][1] == (_m)[3][1]) &&
              ((mat)[3][2] == (_m)[3][2]) &&
              ((mat)[3][3] == (_m)[3][3]));
   }

   //: Clamps the values of the matrix to zero
   //!POST: Any values in matrix < (some epsilon) are set to 0
   void zeroClamp()
   {
      // Clamp values near zero.  Just for looks.
      // Could be removed
       for(int i=0;i<4;i++)
          for(int j=0;j<4;j++)
             mat[i][j] = VJ_ZERO_CLAMP(mat[i][j]);
   }

public:
   //: Set to the transpose of the matrix
   //!POST: mat = transpose(_m)
   void transpose(vjMatrix&  _m)
   {
      for (int i=0; i<4; i++)
         for (int j=0; j<4; j++)
            mat[i][j] = _m.mat[j][i];
   }

   //: Multiple 2 matrices
   //!POST: mat = m1 * m2
   void mult(const vjMatrix& _m1, const vjMatrix &_m2)
   {
      *this = _m1;       //  Set equal to param 1
      postMult(_m2);     // Now post mult be the second param
   }

   //: Add 2 matrices
   //!POST: mat = m1 + m2
   void add(const vjMatrix& _m1, const vjMatrix &_m2)
   {
      for (int n=0;n<4;n++)
         for (int m=0;m<4;m++)
            mat[n][m] = (_m1.mat[n][m] + _m2.mat[n][m]);
   }

   //: Subtract a matrix
   //!POST: mat' = m1 - m2
   void sub(const vjMatrix& _m1, const vjMatrix &_m2)
   {
      for (int n=0;n<4;n++)
         for (int m=0;m<4;m++)
            mat[n][m] = (_m1.mat[n][m] - _m2.mat[n][m]);
   }

   //: Scale a matrix by a scalar
   //!POST: Each element of mat' = mat * _s
   //!NOTE: Not a 3D scale
   void scale(float _s, const vjMatrix &_m)
   {
      for (int n=0;n<4;n++)
         for (int m=0;m<4;m++)
            mat[n][m] = (_m.mat[n][m] * _s);
   }

   //: Post multiply by a matrix
   //!POST: mat' = mat * m
   void postMult(const vjMatrix&  _m);

   //: Pre multiply by a matrix
   //!POST: mat' = m * mat
   void preMult(const vjMatrix&  _m);

   //: Find inverse of a matrix
   //!POST: mat' = inverse(_m)
   int invert(vjMatrix& _m);

public:
   // --- Transformation functions --- //

   //: Pre translate a matrix
   //!POST: mat' = trans(_x,_y,_z) * _m
   void preTrans(float _x, float _y, float _z, vjMatrix&  _m);

   //: Pre translate a matrix
   //!POST: mat' = trans(_x,_y,_z) * _m
   void preTrans(vjVec3& _trans, vjMatrix&  _m);

   //!POST: mat' = _m * trans(_x,_y,_z)
   void postTrans(const vjMatrix&  _m, float _x, float _y, float _z);

   //!POST: mat' = _m * trans(_x,_y,_z)
   void postTrans(const vjMatrix&  _m, vjVec3& _trans);

   //!POST: mat' = rot(_degrees, axis) * _m
   void preRot(float _degrees, vjVec3& axis, vjMatrix&  _m);

   //!POST: mat' = _m * rot(_degrees, axis)
   void postRot(const vjMatrix&  _m, float _degrees, vjVec3& axis);

   void preXYZEuler(float x, float y, float z, vjMatrix& _m);
   void postXYZEuler(vjMatrix& _m, float x, float y, float z);

   //!POST: mat' = scale(_xs,_ys,_zs) * _m;
   void preScale(float _xs, float _ys, float _zs, vjMatrix&  _m);

   //!POST: mat' = _m * scale(_xs,_ys,_zs)
   void postScale(const vjMatrix&  _m, float _xs, float _ys, float _zs);

public:
   //: Get a float pointer to the matrix data
   //!RETVAL: Returns a ptr to the head of the matrix data
   float*   getFloatPtr() { return (float*)mat;}

   // Operators
   /// This accesses in C/C++ way???
   float*       operator [](int i)        { return &mat[i][0];}
   const float* operator [](int i) const  { return &mat[i][0];}
   float    operator ()(int i, int j) { return mat[j][i];}

   int operator == (const vjMatrix&  _m) const {
      return this->equal(_m);
   }

   int operator != (const vjMatrix&  _m) const {
      return !this->equal(_m);
   }

public:
   // vjMatrix operators (N.B. return by value can be quite slow)
   vjMatrix operator *(const vjMatrix&  _m) const {
      vjMatrix dst; dst.mult(*this, _m); return dst;
   }

   vjMatrix operator +(const vjMatrix&  _m) const {
      vjMatrix dst; dst.add(*this, _m); return dst;
   }
   vjMatrix operator -(const vjMatrix&  _m) const {
      vjMatrix dst; dst.sub(*this, _m); return dst;
   }

   friend inline vjMatrix operator *(float _s, const vjMatrix&);
   friend inline vjMatrix operator *(const vjMatrix& _v, float _s);
   friend inline vjMatrix operator /(const vjMatrix& _v, float _s);
   friend ostream& operator <<(ostream& out, vjMatrix& _mat);

public:
   // Assignment operators
   vjMatrix&  operator =(const vjMatrix&  _m)
   {
      ((mat)[0][0] = (_m)[0][0]); ((mat)[0][1] = (_m)[0][1]); ((mat)[0][2] = (_m)[0][2]); ((mat)[0][3] = (_m)[0][3]);
      ((mat)[1][0] = (_m)[1][0]); ((mat)[1][1] = (_m)[1][1]); ((mat)[1][2] = (_m)[1][2]); ((mat)[1][3] = (_m)[1][3]);
      ((mat)[2][0] = (_m)[2][0]); ((mat)[2][1] = (_m)[2][1]); ((mat)[2][2] = (_m)[2][2]); ((mat)[2][3] = (_m)[2][3]);
      ((mat)[3][0] = (_m)[3][0]); ((mat)[3][1] = (_m)[3][1]); ((mat)[3][2] = (_m)[3][2]); ((mat)[3][3] = (_m)[3][3]);
      return *this;
   }

   vjMatrix&  operator *=(const vjMatrix&  _m)  {
      this->postMult(_m); return *this;
   }

   vjMatrix&  operator *=(float _s)
   {
      for (int i=0; i<4; i++)
         for (int j=0; j<4; j++)
            mat[i][j] = mat[i][j] * _s;

      return *this;
   }

   //vjMatrix&  operator /=(float _s);
   //vjMatrix&  operator +=(const vjMatrix&  _m);
   //vjMatrix&  operator -=(const vjMatrix&  _m);
};

#endif
