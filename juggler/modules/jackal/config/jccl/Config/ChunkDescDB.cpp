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
#include <vpr/Util/Assert.h>
#include <jccl/Config/ChunkDescDB.h>
#include <jccl/Config/ChunkDesc.h>
#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ChunkFactory.h>

namespace jccl
{

/** Write self out
* Create temporary parent ChunkDescDB node and make all node children
* Then write this node out
*/
std::ostream& operator << (std::ostream& out, const ChunkDescDB& self)
{
   cppdom::NodePtr chunk_desc_db_node;
   self.createChunkDescDBNode(chunk_desc_db_node);

   try
   {
      chunk_desc_db_node->save(out);
   }
   catch (cppdom::Error& xml_e)
   {
      cppdom::Location where(chunk_desc_db_node->getContext()->getLocation());
      std::string errmsg = xml_e.getStrError();

      // print out where the error occured
      std::cout << "ChunkDescDB::<< XMLError:" << where.getLine() << " "
                << "at position " << where.getPos()
                << ": error: " << errmsg.c_str() << std::endl;
   }
   catch (std::exception& std_e)
   {
      std::cerr << "ChunkDescDB::<<: std::exception: " << std_e.what()
                << std::endl;
      throw;   // rethrow exception
   }

   return out;
}

std::istream& operator >> (std::istream& in, ChunkDescDB& self)
{
   cppdom::NodePtr chunk_desc_db_node = ChunkFactory::instance()->createXMLNode();
   cppdom::ContextPtr context_ptr = chunk_desc_db_node->getContext();

   try
   {
      chunk_desc_db_node->load( in, context_ptr );
   }
   catch (cppdom::Error& xml_e)
   {
      cppdom::Location where(chunk_desc_db_node->getContext()->getLocation());
      std::string errmsg = xml_e.getStrError();

      // print out where the error occured
      std::cout << "ChunkDescDB::>> XMLError:" << where.getLine() << " "
                << "at position " << where.getPos()
                << ": error: " << errmsg.c_str() << std::endl;
   }
   catch (std::exception& std_e)
   {
      std::cerr << "ChunkDescDB::>>: std::exception: " << std_e.what()
                << std::endl;
      throw;   // rethrow exception
   }

   self.loadFromChunkDescDBNode( chunk_desc_db_node );

   return in;
}

ChunkDescPtr ChunkDescDB::get (const std::string descToken)
{
   std::map<std::string, ChunkDescPtr>::iterator desc;
   desc = mDescs.find(descToken);
   if(desc != mDescs.end())
   {  return (*desc).second;  }
   else
   {  return ChunkDescPtr(); }
}

bool ChunkDescDB::load (const std::string& filename, const std::string& parentfile)
{
   std::string fname = demangleFileName (filename, parentfile);
   cppdom::DocumentPtr chunk_desc_doc = ChunkFactory::instance()->createXMLDocument();
   bool status(false);

   try
   {
      chunk_desc_doc->loadFile( fname );

      cppdom::NodePtr chunk_desc_db_node = chunk_desc_doc->getChild( jccl::chunk_desc_db_TOKEN);
      vprASSERT(chunk_desc_db_node.get() != NULL);

      loadFromChunkDescDBNode( chunk_desc_db_node );
      status = true;
   }
   catch (cppdom::Error& xml_e)
   {
      cppdom::Location where(chunk_desc_doc->getContext()->getLocation());
      std::string errmsg = xml_e.getStrError();

      // print out where the error occured
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ChunkDescDB: XMLError:") << fname << ": line "
         << where.getLine() << " at position " << where.getPos()
         << ": error: " << errmsg.c_str() << std::endl << vprDEBUG_FLUSH;
   }
   catch (std::exception& std_e)
   {
      std::cerr << "ChunkDescDB::load: std::exception: " << std_e.what()
                << std::endl;
      //throw;   // rethrow exception
   }
   catch (...)
   {
      std::cerr << "other exception\n";
      //throw;
   }

   return status;
}

bool ChunkDescDB::save (const std::string& file_name)
{
   cppdom::NodePtr chunk_desc_db_node;
   createChunkDescDBNode(chunk_desc_db_node);                              // Get base db element
   cppdom::DocumentPtr chunk_desc_db_doc = ChunkFactory::instance()->createXMLDocument();        // Put in in a document
   chunk_desc_db_doc->addChild(chunk_desc_db_node);

   try
   {
      chunk_desc_db_doc->saveFile(file_name);                                 // Write out the document
   }
   catch (cppdom::Error& xml_e)
   {
      cppdom::Location where(chunk_desc_db_doc->getContext()->getLocation());
      std::string errmsg = xml_e.getStrError();

      // print out where the error occured
      std::cout << "ChunkDescDB::>> XMLError:" << file_name << ":"
                << where.getLine() << " " << "at position " << where.getPos()
                << ": error: " << errmsg.c_str() << std::endl;
   }
   catch (std::exception& std_e)
   {
      std::cerr << "ChunkDescDB::>>: std::exception: " << std_e.what()
                << std::endl;
      throw;   // rethrow exception
   }

   return true;
}

/** Load the chunks from a given "ChunkDescDB" element into the db */
bool ChunkDescDB::loadFromChunkDescDBNode(cppdom::NodePtr chunkDescDBNode)
{
   if(chunkDescDBNode->getName() != chunk_desc_db_TOKEN)
   {
      vprASSERT(false && "Trying to load a chunk desc node that is not a chunkdesc node");
      return false;
   }

   for(cppdom::NodeList::iterator cur_child = chunkDescDBNode->getChildren().begin();
        cur_child != chunkDescDBNode->getChildren().end(); cur_child++)
   {
      ChunkDescPtr new_desc(new ChunkDesc( *cur_child ) );     // New desc
      mDescs[(*cur_child)->getAttribute(token_TOKEN).getValue<std::string>()] = new_desc;
   }

   return true;
}

void ChunkDescDB::createChunkDescDBNode(cppdom::NodePtr& chunkDescDBNode) const
{
   chunkDescDBNode = ChunkFactory::instance()->createXMLNode();
   chunkDescDBNode->setName(chunk_desc_db_TOKEN);

   std::map<std::string, ChunkDescPtr>::const_iterator cur_desc;

   for(cur_desc = mDescs.begin(); cur_desc != mDescs.end(); ++cur_desc)
   {
      cppdom::NodePtr child_node = (*cur_desc).second->getNode();
      chunkDescDBNode->addChild( child_node );
   }
}


} // namespace jccl
