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

#ifndef _VRJ_CONFIGIOHANDLER_H_
#define _VRJ_CONFIGIOHANDLER_H_

#include <vrj/vjConfig.h>

#include <string.h>
#include <stdlib.h>

namespace vrj
{
   
class ConfigChunkDB;
class ConfigChunk;
class ChunkDescDB;
class ChunkDesc;

//: Parent interface for XMLConfigIOHandler and StandardConfigIOHandler
class ConfigIOHandler {

public:

//     ConfigIOHandler () { 
//         ;
//     }

//     virtual ~ConfigIOHandler () {
//         ;
//     }

    // ConfigChunkDB Methods

    virtual bool readConfigChunkDB (const std::string& filename, ConfigChunkDB& db) = 0;

    virtual bool readConfigChunkDB (std::istream& input, ConfigChunkDB& db) = 0;

    virtual bool writeConfigChunkDB (std::ostream& output, const ConfigChunkDB& db) = 0;

      virtual bool writeConfigChunk (std::ostream& output, const ConfigChunk& ch, const std::string& pad = "") = 0;

    // ChunkDescDB Methods

    virtual bool readChunkDescDB (const std::string& filename, ChunkDescDB& db) = 0;

    virtual bool readChunkDescDB (std::istream& input, ChunkDescDB& db) = 0;

    virtual bool writeChunkDescDB (std::ostream& output, const ChunkDescDB& db) = 0;

    virtual bool writeChunkDesc (std::ostream& output, const ChunkDesc& ch, const std::string& pad = "") = 0;


};

};
#endif
