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

#ifndef _VJXERCESXMLPARSERPOOL_H_
#define _VJXERCESXMLPARSERPOOL_H_

#include <vjConfig.h>

//#include <Config/vjConfigIOHandler.h>
#include <Utils/vjSingleton.h>
#include <Utils/vjXercesXMLParser.h>


//: Singleton object which can handle multi-format reading and writing of
//+ ConfigChunkDB and ChunkDescDB files.
//  Internally, this class knows a lot of specifics about the different
//  config_io handlers.  while this isn't great in terms of modularity, if
//  we ever actually have more than 2 or 3 config io handlers, this'll be the
//  least of our problems.
class VJ_CLASS_API vjXercesXMLParserPool {

public:

    ~vjXercesXMLParserPool ();

    vjXercesXMLParser* getParser();

    void releaseParser (vjXercesXMLParser* parser);

private:

    //: Constructor - private for singleton.
    vjXercesXMLParserPool ();

    typedef std::vector<vjXercesXMLParser*> parser_v;

    parser_v free_parsers;
    parser_v used_parsers;

    vpr::Mutex pool_lock;

    vjXercesXMLParserPool(const vjXercesXMLParserPool& o) {;}
    void operator=(const vjXercesXMLParserPool& o) {;}

vjSingletonHeader(vjXercesXMLParserPool);

};

#endif
