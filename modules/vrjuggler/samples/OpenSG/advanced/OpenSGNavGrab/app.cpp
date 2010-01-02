/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#include <cstdlib>
#include <string>
#include <vector>

#include <vrj/Kernel/Kernel.h>
#include <OpenSGNavGrab.h>


int main(int argc, char* argv[])
{
   vrj::Kernel* kernel = vrj::Kernel::instance();
   OpenSGNavGrab* application = new OpenSGNavGrab(kernel);

   std::vector<std::string> model_files;

   kernel->init(argc, argv);

   // Load any .jconf files specified on the command line.
   const std::string jconf_ext(".jconf");
   for ( int i = 1; i < argc; ++i )
   {
      const std::string cur_arg(argv[i]);
      if ( cur_arg.rfind(jconf_ext) != std::string::npos )
      {
         kernel->loadConfigFile(cur_arg);
      }
      else
      {
         model_files.push_back(cur_arg);
      }
   }

   // Load the models named on the command line.
   if ( ! model_files.empty() )
   {
      application->setModelsToLoad(model_files);
   }
 
   kernel->start();
   kernel->setApplication(application);
   kernel->waitForKernelStop();

   delete application;

   return EXIT_SUCCESS;
}
