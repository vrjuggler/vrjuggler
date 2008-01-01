/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <jccl/jcclConfig.h>

#include <fstream>
#include <sys/types.h>

#include <vpr/vpr.h>
#include <vpr/Util/FileUtils.h>

#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/Config/Configuration.h>

namespace tokens = jccl::configuration_tokens;

namespace jccl
{


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

Configuration::Configuration()
{}

Configuration::Configuration(const Configuration& db)
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

void Configuration::add(ConfigElementPtr newElement)
{
   // Before we can add newElement to the database, we have to determine
   // if there is already a element with the same name.
   std::vector<ConfigElementPtr>::iterator iter =
      std::find_if(mElements.begin(), mElements.end(),
                   ElementNamePred(newElement->getName()));

   // If no existing element has the same name as newElement, then we
   // can just add it to the end.
   if ( iter == mElements.end() )
   {
      mElements.push_back(newElement);
   }
   // Otherwise, overwrite the old version.
   else
   {
      *iter = newElement;
   }
}

/* IO functions: */

std::ostream& operator<<(std::ostream& out, const Configuration& self)
{
   cppdom::NodePtr cfg_node;

   // TODO: Find a better way of doing this, the java side seems to be able to
   // do it just fine.
   out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
   out << "<?" << tokens::SETTINGS_INSTRUCTION << " "
       << tokens::CFG_VERSION_ATTR << "=\"" << tokens::CFG_VERSION << "\"?>";

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

   cppdom::NodePtr def_path_node(cfg_node->getChild(tokens::DEFINITION_PATH));
   if ( def_path_node.get() != NULL )
   {
      self.setDefinitionPath(def_path_node);
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

      // Load the file.
      // NOTE: Loading from a stream means that we have no parent file name to
      // provide to jccl::Configuration::load().
      self.load(cfg_filename);
   }

   return in;
}

bool Configuration::load(const std::string& filename,
                         const std::string& parentfile)
{
   vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
      << "[jccl::Configuration::load()] Loading file '" << filename
      << "' with parent file '" << parentfile << "'\n" << vprDEBUG_FLUSH;

   const std::string expanded_filename = ParseUtil::expandFileName(filename,
                                                                   parentfile);

   std::ostringstream msg;
   msg << "Loading config file '" << expanded_filename << "'\n";
   vprDEBUG_OutputGuard(jcclDBG_CONFIG, vprDBG_CONFIG_LVL, msg.str(), "");

   // XXX: Previously, this used ElementFactory::createXMLDocument(), but for
   // some reason, that caused error reporting to be totally useless.
   cppdom::Document cfg_doc(cppdom::ContextPtr(new cppdom::Context()));
   bool status(false);

   try
   {
      cfg_doc.loadFile(expanded_filename);

      cppdom::NodePtr cfg_node(cfg_doc.getChild(tokens::CONFIGURATION));
      vprASSERT(cfg_node.get() != NULL);

      // Save the configuration node for later use.

      mConfigurationNode = cfg_node;

      cppdom::NodePtr def_path_node(cfg_node->getChild(tokens::DEFINITION_PATH));
      if ( def_path_node.get() != NULL )
      {
         setDefinitionPath(def_path_node);
      }

      // Go through the <include> XML elements.
      cppdom::NodeList inc_list = cfg_node->getChildren(tokens::INCLUDE);
      typedef cppdom::NodeList::iterator iter_type;
      for ( iter_type itr = inc_list.begin(); itr != inc_list.end(); ++itr )
      {
         const std::string cfg_filename = (*itr)->getCdata();
         vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
            << "Including " << cfg_filename << std::endl
            << vprDEBUG_FLUSH;

         // Load the file by making a recursive call to this method. We use
         // expanded_filename so that the fully expanded path to the including
         // file is used as the "parent" file.
         load(cfg_filename, expanded_filename);
      }

      // Load in the elements in the original file.
      loadFromElementNode(cfg_node->getChild(tokens::ELEMENTS),
                          expanded_filename);

      // This is a recursive method, and we want to set mFileName as the last
      // step of the loading process since that will end up using the
      // original file name as the configuration file.
      mFileName = expanded_filename;
      vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
         << "Configuration file name: '" << mFileName << "'" << std::endl
         << vprDEBUG_FLUSH;

      status = true;
   }
   catch (cppdom::Error& xml_e)
   {
      cppdom::Location where(cfg_doc.getContext()->getLocation());
      std::string errmsg = xml_e.getStrError();

      int line_num = where.getLine() + 1;
      int pos      = where.getPos() + 1;

      // print out where the error occured
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "Configuration XML Error:") << " "
         << expanded_filename << ": line " << line_num << " at position "
         << pos << std::endl << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << "Error: " << errmsg << std::endl << vprDEBUG_FLUSH;

      // Print out the actual failed XML.
      std::ifstream errfile(expanded_filename.c_str());
      if ( errfile )
      {
         char linebuffer[1024];
         for ( int i = 0; i < line_num && ! errfile.eof(); ++i )
         {
            errfile.getline(linebuffer, 1024);
         }

         if ( pos >= 80 )
         {
            pos %= 80;
         }

         std::string err_line(linebuffer + (where.getPos() - pos));
         if (err_line.length() >= 79)
         {
            err_line.erase(79);
         }

         std::cout << err_line << std::endl;
         std::cout << linebuffer << std::endl;

         for ( int i = 2; i < pos; ++i )
         {
            std::cout << " ";
         }

         std::cout << '^' << std::endl;
      }
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
      ElementFactory::instance()->loadDefs(vpr::replaceEnvVars((*d)->getCdata()));
   }
}

/** Load the elements from a given 'elements' tree into this configuration. */
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
         add(new_element);
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
      cfgNode->setAttribute("xmlns", tokens::CFG_NS_str);
      cfgNode->setAttribute("name", "Active Configuration");
      cfgNode->setAttribute("xmlns:xsi",
                            "http://www.w3.org/2001/XMLSchema-instance");
      cfgNode->setAttribute("xsi:schemaLocation",
                            tokens::CFG_NS_str + " " + tokens::CFG_SCHEMA);

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
