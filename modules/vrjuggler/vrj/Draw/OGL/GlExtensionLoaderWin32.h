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

#ifndef VRJ_WGL_EXTENSION_LOADER_H_
#define VRJ_WGL_EXTENSION_LOADER_H_

#include <vrj/Draw/OGL/GlExtensionLoader.h>
#include <boost/shared_ptr.hpp>

#include <windows.h>
#include <GL/gl.h>

namespace vrj
{

/** WGL specific class for loading extensions.
 */
class GlExtensionLoaderWin32 : public GlExtensionLoader
{
public:
   GlExtensionLoaderWin32();

   /** Register common extensions that we may need to use. */
   virtual void registerExtensions(); 

public:
   /** @name NVidia swap control. */
   //@{
   
   /** Return true if we have support for NV swap group extensions. */
   bool hasSwapGroupNV();

   BOOL wglJoinSwapGroupNV(HDC hdc, GLuint group);

   BOOL wglBindSwapBarrierNV(GLuint group, GLuint barrier);

   BOOL wglQuerySwapGroupNV(HDC hdc, GLuint *group, GLuint *barrier);

   BOOL wglQueryMaxSwapGroupsNV(HDC hdc, GLuint *maxGroups, GLuint *maxBarriers);

   BOOL wglQueryFrameCountNV(HDC hdc, /*int screen,*/ GLuint *count);

   BOOL wglResetFrameCountNV(HDC hdc/*, int screen*/);
   //@}

protected:
   struct WglFuncs;

   boost::shared_ptr<WglFuncs>   mWglFuncs;  /** Pimpl struct for holding typed functions. */
};

}  // namespace vrj

#endif

