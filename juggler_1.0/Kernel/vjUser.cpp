/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#include <vjConfig.h>

#include <Kernel/vjUser.h>
#include <Kernel/vjDebug.h>


int vjUser::mNextUserId = 0;


int vjUser::getId()
{ return mUserId; }

std::string vjUser::getName()
{ return mName; }

bool vjUser::config(vjConfigChunk* chunk)
{
   vjASSERT(chunk != NULL);
   vjASSERT((std::string)chunk->getType() == std::string("JugglerUser"));

   vjDEBUG_BEGIN(vjDBG_KERNEL,3) << "vjUser::config: Creating a new user\n" << vjDEBUG_FLUSH;

   // Assign user id
   mUserId = mNextUserId++;

   // Setup user name
   mName = (std::string)chunk->getProperty("Name");

   // Initialize the head stuff
   std::string head_alias = (std::string)chunk->getProperty("headPos");
   mHead.init(head_alias);

   vjDEBUG(vjDBG_KERNEL,3) << "id: " << mUserId << "   Name:" << mName.c_str() << "   headPos:" << head_alias.c_str() << endl << vjDEBUG_FLUSH;
   vjDEBUG_END(vjDBG_KERNEL,3) << "\n" << vjDEBUG_FLUSH;
   return true;
}
