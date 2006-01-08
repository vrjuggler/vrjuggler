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

#ifndef _VPR_CondVar_h_
#define _VPR_CondVar_h_

/**
 * \file
 *
 * Include this file to get the full declaration of the platform-specific
 * type that is typedef'd to vpr::CondVar.
 */

#include <vpr/vprConfig.h>

#if VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_IRIX_SPROC
#  include <ulocks.h>
#  include <vpr/Sync/CondVarGeneric.h>
#elif VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX
#ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE VPR_POSIX_C_SOURCE
#endif

#  include <vpr/md/POSIX/Sync/CondVarPosix.h>

#undef _POSIX_C_SOURCE

#elif VPR_THREAD_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR
#  include <vpr/md/NSPR/Sync/CondVarNSPR.h>
#endif   /* VPR_IRIX_SPROC */


/** @example "Example of using a vpr::CondVar"
 *
 * \code
 * vpr::CondVar mCondVar;   // The condition variable to use
 *
 * // --- Set a state and signal everyone else about it --- //
 * mCondVar.acquire();
 * {
 *    mState = NEW_STATE;
 *    mCondVar.signal();
 * }
 * mCondVar.release();
 *
 * // -- Wait for a condition -- //
 * mCondVar.acquire();
 * {
 *    while ( mState != DESIRED_STATE )
 *    {
 *       mCondVar.wait();
 *    }
 * }
 * mCondVar.release();
 * \endcode
 */

#endif
