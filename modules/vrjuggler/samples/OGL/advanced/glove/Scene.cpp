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

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

#include <Scene.h>


Scene::Scene()
   : mTableList(0)
   , mFloorList(0)
   , mConeList(0)
   , mConeHlList(0)
   , mCubeList(0)
   , mCubeHlList(0)
   , mSphereList(0)
   , mSphereHlList(0)
{
   //: Wood texture ---
   // RED channel
   mWoodBitmap[0] = 80; mWoodBitmap[3] = 0; mWoodBitmap[6] = 80; mWoodBitmap[9] = 80;
   mWoodBitmap[12] = 80; mWoodBitmap[15] = 0; mWoodBitmap[18] = 80; mWoodBitmap[21] = 80;
   mWoodBitmap[24] = 0; mWoodBitmap[27] = 80; mWoodBitmap[30] = 80; mWoodBitmap[33] = 0;
   mWoodBitmap[36] = 0; mWoodBitmap[39] = 80; mWoodBitmap[42] = 80; mWoodBitmap[45] = 80;

   // GREEN channel
   mWoodBitmap[1] = 60; mWoodBitmap[4] = 0; mWoodBitmap[7] = 60; mWoodBitmap[10] = 60;
   mWoodBitmap[13] = 60; mWoodBitmap[16] = 0; mWoodBitmap[19] = 60; mWoodBitmap[22] = 60;
   mWoodBitmap[25] = 0; mWoodBitmap[28] = 60; mWoodBitmap[31] = 60; mWoodBitmap[34] = 0;
   mWoodBitmap[37] = 0; mWoodBitmap[40] = 60; mWoodBitmap[43] = 60; mWoodBitmap[46] = 60;

   // BLUE channel
   mWoodBitmap[2] = 0; mWoodBitmap[5] = 0; mWoodBitmap[8] = 0; mWoodBitmap[11] = 0;
   mWoodBitmap[14] = 0; mWoodBitmap[17] = 0; mWoodBitmap[20] = 0; mWoodBitmap[23] = 0;
   mWoodBitmap[26] = 0; mWoodBitmap[29] = 0; mWoodBitmap[32] = 0; mWoodBitmap[35] = 0;
   mWoodBitmap[38] = 0; mWoodBitmap[41] = 0; mWoodBitmap[44] = 0; mWoodBitmap[47] = 0;

   //: Colorful rainbow texture ---
   // RED channel
   mRainbowBitmap[0] = 80; mRainbowBitmap[3] = 0; mRainbowBitmap[6] = 0; mRainbowBitmap[9] = 80;
   mRainbowBitmap[12] = 80; mRainbowBitmap[15] = 0; mRainbowBitmap[18] = 0; mRainbowBitmap[21] = 80;
   mRainbowBitmap[24] = 0; mRainbowBitmap[27] = 80; mRainbowBitmap[30] = 80; mRainbowBitmap[33] = 0;
   mRainbowBitmap[36] = 0; mRainbowBitmap[39] = 80; mRainbowBitmap[42] = 80; mRainbowBitmap[45] = 0;

   // GREEN channel
   mRainbowBitmap[1] = 70; mRainbowBitmap[4] = 0; mRainbowBitmap[7] = 0; mRainbowBitmap[10] = 70;
   mRainbowBitmap[13] = 0; mRainbowBitmap[16] = 70; mRainbowBitmap[19] = 70; mRainbowBitmap[22] = 0;
   mRainbowBitmap[25] = 0; mRainbowBitmap[28] = 70; mRainbowBitmap[31] = 70; mRainbowBitmap[34] = 0;
   mRainbowBitmap[37] = 70; mRainbowBitmap[40] = 0; mRainbowBitmap[43] = 0; mRainbowBitmap[46] = 70;

   // BLUE channel
   mRainbowBitmap[2] = 90; mRainbowBitmap[5] = 90; mRainbowBitmap[8] = 0; mRainbowBitmap[11] = 0;
   mRainbowBitmap[14] = 90; mRainbowBitmap[17] = 90; mRainbowBitmap[20] = 0; mRainbowBitmap[23] = 0;
   mRainbowBitmap[26] = 0; mRainbowBitmap[29] = 0; mRainbowBitmap[32] = 90; mRainbowBitmap[35] = 90;
   mRainbowBitmap[38] = 0; mRainbowBitmap[41] = 0; mRainbowBitmap[44] = 0; mRainbowBitmap[47] = 90;
}

