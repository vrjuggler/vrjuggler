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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _VPR_SOCKET_BASIC_OPT_H_
#define _VPR_SOCKET_BASIC_OPT_H_

#include <vpr/vprConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/IOException.h>
#include <vpr/IO/Socket/SocketOptions.h>

namespace vpr
{

/** \class SocketBasicOpt SocketBasicOpt.h vpr/IO/Socket/SocketBasicOpt.h
 *
 * Options common to all types of sockets.
 */
class VPR_CLASS_API SocketBasicOpt : public vpr::SocketOptionWrapper
{
public:
   /**
    * @throws vpr::IOException if the operation failed.
    */
   bool getKeepAlive() const
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::KeepAlive, option);

      return option.keep_alive;
   }

   /**
    * @throws vpr::IOException if the operation failed.
    */
   void setKeepAlive(const bool enableVal)
   {
      vpr::SocketOptions::Data option;
      option.keep_alive = enableVal;
      setOption(vpr::SocketOptions::KeepAlive, option);
   }

   /**
    * @throws vpr::IOException if the operation failed.
    */
   void getLingerOnClose(bool& enabled, int& lingerSec) const
   {
      vpr::SocketOptions::Data opt;

      getOption(vpr::SocketOptions::Linger, opt);

      enabled   = opt.linger.enabled;
      lingerSec = opt.linger.seconds;
   }

   /**
    * @throws vpr::IOException if the operation failed.
    */
   void setLingerOnClose(const bool enableVal,
                         const int lingerSec)
   {
      vpr::SocketOptions::Data opt;

      opt.linger.enabled = enableVal;
      opt.linger.seconds = lingerSec;

      setOption(vpr::SocketOptions::Linger, opt);
   }

   /**
    * @throws vpr::IOException if the operation failed.
    */
   size_t getRecvBufferSize() const
   {
      vpr::SocketOptions::Data opt;

      getOption(vpr::SocketOptions::RecvBufferSize, opt);

      return opt.recv_buffer_size;
   }

   /**
    *@throws vpr::IOException if the operation failed.
    */
   void setRecvBufferSize(const Int32 size)
   {
      vpr::SocketOptions::Data opt;

      opt.recv_buffer_size = size;

      setOption(vpr::SocketOptions::RecvBufferSize, opt);
   }

   /**
    *@throws vpr::IOException if the operation failed.
    */
   size_t getSendBufferSize() const
   {
      vpr::SocketOptions::Data opt;

      getOption(vpr::SocketOptions::SendBufferSize, opt);

      return opt.send_buffer_size;
   }

   /**
    *@throws vpr::IOException if the operation failed.
    */
   void setSendBufferSize(const Int32 size)
   {
      vpr::SocketOptions::Data opt;

      opt.send_buffer_size = size;

      setOption(vpr::SocketOptions::SendBufferSize, opt);
   }

   /**
    *@throws vpr::IOException if the operation failed.
    */
   bool getReuseAddr() const
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::ReuseAddr, option);

      return option.reuse_addr;
   }

   /**
    * Enables reuse of the address that will be bound by the socket.
    *
    * @pre The socket has been opened, but bind() has not been called.
    * @throws vpr::IOException if the operation failed.
    */
   void setReuseAddr(const bool enableVal)
   {
      vpr::SocketOptions::Data option;
      option.reuse_addr = enableVal;
      setOption(vpr::SocketOptions::ReuseAddr, option);
   }
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_BASIC_OPT_H_ */
