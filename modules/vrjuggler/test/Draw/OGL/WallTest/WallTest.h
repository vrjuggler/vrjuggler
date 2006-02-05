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

/* This Application draws a test pattern in 3D on all the walls in the application
 * It also draws a cube on the left eye, and a tetrahedron on the right eye
 * Button 0 moves the light (off initially)
 * Button 1 toggles lighting
 * Button 2 toggles a line of cubes through the screen on and off (Cubes are 1/2 foot across)
 * Button 3 toggles the cube/tetrahedron on off
 */

#ifndef WALLTEST_H
#define WALLTEST_H

#include <vrj/vrjConfig.h>
#include <vrj/Draw/OGL/GlApp.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>

using namespace vrj;

/** Test program for wall settings.
 *
 */
class WallTest : public GlApp
{
public:
   WallTest()
   {
      mUseLights=false;
      mUseCubesOrTriangles=true;
      mUseCubes=true;
   }

   virtual ~WallTest(){;}

public:
   virtual void init()
   {
      // Initialize devices
      mWand.init("VJWand");
      mHead.init("VJHead");
      mButton0.init("VJButton0");
      mButton1.init("VJButton1");
      mButton2.init("VJButton2");
      mButton3.init("VJButton3");
   }

   virtual void apiInit(){;}

public:
   virtual void bufferPreDraw();

   virtual void preFrame();
   virtual void intraFrame(){;}
   virtual void postFrame(){;}

   virtual void contextInit(){;}
   virtual void draw();
   virtual void drawC6();
 
   void drawGrid();
   void drawCubeLine();
   void drawCube();
   void drawCubeOrTriangle();
 
   void drawWall(gmtl::Point3f ll,gmtl::Point3f lr,gmtl::Point3f ul,gmtl::Point3f ur);
   gmtl::Matrix44f calculateSurfaceRotation(gmtl::Point3f ll,gmtl::Point3f lr,gmtl::Point3f ul,gmtl::Point3f ur);
	
public:
   bool mUseLights;
   bool mUseCubesOrTriangles;
   bool mUseCubes;
   gmtl::Point4f mLightPosition;
   
   gadget::PositionInterface  mWand;    /**< Positional interface for Wand position */
   gadget::PositionInterface  mHead;    /**< Positional interface for Head position */
   gadget::DigitalInterface   mButton0; /**< Digital interface for button 0 */
   gadget::DigitalInterface   mButton1; /**< Digital interface for button 1 */
   gadget::DigitalInterface   mButton2;
   gadget::DigitalInterface   mButton3;
};


#endif
