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

#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#include <vrj/Draw/OpenGL/ExtensionLoaderGL.h>

#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

namespace vrj
{

namespace opengl
{

ExtensionLoaderGL::ExtensionLoaderGL()
   : GenBuffers(NULL)
   , BindBuffer(NULL)
   , BufferData(NULL)
   , DeleteBuffers(NULL)
   , GenVertexArrays(NULL)
   , BindVertexArray(NULL)
   , DeleteVertexArrays(NULL)
   , CreateShader(NULL)
   , CompileShader(NULL)
   , ShaderSource(NULL)
   , GetShaderiv(NULL)
   , GetShaderInfoLog(NULL)
   , AttachShader(NULL)
   , DeleteShader(NULL)
   , CreateProgram(NULL)
   , LinkProgram(NULL)
   , GetProgramiv(NULL)
   , UseProgram(NULL)
   , DeleteProgram(NULL)
   , GetUniformLocation(NULL)
   , GetAttribLocation(NULL)
   , VertexAttribPointer(NULL)
   , EnableVertexAttribArray(NULL)
   , DisableVertexAttribArray(NULL)
   , Uniform1f(NULL)
   , Uniform2f(NULL)
   , Uniform3f(NULL)
   , Uniform4f(NULL)
   , Uniform2fv(NULL)
   , Uniform3fv(NULL)
   , Uniform4fv(NULL)
   , UniformMatrix4fv(NULL)
   , mExtensionsRegistered(false)
{ ; } // Do nothing

void ExtensionLoaderGL::registerExtensions()
{
   if ( !mExtensionsRegistered )
   {
      GenBuffers = (PFNNOGLGENBUFFERSPROC) getFunctionByName("glGenBuffers");
      BindBuffer = (PFNNOGLBINDBUFFERPROC) getFunctionByName("glBindBuffer");
      BufferData = (PFNNOGLBUFFERDATAPROC) getFunctionByName("glBufferData");
      DeleteBuffers = (PFNNOGLDELETEBUFFERSPROC) getFunctionByName("glDeleteBuffers");

      GenVertexArrays = (PFNNOGLGENVERTEXARRAYSPROC) getFunctionByName("glGenVertexArrays");
      BindVertexArray = (PFNNOGLBINDVERTEXARRAYPROC) getFunctionByName("glBindVertexArray");
      DeleteVertexArrays = (PFNNOGLDELETEVERTEXARRAYSPROC) getFunctionByName("glDeleteVertexArrays");

      CreateShader = (PFNNOGLCREATESHADERPROC) getFunctionByName("glCreateShader");
      CompileShader = (PFNNOGLCOMPILESHADERPROC) getFunctionByName("glCompileShader");
      ShaderSource = (PFNNOGLSHADERSOURCEPROC) getFunctionByName("glShaderSource");
      GetShaderiv = (PFNNOGLGETSHADERIVPROC) getFunctionByName("glGetShaderiv");
      GetShaderInfoLog = (PFNNOGLGETSHADERINFOLOGPROC) getFunctionByName("glGetShaderInfoLog");
      AttachShader = (PFNNOGLATTACHSHADERPROC) getFunctionByName("glAttachShader");
      DeleteShader = (PFNNOGLDELETESHADERPROC) getFunctionByName("glDeleteShader");
      CreateProgram = (PFNNOGLCREATEPROGRAMPROC) getFunctionByName("glCreateProgram");
      LinkProgram = (PFNNOGLLINKPROGRAMPROC) getFunctionByName("glLinkProgram");
      GetProgramiv = (PFNNOGLGETPROGRAMIVPROC) getFunctionByName("glGetProgramiv");
      UseProgram = (PFNNOGLUSEPROGRAMPROC) getFunctionByName("glUseProgram");
      DeleteProgram = (PFNNOGLDELETEPROGRAMPROC) getFunctionByName("glDeleteProgram");
      GetUniformLocation = (PFNNOGLGETUNIFORMLOCATIONPROC) getFunctionByName("glGetUniformLocation");
      GetAttribLocation = (PFNNOGLGETATTRIBLOCATIONPROC) getFunctionByName("glGetAttribLocation");
      VertexAttribPointer = (PFNNOGLVERTEXATTRIBPOINTERPROC) getFunctionByName("glVertexAttribPointer");
      EnableVertexAttribArray = (PFNNOGLENABLEVERTEXATTRIBARRAYPROC) getFunctionByName("glEnableVertexAttribArray");
      DisableVertexAttribArray = (PFNNOGLDISABLEVERTEXATTRIBARRAYPROC) getFunctionByName("glDisableVertexAttribArray");
      Uniform1f = (PFNNOGLUNIFORM1FPROC) getFunctionByName("glUniform1f");
      Uniform2f = (PFNNOGLUNIFORM2FPROC) getFunctionByName("glUniform2f");
      Uniform3f = (PFNNOGLUNIFORM3FPROC) getFunctionByName("glUniform3f");
      Uniform4f = (PFNNOGLUNIFORM4FPROC) getFunctionByName("glUniform4f");
      Uniform2fv = (PFNNOGLUNIFORM2FVPROC) getFunctionByName("glUniform2fv");
      Uniform3fv = (PFNNOGLUNIFORM3FVPROC) getFunctionByName("glUniform3fv");
      Uniform4fv = (PFNNOGLUNIFORM4FVPROC) getFunctionByName("glUniform4fv");
      UniformMatrix4fv = (PFNNOGLUNIFORMMATRIX4FVPROC) getFunctionByName("glUniformMatrix4fv");

      mExtensionsRegistered = true;
   }
}

void ExtensionLoaderGL::Clear(GLbitfield mask)
{
   glClear(mask);
}

void ExtensionLoaderGL::ClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
   glClearColor(r, g, b, a);
}

void ExtensionLoaderGL::ClearStencil(GLint s)
{
   glClearStencil(s);
}

void ExtensionLoaderGL::ClearDepth(GLclampd depth)
{
   glClearDepth(depth);
}

void ExtensionLoaderGL::StencilMask (GLuint mask)
{
   glStencilMask(mask);
}

void ExtensionLoaderGL::ColorMask(GLboolean r, GLboolean g, GLboolean b, GLboolean a)
{
   glColorMask(r, g, b, a);
}

void ExtensionLoaderGL::DepthMask(GLboolean flag)
{
   glDepthMask(flag);
}

void ExtensionLoaderGL::DrawArrays(GLenum mode, GLint first, GLsizei count)
{
   glDrawArrays(mode, first, count);
}

void ExtensionLoaderGL::DrawBuffer(GLenum mode)
{
   glDrawBuffer(mode);
}

void ExtensionLoaderGL::DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices)
{
   glDrawElements(mode, count, type, indices);
}

