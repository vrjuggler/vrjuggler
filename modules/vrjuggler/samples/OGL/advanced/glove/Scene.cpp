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
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

#include <Scene.h>


Scene::Scene()
    : _tableList(0), _floorList(0), _coneList(0), _coneHlList(0), _cubeList(0),
      _cubeHlList(0), _sphereList(0), _sphereHlList(0)
{
    //: Wood texture ---
    // RED channel
    woodBitmap[0] = 80;woodBitmap[3] = 0;woodBitmap[6] = 80;woodBitmap[9] = 80;
    woodBitmap[12] = 80;woodBitmap[15] = 0;woodBitmap[18] = 80;woodBitmap[21] = 80;
    woodBitmap[24] = 0;woodBitmap[27] = 80;woodBitmap[30] = 80;woodBitmap[33] = 0;
    woodBitmap[36] = 0;woodBitmap[39] = 80;woodBitmap[42] = 80;woodBitmap[45] = 80;

    // GREEN channel
    woodBitmap[1] = 60;woodBitmap[4] = 0;woodBitmap[7] = 60;woodBitmap[10] = 60;
    woodBitmap[13] = 60;woodBitmap[16] = 0;woodBitmap[19] = 60;woodBitmap[22] = 60;
    woodBitmap[25] = 0;woodBitmap[28] = 60;woodBitmap[31] = 60;woodBitmap[34] = 0;
    woodBitmap[37] = 0;woodBitmap[40] = 60;woodBitmap[43] = 60;woodBitmap[46] = 60;

    // BLUE channel
    woodBitmap[2] = 0;woodBitmap[5] = 0;woodBitmap[8] = 0;woodBitmap[11] = 0;
    woodBitmap[14] = 0;woodBitmap[17] = 0;woodBitmap[20] = 0;woodBitmap[23] = 0;
    woodBitmap[26] = 0;woodBitmap[29] = 0;woodBitmap[32] = 0;woodBitmap[35] = 0;
    woodBitmap[38] = 0;woodBitmap[41] = 0;woodBitmap[44] = 0;woodBitmap[47] = 0;

    //: Colorful rainbow texture ---
    // RED channel
    rainbowBitmap[0] = 80;rainbowBitmap[3] = 0;rainbowBitmap[6] = 0;rainbowBitmap[9] = 80;
    rainbowBitmap[12] = 80;rainbowBitmap[15] = 0;rainbowBitmap[18] = 0;rainbowBitmap[21] = 80;
    rainbowBitmap[24] = 0;rainbowBitmap[27] = 80;rainbowBitmap[30] = 80;rainbowBitmap[33] = 0;
    rainbowBitmap[36] = 0;rainbowBitmap[39] = 80;rainbowBitmap[42] = 80;rainbowBitmap[45] = 0;

    // GREEN channel
    rainbowBitmap[1] = 70;rainbowBitmap[4] = 0;rainbowBitmap[7] = 0;rainbowBitmap[10] = 70;
    rainbowBitmap[13] = 0;rainbowBitmap[16] = 70;rainbowBitmap[19] = 70;rainbowBitmap[22] = 0;
    rainbowBitmap[25] = 0;rainbowBitmap[28] = 70;rainbowBitmap[31] = 70;rainbowBitmap[34] = 0;
    rainbowBitmap[37] = 70;rainbowBitmap[40] = 0;rainbowBitmap[43] = 0;rainbowBitmap[46] = 70;

    // BLUE channel
    rainbowBitmap[2] = 90;rainbowBitmap[5] = 90;rainbowBitmap[8] = 0;rainbowBitmap[11] = 0;
    rainbowBitmap[14] = 90;rainbowBitmap[17] = 90;rainbowBitmap[20] = 0;rainbowBitmap[23] = 0;
    rainbowBitmap[26] = 0;rainbowBitmap[29] = 0;rainbowBitmap[32] = 90;rainbowBitmap[35] = 90;
    rainbowBitmap[38] = 0;rainbowBitmap[41] = 0;rainbowBitmap[44] = 0;rainbowBitmap[47] = 90;
}

