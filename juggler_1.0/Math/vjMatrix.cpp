/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>

#include <Kernel/vjDebug.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec3.h>
#include <Math/vjCoord.h>
#include <Math/vjQuat.h>

// Clamps an angle that is a cos or sin value [-1,1]
static inline void limitAngle(float& angle)
{
   if(angle > 1.0f)
      angle = 1.0f;
   else if(angle < -1.0f)
      angle = -1.0f;
}

// Clamp an angle to zero if it is close
static inline void zeroClampAngle(float& angle)
{
   if(fabs(angle) < 1e-6)
      angle = 0.0f;
}

vjMatrix::vjMatrix(vjCoord coord)
{
   makeXYZEuler(coord.orient[0], coord.orient[1], coord.orient[2]);
   setTrans(coord.pos[0], coord.pos[1], coord.pos[2]);
}

void vjMatrix::makeXYZEuler(float xRot, float yRot, float zRot)
{
   float sx = sin(VJ_DEG2RAD(xRot));  float cx = cos(VJ_DEG2RAD(xRot));
   float sy = sin(VJ_DEG2RAD(yRot));  float cy = cos(VJ_DEG2RAD(yRot));
   float sz = sin(VJ_DEG2RAD(zRot));  float cz = cos(VJ_DEG2RAD(zRot));

   // Derived by simply multiplying out the matrices by hand
   // X*Y*Z
   mat[0][0] = cy*cz;              mat[1][0] = -cy*sz;              mat[2][0] = sy;      mat[3][0] = 0.0f;
   mat[0][1] = sx*sy*cz + cx*sz;   mat[1][1] = -sx*sy*sz + cx*cz;   mat[2][1] = -sx*cy;  mat[3][1] = 0.0f;
   mat[0][2] = -cx*sy*cz + sx*sz;  mat[1][2] = cx*sy*sz + sx*cz;    mat[2][2] = cx*cy;   mat[3][2] = 0.0f;
   mat[0][3] = 0.0f;               mat[1][3] = 0.0f;                mat[2][3] = 0.0f;    mat[3][3] = 1.0f;

   zeroClamp();     // Clamp ~ zero values
}

void vjMatrix::getXYZEuler(float& xRot, float& yRot, float& zRot)
{
   float cz;

   zRot = atan2f(-mat[1][0], mat[0][0]);     // -(-cy*sz)/(cy*cz) - cy falls out
   xRot = atan2f(-mat[2][1], mat[2][2]);     // -(sx*cy)/(cx*cy) - cy falls out
   cz = cos(zRot);
   yRot = atan2f(mat[2][0], mat[0][0]/cz);   // (sy)/((cy*cz)/cz)

   xRot = VJ_RAD2DEG(xRot);
   yRot = VJ_RAD2DEG(yRot);
   zRot = VJ_RAD2DEG(zRot);
}

// ------------------------------------------------------------------- //

void vjMatrix::makeZYXEuler(float zRot, float yRot, float xRot)
{
   float sx = sin(VJ_DEG2RAD(xRot));  float cx = cos(VJ_DEG2RAD(xRot));
   float sy = sin(VJ_DEG2RAD(yRot));  float cy = cos(VJ_DEG2RAD(yRot));
   float sz = sin(VJ_DEG2RAD(zRot));  float cz = cos(VJ_DEG2RAD(zRot));

   // Derived by simply multiplying out the matrices by hand
   // Z*Y*Z
   mat[0][0] = cy*cz;      mat[1][0] = -cx*sz + sx*sy*cz;   mat[2][0] = sx*sz + cx*sy*cz;    mat[3][0] = 0.0f;
   mat[0][1] = cy*sz;      mat[1][1] = cx*cz + sx*sy*sz;    mat[2][1] = -sx*cz + cx*sy*sz;   mat[3][1] = 0.0f;
   mat[0][2] = -sy;        mat[1][2] = sx*cy;               mat[2][2] = cx*cy;               mat[3][2] = 0.0f;
   mat[0][3] = 0.0f;       mat[1][3] = 0.0f;                mat[2][3] = 0.0f;                mat[3][3] = 1.0f;

   zeroClamp();     // Clamp ~ zero values
}

