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

#include <vpr/vprConfig.h>

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
   keyfree();
}

int ThreadKeyWin32::keycreate(KeyDestructor destructor)
{
   if ( 0xffffffff != mKeyID )
   {
      keyfree();
   }

   mDestructor = destructor;

   int retval(0);
   const DWORD key_id = TlsAlloc();

   if ( TLS_OUT_OF_INDEXES == key_id )
   {
      perror("Could not allocate thread local storage");
      retval = -1;
   }
   else
   {
      mKeyID = key_id;
   }

   return retval;
}

int ThreadKeyWin32::keyfree()
{
   int retval(0);

   if ( 0xffffffff != mKeyID )
   {
      if ( ! mDestructor.empty() )
      {
         mDestructor();
      }

      if ( ! TlsFree(mKeyID) )
      {
         perror("Could not free thread local storage");
         retval = -1;
      }

      mKeyID = 0xffffffff;
   }

   return retval;
}

} // End of vpr namespace
