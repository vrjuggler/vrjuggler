#ifndef _VRJ_VEC3_H_
#define _VRJ_VEC3_H_

#include <gmtl/VecBase.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Convert.h>

namespace vrj
{

class Vec3 : public gmtl::Vec3f
{

public:

   Vec3(); //Default constructor
   Vec3(float x, float y, float z );
   Vec3(const gmtl::Vec3f& v);
   ~Vec3();

   //Implements all of the functions in the original Juggler 1.0.x vjVec3 class

   void set(float x, float y, float z);      //Set the element values

   bool equal(const Vec3& v) const;          //Test for equality

   float normalize();                        //Normalize the vector, return old length

   bool isNormalized() const;                //Test for vector normalization

   float lengthSquared() const;              //Compute the length squared

   float length() const;                     //Compute the length

   float dot(const Vec3& v) const;           //Compute the dot product

   Vec3 cross(const Vec3& v) const;          //Calculate the cross product vector

   void lerp(float n, const Vec3& a, const Vec3& b); //Linear interpolation
                                                     //between vec's a and b

//TODO: Note that when implementing this function, GMTL will need a point
//      rather than a vector
/*
   void xformFull(const Matrix& m, const Vec3& v); //Set the vector to be transformed
   void xformFull(const Matrix& m);  //new              //by a full 4x4 matrix

   void xformVec(const Matrix& m, const Vec3& v);  //Transform as a vector
   void xformVec(const Matrix& m); //new
*/

//Inherited operators already exist so we will not reimplement them

/*
x   float& operator [](int i) const;          //Access to individual elements

x   int operator ==(const Vec3& v) const;     //Test for equality
x   int operator !=(const Vec3& v) const;     //Test for INequality

x   Vec3 operator -() const;                  //Invert a vector

x   Vec3 operator +(const Vec3& v) const;     //Add 2 vectors
x   Vec3 operator -(const Vec3& v) const;     //Subtract 2 vectors

x   Vec3& operator =(const Vec3& v);          //Assignment operator

x   Vec3& operator *=(float s);               //Multiply by scalar

x   Vec3& operator /=(float s);               //Divide by scalar

x   Vec3& operator +=(const Vec3& v);         //Add 2 vectors

x   Vec3& operator -=(const Vec3& v);         //Subtract 2 vectors
*/
   
}; //End of class

}; //End of namespace

#endif
