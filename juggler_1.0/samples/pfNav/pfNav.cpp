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
#include <math.h>
//#include <gl/device.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfEarthSky.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfTraverser.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pfdu.h>
#include <Performer/pfutil.h>

    // --- VR Juggler Stuff --- //
#include <Kernel/vjKernel.h>
#include <Kernel/Pf/vjPfApp.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjProjection.h>

#include <pfNaver.h>
#include <collidor.h>
#include <planeCollidor.h>
#include <pfCollidor.h>



// Declare my application class
class myApp : public vjPfApp
{
public:
   myApp(vjKernel* kern) : vjPfApp(kern)
   {
      //rotation = 0.0f;
      //zPos = 0.0f;
   }

   virtual void init()
   {
      vjDEBUG(vjDBG_ALL, 1) << "app::init\n" << vjDEBUG_FLUSH;
      vjProjection::setNearFar(0.5, 1000);
   }

   virtual void apiInit()
   {
      vjDEBUG(vjDBG_ALL,1) << "app::apiInit\n" << vjDEBUG_FLUSH;
   }

   virtual void preForkInit()
   {
      // Initialize type system
      vjDEBUG(vjDBG_ALL,1) << "app::preForkInit: Initializing new types.\n" << vjDEBUG_FLUSH;
      pfNaver::init();

      // Initialize loaders
   }

   /// Initialize the scene graph
   virtual void initScene()
   {
      // Setup the state like we need

      //int cur_light_state = pfGetEnable(PFEN_LIGHTING);
      //vjDEBUG(vjDBG_ALL,0) << "initScene: Current lighting state: " << cur_light_state << endl << vjDEBUG_FLUSH;

      // Load the scene
      vjDEBUG(vjDBG_ALL, 0) << "app::initScene\n" << vjDEBUG_FLUSH;
      rootNode = new pfGroup;

      naver = new pfNaver();

      sun1 = new pfLightSource;
      sun1->setPos(0.3f, 0.0f, 0.3f, 0.0f);
      sun1->setColor(PFLT_DIFFUSE,0.3f,0.0f,0.95f);
      sun1->setColor(PFLT_AMBIENT,0.4f,0.4f,0.4f);
      sun1->setColor(PFLT_SPECULAR, 1.0f, 1.0f, 1.0f);
      sun1->on();
      naver->addChild(sun1);

      // Light the root node
      ///*
      pfDCS* sun_position = new pfDCS;
      sun_position->addChild(sun1);
      rootNode->addChild(sun_position);
      //*/

      //sun1->on();     // By default

            /// Load SIMPLE geometry
      ///*
      pfFilePath("/usr/share/Performer/data");
      //pfNode* obj = new pfGroup;
      pfNode* obj = pfdLoadFile("/usr/share/Performer/data/klingon.flt");
      //rootNode->addChild(naver);

      pfDCS* world_model = new pfDCS;    // The node with the world under it
      rootNode->addChild(world_model);
      world_model->addChild(obj);
      world_model->setScale(0.25f);
      world_model->setTrans(0.0,5.0,-5.0);

      vjMatrix initial_pos;
      initial_pos.setTrans(0,0,0);
      //naver->getNavigator()->setCurPos(initial_pos);
      //naver->addChild(world_model);
      //*/

      // Load the TOWN
      /*
      pfFilePath("/usr/share/Performer/data:/usr/share/Performer/data/town");
      pfNode* obj = pfdLoadFile("/usr/share/Performer/data/town/town_ogl_pfi.pfb");
      pfDCS* world_model = new pfDCS;    // The node with the world under it
      rootNode->addChild(naver);

      world_model->addChild(obj);
      world_model->setScale(3.0f);
      vjMatrix initial_pos;
      initial_pos.setTrans(7500,50,-7500);
      naver->getNavigator()->setCurPos(initial_pos);
      naver->addChild(world_model);

      //planeCollidor* collide = new planeCollidor;
      pfVolumeCollidor* correction_collide = new pfVolumeCollidor(world_model);
      pfRideCollidor*  ride_collide = new pfRideCollidor(world_model);

      naver->getNavigator()->setGravityCollidor(ride_collide);
      naver->getNavigator()->setCorrectingCollidor(correction_collide);
      */

      pfuTravPrintNodes(rootNode, "nodes.out");
      pfdStoreFile(rootNode, "nodes.pfb");
   }

   /// Return the current scene graph
   virtual pfGroup* getScene()
   {
      vjDEBUG(vjDBG_ALL, 0) << "app::getScene\n" << vjDEBUG_FLUSH;
      return rootNode;
   }

    // Function called by the DEFAULT drawChan function before clearing the channel
   // and drawing the next frame (pfFrame())
   virtual void preDrawChan(pfChannel* chan, void* chandata)
   {
      pfDisable(PFEN_TEXTURE);
      pfOverride(PFSTATE_TEXTURE,PF_ON);     // Override texturing to turn it off;
   }


   /// Function called before pfSync
   virtual void preSync()
   {
      //vjDEBUG(vjDBG_ALL, 1) << "app::preSync\n" << vjDEBUG_FLUSH;
   }

   /// Function called after pfSync and before pfDraw
   virtual void preFrame()
   {
      //vjDEBUG(vjDBG_ALL, 1) << "app::preFrame\n" << vjDEBUG_FLUSH;
      static float x=10;
      static float y=-5;
      static float z=3;
      static float amb=0.0f;

      x+=0.05;
      y+=0.04;
      z+=0.02;
      amb += 0.025f;

      if(x>20.0f)
         x = -20.0f;
      if(y>20.0f)
         y = -20.0f;
      if(z>20.0f)
         z = -20.0f;
      if(amb>1.0f)
         amb = 0.0f;

      //sun1->setPos(x,y,z, 1.0f);
      //cerr << "set ambient: " << amb << endl;
      //sun1->setColor(PFLT_AMBIENT, amb, amb, amb);
   }

   /// Function called after pfDraw
   virtual void intraFrame()
   {
      //vjDEBUG(vjDBG_ALL,1) << "app::intraFrame\n" << vjDEBUG_FLUSH;
   }

public:
   pfLightSource* sun1;

   pfNaver*    naver;
   //pfDCS*      baseDCS;
   pfGroup*   rootNode;
};


float transSpeed = 0.1;

int main(int argc, char* argv[])
{
    vjKernel* kernel = vjKernel::instance(); // Declare a new Kernel
    myApp* application = new myApp(kernel);  // Delcare an instance of my application

    for ( int i = 1; i < argc; i++ ) {
        kernel->loadConfigFile(argv[i]);
    }

    kernel->start();

    kernel->setApplication(application);    // Set up the kernel

    //while(!kernel->done())
    while(1)
     {;}
}


