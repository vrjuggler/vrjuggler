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

#ifndef _VRJ_OPENGL_EXTENSION_LOADER_GL_H_
#define _VRJ_OPENGL_EXTENSION_LOADER_GL_H_

#include <vrj/Draw/OpenGL/Config.h>

#include <stack>
#include <boost/shared_ptr.hpp>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>

#include <vrj/Draw/OpenGL/ExtensionLoader.h>

// GL Defines
#ifndef GL_MODELVIEW
#define GL_MODELVIEW                      0x1700
#endif
#ifndef GL_PROJECTION
#define GL_PROJECTION                     0x1701
#endif

namespace vrj
{

namespace opengl
{

/**
 * GL-specific base class for loading extensions.
 */
class ExtensionLoaderGL : public vrj::opengl::ExtensionLoader
{
private:
   ExtensionLoaderGL();

public:
   friend class Window;
   friend class ExtensionLoaderGLCore;

   void Clear(GLbitfield mask)
   {
      glClear(mask);
   }

   void ClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
   {
      glClearColor(r, g, b, a);
   }

   void ClearStencil(GLint s)
   {
      glClearStencil(s);
   }

   void ClearDepth(GLclampd depth)
   {
      glClearDepth(depth);
   }

   void StencilMask (GLuint mask)
   {
      glStencilMask(mask);
   }

   void ColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a)
   {
      glColorMask(r, g, b, a);
   }

   void DepthMask(GLboolean flag)
   {
      glDepthMask(flag);
   }

   void DrawArrays(GLenum mode, GLint first, GLsizei count)
   {
      glDrawArrays(mode, first, count);
   }

   void DrawBuffer(GLenum mode)
   {
      glDrawBuffer(mode);
   }

   void DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
   {
      glDrawElements(mode, count, type, indices);
   }

   void Finish(void)
   {
      glFinish();
   }

   void Flush(void)
   {
      glFlush();
   }

   void Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
   {
      glViewport(x, y, width, height);
   }

   virtual void Enable(GLenum cap)
   {
      glEnable(cap);
   }

   virtual void Disable(GLenum cap)
   {
      glDisable(cap);
   }

   virtual void PushAttrib(GLbitfield mask)
   {
      glPushAttrib(mask);
   }

   virtual void PopAttrib()
   {
      glPopAttrib();
   }

   virtual void MatrixMode(GLenum mode)
   {
      glMatrixMode(mode);
   }

   virtual void PushMatrix()
   {
      glPushMatrix();
   }

   virtual void PopMatrix()
   {
      glPopMatrix();
   }

   virtual void LoadIdentity()
   {
      glLoadIdentity();
   }

   virtual void LoadMatrix(const gmtl::Matrix44f& mat)
   {
      glLoadMatrixf(mat.mData);
   }

   virtual void MultMatrix(const gmtl::Matrix44f& mat)
   {
      glMultMatrixf(mat.mData);
   }

   virtual void Translate(float tx, float ty, float tz)
   {
      glTranslatef(tx, ty, tz);
   }

   virtual void Scale(float sx, float sy, float sz)
   {
      glScalef(sx, sy, sz);
   }

   virtual void Ortho(float x0, float x1, float y0, float y1, float z0, float z1)
   {
      glOrtho(x0, x1, y0, y1, z0, z1);
   }

   virtual void Frustum(float x0, float x1, float y0, float y1, float z0, float z1)
   {
      glFrustum(x0, x1, y0, y1, z0, z1);
   }


   PFNGLGENBUFFERSPROC GenBuffers;
   PFNGLBINDBUFFERPROC BindBuffer;
   PFNGLBUFFERDATAPROC BufferData;
   PFNGLDELETEBUFFERSPROC DeleteBuffers;

   PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
   PFNGLBINDVERTEXARRAYPROC BindVertexArray;
   PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;

