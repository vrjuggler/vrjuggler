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


#ifndef _VJ_PROXY_H_
#define _VJ_PROXY_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>
class vjInput;

//: Base class for all proxies
//!PUBLIC_API:
class vjProxy
{
public:
   vjProxy() {;}

   //: Configure the proxy
   //! POST:
   //+  Proxy is configured
   //+  Proxy is registered with input manager
   //! RETURNS: success
   virtual bool config(vjConfigChunk* chunk) = 0;


   //: Return a pointer to the base class of the devices being proxied
   virtual vjInput* getProxiedInputDevice() = 0;

   //: Returns the string rep of the chunk type used to config this device
   // Used by input manager to find chunks that construct devices
   static std::string getChunkType() { return "Undefined"; }
};

#endif
