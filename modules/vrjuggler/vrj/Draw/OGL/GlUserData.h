/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_GL_USER_DATA_H_
#define _VJ_GL_USER_DATA_H_
//#pragma once

#include <Kernel/vjUser.h>
#include <Kernel/vjProjection.h>

//---------------------------------------------------
//: Holds data about gl users for draw process
//
// This class holds interesting information that can
// be used in a gl draw callback to find information
// about the user.
//---------------------------------------------------
//!PUBLIC_API:
class vjGlUserData
{
public:
   vjGlUserData()
   {
      mUser = NULL;
      mProj = NULL;
   }

   vjUser* getUser()
      { return mUser; }
   void setUser(vjUser* user)
      { mUser = user; }

   vjProjection* getProjection()
      { return mProj; }
   void setProjection(vjProjection* proj)
      { mProj = proj; }

protected:
   vjUser* mUser;
   vjProjection* mProj;
};

#endif
