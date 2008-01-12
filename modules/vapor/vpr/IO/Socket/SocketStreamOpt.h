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

#ifndef _VPR_SOCKET_STREAM_OPT_H_
#define _VPR_SOCKET_STREAM_OPT_H_

#include <vpr/vprConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/Socket/SocketOptions.h>
#include <vpr/IO/Socket/SocketIpOpt.h>


namespace vpr
{

/** \class SocketStreamOpt SocketStreamOpt.h vpr/IO/Socket/SocketStreamOpt.h
 *
 * Options for stream sockets.
 */
class VPR_CLASS_API SocketStreamOpt : public vpr::SocketOptionWrapper
{
public:
   /**
    * Default constructor.  This does nothing and exists only so that we can
    * build a DLL on Win32.
    */
   SocketStreamOpt();

   /**
    * Default destructor.  This does nothing and exists only so that we can
    * build a DLL on Win32.
    */
   virtual ~SocketStreamOpt();

   /**
    *
    */
   size_t getMaxSegmentSize() const
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::MaxSegment, option);
      return option.max_segment;
   }

   /**
    *
    */
   void setMaxSegmentSize(const vpr::Int32 size)
   {
      vpr::SocketOptions::Data option;
      option.max_segment = size;
      setOption(vpr::SocketOptions::MaxSegment, option);
   }

   /** @name Nagle Algorithm Control */
   //@{
   /**
    * Gets the current no-delay status for this socket.  If no-delay is true,
    * then the Nagle algorithm has been disabled.
    *
    * @return \c true is returned if the Nabel algorithm is \em not being used
    *         to delay the transmission of TCP segments. Otherwise, the Nagle
    *         algorithm is delaying the transmission.
    */
   bool getNoDelay() const
   {
      vpr::SocketOptions::Data option;

      getOption(vpr::SocketOptions::NoDelay, option);
      return option.no_delay;
   }

   /**
    * Sets the current no-delay status for this socket.  If no-delay is true,
    * then the Nagle algorithm will be disabled.
    *
    * @param enableVal The Boolean enable/disable state for no-delay on this
    *                  socket.
    */
   void setNoDelay(const bool enableVal)
   {
      vpr::SocketOptions::Data option;
      option.no_delay = enableVal;
      setOption(vpr::SocketOptions::NoDelay, option);
   }
   //@}

   /** @name TCP Corking Interface */
   //@{
   /**
    * Gets the state of the "no push" or "corking" for this socket. While a
    * TCP socket is in the corked state, write operations are queued for
    * later transmission when the socket is uncorked (that is, when the
    * no-push state is disabled).
    *
    * @return true is returned if this socket is currently in the no-push
    *         (corked) state. false is returned otherwise.
    *
    * @since 2.1.9
    */
   bool getNoPush() const;

   /**
    * Enables or disables the no-push (or "corked") state.
    *
    * @post If this socket was previously in the corked state and the value of
    *       \p enableVal is false, then all queued buffer writes will be
    *       performed.
    *
    * @param enableVal A boolean value indicating whether this socket should
    *                  be in the no-push (corked) state.
    *
    * @since 2.1.9
    */
   void setNoPush(const bool enableVal);
   //@}

protected:
   /** @name Socket Corking Interface */
   //@{
   /**
    * Template method used to inform the platform-specific implementation when
    * this socket is put into the no-push (corked) state. This method is only
    * used for platforms that lack TCP corking as a socket option.
    *
    * @pre setNoPush() was invoked with the enable state value set to \c true.
    *
    * @see setNoPush()
    *
    * @since 2.1.9
    */
   virtual void cork() = 0;

   /**
    * Template method used to inform the platform-specific implementation when
    * this socket is changed from the no-push (corked) state to the regular,
    * "uncorked" state. This method is only used for platforms that lack TCP
    * corking as a socket option.
    *
    * @pre setNoPush() was invoked with the enable state value set to \c false.
    *
    * @see setNoPush()
    *
    * @since 2.1.9
    */
   virtual void uncork() = 0;
   //@}

private:
   bool mCorked;
};

} // End of vpr namespace


#endif  /* _VPR_SOCKET_STREAM_OPT_H_ */
