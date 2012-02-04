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

/**
 * \file
 *
 * @note This file MUST be included by Thread.h, not the other way around.
 */

#ifndef _VPR_THREAD_KEY_WIN32_H_
#define _VPR_THREAD_KEY_WIN32_H_

#include <vpr/vprConfig.h>

#include <cstring>
#include <sstream>
#include <boost/function.hpp>

#include <vpr/Util/IllegalArgumentException.h>


namespace vpr
{

// Key destructor function type.
#if defined(_MSC_VER) && _MSC_VER <= 1300
typedef boost::function1<void, void*> KeyDestructor;
#else
typedef boost::function<void (void*)> KeyDestructor;
#endif

/** \class ThreadKeyWin32 ThreadKeyWin32.h vpr/Thread/Thread.h
 *
 * Wrapper around Win32 thread local storage (thread-specific data). It is
 * typedef'd to vpr::ThreadKey.
 *
 * @note This class was originally part of VR Juggler 1.0 and was brought back
 *       into VPR in version 1.1.36.
 */
class VPR_API ThreadKeyWin32
{
public:
   /**
    * Creates a key that knows how to delete itself using a function pointer.
    *
    * @post A key is created and is associated with the specified destructor
    *       function and argument.
    *
    * @param destructor The destructor function for the key.
    *
    * @throw vpr::ResourceException is thrown if the thread-specific key
    *        could not be created.
    */
   ThreadKeyWin32(KeyDestructor destructor = NULL);

   /**
    * Releases this key.
    */
   ~ThreadKeyWin32();

   /**
    * Allocates a key that is used to identify data that is specific to
    * each thread in the process, is global to all threads in the process
    * and is destroyed using the spcefied destructor function that takes a
    * single argument.
    *
    * @post A key is created and is associated with the specified destructor
    *       function.
    *
    * @param destructor A pointer to the destructor function for the key.
    *                   This parameter is optional and defaults to NULL.
    *
    * @throw vpr::ResourceException is thrown if the thread-specific key
    *        could not be created.
    */
   void keycreate(KeyDestructor destructor = NULL);

   /**
    * Frees up this key so that other threads may reuse it.
    *
    * @pre This key must have been properly created using the keycreate()
    *      member function.
    * @post This key is destroyed using the destructor function previously
    *       associated with it, and its resources are freed.
    *
    * @throw vpr::IllegalArgumentException is thrown if this key is invalid
    *        and therefore cannot be deleted.
    */
   void keyfree();

   /**
    * Binds value to the thread-specific data key for the calling thread.
    *
    * @pre The specified key must have been properly created using the
    *      keycreate() member function.
    * @post The specified value is associated with the key for the calling
    *       thread.
    *
    * @param value Address containing data to be associated with the
    *              specified key for the current thread.
    *
    * @throw vpr::IllegalArgumentException is thrown if this key is invalid
    *        and therefore cannot have a value associated with it.
    */
   void setspecific(void* value)
   {
      if ( ! TlsSetValue(mKeyID, value) )
      {
#if defined(_MSC_VER) && _MSC_VER < 1400
         const char* msg = std::strerror(errno);
#else
         char msg[256];
         strerror_s(msg, sizeof(msg), errno);
#endif
         std::ostringstream msg_stream;
         msg_stream << "Could not set thread local storage: " << msg;
         throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
      }
   }

   /**
    * Stores the current value bound to key for the calling thread into
    * the location pointed to by valuep.
    *
    * @pre The specified key must have been properly created using the
    *      keycreate() member function.
    * @post The value associated with the key is obtained and stored in the
    *       pointer valuep so that the caller may work with it.
    *
    * @param valuep Address of the current data value associated with the
    *               key.
    */
   void getspecific(void** valuep)
   {
      *valuep = TlsGetValue(mKeyID);
   }

private:
   DWORD         mKeyID;        /**< Thread local storage ID */
   KeyDestructor mDestructor;   /**< Destructor for thread local storage */
};

} // End of vpr namespace


#endif	/* _VPR_THREAD_KEY_WIN32_H_ */
