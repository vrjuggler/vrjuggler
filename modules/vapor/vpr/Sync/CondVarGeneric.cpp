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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <iostream>
#include <iomanip>

#include <vpr/Sync/CondVarGeneric.h>


/**
 * Waits for possible condition change.
 *
 * @post The condition has been modifed, but may not be satisfied.
 * @note The call blocks until a condition has been signaled
 */
vpr::ReturnStatus vpr::CondVarGeneric::wait(vpr::Interval timeToWait)
{
   std::cerr << std::setw(5) << getpid() << "  Wait: Begin:" << std::endl;
   // ASSERT:  We have been locked
   if ( mCondMutex->test() == 0 )    // Not locked
   {
      std::cerr << " vpr::CondVarGeneric::wait: INCORRECT USAGE: Mutex was not locked when wait invoked!!!"
                << std::endl;
   }

   mWaiters++;              // We have lock already

   mCondMutex->release();   // Release it

   mSema.acquire();         // Wait for a while
   mSema.dump();

   // We must now regain the lock so that the condition can be re checked upon exit
   // We also need it to decrement waiters
   mCondMutex->acquire();
   mWaiters--;

   std::cerr << std::setw(5) << getpid() << "  Wait: end:" << std::endl;
   return vpr::ReturnStatus();
}
