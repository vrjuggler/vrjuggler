/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
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

#include <stdlib.h>

#ifdef WIN32
#  include <string.h>
#else
#  include <unistd.h>
#endif

#include <string>
#include <boost/python.hpp>

#include <vrj/Kernel/Kernel.h>
#include <PyExtApp.h>


static void usage(const char* progName)
{
   std::cerr << "Usage:\n\t" << progName << " [-m <Python module>] "
             << "[-c <VRJ config file> ...]" << std::endl;
}

int main(int argc, char* argv[])
{
   // Initialize the interpreter.
   Py_Initialize();

   // This will contain the name of the Python module to load for use by
   // the PyExtApp object.  It defaults to app.ExampleModule, but the value
   // can be overridden on the command line with the -s argument.
   std::string module_name("app.ExampleModule");

#ifdef WIN32
   int optind, i;

   for ( i = 1; i < argc; ++i )
   {
      if ( strcmp(argv[i], "-c") == 0 )
      {
         vrj::Kernel::instance()->loadConfigFile(argv[i + i]);
         ++i;
      }
      else if ( strcmp(argv[i], "-m") == 0 )
      {
         module_name = std::string(argv[i + 1]);
         ++i;
      }
      else
      {
         usage(argv[0]);
         exit(1);
      }
   }

   optind = i;
#else
   extern char* optarg;
   extern int optind;
   int ch;

   while ( (ch = getopt(argc, argv, "c:m:")) != -1 )
   {
      switch (ch)
      {
         // Match -c <VRJ config file>
         case 'c':
            vrj::Kernel::instance()->loadConfigFile(optarg);
            break;
         // Match -s <Python module>
         case 'm':
            module_name = std::string(optarg);
            break;
         // Match everything else.
         default:
            usage(argv[0]);
            exit(1);
            break;
      }
   }
#endif

   argc -= optind;
   argv += optind;

   // Fire up the VR Juggler kernel.
   vrj::Kernel::instance()->start();

   // Create our application object and hand it off to the kernel.
   PyExtApp app(module_name);
   vrj::Kernel::instance()->setApplication(&app);

   // Wait for the kernel to stop executing.
   vrj::Kernel::instance()->waitForKernelStop();

   // Release the Python interpreter.
   Py_Finalize();

   return 0;
}
