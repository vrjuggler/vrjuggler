/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


//:

#include <vjConfig.h>
#include <Threads/vjThreadFunctor.h>
#include <Threads/vjThreadManager.h>


//--------------------------------------------
// This is the actual function that is called.
//--------------------------------------------
#if defined(VJ_IRIX_SPROC)	// ---- Using IRIX Sproc ------ //
    void
    ThreadFunctorFunction (void* args) {
        vjBaseThreadFunctor& func = *(vjBaseThreadFunctor*)args;
        func();	// Call the functor's operator ()
    }
#elif defined(VJ_OS_Win32)	// ----- Using Win32 ---- //
    unsigned int __stdcall
    ThreadFunctorFunction (void* args) {
         vjBaseThreadFunctor& func = *(vjBaseThreadFunctor*)args;
         func();
         return 0;
    }
#elif defined(VJ_USE_PTHREADS)
#   ifdef _PTHREADS_DRAFT_4
        void
#   else
        void*
#   endif
        ThreadFunctorFunction (void* args) {
            vjThreadManager* vj_tm_inst;
            vjBaseThreadFunctor& func = *((vjBaseThreadFunctor*) args);

            // Wait until this thread has been registered with the thread
            // manager before continuing execution.
            vj_tm_inst = vjThreadManager::instance();
            vj_tm_inst->lock();
            vj_tm_inst->unlock();

            func();

#   ifdef _PTHREADS_DRAFT_10
            return (void*) NULL;
#   endif
        }

#else
    void
    ThreadFunctorFunction (void* args) {
        vjBaseThreadFunctor& func = *(vjBaseThreadFunctor*)args;
        func();
    }
#endif	/* VJ_IRIX_SPROC */
