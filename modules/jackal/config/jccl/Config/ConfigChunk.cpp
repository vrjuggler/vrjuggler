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

#include <stdlib.h>
#include <ctype.h>

#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ParseUtil.h>
#include <jccl/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/Config/ChunkDesc.h>
#include <jccl/Config/ConfigTokens.h>

namespace jccl
{

ConfigChunk::ConfigChunk (ChunkDescPtr desc)
{
   mValidation = true;
   setDesc(desc);
   mNode = ChunkFactory::instance()->createXMLNode(); // Create a fresh node
}

ConfigChunk::ConfigChunk (cppdom::XMLNodePtr chunkNode)
{
   vprASSERT(chunkNode.get() != NULL);    // Make sure we have a valid node

   // Lookup the chunk desc
   std::string chunk_desc_token = chunkNode->getName();
   ChunkDescPtr chunk_desc_ptr = ChunkFactory::instance()->getChunkDesc(chunk_desc_token);

   vprASSERT( chunk_desc_ptr.get() != NULL && "Trying to construct chunk with no chunk description available");

   mNode = chunkNode;
   mValidation = true;
   setDesc(chunk_desc_ptr);
}

ConfigChunk::ConfigChunk (const ConfigChunk& c)
{
   mValidation = true;
   *this = c;
}

ConfigChunk::~ConfigChunk ()
{
   assertValid();
   mValidation = false;
}


ConfigChunk& ConfigChunk::operator = (const ConfigChunk& c)
{
   assertValid();
   c.assertValid();

   if (! (this == &c) )    // Different objects
   {
      mNode = c.mNode;
      mDesc = c.mDesc;
      mValidation = c.mValidation;
   }

   return *this;
}

bool ConfigChunk::operator== (const ConfigChunk& c) const
{
   assertValid();
   c.assertValid();

   // Check Descs first, then check the xml node contents
   if ( mDesc.get() != c.mDesc.get() )
   {
      return false;
   }

   std::ostringstream self_string, c_string;
   mNode->save(self_string);
   c.mNode->save(c_string);

   return (self_string == c_string);
}

bool ConfigChunk::operator< (const ConfigChunk& c) const
{
   assertValid();

   std::string s1 = getName();
   std::string s2 = c.getName();
   return (s1 < s2);
}

/**
 * This is a helper type for use with cppdom::XMLNode::getChildPred()
 */
struct NamePred
{
   NamePred (const cppdom::XMLString& name) : mName(name)
   {
      ;
   }

   bool operator() (cppdom::XMLNodePtr node)
   {
      return (node->getAttribute(name_TOKEN).getString() == mName);
   }

