#ifndef _VJ_FRUSTUM_H_
#define _VJ_FRUSTUM_H_

#include <vjConfig.h>

//------------------------------
//: A frustum object.
// 
//	This is a frustum.
//
// @author Allen Bierbaum
//  Date: 10-1-97
//------------------------------
class vjFrustum
{
public:
   enum
   {LEFT = 0, RIGHT = 1, BOTTOM, TOP, NEAR, FAR};

   vjFrustum()
   {
      frust[0] = frust[1] = frust[2] = 0.0f;
      frust[3] = frust[4] = frust[5] = 0.0f;
   }

   void setBottomLeftTopRight(float bot, float left, float top, float right)
   {
      frust[BOTTOM] = bot;
      frust[LEFT] = left;
      frust[TOP] = top;
      frust[RIGHT] = right;
   }

   void setNearFar(float near, float far)
   {
      frust[NEAR] = near;
      frust[FAR] = far;
   }

   void set(float left, float right, float bottom, float top, float near, float far)
   {
      frust[LEFT] = left;
      frust[RIGHT] = right;
      frust[BOTTOM] = bottom;
      frust[TOP] = top;
      frust[NEAR] = near;
      frust[FAR] = far;
   }

   float& operator[](int elt)
   {
      return frust[elt];
   }

public:
   float frust[6];  //: Left, Right, Bottom, Top, Near, Far
};


// ---- FRIEND FUNCTIONS ---- //
ostream& operator<<(ostream& out, vjFrustum& _frust);

#endif
