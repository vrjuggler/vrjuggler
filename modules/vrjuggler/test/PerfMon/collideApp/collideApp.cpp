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

#include <vrj/vrjConfig.h>
#include <vpr/Perf/ProfileManager.h>
#include <collideApp.h>

using namespace gmtl;
using namespace vrj;

// initialization
void collideApp::init()
{
   // initialize input devices
   cout << "App:init()" << endl;
   mWand.init("VJWand");
   mHead.init("VJHead");
   mButton0.init("VJButton0");
   mButton1.init("VJButton1");
   
   // initialize 10 balls
   for (int i = 0; i < 10; ++i)
   {
      // random positioning of Spheres
      Point3f  p((i % 5) + (i % 4) - 2.0, (i % 2) + (i % 3) + 1.0,
                 (i % 3) + (i % 2) + 1.0);

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
   for (int a = -5; a < 6; ++a)
   {
      for (int b = -5; b < 6; ++b)
      {
         m1.push_back(Point3f( (float) a, 0.0f, (float) b));
      }
   }
   // front wiring
   for (int a = -5; a < 6; ++a)
   {
      for (int b = 0; b < 11; ++b)
      {
         m2.push_back(Point3f( (float) a, (float) b, 5.0f));
      }
   }
   // back wiring
   for (int a = -5; a < 6; ++a)
   {
      for (int b = 0; b < 11; ++b)
      {
         m3.push_back(Point3f( (float) a, (float) b, -5.0f));
      }
   }
   // left wiring
   for (int a = -5; a < 6; ++a)
   {
      for (int b = 0; b < 11; ++b)
      {
         m4.push_back(Point3f( -5.0f, (float) b, (float) a));
      }
   }
   // right wiring
   for (int a = -5; a < 6; ++a)
   {
      for (int b = 0; b < 11; ++b)
      {
         m5.push_back(Point3f( 5.0f, (float) b, (float) a));
      }
   }
   // top wiring
   for (int a = -5; a < 6; ++a)
   {
      for (int b = -5; b < 6; ++b)
      {
         m6.push_back(Point3f( (float) a, 10.0f, (float) b));
      }
   }
}

void collideApp::bufferPreDraw()
{
   VPR_PROFILE_GUARD("bufferPreDraw");
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   glClear(GL_COLOR_BUFFER_BIT);
}

void collideApp::draw()
{
   VPR_PROFILE_GUARD("draw");
   glClear(GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   Matrix44f wand_matrix;
   wand_matrix = mWand->getData();
   float wand_color[3];
   wand_color[0] = wand_color[1] = wand_color[2] = 0.2f;
   
   if (pc1 == true)
   {
      glColor3f(1.0f, 0.0f, 0.0f);
   }
   else
   {
      glColor3f(0.0f, 1.0f, 0.0f);
   }  
   // bottom wiring
   glBegin(GL_LINES);
      for (int x = 0; x < 111; x+=11)
      {
         glVertex3fv(m1[x].getData());
         glVertex3fv(m1[x+10].getData());
      }
   glEnd();
   glBegin(GL_LINES);
      for (int x = 0; x < 11; x++)
      {
         glVertex3fv(m1[x].getData());
         glVertex3fv(m1[x+110].getData());
      }
   glEnd();

   if (pc2 == true)
   {
      glColor3f(1.0f, 0.0f, 0.0f);
   }
   else
   {
      glColor3f(0.0f, 1.0f, 0.0f);
   }  
   // front wiring
   glBegin(GL_LINES);
      for (int x = 0; x < 111; x+=11)
      {
         glVertex3fv(m2[x].getData());
         glVertex3fv(m2[x+10].getData());
      }
   glEnd();
   glBegin(GL_LINES);
      for (int x = 0; x < 11; x++)
      {
         glVertex3fv(m2[x].getData());
         glVertex3fv(m2[x+110].getData());
      }
   glEnd();

   if (pc3 == true)
   {
      glColor3f(1.0f, 0.0f, 0.0f);
   }
   else
   {
      glColor3f(0.0f, 1.0f, 0.0f);
   }  
   // back wiring
   glBegin(GL_LINES);
      for (int x = 0; x < 111; x+=11)
      {
         glVertex3fv(m3[x].getData());
         glVertex3fv(m3[x+10].getData());
      }
   glEnd();
   glBegin(GL_LINES);
      for (int x = 0; x < 11; x++)
      {
         glVertex3fv(m3[x].getData());
         glVertex3fv(m3[x+110].getData());
      }
   glEnd();
   
   if (pc4 == true)
   {
      glColor3f(1.0f, 0.0f, 0.0f);
   }
   else
   {
      glColor3f(0.0f, 1.0f, 0.0f);
   }  
   // left wiring
   glBegin(GL_LINES);
      for (int x = 0; x < 111; x+=11)
      {
         glVertex3fv(m4[x].getData());
         glVertex3fv(m4[x+10].getData());
      }
   glEnd();
   glBegin(GL_LINES);
      for (int x = 0; x < 11; x++)
      {
         glVertex3fv(m4[x].getData());
         glVertex3fv(m4[x+110].getData());
      }
   glEnd();

   if (pc5 == true)
   {
      glColor3f(1.0f, 0.0f, 0.0f);
   }
   else
   {
      glColor3f(0.0f, 1.0f, 0.0f);
   }  
   // right wiring
   glBegin(GL_LINES);
      for (int x = 0; x < 111; x+=11)
      {
         glVertex3fv(m5[x].getData());
         glVertex3fv(m5[x+10].getData());
      }
   glEnd();
   glBegin(GL_LINES);
      for (int x = 0; x < 11; x++)
      {
         glVertex3fv(m5[x].getData());
         glVertex3fv(m5[x+110].getData());
      }
   glEnd();

   if (pc6 == true)
   {
      glColor3f(1.0f, 0.0f, 0.0f);
   }
   else
   {
      glColor3f(0.0f, 1.0f, 0.0f);
   }  
   // top wiring
   glBegin(GL_LINES);
      for (int x = 0; x < 111; x+=11)
      {
         glVertex3fv(m6[x].getData());
         glVertex3fv(m6[x+10].getData());
      }
   glEnd();
   glBegin(GL_LINES);
      for (int x = 0; x < 11; x++)
      {
         glVertex3fv(m6[x].getData());
         glVertex3fv(m6[x+110].getData());
      }
   glEnd();
   
   // spheres drawn here
   for(std::vector<Ball*>::iterator itr = mBalls.begin() ; itr != mBalls.end() ; ++itr)
   {
      (*itr)->draw();
   }
   
   pc1 = false;
   pc2 = false;
   pc3 = false;
   pc4 = false;
   pc5 = false;
   pc6 = false;
}

void collideApp::initGLState()
{
   GLfloat light0_ambient[]  = { 0.1f, 0.1f, 0.1f, 1.0f };
   GLfloat light0_diffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
   GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat light0_position[] = { 0.0f, 0.75f, 0.75f, 0.0f };

   GLfloat mat_ambient[]   = { 0.7f, 0.7f, 0.7f, 1.0f };
   GLfloat mat_diffuse[]   = { 1.0f, 0.5f, 0.8f, 1.0f };
   GLfloat mat_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_shininess[] = { 50.0f };
   GLfloat no_mat[]        = { 0.0f, 0.0f, 0.0f, 1.0f };

   glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION,  light0_position);

   glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient );
   glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse );
   glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular );
   glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess );
   glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);
}

