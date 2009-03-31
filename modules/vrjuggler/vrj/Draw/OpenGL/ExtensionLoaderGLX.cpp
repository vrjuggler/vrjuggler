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

#include <vrj/Draw/OpenGL/Config.h>

#include <GL/glx.h>
#include <GL/glxext.h>

#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#include <vrj/Draw/OpenGL/ExtensionLoaderGLX.h>


namespace vrj
{

namespace opengl
{

// NVIDIA swap control
typedef Bool (* PFNGLXJOINSWAPGROUPNVPROC) (::Display*, GLXDrawable,
                                           GLuint);

typedef Bool (* PFNGLXBINDSWAPBARRIERNVPROC) (::Display*, GLuint, GLuint);

typedef Bool (* PFNGLXQUERYSWAPGROUPNVPROC) (::Display*, GLXDrawable,
                                             GLuint*, GLuint*);

typedef Bool (* PFNGLXQUERYMAXSWAPGROUPSNVPROC) (::Display*, int, GLuint*,
                                                 GLuint*);

typedef Bool (* PFNGLXQUERYFRAMECOUNTNVPROC) (::Display*, int, GLuint*);

typedef Bool (* PFNGLXRESETFRAMECOUNTNVPROC) (::Display*, int);

// ARB Context Creation
typedef GLXContext (*PFNGLXCREATECONTEXTATTRIBSARBPROC) (::Display*,
                                                         GLXFBConfig,
                                                         GLXContext, Bool,
                                                         const int*);

struct ExtensionLoaderGLX::GlxFuncs
{
   GlxFuncs()
      : glXJoinSwapGroupNV(NULL)
      , glXBindSwapBarrierNV(NULL)
      , glXQuerySwapGroupNV(NULL)
      , glXQueryMaxSwapGroupsNV(NULL)
      , glXQueryFrameCountNV(NULL)
      , glXResetFrameCountNV(NULL)
      , glXCreateContextAttribsARB(NULL)
   {
      /* Do nothing. */ ;
   }

