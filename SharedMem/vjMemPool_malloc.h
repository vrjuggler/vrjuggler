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
 * NOTES:
 *    - It is *required* that one of the following be used depending on the
 *      platform:
 *         HP-UX 10.20 --> -DVJ_OS_HPUX
 *         IRIX 6.x    --> -DVJ_OS_IRIX
 * --------------------------------------------------------------------------
 */

#ifndef _VJ_MEM_POOL_MALLOC_H_
#define _VJ_MEM_POOL_MALLOC_H_

#include <vjConfig.h>
#include <sys/types.h>

#include <Kernel/vjDebug.h>


//: Shared memory pool for multi-threaded code.
//
// It is used by vjMemory (base class) to control allocation and deallocation
// from a "memory pool."  This implementation is what must be used on HP-UX
// (at least for now).
//
// Clients should create memory pools as needed.  Then when objects are
// created, they can pass a pool as a parameter to the new (if the object
// is a derived from vjMemory).

class vjMemPool_malloc : public vjMemPool {
public:
    // -----------------------------------------------------------------------
    //: Constructor for vjMemPool_malloc class.
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
    vjMemPool_malloc (size_t initialSize = 65536,  int numProcs = 8,
                      char* staticTempName = "/var/tmp/memPool_mallocXXXXXX")
    {
        vjDEBUG(vjDBG_ALL,3)
            << "vjMemPool_malloc: Allocating arena ("
            << initialSize << " bytes)\n" << vjDEBUG_FLUSH;
    }

    // -----------------------------------------------------------------------
    //: Destructor for vjMemPool_malloc class.
    //
    //! PRE: None.
    //! POST: None (for now).
    // -----------------------------------------------------------------------
    virtual
    ~vjMemPool_malloc () {
        vjDEBUG(vjDBG_ALL,6) << "vjMemPool_malloc::~vjMemPool_malloc() entered\n"
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
    allocate (size_t size) {
        void* retval;

        retval = malloc(size);

        if ( retval == NULL ) {
            std::cerr << "vjMemPool_malloc: Out of memory!!!\n";
        }

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
        free(ptr);
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
    reallocate (void* ptr, size_t new_size) {
        void* retval;

        retval = realloc(ptr, new_size);

        if ( retval == NULL ) {
            std::cerr << "vjMemPool_malloc: Out of memory!!!\n";
        }

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
    init (size_t initialSize = 32768, int numProcs = 64,
          char* staticTempName = "/var/tmp/memPoolsArenaXXXXXX")
    {
        vjDEBUG(vjDBG_ALL,3)
            << "\nvjMemPool_malloc: Allocating Base Arena for ALL "
            << "vjMemPool_malloc's.\n  "
            << initialSize << " bytes, "
            << numProcs << " procs"
            << "\n" << vjDEBUG_FLUSH;
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
    void*
    operator new (size_t size) {
        vjDEBUG(vjDBG_ALL,6) << "vjMemPool_malloc::new called.\n"
                             << vjDEBUG_FLUSH;

        init();

        return malloc(sizeof(vjMemPool_malloc));
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
        free(ptr);
    }
};

#endif /* _VJ_MEM_POOL_MALLOC_H_ */
