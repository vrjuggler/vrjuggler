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

#ifndef _SWAP_SYNC_APP_
#define _SWAP_SYNC_APP_

#include <vrj/Draw/OpenGL/App.h>

namespace vrjTest
{

/**
 * Simple stupid test application.
 * 
 * Just iteratively clears teh buffer to a color and then swaps.  Does
 * this with a different color each frame.  The point is to allow
 * easy visual detection of synchronized swapping.
 */
class SwapSyncApp : public vrj::opengl::App
{
public:
   SwapSyncApp(vrj::Kernel* kernel);
   ~SwapSyncApp();

   void init();
   void contextInit();
   void preFrame();
   void bufferPreDraw();

   void draw();

private:
   void initGLState();
   
   unsigned mFrameNum;
};

}

#endif