   cppdom::XMLString mName;
};

// used for dependency resolution
ConfigChunkPtr ConfigChunk::getChildChunk (const std::string &path)
{
   std::string working_path(path), prop_token;
   cppdom::XMLNodePtr root(mNode);
   cppdom::XMLNodeList props;

   while ( jccl::hasSeparator(working_path) )
   {
      prop_token = jccl::getFirstNameComponent(working_path);

      // Move on to the next element in the path.
      working_path = jccl::getRemainder(working_path);

      PropertyDesc prop_desc = mDesc->getPropertyDesc(prop_token);

      // Now find the property child matching the current path element and the
      // corresponding property description.
      props = root->getChildren(prop_token);

      if ( prop_desc.getVarType() == jccl::T_EMBEDDEDCHUNK && ! props.empty() )
      {
         // Get the name of the child chunk we want.
         std::string child_chunk_name = jccl::getFirstNameComponent(working_path);
         working_path = jccl::getRemainder(working_path);

         // Reset root to NULL so that we can use its internal value later to
         // determine if we are done.
         root.reset();

         NamePred name_pred(child_chunk_name);
         cppdom::XMLNodeList children;

         // Search the properties for a child whose "name" attribute matches
         // child_chunk_name.
         for ( cppdom::XMLNodeList::iterator i = props.begin();
               i != props.end();
               ++i )
         {
            children = (*i)->getChildrenPred(name_pred);
            vprASSERT((children.empty() || children.size() <= 1) && "Chunk name not unique");

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
      return ConfigChunkPtr(new ConfigChunk(root));
   }
   else
   {
      return ConfigChunkPtr();
   }
}

std::string ConfigChunk::getName () const
{
   return mNode->getAttribute(name_TOKEN).getString();
}

std::string ConfigChunk::getFullName () const
{
   cppdom::XMLNode *chunk_parent, *prop_parent;
   std::string full_name(getName());

   chunk_parent = mNode.get();

   while ( (prop_parent = chunk_parent->getParent()) != NULL &&
           prop_parent->getName() != std::string("root") &&
           prop_parent->getName() != jccl::chunk_db_TOKEN )
   {
      full_name = prop_parent->getName() + jccl::EMBEDDED_SEPARATOR + full_name;
      chunk_parent = prop_parent->getParent();
      full_name = chunk_parent->getAttribute("name").getString() +
                  EMBEDDED_SEPARATOR + full_name;
   }

   return full_name;
}

ConfigChunkPtr ConfigChunk::getProperty_ChunkPtr(const std::string& prop, int ind) const
{
   VarType var_type = mDesc->getPropertyDesc(prop).getVarType();  // Get the var type
   vprASSERT(var_type == T_EMBEDDEDCHUNK);
   vprASSERT(mNode.get() != NULL);

   // Get all property children matching the given property type.
   cppdom::XMLNodeList prop_children(mNode->getChildren(prop));

   cppdom::XMLNodePtr embedded_node(NULL);

   // --- GET the cdata node --- //
   if(prop_children.size() > unsigned(ind))                       // If we have children in range
   {
      cppdom::XMLNodeList::iterator property_i(prop_children.begin());
      std::advance(property_i, ind);                              // Advance to the correct property

      // Get the children of this property.  There should only be one child
      // that is the chunk.
      cppdom::XMLNodeList emb_nodes = (*property_i)->getChildren();
      vprASSERT(! emb_nodes.empty() && "Empty property");
      embedded_node = *(emb_nodes.begin());
   }

   ConfigChunkPtr ret_val;
   if(embedded_node.get() != NULL)
   {
      ret_val = ConfigChunkPtr( new ConfigChunk(embedded_node));     // Create a new config chunk refrencing the embedded node
   }

   return ret_val;
}

std::vector<jccl::ConfigChunkPtr> ConfigChunk::getEmbeddedChunks() const
{
   assertValid();

   std::vector<jccl::ConfigChunkPtr> embedded_list;     // Create return vector
   std::string prop_token;
   unsigned i,j;

   std::vector<PropertyDesc> prop_descs = mDesc->getAllPropertyDesc();

   //cout << "Dependency test for " << getProperty ("name") << endl;
   for ( i=0; i<prop_descs.size(); i++ )                       // For each property desc
   {
      VarType var_type = prop_descs[i].getVarType();
      if ( (var_type == T_EMBEDDEDCHUNK) )   // If property is an embedded chunk
      {
         prop_token = prop_descs[i].getToken();         // Get the property name
         unsigned int num_props = getNum(prop_token);    // How many entries

         for ( j=0; j<num_props; j++ )             // For each entry
         {
            ConfigChunkPtr embedded_chunk = this->getProperty<ConfigChunkPtr>(prop_token, j);
            vprASSERT(embedded_chunk.get() != NULL);
            embedded_list.push_back(embedded_chunk);
         }
      }
   }
   return embedded_list;      // Return the list
}



// This is used to sort a db by dependancy.
// - For each chunk ptr, add the names of the chunk ptrs
// - For each embedded chunk, recurse to get any contained chunk ptrs
std::vector<std::string> ConfigChunk::getChunkPtrDependencies() const
{
   assertValid();

   std::vector<std::string> dep_list;     // Create return vector
   std::string prop_token;
   unsigned i,j;

   std::vector<PropertyDesc> prop_descs = mDesc->getAllPropertyDesc();

   //cout << "Dependency test for " << getProperty ("name") << endl;
   for ( i=0; i<prop_descs.size(); i++ )                       // For each property desc
   {
      VarType var_type = prop_descs[i].getVarType();
      if ( (var_type == T_CHUNK) || (var_type == T_EMBEDDEDCHUNK) )   // If it is a chunk ptr or Embedded
      {
         std::string prop_token = prop_descs[i].getToken();         // Get the property name
         unsigned int num_props = getNum(prop_token);    // How many entries

         for ( j=0; j<num_props; j++ )             // For each sub property
         {
            if(var_type == T_CHUNK)                // Just get the chunk names and add it
            {
               std::string chunk_name = this->getProperty<std::string>(prop_token, j);
               if ( chunk_name != "")
               { dep_list.push_back(chunk_name); }
            }
            else if(var_type == T_EMBEDDEDCHUNK)   // Recurse and add
            {
               std::vector<std::string> child_deps;
               ConfigChunkPtr embedded_chunk = this->getProperty<ConfigChunkPtr>(prop_token, j);
               vprASSERT(embedded_chunk.get() != NULL);
               child_deps = embedded_chunk->getChunkPtrDependencies();
               dep_list.insert(dep_list.end(), child_deps.begin(), child_deps.end());
            }
         }
      }
   }
   return dep_list;      // Return the list
}


std::ostream& operator << (std::ostream& out, const ConfigChunk& self)
{
   self.assertValid();
   self.mNode->save(out);
   return out;
}

int ConfigChunk::getNum (const std::string& property_token) const
{
   assertValid();
   return mNode->getChildren(property_token).size();
}

std::string ConfigChunk::getDescToken () const
{
   return mDesc->getToken();
}

void ConfigChunk::setDesc (ChunkDescPtr desc)
{
   vprASSERT(desc.get() != NULL && "Trying to set a null description");
   mDesc = desc;
}


/** Get the property's cdata node
* @param prop  The property token
* @param ind   The index inside the property
* @param autoCreate If true, then autocreate the property node to use
*
* @note We always autocreate the cdata node if need be
*/
cppdom::XMLNodePtr ConfigChunk::getPropertyCdataNode(const std::string& prop, int ind, bool autoCreate) const
{
   vprASSERT(mNode.get() != NULL);

   cppdom::XMLNodePtr cdata_node(NULL);
   cppdom::XMLNodeList prop_children(mNode->getChildren(prop));    // Get all children call prop

   // -- Autogrow for children --- //
   if( !(prop_children.size() > unsigned(ind)) && autoCreate)      // If child not in range and autocreate
   {
      PropertyDesc prop_desc = mDesc->getPropertyDesc(prop);

      while( !(prop_children.size() > unsigned(ind) ))     // While not in range
      {
         cppdom::XMLNodePtr new_node = ChunkFactory::instance()->createXMLNode();
         new_node->setName(prop);
         new_node->setCdata(prop_desc.getDefaultValueString(ind));
         mNode->addChild(new_node);                      // Add Child
         prop_children = mNode->getChildren(prop);       // Get children again to check size
      }
   }

   // --- GET the cdata node --- //
   if(prop_children.size() > unsigned(ind))                                 // If we have children in range
   {
      cppdom::XMLNodeList::iterator property_i(prop_children.begin());
      std::advance(property_i, ind);                                    // Advance to the correct child
      cdata_node = (*property_i)->getChild("cdata");        // Get the text node

      if(cdata_node.get() == NULL)     // If no cdata node, then make one
      {
         cppdom::XMLNodePtr new_cdata_node = ChunkFactory::instance()->createXMLNode();
         new_cdata_node->setName("cdata");
         new_cdata_node->setType(cppdom::xml_nt_cdata);
         (*property_i)->addChild(new_cdata_node);
         cdata_node = new_cdata_node;
      }

      vprASSERT(cdata_node.get() != NULL);
   }
   else
   { vprASSERT(!autoCreate && "Auto create failed"); }

   return cdata_node;
}

std::string ConfigChunk::getPropertyString(const std::string& prop, int ind) const
{
   vprASSERT(mNode.get() != NULL);
   vprASSERT(name_TOKEN != prop && "Use getName() to get a chunk's name");

   std::string prop_string_rep("");     // The String rep to convert from
   // Get the cdata node without auto create
   cppdom::XMLNodePtr cdata_node = getPropertyCdataNode(prop,ind,false);

   if(cdata_node.get() != NULL)
   {
      prop_string_rep = cdata_node->getCdata();            // Get the cdata
   }
   // Fall back on the default value.
   else
   {
      PropertyDesc prop_desc = mDesc->getPropertyDesc(prop);
      prop_string_rep = prop_desc.getDefaultValueString(ind);
   }

   return prop_string_rep;
}

} // End of jccl namespace