// Member function to detect ball collision
template<class DATA_TYPE>
void collideApp::testBallCollision(Sphere<DATA_TYPE>& s1,
                                   Vec<DATA_TYPE, 3>& v1,
                                   Sphere<DATA_TYPE>& s2,
                                   Vec<DATA_TYPE, 3>& v2, DATA_TYPE delta,
                                   DATA_TYPE first, DATA_TYPE second,
                                   bool& hit)
{
   VPR_PROFILE_GUARD("testBallCollsion");
   bool collide_balls = false;
   
   collide_balls = intersect(s1, v1, s2, v2, first, second);
   
   if ((collide_balls) && (first < delta))
   {
      // Find normal for ball 1 and 2
      Vec3f norm1((s1.getCenter() + s2.getCenter())/2.0f);
      normalize(norm1);
      Vec3f norm2((s2.getCenter() + s1.getCenter())/2.0f);
      normalize(norm2);

      // Find new velocity for ball 1 and 2
      Vec3f new_vel1;
      reflect(new_vel1, v1, norm1);
      Vec3f new_vel2;
      reflect(new_vel2, v2, norm2);
     
      // Update velocities for ball 1 and 2
      v1 = new_vel1;
      v2 = new_vel2;

      hit = true;
   }
}

// Member function to test plane collision
template<class DATA_TYPE>
void collideApp::testPlaneCollision(Plane<DATA_TYPE>& plane,
                                    Sphere<DATA_TYPE>& sp,
                                    Vec<DATA_TYPE, 3>& vel, DATA_TYPE delta,
                                    bool& hit) 
{
   VPR_PROFILE_GUARD("testPlaneCollision");
   float dist = distance(plane, sp.getCenter());
 
   if ((dist < RADIUS) || (dist == RADIUS))
   {
      Vec3f new_vel;
      reflect(new_vel, vel, plane.getNormal());
            
      // updates velocity
      vel = new_vel;
      hit = true;

      if (plane == p1) {pc1 = true;}
      else if (plane == p2) {pc2 = true;}
      else if (plane == p3) {pc3 = true;}
      else if (plane == p4) {pc4 = true;}
      else if (plane == p5) {pc5 = true;}
      else if (plane == p6) {pc6 = true;}
   }
}

