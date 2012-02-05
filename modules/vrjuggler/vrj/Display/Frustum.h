/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_FRUSTUM_H_
#define _VRJ_FRUSTUM_H_

#include <vrj/vrjConfig.h>

#include <iostream>
#include <vector>


namespace vrj
{

/** \class Frustum Frustum.h vrj/Display/Frustum.h
 *
 * A frustum object.  This is a frustum.
 *
 * @date October 1, 1997
 */
class VJ_API Frustum
{
public:
   enum entry
   {
      VJ_LEFT = 0,      /**< Left (X minium) */
      VJ_RIGHT = 1,     /**< Right (X maximum) */
      VJ_BOTTOM,        /**< Bottom (Y minimum) */
      VJ_TOP,           /**< Top (Y maximum) */
      VJ_NEAR,          /**< Near (Z minimum) */
      VJ_FAR            /**< Far (Z maximum) */
   };

   Frustum();

   void setBottomLeftTopRight(const float bottomVal, const float leftVal,
                              const float topVal, const float rightVal);

   void setNearFar(const float nearVal, const float farVal)
   {
      mFrust[VJ_NEAR] = nearVal;
      mFrust[VJ_FAR]  = farVal;
   }

   void set(const float leftVal, const float rightVal, const float bottomVal,
            const float topVal, const float nearVal, const float farVal);

   float& operator[](const unsigned int elt)
   {
      return mFrust[elt];
   }

   const float& operator[](const unsigned int elt) const
   {
      return mFrust[elt];
   }

   /**
    * Returns the values for this frustum object. The values can (and should)
    * be accessed using the identifiers defined in vrj::Frustum::entry.
    *
    * @since 2.1.27
    */
   const std::vector<float>& getValues() const
   {
      return mFrust;
   }

private:
   std::vector<float> mFrust;   /**< Left, Right, Bottom, Top, Near, Far */
};

std::ostream& operator<<(std::ostream& out, Frustum& _frust);

}

#endif
