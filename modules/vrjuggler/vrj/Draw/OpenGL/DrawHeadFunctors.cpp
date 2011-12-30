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

#include <gmtl/Math.h>
#include <gmtl/Vec.h>

#include <vrj/Kernel/User.h>
#include <vrj/Draw/OpenGL/DrawHeadFunctors.h>

#define NUM_TRIANGLES_ON_SPHERE(nsteps)   (3*(2*(nsteps))+6*((nsteps)-1)*(nsteps))


namespace vrj
{

namespace opengl
{

DrawEllipsoidHeadFunctor::DrawEllipsoidHeadFunctor()
   : mQuadObj(NULL)
{
}

DrawEllipsoidHeadFunctor::~DrawEllipsoidHeadFunctor()
{
   if ( NULL != mQuadObj )
   {
      gluDeleteQuadric(mQuadObj);
   }
}

void DrawEllipsoidHeadFunctor::contextInit()
{
   if ( NULL == mQuadObj )
   {
      mQuadObj = gluNewQuadric();
   }
}

void DrawEllipsoidHeadFunctor::draw(vrj::UserPtr user)
{
   // All units are in meters.
   const float head_height(0.254f);      // 10 inches
   const float head_width_scale(0.7f);
   const float head_depth_scale(0.8f);
   //const float eye_vertical(0.067f);
   //const float eye_horizontal(0.21f);
   const float interocular(user->getInterocularDistance());
   const float eye_radius(0.0254f * 0.5f);      // 0.5 in

   glPushMatrix();
      // Head pos is the center of the eyes, so we need to move the head offset
      // a bit to get it positioned correctly.  We will do this by measuring
      // the distance from center of head to forehead and then translate by
      // the negative of that.
      gmtl::Vec3f forehead_offset(0.0f, head_height*0.17f,
                                  -(head_depth_scale*head_height)*0.45f);
      glTranslatef(-forehead_offset[0], -forehead_offset[1],
                   -forehead_offset[2]);

      // Scale to get an ellipsoid head.
      glScalef(head_width_scale, 1.0f, head_depth_scale);
      //glEnable(GL_BLEND);
      //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(0.5f, 0.75f, 0.90f, 0.67f);
      drawSphere(head_height/2.0f, 10, 10);             // Head
      //glDisable(GL_BLEND);
   glPopMatrix();

   // --- Draw the eyes --- //
   glPushMatrix();
      glColor3f(0.8f, 0.4f, 0.2f);
      //glTranslatef(0.0f, eye_vertical, -eye_horizontal);
      glPushMatrix();                     // Right eye
         glTranslatef((interocular/2.0f), 0.0f, 0.0f);
         drawSphere(eye_radius, 5, 5);
      glPopMatrix();
      glPushMatrix();                     // Left eye
         glTranslatef(-(interocular/2.0f), 0.0f, 0.0f);
         drawSphere(eye_radius, 5, 5);
      glPopMatrix();
   glPopMatrix();
}

void DrawEllipsoidHeadFunctor::drawSphere(const float radius,
                                          const int slices, const int stacks)
{
  gluQuadricDrawStyle(mQuadObj, (GLenum) GLU_FILL);
  gluQuadricNormals(mQuadObj, (GLenum) GLU_SMOOTH);
  gluSphere(mQuadObj, radius, slices, stacks);
}

DrawEllipsoidHeadCoreFunctor::DrawEllipsoidHeadCoreFunctor()
   : mProgram(0)
   , muMVMatrixHandle(0)
   , muPMatrixHandle(0)
   , maVertexCoordHandle(0)
   , muVertexColorHandle(0)
   , mVertexArrayBufferID(0)
   , mCoarseVertexCoordBufferID(0)
   , mCoarseIndexBufferID(0)
   , mFineVertexCoordBufferID(0)
   , mFineIndexBufferID(0)
{
   mFineSphereVertices = this->buildSphere(0.254f * 0.5f, 16);   // 10 inch 'head'
   mCoarseSphereVertices = this->buildSphere(0.0254f * 0.5f, 8); // 1 inch 'eyes'
}

DrawEllipsoidHeadCoreFunctor::~DrawEllipsoidHeadCoreFunctor()
{
   if ( NULL != mCoarseSphereVertices )
   {
      delete[] mCoarseSphereVertices;
   }

   if ( NULL != mFineSphereVertices )
   {
      delete[] mFineSphereVertices;
   }
}

void DrawEllipsoidHeadCoreFunctor::contextInit()
{
   const std::string& sVertexSource = " \
      attribute vec4 aVertexCoord;\n \
      uniform vec4 uVertexColor;\n \
      varying vec4 vFragColor;\n \
      void main(void) {\n \
         mat3 mNormalMatrix;\n \
         mNormalMatrix[0] = gl_ModelViewMatrix[0].xyz;\n \
         mNormalMatrix[1] = gl_ModelViewMatrix[1].xyz;\n \
         mNormalMatrix[2] = gl_ModelViewMatrix[2].xyz;\n \
         vec3 mNormal = normalize(transpose(inverse(mNormalMatrix)) * aVertexCoord.xyz);\n \
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
   muVertexColorHandle = gl.GetUniformLocation(mProgram, "uVertexColor");

   if (gl.GenVertexArrays != NULL)
      gl.GenVertexArrays(1, &mVertexArrayBufferID);
   gl.GenBuffers(1, &mCoarseVertexCoordBufferID);
   gl.GenBuffers(1, &mCoarseIndexBufferID);
   gl.GenBuffers(1, &mFineVertexCoordBufferID);
   gl.GenBuffers(1, &mFineIndexBufferID);

   gl.BindBuffer(GL_ARRAY_BUFFER, mFineVertexCoordBufferID);
   gl.BufferData(GL_ARRAY_BUFFER, 3 * NUM_TRIANGLES_ON_SPHERE(16) * sizeof(GLfloat),
      mFineSphereVertices, GL_STATIC_DRAW);
   gl.BindBuffer(GL_ARRAY_BUFFER, mCoarseVertexCoordBufferID);
   gl.BufferData(GL_ARRAY_BUFFER, 3 * NUM_TRIANGLES_ON_SPHERE(8) * sizeof(GLfloat),
      mCoarseSphereVertices, GL_STATIC_DRAW);
   gl.BindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawEllipsoidHeadCoreFunctor::draw(vrj::UserPtr user)
{
   // All units are in meters.
   const float head_height(0.254f);
   const float head_width_scale(0.7f);
   const float head_depth_scale(0.8f);
   const float interocular(user->getInterocularDistance());

   ExtensionLoaderGL& gl = DrawManager::instance()->getGL();
   gl.UseProgram(mProgram);
   if (mVertexArrayBufferID != 0)
      gl.BindVertexArray(mVertexArrayBufferID);
   gl.Uniform4f(muVertexColorHandle, 0.5f, 0.75f, 0.9f, 0.67f);

   gl.BindBuffer(GL_ARRAY_BUFFER, mFineVertexCoordBufferID);
   gl.VertexAttribPointer(maVertexCoordHandle, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), 0);
   gl.EnableVertexAttribArray(maVertexCoordHandle);

   gl.PushMatrix();
      // Head pos is the center of the eyes, so we need to move the head offset
      // a bit to get it positioned correctly.  We will do this by measuring
      // the distance from center of head to forehead and then translate by
      // the negative of that.
      gmtl::Vec3f forehead_offset(0.0f, head_height*0.17f,
                                  -(head_depth_scale*head_height)*0.45f);
      gl.Translate(-forehead_offset[0], -forehead_offset[1],
                   -forehead_offset[2]);

      // Scale to get an ellipsoid head.
      gl.Scale(head_width_scale, 1.0f, head_depth_scale);

      if (gl.isCoreProfile())
      {
         gl.UniformMatrix4fv(muMVMatrixHandle, 1, false, gl.getModelViewMatrix().mData);
         gl.UniformMatrix4fv(muPMatrixHandle, 1, false, gl.getProjectionMatrix().mData);
      }

      gl.DrawArrays(GL_TRIANGLES, 0, NUM_TRIANGLES_ON_SPHERE(16));
   gl.PopMatrix();

   gl.BindBuffer(GL_ARRAY_BUFFER, mCoarseVertexCoordBufferID);
   gl.VertexAttribPointer(maVertexCoordHandle, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), 0);
   gl.EnableVertexAttribArray(maVertexCoordHandle);

