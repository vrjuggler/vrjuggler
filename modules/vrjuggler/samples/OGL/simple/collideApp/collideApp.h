/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * File:          collideApp.h,v
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
   
      Spheref 	        mSphere;        // Sphere object
      Vec3f 	        mVel;           // Velocity vector of Ball
      GLUquadricObj*    mSphereObj;     
      double            c1, c2, c3;     // Assigns Color
      bool              hCheck;         // keeps track of collision
};
//*****************************************************************


/*
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

      virtual ~collideApp (void) {;}

   public:
      // initialization
      virtual void init()
      {
         // initialize input devices
         cout << "App:init()" << endl;
         mWand.init("VJWand");
         mHead.init("VJHead");
         mButton0.init("VJButton0");
         mButton1.init("VJButton1");
      
         // initialize 10 balls
         for (int i = 0; i < 10; i++)
         {
            // random positioning of Spheres
            Point3f  p((i % 5) + (i % 4) - 2.0, (i % 2) + (i % 3) + 1.0, (i % 3) + (i % 2) + 1.0);

            Spheref s(p, RADIUS);
            Vec3f v( 0.1 * (i % 4) + 0.1, -0.1 * (i % 3) + 0.1, 0.1 * (i % 2) + 0.1);

            // a new ball
            Ball* temp = new Ball(s, v);

            std::cout << "Adding ball at: " << p << std::endl;
            
            // assigns the color of the ball
            unsigned int r1 = rand() % 10;
            unsigned int r2 = rand() % 10;
            unsigned int r3 = rand() % 10;
            temp->c1 = (double) r1 / 10;
            temp->c2 = (double) r2 / 10;
            temp->c3 = (double) r3 / 10;

            temp->hCheck = false;

            mBalls.push_back(temp);
	    temp->init();
         }
      
         // initializing 6 planes
         p1 = Planef(Vec3f (0.0, 1.0, 0.0), Point3f (0.0, 0.0, 0.0));   // bottom
         p2 = Planef(Vec3f (0.0, 0.0, -1.0), Point3f (0.0, 5.0, 5.0));  // front
         p3 = Planef(Vec3f (0.0, 0.0, 1.0), Point3f (0.0, 5.0, -5.0));  // back
         p4 = Planef(Vec3f (1.0, 0.0, 0.0), Point3f (-5.0, 5.0, 0.0));  // left
         p5 = Planef(Vec3f (-1.0, 0.0, 0.0), Point3f (5.0, 5.0, 0.0));  // right
         p6 = Planef(Vec3f (0.0, -1.0, 0.0), Point3f (0.0, 10.0, 0.0)); // top
         pc1 = false;
         pc2 = false;
         pc3 = false;
         pc4 = false;
         pc5 = false;
         pc6 = false;
         // bottom wiring
         for (int a = -5; a < 6; a++)
         {
            for (int b = -5; b < 6; b++)
            {
               m1.push_back(Point3f( (float) a, 0.0f, (float) b));
            }
         }
         // front wiring
         for (int a = -5; a < 6; a++)
         {
            for (int b = 0; b < 11; b++)
            {
               m2.push_back(Point3f( (float) a, (float) b, 5.0f));
            }
         }
         // back wiring
         for (int a = -5; a < 6; a++)
         {
            for (int b = 0; b < 11; b++)
            {
               m3.push_back(Point3f( (float) a, (float) b, -5.0f));
            }
         }
         // left wiring
         for (int a = -5; a < 6; a++)
         {
            for (int b = 0; b < 11; b++)
            {
               m4.push_back(Point3f( -5.0f, (float) b, (float) a));
            }
         }
         // right wiring
         for (int a = -5; a < 6; a++)
         {
            for (int b = 0; b < 11; b++)
            {
               m5.push_back(Point3f( 5.0f, (float) b, (float) a));
            }
         }
         // top wiring
         for (int a = -5; a < 6; a++)
         {
            for (int b = -5; b < 6; b++)
            {
               m6.push_back(Point3f( (float) a, 10.0f, (float) b));
            }
         }
      }

      virtual void apiInit()
      {;}

   public:
      virtual void bufferPreDraw();

      // Sphere against Sphere collision 
      template<class DATA_TYPE> void testBallCollision(Sphere<DATA_TYPE>& s1, Vec<DATA_TYPE, 3>& v1, Sphere<DATA_TYPE>& s2, Vec<DATA_TYPE, 3>& v2, DATA_TYPE delta, DATA_TYPE first, DATA_TYPE second, bool& hit);

      // Sphere against Plane collision
      template<class DATA_TYPE> void testPlaneCollision(Plane<DATA_TYPE>& plane, Sphere<DATA_TYPE>& sp, Vec<DATA_TYPE, 3>& vel, DATA_TYPE delta, bool& hit);
   
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
