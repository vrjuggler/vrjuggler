/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <vpr/Util/Assert.h>
#include <vrj/Math/Matrix.h>
#include <vrj/Math/Vec3.h>
#include <vrj/Math/Coord.h>
#include <vrj/Math/Quat.h>
#include <vrj/Math/Math.h>

namespace vrj
{

Matrix::Matrix(Coord coord)
{
   makeXYZEuler(coord.orient[0], coord.orient[1], coord.orient[2]);
   setTrans(coord.pos[0], coord.pos[1], coord.pos[2]);
}

// args are in degrees
void Matrix::makeXYZEuler(float xRot, float yRot, float zRot)
{
   float sx = Math::sin(Math::deg2rad(xRot));  float cx = Math::cos(Math::deg2rad(xRot));
   float sy = Math::sin(Math::deg2rad(yRot));  float cy = Math::cos(Math::deg2rad(yRot));
   float sz = Math::sin(Math::deg2rad(zRot));  float cz = Math::cos(Math::deg2rad(zRot));

   // Derived by simply multiplying out the matrices by hand
   // X*Y*Z
   mat[0][0] = cy*cz;              mat[1][0] = -cy*sz;              mat[2][0] = sy;      mat[3][0] = 0.0f;
   mat[0][1] = sx*sy*cz + cx*sz;   mat[1][1] = -sx*sy*sz + cx*cz;   mat[2][1] = -sx*cy;  mat[3][1] = 0.0f;
   mat[0][2] = -cx*sy*cz + sx*sz;  mat[1][2] = cx*sy*sz + sx*cz;    mat[2][2] = cx*cy;   mat[3][2] = 0.0f;
   mat[0][3] = 0.0f;               mat[1][3] = 0.0f;                mat[2][3] = 0.0f;    mat[3][3] = 1.0f;

   zeroClamp();     // Clamp ~ zero values
}

// args are in degrees
// TODO: degrees are more inefficient (cause you have to convert),
//       which is why most math libs standardize on radians.
//       think about standardizing our stuff on radians instead?
void Matrix::getXYZEuler(float& xRot, float& yRot, float& zRot) const
{
   float cz;

   zRot = Math::atan2(-mat[1][0], mat[0][0]);     // -(-cy*sz)/(cy*cz) - cy falls out
   xRot = Math::atan2(-mat[2][1], mat[2][2]);     // -(sx*cy)/(cx*cy) - cy falls out
   cz = Math::cos(zRot);
   yRot = Math::atan2(mat[2][0], mat[0][0]/cz);   // (sy)/((cy*cz)/cz)

   xRot = Math::rad2deg(xRot);
   yRot = Math::rad2deg(yRot);
   zRot = Math::rad2deg(zRot);
}

// ------------------------------------------------------------------- //

// args are in degrees
void Matrix::makeZYXEuler(float zRot, float yRot, float xRot)
{
   float sx = Math::sin(Math::deg2rad(xRot));  float cx = Math::cos(Math::deg2rad(xRot));
   float sy = Math::sin(Math::deg2rad(yRot));  float cy = Math::cos(Math::deg2rad(yRot));
   float sz = Math::sin(Math::deg2rad(zRot));  float cz = Math::cos(Math::deg2rad(zRot));

   // Derived by simply multiplying out the matrices by hand
   // Z*Y*Z
   mat[0][0] = cy*cz;      mat[1][0] = -cx*sz + sx*sy*cz;   mat[2][0] = sx*sz + cx*sy*cz;    mat[3][0] = 0.0f;
   mat[0][1] = cy*sz;      mat[1][1] = cx*cz + sx*sy*sz;    mat[2][1] = -sx*cz + cx*sy*sz;   mat[3][1] = 0.0f;
   mat[0][2] = -sy;        mat[1][2] = sx*cy;               mat[2][2] = cx*cy;               mat[3][2] = 0.0f;
   mat[0][3] = 0.0f;       mat[1][3] = 0.0f;                mat[2][3] = 0.0f;                mat[3][3] = 1.0f;

   zeroClamp();     // Clamp ~ zero values
}

// args are in degrees
void Matrix::getZYXEuler(float& zRot, float& yRot, float& xRot) const
{
   float sx;

   zRot = Math::atan2(mat[0][1], mat[0][0]);      // (cy*sz)/(cy*cz) - cy falls out
   xRot = Math::atan2(mat[1][2], mat[2][2]);      // (sx*cy)/(cx*cy) - cy falls out
   sx = Math::sin(xRot);
   yRot = Math::atan2(-mat[0][2],(mat[1][2]/sx) );   // -(-sy)/((sx*cy)/sx)

   xRot = Math::rad2deg(xRot);
   yRot = Math::rad2deg(yRot);
   zRot = Math::rad2deg(zRot);
}

// -------------------------------------------------------------- //

// args are in degrees
void Matrix::makeZXYEuler(float zRot, float xRot, float yRot)
{
   float sx = Math::sin(Math::deg2rad(xRot));  float cx = Math::cos(Math::deg2rad(xRot));
   float sy = Math::sin(Math::deg2rad(yRot));  float cy = Math::cos(Math::deg2rad(yRot));
   float sz = Math::sin(Math::deg2rad(zRot));  float cz = Math::cos(Math::deg2rad(zRot));

   // Derived by simply multiplying out the matrices by hand
   // Z*X*Y
   mat[0][0] = cy*cz - sx*sy*sz;    mat[1][0] = -cx*sz;     mat[2][0] = sy*cz + sx*cy*sz;    mat[3][0] = 0.0f;
   mat[0][1] = cy*sz + sx*sy*cz;    mat[1][1] = cx*cz;      mat[2][1] = sy*sz - sx*cy*cz;    mat[3][1] = 0.0f;
   mat[0][2] = -cx*sy;              mat[1][2] = sx;         mat[2][2] = cx*cy;               mat[3][2] = 0.0f;
   mat[0][3] = 0.0f;                mat[1][3] = 0.0f;       mat[2][3] = 0.0f;               mat[3][3] = 1.0f;

   zeroClamp();     // Clamp ~ zero values
}

//: extract the yaw information from the matrix
//  returned value is from -180 to 180, where 0 is none
// Rotation around Y axis
float Matrix::getYRot() const
{
   const Vec3 forwardPoint( 0, 0, -1 );       // -Z
   const Vec3 originPoint( 0, 0, 0 );
   Vec3 endPoint, startPoint;
   endPoint.xformFull( *this, forwardPoint );
   startPoint.xformFull( *this, originPoint );
   Vec3 directionVector = endPoint - startPoint;

   // constrain the direction to x/z plane only
   directionVector[VJ_Y] = 0.0f;                   // Eliminate Y value
   directionVector.normalize();
   float y_rot = Math::rad2deg( Math::acos(directionVector.dot( forwardPoint )) );
   Vec3 whichSide = forwardPoint.cross(directionVector);
   if (whichSide[VJ_Y] < 0.0f)   // If dir vector to "right" (negative) side of forward
      y_rot = -y_rot;

   return y_rot;
}

//: extract the pitch information from the matrix
//  returned value is from -180 to 180, where 0 none
// Rotation around X axis
float Matrix::getXRot() const
{
   const Vec3 forwardPoint( 0, 0, -1 );    // -Z
   const Vec3 originPoint( 0, 0, 0 );
   Vec3 endPoint, startPoint;
   endPoint.xformFull( *this, forwardPoint );
   startPoint.xformFull( *this, originPoint );
   Vec3 directionVector = endPoint - startPoint;

   // constrain the direction to y/z plane only
   directionVector[VJ_X] = 0.0f;                // Eliminate X value
   directionVector.normalize();
   float x_rot = Math::rad2deg( Math::acos(directionVector.dot( forwardPoint )) );
   Vec3 whichSide = forwardPoint.cross(directionVector);
   if (whichSide[VJ_X] < 0.0f)      // If dir vector to "bottom" (negative) side of forward
      x_rot = -x_rot;

   return x_rot;
}

//: extract the roll information from the matrix
//  returned value is from -180 to 180, where 0 is no roll
// Rotation around Z axis
float Matrix::getZRot() const
{
   const Vec3 up_point( 0, 1, 0 );          // straight up (+Y)
   const Vec3 origin_point( 0, 0, 0 );
   Vec3 end_point, start_point;
   end_point.xformFull( *this, up_point );
   start_point.xformFull( *this, origin_point );
   Vec3 direction_vector = end_point - start_point;

   // constrain the direction to x/y plane only
   direction_vector[VJ_Z] = 0.0f;                    // Eliminate Z component
   direction_vector.normalize();
   float z_rot = Math::rad2deg( Math::acos(direction_vector.dot( up_point )) );
   Vec3 which_side = up_point.cross(direction_vector);
   if (which_side[VJ_Z] < 0.0f)                          // If dirVec it to right (neg side) of up_point
      z_rot = -z_rot;

   return z_rot;
}

// constrain the matrix rotation to a certain axis or axes
// result returned is a constrained matrix.
void Matrix::_kevn_constrainRotAxis( const bool& usePitch, const bool& useYaw, const bool& useRoll, Matrix& result )
{
   // temporary matrix
   Matrix constrainedMatrix;

   // Restrict the rotation to only the axis specified
   float xRot, yRot, zRot;
   Vec3 xAxis( 1, 0, 0 ), yAxis( 0, 1, 0 ), zAxis( 0, 0, 1 );

   // Add back the translation:
   Vec3 trans;
   this->getTrans( trans[0],trans[1],trans[2] );
   constrainedMatrix.makeTrans( trans[0],trans[1],trans[2] );

   // Add back pure pitch
   if (usePitch) // allow rotation about X
   {
      Matrix mx;
      xRot = this->getPitch();
      mx.makeRot( xRot, xAxis );
      constrainedMatrix.preMult( mx );
   }

   // Add back pure yaw
   if (useYaw) // allow rotation about Y
   {
      Matrix my;
      yRot = this->getYaw();
      my.makeRot( yRot, yAxis );
      constrainedMatrix.postMult( my );
   }

   // Add back pure roll
   if (useRoll) // allow rotation about Z
   {
      Matrix mz;
      zRot = this->getRoll();
      mz.makeRot( zRot, zAxis );
      constrainedMatrix.preMult( mz );
   }

   result.copy( constrainedMatrix );
}

// constrain the matrix rotation to a certain axis or axes
// result returned is a constrained matrix.
void Matrix::constrainRotAxis( const bool& allowXRot, const bool& allowYRot, const bool& allowZRot, Matrix& result )
{
   // temporary matrix
   Matrix constrainedMatrix;
   constrainedMatrix = *this;
   //vjMatrix inv_mat;

   // Restrict the rotation to only the axis specified
   float xRot, yRot, zRot;
   //vjVec3 xAxis( 1, 0, 0 ), yAxis( 0, 1, 0 ), zAxis( 0, 0, 1 );

   // Add back the translation:
   /*
   Vec3 trans;
   this->getTrans( trans[0],trans[1],trans[2] );
   constrainedMatrix.makeTrans( trans[0],trans[1],trans[2] );
   */

   //_kevn_constrainRotAxis(allowXRot,allowYRot,allowZRot,constrainedMatrix);

   ///*
   this->getXYZEuler(xRot,yRot,zRot);
   constrainedMatrix.makeXYZEuler(((allowXRot)?xRot:0.0f),((allowYRot)?yRot:0.0f),((allowZRot)?zRot:0.0f));
   //*/


   // Add back pure pitch
   if (!allowXRot) // do not allow rotation about X
   {
      /*
      Matrix mx;
      xRot = constrainedMatrix.getXRot();
      mx.makeRot( xRot, xAxis );
      inv_mat.invert(mx);
      constrainedMatrix.preMult( inv_mat );
      */
      /*
      Matrix mx,mxInv;
      constrainedMatrix.getXYZEuler(xRot,yRot,zRot);
      mx.makeRot(xRot,xAxis);
      mxInv.invert(mx);
      constrainedMatrix.preMult(mxInv);                // Now we don't have X???
      */
   }

   // Add back pure yaw
   if (!allowYRot) // do not allow rotation about Y
   {
      /*
      Matrix my;
      yRot = constrainedMatrix.getYRot();
      my.makeRot( yRot, yAxis );
      inv_mat.invert(my);
      constrainedMatrix.preMult( inv_mat );
      */
      /*
      Matrix my,myInv;
      constrainedMatrix.getZXYEuler(zRot,xRot,yRot);
      my.makeRot(yRot,yAxis);
      myInv.invert(my);
      constrainedMatrix.postMult(myInv);                // Now we don't have Y???
      */
   }

   // Add back pure roll
   if (!allowZRot) // do not allow rotation about Z
   {
      /*
      Matrix mz;
      zRot = constrainedMatrix.getZRot();
      mz.makeRot( zRot, zAxis );
      inv_mat.invert(mz);
      constrainedMatrix.preMult( inv_mat );
      */
      /*
      Matrix mz,mzInv;
      constrainedMatrix.getZYXEuler(zRot,yRot,xRot);
      mz.makeRot(zRot,zAxis);
      mzInv.invert(mz);
      constrainedMatrix.preMult(mzInv);                // Now we don't have Z???
      */
   }

   result = constrainedMatrix;
}

void Matrix::getZXYEuler(float& zRot, float& xRot, float& yRot) const
{
   // Extract the rotation directly fromt he matrix
   float x_angle;
   float y_angle;
   float z_angle;
   float cos_y, sin_y;
   float cos_x, sin_x;
   float cos_z, sin_z;

   sin_x = mat[1][2];
   x_angle = Math::asin(sin_x);     // Get x angle
   cos_x = Math::cos(x_angle);
   Math::zero_clamp(cos_x);          // Clamp it to get a good test

   // Check if cos_x = Zero
   if(cos_x != 0.0f)     // ASSERT: cos_x != 0
   {
         // Get y Angle
      cos_y = mat[2][2]/cos_x;
      sin_y = -mat[0][2]/cos_x;
      y_angle = Math::atan2(cos_y, sin_y);

         // Get z Angle
      cos_z = mat[1][1]/cos_x;
      sin_z = -mat[1][0]/cos_x;
      z_angle = Math::atan2(cos_z, sin_z);
   }
   else
   {
      // Arbitrarily set z_angle = 0
      z_angle = 0;

         // Get y Angle
      cos_y = mat[0][0];
      sin_y = mat[0][1];
      y_angle = Math::atan2(cos_y, sin_y);
   }

   xRot = Math::rad2deg(x_angle);
   yRot = Math::rad2deg(y_angle);
   zRot = Math::rad2deg(z_angle);
}

// --------------------------------------------------------------------------- //

void Matrix::makeDirCos(Vec3 secXAxis, Vec3 secYAxis, Vec3 secZAxis)
{
   vprASSERT(secXAxis.isNormalized());
   vprASSERT(secYAxis.isNormalized());
   vprASSERT(secZAxis.isNormalized());

   float Xa, Xb, Xy;    // Direction cosines of the secondary x-axis
   float Ya, Yb, Yy;    // Direction cosines of the secondary y-axis
   float Za, Zb, Zy;    // Direction cosines of the secondary z-axis

   Vec3 xAxis(1,0,0), yAxis(0,1,0), zAxis(0,0,1);   // The base axis to use

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
void Matrix::makeQuaternion( const Quat& q )
{
   /*
   // A piece of reference code for checking against...
   const int W = VJ_W;
   const int X = VJ_X;
   const int Y = VJ_Y;
   const int Z = VJ_Z;
   const float* const quat = q;

   mat[0][0]  = 1.0f  - 2.0f * (quat[Y] * quat[Y] + quat[Z] * quat[Z]);
   mat[0][1]  = 2.0f         * (quat[X] * quat[Y] + quat[W] * quat[Z]);
   mat[0][2]  = 2.0f         * (quat[X] * quat[Z] - quat[W] * quat[Y]);
   mat[0][3]  = 0.0f;

   mat[1][0]  = 2.0f         * (quat[X] * quat[Y] - quat[W] * quat[Z]);
   mat[1][1]  = 1.0f  - 2.0f * (quat[X] * quat[X] + quat[Z] * quat[Z]);
   mat[1][2]  = 2.0f         * (quat[Y] * quat[Z] + quat[W] * quat[X]);
   mat[1][3]  = 0.0f;

   mat[2][0]  = 2.0f         * (quat[X] * quat[Z] + quat[W] * quat[Y]);
   mat[2][1]  = 2.0f         * (quat[Y] * quat[Z] - quat[W] * quat[X]);
   mat[2][2] = 1.0f  - 2.0f * (quat[X] * quat[X] + quat[Y] * quat[Y]);
   mat[2][3] = 0.0f;

   mat[3][0] = 0;
   mat[3][1] = 0;
   mat[3][2] = 0;
   mat[3][3] = 1.0f;
   */


   float wx, wy, wz, xx, yy, yz, xy, xz, zz, xs, ys, zs;
   //float s;

   //s = 2.0f/(q[VJ_X]*q[VJ_X] + q[VJ_Y]*q[VJ_Y] + q[VJ_Z]*q[VJ_Z] + q[VJ_W]*q[VJ_W]);

   xs = q[VJ_X] + q[VJ_X];   ys = q[VJ_Y] + q[VJ_Y];   zs = q[VJ_Z] + q[VJ_Z];
   xx = q[VJ_X] * xs;      xy = q[VJ_X] * ys;      xz = q[VJ_X] * zs;
   yy = q[VJ_Y] * ys;      yz = q[VJ_Y] * zs;      zz = q[VJ_Z] * zs;
   wx = q[VJ_W] * xs;      wy = q[VJ_W] * ys;      wz = q[VJ_W] * zs;

   mat[0][0] = 1.0 - (yy + zz);
   mat[0][1] = xy + wz;
   mat[0][2] = xz - wy;
   mat[0][3] = 0.0;

   mat[1][0] = xy - wz;
   mat[1][1] = 1.0 - (xx + zz);
   mat[1][2] = yz + wx;
   mat[1][3] = 0.0;

   mat[2][0] = xz + wy;
   mat[2][1] = yz - wx;
   mat[2][2] = 1.0 - (xx + yy);
   mat[2][3] = 0.0;

   mat[3][0] = 0.0;
   mat[3][1] = 0.0;
   mat[3][2] = 0.0;
   mat[3][3] = 1.0;
}

void Matrix::makeQuaternion( const float* const q )
{
   Quat quat( q[VJ_W], q[VJ_X], q[VJ_Y], q[VJ_Z] );
   makeQuaternion( quat );
}

// TODO: consider changing to radians...
// then this func will be fast as possible,
// plus client won't have to convert rad->deg most times they
// want to use this func.
void  Matrix::makeRot(float _degrees, Vec3 _axis)
{
    _axis.normalize();  // NOTE: This could be eliminated by passing normalized
             //       vector.  This could be by ref to make even faster
    // GGI: pg 466
    float s = Math::sin(Math::deg2rad(_degrees));
    float c = Math::cos(Math::deg2rad(_degrees));
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

void  Matrix::makeTrans(float _x, float _y, float _z)
{
   makeIdent();
   mat[3][0] = _x;
   mat[3][1] = _y;
   mat[3][2] = _z;
}

void Matrix::makeTrans(const Vec3& trans)
{ makeTrans(trans[0],trans[1],trans[2]); }

void Matrix::setTrans(float _x, float _y, float _z)
{
   mat[3][0] = _x;
   mat[3][1] = _y;
   mat[3][2] = _z;
}

void Matrix::setTrans(const Vec3& trans)
{ setTrans(trans[0],trans[1],trans[2]); }


void Matrix::getTrans(float& _x, float& _y, float& _z) const
{
   _x = mat[3][0];
   _y = mat[3][1];
   _z = mat[3][2];
}

Vec3 Matrix::getTrans() const
{
   Vec3 trans;;
   getTrans(trans[0],trans[1],trans[2]);
   return trans;
}

void  Matrix::makeScale(float _x, float _y, float _z)
{
    makeIdent();
    mat[0][0] = _x;
    mat[1][1] = _y;
    mat[2][2] = _z;
}

void Matrix::preTrans(float _x, float _y, float _z, const Matrix&  _m)
{
    Matrix transMat;
    transMat.makeTrans(_x, _y, _z);
    transMat.postMult(_m);
    *this = transMat;
}

//: Pre translate a matrix
//!POST: mat' = trans(_x,_y,_z) * _m
void Matrix::preTrans(const Vec3& _trans, const Matrix&  _m)
{ preTrans(_trans.vec[0], _trans.vec[1], _trans.vec[2], _m); }


    /// mat = _m * trans(_x,_y,_z)
void Matrix::postTrans(const Matrix&  _m, float _x, float _y, float _z)
{
    Matrix transMat;
    transMat.makeTrans(_x, _y, _z);
    transMat.preMult(_m);
    *this = transMat;
}

//!POST: mat' = _m * trans(_x,_y,_z)
void Matrix::postTrans(const Matrix&  _m, const Vec3& _trans)
{ postTrans(_m, _trans.vec[0], _trans.vec[1], _trans.vec[2]); }

void Matrix::preRot(const float& _degrees, const Vec3& axis, const Matrix&  _m)
{
    Matrix rotMat;
    rotMat.makeRot(_degrees, axis);
    rotMat.postMult(_m);
    *this = rotMat;
}

void Matrix::postRot(const Matrix&  _m, const float& _degrees, const Vec3& axis)
{
    Matrix rotMat;
    rotMat.makeRot(_degrees, axis);
    rotMat.preMult(_m);
    *this = rotMat;
}

void Matrix::preXYZEuler(float x, float y, float z, const Matrix& _m)
{
    Matrix rotMat;
    rotMat.makeXYZEuler(x, y, z);
    rotMat.postMult(_m);
    *this = rotMat;
}

void Matrix::postXYZEuler(const Matrix& _m, float x, float y, float z)
{
    Matrix rotMat;
    rotMat.makeXYZEuler(x, y, z);
    rotMat.preMult(_m);
    *this = rotMat;
}

    /// mat = scale(_xs,_ys,_zs) * _m;
void Matrix::preScale(float _xs, float _ys, float _zs, const Matrix&  _m)
{
    Matrix scaleMat;
    scaleMat.makeScale(_xs, _ys, _zs);
    scaleMat.postMult(_m);
    *this = scaleMat;
}
    /// mat = _m * scale(_xs,_ys,_zs)
void Matrix::postScale(const Matrix&  _m, float _xs, float _ys, float _zs)
{
    Matrix scaleMat;
    scaleMat.makeScale(_xs, _ys, _zs);
    scaleMat.preMult(_m);
    *this = scaleMat;
}

VJ_IMPLEMENT(Matrix) operator *(float _s, const Matrix& _m) {
    Matrix dst; dst.scale(_s, _m); return dst;
}

VJ_IMPLEMENT(Matrix) operator *(const Matrix& _m, float _s) {
    Matrix dst; dst.scale(_s, _m); return dst;
}

VJ_IMPLEMENT(Matrix) operator /(const Matrix& _m, float _s) {
    Matrix dst; dst.scale(1.0f/_s, _m); return dst;
}

    // Matrix Multiplication of A:(nxl) B:(lxm) ==> C:(nxm)
    //   Cij = Sum(k=1,l) (Aik)(Bkj)
    /// mat = mat * m
    // NOTE: All accesses in the function are using the C/C++ indexing method
void Matrix::postMult(const Matrix&  _m)
{
    Matrix prevMat = *this;     // May be sloooow!!!
    zero();

    for(int i=0;i<4;i++)
   for(int j=0;j<4;j++)
       for(int k=0;k<4;k++)
      mat[j][i] += ( prevMat.mat[k][i] * _m.mat[j][k]);
}

    // mat = m * mat
void Matrix::preMult(const Matrix&  _m)
{
    Matrix prevMat = *this;     // May be sloooow!!!
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
int Matrix::invert(const Matrix& _m)

{
   const float*  a = _m.getFloatPtr();
   float*  b = this->getFloatPtr();

   int   n = 4;
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
                                tmp_m = Math::abs( m[ i][ j]);
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


                if ( Math::abs( pivot) <= 1e-20)
                {
                        std::cerr << "*** pivot = %f in mat_inv. ***\n";
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
VJ_IMPLEMENT(std::ostream&) operator<<(std::ostream& out, const Matrix& _mat)
{
   for(int j=0;j<4;j++)
   {
      for(int i=0;i<4;i++)
         out << _mat.mat[i][j] << " ";
      out << std::endl;
   }

   return out;
}

};
