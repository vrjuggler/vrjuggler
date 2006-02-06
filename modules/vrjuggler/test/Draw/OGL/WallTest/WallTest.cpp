/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <vpr/vprConfig.h>
#include <vrj/vrjConfig.h>

#ifdef VPR_OS_Darwin
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>

#include <gadget/Type/Position/PositionUnitConversion.h>

#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vrj/Display/DisplayManager.h>
#include <vrj/Display/SurfaceViewport.h>
#include <vrj/Display/Display.h>

#include "WallTest.h"
#include "WallTestHelpers.h"

#include "CubeLineMode.h"
#include "TestPatternMode.h"
#include "StereoEyeTestMode.h"
#include "DisplayInfoMode.h"

using namespace gmtl;
using namespace vrj;


void WallTest::init()
{
   // Initialize devices
   mWand.init("VJWand");
   mHead.init("VJHead");
   mButton0.init("VJButton0");
   mButton1.init("VJButton1");
   mButton2.init("VJButton2");
   mButton3.init("VJButton3");

   // Add test modes
   mTestModes.push_back(TestModePtr(new TestPatternMode));
   mTestModes.push_back(TestModePtr(new CubeLineMode));
   mTestModes.push_back(TestModePtr(new StereoEyeTestMode));
   mTestModes.push_back(TestModePtr(new DisplayInfoMode));
}


void WallTest::preFrame()
{
   if(mButton1->getData()==gadget::Digital::TOGGLE_ON)
   {
      mUseLights=!mUseLights;
   }

   /*
   if(mButton0->getData()==gadget::Digital::ON)
   {
      Matrix44f wandMatrix=mWand->getData();
      gmtl::setTrans(mLightPosition,wandMatrix);
      mLightPosition[3]=1;
   }
   else
   {
      mLightPosition=gmtl::Point4f(0,5,0,1);
   }
   */

   if(mButton0->getData() == gadget::Digital::TOGGLE_ON)
   {
      mCurMode += 1;
      if(mCurMode >= mTestModes.size())
      { mCurMode = 0; }
   }

   mTestModes[mCurMode]->update(this);
}


void WallTest::bufferPreDraw()
{   
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void WallTest::contextInit()
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glEnable(GL_DEPTH_TEST);
}

void WallTest::draw()
{   
   if(mUseLights)
   {
      glEnable(GL_NORMALIZE);
      glEnable(GL_LIGHT0);
      glLightfv(GL_LIGHT0,GL_POSITION,mLightPosition.getData());
      glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1f);
      glEnable(GL_COLOR_MATERIAL);
      glEnable(GL_LIGHTING);
   }
   else
   {
      glDisable(GL_LIGHTING);
   }
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   mTestModes[mCurMode]->draw(this);
}



