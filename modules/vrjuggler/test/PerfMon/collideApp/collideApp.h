/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _COLLIDE
#define _COLLIDE

#include <vrj/vrjConfig.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef VPR_OS_Darwin
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <vrj/Draw/OGL/GlApp.h>
#include <gadget/Type/PositionInterface.h>
#include <gadget/Type/DigitalInterface.h>
#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Sphere.h>
#include <gmtl/SphereOps.h>
#include <gmtl/Plane.h>
#include <gmtl/PlaneOps.h>
#include <gmtl/Point.h>
#include <gmtl/Output.h>
#include <gmtl/Intersection.h>
#include <gmtl/Generate.h>
#include <gmtl/LineSeg.h>
#include <gmtl/Ray.h>
#include <vector>

using namespace vrj;
using namespace gmtl;
using namespace std;

static const float RADIUS = 0.25;

//*******************************************************************
// BALL CLASS
// This class holds all information for our spheres
class Ball
{
public:
   Ball(Spheref s, Vec3f vel) : mSphere(s), mVel(vel)
   {;}
      
   void init()
   {
      // Create a GLU Quadric object
      mSphereObj = gluNewQuadric();
   }
      
   void draw()
   {
      // Draw a Sphere
      glPushMatrix();
                  
      glColor3f(c1, c2, c3);
      glTranslatef(mSphere.getCenter()[0],
                   mSphere.getCenter()[1],
                   mSphere.getCenter()[2]);
      gluSphere(mSphereObj, RADIUS, 16, 16);
      glPopMatrix();
   }

   void update(const float delta)
   {
      Vec3f pos = mSphere.getCenter();
      pos = pos + (mVel * delta);
      mSphere.setCenter(pos);
   }
   
   Spheref 	  mSphere;        // Sphere object
   Vec3f 	  mVel;           // Velocity vector of Ball
   GLUquadricObj* mSphereObj;     
   double         c1, c2, c3;     // Assigns Color
   bool           hCheck;         // keeps track of collision
};
//*****************************************************************


/**
 * Simple OGL Demonstration application
 *
 * Various new features of VR Juggler are demonstrated here
 * using collision detection.
 */
class collideApp : public GlApp
{
public:
   collideApp()
   {;}

   virtual ~collideApp()
   {;}

public:
   virtual void init();

   virtual void apiInit()
   {;}

public:
   virtual void bufferPreDraw();

   // Sphere against Sphere collision 
   template<class DATA_TYPE>
   void testBallCollision(Sphere<DATA_TYPE>& s1, Vec<DATA_TYPE, 3>& v1,
                          Sphere<DATA_TYPE>& s2, Vec<DATA_TYPE, 3>& v2,
                          DATA_TYPE delta, DATA_TYPE first, DATA_TYPE second,
                          bool& hit);

   // Sphere against Plane collision
   template<class DATA_TYPE>
   void testPlaneCollision(Plane<DATA_TYPE>& plane, Sphere<DATA_TYPE>& sp,
                           Vec<DATA_TYPE, 3>& vel, DATA_TYPE delta, bool& hit);
   
   // Increase number of balls
   virtual void increase();
       	 
   // Decrease number of balls
   virtual void decrease();
   
   virtual void preFrame();
   virtual void intraFrame() {;}
   virtual void postFrame() {;}

public:
   virtual void contextInit()
   {
      initGLState();
   }
      
   virtual void draw();

private:
   void initGLState();

public:
   vector<Ball*> mBalls;
   Planef p1,p2,p3,p4,p5,p6;
   vector<Point3f> m1,m2,m3,m4,m5,m6;
   bool pc1,pc2,pc3,pc4,pc5,pc6;
   float delta;                      // rate of application/demo
   
   gadget::PositionInterface mWand;
   gadget::PositionInterface mHead;
   gadget::DigitalInterface mButton0;
   gadget::DigitalInterface mButton1;
};

#endif
