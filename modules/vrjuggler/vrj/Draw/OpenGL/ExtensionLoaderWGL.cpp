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

#include <vrj/Draw/OpenGL/Config.h>

#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#include <vrj/Draw/OpenGL/ExtensionLoaderWGL.h>

#ifndef GLAPI
#  define GLAPI __stdcall
#  define __DEFINED_GLAPI
#endif


namespace vrj
{

namespace opengl
{

// NVIDIA swap control
typedef BOOL (GLAPI * PFNWGLJOINSWAPGROUPNVPROC) (HDC hdc, GLuint group);

typedef BOOL (GLAPI * PFNWGLBINDSWAPBARRIERNVPROC) (GLuint group,
                                                    GLuint barrier);

typedef BOOL (GLAPI * PFNWGLQUERYSWAPGROUPNVPROC) (HDC hdc, GLuint* group,
                                                   GLuint* barrier);

typedef BOOL (GLAPI * PFNWGLQUERYMAXSWAPGROUPSNVPROC) (HDC hdc,
                                                       /*int screen,*/
                                                       GLuint* maxGroups,
                                                       GLuint* maxBarriers);

typedef BOOL (GLAPI * PFNWGLQUERYFRAMECOUNTNVPROC) (HDC hdc,
                                                    /*int screen,*/
                                                    GLuint* count);

typedef BOOL (GLAPI * PFNWGLRESETFRAMECOUNTNVPROC) (HDC hdc /*, int screen*/);

// ARB Context Creation
typedef HGLRC (GLAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC, HGLRC,
                                                          const int*);

struct ExtensionLoaderWGL::WglFuncs
{
   WglFuncs()
      : wglJoinSwapGroupNV(NULL)
      , wglBindSwapBarrierNV(NULL)
      , wglQuerySwapGroupNV(NULL)
      , wglQueryMaxSwapGroupsNV(NULL)
      , wglQueryFrameCountNV(NULL)
      , wglResetFrameCountNV(NULL)
      , wglCreateContextAttribsARB(NULL)
   {
      /* Do nothing. */ ;
   }

   PFNWGLJOINSWAPGROUPNVPROC         wglJoinSwapGroupNV;
   PFNWGLBINDSWAPBARRIERNVPROC       wglBindSwapBarrierNV;
   PFNWGLQUERYSWAPGROUPNVPROC        wglQuerySwapGroupNV;
   PFNWGLQUERYMAXSWAPGROUPSNVPROC    wglQueryMaxSwapGroupsNV;
   PFNWGLQUERYFRAMECOUNTNVPROC       wglQueryFrameCountNV;
   PFNWGLRESETFRAMECOUNTNVPROC       wglResetFrameCountNV;
   PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
};

ExtensionLoaderWGL::ExtensionLoaderWGL()
   : mExtensionsRegistered(false)
{
   mWglFuncs = boost::shared_ptr<WglFuncs>(new WglFuncs);
}

void ExtensionLoaderWGL::registerExtensions()
{
   if(!mExtensionsRegistered)
   {
      vrj::opengl::ExtensionLoader::registerExtensions();

      mWglFuncs->wglJoinSwapGroupNV =
         (PFNWGLJOINSWAPGROUPNVPROC) getFunctionByName("wglJoinSwapGroupNV");
      mWglFuncs->wglBindSwapBarrierNV =
         (PFNWGLBINDSWAPBARRIERNVPROC) getFunctionByName("wglBindSwapBarrierNV");
      mWglFuncs->wglQuerySwapGroupNV =
         (PFNWGLQUERYSWAPGROUPNVPROC) getFunctionByName("wglQuerySwapGroupNV");
      mWglFuncs->wglQueryMaxSwapGroupsNV =
         (PFNWGLQUERYMAXSWAPGROUPSNVPROC) getFunctionByName("wglQueryMaxSwapGroupsNV");
      mWglFuncs->wglQueryFrameCountNV =
         (PFNWGLQUERYFRAMECOUNTNVPROC) getFunctionByName("wglQueryFrameCountNV");
      mWglFuncs->wglResetFrameCountNV =
         (PFNWGLRESETFRAMECOUNTNVPROC) getFunctionByName("wglResetFrameCountNV");
      mWglFuncs->wglCreateContextAttribsARB =
         (PFNWGLCREATECONTEXTATTRIBSARBPROC) getFunctionByName("wglCreateContextAttribsARB");
       
      mExtensionsRegistered = true;
   }
}

// --- NVIDIA Swap Control methods --- //
bool ExtensionLoaderWGL::hasSwapGroupNV()
{
   return (mWglFuncs->wglJoinSwapGroupNV != NULL);
}

BOOL ExtensionLoaderWGL::wglJoinSwapGroupNV(HDC hdc, GLuint group)
{
   vprASSERT(mWglFuncs->wglJoinSwapGroupNV != NULL &&
             "Attemped to call unsupported extension.");
   return mWglFuncs->wglJoinSwapGroupNV(hdc, group);
}

BOOL ExtensionLoaderWGL::wglBindSwapBarrierNV(GLuint group, GLuint barrier)
{
   vprASSERT(mWglFuncs->wglBindSwapBarrierNV != NULL &&
             "Attemped to call unsupported extension.");
   return mWglFuncs->wglBindSwapBarrierNV(group, barrier);
}

BOOL ExtensionLoaderWGL::wglQuerySwapGroupNV(HDC hdc, GLuint* group,
                                             GLuint* barrier)
{
   vprASSERT(mWglFuncs->wglQuerySwapGroupNV != NULL &&
             "Attemped to call unsupported extension.");
   return mWglFuncs->wglQuerySwapGroupNV(hdc, group, barrier);
}

BOOL ExtensionLoaderWGL::wglQueryMaxSwapGroupsNV(HDC hdc, GLuint* maxGroups,
                                                 GLuint* maxBarriers)
{
   vprASSERT(mWglFuncs->wglQueryMaxSwapGroupsNV != NULL &&
             "Attemped to call unsupported extension.");
   return mWglFuncs->wglQueryMaxSwapGroupsNV(hdc, maxGroups, maxBarriers);
}

BOOL ExtensionLoaderWGL::wglQueryFrameCountNV(HDC hdc, /*int screen,*/
                                              GLuint* count)
{
   vprASSERT(mWglFuncs->wglQueryFrameCountNV != NULL &&
             "Attemped to call unsupported extension.");
   return mWglFuncs->wglQueryFrameCountNV(hdc, /*screen,*/ count);
}

BOOL ExtensionLoaderWGL::wglResetFrameCountNV(HDC hdc /*, int screen*/)
{
   vprASSERT(mWglFuncs->wglResetFrameCountNV != NULL &&
             "Attemped to call unsupported extension.");
   return mWglFuncs->wglResetFrameCountNV(hdc /*, screen*/);
}

// --- ARB Context Creation methods --- //
bool ExtensionLoaderWGL::hasCreateContextARB()
{
   return (mWglFuncs->wglCreateContextAttribsARB != NULL);
}

HGLRC ExtensionLoaderWGL::wglCreateContextAttribsARB(HDC hdc,
                                                     HGLRC hshareContext,
                                                     const int* attribList)
{
   vprASSERT(mWglFuncs->wglCreateContextAttribsARB != NULL &&
             "Attemped to call unsupported extension."); 
   return mWglFuncs->wglCreateContextAttribsARB(hdc, hshareContext,
                                                attribList);
}

}  // namespace opengl

}  // namespace vrj
