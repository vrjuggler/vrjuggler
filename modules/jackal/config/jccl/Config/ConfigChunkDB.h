
#ifndef _VJ_CONFIGCHUNKDB_H_
#define _VJ_CONFIGCHUNKDB_H_

#include <config.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkDescDB.h>



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
//------------------------------------------------------------------
class vjConfigChunkDB {


private:
  vjChunkDescDB *descs;
  //: We need access to a ChunkDescDB for the >> operator.
  vector<vjConfigChunk*> chunks;
  //: vector of ConfigChunks

public:

  vjConfigChunkDB (vjChunkDescDB *d = NULL);
  //: Constructor
  //! PRE: true
  //! POST: self is created. Chunks is empty.
  //! ARGS: d - a pointer to a vjChunkDescDB.  Note that
  //+       files cannot be read until a ChunkDescDB is supplied.


  ~vjConfigChunkDB ();
  //: Destructor
  //! PRE: true
  //! POST: self is destroyed and its associated memory freed.
  //+       This includes all ConfigChunks stored in self, but
  //+       not any ChunkDescDB

  /** Sets the vjChunkDescDB that self should use for creating ConfigChunks
   */
  void setChunkDescDB (vjChunkDescDB *d);
  

  /** returns ptr to vjChunkDescDB */
  vjChunkDescDB* getChunkDescDB();
  //: Returns a pointer to self's ChunkDescDB
  //! PRE: true
  //! POST: true
  //! RETURNS: the value of desc (remember, this could be NULL)



  bool isEmpty();
  //! RETURNS: true if self contains no vjConfigChunks
  //! RETURNS: false otherwise



  void removeAll();
  //: Removes and destroys all vjConfigChunks in self
  //! PRE: true
  //! POST: All ConfigChunks in self have been removed and their
  //+       associated memory freed.



  /* accessing individual chunks:  We ought to be able to do this by:
   *   1. giving a name of a specific chunk
   *   2. getting all the chunks of a specific type, or at least the
   *      names of all the chunks of a specific type.
   *      Maybe just a vector of names?
   */

  vjConfigChunk *getChunk (char *name);
  //: Finds a chunk with a given name
  //! PRE: true
  //! POST: true
  //! ARGS: name - a non-NULL C string.
  //! RETURNS: A pointer to a ConfigChunk in self whose name matches
  //+          the argument, or NULL if no such element exists.
  //! NOTE: The memory associated with the return value belongs to
  //+       the ConfigChunkDB, and should not be delete()d


  vector<vjConfigChunk*>* getMatching (char *mytypename) {
    //: Returns all chunks of a given type.
    //! PRE: true;
    //! POST: true;
    //! ARGS: property - a non-NULL C string, the name of a property.
    //! ARGS: value - value of a property.  non-NULL C string.
    //! RETURNS: Pointer to a vector of ConfigChunk* containing
    //+          all ConfigChunks in self that have the specified
    //+          type (ie ChunkDesc token).
    //! NOTE: The memory for the vector should be deleted by the
    //+       caller when it is no longer needed.  The individual
    //+       ConfigChunks in the vector should not be freed.

    return getMatching ("type", mytypename);
  }



  vector<vjConfigChunk*>* getMatching (char *property, char *value);
  vector<vjConfigChunk*>* getMatching (char *property, int value);
  vector<vjConfigChunk*>* getMatching (char *property, float value);
  //: Returns all chunks with a given property and value.
  //! PRE: true
  //! POST: true
  //! ARGS: property - a non-NULL C string, the name of a property.
  //! ARGS: value - value of a property.  If char*, must be non-NULL
  //! RETURNS: Pointer to a vector of ConfigChunk* containing
  //+          all ConfigChunks in self that have a property whose
  //+          name matches the first argument and which has as
  //+          one of its values the value given in the second
  //+          argument.
  //! NOTE: The memory for the vector should be deleted by the
  //+       caller when it is no longer needed.  The individual
  //+       ConfigChunks in the vector should not be freed.


  bool erase ();
  //: Alias for removeAll()



  int removeNamed (char *name);
  //: Removes the named chunk
  //! POST: If a ConfigChunk with the specified name exists
  //+       in self, it is removed and its memory freed.
  //! RETURNS: true - a matching chunk was found.
  //! RETURNS: false - otherwise.




  int removeMatching (char *property, int value);
  int removeMatching (char *property, float value);
  int removeMatching (char *property, char *value);
  //: Removes all chunks with a matching property and value
  //! PRE: true
  //! POST: All selected ConfigChunks in self are removed and
  //+       their memory freed.  The selection criteria is 
  //+       the same as for getMatchine()
  //! ARGS: property - Name of a property. Non-NULL C string.
  //! ARGS: value - value to match.  If char*, must be non-NULL
  //+       C string.
  //! RETURNS: - Number of ConfigChunks removed.



  /* IO functions: */

  friend ostream& operator << (ostream& out, vjConfigChunkDB& self);
  //! POST: A text representation of self is written to out
  //! ARGS: out - an output stream
  //! ARGS: self - a ConfigChunkDB
  //! RETURNS: out



  friend istream& operator >> (istream& in, vjConfigChunkDB& self);
  //! POST: ConfigChunks are read from in and added to self until
  //+       an 'end' token or EOF is read.
  //! ARGS: in - an input stream.
  //! ARGS: self - a ConfigChunkDB.
  //! RETURNS: in
  //! NOTE: Any ConfigChunks in self before the >> operation remain,
  //+       unless they have the same name as a newly read chunk
  //+       in which case they are replaced by the newer chunks.



  bool load (const char *fname);
  //: loads ConfigChunks from the given file
  //! ARGS: fname - name of file to read from.
  //! RETURNS: true if file was opened succesfully.
  //! RETURNS: false otherwise.
  //! NOTE: This function calls operator >> to do its work.
  //! NOTE: The return value only considers opening the file,
  //+       and does not account for parsing or other reading 
  //+       errors.



  bool save (const char *fname);
  //: writes ConfigChunks to the given file
  //! ARGS: fname - name of file to write to.
  //! RETURNS: true if file was opened succesfully.
  //! RETURNS: false otherwise.
  //! NOTE: This function calls operator << to do its work.

};


#endif
