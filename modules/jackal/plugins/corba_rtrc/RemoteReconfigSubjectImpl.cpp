#include <jccl/RTRC/RTRCInterface.h>

namespace jccl
{

RTRCInterfaceSubjectImpl::RTRCInterfaceSubjectImpl()
{
}

RTRCInterfaceSubjectImpl::~RTRCInterfaceSubjectImpl()
{
}

void RTRCInterfaceSubjectImpl::add( std::string chunkDBString )
{
   jccl::ConfigChunkDB newChunks;

   //Create a string that contains a full Juggler config chunk DB
   //Loading from an istream
   std::istringstream input (chunkDBString);

   input >> newChunks;
   
   //Send pending changes to ConfigManager
   jccl::ConfigManager::instance()->addPendingAdds( &myChunks );
}


void RTRCInterfaceSubjectImpl::remove( std::string chunkDBString )
{
   jccl::ConfigChunkDB deleteChunks;

   //Create a string that contains a full Juggler config chunk DB
   //Loading from an istream
   std::istringstream input (chunkDBString);

   input >> deleteChunks;

   //Send pending changes to ConfigManager   
   jccl::ConfigManager::instance()->addPendingRemoves( &deleteChunks );

}


void RTRCInterfaceSubjectImpl::swap( std::string addChunkDBString, std::string removeChunkDBString )
{
   jccl::ConfigChunkDB addChunks;
   jccl::ConfigChunkDB deleteChunks;

   //Loading from an istream
   std::istringstream add_input (addChunkDBString);
   std::istringstream remove_input (removeChunkDBString);

   add_input >> addChunks;
   remove_input >> deleteChunks;
   
   //Send pending changes to ConfigManager
   jccl::ConfigManager::instance()->addPendingRemoves( &deleteChunks );
   jccl::ConfigManager::instance()->addPendingAdds( &addChunks );

}

const char* RTRCInterfaceSubjectImpl::getChunks() const
{
   //Lock the config manager
   //Get the list of active config chunks from config manager
   //Unlock the list
   //Compile a string of the config chunk DB
   //Return it

   std::string chalupa = "";
   return chalupa.c_str();
}

}; // namespace jccl
