/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <Common/ViewPlatform.h>
#include <Common/NavStrategy.h>

void ViewPlatform::update(User* user)
{
   mNavStrategy->update(user);
}

gmtl::Matrix44f ViewPlatform::getTransform_virtMplat()
{
   if(NULL != mNavStrategy)
   {
      return mNavStrategy->getPosition();
   }
   else
   {
      return gmtl::Matrix44f();  // Return an identity matrix
   }
}

gmtl::Matrix44f ViewPlatform::getTransform_platMvirt()
{
   if(NULL != mNavStrategy)
   {
      return mNavStrategy->getPositionInv();
   }
   else
   {
      return gmtl::Matrix44f();  // Return an identity matrix
   }
}


void ViewPlatform::setNavStrategy(NavStrategy* navStrategy)
{
   if(NULL != navStrategy)
   {
      // If we already have a strategy, then copy over the current position
      if(NULL != mNavStrategy)
      {
         navStrategy->setPosition( mNavStrategy->getPosition() );
      }
      mNavStrategy = navStrategy;
   }
   else  // Clear out strategy
   {
      mNavStrategy = NULL;
   }
}

NavStrategy* ViewPlatform::getNavStrategy()
{
   return mNavStrategy;
}