void collideApp::increase()
{
   if (mButton0->getData())
   {
      // find # of balls to correctly limit # of balls drawn
      int k = 0;
      for ( vector<Ball*>::iterator m = mBalls.begin(); m!= mBalls.end(); ++m)
      {
         k++;
      }   
      if(0 == k)
      {
         k = 1;
      }
      for (int h = 0; h < (30000000 / k); h++) {}   
      cout << "Increased ball count" << endl;
      
      // random location of new balls
      int rx, ry, rz;
      rx = rand() % 5 - 2;
      ry = rand() % 8 + 1;
      rz = rand() % 5 - 2;
         
      Point3f  p(rx, ry, rz);
      Spheref s(p, RADIUS);
      Vec3f v( 0.1 * (k % 4) + 0.1, -0.1 * (k % 3) + 0.1, 0.1 * (k % 2) + 0.1);
         
      Ball* temp = new Ball(s, v);

      // assigns the color of the new ball
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
}

void collideApp::decrease()
{
   if (mButton1->getData())
   {
      // find # of balls to correctly limit # of balls removed
      int k = 0;
      for( vector<Ball*>::iterator m = mBalls.begin(); m != mBalls.end(); ++m)
      {
         k++;
      }

      // only removes balls if there are balls present
      if (k != 0)
      {
         for (int h = 0; h < (20000000 / k); h++) {}
         
         cout << "Decreased ball count" << endl;
         mBalls.pop_back(); 
      }
   }
}

void collideApp::preFrame()
{   
   VPR_PROFILE_GUARD("preFrame");
   float hit1 = 0.0f;
   float hit2 = 0.0f;
   
   delta = 0.05f; // increase to speed up, decrease to slow down ***************
   
   increase();
   decrease();
    
   for( vector<Ball*>::iterator i = mBalls.begin(); i != mBalls.end(); ++i)
   {              
      (*i)->hCheck = false;
      
      // test against planes
      testPlaneCollision(p1, (*i)->mSphere, (*i)->mVel, delta, (*i)->hCheck);
      testPlaneCollision(p2, (*i)->mSphere, (*i)->mVel, delta, (*i)->hCheck);
      testPlaneCollision(p3, (*i)->mSphere, (*i)->mVel, delta, (*i)->hCheck);
      testPlaneCollision(p4, (*i)->mSphere, (*i)->mVel, delta, (*i)->hCheck);
      testPlaneCollision(p5, (*i)->mSphere, (*i)->mVel, delta, (*i)->hCheck);
      testPlaneCollision(p6, (*i)->mSphere, (*i)->mVel, delta, (*i)->hCheck);

      for(vector<Ball*>::iterator j = mBalls.begin(); j != mBalls.end(); ++j)
      {         
         if (j != i)
         {
            if ((*i)->hCheck == false) // can take this out for more collisions
            {
               testBallCollision((*i)->mSphere, (*i)->mVel, (*j)->mSphere,
                                 (*j)->mVel, delta, hit1, hit2, (*i)->hCheck);
            }
         }
      }
   }
         
   // Apply physics to move all of the balls.
   for( vector<Ball*>::iterator i = mBalls.begin(); i != mBalls.end(); ++i)
   {
      (*i)->update(delta);
   }
}

