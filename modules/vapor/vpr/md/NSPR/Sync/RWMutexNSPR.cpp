/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <vprConfig.h>

#include <md/NSPR/RWMutexNSPR.h>


namespace vpr {

//----------------------------------------------------------
// Try to acquire a read mutex.
//----------------------------------------------------------
int
RWMutexNSPR::tryAcquireRead () {
    int retVal = -1;

/*
    if (stateLock.acquire() != -1)
    {
        if(refCount == -1 || numWaitingWriters >0)
            retVal = -1;
        else
        {
            refCount++;
            retVal = 0;
        }
        stateLock.release();
    }
*/
    return retVal;
}

//----------------------------------------------------------
// Try to acquire a write mutex.
//----------------------------------------------------------
int
RWMutexNSPR::tryAcquireWrite () {
    int retVal = -1;

/*
    if (stateLock.acquire() != -1)
    {
        if(refCount != 0)
            retVal = -1;
        else
        {
            refCount = -1;
            retVal = 0;
        }
        stateLock.release();
    }
*/

    return retVal;
}

}; // End of vpr namespace
