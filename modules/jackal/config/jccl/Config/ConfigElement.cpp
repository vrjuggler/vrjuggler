/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <stdlib.h>
#include <ctype.h>
#include <boost/concept_check.hpp>
#include <cppdom/version.h>

#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/Config/ConfigDefinition.h>
#include <jccl/Config/ConfigElement.h>

namespace tokens = jccl::configuration_tokens;

// Static helper functions.
static bool hasSeparator(const std::string& path)
{
   return(path.find(char('/')) != path.npos);
}

static std::string getRemainder(const std::string& path)
{
   std::string::size_type i = path.find(char('/'));
   if ( i == path.npos )
   {
      return path;
   }
   else
   {
      return path.substr(i + 1);    // Skip the "/"
   }
}

static std::string getFirstNameComponent(const std::string& path)
{
   std::string::size_type i = path.find(char('/'));
   if ( i == path.npos )
   {
      return path;
   }
   else
   {
      return path.substr(0, i);
   }
}

namespace jccl
{

ConfigElement::ConfigElement()
{
   mValid = true;
}

ConfigElement::ConfigElement(ConfigDefinitionPtr def)
{
   mValid = true;
   setDefinition(def);
   mNode = ElementFactory::instance()->createXMLNode(); // Create a fresh node
}

ConfigElement::ConfigElement(const ConfigElement& c)
{
   mValid = true;
   *this = c;
}

ConfigElement::~ConfigElement()
{
   mValid = false;
}

bool ConfigElement::isValid() const
{
   return mValid;
}

bool ConfigElement::initFromNode(cppdom::NodePtr elementNode)
{
   bool ret_val(false);
   vprASSERT(elementNode.get() != NULL);    // Make sure we have a valid node

   // Lookup our definintion.
   std::string my_type = elementNode->getName();
   ConfigDefinitionPtr def_ptr =
      ElementFactory::instance()->getConfigDefinition(my_type);

   if(def_ptr.get() == NULL)
   {
      const unsigned int my_version =
         elementNode->getAttribute(tokens::VERSION).getValue<unsigned int>();
      vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
         << clrOutBOLD(clrYELLOW, "WARNING")
         << ": Failed to get definition for element type '" << my_type
         << "' version " << my_version
         << "\n\tYou may have an invalid configuration element.\n"
         << vprDEBUG_FLUSH;
      ret_val = false;
   }
   else
   {
      mNode = elementNode;
      mValid = true;
      setDefinition(def_ptr);
      ret_val = true;
   }
   return ret_val;
}

ConfigElement& ConfigElement::operator=(const ConfigElement& c)
{
   if (! (this == &c) )    // Different objects
   {
      mNode  = c.mNode;
      mDef   = c.mDef;
      mValid = c.mValid;
   }

   return *this;
}

bool ConfigElement::operator==(const ConfigElement& c) const
{
   // Check definitions first, then check the xml node contents
   if ( mDef.get() != c.mDef.get() )
   {
      return false;
   }

   bool is_equal = mNode->isEqual(c.mNode);
   /*
   std::ostringstream self_string, c_string;
   mNode->save(self_string);
   c.mNode->save(c_string);
   */

   return is_equal;
}

bool ConfigElement::operator!=(const ConfigElement& c) const
{
   return !(*this == c);
}

bool ConfigElement::operator<(const ConfigElement& c) const
{
   std::string s1 = getName();
   std::string s2 = c.getName();
   return (s1 < s2);
}

/**
 * This is a helper type for use with cppdom::Node::getChildPred()
 */
struct NamePred
{
   NamePred(const std::string& name)
      : mName(name)
   {}

   bool operator()(cppdom::NodePtr node)
   {
      return (node->getAttribute(tokens::NAME).getString() == mName);
   }

