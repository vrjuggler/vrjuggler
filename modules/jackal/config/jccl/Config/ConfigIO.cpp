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

#include <jccl/Config/ConfigIO.h>
#include <jccl/Config/XMLConfigIOHandler.h>
#include <jccl/Config/ParseUtil.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

namespace jccl {
   


ConfigIO::ConfigIO () {
    ;
}


ConfigIO::~ConfigIO () {
    std::vector<ConfigIOHandler*>::iterator it;
    for (it = xml_config_handlers.begin(); it != xml_config_handlers.end(); it++)
        delete (*it);
}


ConfigIOHandler* ConfigIO::getHandler () {

    if (xml_config_handlers.empty())
        return new XMLConfigIOHandler();
    else {
        ConfigIOHandler* retval = xml_config_handlers[xml_config_handlers.size()-1];
        xml_config_handlers.pop_back();
        return retval;
    }
}


void ConfigIO::releaseHandler (ConfigIOHandler* handler) {
    XMLConfigIOHandler* h = dynamic_cast<XMLConfigIOHandler*>(handler);
    if (h) {
        xml_config_handlers.push_back (h);
    }
}


//------------------ ConfigChunkDB methods ------------------


// file name should already be mangled.
bool ConfigIO::readConfigChunkDB (std::string file_name, ConfigChunkDB& db) {

    //std::string file_name = demangleFileName (_file_name);
    std::ifstream in (file_name.c_str());
    if (!in) {
        file_name = findFileUsingPathVar (in, file_name, "VJ_CFG_PATH");
        if (!in)
            return false;
    }

    // check if it's the obsolete syntax or not
    char buf[50];

    in.get(buf,50);
    if (buf[0] != '<' ) {
        vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) 
            << clrOutNORM(clrRED, "ERROR:") 
            << "File '" << file_name << "' uses obsolete (non-xml) format.\n"
            << vprDEBUG_FLUSH;
        return false;
    }


    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    db.setFileName(file_name);
    bool retval = h->readConfigChunkDB (file_name, db);
    releaseHandler (h);
    return retval;
}


//! can't guess which handler to use!!!!!!!!!!
bool ConfigIO::readConfigChunkDB (std::istream& input, ConfigChunkDB& db) {
    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    bool retval = h->readConfigChunkDB (input, db);
    releaseHandler (h);
    return retval;
}


bool ConfigIO::writeConfigChunkDB (const std::string& file_name, const ConfigChunkDB& db) {
    std::ofstream out (file_name.c_str());
    if (!out)
        return false;
    return writeConfigChunkDB (out, db);
}


bool ConfigIO::writeConfigChunkDB (std::ostream& output, const ConfigChunkDB& db) {
    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    bool retval = h->writeConfigChunkDB (output, db);
    releaseHandler (h);
    return retval;
}


bool ConfigIO::writeConfigChunk (std::ostream& output, const ConfigChunk& ch) {
    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    bool retval = h->writeConfigChunk (output, ch);
    releaseHandler (h);
    return retval;
}


bool ConfigIO::writeProperty (std::ostream& output, const Property& prop) {
    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    bool retval = h->writeProperty (output, prop);
    releaseHandler (h);
    return retval;
}


//--------------------- ChunkDescDB Methods --------------------

bool ConfigIO::readChunkDescDB (std::string file_name, ChunkDescDB& db) {

    //std::string file_name = demangleFileName (_file_name);
    std::ifstream in (file_name.c_str());
    if (!in) {
        file_name = findFileUsingPathVar (in, file_name, "VJ_CFG_PATH");
        if (!in)
            return false;
    }

    // check if it's the obsolete syntax or not
    char buf[50];
    in.get(buf,50);
    if (buf[0] != '<' ) {
        vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL) 
            << clrOutNORM(clrRED, "ERROR:") 
            << "File '" << file_name << "' uses obsolete (non-xml) format.\n"
            << vprDEBUG_FLUSH;
        return false;
    }

    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    bool retval = h->readChunkDescDB (file_name, db);
    releaseHandler (h);
    return retval;
}



bool ConfigIO::readChunkDescDB (std::istream& input, ChunkDescDB& db) {
    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    bool retval = h->readChunkDescDB (input, db);
    releaseHandler (h);
    return retval;
}


bool ConfigIO::writeChunkDescDB (const char* file_name, const ChunkDescDB& db) {
    std::ofstream out (file_name);
    if (!out)
        return false;
    return writeChunkDescDB (out, db);
}


bool ConfigIO::writeChunkDescDB (std::ostream& output, const ChunkDescDB& db) {
    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    bool retval = h->writeChunkDescDB (output, db);
    releaseHandler (h);
    return retval;
}


bool ConfigIO::writeChunkDesc (std::ostream& output, const ChunkDesc& ch) {
    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    bool retval = h->writeChunkDesc (output, ch);
    releaseHandler (h);
    return retval;
}


bool ConfigIO::writePropertyDesc (std::ostream& output, const PropertyDesc& prop) {
    ConfigIOHandler* h = getHandler();
    vprASSERT (h && "Failed to get handler.");

    bool retval = h->writePropertyDesc (output, prop);
    releaseHandler (h);
    return retval;
}



vprSingletonImp(ConfigIO);

};
