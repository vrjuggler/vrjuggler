#ifndef _VJ_QUAT_H_
#define _VJ_QUAT_H_

#include <Math/vjVec4.h>
#include <Math/vjVec3.h>
#include <math.h>
#include <Math/vjMatrix.h>

#define VJ_QUAT_EPSILON	    0.00001f

// ---------------------------------------------------------------------------------
//: Class to encapsulate quaternion behaviors.
// 
// Note: The code for most of these routines was taken from the the folling sources.
// 
// Advanced Animation and Rendering Techniques: pp363-365
// Quaternion Calculus for Animation,  Ken Shoemake SIGGRAPH course notes 1989  
// Animating Rotation with Quaternion Curves,  Ken Shoemake 
//		SIGGRAPH Proceedings Vol 19, Number 3, 1985
//----------------------------------------------------------------------------------
class vjQuat : public vjVec4
{
public:
   vjQuat()
   {} 

   //: Construct Quat from 4 floats
   vjQuat( const float x,
           const float y,
           const float z,
           const float w)
   {
      set(x, y, z, w);
   }

   //: Construct quat from matrix
   // WARNING: I don't know if this will work.
   void makeQuat(vjMatrix& mat);

   //: Not implemented
   void getRot(float* angle, float* x, float* y, float* z)
   {;}

   //: Not implemented
   void makeRot(float angle, float x, float y, float z)
   {;}

   //: set to conj of quat
   void conj(const vjQuat& quat)
   {
      vec[0] = -quat.vec[0];
      vec[1] = -quat.vec[1]; 
      vec[2] = -quat.vec[2]; 
      vec[3] =  quat.vec[3];
   }

   float length() const
   {
      return ((vec[0]*vec[0])+(vec[1]*vec[1])+(vec[2]*vec[2])+(vec[3]*vec[3]));
   }


public:
   //: Multiply two quaternions
   //!POST: this' = q1 * q2
   void mult(const vjQuat& q1, const vjQuat& q2)
   {
      vjQuat tempQuat;
      tempQuat[VJ_X] = q1[VJ_W]*q2[VJ_X] + q1[VJ_X]*q2[VJ_W] + q1[VJ_Z]*q2[VJ_Y] - q1[VJ_Y]*q2[VJ_Z];
      tempQuat[VJ_Y] = q1[VJ_W]*q2[VJ_Y] + q1[VJ_Y]*q2[VJ_W] + q1[VJ_X]*q2[VJ_Z] - q1[VJ_Z]*q2[VJ_X];
      tempQuat[VJ_Z] = q1[VJ_W]*q2[VJ_Z] + q1[VJ_Z]*q2[VJ_W] + q1[VJ_Y]*q2[VJ_X] - q1[VJ_X]*q2[VJ_Y];
      tempQuat[VJ_W] = q1[VJ_W]*q2[VJ_W] - q1[VJ_X]*q2[VJ_X] - q1[VJ_Y]*q2[VJ_Y] - q1[VJ_Z]*q2[VJ_Z];

      vec[VJ_X] = tempQuat[VJ_X];
      vec[VJ_Y] = tempQuat[VJ_Y];
      vec[VJ_Z] = tempQuat[VJ_Z];
      vec[VJ_W] = tempQuat[VJ_W];
   }

   // quat = q1/q2
   void div(const vjQuat& q1, const vjQuat& q2)
   {
      vjQuat inv;
      inv.invert(q2);
      mult(q1, inv);
   }

   /// quat = inv(q1)
   void invert(const vjQuat& q1)
   {
      float len = q1.length();
      float invLength;

      if (len >= VJ_QUAT_EPSILON)
      {
         invLength = 1.0f/len;
         vec[VJ_X] = -q1.vec[VJ_X]*invLength;
         vec[VJ_Y] = -q1.vec[VJ_Y]*invLength;
         vec[VJ_Z] = -q1.vec[VJ_Z]*invLength;
         vec[VJ_W] =  q1.vec[VJ_W]*invLength;
      }
      else
         set(0.0f, 0.0f, 0.0f, 0.0f);
   }

   //void exp(const vjQuat& _q);
   //void log(const vjQuat& _q);

   void slerp(float _t, const vjQuat& _q1, const vjQuat& _q2);
   void squad(float _t, const vjQuat& _q1, const vjQuat& _q2, const vjQuat& _a, const vjQuat& _b);

   //CAPI:noverb
   //void meanTangent(const vjQuat& _q1, const vjQuat& _q2, const vjQuat& _q3);

public:
   // vjQuat operators
   vjQuat operator *(const vjQuat&  _m) const {
      vjQuat dst; dst.mult(*this, _m); return dst;
   }

   vjQuat operator /(const vjQuat&  _m) const {
      vjQuat dst; dst.div(*this, _m); return dst;
   }

public:
   // Assignment operators
   vjQuat&  operator *=(const vjQuat& _q) {
      this->mult(*this, _q); return *this;
   }
   vjQuat&  operator /=(const vjQuat& _q) {
      this->div(*this, _q); return *this;
   }

};

#endif
