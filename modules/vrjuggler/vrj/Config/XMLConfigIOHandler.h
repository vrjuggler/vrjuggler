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

#ifndef _VRJ_XMLCONFIGIOHANDLER_H_
#define _VRJ_XMLCONFIGIOHANDLER_H_

#include <vrj/vrjConfig.h>
#include <vrj/Config/ConfigIOHandler.h>

#include <util/PlatformUtils.hpp>
#include <parsers/DOMParser.hpp>
#include <framework/XMLFormatter.hpp>
#include <dom/DOM.hpp>
#include <string.h>
#include <stdlib.h>

#include <vrj/Config/ConfigChunkDB.h>
//#include <vrj/Config/ChunkFactory.h>

namespace vrj
{

//: Config IO handler for the new XML-based cfgfile format.
//  This particular class is dependent on the Xerces representation of
//  DOM_Nodes.  (It should probably be renamed XercesXMLConfigIOHandler
//  but that's a little long).
class XMLConfigIOHandler: public ConfigIOHandler {

public:

    XMLConfigIOHandler ();

    virtual ~XMLConfigIOHandler ();

    //----------------- ConfigChunkDB Methods --------------------------

    virtual bool readConfigChunkDB (const std::string& filename, ConfigChunkDB& db);

    virtual bool readConfigChunkDB (std::istream& input, ConfigChunkDB& db);

    virtual bool writeConfigChunkDB (std::ostream& output, const ConfigChunkDB& db);

    virtual bool writeConfigChunk (std::ostream& output, const ConfigChunk& ch, const std::string& pad);

    virtual bool writeConfigChunk (XMLFormatter* formatter, const ConfigChunk& ch, const std::string& pad);

    bool buildChunkDB (ConfigChunkDB& db, const DOM_Node& doc);

    bool buildProperty (ConfigChunk* ch, const DOM_Node& doc, bool use_defaults = true);

    ConfigChunk* buildConfigChunk (const DOM_Node& doc, bool use_defaults = true);

private:

    bool writeProperty (XMLFormatter* formatter, const Property& p, const std::string& pad);

    bool parseTextValues (Property* p, int& startval, char* text);


    //--------------------- ChunkDescDB Methods -----------------------------

public:

    virtual bool readChunkDescDB (const std::string& filename, ChunkDescDB& db);

    virtual bool readChunkDescDB (std::istream& input, ChunkDescDB& db);

    virtual bool writeChunkDescDB (std::ostream& output, const ChunkDescDB& db);

    virtual bool writeChunkDesc (std::ostream& output, const ChunkDesc& ch, const std::string& pad = "");


    bool buildChunkDescDB (ChunkDescDB& db, const DOM_Node& doc);
    ChunkDesc* buildChunkDesc (const DOM_Node& doc);

private:

    bool writeChunkDesc (XMLFormatter* f, const ChunkDesc& ch, const std::string& pad);
    bool writePropertyDesc (XMLFormatter* f, const PropertyDesc& p, const std::string& pad);
    bool parseChunkDescChildElement (ChunkDesc& desc, const DOM_Node doc);
    bool parsePropertyDescChildElement (PropertyDesc &p, const DOM_Node doc);


    //------------------ private parser functions ------------------------

private:

    char* stripQuotes (char* ch);

    char* stringTokenizer (char* newstring, char*& tmp);

    void writeBuf (XMLFormatter* formatter, const char* s, XMLFormatter::EscapeFlags flags = XMLFormatter::NoEscapes);

    void writeBuf (XMLFormatter* formatter, const std::string& s, XMLFormatter::EscapeFlags flags = XMLFormatter::NoEscapes);


};


};
#endif