void vjMatrix::getZYXEuler(float& zRot, float& yRot, float& xRot)
{
   float sx;

   zRot = atan2f(mat[0][1], mat[0][0]);      // (cy*sz)/(cy*cz) - cy falls out
   xRot = atan2f(mat[1][2], mat[2][2]);      // (sx*cy)/(cx*cy) - cy falls out
   sx = sinf(xRot);
   yRot = atan2f(-mat[0][2],(mat[1][2]/sx) );   // -(-sy)/((sx*cy)/sx)

   xRot = VJ_RAD2DEG(xRot);
   yRot = VJ_RAD2DEG(yRot);
   zRot = VJ_RAD2DEG(zRot);
}

// -------------------------------------------------------------- //

void vjMatrix::makeZXYEuler(float zRot, float xRot, float yRot)
{
   float sx = sin(VJ_DEG2RAD(xRot));  float cx = cos(VJ_DEG2RAD(xRot));
   float sy = sin(VJ_DEG2RAD(yRot));  float cy = cos(VJ_DEG2RAD(yRot));
   float sz = sin(VJ_DEG2RAD(zRot));  float cz = cos(VJ_DEG2RAD(zRot));

   // Derived by simply multiplying out the matrices by hand
   // Z*X*Y
   mat[0][0] = cy*cz - sx*sy*sz;    mat[1][0] = -cx*sz;     mat[2][0] = sy*cz + sx*cy*sz;    mat[3][0] = 0.0f;
   mat[0][1] = cy*sz + sx*sy*cz;    mat[1][1] = cx*cz;      mat[2][1] = sy*sz - sx*cy*cz;    mat[3][1] = 0.0f;
   mat[0][2] = -cx*sy;              mat[1][2] = sx;         mat[2][2] = cx*cy;               mat[3][2] = 0.0f;
   mat[0][3] = 0.0f;                mat[1][3] = 0.0f;       mat[2][3] = 0.0f;               mat[3][3] = 1.0f;

   zeroClamp();     // Clamp ~ zero values
}

void vjMatrix::getZXYEuler(float& zRot, float& xRot, float& yRot)
{
   // Extract the rotation directly fromt he matrix
   float x_angle;
   float y_angle;
   float z_angle;
   float cos_y, sin_y;
   float cos_x, sin_x;
   float cos_z, sin_z;

   sin_x = mat[1][2];
   x_angle = asinf(sin_x);     // Get x angle
   cos_x = cos(x_angle);
   zeroClampAngle(cos_x);          // Clamp it to get a good test

   // Check if cos_x = Zero
   if(cos_x != 0.0f)     // ASSERT: cos_x != 0
   {
         // Get y Angle
      cos_y = mat[2][2]/cos_x;
      sin_y = -mat[0][2]/cos_x;
      y_angle = atan2(cos_y, sin_y);

         // Get z Angle
      cos_z = mat[1][1]/cos_x;
      sin_z = -mat[1][0]/cos_x;
      z_angle = atan2(cos_z, sin_z);
   }
   else
   {
      // Arbitrarily set z_angle = 0
      z_angle = 0;

         // Get y Angle
      cos_y = mat[0][0];
      sin_y = mat[0][1];
      y_angle = atan2(cos_y, sin_y);
   }

   xRot = VJ_RAD2DEG(x_angle);
   yRot = VJ_RAD2DEG(y_angle);
   zRot = VJ_RAD2DEG(z_angle);
}

// --------------------------------------------------------------------------- //

void vjMatrix::makeDirCos(vjVec3 secXAxis, vjVec3 secYAxis, vjVec3 secZAxis)
{
   vjASSERT(secXAxis.isNormalized());
   vjASSERT(secYAxis.isNormalized());
   vjASSERT(secZAxis.isNormalized());

   float Xa, Xb, Xy;    // Direction cosines of the secondary x-axis
   float Ya, Yb, Yy;    // Direction cosines of the secondary y-axis
   float Za, Zb, Zy;    // Direction cosines of the secondary z-axis

   vjVec3 xAxis(1,0,0), yAxis(0,1,0), zAxis(0,0,1);   // The base axis to use

   Xa = secXAxis.dot(xAxis);  Xb = secXAxis.dot(yAxis);  Xy = secXAxis.dot(zAxis);
   Ya = secYAxis.dot(xAxis);  Yb = secYAxis.dot(yAxis);  Yy = secYAxis.dot(zAxis);
   Za = secZAxis.dot(xAxis);  Zb = secZAxis.dot(yAxis);  Zy = secZAxis.dot(zAxis);

   // Set the matrix correctly
   set(Xa, Xb, Xy, 0,
       Ya, Yb, Yy, 0,
       Za, Zb, Zy, 0,
       0,  0,  0, 1);
}