   PFNGLCREATESHADERPROC CreateShader;
   PFNGLCOMPILESHADERPROC CompileShader;
   PFNGLSHADERSOURCEPROC ShaderSource;
   PFNGLGETSHADERIVPROC GetShaderiv;
   PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
   PFNGLATTACHSHADERPROC AttachShader;
   PFNGLDELETESHADERPROC DeleteShader;
   
   PFNGLCREATEPROGRAMPROC CreateProgram;
   PFNGLLINKPROGRAMPROC LinkProgram;
   PFNGLGETPROGRAMIVPROC GetProgramiv;
   PFNGLUSEPROGRAMPROC UseProgram;
   PFNGLDELETEPROGRAMPROC DeleteProgram;

   PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
   PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;
   PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
   PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
   PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
   PFNGLUNIFORM1FPROC Uniform1f;
   PFNGLUNIFORM2FPROC Uniform2f;
   PFNGLUNIFORM3FPROC Uniform3f;
   PFNGLUNIFORM4FPROC Uniform4f;
   PFNGLUNIFORM2FVPROC Uniform2fv;
   PFNGLUNIFORM3FVPROC Uniform3fv;
   PFNGLUNIFORM4FVPROC Uniform4fv;
   PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;

   GLuint createProgram(const std::string& vertexSource,
      const std::string& fragmentSource)
   {
      int vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
      if (vertexShader == 0)
      {
         return 0;
      }
     
      int fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
      if (fragmentShader == 0)
      {
         return 0;
      }

      GLuint program = CreateProgram();
      if (program != 0)
      {
         AttachShader(program, vertexShader);
         AttachShader(program, fragmentShader);
         LinkProgram(program);

         GLint status = GL_FALSE;
         GetProgramiv(program, GL_LINK_STATUS, &status);
         if (status != GL_TRUE)
         {
             DeleteProgram(program);
             program = 0;
         }
      }

      return program;
   }

   virtual bool isCoreProfile() const
   {
      return false;
   }

   const virtual gmtl::Matrix44f& getModelViewMatrix() const
   {
      return gmtl::MAT_IDENTITY44F;
   }

   const virtual gmtl::Matrix44f& getProjectionMatrix() const
   {
      return gmtl::MAT_IDENTITY44F;
   }

protected:
   virtual void registerExtensions();

   bool mExtensionsRegistered;

private:
   GLuint loadShader(GLuint shaderType, const std::string& source)
   {
      if (CreateShader == NULL)
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "ERROR")
            << ": [vrj::opengl::DrawManager::loadShader()] "
            << "User defined shaders not supported\n" << vprDEBUG_FLUSH;
         return 0;
      }

      int shader = CreateShader(shaderType);
      if (shader != 0)
      {
         GLchar* ptr[1];

         ptr[0] = (GLchar*)source.c_str();
         ShaderSource(shader, 1, (const GLchar**)ptr, NULL);
         CompileShader(shader);

         GLint status = GL_FALSE;
         GetShaderiv(shader, GL_COMPILE_STATUS, &status);
         if (status != GL_TRUE)
         {
            GLint logLength = 0;
            GetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

            std::string infoLog(logLength, ' ');
            GetShaderInfoLog(shader, logLength, &logLength, &infoLog[0]);
            
            vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
               << clrOutNORM(clrRED, "ERROR")
               << ": [vrj::opengl::DrawManager::loadShader()] Error compiling "
               << "shader:\n" << infoLog << vprDEBUG_FLUSH;
            vprASSERT(false);

            DeleteShader(shader);
            shader = 0;
         }
      }

      return shader;
   }
};

/**
 * GL-specific core profile class.
 */
class ExtensionLoaderGLCore : public vrj::opengl::ExtensionLoaderGL
{
private:
   ExtensionLoaderGLCore();

   std::stack<gmtl::Matrix44f> mModelViewMatrixStack;
   std::stack<gmtl::Matrix44f> mProjectionMatrixStack;
   std::stack<gmtl::Matrix44f>* mCurrentMatrixStack;

public:
   friend class Window;

