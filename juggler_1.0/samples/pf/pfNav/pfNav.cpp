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
#include <simplePfNavApp.h>


int main(int argc, char* argv[])
{
   std::string model_filename;

   // Config params
   std::string file_path("");
   const float dcs_scale(1.0f);
   const vjVec3 dcs_trans(0.0f, 0.0f, 0.0f);
   const vjVec3 initial_pos(0.0f, 0.0f, 0.0f);

   vjKernel* kernel = vjKernel::instance(); // Declare a new Kernel
    simplePfNavApp* app = new simplePfNavApp(kernel);  // Delcare an instance of my application

    cout << "Usage: " << argv[0] << " modelfile vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n" << flush;
    cout << "\n" << flush;

    if (argc < 2)
    {
       cout<<"\n\n[ERROR!!!] you must supply a model database (then config files)\n\n"<<flush;
       return 0;
    }

    model_filename = std::string(argv[1]);

    if (argc <= 2)
    {
       cout<<"\n\n[ERROR!!!] you must supply config files after the model file...\n\n"<<flush;
       return 0;
    }

    for ( int i = 2; i < argc; i++ ) {
        kernel->loadConfigFile(argv[i]);
    }

    kernel->start();

    // Configure that application
    app->setFileName(model_filename);
    app->setFilePath(file_path);
    app->setWorldDcsScale(dcs_scale);
    app->setWorldDcsTrans(dcs_trans);
    app->setInitialNavPos(initial_pos);

    kernel->setApplication(app);    // Set up the kernel

    //while(!kernel->done())
    while(1)
     {;}
}


