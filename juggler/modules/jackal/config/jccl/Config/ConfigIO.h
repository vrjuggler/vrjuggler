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

#ifndef _JCCL_CONFIGIO_H_
#define _JCCL_CONFIGIO_H_

#include <jccl/jcclConfig.h>

#include <jccl/Config/ConfigIOHandler.h>
#include <vpr/Util/Singleton.h>

namespace jccl {
   
//: Singleton object which can handle multi-format reading and writing of
//+ ConfigChunkDB and ChunkDescDB files.
//  Internally, this class knows a lot of specifics about the different
//  config_io handlers.  while this isn't great in terms of modularity, if
//  we ever actually have more than 2 or 3 config io handlers, this'll be the
//  least of our problems.
class ConfigIO {

public:

    ~ConfigIO ();

    // valid strings are "standard_config" and "xml_config"
    ConfigIOHandler* getHandler (const std::string& handler_name);

    // puts handler back in the pool.
    void releaseHandler (ConfigIOHandler* handler);


    //---------- ConfigChunkDB methods -----------

    //: Read db from the named file.
    //  If handler_name is "", we use a heuristic to determine which
    //  kind of IO handler to use for reading the file.
    bool readConfigChunkDB (std::string file_name, ConfigChunkDB& db, const std::string& handler_name = "");


    //: Read db from the stream in.
    //  If handler_name is "", uses the default handler class.
    //  Note that we can't guess which handler to use because we can't
    //  back up the stream by an arbitrary amount.
    bool readConfigChunkDB (std::istream& input, ConfigChunkDB& db, const std::string& handler_name = "");


    //: Write db to the named file.
    //  If handler_name is "", uses the default handler class.
    bool writeConfigChunkDB (const char* file_name, const ConfigChunkDB& db, const std::string& handler_name = "");


    //: Write db to output.
    //  If handler_name is "", uses the default handler class.
    bool writeConfigChunkDB (std::ostream& output, const ConfigChunkDB& db, const std::string& handler_name = "");


    //---------- ChunkDescDB methods -----------

    //: Read db from the named file.
    //  If handler_name is "", we use a heuristic to determine which
    //  kind of IO handler to use for reading the file.
    bool readChunkDescDB (std::string file_name, ChunkDescDB& db, const std::string& handler_name = "");


    //: Read db from the stream in.
    //  If handler_name is "", uses the default handler class.
    //  Note that we can't guess which handler to use because we can't
    //  back up the stream by an arbitrary amount.
    bool readChunkDescDB (std::istream& input, ChunkDescDB& db, const std::string& handler_name = "");


    //: Write db to the named file.
    //  If handler_name is "", uses the default handler class.
    bool writeChunkDescDB (const char* file_name, const ChunkDescDB& db, const std::string& handler_name = "");


    //: Write db to output.
    //  If handler_name is "", uses the default handler class.
    bool writeChunkDescDB (std::ostream& output, const ChunkDescDB& db, const std::string& handler_name = "");


private:

    //: Constructor - private for singleton.
    ConfigIO ();

    //: pool of io handlers.
    std::vector<ConfigIOHandler*> xml_config_handlers;
    ConfigIOHandler* standard_config_handler;

    //: Name of default handler for reading/writing streams.
    std::string default_handler_name;

    //: String name to use for getting an XML handler.
    static const std::string xml_handler_name;

    //: String name to use for getting an old-fasioned io handler.
    static const std::string standard_handler_name;

vprSingletonHeader(ConfigIO);

};

};
#endif
