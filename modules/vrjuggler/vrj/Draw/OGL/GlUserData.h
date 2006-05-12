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

#ifndef _VRJ_GL_USER_DATA_H_
#define _VRJ_GL_USER_DATA_H_
//#pragma once

#include <vrj/vrjConfig.h>

/*
#include <vrj/Kernel/User.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/Viewport.h>
#include <vrj/Draw/OGL/GlWindow.h>
*/

namespace vrj
{

// Declare the class that we need pointers too below
class User;
class Projection;
class Viewport;
class GlWindow;


/** \class GlUserData GlUserData.h vrj/Draw/OGL/GlUserData.h
 *
 * Holds data about gl users for draw process.
 *
 * This class holds interesting information that can be used in an OpenGL draw
 * callback to find information about the user.
 *
 * @note These values are only intended for use by advanced users/applications.
 *       By using these values it is possible to create applications that are
 *       non-portable.
 */
class GlUserData
{
public:
   GlUserData()
      : mUser(NULL)
      , mProj(NULL)
      , mViewport(NULL)
      , mGlWindow(NULL)
   {
      /* Do nothing. */ ;
   }

   User* getUser()
   {
      return mUser;
   }

   void setUser(User* user)
   {
      mUser = user;
   }

   Projection* getProjection()
   {
      return mProj;
   }

   void setProjection(Projection* proj)
   {
      mProj = proj;
   }

   Viewport* getViewport()
   {
      return mViewport;
   }

   void setViewport(Viewport* vp)
   {
      mViewport = vp;
   }

   GlWindow* getGlWindow()
   {
      return mGlWindow;
   }

   void setGlWindow(GlWindow* win)
   {
      mGlWindow = win;
   }

protected:
   User*        mUser;        /**< The current user we are rendering */
   Projection*  mProj;        /**< The current projection being used */
   Viewport*    mViewport;    /**< The current vrj viewport being used */
   GlWindow*    mGlWindow;    /**< The current GL window that we are rendering in. (basically the gl context) */
};

}

#endif
