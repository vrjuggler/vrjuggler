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

#include <vprConfig.h>
#include <Threads/ThreadFunctor.h>
#include <Threads/ThreadManager.h>


namespace vpr {

//--------------------------------------------
// This is the actual function that is called.
//--------------------------------------------
#if defined(VPR_USE_IRIX_SPROC)	/* ---- Using IRIX Sproc ------ */
    void
    ThreadFunctorFunction (void* args) {
        BaseThreadFunctor& func = *(BaseThreadFunctor*)args;
        func();	// Call the functor's operator ()
    }
#elif defined(VPR_USE_WIN32)	/* ----- Using Win32 ---- */
    unsigned int __stdcall
    ThreadFunctorFunction (void* args) {
         BaseThreadFunctor& func = *(BaseThreadFunctor*)args;
         func();
         return 0;
    }
#elif defined(VPR_USE_PTHREADS)
#   ifdef _PTHREADS_DRAFT_4
        void
#   else
        void*
#   endif
        ThreadFunctorFunction (void* args) {
            ThreadManager* vpr_tm_inst;
            BaseThreadFunctor& func = *((BaseThreadFunctor*) args);

            // Wait until this thread has been registered with the thread
            // manager before continuing execution.
            vpr_tm_inst = ThreadManager::instance();
            vpr_tm_inst->lock();
            vpr_tm_inst->unlock();

            func();

#   ifdef _PTHREADS_DRAFT_10
            return (void*) NULL;
#   endif
        }

#elif defined(VPR_USE_NSPR)
    void
    ThreadFunctorFunction (void* args) {
        BaseThreadFunctor& func = *(BaseThreadFunctor*)args;
        func();
    }
#else
    void
    ThreadFunctorFunction (void* args) {
        BaseThreadFunctor& func = *(BaseThreadFunctor*)args;
        func();
    }
#endif	/* VPR_USE_IRIX_SPROC */

}; // End of vpr namespace
