/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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


namespace jccl {
   
    /** Database of ConfigChunks.
     *
     *  The ConfigChunkDB is a general-purpose container for 
     *  ConfigChunks, with functionality for loading/saving files
     *  of ConfigChunks, querying for sets of ConfigChunks with
     *  specific properties, and checking for dependencies between
     *  ConfigChunks.
     */
class JCCL_CLASS_API ConfigChunkDB {


private:

    /** Vector of ConfigChunks.  We use boost shared_ptrs to handle
     *  memory management for all ConfigChunks.
     */
    std::vector<ConfigChunkPtr> chunks;


    /** The name of the file that this DB was loaded from.  Used for
     *  locating files named in "Include" chunks.
     */
    std::string file_name;

public:

    typedef std::vector<ConfigChunkPtr>::iterator iterator;
    typedef std::vector<ConfigChunkPtr>::const_iterator const_iterator;

    inline iterator begin() {
        return chunks.begin();
    }

    inline const_iterator begin() const {
        return chunks.begin();
    }

    inline iterator end() {
        return chunks.end();
    }

    inline const_iterator end() const {
        return chunks.end();
    }



    /** Constructor.  Creates an empty ChunkDB. */
    ConfigChunkDB ();



    /** Copy Constructor.  
     *  This performs a shallow copy - this & db contain the same
     *  instances of ConfigChunk.
     */
    ConfigChunkDB (ConfigChunkDB& db);



    /** Destructor.
     *  Note that since we use ConfigChunkPtrs for memory management,
     *  the Chunks contained in self are only deleted if there are no
     *  other references to them.
     */
    ~ConfigChunkDB ();



    /** Assignment operator.
     *  This performs a shallow copy - self and db contain the same
     *  instances of ConfigChunks.
     */
    ConfigChunkDB& operator = (const ConfigChunkDB& db);



    /** Returns the filename that this DB was last loaded from.
     *  @return A string filename, or the empty string if load() was
     *          never called.
     */
    const std::string& getFileName() {
        return file_name;
    }


    /** Sets the filename associated with this DB to _name. */
    void setFileName(const std::string& _name) {
        file_name = _name;
    }


    /** True if self is empty. */
    bool isEmpty() const;



    /** Alias for erase(). */
    void removeAll();



    /* accessing individual chunks:  We ought to be able to do this by:
     *   1. giving a name of a specific chunk
     *   2. getting all the chunks of a specific type, or at least the
     *      names of all the chunks of a specific type.
     *      Maybe just a vector of names?
     */



    /** Finds a chunk with a given name.
     *  @param The name of an instance of ConfigChunk.
     *  @return A ConfigChunkPtr reference to a ConfigChunk in self whose
     *          name matches the argument, or a ConfigChunkPtr referring 
     *          to NULL if no such exists.
     */
    ConfigChunkPtr getChunk (const std::string& name) const;



    /** Creates a vector containing all the chunks in self.
     *  Note that this is a shallow copy of self's contents.
     */
    std::vector<ConfigChunkPtr> getChunks() const;



    /** Adds all chunks in new_chunks to self.
     *  Note that this is a shallow copy - self and new_chunks refer to the
     *  same instances of ConfigChunks.
     */
    void addChunks(std::vector<ConfigChunkPtr> new_chunks);



    /** Adds all chunks in new_chunks to self.
     *  Note that this is a shallow copy - self and new_chunks refer to the
     *  same instances of ConfigChunks.
     *  @param db - Non-NULL pointer to a ConfigChunkDB.
     */
    void addChunks(const ConfigChunkDB *new_chunks);



    /** Adds new_chunk to self.
     *  Note that this is a shallow copy.
     */
    void addChunk(ConfigChunkPtr new_chunk);



    /** Returns all ConfigChunks of a given type.
     *  @param mytypename - The token of a ChunkDesc.
     *  @return A pointer to a vector containing every ConfigChunk in
     *          self that uses the named ChunkDesc.  The memory for the
     *          vector should be deleted by the caller.
     */ 
    std::vector<ConfigChunkPtr>* getMatching (const std::string& mytypename) const {
        return getMatching ("type", mytypename);
    }



    /** Returns all ConfigChunks matching a given property and value.
     *
     *  @param property - name of a property to search for.
     *  @param value - a value that should match.
     *  @return A pointer to a vector containing every ConfigChunk in
     *          self that matches property and value.  The memory for the
     *          vector should be deleted by the caller.
     */ 
    std::vector<ConfigChunkPtr>* getMatching (const std::string& property, const std::string value) const;
    std::vector<ConfigChunkPtr>* getMatching (const std::string& property, int value) const;
    std::vector<ConfigChunkPtr>* getMatching (const std::string& property, float value) const;



    /** Removes all ConfigChunks from self.
     *  Note that ConfigChunks are only deleted if there are no other
     *  references to them.
     */
    void clear ();



    /** Removes a chunk with the given name.
     *  @param name - name of an instance of ConfigChunk.
     *  @return True if a matching ConfigChunk was found and removed,
     *          false otherwise.
     */
    bool removeNamed (const std::string& name);



    /** Removes all chunks matching property and value.
     *  @param property - name of a property to search for.
     *  @param value - a value that should match.
     *  @return Number of ConfigChunks removed.
     */
    int removeMatching (const std::string& property, int value);
    int removeMatching (const std::string& property, float value);
    int removeMatching (const std::string& property, const std::string& value);



    /** Sorts the ConfigChunks in self based on dependencies.
     *  The chunks are sorted so that element i can only have dependencies
     *  on elements 0 thru i-1.
     *  @param auxChunks - A ConfigChunkDB of ConfigChunks that have
     *                     already been loaded/configured succesfully,
     *                     and which can therefore be used to resolve
     *                     dependencies.
     *  @return -1 iff the sort failed to complete.
     */
    int dependencySort(ConfigChunkDB* auxChunks = NULL);


    /* IO functions: */

    /** Write contents of self to out.
     *  This uses the "old" ConfigChunk file format.
     */
    friend std::ostream& operator << (std::ostream& out, const ConfigChunkDB& self);



    /** Reads contents of self from in.
     *  This uses the old ConfigChunk file format.
     *  Note that the previous contents of self are not removed (although
     *  a ConfigChunk in self can be replaced by a newly-read Chunk with the
     *  same name).
     *  @param in - an istream.
     *  @param self - a ConfigChunkDB.
     *  @return in.
     */
    friend std::istream& operator >> (std::istream& in, ConfigChunkDB& self);



    /** Loads ConfigChunks from the given file.
     *  This uses ConfigIO to handle the actual parsing, and therefore it
     *  can intelligently determine which ConfigFile format to use for 
     *  parsing.
     *  Note that the previous contents of self are not removed (although
     *  a ConfigChunk in self can be replaced by a newly-read Chunk with the
     *  same name).
     *  @param filename - name of the file to load.
     *  @param parentfile - name of the "parent" file.  This is used for
     *                      loading files named in "Include" Chunks - if
     *                      filename is a relative pathname, the path part
     *                      of parentfile is prepended to it.
     *  @return true if the file was found and no fatal errors occurred;
     *          false if the file was not found or there was a fatal
     *          error while parsing.
     */
    bool load (const std::string& fname, const std::string& parentfile = "");



    /** Writes ConfigChunks to the given file.
     *  This uses ConfigIO to handle writing, and uses the "default"
     *  ConfigFile output format (currently XML).
     *  @param filename - name of file to create & write.
     *  @return true if succesful, false if unable to create the file.
     */
    bool save (const std::string& fname) const;

};

};
#endif
