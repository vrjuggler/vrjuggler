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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
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
