#ifndef _VRJ_QUAT_H_
#define _VRJ_QUAT_H_

#include <gmtl/Quat.h>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>

namespace vrj
{

class Quat : public gmtl::Quatf
{
public:

   //Constructor
   Quat()
   {
   }

   Quat( const Quat& q )
   {
      *this = q;
   }

   Quat( const gmtl::Quatf& q )
   {
      *this = q;
   }

   ~Quat()
   {
   }

   //Construct quat from matrix
   inline void makeRot( const gmtl::Matrix44f& m )
   {
      gmtl::setRot( *this, m );
   }

   //make a quat from a twist (radians) about a vector (normalized)        
   inline void makeRot( const float& rad, const float& x, const float& y, const float& z )
   {
      gmtl::setRot( *this, gmtl::AxisAnglef(rad, gmtl::Vec3f(x,y,z)) );
   }

   inline void makeRot( const float& rad, const gmtl::Vec3f& axis ) //new
   {
      gmtl::setRot( *this, gmtl::AxisAnglef(rad, axis) );
   }

   //make a quat from the shortest arc to rotate between 2 vectors (NEW)
   inline void makeRot( const gmtl::Vec3f& from, const gmtl::Vec3f& to )
   {
//      gmtl::setRot( *this, from, to );
      *this = gmtl::makeRot<gmtl::Quatf>( from, to );
   }   

   //get the quat's twist (radians) and vector
   inline void getRot( float& rad, float& x, float& y, float& z ) const
   {
      gmtl::AxisAnglef rot;
      gmtl::setRot( rot, *this );
      rad = rot[0]; x = rot[1]; y = rot[2]; z = rot[3];
   }

   inline void getRot( float& rad, gmtl::Vec3f& axis ) const //new
   {
      gmtl::AxisAnglef rot;
      gmtl::setRot( rot, *this );
      rad = rot[0]; axis = gmtl::Vec3f( rot[1], rot[2], rot[3] );
   }

   //set to conj of quat
   inline void conj(const Quat& q)
   {
      *this = q;
      gmtl::conj( *this );
   }

   inline void conj() //new
   {
      gmtl::conj( *this );
   }

   //Copied from vjQuat.h - no idea what this does
   inline float norm() const
   {
      return ((mData[0]*mData[0])+(mData[1]*mData[1])+(mData[2]*mData[2])+(mData[3]*mData[3]));
   }

   inline void normalize() //New
   {
      gmtl::normalize( *this );
   }

   inline void normalizeFast()
   {
      gmtl::normalize( *this );
   }

   //Multiply two quaternions
   inline void mult(const Quat& q1, const Quat& q2)
   {
      *this = q1 * q2 ;
   }

   inline void mult(const Quat& q) //New
   {
      *this *= q ;
   }

   //Divide two quaternions
   inline void div(const Quat& q1, const Quat& q2)
   {
      gmtl::div( *this, q1, q2 );
   }

   inline void div(const Quat& q) //New
   {
      gmtl::div( *this, *this, q );
   }

   inline void invert(const Quat& q)
   {
      *this = q;
      gmtl::invert( *this );
   }

   inline void invert() //New
   {
      gmtl::invert( *this );
   }


   inline void slerp(float t, const Quat& q1, const Quat& q2)
   {
      gmtl::slerp( *this, t, q1, q2 );
   }

   inline void slerp(float t, const Quat& q) //New
   {
      gmtl::slerp( *this, t, *this, q );
   }

   inline void squad(float t, const Quat& q1, const Quat& q2, const Quat& a, const Quat& b)
   {
      gmtl::squad( *this, t, q1, q2, a, b );
   }

   //New copy operators to go from gmtl::Quatf to vrj::Quat
   inline Quat& operator=( const gmtl::Quatf& q )
   {
      for (int i=0; i < 4; i++)
         mData[i] = q.mData[i];

      return *this;
   }

}; //End of class

}; //End of namespace

#endif