Scene::~Scene()
{
    glDeleteLists(_tableList, 1);
    glDeleteLists(_floorList, 1);
    glDeleteLists(_coneList, 1);
    glDeleteLists(_coneHlList, 1);
    glDeleteLists(_cubeList, 1);
    glDeleteLists(_cubeHlList, 1);
    glDeleteLists(_sphereList, 1);
    glDeleteLists(_sphereHlList, 1);

#ifdef GL_VERSION_1_1
    glDeleteTextures(1, &_woodBitmapBindId);
#else
    glDeleteTexturesEXT(1, &_woodBitmapBindId);
#endif
}

void Scene::drawFloor()
{
    glCallList( _floorList );
}

void Scene::drawSphere(const float& width, const float& height,
                       const float& depth, const bool& highlighted)
{
    glPushMatrix();
        glScalef( width, height, depth );
        if (highlighted)
            glCallList( _sphereHlList );
        else glCallList( _sphereList );
    glPopMatrix();
}

void Scene::drawCone(const float& width, const float& height,
                     const float& depth, const bool& highlighted)
{
    glPushMatrix();
        glScalef( width, height, depth );
        if (highlighted)
            glCallList( _coneHlList );
        else glCallList( _coneList );
    glPopMatrix();
}

void Scene::drawCube(const float& width, const float& height,
                     const float& depth, const bool& highlighted)
{
    glPushMatrix();
        glScalef( width, height, depth );
        if (highlighted)
            glCallList( _cubeHlList );
        else glCallList( _cubeList );
    glPopMatrix();
}

void Scene::drawTable()
{
    glCallList( _tableList );
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

     glPolygonMode( GL_FRONT, GL_LINE );
     glLineWidth( 2 );
     glBegin(GL_TRIANGLE_STRIP);
     for (x = -10; x < 10; ++x)
     {
        glColor3f( 0.4f, 0.7f, 0.4f );
        glNormal3f( 0.1f, 0.9f, -0.1f );
        glVertex3f( (float)x, 0.01f,  y );

        glColor3f( 0.0f, 0.0f, 0.6f );
        glNormal3f( -0.1f, 0.9f, 0.1f );
        glVertex3f( (float)x, 0.01f,  y + 1.0f );
     }
     glEnd();

     glPolygonMode( GL_FRONT,  GL_FILL );
     glBegin(GL_TRIANGLE_STRIP);
     for ( x = -10; x < 10; ++x)
     {
        glColor3f( 0.0f, 0.0f, 0.4f );
        glNormal3f( 0.1f, 0.9f, -0.1f );
        glVertex3f( (float)x, 0.0f,  y );

        glColor3f( 0.2f, 0.2f, 0.0f );
        glNormal3f( -0.1f, 0.9f, 0.1f );
        glVertex3f( (float)x, 0.0f,  y + 1.0f );
     }
     glEnd();
  }
}

void Scene::_drawSphere(const float& width, const float& height,
                        const float& depth, const bool& highlighted)
{
    static GLUquadric* sphereQuadric = gluNewQuadric();

    glPushMatrix();
        glScalef( width, height, depth );
        gluSphere(sphereQuadric, 0.5f, 15, 15);
    glPopMatrix();

    // draw the highlight if requested.
    if ( highlighted == true )
    {
        glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT);
            glColor3f( 0.0f, 0.0f, 1.0f );
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glLineWidth( 2 );
            glDisable(GL_TEXTURE_2D);
            this->drawSphere( width+0.01f, height+0.01f, depth+0.01f, false );
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glPopAttrib();
    }
}

void Scene::_drawCone(const float& width, const float& height,
                      const float& depth, const bool& highlighted)
{
    static GLUquadric* cylinderQuadric = gluNewQuadric();

    glPushMatrix();
        glScalef( width, height, depth );
        gluCylinder(cylinderQuadric, 0.5f, 0.0f, 1.0f, 15, 15);
    glPopMatrix();

    // draw the highlight if requested.
    if ( highlighted == true )
    {
        glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT);
            glColor3f( 0.0f, 0.0f, 1.0f );
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glLineWidth( 2 );
            glDisable(GL_TEXTURE_2D);
            this->drawCone( width+0.01f, height+0.01f, depth+0.01f, false );
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glPopAttrib();
    }
}

