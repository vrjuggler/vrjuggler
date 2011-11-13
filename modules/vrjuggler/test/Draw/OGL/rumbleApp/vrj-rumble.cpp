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

#include "vrj-rumble.h"
#include "RumbleApp.h"

#include <vrj/Kernel/Kernel.h>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

using namespace vrj;

int main(int argc, char **argv)
{

    // Application requires the setting of GADGET_BASE_DIR to $(VJ_BASE_DIR)

    Kernel *kernel = Kernel::instance();
    juggler::RumbleApp *app = new juggler::RumbleApp(kernel);
    
    // Create options for the Rumble application.
    po::options_description rumble_desc("Rumble Options");
    po::options_description& general_desc = kernel->getGeneralOptions();
    po::options_description& cluster_desc = kernel->getClusterOptions();
    rumble_desc.add(general_desc).add(cluster_desc);

    rumble_desc.add_options()
       ("jconf", po::value< std::vector<std::string> >()->composing(), "VR Juggler Configuration File")
    ;

    // jconf files can be given as postion arguments.
    po::positional_options_description pos_desc;
    pos_desc.add("jconf", -1);

    // Construct a parser and do the actual parsing.
    po::command_line_parser parser(argc, argv);
    po::parsed_options parsed = parser.options(rumble_desc).positional(pos_desc).allow_unregistered().run();

    // Finally store our options and use them.
    po::variables_map vm;
    po::store(parsed, vm);
    po::notify(vm);

    if(vm.count("help"))
    {
        std::cout << rumble_desc << std::endl;
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
       std::cout << rumble_desc << std::endl;
       std::cout<<"\n"<<std::flush;
       std::cout<<"\n"<<std::flush;

#ifdef VPR_OS_Windows
       // Create a stringstream to format the output.
       std::stringstream error_output;
       error_output << "No VR Juggler configuration files given!" << std::endl;
       error_output << rumble_desc << std::endl;
       // Display a message box to inform the user of the error.
       MessageBox( NULL, error_output.str().c_str(), "VR Juggler Usage Error",
                   MB_OK | MB_ICONERROR | MB_TASKMODAL );
#endif
       //exit(1);
       return -1;
    }
#endif

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
    //kernel->init(argc, argv);
//
//    kernel->loadConfigFile("/ftiapp/appl/vrjuggler-3.0.0.0/share/vrjuggler-3.0.0/data/configFiles/standalone.jconf");
//    kernel->loadConfigFile("I:/jeff/vrjuggler-trunk/2pc-1screen-bender-virtualxp64-doug.jconf");
//    kernel->loadConfigFile("I:/jeff/vrjuggler-trunk/SDL-test.jconf");
    kernel->start();
    kernel->setApplication(app);
    kernel->waitForKernelStop();
    
	delete app;

    return 0;
}
