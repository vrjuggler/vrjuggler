/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
#include <jccl/RTRC/ConfigManager.h>
#include <jccl/RTRC/ConfigCommand.h>
#include <jccl/Net/Connect.h>
#include <jccl/Net/JackalServer.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/RTRC/XMLConfigCommunicator.h>


namespace jccl
{


XMLConfigCommunicator::XMLConfigCommunicator (ConfigManager* _config_manager):
   NetCommunicator()
{
   vprASSERT (_config_manager != 0);
   mConfigManager = _config_manager;
}


/*virtual*/ XMLConfigCommunicator::~XMLConfigCommunicator ()
{
}


/*virtual*/ void XMLConfigCommunicator::initConnection(Connect* _connection)
{
   NetCommunicator::initConnection (_connection);
}


/*virtual*/ void XMLConfigCommunicator::shutdownConnection()
{
   NetCommunicator::shutdownConnection();
}


/*virtual*/ bool XMLConfigCommunicator::acceptsStreamIdentifier (const std::string& id)
{
   return !strcasecmp (id.c_str(), "xml_config");
}


/*virtual*/ bool XMLConfigCommunicator::readStream( Connect* con,
                                                    std::istream& instream )
{
   bool retval;

   try
   {
      cppdom::XMLNodePtr doc;
      cppdom::XMLContextPtr context = ChunkFactory::instance()->getXMLContext();
      doc->load( instream, context );
      retval = interpretNode( con, doc );
   }
   catch (...)
   {
      retval = false;
   }

   return retval;
}


bool XMLConfigCommunicator::interpretNode (Connect* con, cppdom::XMLNodePtr doc)
{
   bool retval = true;
   cppdom::XMLString node_name = doc->getName();
   ConfigChunkDB newchunkdb;
    //      cout << "ok, we've got a node named '" << name << "' with a value of '" << node_value << "'." << endl;

   switch (doc->getType())
   {
      case cppdom::xml_nt_document:
         {
            //cout << "document node..." << endl;
            cppdom::XMLNodeList& children = doc->getChildren();

            for ( cppdom::XMLNodeList::iterator i = children.begin();
                  i != children.end();
                  ++i )
            {
               if ( ! interpretNode(con, *i) )
               {
                  retval = false;
                  break;
               }
            }
         }

         break;
      case cppdom::xml_nt_node:
         {
            //cout << "command is '" << name << "'." << endl;
            if ( node_name == cppdom::XMLString("apply_chunks") )
            {
               // we've received a set of configchunks to apply.
               newchunkdb.vec().clear();

               cppdom::XMLNodeList& children = doc->getChildren();
               for ( cppdom::XMLNodeList::iterator i = children.begin();
                     i != children.end();
                     ++i )
               {
                  if ( ! newchunkdb.loadFromChunkDBNode(*i) )
                  {
                     retval = false;
                     break;
                  }
               }

               if (retval)
               {
                  mConfigManager->addPendingAdds(&newchunkdb);
               }
            }
            else if ( node_name == cppdom::XMLString("remove_chunks") )
            {
               cppdom::XMLNodeList& children = doc->getChildren();

               for ( cppdom::XMLNodeList::iterator i = children.begin();
                     i != children.end();
                     ++i )
               {
                  // i really want to just send names of chunks, but right now
                  // the ConfigManager actually needs a full chunkdb :(
                  if ( ! newchunkdb.loadFromChunkDBNode(*i) )
                  {
                     retval = false;
                     break;
                  }
               }

               if (retval)
               {
                  mConfigManager->addPendingRemoves (&newchunkdb);
               }
            }
            else if ( node_name == cppdom::XMLString( "remove_descs") )
            {
               // that could be dangerous, so we quietly refuse to honor
               // this request.
            }
            else if ( node_name == cppdom::XMLString("request_current_chunks") )
            {
               mConfigManager->lockActive();
               ConfigChunkDB* db = new ConfigChunkDB((*(mConfigManager->getActiveConfig())));   // Make a copy
               mConfigManager->unlockActive();

               //vprDEBUG(jcclDBG_SERVER,4) << "Connect: Sending (requested) chunkdb.\n" << vprDEBUG_FLUSH;
               //vprDEBUG(jcclDBG_SERVER,5) << *db << std::endl << vprDEBUG_FLUSH;
               con->addCommand (new CommandSendChunkDB (db, true));
            }
            else if ( node_name == cppdom::XMLString("request_current_descs") )
            {
               ChunkDescDB* db = ChunkFactory::instance()->getChunkDescDB();
               vprDEBUG(jcclDBG_SERVER,4) << "Connect: Sending (requested) chunkdesc.\n" << vprDEBUG_FLUSH;
               vprDEBUG(jcclDBG_SERVER,5) << *db << std::endl << vprDEBUG_FLUSH;
               con->addCommand (new CommandSendDescDB (db));
            }
            else
            {
               vprDEBUG (jcclDBG_SERVER,0) << "Connect: Unrecognized command: '"
                                           << node_name << "'\n"
                                           << vprDEBUG_FLUSH;
            }
         }
         break; // ELEMENT_NODE
      default:
         //cout << "confused" << endl;
         break;
   }
   return retval;
} // interpretDOM_Node ();


//----------------------- ConfigStatus Stuff ----------------------------

void XMLConfigCommunicator::configChanged ()
{
   mConfigManager->lockActive();
   ConfigChunkDB* db = new ConfigChunkDB(*(mConfigManager->getActiveConfig()));   // Make a copy
   mConfigManager->unlockActive();

   std::vector<Connect*>& connections
      = JackalServer::instance()->getConnections();
   for (unsigned int i = 0, n = connections.size(); i < n; i++)
   {
      if (connections[i]->getConnectMode() == INTERACTIVE_CONNECT)
      {
         connections[i]->addCommand (new CommandSendChunkDB (db, true));
      }
   }
   JackalServer::instance()->releaseConnections();
}


};
