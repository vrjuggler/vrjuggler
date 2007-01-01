/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef VIEWPLATFORM_H
#define VIEWPLATFORM_H

#include <gmtl/Matrix.h>

class NavStrategy;
class User;

/**
* The view platform is the "relative coordinate" system of the user as they
* move through the virtual world.  This of it as the magic carpet that the
* user is standing upon.  Physically this corresponds closest to the concept
* of an IPT device or just the space imediately around the user in the
* real-world.
*
* See User for a description of the coordinate systems used.
*/
class ViewPlatform
{
public:
   ViewPlatform()
      : mNavStrategy(NULL)
   {;}

    /**
     * Updating the view platform (navigation, etc).
     * Called once per frame
     * @param user - The user that is currently controlling the view platform
     */
    void update(User* user);

    /** Return the current position of the platform in the virtual world. */
    gmtl::Matrix44f getTransform_virtMplat();

    /** Return the current inverse of the current pos of plat in vw.
    * This is the amount to move the vw to center on plat
    */
    gmtl::Matrix44f getTransform_platMvirt();
    

public:
   void setNavStrategy(NavStrategy* navStrategy);
   NavStrategy* getNavStrategy();

protected:    

    /**
     * @supplierCardinality 1
     * @directed 
     */
    NavStrategy* mNavStrategy;
};


#endif //VIEWPLATFORM_H

