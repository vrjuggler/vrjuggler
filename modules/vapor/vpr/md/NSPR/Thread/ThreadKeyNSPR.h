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

/**
 * \file
 *
 * @note This file MUST be included by vprThread.h, not the other way around.
 */

#ifndef _VPR_THREAD_KEY_POSIX_H_
#define _VPR_THREAD_KEY_POSIX_H_

#include <vpr/vprConfig.h>
#include <prthread.h>

#include <vpr/Thread/ThreadFunctor.h>


namespace vpr
{

// Key destructor function type.
typedef PRThreadPrivateDTOR KeyDestructor;

/** \class ThreadKeyNSPR ThreadKeyNSPR.h vpr/Thread/Thread.h
 *
 * Wrapper around NSPR thread-specific data.  It is typedef'd to
 * vpr::ThreadKey.
 */
class ThreadKeyNSPR
{
public:
   /**
    * Default constructor.  After calling this, one of the keycreate()
    * overloads must be called to finish the key allocation process.
    *
    * @see keycreate
    */
   ThreadKeyNSPR()
      : mDestructor(NULL)
   {
      keycreate(NULL);
   }

   /**
    * Creates a key that knows how to delete itself using a function pointer.
    *
    * @post A key is created and is associated with the specified destructor
    *       function and argument.
    *
    * @param destructor The destructor function for the key.
    * @param arg        Argument to be passed to destructor.
    */
   ThreadKeyNSPR(thread_func_t destructor, void* arg)
      : mDestructor(NULL)
   {
      keycreate(destructor, arg);
   }

   /**
    * Creates a key that knows how to delete itself using a functor.
    *
    * @post A key is created and is associated with the specified destructor
    *       function and argument.
    *
    * @param destructor The destructor function for the key.
    */
   ThreadKeyNSPR(BaseThreadFunctor* destructor)
      : mDestructor(NULL)
   {
      keycreate(destructor);
   }

   /**
    * Releases this key.
    */
   ~ThreadKeyNSPR()
   {
      keyfree();

      if ( NULL != mDestructor )
      {
         delete mDestructor;
      }
   }

   /**
    * Allocates a key that is used to identify data that is specific to each
    * thread in the process, is global to all threads in the process, and is
    * is destroyed using the spcefied destructor function that takes a
    * single argument.
    *
    * @post A key is created and is associated with the specified destructor
    *       function and argument.
    *
    * @param destructor The destructor function for the key.
    * @param arg        Argument to be passed to destructor.
    *
    * @return 0 is returned upon successful completion.
    * @return -1 is returned if an error occurs.
    *
    * @note Use this routine to construct the key destructor function if
    *       it requires arguments.  Otherwise, use the single-argument version
    *       of keycreate().
    */
   int keycreate(thread_func_t destructor, void* arg)
   {
      mDestructor = new ThreadNonMemberFunctor(destructor, arg);
      return keycreate(mDestructor);
   }

   /**
    * Allocates a key that is used to identify data that is specific to each
    * thread in the process, is global to all threads in the process and is
    * destroyed by the specified destructor function.
    *
    * @post A key is created and is associated with the specified
    *       destructor function.
    *
    * @param destructor Procedure to be called to destroy a data value
    *                   associated with the key when the thread terminates.
    *
    * @return 0 is returned upon successful completion.
    * @return -1 is returned if an error occurs.
    */
   int keycreate(BaseThreadFunctor* destructor)
   {
      PRStatus ret = PR_NewThreadPrivateIndex(&mKeyID,
                                              (KeyDestructor) destructor);
      return (PR_SUCCESS == ret ? 0 : -1);
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
    */
   int keyfree()
   {
      // Key auto-matically freed
      setspecific(NULL);
      return 0;
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
      PRStatus ret = PR_SetThreadPrivate(mKeyID, value);
      return (PR_SUCCESS == ret ? 0 : -1);
   }

   /**
    * Stores the current value bound to \c mKeyID for the calling thread into
    * the location pointed to by \p valuep.
    *
    * @pre The specified key must have been properly created using the
    *      keycreate() member function.
    * @post The value associated with the key is obtained and stored in the
    *       pointer valuep so that the caller may work with it.
    *
    * @param valuep Address of the current data value associated with the key.
    *
    * @return 0 is returned upon successful completion.
    * @return -1 is returned if an error occurs.
    */
   int getspecific(void** valuep)
   {
      *valuep = PR_GetThreadPrivate(mKeyID);
      return 0;
   }

private:
   PRUintn mKeyID;        /**< Thread key ID */
   vpr::ThreadNonMemberFunctor* mDestructor;
};

} // End of vpr namespace


#endif  /* _VPR_THREAD_KEY_NSPR_H_ */
