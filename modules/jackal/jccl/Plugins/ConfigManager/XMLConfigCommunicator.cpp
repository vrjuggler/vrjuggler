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

#include <jccl/JackalServer/vjXMLConfigCommunicator.h>
#include <jccl/JackalServer/vjConnect.h>
#include <jccl/JackalServer/vjNetCommunicator.h>
#include <iostream.h>
//#include <Kernel/vjConfigManager.h>
#include <jccl/XMLUtil/vjXercesXMLParserPool.h>
#include <jccl/Config/vjConfigIO.h>
#include <jccl/Config/vjXMLConfigIOHandler.h>
#include <jccl/Config/vjChunkFactory.h>

namespace jccl {

//: Constructor
vjXMLConfigCommunicator::vjXMLConfigCommunicator ():vjNetCommunicator() {
    //connection = NULL;
    config_xml_handler = (vjXMLConfigIOHandler*)vjConfigIO::instance()->getHandler ("xml_config");
    xml_parser = vjXercesXMLParserPool::instance()->getParser();
}


//: Destructor
/*virtual*/ vjXMLConfigCommunicator::~vjXMLConfigCommunicator () {
    vjConfigIO::instance()->releaseHandler (config_xml_handler);
    vjXercesXMLParserPool::instance()->releaseParser (xml_parser);
}


//: Called when a new connection is established.
//  Used to send any initial messages on a new connection.
//  Useful for any initial data queries that need to be sent
//  when a new connection is opened.  Frex, requesting the
//  current configuration of whatever's on the other side of
//  the connection.
//! PRE: _connection is open & valid for writing to; connection
//+      is NULL.
//! POST: true.
/*virtual*/ void vjXMLConfigCommunicator::initConnection(vjConnect* _connection) {
    vjNetCommunicator::initConnection (_connection);
    //connection = _connection;
}


//: Called when the connection is shut down.
//  Useful for doing things like removing the Active chunkdb/descdb
//  (at least on vjcontrol's side).
//! PRE: connection != NULL.
//! POST: connection = NULL.
/*virtual*/ void vjXMLConfigCommunicator::shutdownConnection() {
    //connection = 0;
    vjNetCommunicator::shutdownConnection();
}


//: True if the identifier represents a stream we can read.
//  The id is the token in the handler attribute of a
//  protocol tag in the EM/VjC command stream.  A given 
//  NetCommunicator can theoretically serve several handlers;
//  this is useful for backwards compatibility.
//! RETURNS: True - if self knows how to parse this stream.
//! RETURNS: False - otherwise.
/*virtual*/ bool vjXMLConfigCommunicator::acceptsStreamIdentifier (const std::string& id) {
    return !strcasecmp (id.c_str(), "xml_config");
}


//: Reads data from a communications stream.
//  This should only be called by the vjConnect object self is
//  owned by.
//  The Communicator should read data until it reaches the end of
//  the protocol stream (signified by the character string
//  "</protocol>".  readStream should read that string and
//  absolutely no further (as this can confuse parsing of later
//  protocol streams on the same connection).
//! PRE: connection != NULL;
//! RETURNS: true - if reading the protocol stream was succesful.
//! RETURNS: false - if EOF or a fatal error occurs.  This will
//+                  kill the vjConnect.
/*virtual*/ bool vjXMLConfigCommunicator::readStream (std::istream& instream, const std::string& id) {
    DOM_Node doc;
    bool retval = xml_parser->readStream (instream, doc);
    if (retval) {
        retval = interpretDOM_Node (doc);
    }
    return retval;
}


bool vjXMLConfigCommunicator::interpretDOM_Node (DOM_Node& doc) {
    bool retval = true;
    DOMString node_name = doc.getNodeName();
    DOMString node_value = doc.getNodeValue();
    char* name = node_name.transcode();
    DOM_Node child;
    DOM_NamedNodeMap attributes;
//     int attrCount;
//     int i;
    //vjConfigChunk* ch = 0;
    vjConfigChunkDB newchunkdb;
    //      cout << "ok, we've got a node named '" << name << "' with a value of '" << node_value << "'." << endl;
    
    switch (doc.getNodeType()) {
    case DOM_Node::DOCUMENT_NODE:
        cout << "document node..." << endl;
        child = doc.getFirstChild();
        while (child != 0) {
            retval = retval && (interpretDOM_Node(child));
            child = child.getNextSibling();
        }
        break;
    case DOM_Node::ELEMENT_NODE:
        cout << "command is '" << name << "'." << endl;
        if (!strcasecmp (name, "apply_chunks")) {
            // we've received a set of configchunks to apply.
            newchunkdb.erase();
            child = doc.getFirstChild();
            while (child != 0) {
                retval = retval && config_xml_handler->buildChunkDB (newchunkdb, child);
                child = child.getNextSibling();
            }
            if (retval) {
                //vjConfigManager::instance()->addChunkDB(&newchunkdb);
            }
        }
        else if (!strcasecmp (name, "remove_chunks")) {
            child = doc.getFirstChild();
            while (child != 0) {
                // i really want to just send names of chunks, but right now
                // the ConfigManager actually needs a full chunkdb :(
                retval = retval && config_xml_handler->buildChunkDB (newchunkdb, child);
                child = child.getNextSibling();
            }
            if (retval) {
                //vjConfigManager::instance()->removeChunkDB (&newchunkdb);
            }
        }
        else if (!strcasecmp (name, "remove_descs")) {
            // that could be dangerous, so we quietly refuse to honor
            // this request.
        }
        else if (!strcasecmp (name, "request_current_chunks")) {
//              vjConfigManager::instance()->lockActive();
//              vjConfigChunkDB* db = new vjConfigChunkDB((*(vjConfigManager::instance()->getActiveConfig())));   // Make a copy
//              vjConfigManager::instance()->unlockActive();
            
            //vjDEBUG(vjDBG_ENV_MGR,4) << "vjConnect: Sending (requested) chunkdb.\n" << vjDEBUG_FLUSH;
            //vjDEBUG(vjDBG_ENV_MGR,5) << *db << std::endl << vjDEBUG_FLUSH;
            connection->sendChunkDB (db, true);
            
        }
        else if (!strcasecmp (name, "request_current_descs")) {
            vjChunkDescDB* db = vjChunkFactory::instance()->getChunkDescDB();
            vjDEBUG(vjDBG_ENV_MGR,4) << "vjConnect: Sending (requested) chunkdesc.\n" << vjDEBUG_FLUSH;
            vjDEBUG(vjDBG_ENV_MGR,5) << *db << std::endl << vjDEBUG_FLUSH;
            connection->sendDescDB (db);
        }
        else {
            vjDEBUG (vjDBG_ENV_MGR,0) << "vjConnect: Unrecognized command: '"
                                      << name << "'\n" << vjDEBUG_FLUSH;
        }
        break; // ELEMENT_NODE
    default:
        cout << "confused" << endl;
        break;
    }
    return retval;
} // interpretDOM_Node ();

};
