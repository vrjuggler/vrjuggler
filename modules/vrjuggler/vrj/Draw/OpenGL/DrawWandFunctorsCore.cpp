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

#include <vrj/Draw/OpenGL/DrawWandFunctors.h>

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

void DrawRightAngleWandCoreFunctor::contextInit()
{
   GLfloat const VertexData[] = {
       0.014f, -0.140f,  0.014f,
       0.014f,  0.028f,  0.014f,
      -0.014f,  0.028f,  0.014f,
      -0.014f, -0.140f,  0.014f,
       0.014f, -0.140f, -0.014f,
       0.014f,  0.000f, -0.014f,
      -0.014f, -0.140f, -0.014f,
      -0.014f,  0.000f, -0.014f,
       0.028f,  0.056f, -0.042f,
      -0.028f,  0.056f, -0.042f,
       0.028f,  0.042f, -0.042f,
      -0.028f,  0.042f, -0.042f,
   };

   GLfloat const NormData[] = {
       0.577f, -0.577f,  0.577f,
       0.577f,  0.577f,  0.577f,
      -0.577f,  0.577f,  0.577f,
      -0.577f, -0.577f,  0.577f,
       0.577f, -0.577f, -0.577f,
       0.577f,  0.577f, -0.577f,
      -0.577f, -0.577f, -0.577f,
      -0.577f,  0.577f, -0.577f,
       0.577f,  0.577f, -0.577f,
      -0.577f,  0.577f, -0.577f,
       0.577f,  0.577f, -0.577f,
      -0.577f,  0.577f, -0.577f
   };

   GLushort const Indices[] = {
       0,  1,  2,
       0,  2,  3,
       4,  5,  1,
       4,  1,  0,
       6,  7,  5,
       6,  5,  4,
       3,  2,  7,
       3,  7,  6,
       6,  4,  0,
       6,  0,  3,
       2,  1,  8,
       2,  8,  9,
      10, 11,  9,
      10,  9,  8,
       1,  5, 10,
       1, 10,  8,
       2,  9, 11,
      11,  7,  2,
      10,  5,  7,
      10,  7, 11
   };

   const std::string& sVertexSource = " \
      attribute vec4 aVertexCoord;\n \
      attribute vec3 aVertexNormal;\n \
      uniform vec4 uVertexColor;\n \
      varying vec4 vFragColor;\n \
      void main(void) {\n \
         mat3 mNormalMatrix;\n \
         mNormalMatrix[0] = gl_ModelViewMatrix[0].xyz;\n \
         mNormalMatrix[1] = gl_ModelViewMatrix[1].xyz;\n \
         mNormalMatrix[2] = gl_ModelViewMatrix[2].xyz;\n \
         vec3 mNormal = normalize(mNormalMatrix * aVertexNormal);\n \
         vec3 mLightDir = vec3(0.0, 0.0, 1.0);\n \
         float mfDot = max(0.5, dot(mNormal, mLightDir));\n \
         vFragColor.rgb = uVertexColor.rgb * mfDot;\n \
         vFragColor.a = uVertexColor.a;\n \
         mat4 mMVPMatrix = gl_ProjectionMatrix * gl_ModelViewMatrix;\n \
         gl_Position = mMVPMatrix * aVertexCoord;\n \
      }\n";

   const std::string& sFragmentSource = " \
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
   maVertexNormalHandle = gl.GetAttribLocation(mProgram, "aVertexNormal");
   muVertexColorHandle = gl.GetUniformLocation(mProgram, "uVertexColor");

   if (gl.GenVertexArrays != NULL)
      gl.GenVertexArrays(1, &mVertexArrayBufferID);
   gl.GenBuffers(1, &mVertexCoordBufferID);
   gl.GenBuffers(1, &mVertexNormalBufferID);
   gl.GenBuffers(1, &mIndexBufferID);

   gl.BindBuffer(GL_ARRAY_BUFFER, mVertexCoordBufferID);
   gl.BufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), &VertexData[0], GL_STATIC_DRAW);
   gl.BindBuffer(GL_ARRAY_BUFFER, mVertexNormalBufferID);
   gl.BufferData(GL_ARRAY_BUFFER, 36 * sizeof(GLfloat), &NormData[0], GL_STATIC_DRAW);
   gl.BindBuffer(GL_ARRAY_BUFFER, 0);
   gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
   gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), &Indices[0], GL_STATIC_DRAW);
   gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DrawRightAngleWandCoreFunctor::draw(vrj::UserPtr)
{
   ExtensionLoaderGL& gl = DrawManager::instance()->getGL();

   gl.UseProgram(mProgram);
   if (mVertexArrayBufferID != 0)
      gl.BindVertexArray(mVertexArrayBufferID);
   gl.Uniform4f(muVertexColorHandle, 0.831f, 0.192f, 0.549f, 1.0f);

   gl.BindBuffer(GL_ARRAY_BUFFER, mVertexCoordBufferID);
   gl.VertexAttribPointer(maVertexCoordHandle,
      3, GL_FLOAT, false, 3 * sizeof(GLfloat), 0);
   gl.EnableVertexAttribArray(maVertexCoordHandle);

   gl.BindBuffer(GL_ARRAY_BUFFER, mVertexNormalBufferID);
   gl.VertexAttribPointer(maVertexNormalHandle,
      3, GL_FLOAT, false, 3 * sizeof(GLfloat), 0);
   gl.EnableVertexAttribArray(maVertexNormalHandle);

   if (gl.isCoreProfile())
   {
      gl.UniformMatrix4fv(muMVMatrixHandle, 1, false, gl.getModelViewMatrix().mData);
      gl.UniformMatrix4fv(muPMatrixHandle, 1, false, gl.getProjectionMatrix().mData);
   }

   gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
   gl.DrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_SHORT, 0);

   gl.DisableVertexAttribArray(maVertexCoordHandle);
   gl.DisableVertexAttribArray(maVertexNormalHandle);
   gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   gl.BindBuffer(GL_ARRAY_BUFFER, 0);
   if (mVertexArrayBufferID != 0)
      gl.BindVertexArray(0);
   gl.UseProgram(0);
}

} // End of opengl namespace

} // End of vrj namespace
