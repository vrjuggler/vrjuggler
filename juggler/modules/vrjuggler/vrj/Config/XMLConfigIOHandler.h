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

#ifndef _VJXMLCONFIGIOHANDLER_H_
#define _VJXMLCONFIGIOHANDLER_H_

#include <vjConfig.h>
#include <Config/vjConfigIOHandler.h>

#include <util/PlatformUtils.hpp>
#include <parsers/DOMParser.hpp>
#include <framework/XMLFormatter.hpp>
#include <dom/DOM.hpp>
#include <string.h>
#include <stdlib.h>

#include <Config/vjConfigChunkDB.h>
//#include <Config/vjChunkFactory.h>

//: Config IO handler for the new XML-based cfgfile format.
//  This particular class is dependent on the Xerces representation of
//  DOM_Nodes.  (It should probably be renamed vjXercesXMLConfigIOHandler
//  but that's a little long).
class vjXMLConfigIOHandler: public vjConfigIOHandler {

public:

    vjXMLConfigIOHandler ();

    virtual ~vjXMLConfigIOHandler ();

    //----------------- ConfigChunkDB Methods --------------------------

    virtual bool readConfigChunkDB (const std::string& filename, vjConfigChunkDB& db);

    virtual bool readConfigChunkDB (std::istream& input, vjConfigChunkDB& db);

    virtual bool writeConfigChunkDB (std::ostream& output, const vjConfigChunkDB& db);

    virtual bool writeConfigChunk (std::ostream& output, const vjConfigChunk& ch, const std::string& pad);

    virtual bool writeConfigChunk (XMLFormatter* formatter, const vjConfigChunk& ch, const std::string& pad);

    bool buildChunkDB (vjConfigChunkDB& db, const DOM_Node& doc);

    bool buildProperty (vjConfigChunk* ch, const DOM_Node& doc, bool use_defaults = true);

    vjConfigChunk* buildConfigChunk (const DOM_Node& doc, bool use_defaults = true);

private:

    bool writeProperty (XMLFormatter* formatter, const vjProperty& p, const std::string& pad);

    bool parseTextValues (vjProperty* p, int& startval, char* text);


    //--------------------- ChunkDescDB Methods -----------------------------

public:

    virtual bool readChunkDescDB (const std::string& filename, vjChunkDescDB& db);

    virtual bool readChunkDescDB (std::istream& input, vjChunkDescDB& db);

    virtual bool writeChunkDescDB (std::ostream& output, const vjChunkDescDB& db);

    virtual bool writeChunkDesc (std::ostream& output, const vjChunkDesc& ch, const std::string& pad = "");


    bool buildChunkDescDB (vjChunkDescDB& db, const DOM_Node& doc);
    vjChunkDesc* buildChunkDesc (const DOM_Node& doc);

private:

    bool writeChunkDesc (XMLFormatter* f, const vjChunkDesc& ch, const std::string& pad);
    bool writePropertyDesc (XMLFormatter* f, const vjPropertyDesc& p, const std::string& pad);
    bool parseChunkDescChildElement (vjChunkDesc& desc, const DOM_Node doc);
    bool parsePropertyDescChildElement (vjPropertyDesc &p, const DOM_Node doc);


    //------------------ private parser functions ------------------------

private:

    char* stripQuotes (char* ch);
    
    char* stringTokenizer (char* newstring, char*& tmp);

    void writeBuf (XMLFormatter* formatter, const char* s, XMLFormatter::EscapeFlags flags = XMLFormatter::NoEscapes);

    void writeBuf (XMLFormatter* formatter, const std::string& s, XMLFormatter::EscapeFlags flags = XMLFormatter::NoEscapes);


};


#endif