   PFNGLXJOINSWAPGROUPNVPROC         glXJoinSwapGroupNV;
   PFNGLXBINDSWAPBARRIERNVPROC       glXBindSwapBarrierNV;
   PFNGLXQUERYSWAPGROUPNVPROC        glXQuerySwapGroupNV;
   PFNGLXQUERYMAXSWAPGROUPSNVPROC    glXQueryMaxSwapGroupsNV;
   PFNGLXQUERYFRAMECOUNTNVPROC       glXQueryFrameCountNV;
   PFNGLXRESETFRAMECOUNTNVPROC       glXResetFrameCountNV;
   PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;
};

ExtensionLoaderGLX::ExtensionLoaderGLX()
   : mExtensionsRegistered(false)
{
   mGlxFuncs = boost::shared_ptr<GlxFuncs>(new GlxFuncs);
}

void ExtensionLoaderGLX::registerExtensions()
{
   if ( ! mExtensionsRegistered )
   {
      vrj::opengl::ExtensionLoader::registerExtensions();
   
      mGlxFuncs->glXJoinSwapGroupNV =
         (PFNGLXJOINSWAPGROUPNVPROC) getFunctionByName("glXJoinSwapGroupNV");
      mGlxFuncs->glXBindSwapBarrierNV =
         (PFNGLXBINDSWAPBARRIERNVPROC) getFunctionByName("glXBindSwapBarrierNV");
      mGlxFuncs->glXQuerySwapGroupNV =
         (PFNGLXQUERYSWAPGROUPNVPROC) getFunctionByName("glXQuerySwapGroupNV");
      mGlxFuncs->glXQueryMaxSwapGroupsNV  =
         (PFNGLXQUERYMAXSWAPGROUPSNVPROC) getFunctionByName("glXQueryMaxSwapGroupsNV");
      mGlxFuncs->glXQueryFrameCountNV =
         (PFNGLXQUERYFRAMECOUNTNVPROC) getFunctionByName("glXQueryFrameCountNV");
      mGlxFuncs->glXResetFrameCountNV =
         (PFNGLXRESETFRAMECOUNTNVPROC) getFunctionByName("glXResetFrameCountNV");
      mGlxFuncs->glXCreateContextAttribsARB =
         (PFNGLXCREATECONTEXTATTRIBSARBPROC) getFunctionByName("glXCreateContextAttribsARB");
         
      mExtensionsRegistered = true;
   }
}

// --- NVIDIA Swap Control methods --- //
bool ExtensionLoaderGLX::hasSwapGroupNV()
{
   return mGlxFuncs->glXJoinSwapGroupNV != NULL;
}

Bool ExtensionLoaderGLX::glXJoinSwapGroupNV(::Display* dpy,
                                            GLXDrawable drawable,
                                            GLuint group)
{
   vprASSERT(mGlxFuncs->glXJoinSwapGroupNV != NULL &&
             "Attemped to call unsupported extension.");
   return mGlxFuncs->glXJoinSwapGroupNV(dpy, drawable, group);
}

Bool ExtensionLoaderGLX::glXBindSwapBarrierNV(::Display* dpy, GLuint group,
                                              GLuint barrier)
{
   vprASSERT(mGlxFuncs->glXBindSwapBarrierNV != NULL &&
             "Attemped to call unsupported extension.");
   return mGlxFuncs->glXBindSwapBarrierNV(dpy, group, barrier);
}

Bool ExtensionLoaderGLX::glXQuerySwapGroupNV(::Display* dpy,
                                             GLXDrawable drawable,
                                             GLuint* group, GLuint* barrier)
{
   vprASSERT(mGlxFuncs->glXQuerySwapGroupNV != NULL &&
             "Attemped to call unsupported extension.");
   return mGlxFuncs->glXQuerySwapGroupNV(dpy, drawable, group, barrier);
}

Bool ExtensionLoaderGLX::glXQueryMaxSwapGroupsNV(::Display* dpy, int screen,
                                                 GLuint* maxGroups,
                                                 GLuint* maxBarriers)
{
   vprASSERT(mGlxFuncs->glXQueryMaxSwapGroupsNV != NULL &&
             "Attemped to call unsupported extension.");
   return mGlxFuncs->glXQueryMaxSwapGroupsNV(dpy, screen, maxGroups,
                                             maxBarriers);
}

Bool ExtensionLoaderGLX::glXQueryFrameCountNV(::Display* dpy, int screen,
                                              GLuint* count)
{
   vprASSERT(mGlxFuncs->glXQueryFrameCountNV != NULL &&
             "Attemped to call unsupported extension.");
   return mGlxFuncs->glXQueryFrameCountNV(dpy, screen, count);
}

Bool ExtensionLoaderGLX::glXResetFrameCountNV(::Display* dpy, int screen)
{
   vprASSERT(mGlxFuncs->glXResetFrameCountNV != NULL &&
             "Attemped to call unsupported extension.");
   return mGlxFuncs->glXResetFrameCountNV(dpy, screen);
}

// --- ARB Context Creation methods --- //
bool ExtensionLoaderGLX::hasCreateContextARB()
{
  return (mGlxFuncs->glXCreateContextAttribsARB != NULL);
}

GLXContext
ExtensionLoaderGLX::glXCreateContextAttribsARB(::Display *dpy, 
                                               GLXFBConfig config,
                                               GLXContext shareContext,
                                               const Bool direct,
                                               const int* attribList)
{
  vprASSERT(mGlxFuncs->glXCreateContextAttribsARB != NULL &&
            "Attemped to call unsupported extension."); 
  return mGlxFuncs->glXCreateContextAttribsARB(dpy, config, shareContext,
                                               direct, attribList);
}

}  // namespace opengl

}  // namespace vrj
