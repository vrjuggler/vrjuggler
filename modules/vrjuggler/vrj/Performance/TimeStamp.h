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



/* TimeStaMp... a simple class to enclose a timestamp */

   

#ifndef _VJ_TIMESTAMPPPPPP_
#define _VJ_TIMESTAMPPPPPP_

#include <vjConfig.h>




#if VJ_PERFORMANCE == VJ_PERF_SGI
#include <Performance/vjTimeStampSGI.h>
namespace vrj
{
   typedef TimeStaMpSGI TimeStaMp;
};


#elif VJ_PERFORMANCE == VJ_PERF_POSIX
#include <Performance/vjTimeStampPosix.h>
namespace vrj
{
   typedef TimeStaMpPosix TimeStaMp;
}


#else
#include <Performance/vjTimeStampNone.h>
namespace vrj
{
   typedef TimeStaMpNone TimeStaMp;
};

#endif

#endif
