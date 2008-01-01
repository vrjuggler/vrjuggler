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

//----------------------------------------
// Test OpenGL VR Juggler program
//
// cubes.cpp
//----------------------------------------
#include <cubesApp.h>

      // --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/Projection.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

#ifdef BUILD_AS_VRJ_PLUGIN
VRJ_APP_PLUGIN vrj::App* createAppObj(vrj::Kernel* kernel)
{
   return new cubesApp(kernel);
}
#else
int main(int argc, char* argv[])
{
   vrj::Projection::setNearFar(0.01f, 10000.0f);

   vrj::Kernel* kernel = vrj::Kernel::instance();  // Get the kernel
   cubesApp* application = new cubesApp(kernel);   // Declare an instance of the app

   // Create options for the cubes application.
   po::options_description cubes_desc("Cubes Options");
   po::options_description& general_desc = kernel->getGeneralOptions();
   po::options_description& cluster_desc = kernel->getClusterOptions();
   cubes_desc.add(general_desc).add(cluster_desc);

   //po::options_description& config_desc = kernel->getConfigOptions();
   //cubes_desc.add(config_desc);

   cubes_desc.add_options()
       ("jconf", po::value< std::vector<std::string> >()->composing(), "VR Juggler Configuration File")
   ;

   // jconf files can be given as postion arguments.
   po::positional_options_description pos_desc;
   pos_desc.add("jconf", -1);

   // Construct a parser and do the actual parsing.
   po::command_line_parser parser(argc, argv);
   po::parsed_options parsed = parser.options(cubes_desc).positional(pos_desc).allow_unregistered().run();

   // Finally store our options and use them.
   po::variables_map vm;
   po::store(parsed, vm);
   po::notify(vm);

   if(vm.count("help"))
   {
      std::cout << cubes_desc << std::endl;
      return 0;
   }

#if ! defined(VRJ_USE_COCOA)
   if ( 0 == vm.count("jconf") && 0 == vm.count("vrjslave"))
   {
      // display some usage info (holding the user by the hand stuff)
      //  this will probably go away once the kernel becomes separate 
      //  and can load application plugins.
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      std::cout << "No VR Juggler configuration files given!" << std::endl;
      std::cout << cubes_desc << std::endl;
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;

#ifdef VPR_OS_Windows
      // Create a stringstream to format the output.
      std::stringstream error_output;
      error_output << "No VR Juggler configuration files given!" << std::endl;
      error_output << cubes_desc << std::endl;
      // Display a message box to inform the user of the error.
      MessageBox( NULL, error_output.str().c_str(), "VR Juggler Usage Error",
              MB_OK | MB_ICONERROR | MB_TASKMODAL );
#endif
      //exit(1);
      return -1;
   }
#endif

   // Initialize the kernel before loading config files.
   kernel->init(vm);

   // If we have configuration files, load them.
   if ( vm.count("jconf") > 0 )
   {
      std::vector<std::string> jconfs =
         vm["jconf"].as< std::vector<std::string> >();

      std::vector<std::string>::iterator i;
      for ( i = jconfs.begin(); i != jconfs.end(); ++i )
      {
         std::cout << "Reading: " << *i << std::endl;
         kernel->loadConfigFile(*i);
      }
   }


   // Load any config files specified on the command line
   //for( int i = 1; i < argc; ++i )
   //{
   //   kernel->loadConfigFile(argv[i]);
   //}

   kernel->start();
   kernel->setApplication(application);         // Set application

   kernel->waitForKernelStop();

   delete application;

   return 0;
}
#endif
