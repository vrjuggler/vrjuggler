/*
 * vjChunkDesc.h 
 *
 * Author: Christopher Just
 *
 * A vjChunkDesc is a description for a kind of ConfigChunk.  The vjChunkDesc
 * includes a typename and a list of property descriptions.
 *
 */

#ifndef _VJ_CHUNKDESC_H_
#define _VJ_CHUNKDESC_H_

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


/** A vjPropertyDesc describes a single vjProperty entry for a ConfigChunk.
 *  Information stored in a vjPropertyDesc includes vjProperty Name, Type,
 *  number of allowed values, and a Help string describing the purpose
 *  of the particular property.  vjPropertyDescs also include information
 *  for parsing a vjProperty, and (optional) enumeration data for T_INT
 *  type Properties.
 *  Note: Frequently the docs for this class will refer to 'this
 *  vjProperty', which refers to any object of class vjProperty instantiated
 *  using this description.
 */
class vjPropertyDesc {

public:

  /// String descriptive name of the vjProperty this object describes.
  char *name;
  /// Type of values allowed in this vjProperty.
  VarType type;
  /** Number of value entries allowed for this vjProperty.
   *  Typically this is an integer > 0.  For example, a tracker
   *  position offset might be described with 3 Float values (xyz).
   *  A value of -1 indicates that this vjProperty may have a variable
   *  number of values (e.g. for a list of active Walls).
   */
  int  num; 
  /** The token is a string used to identify this vjProperty when
   *  reading and writing ConfigChunks & querying a chunkdesc
   */
  char *help;
  char *token; 
  /* the token used to id this in the parser (not case sensitive) */

  vector<vjEnumEntry*> valuelabels;

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


/** A vjChunkDesc contains a description of a single type of ConfigChunk.
 *  Primarily, it is a name and a list of vjPropertyDescs.
 *  vjChunkDescs will probably only need to be used by the vjChunkDescDB,
 *  the ConfigChunkDB, and the GUI.
 */
class vjChunkDesc {

public:
  /** Name of the vjChunkDesc.  This will be the type of the ConfigChunks
   *  created using it.
   */
  char *name;
  char *token;
  char *help;
  /** Vector of properties.
   */
  vector<vjPropertyDesc*> plist;

  vjChunkDesc& operator= (const vjChunkDesc& other) {
    if (name)
      delete name;
    if (help)
	delete help;
    if (token)
	delete token;
    for (int i = 0; i < plist.size(); i++)
      delete plist[i];

    name = new char[strlen (other.name)+1];
    strcpy (name, other.name);
    token = new char[strlen (other.token)+1];
    strcpy (token, other.token);
    help = new char[strlen(other.help)+1];
    strcpy (help, other.help);
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

  bool setToken (char *n) {
    if (token)
      delete token;
    token = new char[strlen(n)+1];
    if (!token)
      return false;
    strcpy (token, n);
    return true;
  }
  bool setHelp (char *n) {
    if (help)
      delete help;
    help = new char[strlen(n)+1];
    if (!help)
      return false;
    strcpy (help, n);
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
