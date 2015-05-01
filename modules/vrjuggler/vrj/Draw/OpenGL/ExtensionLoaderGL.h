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

#if ! defined(APIENTRY)
#  define APIENTRY
#endif

// "No GL" Defines
#define NOGL_MODELVIEW                      0x1700
#define NOGL_PROJECTION                     0x1701
#define NOGL_FRAGMENT_SHADER                0x8B30
#define NOGL_VERTEX_SHADER                  0x8B31
#define NOGL_FALSE                          0
#define NOGL_TRUE                           1
#define NOGL_COMPILE_STATUS                 0x8B81
#define NOGL_LINK_STATUS                    0x8B82
#define NOGL_INFO_LOG_LENGTH                0x8B84

typedef void (APIENTRY * PFNNOGLGENBUFFERSPROC) (int n, unsigned int *buffers);
typedef void (APIENTRY * PFNNOGLBINDBUFFERPROC) (unsigned int target, unsigned int buffer);
typedef void (APIENTRY * PFNNOGLBUFFERDATAPROC) (unsigned int target, ptrdiff_t size, const void *data, unsigned int usage);
typedef void (APIENTRY * PFNNOGLDELETEBUFFERSPROC) (int n, const unsigned int *buffers);

typedef void (APIENTRY * PFNNOGLBINDVERTEXARRAYPROC) (unsigned int array);
typedef void (APIENTRY * PFNNOGLDELETEVERTEXARRAYSPROC) (int n, const unsigned int *arrays);
typedef void (APIENTRY * PFNNOGLGENVERTEXARRAYSPROC) (int n, unsigned int *arrays);

typedef unsigned int (APIENTRY * PFNNOGLCREATESHADERPROC) (unsigned int type);
typedef void (APIENTRY * PFNNOGLCOMPILESHADERPROC) (unsigned int shader);
typedef void (APIENTRY * PFNNOGLSHADERSOURCEPROC) (unsigned int shader, int count, const char* *string, const int *length);
typedef void (APIENTRY * PFNNOGLGETSHADERIVPROC) (unsigned int shader, unsigned int pname, int *params);
typedef void (APIENTRY * PFNNOGLGETSHADERINFOLOGPROC) (unsigned int shader, int bufSize, int *length, char *infoLog);
typedef void (APIENTRY * PFNNOGLATTACHSHADERPROC) (unsigned int program, unsigned int shader);
typedef void (APIENTRY * PFNNOGLDELETESHADERPROC) (unsigned int shader);
typedef unsigned int (APIENTRY * PFNNOGLCREATEPROGRAMPROC) (void);
typedef void (APIENTRY * PFNNOGLLINKPROGRAMPROC) (unsigned int program);
typedef void (APIENTRY * PFNNOGLGETPROGRAMIVPROC) (unsigned int program, unsigned int pname, int *params);
typedef void (APIENTRY * PFNNOGLUSEPROGRAMPROC) (unsigned int program);
typedef void (APIENTRY * PFNNOGLDELETEPROGRAMPROC) (unsigned int program);
typedef int (APIENTRY * PFNNOGLGETUNIFORMLOCATIONPROC) (unsigned int program, const char *name);
typedef int (APIENTRY * PFNNOGLGETATTRIBLOCATIONPROC) (unsigned int program, const char *name);
typedef void (APIENTRY * PFNNOGLVERTEXATTRIBPOINTERPROC) (unsigned int index, int size, unsigned int type, unsigned char normalized, int stride, const void *pointer);
typedef void (APIENTRY * PFNNOGLENABLEVERTEXATTRIBARRAYPROC) (unsigned int index);
typedef void (APIENTRY * PFNNOGLDISABLEVERTEXATTRIBARRAYPROC) (unsigned int index);
typedef void (APIENTRY * PFNNOGLUNIFORM1FPROC) (int location, float v0);
typedef void (APIENTRY * PFNNOGLUNIFORM2FPROC) (int location, float v0, float v1);
typedef void (APIENTRY * PFNNOGLUNIFORM3FPROC) (int location, float v0, float v1, float v2);
typedef void (APIENTRY * PFNNOGLUNIFORM4FPROC) (int location, float v0, float v1, float v2, float v3);
typedef void (APIENTRY * PFNNOGLUNIFORM2FVPROC) (int location, int count, const float *value);
typedef void (APIENTRY * PFNNOGLUNIFORM3FVPROC) (int location, int count, const float *value);
typedef void (APIENTRY * PFNNOGLUNIFORM4FVPROC) (int location, int count, const float *value);
typedef void (APIENTRY * PFNNOGLUNIFORMMATRIX4FVPROC) (int location, int count, unsigned char transpose, const float *value);

namespace vrj
{

namespace opengl
{

/**
 * GL-specific base class for loading extensions.
 */
  class VJ_OGL_API ExtensionLoaderGL : public vrj::opengl::ExtensionLoader
{
private:
   ExtensionLoaderGL();

public:
   friend class Window;
   friend class ExtensionLoaderGLCore;

   void Clear(unsigned int mask);

   void ClearColor(float r, float g, float b, float a);

   void ClearStencil(int s);

   void ClearDepth(double depth);

   void StencilMask (unsigned int mask);

