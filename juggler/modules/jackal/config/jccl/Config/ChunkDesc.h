/*
 * vjChunkDesc.h 
 *
 * Author: Christopher Just
 *
 * A vjChunkDesc is a description for a kind of vjConfigChunk.  The vjChunkDesc
 * includes a typename and a list of property descriptions.
 *
 */

#ifndef _CHUNKDESC_H_
#define _CHUNKDESC_H_

#include <config.h>
#include <strings.h>

#ifdef VJ_OS_HPUX
#   include <float.h>
#   include <stddef.h>
#   include <vector>
#else
#   if defined(VJ_USE_PTHREADS) && ! defined(_PTHREADS)
#       define _PTHREADS
#   endif

#   include <vector.h>
#endif	/* ifdef VJ_OS_HPUX */

#include <Config/vjVarValue.h>
#include <Config/vjParseUtil.h>

class vjEnumEntry {
private:
  char *entry;
  int val;

public:
  vjEnumEntry (char *e, int v) {
    entry = new char[strlen(e)+1];
    strcpy (entry, e);
    val = v;
  }

  ~vjEnumEntry () {
    delete entry;
  }

  char* getName () {
    return entry;
  }

  int getVal () {
    return val;
  }
};


//: A vjPropertyDesc describes a single Property entry for a vjConfigChunk.
//  Information stored in a vjPropertyDesc includes Property Name, Type,
//  number of allowed values, and a Help string describing the purpose
//  of the particular property.  vjPropertyDescs also include information
//  for parsing a Property, and (optional) enumeration data for T_INT
//  type Properties.
//  Note: Frequently the docs for this class will refer to 'this
//  Property', which refers to any object of class Property instantiated
//  using this description.
//
class vjPropertyDesc {

public:

  /// String name of the Property this object describes.
  char *name;
  /// Type of values allowed in this Property.
  VarType type;
  /** Number of value entries allowed for this Property.
   *  Typically this is an integer > 0.  For example, a tracker
   *  position offset might be described with 3 Float values (xyz).
   *  A value of -1 indicates that this Property may have a variable
   *  number of values (e.g. for a list of active Walls).
   */
  int  num; 
  /** The token is a string used to identify this Property when
   *  reading and writing vjConfigChunks.
   */
  char *help;
  char *token; 
  /* the token used to id this in the parser (not case sensitive) */
  vector<vjEnumEntry*> enumv;
  /* enumv is used to store enumerations - string/int pairs for T_INTs,
   * valid string values for T_STRINGS, and names of acceptable chunk
   * types for T_CHUNK.  Note that in the T_CHUNK case, an empty enumv
   * means _all_ chunk types are accepted
   */

  /// Creates an empty vjPropertyDesc.
  vjPropertyDesc ();

  vjPropertyDesc (char *n, int i, VarType t, char* h);
  
  /// Destroys a vjPropertyDesc, and frees all allocated memory.
  ~vjPropertyDesc ();

  /** Writes a vjPropertyDesc to the named ostream.
   *  The output format is:
   *    name typename num token { enum1 enum2=42 } "help string"
   *  ex. serialport String 1 port "Serial port name."
   */
  friend ostream& operator << (ostream& out, vjPropertyDesc& self);


  /** Reads a vjPropertyDesc from the named istream.
   *  The (expected) format of the description is the same as
   *  specified as the output of the << operator.
   */
  friend istream& operator >> (istream& in, vjPropertyDesc& self);

};


//: A vjChunkDesc contains a description of a single type of vjConfigChunk.
//  Primarily, it is a name and a list of vjPropertyDescs.
//  vjChunkDescs will probably only need to be used by the vjChunkDescDB,
//  the vjConfigChunkDB, and the GUI.
//
class vjChunkDesc {

public:
  /** Name of the vjChunkDesc.  This will be the type of the vjConfigChunks
   *  created using it.
   */
  char *name;
  /** Vector of properties.
   */
  vector<vjPropertyDesc*> plist;

  vjChunkDesc& operator= (const vjChunkDesc& other) {
    if (name)
      delete name;
    for (int i = 0; i < plist.size(); i++)
      delete plist[i];

    name = new char[strlen (other.name)+1];
    strcpy (name, other.name);
    plist = other.plist;

    return *this;

  }

  /// Creates an empty, unnamed vjChunkDesc.
  vjChunkDesc ();

  // Deletes self & associated memory.
  ~vjChunkDesc ();

  /// Sets the name of self.
  bool setName (char *n) {
    if (name)
      delete name;
    name = new char[strlen(n)+1];
    if (!name)
      return false;
    strcpy (name, n);
    return true;
  }

  /// Adds a vjPropertyDesc to self.
  bool add (vjPropertyDesc *pd);

  /// Removes named vjPropertyDesc from self.
  bool remove (char *n);


  /// returns the vjPropertyDesc of the named property, or NULL.
  vjPropertyDesc *getPropertyDesc (char *name);

  ///
  friend ostream& operator << (ostream& out, vjChunkDesc& self);
  ///
  friend istream& operator >> (istream& in, vjChunkDesc& self);


};

#endif
