
#ifndef _VJ_CHUNKFACTORY_H_
#define _VJ_CHUNKFACTORY_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkDescDB.h>



//------------------------------------------------------------------
//: Generator of ConfigChunks...

class vjChunkFactory {

public:

    static void setChunkDescDB (vjChunkDescDB *_descdb) {
	descdb = _descdb;
	if (!nulldesc)
	    nulldesc = new vjChunkDesc();
    }

    static vjChunkDesc* getChunkDesc (char* token) {
	return descdb->getChunkDesc (token);
    }

    static vjConfigChunk* createChunk (char* desctoken) {
	vjChunkDesc* desc = descdb->getChunkDesc (desctoken);
	if (desc)
	    return new vjConfigChunk (desc, descdb);
	else
	    return new vjConfigChunk (nulldesc, descdb);
    }

    static vjConfigChunk* createChunk (vjChunkDesc* d) {
	return new vjConfigChunk (d, descdb);
    }

private:
    static vjChunkDescDB *descdb;
    static vjChunkDesc *nulldesc;


};

#endif
