
#ifndef _JCCL_CONFIGCHUNKPTR_H_
#define _JCCL_CONFIGCHUNKPTR_H_

#include <boost/smart_ptr.hpp>


namespace jccl {

    class ConfigChunk;

    typedef boost::shared_ptr<ConfigChunk> ConfigChunkPtr;

};

#endif
