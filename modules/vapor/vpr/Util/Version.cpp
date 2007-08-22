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
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <vpr/vprParam.h>
#include <vpr/Util/Version.h>


namespace vpr
{

const char* VPR_VERSION(__VPR_VERSION_STRING__);

VPR_IMPLEMENT(std::string) getVersionString ()
{
   return std::string(VPR_VERSION);
}

VPR_IMPLEMENT(vpr::Uint32) getVersionNumber ()
{
   return __VPR_version;
}

VPR_IMPLEMENT(std::string) getSubsystem ()
{
   std::string subsystem_string;

#if defined(VPR_USE_NSPR)
   subsystem_string = std::string("NSPR");
#elif defined(VPR_USE_PTHREADS)
   subsystem_string = std::string("POSIX");
#endif

   return subsystem_string;
}

}