   void ColorMask(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

   void DepthMask(unsigned char flag);

   void DrawArrays(unsigned int mode, int first, int count);

   void DrawBuffer(unsigned int mode);

   void DrawElements(unsigned int mode, int count, unsigned int type, const void* indices);

   void Finish(void);

   void Flush(void);

   void Viewport(int x, int y, int width, int height);

   virtual void Enable(unsigned int cap);

   virtual void Disable(unsigned int cap);

   virtual void PushAttrib(unsigned int mask);

   virtual void PopAttrib();

   virtual void MatrixMode(unsigned int mode);

   virtual void PushMatrix();

   virtual void PopMatrix();

   virtual void LoadIdentity();

   virtual void LoadMatrix(const gmtl::Matrix44f& mat);

   virtual void MultMatrix(const gmtl::Matrix44f& mat);

   virtual void Translate(float tx, float ty, float tz);

   virtual void Scale(float sx, float sy, float sz);

   virtual void Ortho(float x0, float x1, float y0, float y1, float z0, float z1);

   virtual void Frustum(float x0, float x1, float y0, float y1, float z0, float z1);


   PFNNOGLGENBUFFERSPROC GenBuffers;
   PFNNOGLBINDBUFFERPROC BindBuffer;
   PFNNOGLBUFFERDATAPROC BufferData;
   PFNNOGLDELETEBUFFERSPROC DeleteBuffers;

   PFNNOGLGENVERTEXARRAYSPROC GenVertexArrays;
   PFNNOGLBINDVERTEXARRAYPROC BindVertexArray;
   PFNNOGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;

   PFNNOGLCREATESHADERPROC CreateShader;
   PFNNOGLCOMPILESHADERPROC CompileShader;
   PFNNOGLSHADERSOURCEPROC ShaderSource;
   PFNNOGLGETSHADERIVPROC GetShaderiv;
   PFNNOGLGETSHADERINFOLOGPROC GetShaderInfoLog;
   PFNNOGLATTACHSHADERPROC AttachShader;
   PFNNOGLDELETESHADERPROC DeleteShader;
   
   PFNNOGLCREATEPROGRAMPROC CreateProgram;
   PFNNOGLLINKPROGRAMPROC LinkProgram;
   PFNNOGLGETPROGRAMIVPROC GetProgramiv;
   PFNNOGLUSEPROGRAMPROC UseProgram;
   PFNNOGLDELETEPROGRAMPROC DeleteProgram;

   PFNNOGLGETUNIFORMLOCATIONPROC GetUniformLocation;
   PFNNOGLGETATTRIBLOCATIONPROC GetAttribLocation;
   PFNNOGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
   PFNNOGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
   PFNNOGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;
   PFNNOGLUNIFORM1FPROC Uniform1f;
   PFNNOGLUNIFORM2FPROC Uniform2f;
   PFNNOGLUNIFORM3FPROC Uniform3f;
   PFNNOGLUNIFORM4FPROC Uniform4f;
   PFNNOGLUNIFORM2FVPROC Uniform2fv;
   PFNNOGLUNIFORM3FVPROC Uniform3fv;
   PFNNOGLUNIFORM4FVPROC Uniform4fv;
   PFNNOGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;

   unsigned int createProgram(const std::string& vertexSource,
      const std::string& fragmentSource)
   {
      int vertexShader = loadShader(NOGL_VERTEX_SHADER, vertexSource);
      if (vertexShader == 0)
      {
         return 0;
      }
     
      int fragmentShader = loadShader(NOGL_FRAGMENT_SHADER, fragmentSource);
      if (fragmentShader == 0)
      {
         return 0;
      }

      unsigned int program = CreateProgram();
      if (program != 0)
      {
         AttachShader(program, vertexShader);
         AttachShader(program, fragmentShader);
         LinkProgram(program);

         int status = NOGL_FALSE;
         GetProgramiv(program, NOGL_LINK_STATUS, &status);
         if (status != NOGL_TRUE)
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
   unsigned int loadShader(unsigned int shaderType, const std::string& source)
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
         char* ptr[1];

         ptr[0] = (char*)source.c_str();
         ShaderSource(shader, 1, (const char**)ptr, NULL);
         CompileShader(shader);

         int status = NOGL_FALSE;
         GetShaderiv(shader, NOGL_COMPILE_STATUS, &status);
         if (status != NOGL_TRUE)
         {
            int logLength = 0;
            GetShaderiv(shader, NOGL_INFO_LOG_LENGTH, &logLength);

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
class VJ_OGL_API ExtensionLoaderGLCore : public vrj::opengl::ExtensionLoaderGL
{
private:
   ExtensionLoaderGLCore();

   std::stack<gmtl::Matrix44f> mModelViewMatrixStack;
   std::stack<gmtl::Matrix44f> mProjectionMatrixStack;
   std::stack<gmtl::Matrix44f>* mCurrentMatrixStack;

public:
   friend class Window;

   virtual void PushAttrib(unsigned int)
   {
      ;
   }

   virtual void PopAttrib()
   {
      ;
   }

   virtual void MatrixMode(unsigned int mode)
   {
      switch (mode)
      {
      case NOGL_PROJECTION:
         mCurrentMatrixStack = &mProjectionMatrixStack;
         break;

      case NOGL_MODELVIEW:
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
