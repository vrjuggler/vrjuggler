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

#ifndef _VRJ_CHUNKFACTORY_H_
#define _VRJ_CHUNKFACTORY_H_

#include <vrj/vrjConfig.h>
#include <vrj/Config/ConfigChunk.h>
#include <vrj/Config/ChunkDescDB.h>
#include <vrj/Util/Debug.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Util/Singleton.h>

namespace vrj
{


//------------------------------------------------------------------
//: Generator of ConfigChunks...  (singleton)
//
//        The notion of embedded chunks complicated the configuration
//        system - suddenly a chunk needs to be able to find an
//        arbitrary ChunkDesc in order to instantiate embedded chunks,
//        which may themselves embed chunks.
//        We needed a simpler way to generate ConfigChunks on-the-fly
//        inside Juggler apps.  The singleton ChunkFactory is a way to
//        do that.  Note that it relies on the notion that there will be
//        only one ChunkDescDB in the Juggler app, and that it gets
//        told what it is.
//
// @author  Christopher Just
// February 1999
//------------------------------------------------------------------

class ChunkFactory {

public:
    //: Adds descriptions in _descdb to the factory
    void addDescs (ChunkDescDB* _descdb) {
        descdb.insert (_descdb);
    }


    //: Adds descriptions in file 'filename' to the factory
    void loadDescs (const std::string& filename) {
        //vjConfigIO::instance->readChunkDescDB (filename, descdb);
        if (!descdb.load(filename.c_str()))
            vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) <<  "Failed to load descriptions file.\n" << vprDEBUG_FLUSH;

    }


    // we actually do need this so that the EM can send the descdb to the gui...
    ChunkDescDB* getChunkDescDB () {
         return &descdb;
    }

    ChunkDesc* getChunkDesc (const std::string& token) {
        return descdb.getChunkDesc (token);
    }

    //: Creates a Chunk using the named description
    //! RETURNS: chunk - a ConfigChunk based on a ChunkDesc
    //+          whose token matches the argument.  If no such
    //+          ChunkDesc is found, an "empty" ChunkDesc,
    //+          containing only a Name PropertyDesc, is used.
    ConfigChunk* createChunk (const std::string& desctoken, bool use_defaults = true) {
        return createChunk (descdb.getChunkDesc (desctoken), use_defaults);
    }

    //: Creates a Chunk using the given description
    ConfigChunk* createChunk (ChunkDesc* d, bool use_defaults = true);


protected:


    //: Setup the intial environment needed for creating chunks
    //  This just loads $VJ_BASE_DIR/VJ_SHARE_DIR/data/vrj-chunks.desc
    void setupInitialEnvironment();


private:
   ChunkFactory()
   {
      setupInitialEnvironment();
   }

private:
    ChunkDescDB descdb;

vprSingletonHeader(ChunkFactory);
/*
public:
   //: Get instance of singleton object
   static ChunkFactory* instance()
   {
      if(_instance == NULL)                     // First check
      {
         vpr::Guard<vpr::Mutex> guard(_inst_lock);    // Serial critical section
         if (_instance == NULL)                 // Second check
            _instance = new ChunkFactory;
      }
      vprASSERT(_instance != NULL && "vjChunkFactory has NULL _instance");
      return _instance;
   }

private:
   static ChunkFactory* _instance;   //: The instance
   static vpr::Mutex _inst_lock;
   */
};
};
#endif
