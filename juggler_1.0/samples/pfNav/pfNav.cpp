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
#include <Kernel/Pf/vjPfApp.h>    // the performer application base type
#include <Kernel/vjDebug.h>
#include <Kernel/vjProjection.h>  // for setNearFar (for setting clipping planes)
#include <Input/InputManager/vjPosInterface.h>
#include <Input/InputManager/vjDigitalInterface.h>


// nav includes
#include <CaveNavigator.h>
#include <collider.h>
#include <planeCollider.h>
#include <pfPogoCollider.h>
#include <pfRayCollider.h>
#include <pfBoxCollider.h>

char* filename = NULL;


// Declare my application class
class pfNavJugglerApplication : public vjPfApp
{
public:
   pfNavJugglerApplication( vjKernel* kern ) : mFpsEmitCount(0), vjPfApp( kern )
   {
      // get a valid time on the stopwatch...
      stopWatch.stop();
      stopWatch.start();
   }

   virtual void init()
   {
      vjDEBUG(vjDBG_ALL, 1) << "app::init\n" << vjDEBUG_FLUSH;
      vjProjection::setNearFar(0.5, 1000);
      
      mWand.init( "VJWand" );
      mHead.init( "VJHead" );
      mActionButton.init( "VJButton0" );
      mActionButton2.init( "VJButton1" );
      mModeChangeButton.init( "VJButton2" );
   }

   virtual void apiInit()
   {
      vjDEBUG(vjDBG_ALL,1) << "app::apiInit\n" << vjDEBUG_FLUSH;
   }

   virtual void preForkInit()
   {
      // Initialize type system
      vjDEBUG(vjDBG_ALL,1) << "app::preForkInit: Initializing new types.\n" << vjDEBUG_FLUSH;

      // Initialize loaders
      pfdInitConverter("terrain.flt");
   }

   inline void textures( bool state ) const
   {
      if (state == true)
      {
         pfEnable( PFEN_TEXTURE );
         pfOverride(PFSTATE_ENTEXTURE, PF_ON);
      }
      
      else
      {
         pfDisable( PFEN_TEXTURE );
         pfOverride(PFSTATE_ENTEXTURE, PF_OFF);
      }      
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

      mNavigationDCS = new pfDCS();

      sun1 = new pfLightSource;
      sun1->setPos(0.3f, 0.0f, 0.3f, 0.0f);
      sun1->setColor(PFLT_DIFFUSE,0.3f,0.0f,0.95f);
      sun1->setColor(PFLT_AMBIENT,0.4f,0.4f,0.4f);
      sun1->setColor(PFLT_SPECULAR, 1.0f, 1.0f, 1.0f);
      sun1->on();
      mNavigationDCS->addChild(sun1);

      // Light the root node
      ///*
      pfDCS* sun_position = new pfDCS;
      sun_position->addChild(sun1);
      rootNode->addChild(sun_position);
      //*/

      //sun1->on();     // By default

            /// Load SIMPLE geometry
      ///*
      pfFilePath(".:/usr/share/Performer/data");
      //pfNode* obj = new pfGroup;
      //pfNode* obj = pfdLoadFile("/usr/share/Performer/data/klingon.flt");
      pfNode* obj = pfdLoadFile( filename );
      rootNode->addChild( mNavigationDCS );

      pfDCS* world_model = new pfDCS;    // The node with the world under it
      world_model->addChild(obj);
      world_model->setScale(0.25f);
      world_model->setTrans(0.0,5.0,-5.0);
      mNavigationDCS->addChild(world_model);
      
      // Load the TOWN
      //pfFilePath("/usr/share/Performer/data:/usr/share/Performer/data/town");
      //pfNode* obj = pfdLoadFile("/usr/share/Performer/data/town/town_ogl_pfi.pfb");
      //pfDCS* world_model = new pfDCS;    // The node with the world under it
      //rootNode->addChild(mNavigationDCS);
      //world_model->addChild(obj);
      //world_model->setScale(3.0f);
      
      // Configure the Navigator DCS node:
      // Set it's initial position:
      vjMatrix initial_pos;
      //initial_pos.setTrans(7500,50,-7500);
      initial_pos.setTrans(0, 6, 0);
      mNavigator.getNavigator()->setCurPos(initial_pos);

      // Set its terrain follower
      //planeCollider* collide = new planeCollider;
      pfPogoCollider*  ride_collide = new pfPogoCollider(world_model);
      mNavigator.getNavigator()->addCollider(ride_collide);

      // Set the navigator's collider.
      pfBoxCollider* correction_collide = new pfBoxCollider( world_model );
      mNavigator.getNavigator()->addCollider( correction_collide );
      

      // load these files into perfly to see just what your scenegraph 
      // looked like. . . . .useful for debugging.
      pfuTravPrintNodes( rootNode, "nodes.out" );
      pfdStoreFile( rootNode, "nodes.pfb" );
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
      this->textures( true );     // Override texturing to turn it on;
   }