void ExtensionLoaderGL::Finish(void)
{
   glFinish();
}

void ExtensionLoaderGL::Flush(void)
{
   glFlush();
}

void ExtensionLoaderGL::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
   glViewport(x, y, width, height);
}

void ExtensionLoaderGL::Enable(GLenum cap)
{
   glEnable(cap);
}

void ExtensionLoaderGL::Disable(GLenum cap)
{
   glDisable(cap);
}

void ExtensionLoaderGL::PushAttrib(GLbitfield mask)
{
   glPushAttrib(mask);
}

void ExtensionLoaderGL::PopAttrib()
{
   glPopAttrib();
}

void ExtensionLoaderGL::MatrixMode(GLenum mode)
{
   glMatrixMode(mode);
}

void ExtensionLoaderGL::PushMatrix()
{
   glPushMatrix();
}

void ExtensionLoaderGL::PopMatrix()
{
   glPopMatrix();
}

void ExtensionLoaderGL::LoadIdentity()
{
   glLoadIdentity();
}

void ExtensionLoaderGL::LoadMatrix(const gmtl::Matrix44f& mat)
{
   glLoadMatrixf(mat.mData);
}

void ExtensionLoaderGL::MultMatrix(const gmtl::Matrix44f& mat)
{
   glMultMatrixf(mat.mData);
}

void ExtensionLoaderGL::Translate(float tx, float ty, float tz)
{
   glTranslatef(tx, ty, tz);
}

void ExtensionLoaderGL::Scale(float sx, float sy, float sz)
{
   glScalef(sx, sy, sz);
}

void ExtensionLoaderGL::Ortho(float x0, float x1, float y0, float y1, float z0, float z1)
{
   glOrtho(x0, x1, y0, y1, z0, z1);
}

void ExtensionLoaderGL::Frustum(float x0, float x1, float y0, float y1, float z0, float z1)
{
   glFrustum(x0, x1, y0, y1, z0, z1);
}

}

}
