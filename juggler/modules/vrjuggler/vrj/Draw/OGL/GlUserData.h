/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_GL_USER_DATA_H_
#define _VRJ_GL_USER_DATA_H_
//#pragma once

#include <vrj/vjConfig.h>

#include <vrj/Kernel/User.h>
#include <vrj/Display/Projection.h>
#include <vrj/Display/Viewport.h>

namespace vrj
{
   
//---------------------------------------------------
//: Holds data about gl users for draw process
//
// This class holds interesting information that can
// be used in a gl draw callback to find information
// about the user.
//---------------------------------------------------
//!PUBLIC_API:
class GlUserData
{
public:
   GlUserData()
   {
      mUser = NULL;
      mProj = NULL;
   }

   User* getUser()
      { return mUser; }
   void setUser(User* user)
      { mUser = user; }

   Projection* getProjection()
      { return mProj; }
   void setProjection(Projection* proj)
      { mProj = proj; }

   Viewport* getViewport()
      { return mViewport; }
   void setViewport(Viewport* vp)
      { mViewport = vp; }

protected:
   User*        mUser;
   Projection*  mProj;
   Viewport*    mViewport;
};


};
#endif
