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


#include <jccl/jcclConfig.h>

#include <jccl/Config/vjConfigIO.h>
#include <jccl/Config/vjStandardConfigIOHandler.h>
#include <jccl/Config/vjXMLConfigIOHandler.h>
#include <jccl/Config/vjParseUtil.h>

namespace jccl {
   

/*static*/ const std::string ConfigIO::xml_handler_name ("xml_config");
/*static*/ const std::string ConfigIO::standard_handler_name ("standard_config");


ConfigIO::ConfigIO (): default_handler_name(xml_handler_name) {
    standard_config_handler = 0;
}


ConfigIO::~ConfigIO () {
    std::vector<ConfigIOHandler*>::iterator it;
    for (it = xml_config_handlers.begin(); it != xml_config_handlers.end(); it++)
        delete (*it);
}


ConfigIOHandler* ConfigIO::getHandler (const std::string& handler_name) {

    if (handler_name == xml_handler_name) {
        if (xml_config_handlers.empty())
            return new XMLConfigIOHandler();
        else {
            ConfigIOHandler* retval = xml_config_handlers[xml_config_handlers.size()-1];
            xml_config_handlers.pop_back();
            return retval;
        }
    }
    else if (handler_name == standard_handler_name) {
        // all methods are reentrant, so we only ever need one instance
        // create on demand, cuz eventually we won't need it at all.
        if (!standard_config_handler)
            standard_config_handler = new StandardConfigIOHandler();
        return standard_config_handler;
    }
    else
        return 0;
}


void ConfigIO::releaseHandler (ConfigIOHandler* handler) {
    XMLConfigIOHandler* h = dynamic_cast<XMLConfigIOHandler*>(handler);
    if (h) {
        xml_config_handlers.push_back (h);
    }
    // nothing to do for standard io handler.
}


//------------------ ConfigChunkDB methods ------------------


// file name should already be mangled.
bool ConfigIO::readConfigChunkDB (std::string file_name, ConfigChunkDB& db,
                                    const std::string& handler_name /*= ""*/) {

    //std::string file_name = demangleFileName (_file_name);
    std::ifstream in (file_name.c_str());
    if (!in) {
        file_name = findFileUsingPathVar (in, file_name, "VJ_CFG_PATH");
        if (!in)
            return false;
    }

    ConfigIOHandler* h;
    if (handler_name == "") {
        char buf[50];
        readString (in, buf, 50, false);
        if (buf[0] == '<' )
            h = getHandler (xml_handler_name);
        else
            h = getHandler (standard_handler_name);
    }
    else 
        h = getHandler (handler_name);
    if (!h)
        return false;
    db.setFileName(file_name);
    bool retval = h->readConfigChunkDB (file_name, db);
    releaseHandler (h);
    return retval;
}


//! can't guess which handler to use!!!!!!!!!!
bool ConfigIO::readConfigChunkDB (std::istream& input, ConfigChunkDB& db, const std::string& handler_name /*= ""*/) {
    ConfigIOHandler* h;
    if (handler_name == "")
        h = getHandler (default_handler_name);
    else
        h = getHandler (handler_name);
    if (!h)
        return false;
    bool retval = h->readConfigChunkDB (input, db);
    releaseHandler (h);
    return retval;
}


bool ConfigIO::writeConfigChunkDB (const char* file_name, const ConfigChunkDB& db, const std::string& handler_name /*= ""*/) {
    std::ofstream out (file_name);
    if (!out)
        return false;
    return writeConfigChunkDB (out, db, handler_name);
}


bool ConfigIO::writeConfigChunkDB (std::ostream& output, const ConfigChunkDB& db, const std::string& handler_name /*= ""*/) {
    ConfigIOHandler* h;
    if (handler_name == "")
        h = getHandler (default_handler_name);
    else
        h = getHandler (handler_name);
    if (!h) {
        std::cout << "Couldn't find the output handler for '" + handler_name + "'\n" << std::flush;
        return false;
    }
    bool retval = h->writeConfigChunkDB (output, db);
    releaseHandler (h);
    return retval;
}


//--------------------- ChunkDescDB Methods --------------------

bool ConfigIO::readChunkDescDB (std::string file_name, ChunkDescDB& db,
                                    const std::string& handler_name /*= ""*/) {

    //std::string file_name = demangleFileName (_file_name);
    std::ifstream in (file_name.c_str());
    if (!in) {
        file_name = findFileUsingPathVar (in, file_name, "VJ_CFG_PATH");
        if (!in)
            return false;
    }

    ConfigIOHandler* h;
    if (handler_name == "") {
        char buf[50];
        readString (in, buf, 50, false);
        if (buf[0] == '<' ) 
            h = getHandler (xml_handler_name);
        else
            h = getHandler (standard_handler_name);
    }
    else 
        h = getHandler (handler_name);
    if (!h)
        return false;
    bool retval = h->readChunkDescDB (file_name, db);
    releaseHandler (h);
    return retval;
}


//! can't guess which handler to use!!!!!!!!!!
bool ConfigIO::readChunkDescDB (std::istream& input, ChunkDescDB& db, const std::string& handler_name /*= ""*/) {
    ConfigIOHandler* h;
    if (handler_name == "")
        h = getHandler (default_handler_name);
    else
        h = getHandler (handler_name);
    if (!h)
        return false;
    bool retval = h->readChunkDescDB (input, db);
    releaseHandler (h);
    return retval;
}


bool ConfigIO::writeChunkDescDB (const char* file_name, const ChunkDescDB& db, const std::string& handler_name /*= ""*/) {
    std::ofstream out (file_name);
    if (!out)
        return false;
    return writeChunkDescDB (out, db, handler_name);
}


bool ConfigIO::writeChunkDescDB (std::ostream& output, const ChunkDescDB& db, const std::string& handler_name /*= ""*/) {
    ConfigIOHandler* h;
    if (handler_name == "")
        h = getHandler (default_handler_name);
    else
        h = getHandler (handler_name);
    if (!h)
        return false;
    bool retval = h->writeChunkDescDB (output, db);
    releaseHandler (h);
    return retval;
}



vprSingletonImp(ConfigIO);

};