Scene::~Scene()
{
   glDeleteLists(mTableList, 1);
   glDeleteLists(mFloorList, 1);
   glDeleteLists(mConeList, 1);
   glDeleteLists(mConeHlList, 1);
   glDeleteLists(mCubeList, 1);
   glDeleteLists(mCubeHlList, 1);
   glDeleteLists(mSphereList, 1);
   glDeleteLists(mSphereHlList, 1);

#ifdef GL_VERSION_1_1
   glDeleteTextures(1, &mWoodBitmapBindId);
#else
   glDeleteTexturesEXT(1, &mWoodBitmapBindId);
#endif
}

void Scene::drawFloor()
{
   glCallList(mFloorList);
}

void Scene::drawSphere(const float width, const float height,
                       const float depth, const bool highlighted)
{
   glPushMatrix();
      glScalef(width, height, depth);
      if (highlighted)
      {
         glCallList(mSphereHlList);
      }
      else
      {
         glCallList(mSphereList);
      }
   glPopMatrix();
}

void Scene::drawCone(const float width, const float height,
                     const float depth, const bool highlighted)
{
   glPushMatrix();
      glScalef(width, height, depth);
      if (highlighted)
      {
         glCallList(mConeHlList);
      }
      else
      {
         glCallList(mConeList);
      }
   glPopMatrix();
}

void Scene::drawCube(const float width, const float height,
                     const float depth, const bool highlighted)
{
   glPushMatrix();
      glScalef(width, height, depth);
      if (highlighted)
      {
         glCallList(mCubeHlList);
      }
      else
      {
         glCallList(mCubeList);
      }
   glPopMatrix();
}

void Scene::drawTable()
{
   glCallList(mTableList);
}


///////////////////////////////////////////////
// Private... shape drawing routines:
///////////////////////////////////////////////

void Scene::_drawFloor()
{
   // floor
   for (int y = -10; y < 10; ++y)
   {
      int x;

      glPolygonMode(GL_FRONT, GL_LINE);
      glLineWidth(2);
      glBegin(GL_TRIANGLE_STRIP);
      for (x = -10; x < 10; ++x)
      {
          glColor3f(0.4f, 0.7f, 0.4f);
          glNormal3f(0.1f, 0.9f, -0.1f);
          glVertex3f((float) x, 0.01f, y);

          glColor3f(0.0f, 0.0f, 0.6f);
          glNormal3f(-0.1f, 0.9f, 0.1f);
          glVertex3f((float) x, 0.01f, y + 1.0f);
      }
      glEnd();

      glPolygonMode(GL_FRONT,  GL_FILL);
      glBegin(GL_TRIANGLE_STRIP);
      for ( x = -10; x < 10; ++x )
      {
         glColor3f(0.0f, 0.0f, 0.4f);
         glNormal3f(0.1f, 0.9f, -0.1f);
         glVertex3f((float) x, 0.0f, y);

         glColor3f(0.2f, 0.2f, 0.0f);
         glNormal3f(-0.1f, 0.9f, 0.1f);
         glVertex3f((float) x, 0.0f, y + 1.0f);
      }
      glEnd();
   }
}

void Scene::_drawSphere(const float width, const float height,
                        const float depth, const bool highlighted)
{
   static GLUquadric* sphereQuadric = gluNewQuadric();

   glPushMatrix();
      glScalef(width, height, depth);
      gluSphere(sphereQuadric, 0.5f, 15, 15);
   glPopMatrix();

   // draw the highlight if requested.
   if ( highlighted == true )
   {
      glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT);
         glColor3f(0.0f, 0.0f, 1.0f);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glLineWidth(2);
         glDisable(GL_TEXTURE_2D);
         this->drawSphere(width+0.01f, height+0.01f, depth+0.01f, false);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glPopAttrib();
   }
}

void Scene::_drawCone(const float width, const float height,
                      const float depth, const bool highlighted)
{
   static GLUquadric* cylinderQuadric = gluNewQuadric();

   glPushMatrix();
      glScalef(width, height, depth);
      gluCylinder(cylinderQuadric, 0.5f, 0.0f, 1.0f, 15, 15);
   glPopMatrix();

   // draw the highlight if requested.
   if ( highlighted == true )
   {
      glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT);
         glColor3f(0.0f, 0.0f, 1.0f);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glLineWidth(2);
         glDisable(GL_TEXTURE_2D);
         this->drawCone(width+0.01f, height+0.01f, depth+0.01f, false);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      glPopAttrib();
   }
}

