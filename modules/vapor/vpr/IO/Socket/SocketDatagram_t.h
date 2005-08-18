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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VPR_SOCKET_DATAGRAM_BRIDGE_H_
#define _VPR_SOCKET_DATAGRAM_BRIDGE_H_
// NOTE: this is the bridge class for use with SocketDatagram.h

#include <vpr/vprConfig.h>

#include <vpr/IO/Socket/Socket_t.h>
#include <vpr/IO/Socket/SocketDatagramOpt.h>

#include <boost/smart_ptr.hpp>


namespace vpr
{

/** \class SocketDatagram_t SocketDatagram_t.h vpr/IO/Socket/SocketDatagram_t.h
 *
 * Datagram socket interface.  Given a SocketConfiguration declaration, this
 * class is typedef'd to vpr::SocketDatagram.
 *
 * @param SocketConfig_ The SocketConfiguration type that matches the
 *                      platform-specific socket implementation to use.
 *
 * @see vpr::SocketDatagramImplNSPR, vpr::SocketDatagramImplBSD
 */
template<class SocketConfig_>
class SocketDatagram_t
   : public Socket_t<SocketConfig_>
   , public SocketDatagramOpt
{
public:
   typedef SocketConfig_ Config;
   typedef typename Config::SocketDatagramImpl SocketDatagramImpl;

   /**
    * Default constructor.
    */
   SocketDatagram_t()
      : Socket_t<Config>()
      , SocketDatagramOpt()
      , mSocketDgramImpl()
   {
      mSocketDgramImpl =
         boost::shared_ptr<SocketDatagramImpl>(new SocketDatagramImpl);
      Socket_t<SocketConfig_>::mSocketImpl = mSocketDgramImpl;
   }

   /**
    * Constructor.  This takes a reference to a vpr::InetAddr object giving
    * the local socket address and a reference to a vpr::InetAddr object
    * giving the remote address.
    *
    * @pre \p localAddr and \p remoteAddr are references to valid vpr::InetAddr
    *      objects.
    * @post A socket is created using \p localAddr and \p remoteAddr.
    *
    * @param localAddr  Our local address.
    * @param remoteAddr The remote address with which we will communicate.
    */
   SocketDatagram_t(const vpr::InetAddr& localAddr,
                    const vpr::InetAddr& remoteAddr)
      : Socket_t<Config>()
      , SocketDatagramOpt()
   {
      mSocketDgramImpl =
         boost::shared_ptr<SocketDatagramImpl>(new SocketDatagramImpl(localAddr,
                                                                      remoteAddr));
      Socket_t<SocketConfig_>::mSocketImpl = mSocketDgramImpl;
   }

   /**
    * Copy constructor.
    *
    * @param sock The source socket object to be copied.
    */
   SocketDatagram_t(const SocketDatagram_t& sock)
      : Socket_t<Config>()
      , SocketDatagramOpt()
      , mSocketDgramImpl(sock.mSocketDgramImpl)
   {
      Socket_t<SocketConfig_>::mSocketImpl = mSocketDgramImpl;
   }

   /**
    * Destructor.  This currently does nothing.
    */
   virtual ~SocketDatagram_t()
   {
      /* nothing */ ;
   }

   /**
    * Receives a message from some source.  The source's address is writen
    * into the by-reference parameter \p from.
    */
   void recvfrom(void* msg, const vpr::Uint32 len,
                 vpr::InetAddr& from, vpr::Uint32& bytesRead,
                 const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException)
   {
      mSocketDgramImpl->recvfrom(msg, len, from, bytesRead, timeout);
   }

   /**
    * Receives a message from some source.  The source's address is writen
    * into the by-reference parameter \p from.
    */
   void recvfrom(std::string& msg, const vpr::Uint32 len,
                 vpr::InetAddr& from, vpr::Uint32& bytesRead,
                 const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException)
   {
      msg.resize(len);
      memset(&msg[0], '\0', msg.size());

      recvfrom((void*) &msg[0], msg.size(), from, bytesRead, timeout);
   }

   /**
    * Receives a message from some source.  The source's address is writen
    * into the by-reference parameter \p from.
    */
   void recvfrom(std::vector<vpr::Uint8>& msg,
                 const vpr::Uint32 len, vpr::InetAddr& from,
                 vpr::Uint32& bytesRead,
                 const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException)
   {
      msg.resize(len);

      memset(&msg[0], '\0', msg.size());
      recvfrom((void*) &msg[0], msg.size(), from, bytesRead, timeout);

      // Size it down if needed, if (bytesRead==len), then resize does
      // nothing.
      msg.resize(bytesRead);
   }

   /**
    * Sends a message to the designated recipient.
    */
   void sendto(const void* msg, const vpr::Uint32 len,
               const vpr::InetAddr& to, vpr::Uint32& bytesSent,
               const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException)
   {
      mSocketDgramImpl->sendto(msg, len, to, bytesSent, timeout);
   }

   /**
    * Sends a message to the designated recipient.
    */
   void sendto(const std::string& msg, const vpr::Uint32 len,
               const vpr::InetAddr& to, vpr::Uint32& bytesSent,
               const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException)
   {
      vprASSERT(len <= msg.size() && "Length is bigger than data given");
      sendto(msg.c_str(), len, to, bytesSent, timeout);
   }

   /**
    * Sends a message to the designated recipient.
    */
   void sendto(const std::vector<vpr::Uint8>& msg,
               const vpr::Uint32 len, const vpr::InetAddr& to,
               vpr::Uint32& bytesSent,
               const vpr::Interval timeout = vpr::Interval::NoTimeout)
      throw (IOException)
   {
      vprASSERT(len <= msg.size() && "Length is bigger than data given");
      sendto((const void*) &msg[0], len, to, bytesSent, timeout);
   }

protected:
   virtual void getOption(const vpr::SocketOptions::Types option,
                          struct vpr::SocketOptions::Data& data)
      const throw (SocketException)
   {
      return mSocketDgramImpl->getOption(option, data);
   }

   virtual void setOption(const vpr::SocketOptions::Types option,
                          const struct vpr::SocketOptions::Data& data)
      throw (SocketException)
   {
      mSocketDgramImpl->setOption(option, data);
   }

// Put in back door for simulator
#if VPR_IO_DOMAIN_INCLUDE == VPR_DOMAIN_SIMULATOR
public:
#endif
   /// Platform-specific datagram socket implementation object
   boost::shared_ptr<SocketDatagramImpl> mSocketDgramImpl;
};

} // End of vpr namespace


#endif  /* _VJ_SOCKET_DATAGRAM_BRIDGE_H_ */
