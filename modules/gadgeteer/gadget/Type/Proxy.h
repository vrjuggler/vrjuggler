#ifndef _VJ_PROXY_H_
#define _VJ_PROXY_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>

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

   //: Returns the string rep of the chunk type used to config this device
   // Used by input manager to find chunks that construct devices
   static std::string getChunkType() { return "Undefined"; }
};

#endif
