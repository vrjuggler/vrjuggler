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

#include <vrj/vrjConfig.h>

#include <vrj/Kernel/User.h>
#include <vrj/Util/Debug.h>
#include <jccl/Config/ConfigElement.h>


namespace vrj
{

int User::mNextUserId = 0;

vrj::UserPtr User::create()
{
   return vrj::UserPtr(new User());
}

int User::getId() const
{
   return mUserId;
}

const std::string& User::getName() const
{
   return mName;
}

bool User::config(jccl::ConfigElementPtr element)
{
   vprASSERT(element.get() != NULL);
   vprASSERT(element->getID() == "user");

   vprDEBUG_BEGIN(vrjDBG_KERNEL, vprDBG_STATE_LVL)
      << "vjUser::config: Creating a new user\n" << vprDEBUG_FLUSH;

   // Assign user id
   mUserId = mNextUserId++;

   // Setup user name
   mName = element->getName();

   // Initialize the head stuff
   std::string head_alias = element->getProperty<std::string>("head_position");
   mHead.init(head_alias);

   // Initialize interocular distance
   mInterocularDist = element->getProperty<float>("interocular_distance");

   if(mInterocularDist == 0.0f)
   {
      vprDEBUG(vrjDBG_KERNEL,vprDBG_CONFIG_LVL) << clrOutNORM(clrRED, "WARNING:") << "User: " << mName << " has interocular distance is set to 0.0f.  This is probably not what you wanted.\n" << vprDEBUG_FLUSH;
   }

   vprDEBUG(vrjDBG_KERNEL,vprDBG_STATE_LVL) << "id: " << mUserId << "   Name:" << mName.c_str()
                           << "   head_positon:" << head_alias.c_str()
                           << "   interocular_distance:" << mInterocularDist
                           << std::endl << vprDEBUG_FLUSH;

   return true;
}

}
