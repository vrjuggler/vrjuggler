#include <vrj/Kernel/Kernel.h>
#include <gadget/RemoteInputManager/RmtMgrDepChecker.h>
#include <gadget/RemoteInputManager/RemoteInputManager.h>

namespace gadget{

bool RmtMgrDepChecker::depSatisfied(jccl::ConfigChunkPtr chunk){
   // we only pass local connections if we are already listening -- or can use a local connection
   //    to start listening (i.e. no remote input manager exists)

   bool pass = jccl::DepChecker::depSatisfied(chunk);   // Run default check

   // if we can pass normal check and have started listening on a port, we pass all chunks
   std::string iname = (std::string)chunk->getProperty("Name");
   if(vrj::Kernel::instance()->getInputManager()->getRemoteInputManager()->listenWasInitialized() == true){      
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_CRITICAL_LVL) << "RmtMgrDepChecker: passing: " << iname << std::endl << vprDEBUG_FLUSH;
      return pass;
   }

   // when remote input manager has not started listening yet:
   // we only allow local "Remote Connection" chunks to pass if no remote input manager chunk exists

   std::string chunk_type = chunk->getType();

   // Handle Connection Chunks
   if(chunk_type == "RemoteInputHost"){
      std::string hostname = chunk->getProperty("hostname");
      int port = chunk->getProperty("port");

      // so, if a remote input manager chunk exists:
      if (vrj::Kernel::instance()->getInputManager()->getRemoteInputManager()->mgrChunkExists()){
         // don't pass "Remote Connections" referring to our hostname unless listening port has been established

         // check if location has local hostname:      
         if(hostname.size() > 0){
            if(vrj::Kernel::instance()->getInputManager()->getRemoteInputManager()->hostnameMatchesLocalHostname(hostname)){
               // if we've gotten here, the listening port was not initialized (above if statement),
               // and the manager chunk exists, so we are still waiting for manager chunk to be processed.
               vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_CRITICAL_LVL) << "RmtMgrDepChecker:  Waiting for Remote Input Manger Chunk. Failed to pass local address: " << hostname <<":"<< port << std::endl << vprDEBUG_FLUSH;
               return false;
            }
         }
      }
      // if no manager chunk, pass all Remote Connections
      vprDEBUG(vrjDBG_INPUT_MGR, vprDBG_CRITICAL_LVL) << "RmtMgrDepChecker: passing Remote Connection: " << hostname <<":"<< port << std::endl << vprDEBUG_FLUSH;
      return pass;
   }
   else{  // Handle Remote Device Chunks (device or proxy with a location field)
      // Check to see if Remote Input Manager has created the required net connection yet.
      if(vrj::Kernel::instance()->getInputManager()->getRemoteInputManager()->getConnectionByAliasName(iname) != NULL)
         return pass;
      else  // still waiting for connection to be made
         return false;
   }


  //---BELOW-IS-OLD----------------------------------------------------------------
  // we used to not pass most chunks until a listening port was established.

/*
   bool pass = vjDepChecker::depSatisfied(chunk);   // Run default check
      
   // If we can pass normal check and we have initialized the RemoteInputManager,
   //   we pass all Remote Devices and Connections.
   // Remote devices that depend on connections will have an automatic dependency elsewhere
   std::string iname = (std::string)chunk->getProperty("Name");
   if(vjKernel::instance()->getInputManager()->getRemoteInputManager()->wasInitialized() == true){      
      vjDEBUG(vjDBG_INPUT_MGR, vjDBG_CRITICAL_LVL) << "RmtMgrDepChecker: passing: " << iname << std::endl << vjDEBUG_FLUSH;
      return pass;
   }

   // When Remote Manager has not been initialized yet,
   // we only allow "Remote Connection" chunks to pass if the manager chunk has been processed
   //   OR if there is no manager chunk, we allow a "Remote Connection" to pass if it actually refers to our local machine

   std::string chunk_type = chunk->getType();
   if(chunk_type == "RemoteInputHost"){
      //  -- If no RemoteInputManager chunk exists to specify a listening port, the first "Remote Connection"
      //       with our local hostname will passed in order to specify the listening port.
      if (! vjKernel::instance()->getInputManager()->getRemoteInputManager()->mgrChunkExists()){
         // no mgr chunk exists, so we need to pass a "Remote Connection" if it contains our hostname:port (i.e. a local configuration)
         std::string location = chunk->getProperty("hostname_n_port");
         if(location.size() > 0){
            if(vjKernel::instance()->getInputManager()->getRemoteInputManager()->hostnameMatchesLocalHostname(location)){
               vjDEBUG(vjDBG_INPUT_MGR, vjDBG_CRITICAL_LVL) << "RmtMgrDepChecker: passing Remote Connection: " << location << std::endl << vjDEBUG_FLUSH;
               return pass;
            }
         }
      }
      // else mgrChunkExists -- so we'll return false since we're waiting for it to be initialized
   }

   vjDEBUG(vjDBG_INPUT_MGR, vjDBG_CRITICAL_LVL) << "RmtMgrDepChecker: Not passing: " << iname << std::endl << vjDEBUG_FLUSH;
   return false;   
*/
}    


// We can handle Remote Connection Chunks and Remote Device Chunks
bool RmtMgrDepChecker::canHandle(jccl::ConfigChunkPtr chunk){
   // We can handle Remote Connection Chunk
   std::string chunk_type = chunk->getType();
   if (chunk_type == "RemoteInputHost") 
      return true;

   // Check if Remote Input Chunk (contains a location)
   std::string location = chunk->getProperty("location");
   if(location.size() == 0)  // no location specified, not handled by remote input manager 
      return false;
   //if (Kernel::instance()->getInputManager()->getRemoteInputManager()->locationContainsLocalHostname(location))
   //   return false;  // local hostname means not handled by Remote Input Manager
   else
      return true;   // remote input
}

void RmtMgrDepChecker::debugOutDependencies(jccl::ConfigChunkPtr chunk,int dbg_lvl){
   jccl::DepChecker::debugOutDependencies(chunk,dbg_lvl);

   vprDEBUG_NEXT_BEGIN(jcclDBG_CONFIG,dbg_lvl) << "Extra Dependency: Dependent upon configuring a Remote Input Manager: " << vprDEBUG_FLUSH;
   if(depSatisfied(chunk) == false)
      vprDEBUG_CONT(jcclDBG_CONFIG,dbg_lvl) << "FAILED!!!\n" << vprDEBUG_FLUSH;
   else
      vprDEBUG_CONT(jcclDBG_CONFIG,dbg_lvl) << "passed.\n" << vprDEBUG_FLUSH;


   vprDEBUG_NEXT(jcclDBG_CONFIG,dbg_lvl) << "Extra Dependencies for: item: "
                                      << chunk->getProperty("name")
                                      << " type: "
                                      << ((std::string)chunk->getType()).c_str()
                                      << std::endl
                                      << "   Dependant upon RemoteInputManager being configured in InputManager. "
                                      << vprDEBUG_FLUSH;

   vprDEBUG_NEXT_END(jcclDBG_CONFIG,dbg_lvl) << std::endl << vprDEBUG_FLUSH;
}

}  // end namespace gadget

