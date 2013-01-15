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

#include <vrj/Draw/OpenGL/Config.h>

#include <vector>
#include <gmtl/Vec.h>
#include <gmtl/Output.h>
#include <gmtl/VecOps.h>

#include <vrj/Draw/OpenGL/DrawAxesFunctors.h>

#define GL3_PROTOTYPES
#if defined(VPR_OS_Darwin) && (VPR_OS_RELEASE_MAJOR > 10)
//For Mac OS 10.7.x or greater the gl3.h file is supplied by the OS
#  include <OpenGL/gl3.h>
#else
//We append OpenGL here to force the use of the VR Juggler provided glcorearb.h
//header file instead of an OS supplied file on Windows and Linux platforms. In the
//future when the glcorearb file is ditributed with drivers and third party libraries
//this path can be changed to GL/glcorearb.h like GL/gl.h.
#  include <vrj/Draw/OpenGL/GL/glcorearb.h>
#endif

namespace vrj
{

namespace opengl
{

void DrawAxesCoreFunctor::contextInit()
{
   static GLfloat const VertexData[] = {
       0.0f,   0.0f,    0.0f,
       1.0f,   0.0f,    0.0f,
       0.0f,   0.0f,    0.0f,
       0.0f,   1.0f,    0.0f,
       0.0f,   0.0f,    0.0f,
       0.0f,   0.0f,    1.0f
   };

   static GLfloat const ColorData[] = {
       1.0f,   0.0f,    0.0f,
       1.0f,   0.0f,    0.0f,
       0.0f,   1.0f,    0.0f,
       0.0f,   1.0f,    0.0f,
       0.0f,   0.0f,    1.0f,
       0.0f,   0.0f,    1.0f
   };

   static const std::string& sVertexSource = " \
      attribute vec4 aVertexCoord;\n \
      attribute vec4 aVertexColor;\n \
      uniform vec4 uScaleFactor;\n \
      varying vec4 vFragColor;\n \
      void main(void) {\n \
         vFragColor = aVertexColor;\n \
         mat4 mMVPMatrix = gl_ProjectionMatrix * gl_ModelViewMatrix;\n \
         gl_Position = mMVPMatrix * (aVertexCoord * uScaleFactor);\n \
      }\n";

   static const std::string& sFragmentSource = " \
      varying vec4 vFragColor;\n \
      void main(void) {\n \
         gl_FragColor = vFragColor;\n \
      }\n";

   ExtensionLoaderGL& gl = DrawManager::instance()->getGL();
   std::string vertexSource = sVertexSource;
   std::string fragmentSource = sFragmentSource;

   // Prepend matrix uniforms if core profile
   if (gl.isCoreProfile())
   {
      vertexSource = " \
         uniform mat4 gl_ModelViewMatrix;\n \
         uniform mat4 gl_ProjectionMatrix;\n" + vertexSource;
   }

   mProgram = gl.createProgram(vertexSource, fragmentSource);
   if (gl.isCoreProfile())
   {
      muMVMatrixHandle = gl.GetUniformLocation(mProgram, "gl_ModelViewMatrix");
      muPMatrixHandle = gl.GetUniformLocation(mProgram, "gl_ProjectionMatrix");
   }
   maVertexCoordHandle = gl.GetAttribLocation(mProgram, "aVertexCoord");
   maVertexColorHandle = gl.GetAttribLocation(mProgram, "aVertexColor");
   muScaleFactorHandle = gl.GetUniformLocation(mProgram, "uScaleFactor");

   if (gl.GenVertexArrays != NULL)
      gl.GenVertexArrays(1, &mVertexArrayBufferID);
   gl.GenBuffers(1, &mVertexCoordBufferID);
   gl.GenBuffers(1, &mVertexColorBufferID);

   gl.BindBuffer(GL_ARRAY_BUFFER, mVertexCoordBufferID);
   gl.BufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), &VertexData[0], GL_STATIC_DRAW);
   gl.BindBuffer(GL_ARRAY_BUFFER, mVertexColorBufferID);
   gl.BufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), &ColorData[0], GL_STATIC_DRAW);
   gl.BindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawAxesCoreFunctor::draw(vrj::UserPtr)
{
   ExtensionLoaderGL& gl = DrawManager::instance()->getGL();

   gl.UseProgram(mProgram);
   if (mVertexArrayBufferID != 0)
      gl.BindVertexArray(mVertexArrayBufferID);

   float sfac = getScaleFactor();
   gl.Uniform4f(muScaleFactorHandle, sfac, sfac, sfac, 1.0f);

   gl.BindBuffer(GL_ARRAY_BUFFER, mVertexCoordBufferID);
   gl.VertexAttribPointer(maVertexCoordHandle,
      3, GL_FLOAT, false, 3 * sizeof(GLfloat), 0);
   gl.EnableVertexAttribArray(maVertexCoordHandle);

   gl.BindBuffer(GL_ARRAY_BUFFER, mVertexColorBufferID);
   gl.VertexAttribPointer(maVertexColorHandle,
      3, GL_FLOAT, false, 3 * sizeof(GLfloat), 0);
   gl.EnableVertexAttribArray(maVertexColorHandle);

   if (gl.isCoreProfile())
   {
      gl.UniformMatrix4fv(muMVMatrixHandle, 1, false, gl.getModelViewMatrix().mData);
      gl.UniformMatrix4fv(muPMatrixHandle, 1, false, gl.getProjectionMatrix().mData);
   }

   gl.DrawArrays(GL_LINES, 0, 18);

   gl.DisableVertexAttribArray(maVertexCoordHandle);
   gl.DisableVertexAttribArray(maVertexColorHandle);
   gl.BindBuffer(GL_ARRAY_BUFFER, 0);
   if (mVertexArrayBufferID != 0)
      gl.BindVertexArray(0);
   gl.UseProgram(0);
}

} // End of opengl namespace

} // End of vrj namespace
