/*
 *  File:	    $RCSfile$
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
	if (!descdb)
	    return NULL;
	return descdb->getChunkDesc (token);
    }

    //: Creates a Chunk using the named description
    //! RETURNS: chunk - a vjConfigChunk based on a vjChunkDesc
    //+          whose token matches the argument.  If no such
    //+          vjChunkDesc is found, an "empty" vjChunkDesc,
    //+          containing only a Name vjPropertyDesc, is used.
    static vjConfigChunk* createChunk (const std::string& desctoken) {
	vjConfigChunk* ch;
	//cout << " createChunk with val " << flush << desctoken << endl;

	if (!descdb)
	    return NULL;

	vjChunkDesc* desc = descdb->getChunkDesc (desctoken);
	//cout <<" chunkdesc is " << flush << *desc << endl;
	if (desc)
	    ch = new vjConfigChunk (desc);
	else
	    ch = NULL; //return new vjConfigChunk (nulldesc);
	//cout << " chunk is :\n" << flush << *ch << endl;
	return ch;
    }

    //: Creates a Chunk using the given description
    static vjConfigChunk* createChunk (vjChunkDesc* d) {
	if (d)
	    return new vjConfigChunk (d);
	else
	    return NULL;
    }

private:
    static vjChunkDescDB *descdb;
    static vjChunkDesc *nulldesc;


};

#endif
