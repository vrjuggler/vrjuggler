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

#include <sstream>
#include <cppdom/cppdom.h>
#include <cppdom/predicates.h>

#include <vpr/Util/Assert.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/Config/PropertyDesc.h>
#include <jccl/Config/ChunkDesc.h>

namespace jccl
{

ChunkDesc::ChunkDesc ()
{
   mIsValid = true;
   mNode = jccl::ChunkFactory::instance()->createXMLNode();  // Set a default node
}

ChunkDesc::ChunkDesc(cppdom::NodePtr node)
{
   mIsValid = true;
   mNode = node;  // Set a default node
}

ChunkDesc::ChunkDesc (const ChunkDesc& desc)
{
   mIsValid = true;
   *this = desc;
}

ChunkDesc::~ChunkDesc()
{
   mIsValid = false;
}

#ifdef JCCL_DEBUG
void ChunkDesc::assertValid () const
{
   vprASSERT (mIsValid == true && "Trying to use deleted ChunkDesc");
}
#endif


void ChunkDesc::setName (const std::string& name)
{
   assertValid();
   mNode->setAttribute(jccl::name_TOKEN, name);
}

void ChunkDesc::setToken (const std::string& token)
{
   assertValid();
   mNode->setAttribute(jccl::token_TOKEN, token);
}

void ChunkDesc::setHelp (const std::string& help)
{
   assertValid();

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

std::string ChunkDesc::getName () const
{
   assertValid();

   // This must use cppdom::Attribute::getString() because a chunk
   // description's name may contain spaces.
   return mNode->getAttribute(jccl::name_TOKEN).getString();
}

std::string ChunkDesc::getToken() const
{
   assertValid();
   return mNode->getAttribute(jccl::token_TOKEN).getValue<std::string>();
}

std::string ChunkDesc::getHelp () const
{
   assertValid();

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

void ChunkDesc::add (PropertyDesc pd)
{
   assertValid();

   cppdom::NodePtr pd_node = pd.getNode();
   mNode->addChild(pd_node);
}

/*
void ChunkDesc::setDefaultChunk (DOM_Node* n)
{
   default_node = new DOM_Node;
   *default_node = *n;
   default_chunk.reset();
}
*/

/*
void ChunkDesc::setDefaultChunk (ConfigChunkPtr ch)
{
   default_chunk = ch;
}
*/

/*
ConfigChunkPtr ChunkDesc::getDefaultChunk() const
{
   // thread safety???
   if ( (default_chunk.get() == 0) && (default_node != 0) )
   {
      XMLConfigIOHandler* handler = (XMLConfigIOHandler*)ConfigIO::instance()->getHandler();
      ConfigChunkPtr ch = handler->buildConfigChunk (*default_node, false);
      if ( ch.get() )
      {
         // this is a cheat.  and ugly cuz we have to get the real pointer,
         // not the shared_ptr, and then const_cast it :(
         (const_cast<ChunkDesc*>(&(*this)))->default_chunk = ch;
      }
      ConfigIO::instance()->releaseHandler (handler);
   }
//      if (default_chunk)
//          std::cout << "returning a default chunk: " << *default_chunk << std::endl;
//      else
//          std::cout << "getDefaultChunk return null" << std::endl;
   return default_chunk;
}
*/

PropertyDesc ChunkDesc::getPropertyDesc(const std::string& token) const
{
   assertValid();

   cppdom::HasAttributeValuePredicate attrib_pred("token", token);
   cppdom::NodeList prop_descs = mNode->getChildrenPred(attrib_pred);

   vprASSERT((prop_descs.size() < 2) && "Have multiple properties of same token name");
   if(!prop_descs.empty())
   {
      return PropertyDesc( (*prop_descs.begin()) ); // Return the property desc
   }
   else
   {
      return PropertyDesc( cppdom::NodePtr(NULL) ); // Failed to find
   }
}

std::vector<PropertyDesc> ChunkDesc::getAllPropertyDesc() const
{
   assertValid();

   std::vector<PropertyDesc> ret_val;
   cppdom::NodeList prop_descs = mNode->getChildren();

   for(cppdom::NodeList::iterator i=prop_descs.begin(); i!= prop_descs.end(); i++)
   {
      ret_val.push_back( PropertyDesc(*i) );
   }

   return ret_val;
}


bool ChunkDesc::remove (const std::string& _token)
{
   vprASSERT(false && "ChunkDesc::remove: Not implemented yet");
   return false;
}

JCCL_IMPLEMENT(std::ostream&) operator << (std::ostream& out,
                                           const ChunkDesc& self)
{
   self.assertValid();
   self.mNode->save(out);
   return out;
}

//:equality operator
// a little stricter than it needs to be.. it shouldn't care about the order of
// propertydescs...
bool ChunkDesc::operator== (const ChunkDesc& d) const
{
   assertValid();
   d.assertValid();

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
