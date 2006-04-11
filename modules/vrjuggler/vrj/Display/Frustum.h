/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _VRJ_FRUSTUM_H_
#define _VRJ_FRUSTUM_H_

#include <vrj/vrjConfig.h>

#include <iostream>


namespace vrj
{

/**
 * A frustum object.  This is a frustum.
 *
 * @date 10-1-97
 */
class VJ_CLASS_API Frustum
{
public:
   enum entry
   {VJ_LEFT = 0, VJ_RIGHT = 1, VJ_BOTTOM, VJ_TOP, VJ_NEAR, VJ_FAR};

   Frustum()
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
   float frust[6];  /**<  Left, Right, Bottom, Top, Near, Far */
};

// ---- FRIEND FUNCTIONS ---- //
std::ostream& operator<<(std::ostream& out, Frustum& _frust);

}

#endif