// From Watt & Watt
void    vjMatrix::makeQuaternion(float* q)
{
   float wx, wy, wz, xx, yy, yz, xy, xz, zz, xs, ys, zs;
   //float s;

   //s = 2.0f/(q[VJ_X]*q[VJ_X] + q[VJ_Y]*q[VJ_Y] + q[VJ_Z]*q[VJ_Z] + q[VJ_W]*q[VJ_W]);

   xs = q[VJ_X] + q[VJ_X];    ys = q[VJ_Y] + q[VJ_Y];    zs = q[VJ_Z] + q[VJ_Z];
   xx = q[VJ_X] * xs;   xy = q[VJ_X] * ys;   xz = q[VJ_X] * zs;
   yy = q[VJ_Y] * ys;   yz = q[VJ_Y] * zs;   zz = q[VJ_Z] * zs;
   wx = q[VJ_W] * xs;   wy = q[VJ_W] * ys;   wz = q[VJ_W] * zs;

   mat[0][0] = 1.0 - (yy+zz);
    mat[0][1] = xy+wz;
    mat[0][2] = xz-wy;
    mat[0][3] = 0.0;

    mat[1][0] = xy-wz;
    mat[1][1] = 1.0 - (xx+zz);
    mat[1][2] = yz+wx;
    mat[1][3] = 0.0;

    mat[2][0] = xz+wy;
    mat[2][1] = yz-wx;
    mat[2][2] = 1.0 - (xx+yy);
    mat[2][3] = 0.0;

    mat[3][0] = 0.0;
    mat[3][1] = 0.0;
    mat[3][2] = 0.0;
    mat[3][3] = 1.0;
}

void vjMatrix::makeQuaternion(vjQuat& q)
{makeQuaternion(q.vec);}

void	vjMatrix::makeRot(float _degrees, vjVec3 _axis)
{
    _axis.normalize();	// NOTE: This could be eliminated by passing normalized
			    //       vector.  This could be by ref to make even faster
    // GGI: pg 466
    float s = sin(VJ_DEG2RAD(_degrees));
    float c = cos(VJ_DEG2RAD(_degrees));
    float t = 1-c;
    float x = _axis[0];
    float y = _axis[1];
    float z = _axis[2];

    /*
    mat[0][0] = (t*x*x)+c;     mat[1][0] = (t*x*y)+(s*z); mat[2][0] = (t*x*z)-(s*y); mat[3][0] = 0.0f;
    mat[0][1] = (t*x*y)-(s*z); mat[1][1] = (t*y*y)+c;     mat[2][1] = (t*y*z)+(s*x); mat[3][1] = 0.0f;
    mat[0][2] = (t*x*z)+(s*y); mat[1][2] = (t*y*z)-(s*x); mat[2][2] = (t*z*z)+c;     mat[3][2] = 0.0f;
    mat[0][3] = 0.0f;          mat[1][3] = 0.0f;          mat[2][3] = 0.0f;          mat[3][3] = 1.0f;
    */

    /* From: Introduction to robotic.  Craig.  Pg. 52 */
    mat[0][0] = (t*x*x)+c;     mat[1][0] = (t*x*y)-(s*z); mat[2][0] = (t*x*z)+(s*y); mat[3][0] = 0.0f;
    mat[0][1] = (t*x*y)+(s*z); mat[1][1] = (t*y*y)+c;     mat[2][1] = (t*y*z)-(s*x); mat[3][1] = 0.0f;
    mat[0][2] = (t*x*z)-(s*y); mat[1][2] = (t*y*z)+(s*x); mat[2][2] = (t*z*z)+c;     mat[3][2] = 0.0f;
    mat[0][3] = 0.0f;          mat[1][3] = 0.0f;          mat[2][3] = 0.0f;          mat[3][3] = 1.0f;

    zeroClamp();     // Clamp ~ zero values
}

