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


#include <jccl/Config/ChunkFactory.h>

namespace jccl {


    //: Adds descriptions in file 'filename' to the factory
    bool ChunkFactory::loadDescs (const std::string& filename) {
        //vjConfigIO::instance->readChunkDescDB (filename, descdb);
        bool retval = descdb.load(filename.c_str());
        if (!retval) {
            vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << "Failed to load ChunkDesc file: '" << filename.c_str() << "'.\n" << vprDEBUG_FLUSH;
        }
        return retval;
    }


   
//: Creates a Chunk using the given description
ConfigChunk* ChunkFactory::createChunk (ChunkDesc* d, bool use_defaults) {
    if (d) {
        d->assertValid();
        return new ConfigChunk (d, use_defaults);
    }
    else
        return 0;
}



//: Setup the intial environment needed for creating chunks.
//  This just loads $VJ_BASE_DIR/VJ_SHARE_DIR/Data/chunksDesc
void ChunkFactory::setupInitialEnvironment() {
    // ------ OPEN chunksDesc file ----- //
    char* vj_base_dir = getenv("JCCL_BASE_DIR");
    if(vj_base_dir == NULL) {
        vj_base_dir = "";
//         vprDEBUG(vprDBG_ALL,vprDBG_VERB_LVL) << "ChunkFactory::setupInitialEnvironment:\n" << vprDEBUG_FLUSH;
//         vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) <<  "Env var VJ_BASE_DIR not defined." << std::endl << vprDEBUG_FLUSH;
//         exit(1);
    }
    
    std::string chunk_desc_file = vj_base_dir;
    chunk_desc_file += "/";
    chunk_desc_file += JCCL_SHARE_DIR;
    chunk_desc_file += "/Data/chunksDesc";
    vprDEBUG(vprDBG_ALL,vprDBG_CONFIG_LVL) << "Loading chunk desc file: ["
                                        << chunk_desc_file << "]\n" << vprDEBUG_FLUSH;
    
    this->loadDescs(chunk_desc_file);

   }



vprSingletonImp(ChunkFactory);

};
