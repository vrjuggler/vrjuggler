#ifndef AJ_MAT_VEC_FUNCS
#define AJ_MAT_VEC_FUNCS

#include "aj/ajMath.h"
#include "aj/ajMatrix44.h"
#include "aj/ajVec3.h"

namespace ajMath
{

// result[4] = mat * vec[4]
inline void xform( float* result, const ajMatrix44& mat, const float* vec )
{
   for (int j = 0; j < 4; ++j)
	   for (int k = 0; k < 4; ++k)
		   result[j] += vec[k] * mat( k, j );
}

inline ajVec3 xformPoint( const ajMatrix44& mat, const ajVec3& vec )
{
   float vec4[4] = { vec[0], vec[1], vec[2], 1.0f };
   float result[4];
   xform( result, mat, vec4 );
   float w_inv = 1.0f / result[3];
   return ajVec3( result[0] * w_inv, result[1] * w_inv, result[2] * w_inv );
}

inline ajVec3 xformVec( const ajMatrix44& mat, const ajVec3& vec )
{
   float vec4[4] = { vec[0], vec[1], vec[2], 0.0f };
   float result[4];
   xform( result, mat, vec4 );
   return ajVec3( result[0], result[1], result[2] );
}

inline ajVec3 operator+( const ajVec3& vec1, const ajVec3& vec2 )
{
   ajVec3 v( vec1[0] + vec2[0],
             vec1[1] + vec2[1],
             vec1[2] + vec2[2] );
   return v;
}

inline ajVec3 operator-( const ajVec3& vec1, const ajVec3& vec2 )
{
   ajVec3 v( vec1[0] - vec2[0],
             vec1[1] - vec2[1],
             vec1[2] - vec2[2] );
   return v;
}
inline ajVec3 operator*( const ajVec3& vec, const float& scalar )
{
   ajVec3 v( vec[0] * scalar,
             vec[1] * scalar,
             vec[2] * scalar );
   return v;
}

// Linear Interpolation between two vectors.
inline void Lerp(const ajVec3& from, 
      const ajVec3& to, 
      const float &lerp, 
      ajVec3& lerpedValue )
{
    ajVec3 offset = to - from;
    lerpedValue = from + offset*lerp;
}

};

#endif
