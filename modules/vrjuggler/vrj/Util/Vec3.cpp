#include <vrj/Util/Vec3.h>

namespace vrj
{

//Default constructor
Vec3::Vec3()
{
   mData[0] = mData[1] = mData[2] = 0.0f;
}

//Set constructor
Vec3::Vec3(float x, float y, float z )
{
   mData[0] = x;
   mData[1] = y;
   mData[2] = z;
} 

//GMTL constructor
Vec3::Vec3(const gmtl::Vec3f& v)
{
   mData[0] = v[0];
   mData[1] = v[1];
   mData[2] = v[2];
}

//Destructor
Vec3::~Vec3()
{
}


//Implements all of the functions in the original Juggler 1.0.x vjVec3 class

//Set the element values
void Vec3::set(float x, float y, float z)
{
   mData[0] = x;
   mData[1] = y;
   mData[2] = z;
}

//Test for equality
bool Vec3::equal(const Vec3& v) const
{
   return ( *this == v );
}


//Normalize the vector, return old length
float Vec3::normalize()
{
   return gmtl::normalize( *this );
}

//Test for vector normalization
bool Vec3::isNormalized() const
{
   return gmtl::isNormalized( *this );
}

//Compute the length squared
float Vec3::lengthSquared() const
{
   return gmtl::lengthSquared( *this );
}

//Compute the length
float Vec3::length() const
{
   return gmtl::length( *this );
}

//Compute the dot product
float Vec3::dot(const Vec3& v) const
{
   return gmtl::dot( *this, v );
}

//Calculate the cross product vector
Vec3 Vec3::cross(const Vec3& v) const
{
   return Vec3( gmtl::cross( *this, v ) );
}

void Vec3::lerp(float n, const Vec3& a, const Vec3& b)
{
   gmtl::lerp( *this, n, a, b );
}

/*
//TODO: Note that when implementing this function, GMTL will need a point
//      rather than a vector
   void xformFull(const Matrix& m, const Vec3& v); //Set the vector to be transformed
   void xformFull(const Matrix& m);                //by a full 4x4 matrix

   void xformVec(const Matrix& m, const Vec3& v);  //Transform as a vector
   void xformVec(const Matrix& m);

   float& operator [](int i) const;          //Access to individual elements

   int operator ==(const Vec3& v) const;     //Test for equality
   int operator !=(const Vec3& v) const;     //Test for INequality

   Vec3 operator -() const;                  //Invert a vector

   Vec3 operator +(const Vec3& v) const;     //Add 2 vectors
   Vec3 operator -(const Vec3& v) const;     //Subtract 2 vectors

   void lerp(float n, const Vec3& a, const Vec3& b); //Linear interpolation
                                                     //between vec's a and b

   Vec3& operator =(const Vec3& v);          //Assignment operator

   Vec3& operator *=(float s);               //Multiply by scalar

   Vec3& operator /=(float s);               //Divide by scalar

   Vec3& operator +=(const Vec3& v);         //Add 2 vectors

   Vec3& operator -=(const Vec3& v);         //Subtract 2 vectors

*/

};
