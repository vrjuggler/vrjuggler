/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



#ifndef _VJ_CONFIGCHUNKDB_H_
#define _VJ_CONFIGCHUNKDB_H_

#include <vjConfig.h>
#include <Config/vjConfigChunk.h>


//------------------------------------------------------------------
//: Database of ConfigChunks
//
// The vjConfigChunkDB is a general-purpose container for
// ConfigChunks, with functionality for reading/writing
// files of ConfigChunks and querying for sets of configchunks
// with specific properties.
//
// @author Christopher Just
// October 1997
//
//!PUBLIC_API:
//------------------------------------------------------------------
class vjConfigChunkDB {

private:
    //: vector of ConfigChunks
    std::vector<vjConfigChunk*> chunks;

    //: utility for load() - replaces $HOME and $VJ_BASE_DIR
    std::string demangleFileName (const std::string& n);

public:

    typedef std::vector<vjConfigChunk*>::iterator iterator;

    iterator begin() {
	return chunks.begin();
    }

    iterator end() {
	return chunks.end();
    }

    //: Constructor
    //! PRE: true
    //! POST: self is created. Chunks is empty.
    vjConfigChunkDB ();



    //: Copy constructor
    vjConfigChunkDB (vjConfigChunkDB& db);



    //: Destructor
    //! PRE: true
    //! POST: self is destroyed and its associated memory freed.
    //+       This includes all ConfigChunks stored in self, but
    //+       not any ChunkDescDB
    ~vjConfigChunkDB ();



    //: Assignment operator
    vjConfigChunkDB& operator = (vjConfigChunkDB& db);



    //: Checks if self is empty
    //! RETURNS: true - if self contains no vjConfigChunks
    //! RETURNS: false - otherwise
    bool isEmpty();



    //: Removes and destroys all vjConfigChunks in self
    //! PRE: true
    //! POST: All ConfigChunks in self have been removed and their
    //+       associated memory freed.
    void removeAll();



    /* accessing individual chunks:  We ought to be able to do this by:
     *   1. giving a name of a specific chunk
     *   2. getting all the chunks of a specific type, or at least the
     *      names of all the chunks of a specific type.
     *      Maybe just a vector of names?
     */



    //: Finds a chunk with a given name
    //! PRE: true
    //! ARGS: name - a non-NULL C string.
    //! RETURNS: p - A pointer to a ConfigChunk in self whose name matches
    //+          the argument, or NULL if no such element exists.
    //! NOTE: The memory associated with the return value belongs to
    //+       the ConfigChunkDB, and should not be delete()d
    vjConfigChunk *getChunk (const std::string& name);


    //: return a vector of all the chunks
    //! POST: returns
    //! RETURNS: Copy of the pointers to the chunks in this.
    std::vector<vjConfigChunk*> getChunks();


    //: Add chunks to this
    //! PRE: Description DB that this points to must have all desc's for added chunks
    //! POST: this has added the chunks specified
    void addChunks(std::vector<vjConfigChunk*> new_chunks);



    //: Adds a chunk to this
    void addChunk(vjConfigChunk* new_chunk);



    //: Returns all chunks of a given type.
    //! PRE: true;
    //! POST: true;
    //! ARGS: property - a non-NULL C string, the name of a property.
    //! ARGS: value - value of a property.  non-NULL C string.
    //! RETURNS: p - Pointer to a vector of ConfigChunk* containing
    //+          all vjConfigChunks in self that have the specified
    //+          type (ie vjChunkDesc token).
    //! NOTE: The memory for the vector should be deleted by the
    //+       caller when it is no longer needed.  The individual
    //+       vjConfigChunks in the vector should not be freed.
    std::vector<vjConfigChunk*>* getMatching (const std::string& mytypename) {
	return getMatching ("type", mytypename);
    }



    //: Returns all chunks with a given property and value.
    //! PRE: true
    //! ARGS: property - a non-NULL C string, the name of a property.
    //! ARGS: value - value of a property.  If char*, must be non-NULL
    //! RETURNS: p - Pointer to a vector of ConfigChunk* containing
    //+          all ConfigChunks in self that have a property whose
    //+          name matches the first argument and which has as
    //+          one of its values the value given in the second
    //+          argument.
    //! NOTE: The memory for the vector should be deleted by the
    //+       caller when it is no longer needed.  The individual
    //+       ConfigChunks in the vector should not be freed.
    std::vector<vjConfigChunk*>* getMatching (const std::string& property, const std::string value);
    std::vector<vjConfigChunk*>* getMatching (const std::string& property, int value);
    std::vector<vjConfigChunk*>* getMatching (const std::string& property, float value);



    //: Alias for removeAll()
    bool erase ();



    //: Removes the named chunk
    //! POST: If a ConfigChunk with the specified name exists
    //+       in self, it is removed and its memory freed.
    //! RETURNS: true - a matching chunk was found.
    //! RETURNS: false - otherwise.
    int removeNamed (const std::string& name);



    //: Removes all chunks with a matching property and value
    //! PRE: true
    //! POST: All selected ConfigChunks in self are removed and
    //+       their memory freed.  The selection criteria is
    //+       the same as for getMatching(property, value)
    //! ARGS: property - Name of a property. Non-NULL C string.
    //! ARGS: value - value to match.  If char*, must be non-NULL
    //+       C string.
    //! RETURNS: n - Number of ConfigChunks removed.
    int removeMatching (const std::string& property, int value);
    int removeMatching (const std::string& property, float value);
    int removeMatching (const std::string& property, const std::string& value);



    //: Sorts the chunks based on dependencies
    //! PRE: true
    //! MODIFIES: self.  In place sort of the config chunks
    //! ARGS: auxChunks - Auxilary chunks that have been loaded already
    //! POST: (if returns != -1)
    //+     for( i>=0 && i<chunks.size()) <br>
    //+        chunks[i] dependent only of chunks[0...(i-1)] <br>
    //+     This is a topological sorting of the dependencies. <br>
    //+     informally( forall elts in the chunks list ) <br>
    //! RETURNS: -1 - Failed to complete sort
    int dependencySort(vjConfigChunkDB* auxChunks = NULL);


    /* IO functions: */

    //! POST: A text representation of self is written to out
    //! ARGS: out - an output stream
    //! ARGS: self - a ConfigChunkDB
    //! RETURNS: out
    friend ostream& operator << (ostream& out, vjConfigChunkDB& self);



    //! POST: ConfigChunks are read from in and added to self until
    //+       an 'end' token or EOF is read.
    //! ARGS: in - an input stream.
    //! ARGS: self - a ConfigChunkDB.
    //! RETURNS: in
    //! NOTE: Any ConfigChunks in self before the >> operation remain,
    //+       unless they have the same name as a newly read chunk
    //+       in which case they are replaced by the newer chunks.
    friend istream& operator >> (istream& in, vjConfigChunkDB& self);



    //: loads ConfigChunks from the given file
    //! ARGS: fname - name of file to read from.
    //! RETURNS: true - file was opened succesfully.
    //! RETURNS: false - otherwise.
    //! NOTE: This function calls operator >> to do its work.
    //! NOTE: The return value only considers opening the file,
    //+       and does not account for parsing or other reading
    //+       errors.
    bool load (const std::string& fname);



    //: writes ConfigChunks to the given file
    //! ARGS: fname - name of file to write to.
    //! RETURNS: true - file was opened succesfully.
    //! RETURNS: false - otherwise.
    //! NOTE: This function calls operator << to do its work.
    bool save (const std::string& fname);

};


#endif
