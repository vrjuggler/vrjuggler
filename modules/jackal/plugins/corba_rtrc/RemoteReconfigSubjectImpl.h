#ifndef _JCCL_RTRC_INTERFACE_SUBJECT_IMPL_H_
#define _JCCL_RTRC_INTERFACE_SUBJECT_IMPL_H_

#include <jccl/jcclConfig.h>
#include <jccl/Net/JackalControl.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ChunkDescDB.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/RTRC/XMLConfigCommunicator.h>
#include <jccl/Util/Debug.h>

#include <tweek/tweekConfig.h>
#include <tweek/CORBA/SubjectImpl.h>
#include <tweek/CORBA/Observer.h>

#include <jccl/RTRC/RTRCInterfaceSubject.h>
#include <tweek/CORBA/SubjectImpl.h>

#include <jccl/RTRC/ConfigManager.h>

#include <list>
#include <sstream>

namespace jccl 
{

/** CORBA interface subject implementation to the dynamic reconfiguration 
 *  management plugin. Use these functions to view and modify the chunks in
 *  the config manager.
 *
 *  Created: July-31-2002
 */

class JCCL_CLASS_API RTRCInterfaceSubjectImpl : public POA_RTRCInterface::RTRCInterfaceSubject,
                                                public tweek::SubjectImpl
{
public:

   RTRCInterfaceSubjectImpl()
   {;}

   ~RTRCInterfaceSubjectImpl()
   {;}

   /**
    * Add a string containing an XML definition of a config chunk DB
    */
   virtual void add( const char* chunkDBString );

   /**
    * Remove a string containing an XML definition of a config chunk DB
    */
   virtual void remove( const char* chunkDBString );

   /** 
    * Swap in/out a string containing XML definitions of config chunk DBs
    */
   virtual void swap( const char* addChunkDBString, const char* removeChunkDBString );

   /**
    * Get the current chunks in the config system
    */
   virtual char* getChunks();

//private:

};

}; // namespace jccl

#endif
