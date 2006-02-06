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

using namespace gmtl;
using namespace vrj;

void WallTest::bufferPreDraw()
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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

   glEnable(GL_DEPTH_TEST);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
 
   DisplayManager* displayManager=vrj::GlDrawManager::instance()->getDisplayManager();
   std::vector<Display*> disps=displayManager->getAllDisplays();
    
  
   for(unsigned int i=0;i<disps.size();i++)
   {
      for (unsigned int v=0;v<disps[i]->getNumViewports();v++)
      {
         Viewport* viewport=disps[i]->getViewport(v);

         if (viewport->isSurface())
         {
            // Get a pointer to the surface
            SurfaceViewport* surface = dynamic_cast<SurfaceViewport*>(viewport);
            vprASSERT(surface!=NULL);

            drawWall(surface);            
         }
      }
   }  
}


void WallTest::drawWall(SurfaceViewport* surf)
{
   gmtl::Matrix44f rotate;
   gmtl::Vec3f center;
   float wall_width, wall_height;

   calcSurfaceParameters(surf, rotate, center, wall_width, wall_height);
   
   glPushMatrix();
   {
      // Translate to center of wall and rotate to looking down wall
      glTranslatef(center[0],center[1],center[2]);
      glMultMatrixf(rotate.getData());
         
      drawGrid(wall_width, wall_height);
      if(mUseCubesOrTriangles)
      {
         drawCubeOrTriangle();
      }      
      
      if(mUseCubes)
      {
         drawCubeLine();
      }
   }
   glPopMatrix();
}


void WallTest::preFrame()
{
   if(mButton1->getData()==gadget::Digital::TOGGLE_ON)
   {
      mUseLights=!mUseLights;
   }

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

   if(mButton2->getData()==gadget::Digital::TOGGLE_ON)
   {
      mUseCubesOrTriangles=!mUseCubesOrTriangles;
   }

   if(mButton3->getData()==gadget::Digital::TOGGLE_ON)
   {
      mUseCubes=!mUseCubes;
   }
}

void WallTest::drawGrid(float wallWidth, float wallHeight)
{
   // Scale to size of wall and then to 10 units wide and tall
   glPushMatrix();
   {
      glScalef(wallWidth, wallHeight, 1.0f);
      glScalef(0.1, 0.1, 0.1);
   
      glPushAttrib(GL_LIGHTING);
      glDisable(GL_LIGHTING);
   
      glLineWidth(2);
      
      glBegin(GL_LINES);
   
      float x;
      for(x=-5;x<=5;x+=0.5)
      {
         if(x==-2.5)      glColor3f(0,1,1);
         else if(x==-2)   glColor3f(1,1,0);
         else if(x==-1.5) glColor3f(1,0,1);
         else if(x==-1)   glColor3f(1,0,0);
         else if(x==-0.5) glColor3f(0,1,0);
         else if(x==0)    glColor3f(0,0,1);
         else if(x==0.5)  glColor3f(0,1,0);
         else if(x==1)    glColor3f(1,0,0);
         else if(x==1.5)  glColor3f(1,0,1);
         else if(x==2)    glColor3f(1,1,0);
         else if(x==2.5)  glColor3f(0,1,1);
         else             glColor3f(1,1,1);
   
         glVertex2f(x,-5);
         glVertex2f(x,5);
   
         glVertex2f(-5,x);
         glVertex2f(5,x);
      }
   
      glEnd();
   
      glColor3f(1,1,0);
      glBegin(GL_LINE_LOOP);
      for ( x = 0; x < Math::TWO_PI; x += Math::PI / 20.0 )
      {
         glVertex2f(sin(x)*5,cos(x)*5);
      }
      glEnd();
   
      glColor3f(0,1,1);
      glLineWidth(5);
      glBegin(GL_LINES);
         glVertex2f(-1,0);
         glVertex2f(1,0);
         glVertex2f(0,-1);
         glVertex2f(0,1);
      glEnd();
      glLineWidth(1);
   
      glPopAttrib();
   }
   glPopMatrix();
}

void WallTest::drawCubeLine()
{
   glColor3f(0,0,1);
	   
   for(float z=5;z>=-25;z-=2.5)
   {
      glPushMatrix();
         glTranslatef(0,0,z);
         glScalef(0.25,0.25,0.25);
         drawCube();
      glPopMatrix();
   }
}


void WallTest::drawCubeOrTriangle()
{
   vrj::Projection *currentProjection=vrj::GlDrawManager::instance()->currentUserData()->getProjection();
   int currentEye=currentProjection->getEye();

   glPushMatrix();
   
   if(currentEye==vrj::Projection::LEFT)
   {
      glTranslatef(1.5,0,0);
      glColor3f(1,0,0);
      drawCube();
   }
   else
   {
      glTranslatef(-1.5,0,0);
      glColor3f(0,1,0);
	   drawPyramid();
   }

   glPopMatrix();
}
