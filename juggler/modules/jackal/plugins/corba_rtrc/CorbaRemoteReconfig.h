#ifndef _JCCL_RTRC_INTERFACE_SUBJECT_IMPL_H_
#define _JCCL_RTRC_INTERFACE_SUBJECT_IMPL_H_

#include <jccl/jcclConfig.h>
#include <jccl/Net/JackalControl.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkDescDB.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/RTRC/XMLConfigCommunicator.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <jccl/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Singleton.h>

#include <tweek/CORBA/CorbaManager.h>
#include <jccl/RTRC/RTRCInterfaceSubjectImpl.h>

#include <list>
#include <sstream>

namespace jccl 
{


/** CORBA RTRC interface object. Handles configuration of the Tweek corba
 *  manager and creation of the interface subject.
 *
 *  Created: July-31-2002
 */

class JCCL_CLASS_API RTRCInterface
{
public:
   RTRCInterface()
   {;}

   ~RTRCInterface()
   {
      ;
      //TODO: Deconstruct/disconnect cleanly
   }

   /**
    *  Initialize the RTRC interface
    */
   void init();

   /**
    *  Turn on the interface to RTRC (allow incoming connections)
    */   
   void enable();

   /**
    *  Turn off the interface to RTRC (disallow incoming connections)
    */
   void disable();
   
private:

   tweek::CorbaManager* mCorbaManager;
   jccl::RTRCInterfaceSubjectImpl* mInterface;

};

}; // namespace jccl

#endif
