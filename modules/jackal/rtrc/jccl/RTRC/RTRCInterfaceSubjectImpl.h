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

#include <tweek/tweekConfig.h>
#include <tweek/CORBA/SubjectImpl.h>
#include <tweek/CORBA/Observer.h>
#include <jccl/RTRC/RTRCInterfaceSubject.h>
#include <jccl/RTRC/ConfigManager.h>

#include <list>
#include <sstream>

namespace jccl 
{

class ConfigChunkHandler;

/** CORBA interface subject implementation to the dynamic reconfiguration 
 *  management plugin. Use these functions to view and modify the chunks in
 *  the config manager.
 *
 *  Created: July-31-2002
 */

class JCCL_CLASS_API RTRCInterfaceSubjectImpl : public tweek::SubjectImpl
{
public:

   RTRCInterfaceSubjectImpl()
   {;}

   ~RTRCInterfaceSubjectImpl()
   {;}

   /**
    * Add a string containing an XML definition of a config chunk DB
    */
   void add( const char* chunkDBString );

   /**
    * Remove a string containing an XML definition of a config chunk DB
    */
   void remove( const char* chunkDBString );

   /** 
    * Swap in/out a string containing XML definitions of config chunk DBs
    */
   void swap( char* addChunkDBString, char* removeChunkDBString );

   /**
    * Get the current chunks in the config system
    */
   const char* getChunks() const;

private:

};

}; // namespace jccl

#endif
