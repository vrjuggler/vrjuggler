/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

// reconfig.cpp
//----------------------------------------

#include <reconfigApp.h>

      // --- Lib Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include <vrj/Display/Projection.h>

#ifndef WIN32
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
               <<" [path to Chunks]\n"
               <<std::flush;
      std::cout<<"\n"<<std::flush;
      std::cout<<"\n"<<std::flush;
      exit(1);
   }

   //Get the path to the chunks
   std::string path(argv[1]);

   //Tack on a slash if it isn't there already
   if (path[path.length()-1] != '/') 
      path = path + "/";

   std::cout << "Using path: " << argv[1] << std::endl << std::flush;

   application->setPath( path );

   kernel->loadConfigFile( path + "startup/sim.analogdevice1.config" );
   kernel->loadConfigFile( path + "startup/sim.analogdevice2.config" );
   kernel->loadConfigFile( path + "startup/sim.appdigitalsim.config" );
   kernel->loadConfigFile( path + "startup/sim.appexit.config" );
   kernel->loadConfigFile( path + "startup/sim.appexitproxy.config" );
   kernel->loadConfigFile( path + "startup/sim.button0proxy.config" );
   kernel->loadConfigFile( path + "startup/sim.button1proxy.config" );
   kernel->loadConfigFile( path + "startup/sim.button2proxy.config" );
   kernel->loadConfigFile( path + "startup/sim.button3proxy.config" );
   kernel->loadConfigFile( path + "startup/sim.button4proxy.config" );
   kernel->loadConfigFile( path + "startup/sim.button5proxy.config" );
   kernel->loadConfigFile( path + "startup/sim.camerakeyboardproxy.config" );
   kernel->loadConfigFile( path + "startup/sim.digitalsystemcontrols.config" );
   kernel->loadConfigFile( path + "startup/sim.displaysystem.config" );
   kernel->loadConfigFile( path + "startup/sim.headkeyboard.config" );
   kernel->loadConfigFile( path + "startup/sim.headproxy.config" );
   kernel->loadConfigFile( path + "startup/sim.mainsimkeyboardproxy.config" );
   kernel->loadConfigFile( path + "startup/sim.simcam1proxy.config" );
   kernel->loadConfigFile( path + "startup/sim.simcam2proxy.config" );
   kernel->loadConfigFile( path + "startup/sim.simcamera1pos.config" );
   kernel->loadConfigFile( path + "startup/sim.simcamera2pos.config" );
   kernel->loadConfigFile( path + "startup/sim.simheadpos.config" );
   kernel->loadConfigFile( path + "startup/sim.simviewkeyboard.config" );
   kernel->loadConfigFile( path + "startup/sim.stopkernel.config" );
   kernel->loadConfigFile( path + "startup/sim.stopkernelproxy.config" );
   kernel->loadConfigFile( path + "startup/sim.user1.config" );
   kernel->loadConfigFile( path + "startup/sim.vjaccelerateproxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton0proxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton1proxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton2proxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton3proxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton4proxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjbutton5proxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjgrabproxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjhead.config" );
   kernel->loadConfigFile( path + "startup/sim.vjkeyboard.config" );
   kernel->loadConfigFile( path + "startup/sim.vjpointerproxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjstopproxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.vjwandproxyalias.config" );
   kernel->loadConfigFile( path + "startup/sim.wandbuttonsdigital.config" );
   kernel->loadConfigFile( path + "startup/sim.wandbuttonsdigital02.config" );
   kernel->loadConfigFile( path + "startup/sim.wandkeyboard.config" );
   kernel->loadConfigFile( path + "startup/sim.wandkeyboardproxy.config" );
   kernel->loadConfigFile( path + "startup/sim.wandpos.config" );
   kernel->loadConfigFile( path + "startup/sim.wandposproxy.config" );

   kernel->start();
      //- Kernel load global config  -- Environment variable
      //- App KernelConfig function
      //- Load user local config

   kernel->setApplication(application);         // Set application

   kernel->waitForKernelStop();

   return 1;
}
