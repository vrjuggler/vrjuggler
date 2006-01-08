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
    *
    */
   void getKeepAlive(bool& enabled) throw (IOException)
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::KeepAlive, option);

      enabled = option.keep_alive;
   }

   /**
    *
    */
   void setKeepAlive(const bool enableVal) throw (IOException)
   {
      vpr::SocketOptions::Data option;
      option.keep_alive = enableVal;
      setOption(vpr::SocketOptions::KeepAlive, option);
   }

   /**
    *
    */
   void getLingerOnClose(bool& enabled, int& lingerSec) throw (IOException)
   {
      vpr::SocketOptions::Data opt;

      getOption(vpr::SocketOptions::Linger, opt);

      enabled   = opt.linger.enabled;
      lingerSec = opt.linger.seconds;
   }

   /**
    *
    */
   void setLingerOnClose(const bool enableVal,
                         const int lingerSec) throw (IOException)
   {
      vpr::SocketOptions::Data opt;

      opt.linger.enabled = enableVal;
      opt.linger.seconds = lingerSec;

      setOption(vpr::SocketOptions::Linger, opt);
   }

   /**
    *
    */
   void getRecvBufferSize(size_t& size) throw (IOException)
   {
      vpr::SocketOptions::Data opt;
      
      try
      {
         getOption(vpr::SocketOptions::RecvBufferSize, opt);
      }
      catch (IOException ex)
      {
         size = 0;
         throw;
      }
      size = opt.recv_buffer_size;
   }

   /**
    *
    */
   void setRecvBufferSize(const Int32 size) throw (IOException)
   {
      vpr::SocketOptions::Data opt;

      opt.recv_buffer_size = size;

      setOption(vpr::SocketOptions::RecvBufferSize, opt);
   }

   /**
    *
    */
   void getSendBufferSize(size_t& size) throw (IOException)
   {
      vpr::SocketOptions::Data opt;
      
      try
      {
         getOption(vpr::SocketOptions::SendBufferSize, opt);
      }
      catch (IOException ex)
      {
         size = 0;
         throw;
      }
      size = opt.send_buffer_size;
   }

   /**
    *
    */
   void setSendBufferSize(const Int32 size) throw (IOException)
   {
      vpr::SocketOptions::Data opt;

      opt.send_buffer_size = size;

      setOption(vpr::SocketOptions::SendBufferSize, opt);
   }

   /**
    *
    */
   void getReuseAddr(bool& enabled) throw (IOException)
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::ReuseAddr, option);

      enabled = option.reuse_addr;
   }

   /**
    * Enables reuse of the address that will be bound by the socket.
    *
    * @pre The socket has been opened, but bind() has not been called.
    */
   void setReuseAddr(const bool enableVal) throw (IOException)
   {
      vpr::SocketOptions::Data option;
      option.reuse_addr = enableVal;
      setOption(vpr::SocketOptions::ReuseAddr, option);
   }
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_BASIC_OPT_H_ */