void  vjMatrix::makeTrans(float _x, float _y, float _z)
{
   makeIdent();
   mat[3][0] = _x;
   mat[3][1] = _y;
   mat[3][2] = _z;
}

void vjMatrix::setTrans(float _x, float _y, float _z)
{
   mat[3][0] = _x;
   mat[3][1] = _y;
   mat[3][2] = _z;
}

void vjMatrix::getTrans(float& _x, float& _y, float& _z)
{
   _x = mat[3][0];
   _y = mat[3][1];
   _z = mat[3][2];
}

void	vjMatrix::makeScale(float _x, float _y, float _z)
{
    makeIdent();
    mat[0][0] = _x;
    mat[1][1] = _y;
    mat[2][2] = _z;
}

void vjMatrix::preTrans(float _x, float _y, float _z, vjMatrix&  _m)
{
    vjMatrix transMat;
    transMat.makeTrans(_x, _y, _z);
    transMat.postMult(_m);
    *this = transMat;	
}

//: Pre translate a matrix
//!POST: mat' = trans(_x,_y,_z) * _m
void vjMatrix::preTrans(vjVec3& _trans, vjMatrix&  _m)
{ preTrans(_trans.vec[0], _trans.vec[1], _trans.vec[2], _m); }


    /// mat = _m * trans(_x,_y,_z)
void vjMatrix::postTrans(const vjMatrix&  _m, float _x, float _y, float _z)
{
    vjMatrix transMat;
    transMat.makeTrans(_x, _y, _z);
    transMat.preMult(_m);
    *this = transMat;
}

//!POST: mat' = _m * trans(_x,_y,_z)
void vjMatrix::postTrans(const vjMatrix&  _m, vjVec3& _trans)
{ postTrans(_m, _trans.vec[0], _trans.vec[1], _trans.vec[2]); }

void vjMatrix::preRot(float _degrees, vjVec3& axis, vjMatrix&  _m)
{
    vjMatrix rotMat;
    rotMat.makeRot(_degrees, axis);
    rotMat.postMult(_m);
    *this = rotMat;	
}

void vjMatrix::postRot(const vjMatrix&  _m, float _degrees, vjVec3& axis)
{
    vjMatrix rotMat;
    rotMat.makeRot(_degrees, axis);
    rotMat.preMult(_m);
    *this = rotMat;
}

void vjMatrix::preXYZEuler(float x, float y, float z, vjMatrix& _m)
{
    vjMatrix rotMat;
    rotMat.makeXYZEuler(x, y, z);
    rotMat.postMult(_m);
    *this = rotMat;
}

void vjMatrix::postXYZEuler(vjMatrix& _m, float x, float y, float z)
{
    vjMatrix rotMat;
    rotMat.makeXYZEuler(x, y, z);
    rotMat.preMult(_m);
    *this = rotMat;
}

    /// mat = scale(_xs,_ys,_zs) * _m;
void vjMatrix::preScale(float _xs, float _ys, float _zs, vjMatrix&  _m)
{
    vjMatrix scaleMat;
    scaleMat.makeScale(_xs, _ys, _zs);
    scaleMat.postMult(_m);
    *this = scaleMat;
}
    /// mat = _m * scale(_xs,_ys,_zs)
void vjMatrix::postScale(const vjMatrix&  _m, float _xs, float _ys, float _zs)
{
    vjMatrix scaleMat;
    scaleMat.makeScale(_xs, _ys, _zs);
    scaleMat.preMult(_m);
    *this = scaleMat;
}

inline vjMatrix operator *(float _s, const vjMatrix& _m) {
    vjMatrix dst; dst.scale(_s, _m); return dst;
}

inline vjMatrix operator *(const vjMatrix& _m, float _s) {
    vjMatrix dst; dst.scale(_s, _m); return dst;
}

inline vjMatrix operator /(const vjMatrix& _m, float _s) {
    vjMatrix dst; dst.scale(1.0f/_s, _m); return dst;
}

    // Matrix Multiplication of A:(nxl) B:(lxm) ==> C:(nxm)
    //   Cij = Sum(k=1,l) (Aik)(Bkj)
    /// mat = mat * m
    // NOTE: All accesses in the function are using the C/C++ indexing method
