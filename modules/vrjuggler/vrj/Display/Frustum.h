/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


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
   enum entry
   {VJ_LEFT = 0, VJ_RIGHT = 1, VJ_BOTTOM, VJ_TOP, VJ_NEAR, VJ_FAR};

   vjFrustum()
   {
      frust[0] = frust[1] = frust[2] = 0.0f;
      frust[3] = frust[4] = frust[5] = 0.0f;
   }

   void setBottomLeftTopRight(float bot_val, float left_val, float top_val, float right_val)
   {
      frust[VJ_BOTTOM] = bot_val;
      frust[VJ_LEFT] = left_val;
      frust[VJ_TOP] = top_val;
      frust[VJ_RIGHT] = right_val;
   }

   void setNearFar(float near_val, float far_val)
   {
      frust[VJ_NEAR] = near_val;
      frust[VJ_FAR] = far_val;
   }

   void set(float left_val, float right_val, float bottom_val, float top_val, float near_val, float far_val)
   {
      frust[VJ_LEFT] = left_val;
      frust[VJ_RIGHT] = right_val;
      frust[VJ_BOTTOM] = bottom_val;
      frust[VJ_TOP] = top_val;
      frust[VJ_NEAR] = near_val;
      frust[VJ_FAR] = far_val;
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