void Scene::_drawCube(const float& _width, const float& _height,
                      const float& _depth, const bool& highlighted)
{
    float width = _width * 0.5f;
    float height = _height * 0.5f;
    float depth = _depth * 0.5f;

    // cube top
  glBegin(GL_TRIANGLE_STRIP);
     glTexCoord2f( 0.0f, 0.0f );
     glNormal3f( -0.75f, 0.75f, 0.75f );
     glVertex3f( -width, height,  depth );

     glTexCoord2f( 0.0f, 1.0f );
     glNormal3f(  0.75f, 0.75f, 0.75f );
     glVertex3f(  width, height,  depth );

     glTexCoord2f( 1.0f, 0.0f );
     glNormal3f( -0.75f, 0.75f, -0.75f );
     glVertex3f( -width, height, -depth );

     glTexCoord2f( 1.0f, 1.0f );
     glNormal3f(  0.75f, 0.75f, -0.75f );
     glVertex3f(  width, height, -depth );
  glEnd();

    // cube bottom
  glBegin(GL_TRIANGLE_STRIP);
     glTexCoord2f( 0.0f, 0.0f );
     glNormal3f(  0.75f, -0.75f, 0.75f );
     glVertex3f(  width, -height,  depth );

     glTexCoord2f( 0.0f, 1.0f );
     glNormal3f( -0.75f, -0.75f, 0.75f );
     glVertex3f( -width, -height,  depth );

     glTexCoord2f( 1.0f, 0.0f );
     glNormal3f(  0.75f, -0.75f, -0.75f );
     glVertex3f(  width, -height, -depth );

     glTexCoord2f( 1.0f, 1.0f );
     glNormal3f( -0.75f, -0.75f, -0.75f );
     glVertex3f( -width, -height, -depth );
  glEnd();

    // cube sides
  glBegin(GL_TRIANGLE_STRIP);
     glTexCoord2f( 0.0f, 0.0f );
     glNormal3f( -0.75f, 0.75f, 0.75f );
     glVertex3f( -width, height,  depth );

     glTexCoord2f( 0.0f, 1.0f );
     glNormal3f( -0.75f, -0.75f, 0.75f );
     glVertex3f( -width, -height, depth );

     glTexCoord2f( 1.0f, 0.0f );
     glNormal3f( 0.75f, 0.75f, 0.75f );
     glVertex3f( width, height,  depth );

     glTexCoord2f( 1.0f, 1.0f );
     glNormal3f( 0.75f, -0.75f, 0.75f );
     glVertex3f( width, -height, depth );

     glTexCoord2f( 1.0f, 0.0f );
     glNormal3f( 0.75f, 0.75f, -0.75f );
     glVertex3f( width, height,  -depth );

     glTexCoord2f( 1.0f, 1.0f );
     glNormal3f( 0.75f, -0.75f, -0.75f );
     glVertex3f( width, -height, -depth );

     glTexCoord2f( 1.0f, 0.0f );
     glNormal3f( -0.75f, 0.75f, -0.75f );
     glVertex3f( -width, height,  -depth );

     glTexCoord2f( 1.0f, 1.0f );
     glNormal3f( -0.75f, -0.75f, -0.75f );
     glVertex3f( -width, -height, -depth );

     glTexCoord2f( 1.0f, 0.0f );
     glNormal3f( -0.75f, 0.75f, 0.75f );
     glVertex3f( -width, height,  depth );

     glTexCoord2f( 1.0f, 1.0f );
     glNormal3f( -0.75f, -0.75f, 0.75f );
     glVertex3f( -width, -height, depth );
  glEnd();

  // draw the highlight if requested.
    if ( highlighted == true )
    {
        glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT);
            glColor3f( 0.0f, 0.0f, 1.0f );
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glLineWidth( 2 );
            glDisable(GL_TEXTURE_2D);
            this->drawCube( _width+0.01f, _height+0.01f, _depth+0.01f, false );
        glPopAttrib();
    }
}

