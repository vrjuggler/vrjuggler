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

#ifndef _VPR_SOCKET_H_
#define _VPR_SOCKET_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/IO/BlockIO.h>
#include <vpr/IO/Socket/SocketTypes.h>
#include <vpr/IO/Socket/InetAddr.h>

// include bridge class
#include <vpr/IO/Socket/Socket_t.h>

// make the connection
#if defined(VPR_USE_NSPR)
#   include <vpr/md/NSPR/IO/Socket/SocketImplNSPR.h>
namespace vpr {
  typedef Socket_t<SocketImplNSPR> Socket;
};

#else
#   include <vpr/md/POSIX/IO/Socket/SocketImplBSD.h>
namespace vpr {
  typedef Socket_t<SocketImplBSD> Socket;
};
#endif


#endif  /* _VPR_SOCKET_H_ */
