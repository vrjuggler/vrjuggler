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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <vpr/vprConfig.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <limits.h>

#include <vpr/Util/ResourceException.h>
#include <vpr/Util/Assert.h>
#include <vpr/md/POSIX/Sync/SemaphorePosix.h>


namespace vpr
{

SemaphorePosix::SemaphorePosix(const int initialValue)
{
#if ! defined(VPR_HAVE_UNNAMED_POSIX_SEMAPHORE)
   // Use strdup(3) here so that mktemp(3) can modify the memory.  Trying
   // to modify a constant string would be bad.
   // NOTE: The memory allocated by strdup(3) will be released in the
   // destructor.
   mSemaFile = mktemp(strdup("/tmp/vprsema.XXXXXX"));

   // Reset the errno global variable because mktemp(3) should not have failed.
   // This seems to be critical for getting proper behavior out of sem_open(3)
   // on Darwin 9.x (Mac OS X 10.5).
   errno = 0;

   // ----- Allocate the named semaphore ----- //
   // This sets the semaphore file permissions to 0600.
   mSema = sem_open(mSemaFile, O_CREAT, S_IRUSR | S_IWUSR, initialValue);

   if ( mSema == reinterpret_cast<sem_t*>(SEM_FAILED) )
   {
      std::ostringstream msg_stream;
      msg_stream << "Named semaphore allocation failed: "
                 << std::strerror(errno);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
#else
   // ----- Allocate the unnamed semaphore ----- //
   mSema = reinterpret_cast<sem_t*>(std::malloc(sizeof(sem_t)));

   if ( NULL == mSema )
   {
      std::ostringstream msg_stream;
      msg_stream << "Semaphore object allocation failed: "
                 << std::strerror(errno);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }

   if ( sem_init(mSema, 0, initialValue) != 0 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Unnamed semaphore initialization failed: "
                 << std::strerror(errno);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
#endif
}

SemaphorePosix::~SemaphorePosix()
{
   // ---- Delete the semaphore --- //
#if ! defined(VPR_HAVE_UNNAMED_POSIX_SEMAPHORE)
   const int result = sem_close(mSema);
   vprASSERT(result == 0);

   sem_unlink(mSemaFile);
   std::free(mSemaFile);
#else
   const int result = sem_destroy(mSema);
   vprASSERT(result == 0);

   std::free(mSema);
#endif
}

void SemaphorePosix::reset(const int val)
{
#if ! defined(VPR_HAVE_UNNAMED_POSIX_SEMAPHORE)
   // First deallocate the current semaphore.
   const int result = sem_close(mSema);

   if ( 0 != result )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to close named semaphore: "
                 << std::strerror(errno);
      throw vpr::Exception(msg_stream.str(), VPR_LOCATION);
   }

   // Now recreate it with the new value in val.
   mSema = sem_open(mSemaFile, O_CREAT, 0600, val);

   if ( mSema == reinterpret_cast<sem_t*>(SEM_FAILED) )
   {
      std::ostringstream msg_stream;
      msg_stream << "Named semaphore re-allocation failed: "
                 << std::strerror(errno);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
#else
   // First destroy the current semaphore.
   const int result = sem_destroy(mSema);
   vprASSERT(result == 0);

   // Now recreate it with the new value in val.
   if ( sem_init(mSema, 0, val) != 0 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Unnamed semaphore re-initialization failed: "
                 << std::strerror(errno);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
#endif
}

void SemaphorePosix::dump(FILE* dest, const char* message) const
{
   int value;
   sem_getvalue(mSema, &value);
   std::fprintf(dest, "%s", message);
   std::fprintf(dest, "Current semaphore value: %d\n", value);
}

}