   /// Function called before pfSync
   virtual void preSync()
   {
      //vjDEBUG(vjDBG_ALL, 1) << "app::preSync\n" << vjDEBUG_FLUSH;
   }

   /// Function called after pfSync and before pfDraw
   virtual void preFrame()
   {
      // Keep time, for FPS measurments...
      stopWatch.stop();
      stopWatch.start();
      
      /////////////////////////////////////////////////////////
      //: Handle navigation

      if (mActionButton2->getData() == vjDigital::TOGGLE_ON)
         cout<<"Brake\n"<<flush;
      if (mActionButton->getData() == vjDigital::TOGGLE_ON)
         cout<<"Accelerate\n"<<flush;

      // let the navigator collect some instructions from input devices...
      mNavigator.accelerate( mActionButton->getData() == vjDigital::ON ||
                             mActionButton->getData() == vjDigital::TOGGLE_ON );
      mNavigator.brake( mActionButton2->getData() == vjDigital::ON ||
                       mActionButton2->getData() == vjDigital::TOGGLE_ON );
      mNavigator.rotate( mActionButton2->getData() != vjDigital::ON && 
                         mActionButton2->getData() != vjDigital::TOGGLE_ON );
      vjMatrix* wandMatrix = mWand->getData();
      vjMatrix rotMatrix = *wandMatrix;
      rotMatrix( 0, 3 ) = 0.0f;
      rotMatrix( 1, 3 ) = 0.0f;
      rotMatrix( 2, 3 ) = 0.0f;
      mNavigator.setMatrix( rotMatrix );
      
      // tell the navigator to update itself with any new instructions just given to it.
      mNavigator.update();
      
      // notify the navigator DCS of the mNavigator's new matrix
      pfMatrix mNavigator_pf = vjGetPfMatrix( mNavigator );
      mNavigationDCS->setMat( mNavigator_pf );
      
      // output the FPS so the team artist can get metrics on their model
      ++mFpsEmitCount;
      if (mFpsEmitCount >= 15)
      {
         cout<<"FPS: "<<stopWatch.fpsAverage<<"\n"<<flush;
         mFpsEmitCount = 0;
      }
   }

   int mFpsEmitCount;
   StopWatch stopWatch;
   
   /// Function called after pfDraw
   virtual void intraFrame()
   {
      //vjDEBUG(vjDBG_ALL,1) << "app::intraFrame\n" << vjDEBUG_FLUSH;
   }

public:
   // the sun
   pfLightSource* sun1;

   // navigation objects.
   CaveNavigator  mNavigator;
   pfDCS*         mNavigationDCS;
   
   // juggler device interface objects
   vjPosInterface          mWand;      // the Wand
   vjPosInterface          mHead;      // the Head
   vjDigitalInterface      mActionButton;
   vjDigitalInterface      mActionButton2;
   vjDigitalInterface      mModeChangeButton;
   
   // scene's root (as far as we're concerned here)
   pfGroup*   rootNode;
};


float transSpeed = 0.1;


int main(int argc, char* argv[])
{
    vjKernel* kernel = vjKernel::instance(); // Declare a new Kernel
    pfNavJugglerApplication* application = new pfNavJugglerApplication(kernel);  // Delcare an instance of my application
       
    cout<<"Usage: "<<argv[0]<<" modelfile.flt vjconfigfile[0] vjconfigfile[1] ... vjconfigfile[n]\n"<<flush;
    cout<<"\n"<<flush;
  
    if (argc < 2)
    {
       cout<<"\n\n[ERROR!!!] you must supply a .flt database (then config files)\n\n"<<flush;
       return 0;
    }    
    
    filename = argv[1];
    
    if (argc <= 2)
    {
       cout<<"\n\n[ERROR!!!] you must supply config files after the .flt file...\n\n"<<flush;
       return 0;
    }    
    
    for ( int i = 2; i < argc; i++ ) {
        kernel->loadConfigFile(argv[i]);
    }

    kernel->start();

    kernel->setApplication(application);    // Set up the kernel

    //while(!kernel->done())
    while(1)
     {;}
}


