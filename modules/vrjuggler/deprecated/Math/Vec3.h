#ifndef _VRJ_VEC3_H_
#define _VRJ_VEC3_H_

#include <gmtl/VecBase.h>
#include <gmtl/Point.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>

namespace vrj
{

class Vec3 : public gmtl::Vec3f
{

public:

   //Default constructor
   Vec3()
   {
      mData[0] = mData[1] = mData[2] = 0.0f;
   }

   Vec3(float x, float y, float z )
   {
      mData[0] = x;
      mData[1] = y;
      mData[2] = z;
   }

   Vec3(const gmtl::Vec3f& v)
   {
      mData[0] = v[0];
      mData[1] = v[1];
      mData[2] = v[2];
   }

   ~Vec3()
   {
   }

   //Implements all of the functions in the original Juggler 1.0.x vjVec3 class

   inline void set(float x, float y, float z)       //Set the element values
   {
      mData[0] = x;
      mData[1] = y;
      mData[2] = z;
   }

   inline bool equal(const Vec3& v) const           //Test for equality
   {
      return ( *this == v );
   }

   inline float normalize()                         //Normalize the vector, return old length
   {
      return gmtl::normalize( *this );
   }

   inline bool isNormalized() const                 //Test for vector normalization
   {
      return gmtl::isNormalized( *this ); 
   }

   inline float lengthSquared() const               //Compute the length squared
   {
      return gmtl::lengthSquared( *this );
   }

   inline float length() const                      //Compute the length
   {
      return gmtl::length( *this );
   }

   inline float dot(const Vec3& v) const            //Compute the dot product
   {
      return gmtl::dot( *this, v );
   }

   inline Vec3 cross(const Vec3& v) const           //Calculate the cross product vector
   {
      return Vec3( gmtl::makeCross( *this, v ) );
   }

   inline void lerp(float n, const Vec3& a, const Vec3& b)  //Linear interpolation
   {                                                        //between vec's a and b
      gmtl::lerp( *this, n, a, b );
   }


   //Note that when implementing this function, GMTL will need a point
   //rather than a vector
   //Set the vector to be transformed by a full 4x4 matrix
   inline void xformFull(const gmtl::Matrix44f& m)  //new 
   {
      gmtl::Point3f temp;
      gmtl::xform( temp, m, gmtl::Point3f(mData[0], mData[1], mData[2]) ) ;
      mData[0] = temp[0]; mData[1] = temp[1]; mData[2] = temp[2];
   }

   inline void xformFull(const gmtl::Matrix44f& m, const Vec3& v)
   {
      gmtl::Point3f temp;
      gmtl::xform( temp, m, gmtl::Point3f(v[0], v[1], v[2]) );
      mData[0] = temp[0]; mData[1] = temp[1]; mData[2] = temp[2];
   } 

   inline void xformVec(const gmtl::Matrix44f& m, const Vec3& v)  //Transform as a vector
   {
      gmtl::xform( *this, m, v );
   }

   inline void xformVec(const gmtl::Matrix44f& m) //new
   {
      gmtl::xform( *this, m, *this );
   }

   
}; //End of class

}; //End of namespace

#endif