void Scene::_drawTable()
{
  glColor3f( 0.4f, 0.3f, 0.1f );

    // table top (top side) - flat shaded
    glPushMatrix();
        glTranslatef(0.0f, 2.85f, 0.0f);
        this->drawCube( 10.0f, 0.25f, 4.0f, false );
    glPopMatrix();

    // Table Legs:
    glPushMatrix();
        glTranslatef( -4.65f, 1.5f, 1.65f );
        this->drawCube( 0.25f, 3.0f, 0.25f, false );
    glPopMatrix();

    glPushMatrix();
        glTranslatef( 4.65f, 1.5f, -1.65f );
        this->drawCube( 0.25f, 3.0f, 0.25f, false );
    glPopMatrix();

    glPushMatrix();
        glTranslatef( -4.65f, 1.5f, -1.65f );
        this->drawCube( 0.25f, 3.0f, 0.25f, false );
    glPopMatrix();

    glPushMatrix();
        glTranslatef( 4.65f, 1.5f, 1.65f );
        this->drawCube( 0.25f, 3.0f, 0.25f, false );
    glPopMatrix();
}

void Scene::renderWoodTexture()
{
#ifdef GL_VERSION_1_1
    glBindTexture(GL_TEXTURE_2D, _woodBitmapBindId);
#else
    glBindTextureEXT(GL_TEXTURE_2D, _woodBitmapBindId);
#endif
}

void Scene::renderRainbowTexture()
{
#ifdef GL_VERSION_1_1
    glBindTexture(GL_TEXTURE_2D, _rainbowBitmapBindId);
#else
    glBindTextureEXT(GL_TEXTURE_2D, _rainbowBitmapBindId);
#endif
}

void Scene::_renderTexture(char bitmap[16*3])
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
    _tableList = glGenLists(1);
    _floorList = glGenLists(1);

    _coneList = glGenLists(1);
    _cubeList = glGenLists(1);
    _sphereList = glGenLists(1);

    _coneHlList = glGenLists(1);
    _cubeHlList = glGenLists(1);
    _sphereHlList = glGenLists(1);

    glNewList(_tableList, GL_COMPILE);
        this->_drawTable();
    glEndList();

    glNewList(_floorList, GL_COMPILE);
        this->_drawFloor();
    glEndList();

    glNewList(_coneList, GL_COMPILE);
        this->_drawCone( 1.0f, 1.0f, 1.0f, false);
    glEndList();

    glNewList(_coneHlList, GL_COMPILE);
        this->_drawCone( 1.0f, 1.0f, 1.0f, true);
    glEndList();

    glNewList(_cubeList, GL_COMPILE);
        this->_drawCube( 1.0f, 1.0f, 1.0f, false);
    glEndList();

    glNewList(_cubeHlList, GL_COMPILE);
        this->_drawCube( 1.0f, 1.0f, 1.0f, true);
    glEndList();

    glNewList(_sphereList, GL_COMPILE);
        this->_drawSphere( 1.0f, 1.0f, 1.0f, false);
    glEndList();

    glNewList(_sphereHlList, GL_COMPILE);
        this->_drawSphere( 1.0f, 1.0f, 1.0f, true);
    glEndList();

#ifdef GL_VERSION_1_1
    glGenTextures(1, &_woodBitmapBindId);
    glBindTexture(GL_TEXTURE_2D, _woodBitmapBindId);
#else
    glGenTexturesEXT(1, &_woodBitmapBindId);
    glBindTextureEXT(GL_TEXTURE_2D, _woodBitmapBindId);
#endif
    this->_renderTexture(woodBitmap);
    std::cout<<"Loaded wood texture.\n"<<std::flush;

#ifdef GL_VERSION_1_1
    glGenTextures(1, &_rainbowBitmapBindId);
    glBindTexture(GL_TEXTURE_2D, _rainbowBitmapBindId);
#else
    glGenTexturesEXT(1, &_rainbowBitmapBindId);
    glBindTextureEXT(GL_TEXTURE_2D, _rainbowBitmapBindId);
#endif
    this->_renderTexture(rainbowBitmap);
    std::cout<<"Loaded rainbow texture.\n"<<std::flush;

    std::cout<<"Initialized scene.\n"<<std::flush;
}
