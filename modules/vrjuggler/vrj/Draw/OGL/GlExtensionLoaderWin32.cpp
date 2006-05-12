/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#include <vrj/Draw/OGL/GlExtensionLoaderWin32.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>

#ifndef GLAPI
#  define GLAPI __stdcall
#  define __DEFINED_GLAPI
#endif

namespace vrj
{
   // NVIDIA swap control
   typedef BOOL (GLAPI * PFNWGLJOINSWAPGROUPNVPROC) (HDC hdc,
                                                GLuint group);
   
   typedef BOOL (GLAPI * PFNWGLBINDSWAPBARRIERNVPROC) (GLuint group,
                                                  GLuint barrier);
   
   typedef BOOL (GLAPI * PFNWGLQUERYSWAPGROUPNVPROC) (HDC hdc,
                                                 GLuint *group,
                                                 GLuint *barrier);
   
   typedef BOOL (GLAPI * PFNWGLQUERYMAXSWAPGROUPSNVPROC) (HDC hdc,
                                                     /*int screen,*/
                                                     GLuint *maxGroups,
                                                     GLuint *maxBarriers);
   
   typedef BOOL (GLAPI * PFNWGLQUERYFRAMECOUNTNVPROC) (HDC hdc,
                                                  /*int screen,*/
                                                  GLuint *count);
   
   typedef BOOL (GLAPI * PFNWGLRESETFRAMECOUNTNVPROC) (HDC hdc/*, int screen*/);


struct GlExtensionLoaderWin32::WglFuncs
{
   WglFuncs()
   {
      wglJoinSwapGroupNV = NULL;
      wglBindSwapBarrierNV = NULL;
      wglQuerySwapGroupNV = NULL;
      wglQueryMaxSwapGroupsNV = NULL;
      wglQueryFrameCountNV = NULL;
      wglResetFrameCountNV = NULL;
   }

   PFNWGLJOINSWAPGROUPNVPROC        wglJoinSwapGroupNV;
   PFNWGLBINDSWAPBARRIERNVPROC      wglBindSwapBarrierNV;
   PFNWGLQUERYSWAPGROUPNVPROC       wglQuerySwapGroupNV;
   PFNWGLQUERYMAXSWAPGROUPSNVPROC   wglQueryMaxSwapGroupsNV;
   PFNWGLQUERYFRAMECOUNTNVPROC      wglQueryFrameCountNV;
   PFNWGLRESETFRAMECOUNTNVPROC      wglResetFrameCountNV;
};

GlExtensionLoaderWin32::GlExtensionLoaderWin32()
{
   mWglFuncs = boost::shared_ptr<WglFuncs>(new WglFuncs);
}

void GlExtensionLoaderWin32::registerExtensions()
{
   GlExtensionLoader::registerExtensions();

   mWglFuncs->wglJoinSwapGroupNV       = (PFNWGLJOINSWAPGROUPNVPROC)       (getFunctionByName("wglJoinSwapGroupNV"));
   mWglFuncs->wglBindSwapBarrierNV     = (PFNWGLBINDSWAPBARRIERNVPROC)     (getFunctionByName("wglBindSwapBarrierNV"));
   mWglFuncs->wglQuerySwapGroupNV      = (PFNWGLQUERYSWAPGROUPNVPROC)      (getFunctionByName("wglQuerySwapGroupNV"));
   mWglFuncs->wglQueryMaxSwapGroupsNV  = (PFNWGLQUERYMAXSWAPGROUPSNVPROC)  (getFunctionByName("wglQueryMaxSwapGroupsNV"));
   mWglFuncs->wglQueryFrameCountNV     = (PFNWGLQUERYFRAMECOUNTNVPROC)     (getFunctionByName("wglQueryFrameCountNV"));
   mWglFuncs->wglResetFrameCountNV     = (PFNWGLRESETFRAMECOUNTNVPROC)     (getFunctionByName("wglResetFrameCountNV"));
}

// --- NVIDIA Swap Control methods --- //
bool GlExtensionLoaderWin32::hasSwapGroupNV()
{
   return (mWglFuncs->wglJoinSwapGroupNV != NULL);
}

BOOL GlExtensionLoaderWin32::wglJoinSwapGroupNV(HDC hdc, GLuint group)
{
   vprASSERT(mWglFuncs->wglJoinSwapGroupNV != NULL && "Attemped to call unsupported extension.");
   return mWglFuncs->wglJoinSwapGroupNV(hdc, group);
}

BOOL GlExtensionLoaderWin32::wglBindSwapBarrierNV(GLuint group, GLuint barrier)
{
   vprASSERT(mWglFuncs->wglBindSwapBarrierNV != NULL && "Attemped to call unsupported extension.");
   return mWglFuncs->wglBindSwapBarrierNV(group, barrier);
}

BOOL GlExtensionLoaderWin32::wglQuerySwapGroupNV(HDC hdc, GLuint *group, GLuint *barrier)
{
   vprASSERT(mWglFuncs->wglQuerySwapGroupNV != NULL && "Attemped to call unsupported extension.");
   return mWglFuncs->wglQuerySwapGroupNV(hdc, group, barrier);
}

BOOL GlExtensionLoaderWin32::wglQueryMaxSwapGroupsNV(HDC hdc, GLuint *maxGroups, GLuint *maxBarriers)
{
   vprASSERT(mWglFuncs->wglQueryMaxSwapGroupsNV != NULL && "Attemped to call unsupported extension.");
   return mWglFuncs->wglQueryMaxSwapGroupsNV(hdc, maxGroups, maxBarriers);
}

BOOL GlExtensionLoaderWin32::wglQueryFrameCountNV(HDC hdc, /*int screen,*/ GLuint *count)
{
   vprASSERT(mWglFuncs->wglQueryFrameCountNV != NULL && "Attemped to call unsupported extension.");
   return mWglFuncs->wglQueryFrameCountNV(hdc, /*screen,*/ count);
}

BOOL GlExtensionLoaderWin32::wglResetFrameCountNV(HDC hdc/*, int screen*/)
{
   vprASSERT(mWglFuncs->wglResetFrameCountNV != NULL && "Attemped to call unsupported extension.");
   return mWglFuncs->wglResetFrameCountNV(hdc/*, screen*/);
}

}  // namespace vrj
