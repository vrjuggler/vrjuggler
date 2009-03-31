/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _VRJ_OPENGL_EXTENSION_LOADER_GLX_H_
#define _VRJ_OPENGL_EXTENSION_LOADER_GLX_H_

#include <vrj/Draw/OpenGL/Config.h>

#include <boost/shared_ptr.hpp>
#include <GL/glx.h>

#include <vrj/Draw/OpenGL/ExtensionLoader.h>

// GLX Defines
#define GLX_CONTEXT_MAJOR_VERSION_ARB		0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB		0x2092
#define GLX_CONTEXT_LAYER_PLANE_ARB		0x2093
#define GLX_CONTEXT_FLAGS_ARB			0x2094
#define GLX_CONTEXT_DEBUG_BIT_ARB		0x0001
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	0x0002

// New error returned by GetLastError
#define	ERROR_INVALID_VERSION_ARB		0x2095

namespace vrj
{

namespace opengl
{

/**
 * GLX-specific class for loading extensions.
 *
 * @note This class was renamed from vrj::GlxExtensionLoader in VR Juggler
 *       2.3.11.
 */
class ExtensionLoaderGLX : public vrj::opengl::ExtensionLoader
{
public:
   ExtensionLoaderGLX();

   /** Register common extensions that we may need to use. */
   virtual void registerExtensions(); 

   /** @name NVidia swap control. */
   //@{
   
   /** Return true if we have support for NV swap group extensions. */
   bool hasSwapGroupNV();

   Bool glXJoinSwapGroupNV(::Display* dpy, GLXDrawable drawable,
                           GLuint group);

   Bool glXBindSwapBarrierNV(::Display* dpy, GLuint group, GLuint barrier);

   Bool glXQuerySwapGroupNV(::Display* dpy, GLXDrawable drawable,
                            GLuint* group, GLuint *barrier);

   Bool glXQueryMaxSwapGroupsNV(::Display* dpy, int screen, GLuint* maxGroups,
                                GLuint *maxBarriers);

   Bool glXQueryFrameCountNV(::Display* dpy, int screen, GLuint* count);

   Bool glXResetFrameCountNV(::Display* dpy, int screen);
   //@}

   /** @name Context Creation/ */
   //@{
   
   //** Return true if we have support for ARB context creation. */
   bool hasCreateContextARB();

   GLXContext glXCreateContextAttribsARB(::Display* dpy, GLXFBConfig config,
                                         GLXContext shareContext,
                                         const Bool direct,
                                         const int* attribList);

   //@}

private:
   struct GlxFuncs;
   bool   mExtensionsRegistered;

   boost::shared_ptr<GlxFuncs>   mGlxFuncs;  /** Pimpl struct for holding typed functions. */
};

}  // namespace opengl

}  // namespace vrj


#endif
