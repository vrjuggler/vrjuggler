/*
 * vjChunkDescDB.h 
 *
 * Author: Christopher Just
 *
 * Storage class for vjChunkDescs used by the ConfigChunkDB.
 *
 */


#ifndef _VJ_CHUNKDESCDB_H_
#define _VJ_CHUNKDESCDB_H_

#include <vjConfig.h>
#include <iostream.h>
#include <Config/vjChunkDesc.h>

class vjChunkDescDB {

private:
  vector<vjChunkDesc*> descs;

public:

  vjChunkDescDB ();
  /* PRE:  true.
   * POST: self is created as an empty vector of Chunkdescs
   */

  ~vjChunkDescDB ();

  vjChunkDesc *getChunkDesc (char *name);
  /* PRE:  name is a non-null string.
   * POST: returns a pointer to an entry in self named name, if such
   *       exists.  Returns NULL if no such entry is found.
   */

  bool insert (vjChunkDesc *d);
  /* PRE:  d points to a vjChunkDesc.
   * POST: d is inserted into self.  If a vjChunkDesc with the same name as
   *       d is inself, d replaces the older vjChunkDesc.
   *       Returns TRUE.
   */

  bool remove (char *name);
  /* PRE:  name is a string of characters.  Empty string is OK; NULL isn't.
   * POST: Any vjChunkDesc whose name matches (ignoring case) is removed from
   *       self and deleted.
   */

  void removeAll ();
  /* PRE:  none
   * POST: All vjChunkDescs are removed from self & their associated memory 
   *       freed.  This _can_ be dangerous, if there still exist ConfigChunks
   *       that refer to the deleted vjChunkDescs.
   */

  int size ();
  /* PRE:  TRUE.
   * POST: Returns the number of vjChunkDescs in self.
   */

  friend ostream& operator << (ostream& out, vjChunkDescDB& self);
  /* PRE:  out is an open ostream.
   * POST: The format of every vjChunkDesc in self is written to out,
   *       in a format that can be read with >>.
   */

  friend istream& operator >> (istream& in, vjChunkDescDB& self);
  /* PRE:  in is an open istream.
   * POST: Reads 0 or more vjChunkDescs from in and adds them to self.
   *       Stops reading when an "End" string (that isn't part of
   *       a vjChunkDesc) is read, or eof is reached.
   */

  bool load (char *fname);
  bool save (char *fname);
  /* PRE:  fname is a non-null string.
   * POST: load and save are convenience functions, that attempt to open
   *       the named file and read or write self.  Returns true if no 
   *       errors are encountered.  False most likely means failure to open
   *       the given file.
   */

};

#endif




