/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <jccl/jcclConfig.h>

#include <string>
#include <list>
#include <sstream>

#include <jccl/Config/ChunkFactory.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkDescDB.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/RTRC/ConfigManager.h>

#include <jccl/RTRC/RTRCInterfaceSubjectImpl.h>

namespace jccl
{

void RTRCInterfaceSubjectImpl::add( const char* chunkDBString )
{
   jccl::ConfigChunkDB newChunks;
   std::string chunkString = chunkDBString;

   //Create a string that contains a full Juggler config chunk DB
   //Loading from an istream
   std::istringstream input (chunkString);

   input >> newChunks;
   
   //Send pending changes to ConfigManager
   jccl::ConfigManager::instance()->addPendingAdds( &newChunks );
}


void RTRCInterfaceSubjectImpl::remove( const char* chunkDBString )
{
   jccl::ConfigChunkDB deleteChunks;
   std::string chunkString = chunkDBString;

   //Create a string that contains a full Juggler config chunk DB
   //Loading from an istream
   std::istringstream input (chunkString);

   input >> deleteChunks;

   //Send pending changes to ConfigManager   
   jccl::ConfigManager::instance()->addPendingRemoves( &deleteChunks );

}


void RTRCInterfaceSubjectImpl::swap( const char* addChunkDBString, const char* removeChunkDBString )
{
   jccl::ConfigChunkDB addChunks;
   jccl::ConfigChunkDB deleteChunks;
   std::string addChunkString = addChunkDBString;
   std::string removeChunkString = removeChunkDBString;

   //Loading from an istream
   std::istringstream add_input (addChunkString);
   std::istringstream remove_input (removeChunkString);

   add_input >> addChunks;
   remove_input >> deleteChunks;
   
   //Send pending changes to ConfigManager
   jccl::ConfigManager::instance()->addPendingRemoves( &deleteChunks );
   jccl::ConfigManager::instance()->addPendingAdds( &addChunks );

}

char* RTRCInterfaceSubjectImpl::getChunks()
{
   //Lock the config manager
   jccl::ConfigManager::instance()->lockActive();

   //Get the list of active config chunks from config manager
   ConfigChunkDB* activeChunkDB = jccl::ConfigManager::instance()->getActiveConfig();

   //Compile a string of the config chunk DB
   std::ostringstream active_output;

   active_output << activeChunkDB;

   //Unlock the list
   jccl::ConfigManager::instance()->unlockActive();

   //Return the full string
   return CORBA::string_dup(active_output.str().c_str());
}

char* RTRCInterfaceSubjectImpl::getChunkDescs()
{

   //Get the list of chunk descs from the ChunkFactory
   ChunkDescDB* chunkDescs = jccl::ChunkFactory::instance()->getChunkDescDB();

   //Compile a string of the chunk desc DB
   std::ostringstream descs_output;

   descs_output << chunkDescs;

   //Return the full string
   return CORBA::string_dup(descs_output.str().c_str());
}


}; // namespace jccl
