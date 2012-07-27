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
      GenBuffers = (PFNGLGENBUFFERSPROC) getFunctionByName("glGenBuffers");
      BindBuffer = (PFNGLBINDBUFFERPROC) getFunctionByName("glBindBuffer");
      BufferData = (PFNGLBUFFERDATAPROC) getFunctionByName("glBufferData");
      DeleteBuffers = (PFNGLDELETEBUFFERSPROC) getFunctionByName("glDeleteBuffers");

      GenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) getFunctionByName("glGenVertexArrays");
      BindVertexArray = (PFNGLBINDVERTEXARRAYPROC) getFunctionByName("glBindVertexArray");
      DeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) getFunctionByName("glDeleteVertexArrays");

      CreateShader = (PFNGLCREATESHADERPROC) getFunctionByName("glCreateShader");
      CompileShader = (PFNGLCOMPILESHADERPROC) getFunctionByName("glCompileShader");
      ShaderSource = (PFNGLSHADERSOURCEPROC) getFunctionByName("glShaderSource");
      GetShaderiv = (PFNGLGETSHADERIVPROC) getFunctionByName("glGetShaderiv");
      GetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) getFunctionByName("glGetShaderInfoLog");
      AttachShader = (PFNGLATTACHSHADERPROC) getFunctionByName("glAttachShader");
      DeleteShader = (PFNGLDELETESHADERPROC) getFunctionByName("glDeleteShader");
      CreateProgram = (PFNGLCREATEPROGRAMPROC) getFunctionByName("glCreateProgram");
      LinkProgram = (PFNGLLINKPROGRAMPROC) getFunctionByName("glLinkProgram");
      GetProgramiv = (PFNGLGETPROGRAMIVPROC) getFunctionByName("glGetProgramiv");
      UseProgram = (PFNGLUSEPROGRAMPROC) getFunctionByName("glUseProgram");
      DeleteProgram = (PFNGLDELETEPROGRAMPROC) getFunctionByName("glDeleteProgram");
      GetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) getFunctionByName("glGetUniformLocation");
      GetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) getFunctionByName("glGetAttribLocation");
      VertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) getFunctionByName("glVertexAttribPointer");
      EnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) getFunctionByName("glEnableVertexAttribArray");
      DisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) getFunctionByName("glDisableVertexAttribArray");
      Uniform1f = (PFNGLUNIFORM1FPROC) getFunctionByName("glUniform1f");
      Uniform2f = (PFNGLUNIFORM2FPROC) getFunctionByName("glUniform2f");
      Uniform3f = (PFNGLUNIFORM3FPROC) getFunctionByName("glUniform3f");
      Uniform4f = (PFNGLUNIFORM4FPROC) getFunctionByName("glUniform4f");
      Uniform2fv = (PFNGLUNIFORM2FVPROC) getFunctionByName("glUniform2fv");
      Uniform3fv = (PFNGLUNIFORM3FVPROC) getFunctionByName("glUniform3fv");
      Uniform4fv = (PFNGLUNIFORM4FVPROC) getFunctionByName("glUniform4fv");
      UniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) getFunctionByName("glUniformMatrix4fv");

      mExtensionsRegistered = true;
   }
}

ExtensionLoaderGLCore::ExtensionLoaderGLCore()
{
   mModelViewMatrixStack.push(gmtl::MAT_IDENTITY44F);
   mProjectionMatrixStack.push(gmtl::MAT_IDENTITY44F);
   mCurrentMatrixStack = &mModelViewMatrixStack;
}

}

}