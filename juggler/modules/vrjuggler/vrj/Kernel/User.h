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

#ifndef _VRJ_USER_H_
#define _VRJ_USER_H_

#include <vrj/vjConfig.h>
//#include <vrj/Config/ConfigChunk.h>
#include <vrj/Input/Type/PosInterface.h>


namespace vrj
{
   
class ConfigChunk;

//-----------------------------------------
//: Representation for Juggler user in multi-user environments
//
// Each user has a system assigned id as well
// as a string name from the user config chunk
// the created the user.  These ids can be
// used to identify a user at set points in
// a juggler application where user information
// is given.
// <br>
// The system assigned id number can also be useful to use as an index
// into a program array (or other data strucutre) that stores user
// specific data.  Ex. Navigation matrices, input devices
//---------------------------------------------
//! PUBLIC_API:
class VJ_CLASS_API User
{
public:
   // Cosntruct the user
   User() : mUserId(-1), mName("")
   {;}

   virtual ~User()
   {;}

   //: Get the id of the user in the system
   //! RETURNS: -1 => User has not been configured
   int getId();

   //: Get the name of the user object
   //! RETURNS: string name of the head
   std::string getName();

   //: Configure the user object
   //! POST: User has valid ids (int and string)
   //! POST: Positional device for user location has been set
   virtual bool config(ConfigChunk* chunk);

   Matrix* getHeadPos()
   { return mHead->getData(); }

   TimeStaMp* getHeadUpdateTime()
   { return mHead->getUpdateTime(); }

   float getInterocularDistance()
   { return mInterocularDist; }

private:
   int               mUserId;    //: the id of the user
   std::string       mName;      //: The string name of the user
   PosInterface    mHead;      //: The head positon
   float             mInterocularDist;    //: Eye seperation

private:
   static int mNextUserId;     //: the next user id to assign

   User(const User& u) {;}
   void operator=(const User& u) {;}
};


};

#endif
