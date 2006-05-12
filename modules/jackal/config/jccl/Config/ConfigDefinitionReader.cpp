/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/
#include "ConfigDefinitionReader.h"
#include <sstream>
#include <fstream>
#include <vpr/Util/Debug.h>
#include <jccl/Config/ConfigDefinition.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/Config/ConfigTokens.h>

namespace jccl
{
   namespace tokens = definition_tokens;

   ConfigDefinitionReader::ConfigDefinitionReader()
   {}

   ConfigDefinitionReader::~ConfigDefinitionReader()
   {}

   std::vector<ConfigDefinitionPtr>
   ConfigDefinitionReader::read(std::istream& in)
      throw(std::runtime_error)
   {
      std::vector<ConfigDefinitionPtr> defs;

      // Load the document from the file
      cppdom::DocumentPtr doc = ElementFactory::instance()->createXMLDocument();
      try
      {
         cppdom::ContextPtr context = ElementFactory::instance()->getXMLContext();
         doc->load(in, context);

         // Verify the file format version
         std::string file_version = getFileFormatVersion(doc);
         if (file_version != tokens::DEF_VERSION)
         {
            throw std::runtime_error(std::string("Invalid version: ")+file_version+std::string(" Expected: "+tokens::DEF_VERSION));
         }

         // Get the token for this set of definitions
         cppdom::NodePtr root = doc->getChild(tokens::DEFINITION);
         if (NULL == root.get())
         {
            throw std::runtime_error(std::string("Missing root element: ")+tokens::DEFINITION);
         }
         const std::string token = root->getAttribute(tokens::NAME).getValue<std::string>();

         // Get each definition version
         cppdom::NodeList def_ver_nodes = root->getChildren(tokens::DEFINITION_VERSION);
         for (cppdom::NodeList::iterator itr = def_ver_nodes.begin();
              itr != def_ver_nodes.end();
              ++itr)
         {
            cppdom::NodePtr def_node = *itr;
            ConfigDefinitionPtr def(new ConfigDefinition(token, def_node));
            defs.push_back(def);
         }
      }
      catch (cppdom::Error& err)
      {
         cppdom::Location where(doc->getContext()->getLocation());
         const std::string errmsg = err.getStrError();

         std::ostringstream out;
         out << "XML ERROR: line " << where.getLine() << " at position "
             << where.getPos() << ": error: " << errmsg;

         // Print out where the error occurred
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, out.str()) << "\n"
            << vprDEBUG_FLUSH;

         throw std::runtime_error(out.str());
      }

      return defs;
   }

   std::vector<ConfigDefinitionPtr>
   ConfigDefinitionReader::read(const std::string& filename)
      throw(std::runtime_error)
   {
      std::ifstream in(filename.c_str());
      if (!in)
      {
         throw std::runtime_error(std::string("Failed to open file: ")+filename);
      }
      return read(in);
   }

   std::string
   ConfigDefinitionReader::getFileFormatVersion(cppdom::DocumentPtr doc)
      throw(std::runtime_error)
   {
      cppdom::NodeList pis = doc->getPiList();
      for (cppdom::NodeList::iterator itr = pis.begin(); itr != pis.end(); ++itr)
      {
         cppdom::NodePtr node = *itr;
         if (node->getName() == tokens::SETTINGS_INSTRUCTION)
         {
            if (node->hasAttribute(tokens::DEF_VERSION_ATTR))
            {
               return node->getAttribute(tokens::DEF_VERSION_ATTR).getValue<std::string>();
            }
            if (node->hasAttribute(tokens::DESC_DB_VERSION_ATTR))
            {
               return node->getAttribute(tokens::DESC_DB_VERSION_ATTR).getValue<std::string>();
            }
         }
      }
      throw std::runtime_error("Unknown file format version.");
   }

} // End of jccl namespace