   std::string mName;
};

// used for dependency resolution
ConfigElementPtr ConfigElement::getChildElement(const std::string &path)
{
   std::string working_path(path), prop_token;
   cppdom::NodePtr root(mNode);
   cppdom::NodeList props;

   while ( hasSeparator(working_path) )
   {
      prop_token = getFirstNameComponent(working_path);

      // Move on to the next element in the path.
      working_path = getRemainder(working_path);

      PropertyDefinition prop_def = mDef->getPropertyDefinition(prop_token);

      // Now find the property child matching the current path element and the
      // corresponding property definition.
      props = root->getChildren(prop_token);

      if ( prop_def.getVarType() == jccl::T_CHILD_ELEMENT && ! props.empty() )
      {
         // Get the name of the child element we want.
         std::string child_element_name = getFirstNameComponent(working_path);
         working_path = getRemainder(working_path);

         // Reset root to NULL so that we can use its internal value later to
         // determine if we are done.
         root.reset();

         NamePred name_pred(child_element_name);
         cppdom::NodeList children;

         // Search the properties for a child whose "name" attribute matches
         // child_element_name.
         for ( cppdom::NodeList::iterator i = props.begin();
               i != props.end();
               ++i )
         {
            children = (*i)->getChildrenPred(name_pred);
            vprASSERT((children.empty() || children.size() <= 1) && "Element name not unique");

            if ( ! children.empty() )
            {
               root = *(children.begin());
               break;
            }
         }
      }
   }

   if ( root.get() != NULL )
   {
      ConfigElementPtr ret_node(new ConfigElement());
      ret_node->initFromNode(root);
      return ret_node;
   }
   else
   {
      return ConfigElementPtr();
   }
}

std::string ConfigElement::getName() const
{
   return mNode->getAttribute(tokens::NAME).getString();
}

std::string ConfigElement::getFullName() const
{
   cppdom::Node *element_parent, *prop_parent;
   std::string full_name(getName());

   element_parent = mNode.get();

   while ( (prop_parent = element_parent->getParent()) != NULL &&
           prop_parent->getName() != std::string("root") &&
           prop_parent->getName() != tokens::ELEMENTS )
   {
      full_name = prop_parent->getName() + jccl::EMBEDDED_SEPARATOR + full_name;
      element_parent = prop_parent->getParent();
      full_name = element_parent->getAttribute(tokens::NAME).getString() +
                  EMBEDDED_SEPARATOR + full_name;
   }

   return full_name;
}

unsigned int ConfigElement::getVersion() const
{
   return mNode->getAttribute(tokens::VERSION).getValue<unsigned int>();
}

bool ConfigElement::getProperty_bool(const std::string& prop, int ind) const
{
   const std::string prop_string = getPropertyString(prop, ind);
   if ( std::string("false") == prop_string || std::string("0") == prop_string )
   {
      return false;
   }
   else if ( std::string("true") == prop_string ||
             std::string("1") == prop_string )
   {
      return true;
   }
   else
   {
      vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
         << "Expecting boolean string for property '" << prop
         << "' in config element '" << getName() << "'. Got '"
         << prop_string << "' instead. Assuming value of true.\n"
         << vprDEBUG_FLUSH;
      return true;
   }
}

ConfigElementPtr ConfigElement::getProperty_ElementPtr(const std::string& prop,
                                                       int ind)
   const
{
   // Get the var type
   vprASSERT(mDef->getPropertyDefinition(prop).getVarType() == T_CHILD_ELEMENT);
   vprASSERT(mNode.get() != NULL);

   // Get all property children matching the given property type.
   cppdom::NodeList prop_children(mNode->getChildren(prop));

   cppdom::NodePtr embedded_node(NULL);

   // --- GET the cdata node --- //
   if(prop_children.size() > unsigned(ind))                       // If we have children in range
   {
      cppdom::NodeList::iterator property_i(prop_children.begin());
      std::advance(property_i, ind);                              // Advance to the correct property

      // Get the children of this property.  There should only be one child
      // that is the element.
      cppdom::NodeList emb_nodes = (*property_i)->getChildren();
      vprASSERT(! emb_nodes.empty() && "Empty property");
      embedded_node = *(emb_nodes.begin());
   }

   ConfigElementPtr ret_val;
   if(embedded_node.get() != NULL)
   {
      // Create a new config element refrencing the embedded node.
      ret_val = ConfigElementPtr( new ConfigElement());
      bool init_status = ret_val->initFromNode(embedded_node);
      // check for failure
      if(!init_status)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "Failed to initialize embedded config element" << std::endl
            << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "(property '" << prop << "')\n" << vprDEBUG_FLUSH;

         ret_val = ConfigElementPtr();      // Set to NULL
      }
   }

   return ret_val;
}

