
#include "ajMath.h"
#include "ajMatrix44.h"
#include "ajVec3.h"

void xform( float* result, const ajMatrix44& mat, const float* vec )
{
   // result = mat * vec4
   float result[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
   for (int j = 0; j < 4; ++j)
	   for (int k = 0; k < 4; ++k)
		   result[j] += vec[k] * mat( k, j );
}

ajVec3 xformPoint( const ajMatrix44& mat, const ajVec3& vec )
{
   float vec4[4] = { vec[0], vec[1], vec[2], 1.0f };
   float result[4];
   xform( result, mat, vec4 );
   float w_inv = 1.0f / result[3];
   return ajVec3( result[0] * w_inv, result[1] * w_inv, result[2] * w_inv );
}

ajVec3 xformVec( const ajMatrix44& mat, const ajVec3& vec )
{
   float vec4[4] = { vec[0], vec[1], vec[2], 0.0f };
   float result[4];
   xform( result, mat, vec4 );
   return ajVec3( result[0], result[1], result[2] );
}

ajVec3 operator+( const ajVec3& vec1, const ajVec3& vec2 )
{
   ajVec3 v( vec1[0] + vec2[0],
             vec1[1] + vec2[1],
             vec1[2] + vec2[2] );
   return v;
}
ajVec3 operator-( const ajVec3& vec1, const ajVec3& vec2 )
{
   ajVec3 v( vec1[0] - vec2[0],
             vec1[1] - vec2[1],
             vec1[2] - vec2[2] );
   return v;
}

// Linear Interpolation between two vectors.
inline void ajVec3::Lerp(const ajVec3& from, 
      const ajVec3& to, 
      const float &lerp, 
      ajVec3& lerpedValue )
{
    ajVec3 offset = to - from;
    lerpedValue = from + offset*lerp;
}
