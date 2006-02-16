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

/**
 * \file
 *
 * @note This file MUST be included by Thread.h, not the other way around.
 */

#ifndef _VPR_THREAD_KEY_POSIX_H_
#define _VPR_THREAD_KEY_POSIX_H_

#include <vpr/vprConfig.h>

#include <boost/function.hpp>
#include <pthread.h>
#include <sys/types.h>


namespace vpr
{

// Key destructor function type.
typedef void (*KeyDestructor)(void*);


/** \class ThreadKeyPosix ThreadKeyPosix.h vpr/Thread/Thread.h
 *
 * Wrapper around pthread keys (thread-specific data).  It is typedef'd to
 * vpr::ThreadKey.
 */
class ThreadKeyPosix
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
   ThreadKeyPosix(KeyDestructor destructor = NULL)
   {
      keycreate(destructor);
   }

   /**
    * Releases this key.
    */
   ~ThreadKeyPosix()
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
    * @return -1 is returned if an error occurs.
    */
   int keycreate(KeyDestructor destructor = NULL)
   {
      return pthread_key_create(&mKeyID, destructor);
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
    * @return -1 is returned if an error occurs.
    *
    * @note This is not currently supported with Pthreads Draft 4.
    */
   int keyfree()
   {
      return pthread_key_delete(mKeyID);
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
    * @return -1 is returned if an error occurs.
    */
   int setspecific(void* value)
   {
      return pthread_setspecific(mKeyID, value);
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
    * @return -1 is returned if an error occurs.
    */
   int getspecific(void** valuep)
   {
      *valuep = pthread_getspecific(mKeyID);
      return 0;
   }

private:
   pthread_key_t mKeyID;        /**< Thread key ID */
};

} // End of vpr namespace


#endif  /* _VPR_THREAD_KEY_POSIX_H_ */
