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

#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Util/Debug.h>


namespace jccl
{


ConfigChunkDB::ConfigChunkDB(ConfigChunkDB& db)
{
   *this = db;
}

ConfigChunkPtr ConfigChunkDB::get(const std::string& name) const
{
   for ( std::vector<ConfigChunkPtr>::const_iterator i = mChunks.begin();
         i != mChunks.end();
         ++i )
   {
      if ( name == (*i)->getName() )
      {
         return *i;
      }
   }
   return ConfigChunkPtr();
}

void ConfigChunkDB::getByType(const std::string& typeName, std::vector<ConfigChunkPtr>& chunks) const
{

   for ( std::vector<ConfigChunkPtr>::const_iterator i = mChunks.begin();
         i != mChunks.end();
         ++i )
   {
      if ( typeName == (*i)->getDescToken() )
      {
         chunks.push_back(*i);
      }
   }
}

bool ConfigChunkDB::remove(const std::string& name)
{
   for ( std::vector<ConfigChunkPtr>::iterator i = mChunks.begin();
         i != mChunks.end();
         ++i )
   {
      if ( name == (*i)->getName() )
      {
         mChunks.erase(i);
         return true;
      }
   }
   return false;
}

//: Sorts the chunks based on dependancies of chunk ptrs
//! PRE: we need a "good object"
//! MODIFIES: self.  We move the objects around so they are sorted
//! ARGS: auxChunks - Auxilary chunks that have been loaded already
//! POST: Topologically sorted
// Impl
// - Copy the chunks over to a new list.
// - Repetatively try to find an item in the source list that already
//     has it's dependencies in the dst list of in the aux chunks
// - Do this iteratively until done or until fail.
/*
bool ConfigChunkDB::dependencySort(ConfigChunkDB* auxChunks)
{
   // Print out dependancies
#ifdef JCCL_DEBUG
   vprDEBUG_BEGIN(jcclDBG_CONFIG,4) << "---- Dependencies -----------\n" << vprDEBUG_FLUSH;

   for ( unsigned int i=0; i< mChunks.size(); i++ )
   {
      vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL) << "Chunk:" << mChunks[i]->getName()
                                 << std::endl << "\tDepends on:\n"
                                 << vprDEBUG_FLUSH;
      std::vector<std::string> deps = mChunks[i]->getChunkPtrDependencies();

      if (!deps.empty())
      {
         for ( unsigned int j=0;j<deps.size();j++ )
         {
            vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL) << "   " << j << ": "
                                       << deps[j].c_str() << std::endl
                                       << vprDEBUG_FLUSH;
         }
      }
      else
      {
         vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL) << "   None.\n" << vprDEBUG_FLUSH;
      }
   }
   vprDEBUG_END(jcclDBG_CONFIG,4) << "-----------------------------\n" << vprDEBUG_FLUSH;
#endif

   // --- THE SORT --- //
   // - Create new src list to work from
   // - Targetting the local data
   // So basically, we take an element from the src list one at a time
   // If it's dependencies are already in the local list, add it to the local list
   // else go on to the next one
   // Kinda like an insertion sort
   std::vector<ConfigChunkPtr> src_chunks = mChunks;
   mChunks.clear();                 // Zero out the current state

   bool dep_pass(true);             // Flag for Pass dependency check
   std::vector<std::string> deps;   // Dependencies of current item
   std::vector<ConfigChunkPtr>::iterator cur_item = src_chunks.begin();          // The current src item to look at

   while ( cur_item != src_chunks.end() )    // While not at end of src list
   {
      vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL)
         << "Checking depencies for: " << (*cur_item)->getName()
         << "\n" << vprDEBUG_FLUSH;

      dep_pass = true;                                         // Default to deps passing
      deps = (*cur_item)->getChunkPtrDependencies();           // Get src dependencies

      // For each dep: While (still have another dep) AND (deps haven't failed)
      // - If embedded chunk dep, then check that
      // - Else check as a normal chunk name
      for ( unsigned int dep_num=0; (dep_num < deps.size()) && dep_pass; dep_num++ )
      {
         vprASSERT(dep_pass == true);

         if ( jccl::hasSeparator(deps[dep_num]) )      // If it is an embedded chunk
         {
            std::string chunkname = jccl::getFirstNameComponent(deps[dep_num]);
            std::string child_chunkname = jccl::getRemainder(deps[dep_num]);
            ConfigChunkPtr parent_ch = get(chunkname);
            dep_pass = ((parent_ch.get() != NULL) &&
                         (parent_ch->getChildChunk(child_chunkname).get() != NULL));
            if ( !dep_pass && (auxChunks != NULL) )            // Check chunks in aux chunks
            {
               parent_ch = auxChunks->get(chunkname);                 // Get the aux chunk parent chunk
               dep_pass = ((parent_ch.get() != 0) &&
                           (parent_ch->getChildChunk(child_chunkname).get() != 0));
            }
         }
         else
         {
            // = found local or found in aux
            dep_pass = (get(deps[dep_num]).get() != NULL);
            if (!dep_pass && (auxChunks != NULL))  // If didn't pass and have aux to check
            {
               dep_pass = (auxChunks->get(deps[dep_num]).get() != NULL);
            }
         }
      }

      if ( dep_pass )      // If all dependencies are accounted for
      {
         mChunks.push_back(*cur_item);       // Copy src to dst
         src_chunks.erase(cur_item);         // Erase it from source
         cur_item = src_chunks.begin();      // Goto first item
      }
      else
      {
         cur_item++;             // Try next item
      }
   }

   // ASSERT: (Done with sort)
   //   Either, all depencies have been accounted for and the src list is empty
   //   OR we went all the way through list without finding an item that passes
   if ( src_chunks.size() > 0 )     // Items left, so we failed -- ouput error
   {
      for ( unsigned int i=0;i<src_chunks.size();i++ )
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "ERROR:") << " Dependency error:  Chunk:"
            << src_chunks[i]->getName() << "\tDepends on: \n"
            << vprDEBUG_FLUSH;

         std::vector<std::string> deps = src_chunks[i]->getChunkPtrDependencies();
         if ( deps.size() > 0 )
         {
            for ( unsigned j=0; j<deps.size(); j++ )
            {
               vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL) << "\tdep " << j << ": "
                                        << deps[j].c_str() << std::endl
                                        << vprDEBUG_FLUSH;
            }
         }
         else
         {
            vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL) << "Nothing.\n" << vprDEBUG_FLUSH;
         }

         vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "Check for missing files or missing chunks in loaded files.\n")
            << vprDEBUG_FLUSH;
      }
      mChunks.insert(mChunks.end(), src_chunks.begin(), src_chunks.end());   // Copy over the rest anyway

      return false;
   }
   else
   {
      // Print out sorted dependancies
#ifdef JCCL_DEBUG
      vprDEBUG_BEGIN(jcclDBG_CONFIG,4) << "---- After sort ----"
                                       << std::endl << vprDEBUG_FLUSH;

      for ( unsigned i=0; i<mChunks.size(); ++i )
      {
         vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL) << "Chunk:"
                                    << mChunks[i]->getName()
                                    << "\n\tDepends on:\n"
                                    << vprDEBUG_FLUSH;

         std::vector<std::string> deps = mChunks[i]->getChunkPtrDependencies();
         if ( deps.size() > 0 )
         {
            for ( unsigned j=0; j<deps.size(); j++ )
            {
               vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL) << "   " << j << ": "
                                          << deps[j].c_str()
                                          << std::endl << vprDEBUG_FLUSH;
            }
         }
         else
         {
            vprDEBUG(jcclDBG_CONFIG, vprDBG_VERB_LVL) << "   Nothing.\n" << vprDEBUG_FLUSH;
         }
      }

      vprDEBUG_END(jcclDBG_CONFIG,4) << "-----------------\n" << vprDEBUG_FLUSH;
#endif

      return true;      // Success
   }

   return true;
}
*/

/* IO functions: */

std::ostream& operator<<(std::ostream& out, const ConfigChunkDB& self)
{
   cppdom::NodePtr chunk_db_node;
   self.createChunkDBNode(chunk_db_node);
   chunk_db_node->save(out);
   return out;
}

std::istream& operator>>(std::istream& in, ConfigChunkDB& self)
{
   cppdom::NodePtr chunk_db_node = ChunkFactory::instance()->createXMLNode();
   cppdom::ContextPtr context_ptr = chunk_db_node->getContext();
   chunk_db_node->load( in, context_ptr );
   self.loadFromChunkDBNode( chunk_db_node );

   return in;
}

bool ConfigChunkDB::load(const std::string& filename, const std::string& parentfile)
{
   vprDEBUG_OutputGuard(jcclDBG_CONFIG, vprDBG_CONFIG_LVL,
                        std::string("Loading config file ") + filename + std::string("\n"),
                        std::string("\n"));

   std::string absolute_filename = demangleFileName(filename, parentfile);
   mFileName = absolute_filename;

   cppdom::DocumentPtr chunk_db_doc(ChunkFactory::instance()->createXMLDocument());
   bool status(false);

   try
   {
      chunk_db_doc->loadFile(absolute_filename);

      // Go through the include processing instructions
      cppdom::NodeList pi_list = chunk_db_doc->getPiList();
      for ( cppdom::NodeList::iterator itr = pi_list.begin();
            itr != pi_list.end();
            ++itr )
      {
         cppdom::NodePtr pi(*itr);

         // A desc DB has been included
         if (pi->getName() == include_desc_INSTRUCTION)
         {
            // Get the path to the included file relative to the current file
            std::string desc_filename =
               pi->getAttribute(file_TOKEN).getValue<std::string>();

            vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
               << "Including " << desc_filename << std::endl << vprDEBUG_FLUSH;

            // Load the file
            ChunkFactory::instance()->loadDescs(desc_filename, filename);
         }
         // A chunk DB has been included
         else if (pi->getName() == include_INSTRUCTION)
         {
            // Get the path to the included file relative to the current file
            std::string chunk_filename =
               pi->getAttribute(file_TOKEN).getValue<std::string>();

            vprDEBUG(jcclDBG_CONFIG, vprDBG_CONFIG_LVL)
               << "Including " << chunk_filename << std::endl
               << vprDEBUG_FLUSH;

            // Load the file
            load(chunk_filename, filename);
         }
      }

      cppdom::NodePtr chunk_db_node(chunk_db_doc->getChild(jccl::chunk_db_TOKEN));
      vprASSERT(chunk_db_node.get() != NULL);

      // Load in the chunk DB in the original file
      loadFromChunkDBNode(chunk_db_node, filename);

      status = true;
   }
   catch (cppdom::Error& xml_e)
   {
      cppdom::Location where( chunk_db_doc->getContext()->getLocation() );
      std::string errmsg = xml_e.getStrError();

      // print out where the error occured
      vprDEBUG(vprDBG_ERROR, vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED, "ConfigChunkDB: XMLError:") << mFileName
         << ": line " << where.getLine() << " at position " << where.getPos()
         << ": error: " << errmsg.c_str() << std::endl << vprDEBUG_FLUSH;
   }

   return status;
}

bool ConfigChunkDB::save(const std::string& fname) const
{
   cppdom::NodePtr chunk_db_node;
   createChunkDBNode(chunk_db_node);                              // Get base db element
   cppdom::DocumentPtr chunk_db_doc(new cppdom::Document);        // Put in in a document
   chunk_db_doc->addChild(chunk_db_node);
   chunk_db_doc->saveFile(fname);                                 // Write out the document
   return true;
}

/**
 * This is a helper for use with std::find_if() for comparing chunk names.
 */
struct ChunkNamePred
{
   ChunkNamePred(const std::string& name) : mName(name)
   {
      ;
   }