   // --- Draw the eyes --- //
   gl.Uniform4f(muVertexColorHandle, 0.8f, 0.4f, 0.2f, 1.0f);
   gl.PushMatrix();                     // Right eye
      gl.Translate((interocular/2.0f), 0.0f, 0.0f);
      if (gl.isCoreProfile())
      {
         gl.UniformMatrix4fv(muMVMatrixHandle, 1, false, gl.getModelViewMatrix().mData);
         gl.UniformMatrix4fv(muPMatrixHandle, 1, false, gl.getProjectionMatrix().mData);
      }
      gl.DrawArrays(GL_TRIANGLES, 0, NUM_TRIANGLES_ON_SPHERE(8));
   gl.PopMatrix();
   gl.PushMatrix();                     // Left eye
      gl.Translate(-(interocular/2.0f), 0.0f, 0.0f);
      if (gl.isCoreProfile())
      {
         gl.UniformMatrix4fv(muMVMatrixHandle, 1, false, gl.getModelViewMatrix().mData);
         gl.UniformMatrix4fv(muPMatrixHandle, 1, false, gl.getProjectionMatrix().mData);
      }
      gl.DrawArrays(GL_TRIANGLES, 0, NUM_TRIANGLES_ON_SPHERE(8));
   gl.PopMatrix();

