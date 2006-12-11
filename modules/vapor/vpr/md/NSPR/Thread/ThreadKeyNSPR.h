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
 * @note This file MUST be included by vprThread.h, not the other way around.
 */

#ifndef _VPR_THREAD_KEY_POSIX_H_
#define _VPR_THREAD_KEY_POSIX_H_

#include <vpr/vprConfig.h>
#include <prthread.h>

#include <vpr/Util/IllegalArgumentException.h>


namespace vpr
{

// Key destructor function type.
typedef PRThreadPrivateDTOR KeyDestructor;

/** \class ThreadKeyNSPR ThreadKeyNSPR.h vpr/Thread/Thread.h
 *
 * Wrapper around NSPR thread-specific data.  It is typedef'd to
 * vpr::ThreadKey.
 */
class VPR_CLASS_API ThreadKeyNSPR
{
public:
   /**
    * Default constructor.  After calling this, one of the keycreate()
    * overloads must be called to finish the key allocation process.
    *
    * @see keycreate
    *
    * @throw vpr::ResourceException is thrown if the thread-specific key
    *        could not be created.
    */
   ThreadKeyNSPR(KeyDestructor destructor = NULL)
   {
      keycreate(destructor);
   }

   /**
    * Releases this key.
    */
   ~ThreadKeyNSPR();

   /**
    * Allocates a key that is used to identify data that is specific to each
    * thread in the process, is global to all threads in the process, and is
    * is destroyed using the spcefied destructor function.
    *
    * @post A key is created and is associated with the specified destructor
    *       function and argument.
    *
    * @param destructor The destructor function for the key.
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
   void keyfree()
   {
      // Key automatically freed.
      setspecific(NULL);
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
    * @throw vpr::IllegalArgumentException is thrown if this key is invalid
    *        and therefore cannot have a value associated with it.
    */
   void setspecific(void* value)
   {
      const PRStatus ret = PR_SetThreadPrivate(mKeyID, value);

      if ( PR_FAILURE == ret )
      {
         throw vpr::IllegalArgumentException(
            "Tried to set value on an invalid thread-specific key.",
            VPR_LOCATION
         );
      }
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
    */
   void getspecific(void** valuep)
   {
      *valuep = PR_GetThreadPrivate(mKeyID);
   }

private:
   PRUintn mKeyID;        /**< Thread key ID */
};

} // End of vpr namespace


#endif  /* _VPR_THREAD_KEY_NSPR_H_ */