void vjMatrix::postMult(const vjMatrix&  _m)
{
    vjMatrix prevMat = *this;	    // May be sloooow!!!
    zero();

    for(int i=0;i<4;i++)
	for(int j=0;j<4;j++)
	    for(int k=0;k<4;k++)
		mat[j][i] += ( prevMat.mat[k][i] * _m.mat[j][k]);
}

    // mat = m * mat
void vjMatrix::preMult(const vjMatrix&  _m)
{
    vjMatrix prevMat = *this;	    // May be sloooow!!!
    zero();

    for(int i=0;i<4;i++)
	for(int j=0;j<4;j++)
	    for(int k=0;k<4;k++)
		mat[i][j] += ( prevMat.mat[i][k] * _m.mat[k][j]);;
}


/*---------------------------------------------------------------------------*
 | mat_inv: Compute the inverse of a n x n matrix, using the maximum pivot   |
 |          strategy.  n <= MAX1.                                            |
 *---------------------------------------------------------------------------*

   Parameters:
       a        a n x n square matrix
       b        inverse of input a.
       n        dimenstion of matrix a.
*/
//void mat_inv( a, b, n )
//double  a[], b[];
//int     n;

    /// Set mat = inverse(_m)
int vjMatrix::invert(vjMatrix& _m)

{
        float*  a = _m.getFloatPtr();
	float*  b = getFloatPtr();
	
	int	n = 4;
	//int      is, js;      // Was never referenced
   int     i, j, k;
        int     r[ 4], c[ 4], row[ 4], col[ 4];
        float  m[ 4][ 4*2], pivot, max_m, tmp_m, fac;

	
        /* Initialization */
        for ( i = 0; i < n; i ++ )
        {
                r[ i] = c[ i] = 0;
                row[ i] = col[ i] = 0;
        }

        /* Set working matrix */
        for ( i = 0; i < n; i++ )
        {
                for ( j = 0; j < n; j++ )
                {
                        m[ i][ j] = a[ i * n + j];
                        m[ i][ j + n] = ( i == j ) ? 1.0 : 0.0 ;
                }
        }

        /* Begin of loop */
        for ( k = 0; k < n; k++ )
        {
                /* Choosing the pivot */
                for ( i = 0, max_m = 0; i < n; i++ )
                {
                        if ( row[ i]  )      continue;
                        for ( j = 0; j < n; j++ )
                        {
                                if ( col[ j] )          continue;
                                tmp_m = fabs( m[ i][ j]);
                                if ( tmp_m > max_m)
                                {
                                        max_m = tmp_m;
                                        r[ k] = i;
                                        c[ k] = j;
                                }
                        }
                }
                row[ r[k] ] = col[ c[k] ] = 1;
                pivot = m[ r[ k] ][ c[ k] ];


                if ( fabs( pivot) <= 1e-20)
                {
                        cerr << "*** pivot = %f in mat_inv. ***\n";
                        //exit( 0);
			return -1;
                }

                /* Normalization */
                for ( j = 0; j < 2*n; j++ )
                {
                        if ( j == c[ k] )
                                m[ r[ k]][ j] = 1.0;
                        else
                                m[ r[ k]][ j] /= pivot;
                }

                /* Reduction */
                for ( i = 0; i < n; i++ )
                {
                        if ( i == r[ k] )
                                continue;

                        for ( j=0, fac = m[ i][ c[k]]; j < 2*n; j++ )
                        {
                                if ( j == c[ k] )
                                        m[ i][ j] = 0.0;
                                else
                                        m[ i][ j] -= fac * m[ r[k]][ j];
                        }
                }
        }

        /* Assign invers to a matrix */
        for ( i = 0; i < n; i++ )
                for ( j = 0; j < n; j++ )
                        row[ i] = ( c[ j] == i ) ? r[ j] : row[ i];

        for ( i = 0; i < n; i++ )
                for ( j = 0; j < n; j++ )
                        b[ i * n +  j] = m[ row[ i]][ j + n];
			
	return 1;   // It worked
}

	// ---- FRIEND FUNCTIONS ---- //
ostream& operator<<(ostream& out, vjMatrix& _mat)
{
   for(int j=0;j<4;j++)
   {
      for(int i=0;i<4;i++)
         out << _mat.mat[i][j] << " ";
      out << endl;
   }

   return out;	
}

