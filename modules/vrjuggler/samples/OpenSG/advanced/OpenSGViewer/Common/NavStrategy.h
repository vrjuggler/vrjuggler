/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef NAVSTRATEGY_H
#define NAVSTRATEGY_H

#include <gmtl/Matrix.h>

class User;

/**
* Strategy for doing navigation.
* The Viewplatform uses this strategy to move itself through the virtual world.
*
* We just track the simple idea of our position within the virtual world.  The platform
* and user handle all the math to update the other matricies in the system.
*/
class NavStrategy
{
public:
   /** Explicitly set the navigation position */
   virtual void setPosition(gmtl::Matrix44f pos)
   { mCurPos = pos; }

   /** Get the current navigation position */
   virtual gmtl::Matrix44f getPosition()
   { return mCurPos; }

   /** Get the current navigation position */
   virtual gmtl::Matrix44f getPositionInv()
   { return mCurPosInv; }

   /**
     * Template method for updating the view platform (navigation, etc).
     * Called once per frame
     * @param user - The user that is currently controlling the view platform
     */
    virtual void update(User* user) = 0;
protected:
   gmtl::Matrix44f mCurPos;   /**< The current position */
   gmtl::Matrix44f mCurPosInv;   /**< The current position inverse*/
};
#endif //NAVSTRATEGY_H

