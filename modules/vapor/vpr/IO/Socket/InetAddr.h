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

#ifndef _VPR_INET_ADDR_H_
#define _VPR_INET_ADDR_H_

#include <vpr/vprConfig.h>
#include <iostream>

#if defined(VPR_USE_NSPR)
#   include <vpr/md/NSPR/IO/Socket/InetAddrNSPR.h>
namespace vpr {
    typedef InetAddrNSPR InetAddr;
};
#else
#   include <vpr/md/POSIX/IO/Socket/InetAddrBSD.h>
namespace vpr {
    typedef InetAddrBSD InetAddr;
};
#endif

#ifdef HAVE_HASH_MAP_H
#    include <hash_map.h>
#endif

namespace std {

#ifdef HAVE_HASH_MAP_H
// Nice little helper class for hashing an inetaddr
template<>
struct hash<vpr::InetAddr> {
   size_t operator() (vpr::InetAddr addr) const {
      return ((addr.getAddressValue() << 16) | addr.getPort());
   }
};
#else
struct hash {
   size_t operator() (vpr::InetAddr addr) const {
      return ((addr.getAddressValue() << 16) | addr.getPort());
   }
};
#endif

}

inline bool operator<(const vpr::InetAddr& addr1, const vpr::InetAddr& addr2)
{
   return ((addr1.getAddressValue() < addr2.getAddressValue()) ? true : (addr1.getPort() < addr2.getPort()));
}

inline std::ostream& operator<<(std::ostream& out, const vpr::InetAddr& addr)
{
   out << addr.getAddressString() << ":" << addr.getPort();
   return out;
}

#endif   /* _VPR_INET_ADDR_H_ */
