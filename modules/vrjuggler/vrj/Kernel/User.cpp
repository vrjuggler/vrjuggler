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


#include <vjConfig.h>

#include <Kernel/vjUser.h>
#include <Utils/vjDebug.h>
#include <Config/vjConfigChunk.h>


namespace vrj
{
   
int User::mNextUserId = 0;


int User::getId()
{ return mUserId; }

std::string User::getName()
{ return mName; }

bool User::config(ConfigChunk* chunk)
{
   vprASSERT(chunk != NULL);
   vprASSERT((std::string)chunk->getType() == std::string("JugglerUser"));

   vjDEBUG_BEGIN(vjDBG_KERNEL,3) << "vjUser::config: Creating a new user\n" << vjDEBUG_FLUSH;

   // Assign user id
   mUserId = mNextUserId++;

   // Setup user name
   mName = (std::string)chunk->getProperty("Name");

   // Initialize the head stuff
   std::string head_alias = (std::string)chunk->getProperty("headPos");
   mHead.init(head_alias);

   // Initialize interocular distance
   mInterocularDist = chunk->getProperty("interocular_distance");

   if(mInterocularDist == 0.0f)
   {
      vjDEBUG(vjDBG_KERNEL,vjDBG_CONFIG_LVL) << clrOutNORM(clrRED, "WARNING:") << "User: " << mName << " has interocular distance is set to 0.0f.  This is probably not what you wanted.\n" << vjDEBUG_FLUSH;
   }

   vjDEBUG(vjDBG_KERNEL,vjDBG_STATE_LVL) << "id: " << mUserId << "   Name:" << mName.c_str()
                           << "   headPos:" << head_alias.c_str()
                           << "   interocular_distance:" << mInterocularDist
                           << std::endl << vjDEBUG_FLUSH;

   return true;
}


};
