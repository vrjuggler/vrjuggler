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

#ifndef _VPR_DOMAIN_H_
#define _VPR_DOMAIN_H_

// The following define "domains" supported by VPR.  These are used to
// determine which platform specific headers should be included when
// compiling.  See vpr/Thread/Thread.h for an example of these values are
// used with the domain settings defined below.
#define VPR_DOMAIN_NONE       0
#define VPR_DOMAIN_NSPR       1
#define VPR_DOMAIN_POSIX      2
#define VPR_DOMAIN_IRIX_SPROC 3

// This is the VPR I/O domain.  It defines all the platform-independent types
// used for doing I/O (sockets and serial ports mostly) in VPR.
#ifndef _IO_DOMAIN_DEFINED
namespace vpr
{
   template<class> class Selector_t;
   template<class> class SerialPort_t;

   template<class BASE_ONE, class BASE_TWO> class IOStatsStrategyAdapter;

#ifdef VPR_USE_NSPR
#  define VPR_IO_DOMAIN_INCLUDE VPR_DOMAIN_NSPR

   struct SocketConfiguration
   {
      typedef class SocketImplNSPR           SocketImpl;
      typedef class SocketDatagramImplNSPR   SocketDatagramImpl;
      typedef class SocketStreamImplNSPR     SocketStreamImpl;
      typedef class IOStatsStrategyAdapter<class BaseIOStatsStrategy, class BandwidthIOStatsStrategy>     SocketIOStatsStrategy;
   };

   typedef class InetAddrNSPR InetAddr;
   typedef class IOSysNSPR    IOSys;

   typedef class Selector_t<class SelectorImplNSPR> Selector;

#  ifdef VPR_OS_Win32
      typedef SerialPort_t<class SerialPortImplWin32>   SerialPort;
#  else
      typedef SerialPort_t<class SerialPortImplTermios> SerialPort;
#  endif  /* VPR_OS_Win32 */

/* POSIX and SPROC */
#else
#  define VPR_IO_DOMAIN_INCLUDE VPR_DOMAIN_POSIX

   struct SocketConfiguration
   {
      typedef class SocketImplBSD           SocketImpl;
      typedef class SocketDatagramImplBSD   SocketDatagramImpl;
      typedef class SocketStreamImplBSD     SocketStreamImpl;
      typedef IOStatsStrategyAdapter<class BaseIOStatsStrategy, class BandwidthIOStatsStrategy>     SocketIOStatsStrategy;
   };

   typedef class InetAddrBSD InetAddr;
   typedef class IOSysUnix   IOSys;

   typedef class Selector_t<class SelectorImplBSD>         Selector;
   typedef class SerialPort_t<class SerialPortImplTermios> SerialPort;

#endif /* ifdef VPR_USE_NSPR */

}
#endif /* ifdef _IO_DOMAIN_DEFINED */

// This is the threading (and synchronization) domain.  It defins the types
// used for platform-independent constructs for doing multi-threaded
// programming.
#ifndef _THREAD_DOMAIN_DEFINED
namespace vpr
{
#ifdef VPR_USE_NSPR
#  define VPR_THREAD_DOMAIN_INCLUDE VPR_DOMAIN_NSPR

   typedef class SystemNSPR    System;
   typedef class CondVarNSPR   CondVar;
   typedef class MutexNSPR     Mutex;
   typedef class RWMutexNSPR   RWMutex;
   typedef class SemaphoreNSPR Semaphore;
   typedef class ThreadNSPR    Thread;
//   typedef class ThreadKeyNSPR KeyId;

/* POSIX and SPROC */
#else
   typedef class SystemPosix System;

#  ifdef VPR_USE_IRIX_SPROC
#     define VPR_THREAD_DOMAIN_INCLUDE VPR_DOMAIN_IRIX_SPROC

      typedef class BarrierSGI     Barrier;
      typedef class CondVarGeneric CondVar;
      typedef class MutexSGI       Mutex;
      typedef class SemaphoreSGI   Semaphore;

      typedef struct cancel_state  cancel_state_t;
      typedef class ThreadSGI      Thread;
      typedef class ThreadKeySGI   KeyId;
#  else
#     define VPR_THREAD_DOMAIN_INCLUDE VPR_DOMAIN_POSIX

      typedef class CondVarPosix   CondVar;
      typedef class MutexPosix     Mutex;
      typedef class SemaphorePosix Semaphore;

      typedef int                  cancel_state_t;
      typedef class ThreadPosix    Thread;
      typedef class ThreadKeyPosix KeyId;
#  endif /* ifdef VPR_USE_IRIX_SPROC */

#endif /* ifdef VPR_USE_NSPR */
}
#endif

#ifndef VPR_IO_DOMAIN_INCLUDE
#define VPR_IO_DOMAIN_INCLUDE VPR_DOMAIN_NONE
#endif

#ifndef VPR_THREAD_DOMAIN_INCLUDE
#define VPR_THREAD_DOMAIN_INCLUDE VPR_DOMAIN_NONE
#endif

#endif  /* ifndef _VPR_DOMAIN_H_ */