// This is used to sort a db by dependancy.
// - For each element ptr, add the names of the element ptrs
// - For each child element, recurse to get any contained element ptrs
std::vector<std::string> ConfigElement::getElementPtrDependencies() const
{
   std::vector<std::string> dep_list;     // Create return vector
   std::string prop_token;

   std::vector<PropertyDefinition> prop_defs = mDef->getAllPropertyDefinitions();

   //cout << "Dependency test for " << getProperty ("name") << endl;
   // For each property definition.
   for ( unsigned int i = 0; i < prop_defs.size(); ++i )
   {
      VarType var_type = prop_defs[i].getVarType();

      // If it is an element ptr or a child element.
      if ( (var_type == T_ELEMENT_PTR) || (var_type == T_CHILD_ELEMENT) )
      {
         std::string prop_token = prop_defs[i].getToken();         // Get the property name
         unsigned int num_props = getNum(prop_token);    // How many entries

         // For each sub property.
         for ( unsigned int j = 0; j < num_props; ++j )
         {
            // Just get the element names and add it.
            if(var_type == T_ELEMENT_PTR)
            {
               std::string element_name = this->getProperty<std::string>(prop_token, j);
               if ( element_name != "")
               {
                  dep_list.push_back(element_name);
               }
            }
            else if(var_type == T_CHILD_ELEMENT)   // Recurse and add
            {
               std::vector<std::string> child_deps;
               ConfigElementPtr child_element = this->getProperty<ConfigElementPtr>(prop_token, j);
               vprASSERT(child_element.get() != NULL);
               child_deps = child_element->getElementPtrDependencies();
               dep_list.insert(dep_list.end(), child_deps.begin(), child_deps.end());
            }
         }
      }
   }

   return dep_list;      // Return the list
}

std::vector<jccl::ConfigElementPtr> ConfigElement::getChildElements() const
{
   std::vector<jccl::ConfigElementPtr> embedded_list;     // Create return vector
   std::string prop_token;

   std::vector<PropertyDefinition> prop_defs = mDef->getAllPropertyDefinitions();

   //cout << "Dependency test for " << getProperty ("name") << endl;
   // For each property definition.
   for ( unsigned int i = 0; i < prop_defs.size(); ++i )
   {
      VarType var_type = prop_defs[i].getVarType();
      if ( (var_type == T_CHILD_ELEMENT) )   // If property is a child element
      {
         prop_token = prop_defs[i].getToken();         // Get the property name
         unsigned int num_props = getNum(prop_token);    // How many entries

         // For each entry.
         for ( unsigned int j = 0; j < num_props; ++j )
         {
            ConfigElementPtr child_element =
               this->getProperty<ConfigElementPtr>(prop_token, j);
            vprASSERT(child_element.get() != NULL);
            embedded_list.push_back(child_element);
         }
      }
   }

   return embedded_list;      // Return the list
}

std::ostream& operator<<(std::ostream& out, const ConfigElement& self)
{
   self.mNode->save(out);
   return out;
}

unsigned int ConfigElement::getNum(const std::string& property_token) const
{
   return mNode->getChildren(property_token).size();
}

std::string ConfigElement::getID() const
{
   return mDef->getToken();
}

bool ConfigElement::setProperty(const std::string& prop, const int ind, bool val)
{
   cppdom::NodePtr cdata_node = getPropertyCdataNode(prop, ind, true);
   vprASSERT(cdata_node.get() != NULL && "Autogrow failed");

   std::string str_val;

   if (true == val)
   {
      str_val = "true";
   }
   else
   {
      str_val = "false";
   }
   cdata_node->setCdata(str_val);

   return true;
}

bool ConfigElement::setProperty(const std::string& prop, const int ind, ConfigElementPtr val)
{
   boost::ignore_unused_variable_warning(prop);
   boost::ignore_unused_variable_warning(ind);
   boost::ignore_unused_variable_warning(val);
   vprASSERT(false && "Not implemented");
   return false;
}

void ConfigElement::setDefinition(ConfigDefinitionPtr def)
{
   vprASSERT(def.get() != NULL && "Trying to set a null definition");
   mDef = def;
}

ConfigDefinitionPtr ConfigElement::getConfigDefinition()
{
   return mDef;
}

cppdom::NodePtr ConfigElement::getNode() const
{
   return mNode;
}

