/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/



#ifndef _VJ_CHUNKFACTORY_H_
#define _VJ_CHUNKFACTORY_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkDescDB.h>
#include <Kernel/vjDebug.h>
#include <Sync/vjMutex.h>
#include <Utils/vjSingleton.h>



//------------------------------------------------------------------
//: Generator of ConfigChunks...  (singleton)
//
//        The notion of embedded chunks complicated the configuration
//        system - suddenly a chunk needs to be able to find an
//        arbitrary vjChunkDesc in order to instantiate embedded chunks,
//        which may themselves embed chunks.
//        We needed a simpler way to generate vjConfigChunks on-the-fly
//        inside Juggler apps.  The singleton vjChunkFactory is a way to
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
    void addDescs (vjChunkDescDB* _descdb) {
        descdb.insert (_descdb);
    }


    //: Adds descriptions in file 'filename' to the factory
    void loadDescs (const std::string& filename) {
        descdb.load(filename.c_str());
    }


    // we actually do need this so that the EM can send the descdb to the gui...
    vjChunkDescDB* getChunkDescDB () {
         return &descdb;
    }

    vjChunkDesc* getChunkDesc (const std::string& token) {
        return descdb.getChunkDesc (token);
    }

    //: Creates a Chunk using the named description
    //! RETURNS: chunk - a vjConfigChunk based on a vjChunkDesc
    //+          whose token matches the argument.  If no such
    //+          vjChunkDesc is found, an "empty" vjChunkDesc,
    //+          containing only a Name vjPropertyDesc, is used.
    vjConfigChunk* createChunk (const std::string& desctoken) {
        return createChunk (descdb.getChunkDesc (desctoken));
    }

    //: Creates a Chunk using the given description
    vjConfigChunk* createChunk (vjChunkDesc* d);


protected:

    //: Setup the intial environment needed for creating chunks.
    //  This just means that we load VJ_BASE_DIR/VJ_SHARE_DIR/Data/chunksDesc.
    void setupInitialEnvironment();


private:

    //: Constructor.  Calls setupInitialEnvironment.
    vjChunkFactory() {
        setupInitialEnvironment();
    }


    vjChunkDescDB descdb;


    vjSingletonHeader(vjChunkFactory);

/*
public:
   //: Get instance of singleton object
   static vjChunkFactory* instance()
   {
      if(_instance == NULL)                     // First check
      {
         vjGuard<vjMutex> guard(_inst_lock);    // Serial critical section
         if (_instance == NULL)                 // Second check
            _instance = new vjChunkFactory;
      }
      vjASSERT(_instance != NULL && "vjChunkFactory has NULL _instance");
      return _instance;
   }

private:
   static vjChunkFactory* _instance;   //: The instance
   static vjMutex _inst_lock;
   */
};

#endif