void Scene::_drawCube(const float _width, const float _height,
                      const float _depth, const bool highlighted)
{
   const float width = _width * 0.5f;
   const float height = _height * 0.5f;
   const float depth = _depth * 0.5f;

   // cube top
   glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(0.0f, 0.0f);
      glNormal3f(-0.75f, 0.75f, 0.75f);
      glVertex3f(-width, height, depth);

      glTexCoord2f(0.0f, 1.0f);
      glNormal3f(0.75f, 0.75f, 0.75f);
      glVertex3f(width, height, depth);

      glTexCoord2f(1.0f, 0.0f);
      glNormal3f(-0.75f, 0.75f, -0.75f);
      glVertex3f(-width, height, -depth);

      glTexCoord2f(1.0f, 1.0f);
      glNormal3f(0.75f, 0.75f, -0.75f);
      glVertex3f(width, height, -depth);
   glEnd();

   // cube bottom
   glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(0.0f, 0.0f);
      glNormal3f(0.75f, -0.75f, 0.75f);
      glVertex3f(width, -height, depth);

      glTexCoord2f(0.0f, 1.0f);
      glNormal3f(-0.75f, -0.75f, 0.75f);
      glVertex3f(-width, -height, depth);

      glTexCoord2f(1.0f, 0.0f);
      glNormal3f(0.75f, -0.75f, -0.75f);
      glVertex3f(width, -height, -depth);

      glTexCoord2f(1.0f, 1.0f);
      glNormal3f(-0.75f, -0.75f, -0.75f);
      glVertex3f(-width, -height, -depth);
   glEnd();

   // cube sides
   glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(0.0f, 0.0f);
      glNormal3f(-0.75f, 0.75f, 0.75f);
      glVertex3f(-width, height, depth);

      glTexCoord2f(0.0f, 1.0f );
      glNormal3f(-0.75f, -0.75f, 0.75f);
      glVertex3f(-width, -height, depth);

      glTexCoord2f(1.0f, 0.0f );
      glNormal3f(0.75f, 0.75f, 0.75f);
      glVertex3f(width, height, depth);

      glTexCoord2f(1.0f, 1.0f);
      glNormal3f(0.75f, -0.75f, 0.75f);
      glVertex3f(width, -height, depth);

      glTexCoord2f(1.0f, 0.0f);
      glNormal3f(0.75f, 0.75f, -0.75f);
      glVertex3f(width, height, -depth);

      glTexCoord2f(1.0f, 1.0f);
      glNormal3f(0.75f, -0.75f, -0.75f);
      glVertex3f(width, -height, -depth);

      glTexCoord2f(1.0f, 0.0f);
      glNormal3f(-0.75f, 0.75f, -0.75f);
      glVertex3f(-width, height, -depth);

      glTexCoord2f(1.0f, 1.0f);
      glNormal3f(-0.75f, -0.75f, -0.75f);
      glVertex3f(-width, -height, -depth);

      glTexCoord2f(1.0f, 0.0f);
      glNormal3f(-0.75f, 0.75f, 0.75f);
      glVertex3f(-width, height, depth);

      glTexCoord2f(1.0f, 1.0f);
      glNormal3f(-0.75f, -0.75f, 0.75f);
      glVertex3f(-width, -height, depth);
   glEnd();

   // draw the highlight if requested.
   if ( highlighted == true )
   {
      glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT);
         glColor3f(0.0f, 0.0f, 1.0f);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glLineWidth(2);
         glDisable(GL_TEXTURE_2D);
         this->drawCube(_width+0.01f, _height+0.01f, _depth+0.01f, false);
      glPopAttrib();
   }
}

