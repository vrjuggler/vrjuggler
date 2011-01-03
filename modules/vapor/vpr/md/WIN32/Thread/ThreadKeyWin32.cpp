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

#include <sstream>

#include <vpr/Util/ResourceException.h>
#include <vpr/Util/IllegalArgumentException.h>
#include <vpr/md/WIN32/Thread/ThreadKeyWin32.h>


namespace vpr
{

ThreadKeyWin32::ThreadKeyWin32(KeyDestructor destructor)
   : mKeyID(0xffffffff)
{
   keycreate(destructor);
}

ThreadKeyWin32::~ThreadKeyWin32()
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

void ThreadKeyWin32::keycreate(KeyDestructor destructor)
{
   if ( 0xffffffff != mKeyID )
   {
      keyfree();
   }

   mDestructor = destructor;

   const DWORD key_id = TlsAlloc();

   if ( TLS_OUT_OF_INDEXES == key_id )
   {
      std::ostringstream msg_stream;
      msg_stream << "Could not allocate thread local storage: "
                 << std::strerror(errno);
      throw vpr::ResourceException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      mKeyID = key_id;
   }
}

void ThreadKeyWin32::keyfree()
{
   if ( 0xffffffff != mKeyID )
   {
      if ( ! mDestructor.empty() )
      {
         void* value(NULL);
         getspecific(&value);
         mDestructor(value);
      }

      if ( ! TlsFree(mKeyID) )
      {
         std::ostringstream msg_stream;
         msg_stream << "Could not free thread local storage: "
                    << std::strerror(errno);
         throw vpr::IllegalArgumentException(msg_stream.str(), VPR_LOCATION);
      }

      mKeyID = 0xffffffff;
   }
}

} // End of vpr namespace
