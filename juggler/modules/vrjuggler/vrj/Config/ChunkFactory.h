
#ifndef _VJ_CHUNKFACTORY_H_
#define _VJ_CHUNKFACTORY_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkDescDB.h>



//------------------------------------------------------------------
//: Generator of ConfigChunks...
//
//        The notion of embedded chunks complicated the configuration
//        system - suddenly a chunk needs to be able to find an
//        arbitrary vjChunkDesc in order to instantiate embedded chunks,
//        which may themselves embed chunks.
//        We needed a simpler way to generate vjConfigChunks on-the-fly
//        inside Juggler apps.  The static vjChunkFactory is a way to
//        do that.  Note that it relies on the notion that there will be
//        only one vjChunkDescDB in the Juggler app, and that it gets
//        told what it is.
//
// @author  Christopher Just
// February 1999
//------------------------------------------------------------------

class vjChunkFactory {

public:

    static void setChunkDescDB (vjChunkDescDB *_descdb) {
	descdb = _descdb;
	if (!nulldesc)
	    nulldesc = new vjChunkDesc();
    }


    static vjChunkDescDB* getChunkDescDB () {
	return descdb;
    }

    static vjChunkDesc* getChunkDesc (const std::string& token) {
	return descdb->getChunkDesc (token);
    }

    //: Creates a Chunk using the named description
    //! RETURNS: chunk - a vjConfigChunk based on a vjChunkDesc
    //+          whose token matches the argument.  If no such
    //+          vjChunkDesc is found, an "empty" vjChunkDesc,
    //+          containing only a Name vjPropertyDesc, is used.
    static vjConfigChunk* createChunk (const std::string& desctoken) {
	vjChunkDesc* desc = descdb->getChunkDesc (desctoken);
	if (desc)
	    return new vjConfigChunk (desc);
	else
	    return new vjConfigChunk (nulldesc);
    }

    //: Creates a Chunk using the given description
    static vjConfigChunk* createChunk (vjChunkDesc* d) {
	return new vjConfigChunk (d);
    }

private:
    static vjChunkDescDB *descdb;
    static vjChunkDesc *nulldesc;


};

#endif
