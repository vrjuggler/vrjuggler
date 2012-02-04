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
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_STREAM_LOCK_H_
#define _VPR_STREAM_LOCK_H_
//#pragma once

#include <vpr/vprConfig.h>
#include <vpr/Sync/Mutex.h>


namespace vpr
{

/** \class StreamLock StreamLock.h vpr/Util/StreamLock.h
 *
 * Class to lock a stream for multi-process output.
 * This class can be used to share a stream between two processes.
 * It is used by simply outputing in the stream a vpr::StreamLock constructed
 * using a mutex.
 *
 * Example:
 * \code
 * std::cout << vpr::StreamLock(mutex) << "Hello world" << std::flush
 *           << vpr::StreamUnLock(mutex);
 * \endcode
 */
class StreamLock
{
public:
   explicit StreamLock(Mutex& mutex) : mMutex(mutex)
   {;}

   friend VPR_API std::ostream& operator<<(std::ostream&, const StreamLock&);

private:
   Mutex& mMutex;
};

/** \class StreamUnLock StreamLock.h vpr/Util/StreamLock.h
 *
 * Class to unlock a stream that has been previously locked.
 * This class can be used to share a stream between two processes
 * It is used by simply outputing in the stream a vpr::StreamUnLock
 * constructed using a mute.
 *
 * Examle:
 * \code
 * std::cout << vpr::StreamLock(mutex) << "Hello world" << std::flush
 *           << vpr::StreamUnLock(mutex);
 * \endcode
 */
class StreamUnLock
{
public:
   explicit StreamUnLock(Mutex& mutex) : mMutex(mutex)
   {;}

   friend VPR_API std::ostream& operator<<(std::ostream&, const StreamUnLock&);

private:
   Mutex& mMutex;
};

} // End of vpr namespace


#endif
