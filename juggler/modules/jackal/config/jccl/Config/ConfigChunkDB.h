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

#ifndef _JCCL_CONFIGCHUNKDB_H_
#define _JCCL_CONFIGCHUNKDB_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <cppdom/cppdom.h>

#include <vector>


namespace jccl
{

/** List of ConfigChunks.
 *
 */
class JCCL_CLASS_API ConfigChunkDB
{
public:

   /** Constructor.  Creates an empty ChunkDB. */
   ConfigChunkDB()
   {;}

   /** Copy Constructor.
    *  This performs a shallow copy - this & db contain the same
    *  instances of ConfigChunk.
    */
   ConfigChunkDB(ConfigChunkDB& db);

   /** Destructor.
    *  Note that since we use ConfigChunkPtrs for memory management,
    *  the Chunks contained in self are only deleted if there are no
    *  other references to them.
    */
   ~ConfigChunkDB()
   {;}

   /** Returns the filename that this DB was last loaded from.
    *  @return A string filename, or the empty string if load() was
    *          never called.
    */
   const std::string& getFileName()
   { return mFileName; }

   /** Sets the filename associated with this DB to _name. */
   void setFileName(const std::string& fname)
   { mFileName = fname; }


   /** Finds a chunk with a given name.
    *  @param The name of an instance of ConfigChunk.
    *  @return A ConfigChunkPtr reference to a ConfigChunk in self whose
    *          name matches the argument, or a ConfigChunkPtr referring
    *          to NULL if no such exists.
    */
   ConfigChunkPtr get(const std::string& name) const;

   /** Returns all ConfigChunks of a given type.
    *  @param typeName - The token of a ChunkDesc.
    *  @param chunks - Vector that will return with only the chunks found of the given type
    */
   void getByType(const std::string& typeName, std::vector<ConfigChunkPtr>& chunks) const;

   /** Removes a chunk with the given name.
    *  @param name - name of an instance of ConfigChunk.
    *  @return True if a matching ConfigChunk was found and removed,
    *          false otherwise.
    */
   bool remove(const std::string& name);

   /** Sorts the ConfigChunks in self based on dependencies.
    *  The chunks are sorted so that for all elements i (0..n) can only
    *  have dependencies on elements 0 thru i-1.
    *  @param auxChunks - A ConfigChunkDB of ConfigChunks that have
    *                     already been loaded/configured succesfully,
    *                     and which can be used to resolve dependencies.
    *  @return true if sort succeded.
    */
   //bool dependencySort(ConfigChunkDB* auxChunks = NULL);

   /* IO functions: */

   /** Write contents of self to out. */
   friend std::ostream& operator<<(std::ostream& out, const ConfigChunkDB& self);

   /** Reads contents of self from in.
    *  Note that the previous contents of self are not removed (although
    *  a ConfigChunk in self can be replaced by a newly-read Chunk with the
    *  same name).
    *  @param in - an istream.
    *  @param self - a ConfigChunkDB.
    *  @return in.
    */
   friend std::istream& operator>>(std::istream& in, ConfigChunkDB& self);

   /** Loads ConfigChunks from the given file.
    *  @param filename - name of the file to load.
    *  @param parentfile - name of the "parent" file.  This is used for
    *                      loading files named in "Include" Chunks - if
    *                      filename is a relative pathname, the path part
    *                      of parentfile is prepended to it.
    *  @return true if the file was found and no fatal errors occurred;
    *          false if the file was not found or there was a fatal
    *          error while parsing.
    */
   bool load(const std::string& fname, const std::string& parentfile = "");

   /** Writes ConfigChunks to the given file.
    *  This uses ConfigIO to handle writing, and uses the "default"
    *  ConfigFile output format (currently XML).
    *  @param filename - name of file to create & write.
    *  @return true if succesful, false if unable to create the file.
    */
   bool save(const std::string& fname) const;

   /**
    * Returns a reference to the internal collection of chunk pointers.
    * Use with care!
    */
   std::vector<jccl::ConfigChunkPtr>& vec()
   {
      return mChunks;
   }

public:
   bool loadFromChunkDBNode(cppdom::NodePtr chunkDBNode, std::string currentFile = "<unknown>");

   void createChunkDBNode(cppdom::NodePtr& chunkDBNode) const;

protected :
   /** The name of the file that this DB was loaded from.  Used for
    *  locating files named in "Include" chunks.
    */
   std::string mFileName;

   std::vector<ConfigChunkPtr> mChunks;
};

} // End of jccl namespace

#endif
