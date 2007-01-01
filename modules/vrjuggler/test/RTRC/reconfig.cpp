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

// reconfig.cpp
//----------------------------------------

#include <reconfigApp.h>

      // --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/Projection.h>

#ifndef VPR_OS_Windows
#include <sched.h>
#endif

using namespace vrj;

int main(int argc, char* argv[])
{
   Projection::setNearFar(0.01f, 10000.0f);
   
   Kernel* kernel = Kernel::instance();        // Get the kernel
   reconfigApp* application = new reconfigApp(kernel);   // Declare an instance of the app

   if (argc != 2)
   {
      // display some usage info (holding the user by the hand stuff)
      //  this will probably go away once the kernel becomes separate 
      //  and can load application plugins.
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      std::cout<<"Usage: "<<argv[0]
               <<" [path to Elements]\n"
               <<std::flush;
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      exit(1);
   }

   //Get the path to the elements
   std::string path(argv[1]);

   //Tack on a slash if it isn't there already
   if (path[path.length()-1] != '/') 
      path = path + "/";

   std::cout << "Using path: " << argv[1] << std::endl << std::flush;

   application->setPath( path );

   kernel->loadConfigFile( path + "startup/sim.analogdevice1.jconf" );
   kernel->loadConfigFile( path + "startup/sim.analogdevice2.jconf" );
   kernel->loadConfigFile( path + "startup/sim.appdigitalsim.jconf" );
   kernel->loadConfigFile( path + "startup/sim.appexit.jconf" );
   kernel->loadConfigFile( path + "startup/sim.appexitproxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.button0proxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.button1proxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.button2proxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.button3proxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.button4proxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.button5proxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.camerakeyboardproxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.digitalsystemcontrols.jconf" );
   kernel->loadConfigFile( path + "startup/sim.displaysystem.jconf" );
   kernel->loadConfigFile( path + "startup/sim.headkeyboard.jconf" );
   kernel->loadConfigFile( path + "startup/sim.headproxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.mainsimkeyboardproxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.simcam1proxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.simcam2proxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.simcamera1pos.jconf" );
   kernel->loadConfigFile( path + "startup/sim.simcamera2pos.jconf" );
   kernel->loadConfigFile( path + "startup/sim.simheadpos.jconf" );
   kernel->loadConfigFile( path + "startup/sim.simviewkeyboard.jconf" );
   kernel->loadConfigFile( path + "startup/sim.stopkernel.jconf" );
   kernel->loadConfigFile( path + "startup/sim.stopkernelproxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.user1.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjaccelerateproxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton0proxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton1proxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton2proxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton3proxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton4proxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton5proxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjgrabproxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjhead.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjkeyboard.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjpointerproxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjstopproxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.vjwandproxyalias.jconf" );
   kernel->loadConfigFile( path + "startup/sim.wandbuttonsdigital.jconf" );
   kernel->loadConfigFile( path + "startup/sim.wandbuttonsdigital02.jconf" );
   kernel->loadConfigFile( path + "startup/sim.wandkeyboard.jconf" );
   kernel->loadConfigFile( path + "startup/sim.wandkeyboardproxy.jconf" );
   kernel->loadConfigFile( path + "startup/sim.wandpos.jconf" );
   kernel->loadConfigFile( path + "startup/sim.wandposproxy.jconf" );

   kernel->start();
      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   kernel->setApplication(application);         // Set application

   kernel->waitForKernelStop();

   return 1;
}
