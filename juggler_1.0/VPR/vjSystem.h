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

#ifndef _VJ_SYSTEM_FAKE_H_
#define _VJ_SYSTEM_FAKE_H_

// include base class for vjSystem
#include <VPR/vjSystemBase.h>

// determine which implementation of vjSystem to include.
#if defined(VJ_IRIX_SPROC) || defined(VJ_USE_PTHREADS)
   #include <VPR/md/POSIX/vjSystemPosix.h>
#elif defined(VJ_OS_Win32)
   #include <VPR/md/WIN32/vjSystemWin32.h>
#else
   #include <VPR/md/NSPR/vjSystemNSPR.h>
#endif  /* VJ_IRIX_SPROC || VJ_USE_PTHREADS */

#endif // _VJ_SYSTEM_FAKE_H_
