
// vjConfigChunk.h
// Header for vjConfigChunk class
//
// Author: Christopher Just


#ifndef _VJ_CONFIGCHUNK_H_
#define _VJ_CONFIGCHUNK_H_

#include <config.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <iostream.h>
#include <string.h>

#ifdef VJ_OS_HPUX
#   include <float.h>
#   include <stddef.h>
#   include <vector>
#else
#   if defined(VJ_USE_PTHREADS) && ! defined(_PTHREADS)
#       define _PTHREADS
#   endif

#   include <vector.h>
#endif

#include <Config/vjVarValue.h>
#include <Config/vjChunkDesc.h>


/***************************************************************************
 *                              vjProperty                                 *
 ***************************************************************************/

/** A vjProperty is a unit of storage inside of a vjConfigChunk.
 *  It has a name, a type, and some number of values.
 *  Properties are meant for use only inside of a vjConfigChunk,
 *  and should not be directly used or manipulated by other classes.
 */
class vjProperty {

public:

  char *name;                    // Name of vjProperty.
  /** Number of entries allowed.
   *  -1 for variable number.  Note that this may not neccessarily be
   *  the number of values in self - use getNum for that.
   */
  int num;  
  vjPropertyDesc *description;      // Pointer to this vjProperty's description.
  VarType type;                   // Type of value entries.
  CfgUnit units;                  // A unit, if type is T_DISTANCE.
  vector<vjVarValue*> value;        // Vector of vjVarValues.



  vjProperty (vjPropertyDesc *pd);
  ~vjProperty ();


  /// Returns the actual current number of values for this vjProperty.
  int getNum ();


  vjVarValue& getValue (int ind = 0);
  // returns the value at the given index


  bool setValue (int val, int ind = 0);
  bool setValue (float val, int ind = 0);
  bool setValue (char* val, int ind = 0);


  vjEnumEntry* getEnumEntry (char *n);
  vjEnumEntry* getEnumEntry (int val);

  /** Converts the values in this property from units of u to units of feet.
   *  This is used when we're reading in a new distance property - The
   *  values have been read in as raw numbers, then we read in the unit type
   *  and realize what kind of conversion we have to do to put them in
   *  feet (our standard unit for internal storage of distances).
   */
  bool applyUnits (CfgUnit u);

  friend ostream& operator << (ostream &out, vjProperty& p);

private:
  // private fn used inside setValue()s.
  // just expands the vector if needed & allowed & checks that index is valid.
  bool preSet (int ind);

};



typedef enum {
  TK_String, TK_Float, TK_End, TK_Int, TK_Bool,
  TK_OpenBracket, TK_CloseBracket, TK_Unit, TK_Error
} CfgTok;

struct Token {
  CfgTok type;
  int intval;
  float floatval;
  bool boolval;
  CfgUnit unitval;
  char strval[1024];
};


/*****************************************************************************
 *                             vjConfigChunk                                   *
 *****************************************************************************/


/** A vjConfigChunk is essentially a list of properties, each of which has
 *  a specific type and zero or more values (which can be individually
 *  indexed).
 */

class vjConfigChunk {

private:
  vjChunkDesc* desc;
  vector<vjProperty*> props;                  // Stores the set of properties


public:

  /// Constructs a vjConfigChunk matching the given description.
  vjConfigChunk (vjChunkDesc *desc);



  /// Destroys a vjConfigChunk, freeing all properties and related memory. 
  ~vjConfigChunk ();



  /// Writes vjConfigChunk to out.
  friend ostream& operator << (ostream& out, vjConfigChunk& self);



  /// Attempts to read vjConfigChunk from in.
  friend istream& operator >> (istream& in, vjConfigChunk& self);



  /** Returns the name of a chunk's type.
   *  This is the same as a call to getvjProperty ("type",0).
   */
  vjVarValue getType ();



  /** @name Functions to query Properties of a vjConfigChunk.
   */
  //@{
  /** returns the number of values in self for the given property.
   * pre:  property points to a null-terminated string.
   * post: if property isn't the name of a property in self, we
   *       just return 0.
   *       Otherwise, we return the number of values for that
   *       property.  (So the range of valid indices when querying
   *       that property is 0 to retval -1 ).
   */
  int getNum (char *property);

  /** returns the value associated with a property.
   * pre:  property is a non-null string, ind >= 0.
   * post:
   *       The idea of returning a vjVarValue is that it gets implicitly
   *       cast to whatever value it's getting assigned to.
   *       And my overloads of vjVarValue casts make this typesafe.
   *       Is that cool or what?
   */
  vjVarValue getProperty (char *property, int ind = 0);
  //@}

  /** @name Functions to Set/Add values of properties.
   *  Use the setvjProperty functions to change a value at a given index
   *  for a property, and use addvjProperty to add values to Properties
   *  that have a variable number of properties.
   */
  //@{
  /** Sets a value for the given property.
   * pre:  property is a non-null string, ind >= 0.
   * post: If the named property is in self,  and if the index
   *       given is valid, then val is assigned to that property
   *       and we return true.  If the property isn't found, or
   *       the given index is out of range, we return false.
   * note: the char* version of setvjProperty allocates its own
   *       memory for the string.
   */
  bool setProperty (char *property, int val, int ind=0);
  ///
  bool setProperty (char *property, float val, int ind=0);
  ///
  bool setProperty (char *property, char *val,  int ind=0);

  /** Appends val to the set of values for the named property.
   *  pre:  vjProperty is a non-null string.
   *  post: If property has a fixed number of values, this
   *        does nothing and returns false.  Otherwise, we
   *        attempt to add val to the property.  A false
   *        return value would indicate a type mismatch or
   *        memory problem.
   */
  bool addValue (char *property, int val);
  ///
  bool addValue (char *property, float val);
  ///
  bool addValue (char *property, char* val);
  //@}



private:
  vjProperty *getPropertyPtr (char *name);

  vjProperty *getPropertyFromToken (char *token);

  /** Tokenizer for vjConfigChunk read.
   *  Fills in the Token object passed to it with the next token in _in_.
   *  Returns false to indicate a parsing failure.
   */
  bool getToken (istream& in, Token& tok);

  /** Attempts to assign a value (in tok) to the vjProperty's ith value.
   *  This function does a certain amount of type-mangling, and also
   *  handles enumeration lookups.  Return value is success/failure.
   */
  bool tryassign (vjProperty *p, Token &tok, int i);

};

#endif
