/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <vpr/md/POSIX/SystemPosix.h>


namespace vpr
{

int SystemPosix::usleep(vpr::Uint32 micro)
{
#ifdef VPR_OS_Linux
   ::usleep(micro);
   return 0;  // usleep can't report failure, so assume success.
#else
   return ::usleep(micro);
#endif
}

int SystemPosix::msleep(vpr::Uint32 milli)
{
   // usleep() cannot sleep for more than 1 second, so we have to work
   // around that here.  First, we sleep for N seconds.
   if ( milli >= 1000 )
   {
      SystemPosix::sleep(milli / 1000);
   }

   // Then we finish off by sleeping for (N mod 1000) milliseconds.
   return SystemPosix::usleep((milli % 1000) * 1000);
}

} // End of vpr namespace
