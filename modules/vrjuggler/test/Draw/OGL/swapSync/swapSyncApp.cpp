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

#include <iostream>

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#include <gmtl/Generate.h>
#include <stdlib.h>
#include <vpr/System.h>
#include <vpr/Util/Debug.h>

#include "swapSyncApp.h"


namespace vrjTest
{

SwapSyncApp::SwapSyncApp(vrj::Kernel* kernel)
   : vrj::GlApp(kernel), mFrameNum(0)
{}

SwapSyncApp::~SwapSyncApp()
{}

void SwapSyncApp::init()
{
}

void SwapSyncApp::contextInit()
{
   initGLState();
}

void SwapSyncApp::preFrame()
{
   mFrameNum += 1;   
}

void SwapSyncApp::bufferPreDraw()
{
   unsigned choice = (mFrameNum % 5);
   
   if(0 == choice)
   { glClearColor(0.0, 0.0, 0.0, 0.0); }     // Black
   else if(1==choice)
   { glClearColor(1.0, 1.0, 1.0, 0.0); }     // White
   else if(2==choice)
   { glClearColor(1.0, 0.0, 0.0, 0.0); }     // Red
   else if(3==choice)
   { glClearColor(0.0, 1.0, 0.0, 0.0); }     // Green
   else if(4==choice)
   { glClearColor(0.0, 0.0, 1.0, 0.0); }     // Blue

   glClear(GL_COLOR_BUFFER_BIT);
}

void SwapSyncApp::draw()
{
   // Sleep for one of a predetermined times.
   // - Use this instead of random because we want to generate worst case
   //   on some of these sleeps (ie. one node 0, one node 2 seconds)
   unsigned sleep_choice = rand() % 3;
   vpr::Uint32 sleep_ms;
   
   if(0 == sleep_choice) { sleep_ms = 0; }
   else if(1 == sleep_choice) { sleep_ms = 1000; }   
   else if(2 == sleep_choice) { sleep_ms = 4000; }      
   
   /*
   float rand_num = drand48();
   float base_ms = 1500.0f;         // Sleep this much each time
   float var_ms = 2000.0f;          // Variability each frame
   
   vpr::Uint32 sleep_ms = vpr::Uint32(float(base_ms + (var_ms * rand_num)));
   */
   
   vpr::System::msleep(sleep_ms);   
   vprDEBUG(vprDBG_ALL, 0) << mFrameNum << ": Sleep: " << sleep_ms << "ms\n" << vprDEBUG_FLUSH;;
}

void SwapSyncApp::initGLState()
{
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glShadeModel(GL_SMOOTH);
}

} // namespace vrjTest