   virtual void PushAttrib(GLbitfield mask)
   {
      ;
   }

   virtual void PopAttrib()
   {
      ;
   }

   virtual void MatrixMode(GLenum mode)
   {
      switch (mode)
      {
      case GL_PROJECTION:
         mCurrentMatrixStack = &mProjectionMatrixStack;
         break;

      case GL_MODELVIEW:
         mCurrentMatrixStack = &mModelViewMatrixStack;
         break;
      }
   }

   virtual void PushMatrix()
   {
      mCurrentMatrixStack->push(mCurrentMatrixStack->top());
   }

   virtual void PopMatrix()
   {
      if (mCurrentMatrixStack->size() > 0)
         mCurrentMatrixStack->pop();
   }

   virtual void LoadIdentity()
   {
      this->LoadMatrix(gmtl::MAT_IDENTITY44F);
   }

   virtual void LoadMatrix(const gmtl::Matrix44f& mat)
   {
      mCurrentMatrixStack->pop();
      mCurrentMatrixStack->push(mat);
   }

   virtual void MultMatrix(const gmtl::Matrix44f& mat)
   {
      const gmtl::Matrix44f& top = mCurrentMatrixStack->top();
      mCurrentMatrixStack->pop();
      mCurrentMatrixStack->push(top * mat);
   }

   virtual void Translate(float tx, float ty, float tz)
   {
      gmtl::Matrix44f mat;
      mat.set(1.0f,  0.0f,  0.0f,    tx,
              0.0f,  1.0f,  0.0f,    ty,
              0.0f,  0.0f,  1.0f,    tz,
              0.0f,  0.0f,  0.0f,  1.0f);

      this->MultMatrix(mat);
   }

   virtual void Scale(float sx, float sy, float sz)
   {
      gmtl::Matrix44f mat;
      mat.set(sx,  0.0f,  0.0f,  0.0f,
            0.0f,    sy,  0.0f,  0.0f,
            0.0f,  0.0f,    sz,  0.0f,
            0.0f,  0.0f,  0.0f,  1.0f);

      this->MultMatrix(mat);
   }

   virtual void Ortho(float x0, float x1, float y0, float y1, float z0, float z1)
   {
      float dx = x1 - x0;
      float dy = y1 - y0;
      float dz = z1 - z0;
      float a = -(x0 + x1) / dx;
      float b = -(y0 + y1) / dy;
      float c = -(z0 + z1) / dz;

      gmtl::Matrix44f mat;
      mat.set(2.0f / dx,      0.0f,       0.0f,           a,
                   0.0f, 2.0f / dy,       0.0f,           b,
                   0.0f,      0.0f, -2.0f / dz,           c,
                   0.0f,      0.0f,       0.0f,        1.0f);

      this->MultMatrix(mat);
   }

   virtual void Frustum(float x0, float x1, float y0, float y1, float z0, float z1)
   {
      float dx = x1 - x0;
      float dy = y1 - y0;
      float dz = z1 - z0;
      float a = (x0 + x1) / dx;
      float b = (y0 + y1) / dy;
      float c = -(z0 + z1) / dz;
      float d = -2.0f * z0 * z1 / dz;

      gmtl::Matrix44f mat;
      mat.set(2.0f * z0 / dx,           0.0f,            a,           0.0f,
                        0.0f, 2.0f * z0 / dy,            b,           0.0f,
                        0.0f,           0.0f,            c,              d,
                        0.0f,           0.0f,        -1.0f,           0.0f);

      this->MultMatrix(mat);
   }

   virtual bool isCoreProfile() const
   {
      return true;
   }

   const virtual gmtl::Matrix44f& getModelViewMatrix() const
   {
      return mModelViewMatrixStack.top();
   }

   const gmtl::Matrix44f& getProjectionMatrix() const
   {
      return mProjectionMatrixStack.top();
   }
};

}  // namespace opengl

}  // namespace vrj


#endif