void Scene::_drawTable()
{
   glColor3f(0.4f, 0.3f, 0.1f);

   // table top (top side) - flat shaded
   glPushMatrix();
      glTranslatef(0.0f, 2.85f, 0.0f);
      this->drawCube( 10.0f, 0.25f, 4.0f, false );
   glPopMatrix();

   // Table Legs:
   glPushMatrix();
      glTranslatef(-4.65f, 1.5f, 1.65f);
      this->drawCube(0.25f, 3.0f, 0.25f, false);
   glPopMatrix();

   glPushMatrix();
      glTranslatef(4.65f, 1.5f, -1.65f);
      this->drawCube(0.25f, 3.0f, 0.25f, false);
   glPopMatrix();

   glPushMatrix();
      glTranslatef(-4.65f, 1.5f, -1.65f);
      this->drawCube(0.25f, 3.0f, 0.25f, false);
   glPopMatrix();

   glPushMatrix();
      glTranslatef(4.65f, 1.5f, 1.65f);
      this->drawCube(0.25f, 3.0f, 0.25f, false);
   glPopMatrix();
}

void Scene::renderWoodTexture()
{
#ifdef GL_VERSION_1_1
    glBindTexture(GL_TEXTURE_2D, mWoodBitmapBindId);
#else
    glBindTextureEXT(GL_TEXTURE_2D, mWoodBitmapBindId);
#endif
}

void Scene::renderRainbowTexture()
{
#ifdef GL_VERSION_1_1
   glBindTexture(GL_TEXTURE_2D, mRainbowBitmapBindId);
#else
   glBindTextureEXT(GL_TEXTURE_2D, mRainbowBitmapBindId);
#endif
}

void Scene::_renderTexture(char* bitmap)
{
   glEnable(GL_TEXTURE_2D);

   //make sure the alignment matches the pixel size in bytes
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   const int mipmapLevelOfDetail = 0;
   const int channels = 3;
   const int width = 4;
   const int height = 4;
   const int bordersize = 0;
   const int format = GL_RGB;
   const int type = GL_UNSIGNED_BYTE;
   glTexImage2D(GL_TEXTURE_2D, mipmapLevelOfDetail, channels, width, height,
                bordersize, format, type, bitmap);

   // set the filtering for the texture...
   // use mipmap filtering if making mipmaps.
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   //Set blending model
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

   //set repeat or clamp mode
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Scene::init()
{
   mTableList = glGenLists(1);
   mFloorList = glGenLists(1);

   mConeList = glGenLists(1);
   mCubeList = glGenLists(1);
   mSphereList = glGenLists(1);

   mConeHlList = glGenLists(1);
   mCubeHlList = glGenLists(1);
   mSphereHlList = glGenLists(1);

   glNewList(mTableList, GL_COMPILE);
      this->_drawTable();
   glEndList();

   glNewList(mFloorList, GL_COMPILE);
      this->_drawFloor();
   glEndList();

   glNewList(mConeList, GL_COMPILE);
      this->_drawCone(1.0f, 1.0f, 1.0f, false);
   glEndList();

   glNewList(mConeHlList, GL_COMPILE);
      this->_drawCone(1.0f, 1.0f, 1.0f, true);
   glEndList();

   glNewList(mCubeList, GL_COMPILE);
      this->_drawCube(1.0f, 1.0f, 1.0f, false);
   glEndList();

   glNewList(mCubeHlList, GL_COMPILE);
      this->_drawCube(1.0f, 1.0f, 1.0f, true);
   glEndList();

   glNewList(mSphereList, GL_COMPILE);
      this->_drawSphere(1.0f, 1.0f, 1.0f, false);
   glEndList();

   glNewList(mSphereHlList, GL_COMPILE);
      this->_drawSphere(1.0f, 1.0f, 1.0f, true);
   glEndList();

#ifdef GL_VERSION_1_1
   glGenTextures(1, &mWoodBitmapBindId);
   glBindTexture(GL_TEXTURE_2D, mWoodBitmapBindId);
#else
   glGenTexturesEXT(1, &mWoodBitmapBindId);
   glBindTextureEXT(GL_TEXTURE_2D, mWoodBitmapBindId);
#endif
   this->_renderTexture(mWoodBitmap);
   std::cout << "Loaded wood texture.\n" << std::flush;

#ifdef GL_VERSION_1_1
   glGenTextures(1, &mRainbowBitmapBindId);
   glBindTexture(GL_TEXTURE_2D, mRainbowBitmapBindId);
#else
   glGenTexturesEXT(1, &mRainbowBitmapBindId);
   glBindTextureEXT(GL_TEXTURE_2D, mRainbowBitmapBindId);
#endif
   this->_renderTexture(mRainbowBitmap);
   std::cout << "Loaded rainbow texture.\n" << std::flush;

   std::cout << "Initialized scene.\n" << std::flush;
}
