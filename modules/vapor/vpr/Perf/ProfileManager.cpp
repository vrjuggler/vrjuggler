/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <vpr/Perf/ProfileManager.h>
#include <iostream>

namespace vpr
{


   void  ProfileManager::startProfile( const char * name )
   {
      if ( name != mCurrentNode->getName() )
      {
         mCurrentNode = mCurrentNode->getSubNode( name );
      }

      mCurrentNode->call();
   }

   void  ProfileManager::startProfile( const char * profileName , const unsigned int queueSize)
   {
      if ( profileName != mCurrentNode->getName() )
      {
         mCurrentNode = mCurrentNode->getSubNode( profileName, queueSize);
      }

      mCurrentNode->call();
   }

   void  ProfileManager::stopProfile( void )
   {
      // Return will indicate whether we should back up to our parent (we may
      // be profiling a recursive function)
      if ( mCurrentNode->Return() )
      {
         mCurrentNode = mCurrentNode->getParent();
      }
   }

   void  ProfileManager::reset( void )
   {
      mRoot.reset(); 
      mFrameCounter = 0;
      profileGetTicks(mResetTime);
   }

   void ProfileManager::incrementFrameCounter( void )
   {
      mFrameCounter++;
   }

   float ProfileManager::getTimeSinceReset( void )
   {
      vpr::Interval time;
      profileGetTicks(&time);
      time = time - *mResetTime;
      return(float)(time.secf() / profileGetTickRate().secf());
   }

} // end namespace vpr
