
#ifndef _JCCL_CHUNKDESCPTR_H_
#define _JCCL_CHUNKDESCPTR_H_

#include <boost/smart_ptr.hpp>


namespace jccl {

    class ChunkDesc;

    typedef boost::shared_ptr<ChunkDesc> ChunkDescPtr;

};

#endif
