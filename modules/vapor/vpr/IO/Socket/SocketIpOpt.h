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

#ifndef _VPR_SOCKET_IP_OPT_H_
#define _VPR_SOCKET_IP_OPT_H_

#include <vpr/IO/Socket/SocketBasicOpt.h>


namespace vpr
{

/** \class SocketIpOpt SocketIpOpt.h vpr/IO/Socket/SocketIpOpt.h
 *
 * IP-level options for sockets.
 */
class VPR_CLASS_API SocketIpOpt : public SocketBasicOpt
{
public:
   /**
    *
    */
   void getTypeOfService(vpr::SocketOptions::TypeOfService& tos)
      throw (IOException)
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::IpTypeOfService, option);
      tos = option.type_of_service;
   }

   /**
    *
    */
   void setTypeOfService(const vpr::SocketOptions::TypeOfService& tos)
      throw (IOException)
   {
      vpr::SocketOptions::Data option;
      option.type_of_service = tos;
      setOption(vpr::SocketOptions::IpTypeOfService, option);
   }

   /**
    *
    */
   void getTimeToLive(vpr::Int32& ttl) throw (IOException)
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::IpTimeToLive, option);
      ttl = option.ip_ttl;
   }

   /**
    *
    */
   void setTimeToLive(const vpr::Int32 ttl) throw (IOException)
   {
      vpr::SocketOptions::Data option;
      option.ip_ttl = ttl;
      setOption(vpr::SocketOptions::IpTimeToLive, option);
   }
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_IP_OPT_H_ */
