
#ifndef _VJ_CHUNKFACTORY_H_
#define _VJ_CHUNKFACTORY_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkDescDB.h>



//------------------------------------------------------------------
//: Generator of ConfigChunks...

class vjChunkFactory {

public:
    static vjChunkDescDB *descdb;

    static void setChunkDescDB (vjChunkDescDB *_descdb) {
	descdb = _descdb;
    }

    static vjConfigChunk* createChunk (char* desctoken) {
	vjChunkDesc* desc = descdb->getChunkDesc (desctoken);
	if (desc)
	    return new vjConfigChunk (desc, descdb);
	else
	    return NULL;
    }

    static vjConfigChunk* createChunk (vjChunkDesc* d) {
	return new vjConfigChunk (d, descdb);
    }

};

#endif
