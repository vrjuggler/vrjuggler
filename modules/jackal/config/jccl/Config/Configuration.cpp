/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <sys/types.h>

#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/Config/Configuration.h>

namespace tokens = jccl::configuration_tokens;

namespace jccl
{

Configuration::Configuration()
{}

Configuration::Configuration(Configuration& db)
{
   *this = db;
}

Configuration::~Configuration()
{}

const std::string& Configuration::getFileName() const
{
   return mFileName;
}

void Configuration::setFileName(const std::string& fname)
{
   mFileName = fname;
}

ConfigElementPtr Configuration::get(const std::string& name) const
{
   for ( std::vector<ConfigElementPtr>::const_iterator i = mElements.begin();
         i != mElements.end();
         ++i )
   {
      if ( name == (*i)->getName() )
      {
         return *i;
      }
   }
   return ConfigElementPtr();
}

void Configuration::getByType(const std::string& typeName,
                              std::vector<ConfigElementPtr>& elements) const
{

   for ( std::vector<ConfigElementPtr>::const_iterator i = mElements.begin();
         i != mElements.end();
         ++i )
   {
      if ( typeName == (*i)->getID() )
      {
         elements.push_back(*i);
      }
   }
}

bool Configuration::remove(const std::string& name)
{
   for ( std::vector<ConfigElementPtr>::iterator i = mElements.begin();
         i != mElements.end();
         ++i )
   {
      if ( name == (*i)->getName() )
      {
         mElements.erase(i);
         return true;
      }
   }
   return false;
}

/* IO functions: */

std::ostream& operator<<(std::ostream& out, const Configuration& self)
{
   cppdom::NodePtr cfg_node;
   
   // TODO: Find a better way of doing this, the java side seems to be able to
   // do it just fine.
   out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
   out << "<?org-vrjuggler-jccl-settings configuration.version=\"3.0\"?>";
   
   self.createConfigurationNode(cfg_node);
   cfg_node->save(out);
   return out;
}

std::istream& operator>>(std::istream& in, Configuration& self)
{
   // Create a new XML document
   cppdom::DocumentPtr cfg_doc(ElementFactory::instance()->createXMLDocument());
   cppdom::ContextPtr context_ptr = cfg_doc->getContext();
   
   // Load the document from the input stream.
   cfg_doc->load(in, context_ptr);

   // Get the configuration node.
   cppdom::NodePtr cfg_node(cfg_doc->getChild(tokens::CONFIGURATION));
   self.setConfigurationNode(cfg_node);

   // Load in the elements from the confiruration node.
   self.loadFromElementNode(cfg_node->getChild(tokens::ELEMENTS));

   // XXX: The following would work, but are not currently being used.
   
   /*cppdom::NodePtr def_path_node(cfg_doc->getChild(tokens::DEFINITION_PATH));
   if ( def_path_node.get() != NULL )
   {
      setDefinitionPath(def_path_node);
   }
   */

   // Go through the <include> XML elements.
   /*
   cppdom::NodeList inc_list = cfg_node->getChildren(tokens::INCLUDE);
   for ( cppdom::NodeList::iterator itr = inc_list.begin();
         itr != inc_list.end();
         ++itr )
   {
      std::string cfg_filename = (*itr)->getCdata();
      vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
         << "Including " << cfg_filename << std::endl
         << vprDEBUG_FLUSH;

      // Load the file
      load(cfg_filename, filename);
   }
   */

   return in;
}

bool Configuration::load(const std::string& filename, const std::string& parentfile)
{
   vprDEBUG_OutputGuard(jcclDBG_CONFIG, vprDBG_CONFIG_LVL,
                        std::string("Loading config file ")+filename+std::string("\n"),
                        std::string("\n"));

   std::string absolute_filename = demangleFileName(filename, parentfile);
   mFileName = absolute_filename;

   cppdom::DocumentPtr cfg_doc(ElementFactory::instance()->createXMLDocument());
   bool status(false);

   try
   {
      cfg_doc->loadFile(absolute_filename);

      cppdom::NodePtr cfg_node(cfg_doc->getChild(tokens::CONFIGURATION));
      vprASSERT(cfg_node.get() != NULL);

      // Save the configuration node for later use.
      
      mConfigurationNode = cfg_node;

      cppdom::NodePtr def_path_node(cfg_doc->getChild(tokens::DEFINITION_PATH));
      if ( def_path_node.get() != NULL )
      {
         setDefinitionPath(def_path_node);
      }

      // Go through the <include> XML elements.
      cppdom::NodeList inc_list = cfg_node->getChildren(tokens::INCLUDE);
      for ( cppdom::NodeList::iterator itr = inc_list.begin();
            itr != inc_list.end();
            ++itr )
      {
         std::string cfg_filename = (*itr)->getCdata();
         vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
            << "Including " << cfg_filename << std::endl
            << vprDEBUG_FLUSH;

         // Load the file
         load(cfg_filename, filename);
      }

      // Load in the elements in the original file.
      loadFromElementNode(cfg_node->getChild(tokens::ELEMENTS), filename);

      status = true;
   }
   catch (cppdom::Error& xml_e)
   {
      cppdom::Location where(cfg_doc->getContext()->getLocation());
      std::string errmsg = xml_e.getStrError();

      // print out where the error occured
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "Configuration: XMLError:") << mFileName
         << ": line " << where.getLine() << " at position " << where.getPos()
         << ": error: " << errmsg.c_str() << std::endl << vprDEBUG_FLUSH;
   }

