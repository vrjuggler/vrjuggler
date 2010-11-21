/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SOCKET_STREAM_H_
#define _VPR_SOCKET_STREAM_H_

/**
 * \file
 *
 * Include this file to get the full declaration of the platform-specific
 * class that is typedef'd to vpr::SocketStream.
 */

#include <vpr/vprConfig.h>

#include <vpr/IO/Stats/BandwidthIOStatsStrategy.h>
#include <vpr/IO/Stats/BaseIOStatsStrategy.h>
#include <vpr/IO/Stats/IOStatsStrategyAdapter.h>

#include <vpr/IO/Socket/SocketStream_t.h>             // include bridge class

#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR
#include <vpr/md/NSPR/IO/Socket/SocketStreamImplNSPR.h>
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX
#include <vpr/md/POSIX/IO/Socket/SocketStreamImplBSD.h>
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_SIMULATOR
#include <vpr/md/SIM/IO/Socket/SocketStreamImplSIM.h>
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_BOOST
#include <vpr/md/BOOST/IO/Socket/SocketStreamImplBOOST.h>
#endif

namespace vpr
{
   typedef SocketStream_t<SocketConfiguration>  SocketStream;
}


#endif	/* _VPR_SOCKET_STREAM_H_ */
