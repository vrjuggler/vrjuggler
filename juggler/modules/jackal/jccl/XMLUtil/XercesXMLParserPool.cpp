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


#include <jccl/XMLUtil/vjXercesXMLParserPool.h>
#include <util/PlatformUtils.hpp>

namespace jccl {

vjXercesXMLParserPool::vjXercesXMLParserPool () {
    // Initialize the XML4C2 system
    try {
        XMLPlatformUtils::Initialize();
    }
    catch(const XMLException& toCatch) {
//         cerr << "Error during Xerces-c Initialization.\n"
//              << "  Exception message:"
//              << toCatch.getMessage() << endl;
    }
}


vjXercesXMLParserPool::~vjXercesXMLParserPool () {
    XMLPlatformUtils::Terminate();
}

vjXercesXMLParser* vjXercesXMLParserPool::getParser() {
    vjXercesXMLParser* p;
    pool_lock.acquire();
    if (free_parsers.empty()) {
        p = new vjXercesXMLParser();
        used_parsers.push_back (p);
    }
    else {
        p = free_parsers[free_parsers.size()-1];
        free_parsers.pop_back();
        used_parsers.push_back (p);
    }
    pool_lock.release();
    return p;
}

void vjXercesXMLParserPool::releaseParser (vjXercesXMLParser* parser) {
    pool_lock.acquire();
    free_parsers.push_back (parser);
    used_parsers.erase (std::find (used_parsers.begin(), used_parsers.end(), parser));
    pool_lock.release();
}


vprSingletonImp (vjXercesXMLParserPool);

};
