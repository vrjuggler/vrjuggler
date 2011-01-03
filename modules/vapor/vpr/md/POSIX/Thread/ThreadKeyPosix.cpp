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
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>

#include <cstring>
#include <sstream>

#include <vpr/Util/ResourceException.h>
#include <vpr/Util/IllegalArgumentException.h>
#include <vpr/md/POSIX/Thread/ThreadKeyPosix.h>


namespace vpr
{

ThreadKeyPosix::~ThreadKeyPosix()
{
   try
   {
      keyfree();
   }
   catch (...)
   {
      /* Do nothing. */ ;
   }
}

void ThreadKeyPosix::keycreate(KeyDestructor destructor)
{
   const int result = pthread_key_create(&mKeyID, destructor);

   if ( result != 0 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to create thread-specific key: "
                 << std::strerror(result);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
}

void ThreadKeyPosix::keyfree()
{
   const int result = pthread_key_delete(mKeyID);

   if ( EINVAL == result )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to delete invalid thread-specific key: "
                 << std::strerror(result);
      throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
   }
}

}
