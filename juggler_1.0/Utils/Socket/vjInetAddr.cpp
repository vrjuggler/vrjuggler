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

#include <stdio.h>

#if defined __WIN32__ || defined WIN32 || defined _Windows || defined _WIN32
typedef unsigned int u_int32_t;
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include <Utils/Socket/vjInetAddr.h>


// ----------------------------------------------------------------------------
// Get the protocol family of this address structure.
// XXX: Are raw sockets allowed with WinSock, and if so, what is the PF value?
// ----------------------------------------------------------------------------
vjSocketTypes::Domain
vjInetAddr::getFamily (void) const {
    vjSocketTypes::Domain family;

    switch (m_addr.sin_family) {
#ifdef PF_LOCAL
      case PF_LOCAL:
#else
      case PF_UNIX:
#endif
        family = vjSocketTypes::LOCAL;
        break;
      case PF_INET:
        family = vjSocketTypes::INET;
        break;
      case PF_INET6:
        family = vjSocketTypes::INET6;
        break;
#ifndef _WINSOCKAPI_
#ifdef PF_LINK
      case PF_LINK:
#else
      case PF_RAW:
#endif
        family = vjSocketTypes::LINK;
        break;
#endif	/* ! _WINSOCKAPI_ */
    }

    return family;
}

// ----------------------------------------------------------------------------
// Set the protocol family of this address structure.
// XXX: Are raw sockets allowed with WinSock, and if so, what is the PF value?
// ----------------------------------------------------------------------------
void
vjInetAddr::setFamily (const vjSocketTypes::Domain family) {
    switch (family) {
      case vjSocketTypes::LOCAL:
#ifdef PF_LOCAL
        m_addr.sin_family = PF_LOCAL;
#else
        m_addr.sin_family = PF_UNIX;
#endif
        break;
      case vjSocketTypes::INET:
        m_addr.sin_family = PF_INET;
        break;
      case vjSocketTypes::INET6:
        m_addr.sin_family = PF_INET6;
        break;
#ifndef _WINSOCKAPI_
      case vjSocketTypes::LINK:
#ifdef PF_LINK
        m_addr.sin_family = PF_LINK;
#else
        m_addr.sin_family = PF_RAW;
#endif
        break;
#endif	/* ! _WINSOCKAPI_ */
      default:
        fprintf(stderr,
                "[vjInetAddr] ERROR: Unknown socket family value %d\n",
                family);
        break;
    }
}

// ----------------------------------------------------------------------------
// Get the IP address associated with this structure as a human-readable
// string.
// ----------------------------------------------------------------------------
std::string
vjInetAddr::getAddressString (void) const {
    char ip_addr[sizeof("255.255.255.255")];
    std::string ip_str;
    union {
        char c[sizeof(u_int32_t)];
        u_int32_t value;
    } addr;

    // XXX: This isn't working quite right on little endian systems...
    addr.value = m_addr.sin_addr.s_addr;

    // I don't know why Win32 has this stupid _snprintf() function.  Maybe I
    // am missing something because I'm not a Windows programmer...
#ifdef WIN32
    _snprintf(ip_addr, sizeof(ip_addr), "%u.%u.%u.%u", addr.c[0], addr.c[1],
              addr.c[2], addr.c[3]);
#else
    snprintf(ip_addr, sizeof(ip_addr), "%u.%u.%u.%u", addr.c[0], addr.c[1],
             addr.c[2], addr.c[3]);
#endif
    ip_str = ip_addr;

    return ip_str;
}
