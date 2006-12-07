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

/**
 * \file
 *
 * @note This file MUST be included by Thread.h, not the other way around.
 */

#ifndef _VPR_THREAD_KEY_WIN32_H_
#define _VPR_THREAD_KEY_WIN32_H_

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <boost/function.hpp>


namespace vpr
{

// Key destructor function type.
typedef boost::function<void (void*)> KeyDestructor;

/** \class ThreadKeyWin32 ThreadKeyWin32.h vpr/Thread/Thread.h
 *
 * Wrapper around Win32 thread local storage (thread-specific data). It is
 * typedef'd to vpr::ThreadKey.
 *
 * @note This class was originally part of VR Juggler 1.0 and was brought back
 *       into VPR in version 1.1.36.
 */
class ThreadKeyWin32
{
public:
   /**
    * Creates a key that knows how to delete itself using a function pointer.
    *
    * @post A key is created and is associated with the specified destructor
    *       function and argument.
    *
    * @param destructor The destructor function for the key.
    */
   ThreadKeyWin32(KeyDestructor destructor = NULL)
      : mKeyID(0xffffffff)
   {
      keycreate(destructor);
   }

   /**
    * Releases this key.
    */
   ~ThreadKeyWin32()
   {
      keyfree();
   }

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
    * @return 0 is returned upon successful completion.
    *         -1 is returned if an error occurs.
    */
   int keycreate(KeyDestructor destructor = NULL)
   {
      if ( 0xffffffff != mKeyID )
      {
         keyfree();
      }

      mDestructor = destructor;

      int retval(0);
      const DWORD key_id = TlsAlloc();

      if ( TLS_OUT_OF_INDEXES == key_id )
      {
         perror("Could not allocate thread local storage");
         retval = -1;
      }
      else
      {
         mKeyID = key_id;
      }

      return retval;
   }

   /**
    * Frees up this key so that other threads may reuse it.
    *
    * @pre This key must have been properly created using the keycreate()
    *      member function.
    * @post This key is destroyed using the destructor function previously
    *       associated with it, and its resources are freed.
    *
    * @return 0 is returned upon successful completion.
    *         -1 is returned if an error occurs.
    */
   int keyfree()
   {
      int retval(0);

      if ( 0xffffffff != mKeyID )
      {
         mDestructor();

         if ( ! TlsFree(mKeyID) )
         {
            perror("Could not free thread local storage");
            retval = -1;
         }

         mKeyID = 0xffffffff;
      }

      return retval;
   }

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
    * @return 0 is returned upon successful completion.
    *         -1 is returned if an error occurs.
    */
   int setspecific(void* value)
   {
      int retval(0);
      if ( ! TlsSetValue(mKeyID, value) )
      {
         perror("Could not store thread local storage");
         retval = -1;
      }

      return retval;
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
    *
    * @return 0 is returned upon successful completion.
    *         -1 is returned if an error occurs.
    */
   int getspecific(void** valuep)
   {
      *valuep = TlsGetValue(mKeyID);
      return 0;
   }

private:
   DWORD         mKeyID;        /**< Thread local storage ID */
   KeyDestructor mDestructor;   /**< Destructor for thread local storage */
};

} // End of vpr namespace


#endif	/* _VPR_THREAD_KEY_WIN32_H_ */
