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
   //Get the list of active config chunks from config manager
   //Unlock the list
   //Compile a string of the config chunk DB
   //Return it

   vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
      << "RTRCInterfaceSubjectImpl::getChunks() not implemented yet! Returning an empty string...\n"
      << vprDEBUG_FLUSH;

   return NULL;
}

}; // namespace jccl
