/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_OPENGL_EXTENSION_LOADER_WGL_H_
#define _VRJ_OPENGL_EXTENSION_LOADER_WGL_H_

#include <vrj/Draw/OpenGL/Config.h>

#include <boost/shared_ptr.hpp>

#include <GL/gl.h>

#include <vrj/Draw/OpenGL/ExtensionLoader.h>

// WGL Defines
#define WGL_CONTEXT_MAJOR_VERSION_ARB		    0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB		    0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB		        0x2093
#define WGL_CONTEXT_FLAGS_ARB			        0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB		        0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	0x0002

// New error returned by GetLastError
#define	ERROR_INVALID_VERSION_ARB		        0x2095

namespace vrj
{

namespace opengl
{

/**
 * WGL-specific class for loading extensions.
 *
 * @note This class was renamed from vrj::GlExtensionLoaderWin32 in VR Juggler
 *       2.3.11.
 */
class ExtensionLoaderWGL : public vrj::opengl::ExtensionLoader
{
public:
   ExtensionLoaderWGL();

   /** Register common extensions that we may need to use. */
   virtual void registerExtensions();

public:
   /** @name NVidia swap control. */
   //@{
   
   /** Return true if we have support for NV swap group extensions. */
   bool hasSwapGroupNV();

   BOOL wglJoinSwapGroupNV(HDC hdc, GLuint group);

   BOOL wglBindSwapBarrierNV(GLuint group, GLuint barrier);

   BOOL wglQuerySwapGroupNV(HDC hdc, GLuint* group, GLuint* barrier);

   BOOL wglQueryMaxSwapGroupsNV(HDC hdc, GLuint* maxGroups,
                                GLuint* maxBarriers);

   BOOL wglQueryFrameCountNV(HDC hdc, /*int screen,*/ GLuint* count);

   BOOL wglResetFrameCountNV(HDC hdc /*, int screen*/);
   //@}

   /** @name Context Creation/ */
   //@{
   
   //** Return true if we have support for ARB context creation. */
   bool hasCreateContextARB();

   HGLRC wglCreateContextAttribsARB(HDC hdc, HGLRC hshareContext, const int *attribList);

   //@}

private:
   struct WglFuncs;
   bool   mExtensionsRegistered;

   boost::shared_ptr<WglFuncs>   mWglFuncs;  /** Pimpl struct for holding typed functions. */
};

}  // namespace opengl

}  // namespace vrj


#endif
