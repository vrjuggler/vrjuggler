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

#ifndef _VJXERCESXMLPARSER_H_
#define _VJXERCESXMLPARSER_H_

#include <vjConfig.h>

#include <dom/DOM.hpp>

class DOMParser;
class ErrorHandler;

//: Wrapper for libxerces-c parsers.
//  This class should never be instantiated by itself; only thru the
//  vjXercesXMLParserPool object.
class vjXercesXMLParser {

public:

    vjXercesXMLParser ();

    ~vjXercesXMLParser ();

    bool readStream (std::istream& input, DOM_Node& doc);

    bool readFile (const std::string& filename, DOM_Node& doc);

    bool writeStream (std::ostream& output, DOM_Node& doc);

    bool writeFile (const std::string& filename, DOM_Node& doc);

private:

    DOMParser *parser;

    ErrorHandler *error_handler;

};


#endif
