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
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
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


    //: Adds descriptions in _descdb to the factory
    static void addDescs (vjChunkDescDB* _descdb) {
        descdb.insert (_descdb);
    }


    //: Adds descriptions in file 'filename' to the factory
    static void loadDescs (const std::string& filename) {
        descdb.load(filename.c_str());
    }


    // we actually do need this so that the EM can send the descdb to the gui...
    static vjChunkDescDB* getChunkDescDB () {
         return &descdb;
    }

    static vjChunkDesc* getChunkDesc (const std::string& token) {
        return descdb.getChunkDesc (token);
    }

    //: Creates a Chunk using the named description
    //! RETURNS: chunk - a vjConfigChunk based on a vjChunkDesc
    //+          whose token matches the argument.  If no such
    //+          vjChunkDesc is found, an "empty" vjChunkDesc,
    //+          containing only a Name vjPropertyDesc, is used.
    static vjConfigChunk* createChunk (const std::string& desctoken) {
        vjConfigChunk* ch;

        vjChunkDesc* desc = descdb.getChunkDesc (desctoken);
        if (desc)
            ch = new vjConfigChunk (desc);
        else
            ch = NULL; //return new vjConfigChunk (nulldesc);
        return ch;
    }

    //: Creates a Chunk using the given description
    static vjConfigChunk* createChunk (vjChunkDesc* d) {
        if (d)
            return new vjConfigChunk (d);
        else
            return 0;
    }

private:
    static vjChunkDescDB descdb;
    static vjChunkDesc *nulldesc;


};

#endif
