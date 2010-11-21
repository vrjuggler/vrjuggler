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

#ifndef _VPR_SOCKET_DATAGRAM_H_
#define _VPR_SOCKET_DATAGRAM_H_

/**
 * \file
 *
 * Include this file to get the full declaration of the platform-specific
 * class that is typedef'd to vpr::SocketDatagram.
 */

#include <vpr/vprConfig.h>

#include <vpr/IO/Stats/BandwidthIOStatsStrategy.h>
#include <vpr/IO/Stats/BaseIOStatsStrategy.h>
#include <vpr/IO/Stats/IOStatsStrategyAdapter.h>

// include bridge class
#include <vpr/IO/Socket/SocketDatagram_t.h>

#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_NSPR
#include <vpr/md/NSPR/IO/Socket/SocketDatagramImplNSPR.h>
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_POSIX
#include <vpr/md/POSIX/IO/Socket/SocketDatagramImplBSD.h>
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_SIMULATOR
#include <vpr/md/SIM/IO/Socket/SocketDatagramImplSIM.h>
#elif VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_BOOST
#include <vpr/md/BOOST/IO/Socket/SocketDatagramImplBOOST.h>
#endif

namespace vpr
{
   typedef SocketDatagram_t<SocketConfiguration>  SocketDatagram;
}


#endif	/* _VJ_SOCKET_DATAGRAM_H_ */