/** Get the property's cdata node
* @param prop  The property token
* @param ind   The index inside the property
* @param autoCreate If true, then autocreate the property node to use
*
* @note We always autocreate the cdata node if need be
*/
cppdom::NodePtr ConfigElement::getPropertyCdataNode(const std::string& prop, int ind, bool autoCreate) const
{
   vprASSERT(mNode.get() != NULL);

   cppdom::NodePtr cdata_node(NULL);
   cppdom::NodeList prop_children(mNode->getChildren(prop));    // Get all children call prop

   // -- Autogrow for children --- //
   if( !(prop_children.size() > unsigned(ind)) && autoCreate)      // If child not in range and autocreate
   {
      PropertyDefinition prop_def = mDef->getPropertyDefinition(prop);

      while( !(prop_children.size() > unsigned(ind) ))     // While not in range
      {
         cppdom::NodePtr new_node = ElementFactory::instance()->createXMLNode();
         new_node->setName(prop);
         new_node->setCdata(prop_def.getDefaultValueString(ind));
         mNode->addChild(new_node);                      // Add Child
         prop_children = mNode->getChildren(prop);       // Get children again to check size
      }
   }

   // --- GET the cdata node --- //
   if(prop_children.size() > unsigned(ind))                                 // If we have children in range
   {
      cppdom::NodeList::iterator property_i(prop_children.begin());
      std::advance(property_i, ind);                                    // Advance to the correct child
      cdata_node = (*property_i)->getChild("cdata");        // Get the text node

      if(cdata_node.get() == NULL)     // If no cdata node, then make one
      {
         cppdom::NodePtr new_cdata_node = ElementFactory::instance()->createXMLNode();
         new_cdata_node->setName("cdata");
#if CPPDOM_VERSION_MAJOR == 0 && CPPDOM_VERSION_MINOR < 4
         new_cdata_node->setType(cppdom::xml_nt_cdata);
#else
         new_cdata_node->setType(cppdom::Node::xml_nt_cdata);
#endif
         (*property_i)->addChild(new_cdata_node);
         cdata_node = new_cdata_node;
      }

      vprASSERT(cdata_node.get() != NULL);
   }
   else
   { vprASSERT(!autoCreate && "Auto create failed"); }

   return cdata_node;
}

std::string ConfigElement::getPropertyString(const std::string& prop, int ind) const
{
   vprASSERT(mNode.get() != NULL);
   vprASSERT(tokens::NAME != prop && "Use getName() to get an element's name");

   std::string prop_string_rep("");     // The String rep to convert from
   PropertyDefinition prop_def = mDef->getPropertyDefinition(prop);

   // Ensure that a valid property was requested before taking any action.
   if ( prop_def.getNode().get() == NULL )
   {
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": [jccl::ConfigElement::getPropertyString()] Invalid property '"
         << prop << "' requested!\n" << vprDEBUG_FLUSH;
      vprASSERT(false && "Invalid property requested.");
   }
   else
   {
      // Get the cdata node without auto create
      cppdom::NodePtr cdata_node = getPropertyCdataNode(prop,ind,false);

      if(cdata_node.get() != NULL)
      {
         prop_string_rep = cdata_node->getCdata();            // Get the cdata
      }
      // Fall back on the default value.
      else
      {
         prop_string_rep = prop_def.getDefaultValueString(ind);
      }

      cppdom::NodePtr enum_child(prop_def.getNode()->getChild(definition_tokens::ENUMERATION));

      if ( enum_child.get() != NULL )
      {
         cppdom::NodeList enum_vals(enum_child->getChildren(definition_tokens::ENUM_VALUE));

         // If the property definition has an enumeration, the string in
         // prop_string_rep may be symbolic.  We need to translate it to the
         // real value before returning.
         if ( ! enum_vals.empty() )
         {
            for ( cppdom::NodeList::iterator i = enum_vals.begin();
                  i != enum_vals.end();
                  ++i )
            {
               if ( prop_string_rep == (std::string) (*i)->getAttribute(definition_tokens::LABEL) )
               {
                  vprDEBUG(jcclDBG_CONFIG, vprDBG_HVERB_LVL)
                     << "jccl::ConfigElement::getPropertyString(): Converting '"
                     << prop_string_rep << "' to '"
                     << (std::string) (*i)->getAttribute(definition_tokens::VALUE)
                     << "'\n" << vprDEBUG_FLUSH;
                  prop_string_rep =
                     (std::string) (*i)->getAttribute(definition_tokens::VALUE);
               }
            }
         }
      }
   }

   return prop_string_rep;
}

} // End of jccl namespace
