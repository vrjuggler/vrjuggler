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
#include <Config/vjEnumEntry.h>
#include <Config/vjPropertyDesc.h>



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
