/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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
#include <Kernel/vjDebug.h>


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
        vjConfigChunk* ch;

        vjChunkDesc* desc = descdb.getChunkDesc (desctoken);
        if (desc)
            ch = new vjConfigChunk (desc);
        else
            ch = NULL; //return new vjConfigChunk (nulldesc);
        return ch;
    }

    //: Creates a Chunk using the given description
    vjConfigChunk* createChunk (vjChunkDesc* d) {
        if (d)
            return new vjConfigChunk (d);
        else
            return 0;
    }

protected:
   // Setup the intial environment needed for creating chunks
   void setupInitialEnvironment()
   {
      // ------ OPEN chunksDesc file ----- //
      char* vj_base_dir = getenv("VJ_BASE_DIR");
      if(vj_base_dir == NULL)
      {
         vjDEBUG(vjDBG_ERROR,0) << "vjChunkFactory::setupInitialEnvironment: Env var VJ_BASE_DIR not defined." << endl << vjDEBUG_FLUSH;
         exit(1);
      }
   
      char chunk_desc_file[250];
      strcpy(chunk_desc_file, vj_base_dir);
      strcat(chunk_desc_file, "/Data/chunksDesc");
      vjDEBUG(vjDBG_ALL,0) << "vjChunkFactory::setupInitialEnvironment: Loading chunk description file: ["
                           << chunk_desc_file << "]\n" << vjDEBUG_FLUSH;

      vjChunkDescDB* cfg_desc = new vjChunkDescDB;
      if (!cfg_desc->load(chunk_desc_file))
      {
         vjDEBUG(vjDBG_ERROR,0) << "ERROR: vjChunkFactory::setupInitialEnvironment: Config Desc failed to load file: " << endl << vjDEBUG_FLUSH;
         exit(1);
      }
      this->addDescs(cfg_desc);
   
   }

private:
   vjChunkFactory() : nulldesc(NULL)
   {
      setupInitialEnvironment();
   }

private:
    vjChunkDescDB descdb;
    vjChunkDesc *nulldesc;

public:
   //: Get instance of singleton object
   static vjChunkFactory* instance()
   {
      if (_instance == NULL)
         _instance = new vjChunkFactory;
      return _instance;
   }

private:
   static vjChunkFactory* _instance;   //: The instance

};

#endif
