/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 // --- VR Juggler Stuff --- //
#include <Kernel/vjKernel.h>
#include <pfSwitcherApp.h>
#include <simplePfNavApp.h>

void registerSomeApps(pfSwitcherApp* switcher);

void main(int argc, char* argv[])
{
   vjKernel* kernel = vjKernel::instance();                    // Declare a new Kernel
   pfSwitcherApp* application = pfSwitcherApp::instance();     // Delcare an instance of my application

   registerSomeApps(application);

   // Load config information
   kernel->loadChunkDescFile(std::string("pfNavChunks.dsc"));

   for ( int i = 1; i < argc; i++ )
   {
     kernel->loadConfigFile(argv[i]);
   }

   kernel->start();
   kernel->setApplication( application );    // Set up the kernel

   //while(!kernel->done())
   while (1)
   {
      usleep( 100000 );
   }
}


void registerSomeApps(pfSwitcherApp* switcher)
{
   // APPLICATION 1
   simplePfNavApp* app1 = new simplePfNavApp();

   std::string filename1("terrain.flt");
   std::string file_path1( "/home/users/allenb/Data/Models/collision" );
   const float dcs_scale1( 1.0f );
   const vjVec3 dcs_trans1( 0.0f, 0.0f, 0.0f );
   const vjVec3 dcs_rot1(0.0,0.0,0.0f);
   const vjVec3 initial_pos1( 0.0f, 0.0f, 0.0f );
   bool  drive_mode1(false);

   // Configure that application
   app1->addModel( simplePfNavApp::Model(filename1,filename1,dcs_scale1,dcs_trans1,dcs_rot1,true ));
   app1->setFilePath( file_path1 );
   app1->setInitialNavPos( initial_pos1 );
   app1->setUseDriveMode(drive_mode1);

   // APPLICATION 2
   simplePfNavApp* app2 = new simplePfNavApp();

   std::string filename2("barcelona-l.gfo");
   std::string file_path2( "/usr/share/Performer/data" );
   const float dcs_scale2( 1.0f );
   const vjVec3 dcs_trans2( 0.0f, 0.0f, 0.0f );
   const vjVec3 dcs_rot2(0.0,0.0,0.0f);
   const vjVec3 initial_pos2( 0.0f, 0.0f, 0.0f );
   bool  drive_mode2(false);

   // Configure that application
   app2->addModel( simplePfNavApp::Model(filename2,filename2,dcs_scale2,dcs_trans2,dcs_rot2,true ));
   app2->setFilePath( file_path2 );
   app2->setInitialNavPos( initial_pos2 );
   app2->setUseDriveMode(drive_mode2);

   // APPLICATION 3
   simplePfNavApp* app3 = new simplePfNavApp();

   std::string filename3("/home/vr/miller/Data/pfb_fastload/pantheon_rel.pfb");
   std::string file_path3( "" );
   const float dcs_scale3( 1.0f );
   const vjVec3 dcs_trans3( 0.0f, 0.0f, 0.0f );
   const vjVec3 dcs_rot3(0.0,0.0,0.0f);
   const vjVec3 initial_pos3( 0.0f, 0.0f, 0.0f );
   bool  drive_mode3(false);

   // Configure that application
   app3->addModel( simplePfNavApp::Model(filename3,filename3,dcs_scale3,dcs_trans3,dcs_rot3,true ));
   app3->setFilePath( file_path3 );
   app3->setInitialNavPos( initial_pos3 );
   app3->setUseDriveMode(drive_mode3);

   switcher->registerApp(pfAppHandle(app1,std::string("app1")));
   switcher->registerApp(pfAppHandle(app2,std::string("app2")));
   switcher->registerApp(pfAppHandle(app3,std::string("app3")));
}


