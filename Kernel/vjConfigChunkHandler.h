/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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


#ifndef _VJ_CONFIG_CHUNK_HANDLER_H_
#define _VJ_CONFIG_CHUNK_HANDLER_H_

#include <vjConfig.h>
class vjConfigChunk;

//-----------------------------------------
//: Abstract base class for all classes that can handle config chunks
//
// This means that they can be "configured"
// and queried using the specified interface.
//
//
// The idea is that you override canHandle()
// to recognize chunks the derived class cares about.
// Then you over ride configAdd and configRemove
// to actually process those chunks.
// It is also possible to override configProcessPending although the 
// default one will work in most cases.
//---------------------------------------------
//! PUBLIC_API:
class vjConfigChunkHandler
{
public:
   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk) = 0;

   //: Process any pending reconfiguration that we can deal with
   // This function processes each pending reconfiguration in configuration manager
   // The default implementation does the following. <br>
   // if(this->configCanHandle && passesDepCheck)
   //    configAdd/configRemove(chunk)
   // if(success)
   //   add/remove from pending
   //   remove/add from current
   //
   //! ARGS: lockIt - Should we lock the cfg_mgr pending list
   //+       defaults to true.  The only time this should be false is 
   //+       if you override this function and still want to make use of it's abilities
   //+         (ex.  The kernel needs to do this because if has to call configProcessPending on other managers in addition to itself)
   virtual void configProcessPending(bool lockIt = true);

protected:
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(vjConfigChunk* chunk) = 0;

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(vjConfigChunk* chunk) = 0;
};


#endif
