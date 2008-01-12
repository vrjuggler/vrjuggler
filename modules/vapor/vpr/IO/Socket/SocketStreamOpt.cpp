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

#include <vpr/IO/Socket/SocketStreamOpt.h>


namespace vpr
{

SocketStreamOpt::SocketStreamOpt()
   : mCorked(false)
{
   /* Do nothing. */ ;
}

SocketStreamOpt::~SocketStreamOpt()
{
   /* Do nothing. */ ;
}

bool SocketStreamOpt::getNoPush() const
{
#if defined(HAVE_CORKABLE_TCP)
   vpr::SocketOptions::Data option;

   getOption(vpr::SocketOptions::NoPush, option);
   return option.no_push;
#else
   return mCorked;
#endif
}

void SocketStreamOpt::setNoPush(const bool enableVal)
{
// NSPR sockets are not corkable, but the OS may still support TCP corking.
#if defined(HAVE_CORKABLE_TCP) && VPR_IO_DOMAIN_INCLUDE != VPR_DOMAIN_NSPR
   vpr::SocketOptions::Data option;
   option.no_push = enableVal;
   setOption(vpr::SocketOptions::NoPush, option);
#else
   if ( enableVal != mCorked )
   {
      // Changing from the uncorked state to the corked state.
      if ( enableVal )
      {
         cork();
      }
      // Changing from the corked state to the uncorked state.
      else
      {
         uncork();
      }
   }
#endif

   mCorked = enableVal;
}

}
