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

#ifndef _VJSTANDARDCONFIGIO_H_
#define _VJSTANDARDCONFIGIO_H_

#include <vjConfig.h>

//#include <string.h>
//#include <stdlib.h>

#include <Config/vjConfigIOHandler.h>
#include <Config/vjConfigChunkDB.h>

//: Parent interface for vjXMLConfigIO and vjStandardConfigIO
class vjStandardConfigIOHandler: public vjConfigIOHandler {

public:

    vjStandardConfigIOHandler () { 
        ;
    }

    virtual ~vjStandardConfigIOHandler () {
        ;
    }

    virtual bool readConfigChunkDB (const std::string& filename, vjConfigChunkDB& db);

    virtual bool readConfigChunkDB (std::istream& input, vjConfigChunkDB& db) {
        input >> db;
        return true;
    }

    virtual bool writeConfigChunkDB (std::ostream& output, const vjConfigChunkDB& db) {
        output << db;
        return true;
    }

    virtual bool writeConfigChunk (std::ostream& output, const vjConfigChunk& ch, const std::string& pad = "") {
        output << ch;
        return true;
    }

    // ChunkDescDB Methods

    virtual bool readChunkDescDB (const std::string& filename, vjChunkDescDB& db);

    virtual bool readChunkDescDB (std::istream& input, vjChunkDescDB& db);

    virtual bool writeChunkDescDB (std::ostream& output, const vjChunkDescDB& db);

    virtual bool writeChunkDesc (std::ostream& output, const vjChunkDesc& ch, const std::string& pad = "");

};


#endif
