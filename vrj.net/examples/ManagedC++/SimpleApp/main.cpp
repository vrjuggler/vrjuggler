// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

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
