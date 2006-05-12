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

#include <jccl/jcclConfig.h>

#include <sstream>

#include <vpr/vpr.h>
#include <vpr/Util/Assert.h>

#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ElementFactory.h>
#include <jccl/Config/PropertyDefinition.h>
#include <jccl/Util/Debug.h>

namespace tokens = jccl::definition_tokens;

namespace jccl
{

PropertyDefinition::PropertyDefinition()
{
   mIsValid = true;
   mNode = ElementFactory::instance()->createXMLNode();
   // cppdom::NodePtr(new cppdom::Node);  // Set a default node
}

PropertyDefinition::PropertyDefinition(cppdom::NodePtr node)
{
   mIsValid = true;
   mNode = node;
}

PropertyDefinition::PropertyDefinition(const PropertyDefinition& d)
{
   mIsValid = true;
   *this = d;
}

PropertyDefinition::~PropertyDefinition()
{
   mIsValid = false;
}

std::string PropertyDefinition::getToken() const
{
   return mNode->getAttribute(tokens::NAME).getValue<std::string>();
}

std::string PropertyDefinition::getName() const
{
   /// @todo Fix this when the file format changes to support it
   return getToken();
}

std::string PropertyDefinition::getHelp() const
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

VarType PropertyDefinition::getVarType() const
{
   const std::string str(mNode->getAttribute(tokens::VALUETYPE));

   if (str == jccl::types_tokens::INTEGER)
   {
      return T_INT;
   }
   if (str == jccl::types_tokens::FLOAT)
   {
      return T_FLOAT;
   }
   if (str == jccl::types_tokens::BOOLEAN)
   {
      return T_BOOL;
   }
   if (str == jccl::types_tokens::STRING)
   {
      return T_STRING;
   }
   if (str == jccl::types_tokens::CONFIGELEMENTPOINTER)
   {
      return T_ELEMENT_PTR;
   }
   if (str == jccl::types_tokens::CONFIGELEMENT)
   {
      return T_CHILD_ELEMENT;
   }

   return VJ_T_INVALID;
}

int PropertyDefinition::getNumAllowed() const
{
   return mNode->getChildren(tokens::VALUE).size();
}

std::string PropertyDefinition::getDefaultValueString(int index)
{
   std::string value_str("");

   if ( mNode.get() != NULL )
   {
      cppdom::NodeList children(mNode->getChildren(tokens::VALUE));

      if ( unsigned(index) < children.size() )
      {
         cppdom::NodeList::iterator i(children.begin());
         std::advance(i, index);
         value_str = (*i)->getAttribute(tokens::DEFAULTVALUE).getString();
      }
   }

   return value_str;
}

std::ostream& operator<< (std::ostream& out, const PropertyDefinition& self)
{
   self.mNode->save(out);
   return out;
}


//: Equality Operator
// BUG (IPTHACK) - doesn't check equality of enumerations and valuelabels
bool PropertyDefinition::operator== (const PropertyDefinition& pd) const
{
   std::ostringstream self_string, d_string;
   mNode->save(self_string);
   pd.mNode->save(d_string);

   return (self_string == d_string);
}

bool PropertyDefinition::operator!= (const PropertyDefinition& pd) const
{
   return !(*this == pd);
}

cppdom::NodePtr PropertyDefinition::getNode()
{
   return mNode;
}

} // End of jccl namespace
