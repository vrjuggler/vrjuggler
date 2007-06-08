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
#ifndef _APPLICATION_DATA_APP_
#define _APPLICATION_DATA_APP_

#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <vrj/Draw/OGL/GlApp.h>
#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <vpr/IO/SerializableObject.h>
#include <plugins/ApplicationDataManager/UserData.h>
#include "MyType.h"

#include <cluster/ClusterNetwork.h>

namespace vrjTest
{

/**
 * Demonstration OpenGL application class
 *
 * This application simply renders a box on the wand with the axes for the
 * wand's coordinate space.
 */
class applicationDataApp : public vrj::GlApp
{
public:
   /** Constructor. */
   applicationDataApp(vrj::Kernel* kernel);

   /** Destructor. */
   ~applicationDataApp();

   /**
    * Execute any initialization needed before the API is started<BR><BR>
    * Initialize VR Juggler device interfaces here.
    *
    * This is called once before OGL is initialized
    */
   void init();

   /**
    * Function that is called immediately after a new OGL context is created.
    * This is called for every display window that is opened.  Initialize GL
    * state here. Also used to create context specific information<BR>
    *
    * This is called once for each display (twice for each stereo display)
    */
   void contextInit();

   /**
    * Function called after tracker update but before start of drawing<BR><BR>
    * Called once before every frame.  Do calculations and state modifications
    * here.
    */
   void preFrame();

   void bufferPreDraw();

   /**
    * Function to draw the scene.  Put OpenGL draw functions here.
    *
    * PRE: OpenGL state has correct transformation and buffer selected
    * POST: The current scene has been drawn<BR><BR>
    *
    * called [once * number of displays] per frame
    */
   void draw();
   
   void drawNetwork();

private:
   void initGLState();

   void drawCube();
private:
   gadget::PositionInterface    mWand;
   gadget::PositionInterface    mHead;
   gadget::DigitalInterface     mButton0;
   gadget::DigitalInterface     mButton1;
   gadget::DigitalInterface     mButton2;
   gadget::DigitalInterface     mButton3;
   gadget::DigitalInterface     mButton4;
   gadget::DigitalInterface     mButton5;
   /**
    * We can either derive our class from SerialableObject or use the
    * SerializableObjectMixin class and specialize the readObject and
    * writeObject functions somewhere in our application.
    */
   cluster::UserData< vpr::SerializableObjectMixin<MyType> >  mMyData;
   cluster::UserData< vpr::SerializableObjectMixin<MyType> >  mMyDataCopy;
   std::vector<gmtl::Vec3f>     mHeadHistory;
};

}

#endif
