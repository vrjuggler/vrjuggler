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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_COCOA_WRAPPER_H_
#define _VRJ_COCOA_WRAPPER_H_

#include <vrj/vrjConfig.h>

#include <vpr/Thread/TSObjectProxy.h>

#if defined(__OBJC__)
@class NSAutoreleasePool;
#else
class NSAutoreleasePool;
#endif


namespace vrj
{

/** \class CocoaWrapper CocoaWrapper.h vrj/Kernel/CocoaWrapper.h
 *
 * This class provides a bridge between the VR Juggler kernel and the
 * execution environment required by NSApplication.
 *
 * @since 2.1.21
 */
class CocoaWrapper
{
public:
   CocoaWrapper();

   ~CocoaWrapper();

   /**
    * Starts the run loop and does not return until stop() is innvoked.
    *
    * @note In the current implementation, this method never returns.
    */
   void run();

   /**
    * Stops the run loop. This must be invoked before this object is
    * deleted.
    */
   void stop();

private:
   void threadStarted();

   void threadEnded();

   boost::signals::connection mThreadStartConn;
   boost::signals::connection mThreadExitConn;

   /** @name Auto-Release Pools */
   //@{
   NSAutoreleasePool* mMainPool;        /**< Main auto-release pool */

   /** Thread-specific auto-release pools. */
   vpr::TSObjectProxy<NSAutoreleasePool*> mThreadPool;
   //@}

   bool mRunning;
};

}


#endif /* _VRJ_COCOA_WRAPPER_H_ */
