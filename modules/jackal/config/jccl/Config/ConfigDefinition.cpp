/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <jccl/jcclConfig.h>

#include <sstream>
#include <cppdom/cppdom.h>
#include <cppdom/predicates.h>
#include <boost/concept_check.hpp>

#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/Config/PropertyDefinition.h>
#include <jccl/Config/ConfigDefinition.h>

namespace tokens = jccl::definition_tokens;

namespace jccl
{

ConfigDefinition::ConfigDefinition()
{
   mIsValid = true;
   mNode = jccl::ElementFactory::instance()->createXMLNode();  // Set a default node
}

ConfigDefinition::ConfigDefinition(const std::string& token, cppdom::NodePtr node)
{
   mIsValid = true;
   mToken = token;
   mNode = node;  // Set a default node
}

ConfigDefinition::ConfigDefinition(const ConfigDefinition& def)
{
   mIsValid = true;
   *this = def;
}

ConfigDefinition::~ConfigDefinition()
{
   mIsValid = false;
}

std::string ConfigDefinition::getName() const
{
   // This must use cppdom::Attribute::getString() because a configuration
   // definition's name may contain spaces.
   return mNode->getAttribute(tokens::LABEL).getString();
}

std::string ConfigDefinition::getToken() const
{
   return mToken;
}

unsigned int ConfigDefinition::getVersion() const
{
   return mNode->getAttribute(tokens::VERSION).getValue<unsigned int>();
}

std::string ConfigDefinition::getHelp() const
{
   std::string help_str("");
   cppdom::NodePtr help_node = mNode->getChild(tokens::HELP);

   if ( help_node.get() != NULL )
   {
      cppdom::NodePtr help_cdata = help_node->getChild("cdata");

      if ( help_cdata.get() != NULL )
      {
         help_str = help_cdata->getCdata();
      }
   }

   return help_str;
}

bool ConfigDefinition::isParent(const std::string& token) const
{
      cppdom::NodeList parents = mNode->getChildren(tokens::PARENT);

      // Iterate over each parent
      for ( cppdom::NodeList::iterator p = parents.begin(); p != parents.end(); ++p )
      {
         const std::string p_name((*p)->getCdata());

         // Minor optimization for empty <parent> XML nodes.  There may be a
         // nicer way of doing this via the cppdom::Node interface.
         if ( p_name == std::string("") )
         {
            continue;
         }

         if(p_name == token)
         {
            return true;
         }

         vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
            << "[ConfigDefinition::isParent()] Checking parent '"
            << p_name << "' for property '" << token << "'\n"
            << vprDEBUG_FLUSH;

         ConfigDefinitionPtr parent_def =
            ElementFactory::instance()->getConfigDefinition(p_name);

         if ( parent_def.get() != NULL )
         {
            return (parent_def->isParent(token));
         }
         else
         {
            vprDEBUG(jcclDBG_CONFIG, vprDBG_WARNING_LVL)
               << "[ConfigDefinition::isParent()] WARNING: "
               << "Failed to find parent of '" << this->getName()
               << "' named '" << p_name << "'\n" << vprDEBUG_FLUSH;
         }
      }

      // We failed to find the requested parent token.
      return false;
}

PropertyDefinition ConfigDefinition::getPropertyDefinition(const std::string& token) const
{
   cppdom::HasAttributeValuePredicate attrib_pred(tokens::NAME, token);
   cppdom::NodeList prop_defs = mNode->getChildrenPred(attrib_pred);

   vprASSERT((prop_defs.size() < 2) && "Have multiple properties of same token name");
   if(!prop_defs.empty())
   {
      return PropertyDefinition(*prop_defs.begin()); // Return the property def
   }
   // If we do not have the definition for the requested property, check our
   // parent(s).
   else
   {
      cppdom::NodeList parents = mNode->getChildren(tokens::PARENT);

      // Iterate over each parent until we find the property definition the
      // caller requested.
      for ( cppdom::NodeList::iterator p = parents.begin(); p != parents.end(); ++p )
      {
         const std::string p_name((*p)->getCdata());

         // Minor optimization for empty <parent> XML nodes.  There may be a
         // nicer way of doing this via the cppdom::Node interface.
         if ( p_name == std::string("") )
         {
            continue;
         }

         vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
            << "[ConfigDefinition::getPropertyDefinition()] Checking parent '"
            << p_name << "' for property '" << token << "'\n"
            << vprDEBUG_FLUSH;

         ConfigDefinitionPtr parent_def =
            ElementFactory::instance()->getConfigDefinition(p_name);

         if ( parent_def.get() != NULL )
         {
            PropertyDefinition prop_def =
               parent_def->getPropertyDefinition(token);

            // We found the property definition we need.
            if ( prop_def.getNode().get() != NULL )
            {
               return prop_def;
            }
         }
         else
         {
            vprDEBUG(jcclDBG_CONFIG, vprDBG_WARNING_LVL)
               << "[ConfigDefinition::getPropertyDefinition()] WARNING: "
               << "Failed to find parent of '" << this->getName()
               << "' named '" << p_name << "'\n" << vprDEBUG_FLUSH;
         }
      }

      // We failed to find the requested property definition.
      return PropertyDefinition(cppdom::NodePtr(NULL));
   }
}

std::vector<PropertyDefinition> ConfigDefinition::getAllPropertyDefinitions() const
{
   std::vector<PropertyDefinition> ret_val;

   cppdom::NodeList parents = mNode->getChildren(tokens::PARENT);

   // Iterate over each parent grabbing all of their property definitions.
   for ( cppdom::NodeList::iterator p = parents.begin(); p != parents.end(); ++p )
   {
      const std::string p_name((*p)->getCdata());

      // Minor optimization for empty <parent> XML nodes.  There may be a
      // nicer way of doing this via the cppdom::Node interface.
      if ( p_name == std::string("") )
      {
         continue;
      }

      vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
         << "[ConfigDefinition::getAllPropertyDefinitions()] "
         << "grabbing all properties for parent'" << p_name
         << std::endl << vprDEBUG_FLUSH;

      ConfigDefinitionPtr parent_def =
         ElementFactory::instance()->getConfigDefinition(p_name);

      if ( parent_def.get() != NULL )
      {
         std::vector<PropertyDefinition> parent_props =
            parent_def->getAllPropertyDefinitions();
         
         ret_val.insert(ret_val.end(), parent_props.begin(), parent_props.end());
      }
      else
      {
         vprDEBUG(jcclDBG_CONFIG, vprDBG_WARNING_LVL)
            << "[ConfigDefinition::getAllPropertyDefinitions()] WARNING: "
            << "Failed to find parent of '" << this->getName()
            << "' named '" << p_name << "'\n" << vprDEBUG_FLUSH;
      }
   }

   // Grab our own property definitions.
   cppdom::NodeList prop_defs = mNode->getChildren(tokens::PROPERTY);

   for(cppdom::NodeList::iterator i=prop_defs.begin(); i!= prop_defs.end(); ++i)
   {
      ret_val.push_back(PropertyDefinition(*i));
   }

   return ret_val;
}

std::ostream& operator<<(std::ostream& out, const ConfigDefinition& self)
{
   self.mNode->save(out);
   return out;
}

//:equality operator
// a little stricter than it needs to be.. it shouldn't care about the order of
// property definitions...
bool ConfigDefinition::operator== (const ConfigDefinition& d) const
{
   bool is_equal(false);
   is_equal = mNode->isEqual(d.mNode);
   /*
   std::ostringstream self_string, d_string;
   mNode->save(self_string);
   d.mNode->save(d_string);
   */

   return is_equal;
}

} // End of jccl namespace
