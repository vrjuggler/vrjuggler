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

/*
 * --------------------------------------------------------------------------
 * Author:
 *   Patrick Hartling
 * --------------------------------------------------------------------------
 */

#ifndef _VJ_MEM_POOL_NSPR_H_
#define _VJ_MEM_POOL_NSPR_H_

#include <vjConfig.h>
#include <prmem.h>

#include <Utils/vjDebug.h>


//: Shared memory pool for multi-threaded code.
//
// It is used by vjMemory (base class) to control allocation and deallocation
// from a "memory pool."  This implementation is what must be used on HP-UX
// (at least for now).
//
// Clients should create memory pools as needed.  Then when objects are
// created, they can pass a pool as a parameter to the new (if the object
// is a derived from vjMemory).

class vjMemPoolNSPR : public vjMemPool {
public:
    // -----------------------------------------------------------------------
    //: Constructor for vjMemPoolNSPR class.
    //
    //! PRE: None.
    //! POST: None (for now).
    //
    //! ARGS: initialSize - The initial size of the arena.  (Not currently
    //+                     used.)
    //! ARGS: numProcs - The number of threads that will share this arena.
    //+                  (Not currently used.)
    //! ARGS: staticTempName - The name of the arena file.  (Not currently
    //+                        used.)
    // -----------------------------------------------------------------------
    vjMemPoolNSPR (size_t initialSize = 65536,  int numProcs = 8,
                   char* staticTempName = "/var/tmp/memPoolNSPRXXXXXX")
    {
        vjDEBUG(vjDBG_ALL,3) << "\n\nvjMemPoolNSPR: Allocating Arena.\n"
                             << "Initial size: " << initialSize << std::endl
                             << vjDEBUG_FLUSH;
    }

    // -----------------------------------------------------------------------
    //: Destructor for vjMemPoolNSPR class.
    //
    //! PRE: None.
    //! POST: None (for now).
    // -----------------------------------------------------------------------
    virtual
    ~vjMemPoolNSPR (void) {
        vjDEBUG(vjDBG_ALL,6) << "vjMemPoolNSPR::~vjMemPoolNSPR() entered\n"
                             << vjDEBUG_FLUSH;
    }

    // -----------------------------------------------------------------------
    //: Allocate a chunk of memory of the specified size.
    //
    //! PRE: None.
    //! POST: A piece of memory is returned to the caller that, in a
    //+       multi-threaded environment, can be shared among threads.
    //
    //! ARGS: size - The size of the memory block to allocate.
    //
    //! RETURNS: pointer - A pointer to the newly allocated memory.
    // -----------------------------------------------------------------------
    virtual void*
    allocate (PRUint32 size) {
        void* retval;

        retval = PR_Malloc(size);

        vjASSERT(retval != NULL);
//        if ( retval == NULL ) {
//            cerr << "vjMemPoolNSPR: Out of memory!!!" << std::endl;
//        }

        return retval;
    }

    // -----------------------------------------------------------------------
    //: Deallocate a chunk of user-specified memory.
    //
    //! PRE: None.
    //! POST: The memory pointed to by ptr is freed.
    //
    //! ARGS: ptr - The chunk of memory to be deallocated.
    // -----------------------------------------------------------------------
    inline virtual void
    deallocate (void* ptr) {
        PR_Free(ptr);
    }

    // -----------------------------------------------------------------------
    //: Reallocate a specified chunk of memory using the given size.
    //
    //! PRE: None.
    //! POST: The memory pointed to by ptr is reallocated at the given
    //+       size and returned to the caller.
    //
    //! ARGS: ptr - The chunk of memory to be reallocated.
    //! ARGS: new_size - The new size to be reallocated.
    //
    //! RETURNS: pointer - A pointer to the newly reallocated memory.
    // -----------------------------------------------------------------------
    virtual void*
    reallocate (void* ptr, PRUint32 new_size) {
        void* retval;

        retval = PR_Realloc(ptr, new_size);

        vjASSERT(retval != NULL);
//        if ( retval == NULL ) {
//            cerr << "vjMemPoolNSPR: Out of memory!!!\n";
//        }

        return ptr;
    }

    // Functions that initialize, allocate and deallocate memory from the
    // class-wide memory "arena."

    // -----------------------------------------------------------------------
    //: Initialize the class-wide arena.
    //
    //! PRE: None.
    //! POST: None (for now).
    //
    //! ARGS: initialSize - The initial size of the arena.  (Not currently
    //+                     used.)
    //! ARGS: numProcs - The number of threads that will share this arena.
    //+                  (Not currently used.)
    //! ARGS: staticTempName - The name of the arena file.  (Not currently
    //+                        used.)
    // -----------------------------------------------------------------------
    static void
    init (PRUint32 initialSize = 32768) {
        vjDEBUG(vjDBG_ALL,3) << "\n\nvjMemPoolNSPR: Allocating Base Arena for ALL "
                             << "vjMemPoolNSPR's.\n\tSize: " << initialSize
                             << std::endl << vjDEBUG_FLUSH;
    }

    // -----------------------------------------------------------------------
    //: Allocate a new piece of memory of the given size.
    //
    //! PRE: None.
    //! POST: A piece of memory is returned to the caller that, in a
    //+       multi-threaded environment, can be shared among threads.
    //
    //! ARGS: size - The size of the memory block to allocate.
    // -----------------------------------------------------------------------
#ifndef VJ_OS_Darwin
    void* operator new (PRUint32 size) {
#else
// Mac OS X compilers were complaining about the PRUint32.  It demands size_t!
    void* operator new (size_t size) {
#endif

        vjDEBUG(vjDBG_ALL,6) << "vjMemPoolNSPR::new called.\n" << vjDEBUG_FLUSH;

        init();

        return PR_Malloc(sizeof(vjMemPoolNSPR));
    }

    // -----------------------------------------------------------------------
    //: Delete a chunk of user-specified memory.
    //
    //! PRE: None.
    //! POST: The memory pointed to by ptr is freed.
    //
    //! ARGS: ptr - A pointer to previously allocated memory.
    // -----------------------------------------------------------------------
    void
    operator delete (void* ptr) {
        PR_Free(ptr);
    }
};


#endif /* _VJ_MEM_POOL_NSPR_H_ */
