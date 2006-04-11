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

#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ChunkDesc.h>
#include <jccl/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ChunkFactory.h>

#include <sstream>


namespace jccl
{

PropertyDesc::PropertyDesc ()
{
   mIsValid = true;
   mNode = ChunkFactory::instance()->createXMLNode(); // cppdom::NodePtr(new cppdom::Node);  // Set a default node
}

PropertyDesc::PropertyDesc(cppdom::NodePtr node)
{
   mIsValid = true;
   mNode = node;
}

PropertyDesc::PropertyDesc (const PropertyDesc& d)
{
   mIsValid = true;
   *this = d;
}

/*
PropertyDesc::PropertyDesc (const std::string& n, int i, VarType t,
                            const std::string& h)
{
   validation = 1;
   name = n;
   token = n;
   help = h;
   num = i;
   type = t;
   enum_val = 0;
}
*/

PropertyDesc::~PropertyDesc ()
{
   mIsValid = false;
}

#ifdef JCCL_DEBUG
void PropertyDesc::assertValid () const
{
   vprASSERT (mIsValid == true && "Trying to use deleted PropertyDesc");
}
#endif

std::string PropertyDesc::getHelp () const
{
   std::string help_str("");

   cppdom::NodePtr help_node = mNode->getChild(jccl::help_TOKEN);

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

void PropertyDesc::setHelp (const std::string& help)
{
   cppdom::NodePtr help_node = mNode->getChild(jccl::help_TOKEN);

   // If this chunk description does not have a <help> child, create one.
   if ( help_node.get() == NULL )
   {
      help_node = jccl::ChunkFactory::instance()->createXMLNode();
      help_node->setName(jccl::help_TOKEN);
      mNode->addChild(help_node);
      vprASSERT(mNode->getChild(jccl::help_TOKEN).get() != NULL && "Node addition failed");
   }

   cppdom::NodePtr help_cdata = help_node->getChild("cdata");

   if ( help_cdata.get() == NULL )
   {
      help_cdata = jccl::ChunkFactory::instance()->createXMLNode();
      help_cdata->setName("cdata");
      help_cdata->setType(cppdom::xml_nt_cdata);
      help_node->addChild(help_cdata);
      vprASSERT(help_node->getChild("cdata").get() != NULL && "CDATA addition failed");
   }

   help_cdata->setCdata(help);
}

std::string PropertyDesc::getDefaultValueString(int index)
{
   std::string value_str("");

   if ( mNode.get() != NULL )
   {
      cppdom::NodeList children(mNode->getChildren(item_TOKEN));

      if ( unsigned(index) < children.size() )
      {
         cppdom::NodeList::iterator i(children.begin());
         std::advance(i, index);
         value_str = (*i)->getAttribute(default_value_TOKEN).getString();
      }
   }

   return value_str;
}

/*
void PropertyDesc::appendValueLabel (const std::string& _label)
{
   valuelabels.push_back (new EnumEntry (_label, T_STRING));
}

// used as a possible return value below.
static const std::string PropertyDesc_empty_string ("");

const std::string& PropertyDesc::getValueLabel (unsigned int i) const
{
   assertValid();

   if ( i < valuelabels.size() )
   {
      return valuelabels[i]->getName();
   }
   else
   {
      return PropertyDesc_empty_string;
   }
}

void PropertyDesc::appendEnumeration (const std::string& _label,
                                      const std::string& _value)
{
   VarValue *v;
   // this is slightly kludgey.  We make a varvalue to store the enumeration
   // value... except for T_CHUNK and T_EMBEDDEDCHUNK where we store a chunk
   // name type...
   if ( (type == T_CHUNK) || (type == T_EMBEDDEDCHUNK) )
   {
      v = new VarValue (T_STRING);
   }
   else
   {
      v = new VarValue (type);
   }

   if ( type == T_STRING || type == T_CHUNK ||
        type == T_EMBEDDEDCHUNK )
   {
      *v = (_value == "")?_label : _value;
   }
   else
   {
      if ( _value == "" )
      {
         *v = enum_val;
      }
      else
      {
         *v = _value;
      }
      // *v = (_value == "")?enum_val:_value;
   }
   enum_val++;
   enumv.push_back (new EnumEntry (_label, *v));
}

EnumEntry* PropertyDesc::getEnumEntry (const std::string& s) const
{
   assertValid();

   for ( unsigned int i = 0; i < enumv.size(); i++ )
   {
      if ( !vjstrcasecmp (enumv[i]->getName(), s) )
      {
         return enumv[i];
      }
   }
   return NULL;
}

EnumEntry* PropertyDesc::getEnumEntryAtIndex (unsigned int index) const
{
   assertValid();

   if ( enumv.size() > index )
   {
      return enumv[index];
   }
   else
   {
      return NULL;
   }
}

EnumEntry* PropertyDesc::getEnumEntryWithValue (const VarValue& val) const
{
   assertValid();

   for ( unsigned int i = 0; i < enumv.size(); i++ )
   {
      if ( enumv[i]->getValue() == val )
      {
         return enumv[i];
      }
   }

   return NULL;
}
*/

std::ostream& operator << (std::ostream& out, const PropertyDesc& self)
{
   self.assertValid();
   self.mNode->save(out);
   return out;
}


//: Equality Operator
// BUG (IPTHACK) - doesn't check equality of enumerations and valuelabels
bool PropertyDesc::operator== (const PropertyDesc& pd) const
{
   assertValid();
   pd.assertValid();

   std::ostringstream self_string, d_string;
   mNode->save(self_string);
   pd.mNode->save(d_string);

   return (self_string == d_string);
}

} // End of jccl namespace