   gl.DisableVertexAttribArray(maVertexCoordHandle);
   gl.BindBuffer(GL_ARRAY_BUFFER, 0);
   if (mVertexArrayBufferID != 0)
      gl.BindVertexArray(0);
   gl.UseProgram(0);
}

GLfloat* DrawEllipsoidHeadCoreFunctor::buildSphere(const float radius, const int nSteps)
{
   GLfloat* v = new GLfloat[3 * NUM_TRIANGLES_ON_SPHERE(nSteps)];
   double dTheta = gmtl::Math::TWO_PI / nSteps;
   double dPhi = gmtl::Math::PI / nSteps;

   int i = 0;
   double phi = dPhi;
   double theta = 0.0;

   do
   {
      v[i++] = 0.0f;  v[i++] = 0.0f;  v[i++] = radius;

      double x = radius * gmtl::Math::sin(phi) * gmtl::Math::cos(theta);
      double y = radius * gmtl::Math::sin(phi) * gmtl::Math::sin(theta);
      double z = radius * gmtl::Math::cos(phi);
      v[i++] = x;  v[i++] = y;  v[i++] = z;
      
      theta += dTheta;
      x = radius * gmtl::Math::sin(phi) * gmtl::Math::cos(theta);
      y = radius * gmtl::Math::sin(phi) * gmtl::Math::sin(theta);
      z = radius * gmtl::Math::cos(phi);
      v[i++] = x;  v[i++] = y;  v[i++] = z;
   } while (gmtl::Math::abs(theta - gmtl::Math::TWO_PI) > 1.0e-3);

   for (int p = 2; p < nSteps; ++p)
   {
      phi = p * dPhi;
      theta = 0.0;

      do
      {
         double x = radius * gmtl::Math::sin(phi - dPhi) * gmtl::Math::cos(theta);
         double y = radius * gmtl::Math::sin(phi - dPhi) * gmtl::Math::sin(theta);
         double z = radius * gmtl::Math::cos(phi - dPhi);
         v[i++] = x;  v[i++] = y;  v[i++] = z;

         x = radius * gmtl::Math::sin(phi) * gmtl::Math::cos(theta);
         y = radius * gmtl::Math::sin(phi) * gmtl::Math::sin(theta);
         z = radius * gmtl::Math::cos(phi);
         v[i++] = x;  v[i++] = y;  v[i++] = z;

         theta += dTheta;
         x = radius * gmtl::Math::sin(phi) * gmtl::Math::cos(theta);
         y = radius * gmtl::Math::sin(phi) * gmtl::Math::sin(theta);
         z = radius * gmtl::Math::cos(phi);
         v[i++] = x;  v[i++] = y;  v[i++] = z;

         v[i++] = v[i-9];  v[i++] = v[i-9];  v[i++] = v[i-9];
         v[i++] = v[i-6];  v[i++] = v[i-6];  v[i++] = v[i-6];

         x = radius * gmtl::Math::sin(phi - dPhi) * gmtl::Math::cos(theta);
         y = radius * gmtl::Math::sin(phi - dPhi) * gmtl::Math::sin(theta);
         z = radius * gmtl::Math::cos(phi - dPhi);
         v[i++] = x;  v[i++] = y;  v[i++] = z;
      } while (gmtl::Math::abs(theta - gmtl::Math::TWO_PI) > 1.0e-3);
   }

   theta = 0.0;
   do
   {
      v[i++] = 0.0f;  v[i++] = 0.0f;  v[i++] = -radius;

      double x = radius * gmtl::Math::sin(phi) * gmtl::Math::cos(theta);
      double y = radius * gmtl::Math::sin(phi) * gmtl::Math::sin(theta);
      double z = radius * gmtl::Math::cos(phi);
      v[i++] = x;  v[i++] = y;  v[i++] = z;
      
      theta += dTheta;
      x = radius * gmtl::Math::sin(phi) * gmtl::Math::cos(theta);
      y = radius * gmtl::Math::sin(phi) * gmtl::Math::sin(theta);
      z = radius * gmtl::Math::cos(phi);
      v[i++] = x;  v[i++] = y;  v[i++] = z;
   } while (gmtl::Math::abs(theta - gmtl::Math::TWO_PI) > 1.0e-3);

   return v;
}

}

}