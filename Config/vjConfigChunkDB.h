 
// vjConfigChunkDB.h
// Header for vjConfigChunkDB class
//
// Author: Christopher Just
// V1 - October 1997 
 

#ifndef _CONFIGCHUNKDB_H_
#define _CONFIGCHUNKDB_H_

#include <config.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkDescDB.h>



/** The vjConfigChunkDB is a general-purpose container for vjConfigChunks,
 *  with functionality for reading/writing files of vjConfigChunks and
 *  querying for sets of configchunks with specific properties.
 */
class vjConfigChunkDB {



private:
  vjChunkDescDB *descs;
  vector<vjConfigChunk*> chunks;

public:

  /** Creates a new vjConfigChunkDB.  You should pass the constructor a
   *  pointer to a vjChunkDescDB so that we'll know how to, if you'll 
   *  pardon the expression, make chunks.
   *  If you don't provide a vjChunkDescDB* you won't be able to read
   *  config files or create Chunks.
   */
  vjConfigChunkDB (vjChunkDescDB *d = NULL);

  /** Destroys a vjConfigChunkDB, freeing all associated memory.  This
   *  includes all the Chunks in the DB and their properties, but not
   *  the vjChunkDescDB.
   */
  ~vjConfigChunkDB ();

  /** Sets the vjChunkDescDB that self should use for creating vjConfigChunks
   */
  void setChunkDescDB (vjChunkDescDB *d);

  /** returns ptr to vjChunkDescDB */
  vjChunkDescDB* getChunkDescDB() {
    return descs;
  }

  bool isEmpty() {
    return (chunks.size() == 0);
  }

  void removeAll() {
    // just an alias
    erase();
  }

  /* accessing individual chunks:  We ought to be able to do this by:
   *   1. giving a name of a specific chunk
   *   2. getting all the chunks of a specific type, or at least the 
   *      names of all the chunks of a specific type.
   *      Maybe just a vector of names?
   */
  vjConfigChunk *getChunk (char *name);

  /** @name vjConfigChunkDB Query Functions.
   *
   * getMatching() lets you search for sets of vjConfigChunks.
   *
   * In all these functions, the return value is a vector<vjConfigChunk*>* - 
   * that is, a pointer to a vector whose elements are pointers to 
   * vjConfigChunks.  The memory for the vector itself must be delete()ed by
   * the calling client when you're done with it.  The memory of the
   * individual vjConfigChunks should _not_.
   */
  //@{
  /// Returns all vjConfigChunks of type mytypename.
  vector<vjConfigChunk*>* getMatching (char *mytypename) {
    return getMatching ("type", mytypename);
  }
  /** Returns all vjConfigChunks whose named property has the given value.
   *  The two-argument versions of getMatching take a property and a
   *  value as arguments, and returns all vjConfigChunks where the 
   *  named property has the specified value.
   */
  vector<vjConfigChunk*>* getMatching (char *property, char *value);
  ///
  vector<vjConfigChunk*>* getMatching (char *property, int value);
  ///
  vector<vjConfigChunk*>* getMatching (char *property, float value);
  //@}


  /// Removes all vjConfigChunks from self, and frees their memory.
  bool erase ();

  /** @name Remove vjConfigChunks functions.
   *  These functions remove the specified vjConfigChunks from self,
   *  and free thier memory.  The forms of the functions are the
   *  same as for getMatching().
   */
  //@{
  /// 
  int remove (char *mytypename) {
    return remove ("type", mytypename);
  }
  ///
  int remove (char *property, int value);
  ///
  int remove (char *property, float value);
  ///
  int remove (char *property, char *value);
  //@}


  /* IO functions: */

  /** Writes all the vjConfigChunks in self to the stream out.
   */
  friend ostream& operator << (ostream& out, vjConfigChunkDB& self);
  /** Reads vjConfigChunks from in and adds them to self until EOF
   *  is reached.
   */
  friend istream& operator >> (istream& in, vjConfigChunkDB& self);

  /** Reads vjConfigChunks from the named file and adds them to self 
   *  until EOF is reached.  Returns false if an error occurs.
   */
  bool load (char *fname);
  /** Writes all vjConfigChunks in self to the named file.  Returns
   *  false if an error occurs.
   */
  bool save (char *fname);

};


#endif
