

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

#include <Config/vjParseUtil.h>

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
