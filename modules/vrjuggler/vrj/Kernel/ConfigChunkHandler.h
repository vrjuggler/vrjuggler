/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_CONFIG_CHUNK_HANDLER_H_
#define _VJ_CONFIG_CHUNK_HANDLER_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>

//-----------------------------------------
//: Base class for all classes that can handle config chunks
//
// This means that they can be "configured"
// and queried using the specified interface.
//
//---------------------------------------------
//! PUBLIC_API:
class vjConfigChunkHandler
{
public:
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(vjConfigChunk* chunk)
   { return false; }

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(vjConfigChunk* chunk)
   { return false; }

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk)
   { return false; }
};

#endif
