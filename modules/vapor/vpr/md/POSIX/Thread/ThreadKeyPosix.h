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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

/*
 * --------------------------------------------------------------------------
 * NOTES:
 *    - This file (vprThreadKeyPosix.h) MUST be included by vprThread.h, not
 *      the other way around.
 * --------------------------------------------------------------------------
 */

#ifndef _VPR_THREAD_KEY_POSIX_H_
#define _VPR_THREAD_KEY_POSIX_H_

#include <vpr/vprConfig.h>
#include <pthread.h>
#include <sys/types.h>

#include <vpr/Thread/ThreadFunctor.h>


namespace vpr
{

// Key destructor function type.
#ifdef _PTHREADS_DRAFT_4
typedef pthread_destructor_t KeyDestructor;
#else
typedef thread_func_t        KeyDestructor;
#endif


/**
 * Wrapper around pthread keys (thread-specific data).
 */
class ThreadKeyPosix
{
public:
   /// Default constructor.
   ThreadKeyPosix ()
   {
      keycreate(NULL);
   }

   /// Create a key that knows how to delete itself using a function pointer.
   ThreadKeyPosix (thread_func_t destructor, void* arg)
   {
      keycreate(destructor, arg);
   }

   /// Create a key that knows how to delete itself using a functor.
   ThreadKeyPosix (BaseThreadFunctor* destructor)
   {
      keycreate(destructor);
   }

   /**
    * Releases this key.
    */
   ~ThreadKeyPosix (void)
   {
      keyfree();
   }

   /**
    * Allocates a <keyp> that is used to identify data that is specific to
    * each thread in the process, is global to all threads in the process
    * and is destroyed using the spcefied destructor function that takes a
    * single argument.
    *
    * @pre None.
    * @post A key is created and is associated with the specified destructor
    *       function and argument.
    *
    * @param dest_func The destructor function for the key.
    * @param arg       Argument to be passed to destructor (optional).
    *
    * @return 0 is returned upown successful completion.<br>
    *         -1 is returned if an error occurs.
    *
    * @note Use this routine to construct the key destructor function if
    *       it requires arguments.  Otherwise, use the two-argument version
    *       of keycreate().
    */
   int keycreate (thread_func_t destructor, void* arg)
   {
      // XXX: Memory leak!
      ThreadNonMemberFunctor *NonMemFunctor = new ThreadNonMemberFunctor(destructor, arg);

      return keycreate(NonMemFunctor);
   }

   /**
    * Allocates a <keyp> that is used to identify data that is specific to
    * each thread in the process, is global to all threads in the process
    * and is destroyed by the specified destructor function.
    *
    * @pre None.
    * @post A key is created and is associated with the specified
    *       destructor function.
    *
    * @param desctructor Procedure to be called to destroy a data value
    *                    associated with the key when the thread terminates.
    *
    * @return 0 is returned upown successful completion.<br>
    *         -1 is returned if an error occurs.
    */
   int keycreate (BaseThreadFunctor* destructor)
   {
#ifdef _PTHREADS_DRAFT_4
      return pthread_keycreate(&keyID, (KeyDestructor) destructor);
#else
      return pthread_key_create(&keyID, (KeyDestructor) destructor);
#endif
   }

   /**
    * Frees up this key so that other threads may reuse it.
    *
    * @pre This key must have been properly created using the keycreate()
    *      member function.
    * @post This key is destroyed using the destructor function previously
    *       associated with it, and its resources are freed.
    *
    * @return 0 is returned upown successful completion.<br>
    *         -1 is returned if an error occurs.
    *
    * @note This is not currently supported with Pthreads Draft 4.
    */
   int keyfree (void)
   {
#ifdef _PTHREADS_DRAFT_4
      cerr << "keyfree() not supported with this POSIX threads "
           << "implementation\n";

      return -1;
#else
      return pthread_key_delete(keyID);
#endif
   }

   /**
    * Binds value to the thread-specific data key, <key>, for the calling
    * thread.
    *
    * @pre The specified key must have been properly created using the
    *      keycreate() member function.
    * @post The specified value is associated with the key for the calling
    *       thread.
    *
    * @param value Address containing data to be associated with the
    *              specified key for the current thread.
    *
    * @return 0 is returned upown successful completion.<br>
    *         -1 is returned if an error occurs.
    */
   int setspecific (void* value)
   {
      return pthread_setspecific(keyID, value);
   }

   /**
    * Stores the current value bound to <key> for the calling thread into
    * the location pointed to by <valuep>.
    *
    * @pre The specified key must have been properly created using the
    *      keycreate() member function.
    * @post The value associated with the key is obtained and stored in the
    *       pointer valuep so that the caller may work with it.
    *
    * @param valuep Address of the current data value associated with the
    *               key.
    *
    * @return 0 is returned upown successful completion.<br>
    *         -1 is returned if an error occurs.
    */
   int getspecific (void** valuep)
   {
#ifdef _PTHREADS_DRAFT_4
      return pthread_getspecific(keyID, valuep);
#else
      *valuep = pthread_getspecific(keyID);

      return 0;
#endif
   }

private:
   pthread_key_t keyID;        /**< Thread key ID */
};

} // End of vpr namespace


#endif  /* _VPR_THREAD_KEY_POSIX_H_ */
