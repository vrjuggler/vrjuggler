#ifndef AJ_MAT_VEC_FUNCS
#define AJ_MAT_VEC_FUNCS

#include "aj/Math.h"
#include "aj/Matrix44.h"
#include "aj/Vec3.h"

namespace aj
{

// result[4] = mat * vec[4]
inline void xform( float* result, const aj::Matrix44& mat, const float* vec )
{
   for (int j = 0; j < 4; ++j)
	   for (int k = 0; k < 4; ++k)
		   result[j] += vec[k] * mat( k, j );
}

inline aj::Vec3 xformFull( const aj::Matrix44& mat, const aj::Vec3& vec )
{
   float vec4[4] = { vec[0], vec[1], vec[2], 1.0f };
   float result[4];
   xform( result, mat, vec4 );
   float w_inv = 1.0f / result[3];
   return aj::Vec3( result[0] * w_inv, result[1] * w_inv, result[2] * w_inv );
}

inline aj::Vec3 xformVec( const aj::Matrix44& mat, const aj::Vec3& vec )
{
   float vec4[4] = { vec[0], vec[1], vec[2], 0.0f };
   float result[4];
   xform( result, mat, vec4 );
   return aj::Vec3( result[0], result[1], result[2] );
}

inline aj::Vec3 operator+( const aj::Vec3& vec1, const aj::Vec3& vec2 )
{
   aj::Vec3 v( vec1[0] + vec2[0],
             vec1[1] + vec2[1],
             vec1[2] + vec2[2] );
   return v;
}

inline aj::Vec3 operator-( const aj::Vec3& vec1, const aj::Vec3& vec2 )
{
   aj::Vec3 v( vec1[0] - vec2[0],
             vec1[1] - vec2[1],
             vec1[2] - vec2[2] );
   return v;
}
inline aj::Vec3 operator*( const aj::Vec3& vec, const float& scalar )
{
   aj::Vec3 v( vec[0] * scalar,
             vec[1] * scalar,
             vec[2] * scalar );
   return v;
}

// Linear Interpolation between two vectors.
inline void Lerp(const aj::Vec3& from, 
      const aj::Vec3& to, 
      const float &lerp, 
      aj::Vec3& lerpedValue )
{
    aj::Vec3 offset = to - from;
    lerpedValue = from + offset*lerp;
}

};

#endif
