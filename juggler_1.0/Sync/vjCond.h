/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJCond_h_
#define _VJCond_h_

#include <vjConfig.h>

#if defined(VJ_IRIX_SPROC)
#    include <ulocks.h>
#    include <Sync/vjCondGeneric.h>
    
    typedef  vjCondGeneric vjCond;
#elif defined(VJ_USE_PTHREADS)
#ifndef _POSIX_C_SOURCE
#   define _POSIX_C_SOURCE VJ_POSIX_C_SOURCE
#endif

#   include <Sync/vjCondPosix.h>
    
    typedef  vjCondPosix vjCond;
#elif defined(VJ_OS_Win32)
#    include <Sync/vjCondGeneric.h>
    
    typedef  vjCondGeneric vjCond;
#endif   /* VJ_IRIX_SPROC */

#endif
