
#ifndef VEC3__INCLUDED
#define VEC3__INCLUDED

#include "aj/Math.h"

namespace aj
{

class Vec3
{
public:
   Vec3(){ mData[0] = mData[1] = mData[2] = 0.0f; }
   Vec3( const Vec3& vec );
   Vec3( const float& v0, const float& v1, const float& v2 );

   void            copy( const Vec3& vec );
   void            set( const float* vec );
   void            set( const float& v0, const float& v1, const float& v2 );
   void            get( float &v0, float &v1, float &v2 ) const;

   Vec3            cross( const Vec3& vec ) const;
   float             dot( const Vec3& vec ) const;
   float             lengthSquared() const;
   float             length() const;
   void              normalize();
   
   Vec3&             operator*=( const float& value );
   Vec3&             operator/=( const float& value );
   Vec3&             operator+=( const Vec3& vec );
   Vec3&             operator-=( const Vec3& vec );
   Vec3&             operator=( const Vec3& vec );
   void                negate();

   inline float*             data() { return mData; }
   inline const float*       data() const { return mData; }
   
   inline float &            operator[]( int i ) { return mData[i]; }
   inline const float &      operator[]( int i ) const { return mData[i]; }
   
protected:
   float mData[3];
};





//  Set the value.
inline void Vec3::set( const float& v0, const float& v1, const float& v2 )
{
   mData[0] = v0;
   mData[1] = v1;
   mData[2] = v2;
}

//  Set the value.
inline void Vec3::set( const float* v )
{
   this->set( v[0], v[1], v[2] );
}

//  Get the value.
inline void Vec3::get( float &v0, float &v1, float &v2 ) const
{
   v0 = mData[0];
   v1 = mData[1];
   v2 = mData[2];
}

//  Set the value.
inline void Vec3::copy( const Vec3& vec )
{
   this->set( vec.mData );
}

//  Constructor.
inline Vec3::Vec3( const Vec3& vec )
{
   this->copy( vec );
}

//  Constructor.
inline Vec3::Vec3( const float& v0, const float& v1, const float& v2 )
{
   this->set( v0, v1, v2 );
}


//  Return the cross product.
inline Vec3 Vec3::cross( const Vec3& vec ) const
{
   return Vec3 ( mData[1] * vec[2] - mData[2] * vec[1],
                   mData[2] * vec[0] - mData[0] * vec[2],
                   mData[0] * vec[1] - mData[1] * vec[0] );
}

//  Return the dot product.
inline float Vec3::dot ( const Vec3& vec ) const
{
   return mData[0] * vec[0] +
          mData[1] * vec[1] +
          mData[2] * vec[2];
}

//  Return the length squared. (same as dot product)
inline float Vec3::lengthSquared() const
{
   return mData[0] * mData[0] + 
          mData[1] * mData[1] + 
          mData[2] * mData[2];
}

//  Return the length. (ajMath::SQRT of dot product)
inline float Vec3::length() const
{
   return ajMath::sqrt( this->lengthSquared() );
}

//  Normalize
inline void Vec3::normalize()
{
   float invLength = 1.0f / this->length();

   mData[0] *= invLength;
   mData[1] *= invLength;
   mData[2] *= invLength;
}

//  Multiply all the components by the value.
inline Vec3& Vec3::operator*=( const float& value )
{
   mData[0] *= value;
   mData[1] *= value;
   mData[2] *= value;

   return *this;
}

//  Divide all the components by the value.
inline Vec3& Vec3::operator/=( const float& value )
{
   float invValue = 1.0f / value;

   mData[0] *= invValue;
   mData[1] *= invValue;
   mData[2] *= invValue;

   return *this;
}

//  Add the vector to this one.
inline Vec3& Vec3::operator+=( const Vec3& vec )
{
   mData[0] += vec[0];
   mData[1] += vec[1];
   mData[2] += vec[2];

   return *this;
}

//  Subtract the vector from this one.
inline Vec3& Vec3::operator-=( const Vec3& vec )
{
   mData[0] -= vec[0];
   mData[1] -= vec[1];
   mData[2] -= vec[2];

   return *this;
}

//  Assign this vector.
inline Vec3& Vec3::operator=( const Vec3& vec )
{
   this->copy( vec );
   return *this;
}

//  make the vec it's negative (flip 180 about origin)
inline void Vec3::negate()
{
   mData[0] = -mData[0];
   mData[1] = -mData[1];
   mData[2] = -mData[2];
}

}; // end namespace

#endif
