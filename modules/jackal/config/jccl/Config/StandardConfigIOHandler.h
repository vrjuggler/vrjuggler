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

#ifndef _JCCL_STANDARDCONFIGIO_H_
#define _JCCL_STANDARDCONFIGIO_H_

#include <jccl/jcclConfig.h>

#include <jccl/Config/ConfigIOHandler.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ConfigChunk.h>

namespace jccl
{
   
//: Parent interface for XMLConfigIO and StandardConfigIO
class StandardConfigIOHandler: public ConfigIOHandler {

public:

    StandardConfigIOHandler () { 
        ;
    }

    virtual ~StandardConfigIOHandler () {
        ;
    }

    virtual bool readConfigChunkDB (const std::string& filename, ConfigChunkDB& db);

    virtual bool readConfigChunkDB (std::istream& input, ConfigChunkDB& db) {
        input >> db;
        return true;
    }

    virtual bool writeConfigChunkDB (std::ostream& output, const ConfigChunkDB& db) {
        output << db;
        return true;
    }

    virtual bool writeConfigChunk (std::ostream& output, const ConfigChunk& ch, const std::string& pad = "") {
        output << ch;
        return true;
    }

    // ChunkDescDB Methods

    virtual bool readChunkDescDB (const std::string& filename, ChunkDescDB& db);

    virtual bool readChunkDescDB (std::istream& input, ChunkDescDB& db);

    virtual bool writeChunkDescDB (std::ostream& output, const ChunkDescDB& db);

    virtual bool writeChunkDesc (std::ostream& output, const ChunkDesc& ch, const std::string& pad = "");

};

};
#endif