   return status;
}

bool Configuration::save(const std::string& fname) const
{
   cppdom::NodePtr cfg_node;
   createConfigurationNode(cfg_node);                        // Get base configuration element
   cppdom::DocumentPtr cfg_doc(new cppdom::Document);        // Put in in a document
   cfg_doc->addChild(cfg_node);
   cfg_doc->saveFile(fname);                                 // Write out the document
   return true;
}

std::vector<jccl::ConfigElementPtr>& Configuration::vec()
{
   return mElements;
}

void Configuration::setDefinitionPath(cppdom::NodePtr defPathNode)
{
   mDefsPath.clear();
   extendDefinitionPath(defPathNode);
}

void Configuration::setConfigurationNode(cppdom::NodePtr cfgNode)
{
   mConfigurationNode = cfgNode;
}


void Configuration::extendDefinitionPath(cppdom::NodePtr defPathNode)
{
   cppdom::NodeList dirs = defPathNode->getChildren(tokens::DIR);
   for (cppdom::NodeList::iterator d = dirs.begin(); d != dirs.end(); ++d)
   {
      mDefsPath.push_back((*d)->getCdata());
   }
}

/**
 * This is a helper for use with std::find_if() for comparing element names.
 */
struct ElementNamePred
{
   ElementNamePred(const std::string& name)
      : mName(name)
   {}

   bool operator()(jccl::ConfigElementPtr element)
   {
      return (element->getName() == mName);
   }

   std::string mName;
};

/** Load the elements from a given <elements> tree into this configuration. */
bool Configuration::loadFromElementNode(cppdom::NodePtr elementsNode,
                                        const std::string& currentFile)
{
   if(elementsNode->getName() != tokens::ELEMENTS)
   {
      vprASSERT(false && "Trying to load a elements tree that is not an <elements> node");
      return false;
   }

   for (cppdom::NodeList::iterator cur_child = elementsNode->getChildren().begin();
        cur_child != elementsNode->getChildren().end();
        ++cur_child)
   {
      ConfigElementPtr new_element(new ConfigElement());            // New element
      bool init_status = new_element->initFromNode(*cur_child);     // Initialize it

      // Make sure that there were no problems creating the new element
      // (ex: invalid definition).
      if(!init_status)
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
            << "  Config element load problem in file: " << currentFile
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
            << "  Skipping element: " << (*cur_child)->getAttribute(tokens::NAME).getString()
            << std::endl << vprDEBUG_FLUSH;
      }
      else
      {
         // Before we can add new_element to the database, we have to determine
         // if there is already a element with the same name.
         std::vector<ConfigElementPtr>::iterator iter =
            std::find_if(mElements.begin(), mElements.end(),
                         ElementNamePred(new_element->getName()));

         // If no existing element has the same name as new_element, then we
         // can just add it to the end.
         if ( iter == mElements.end() )
         {
            mElements.push_back(new_element);
         }
         // Otherwise, overwrite the old version.
         else
         {
            *iter = new_element;
         }
      }
   }

   return true;
}

void Configuration::createConfigurationNode(cppdom::NodePtr& cfgNode) const
{
   if (NULL == mConfigurationNode.get())
   {
      cfgNode = ElementFactory::instance()->createXMLNode();
      cfgNode->setName(tokens::CONFIGURATION);
      cfgNode->setAttribute("xmlns", "http://www.vrjuggler.org/jccl/xsd/3.0/configuration");
      cfgNode->setAttribute("name", "Active Configuration");
      cfgNode->setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
      cfgNode->setAttribute("xsi:schemaLocation", "http://www.vrjuggler.org/jccl/xsd/3.0/configuration http://www.vrjuggler.org/jccl/xsd/3.0/configuration.xsd");

      cppdom::NodePtr elements_node = ElementFactory::instance()->createXMLNode();
      elements_node->setName(tokens::ELEMENTS);

      std::vector<ConfigElementPtr>::const_iterator cur_element;

      for (cur_element = mElements.begin();
           cur_element != mElements.end();
           ++cur_element)
      {
         cppdom::NodePtr child_node = (*cur_element)->getNode();
         elements_node->addChild(child_node);
      }

      cfgNode->addChild(elements_node);
   }
   else
   {
      cfgNode = mConfigurationNode;
   }
}

} // End of jccl namespace
