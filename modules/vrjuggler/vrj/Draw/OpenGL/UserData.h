/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _VRJ_OPENGL_USER_DATA_H_
#define _VRJ_OPENGL_USER_DATA_H_

#include <vrj/Draw/OpenGL/Config.h>

#include <vrj/Kernel/UserPtr.h>
#include <vrj/Display/ProjectionPtr.h>
#include <vrj/Display/ViewportPtr.h>
#include <vrj/Draw/OpenGL/WindowPtr.h>


namespace vrj
{

namespace opengl
{

/** \class vrj::opengl::UserData UserData.h vrj/Draw/OpenGL/UserData.h
 *
 * Holds data about OpenGL users for draw process.
 *
 * This class holds interesting information that can be used in an OpenGL draw
 * callback to find information about the user.
 *
 * @note These values are only intended for use by advanced users/applications.
 *       By using these values it is possible to create applications that are
 *       non-portable.
 *
 * @note This class was renamed from vrj::GlUserData in VR Juggler 2.3.11.
 */
class VJ_OGL_CLASS_API UserData
{
public:
   UserData();

   const UserPtr getUser() const
   {
      return mUser;
   }

   void setUser(UserPtr user)
   {
      mUser = user;
   }

   const ProjectionPtr getProjection() const
   {
      return mProj;
   }

   void setProjection(ProjectionPtr proj)
   {
      mProj = proj;
   }

   const ViewportPtr getViewport() const
   {
      return mViewport;
   }

   void setViewport(ViewportPtr vp)
   {
      mViewport = vp;
   }

   const vrj::opengl::WindowPtr getGlWindow() const
   {
      return mGlWindow;
   }

   void setGlWindow(vrj::opengl::WindowPtr win)
   {
      mGlWindow = win;
   }

private:
   UserPtr       mUser;        /**< The current user we are rendering */
   ProjectionPtr mProj;        /**< The current projection being used */
   ViewportPtr   mViewport;    /**< The current vrj viewport being used */

   /**
    * The current OpenGL window that we are rendering in (basically the OpenGL
    * context).
    */
   vrj::opengl::WindowPtr mGlWindow;
};

}

}


#endif
