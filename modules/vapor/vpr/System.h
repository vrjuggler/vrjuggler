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

#ifndef _VPR_SYSTEM_H_
#define _VPR_SYSTEM_H_

// include base class for vprSystem
//#include <vpr/SystemBase.h>

// determine which implementation of vpr::System to include.
#if defined(VPR_USE_IRIX_SPROC) || defined(VPR_USE_PTHREADS)
#   include <vpr/md/POSIX/SystemPosix.h>
namespace vpr {
    typedef SystemPosix System;
};

#else
#   include <vpr/md/NSPR/SystemNSPR.h>
namespace vpr {
    typedef SystemNSPR System;
};

#endif  /* VPR_IRIX_SPROC || VPR_USE_PTHREADS */

#endif /* _VPR_SYSTEM_FAKE_H_ */
