/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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

#include "SimpleApp.h"

#using <mscorlib.dll>
#using <vrjnet.dll>

using namespace System;


// This is the entry point for this application
int main(int argc, char* argv[])
{
   vrj::Kernel* kernel = vrj::Kernel::instance();

   for ( int i = 1; i < argc; ++i )
   {
      kernel->loadConfigFile(new String(argv[i]));
   }

   SimpleApp* app = new SimpleApp();
   kernel->start();
   kernel->setApplication(app);
   kernel->waitForKernelStop();

   return 0;
}