   bool operator()(jccl::ConfigChunkPtr chunk)
   {
      return (chunk->getName() == mName);
   }

   std::string mName;
};

/** Load the chunks from a given "ChunkDB" element into the db */
bool ConfigChunkDB::loadFromChunkDBNode(cppdom::NodePtr chunkDBNode,
                                        std::string currentFile)
{
   if(chunkDBNode->getName() != chunk_db_TOKEN)
   {
      vprASSERT(false && "Trying to load a chunk db node that is not a chunkdb node");
      return false;
   }

   for(cppdom::NodeList::iterator cur_child = chunkDBNode->getChildren().begin();
        cur_child != chunkDBNode->getChildren().end(); cur_child++)
   {
      ConfigChunkPtr new_chunk(new ConfigChunk());                // New chunk
      bool init_status = new_chunk->initFromNode(*cur_child);     // Initialize it

      // Make sure that there were no problems creating the new chunk (ex: invalid desc)
      if(!init_status)
      {
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
            << "  Config element load problem in file: " << currentFile
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
            << "  Skipping element: " << (*cur_child)->getAttribute(name_TOKEN).getString()
            << std::endl << vprDEBUG_FLUSH;
      }
      else
      {
         // Before we can add new_chunk to the database, we have to determine
         // if there is already a chunk with the same name.
         std::vector<ConfigChunkPtr>::iterator iter =
            std::find_if(mChunks.begin(), mChunks.end(),
                         ChunkNamePred(new_chunk->getName()));

         // If no existing chunk has the same name as new_chunk, then we can
         // just add it to the end.
         if ( iter == mChunks.end() )
         {
            mChunks.push_back(new_chunk);
         }
         // Otherwise, overwrite the old version.
         else
         {
            *iter = new_chunk;
         }
      }
   }

   return true;
}

void ConfigChunkDB::createChunkDBNode(cppdom::NodePtr& chunkDBNode) const
{
   chunkDBNode = ChunkFactory::instance()->createXMLNode();
   chunkDBNode->setName(chunk_db_TOKEN);

   std::vector<ConfigChunkPtr>::const_iterator cur_chunk;

   for(cur_chunk = mChunks.begin(); cur_chunk != mChunks.end(); ++cur_chunk)
   {
      cppdom::NodePtr child_node = (*cur_chunk)->getNode();
      chunkDBNode->addChild( child_node );
   }
}


} // End of jccl namespace
