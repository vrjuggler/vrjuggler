
#ifndef _VJ_VARVALUE_H_
#define _VJ_VARVALUE_H_

#include <vjConfig.h>
#include <ctype.h>
#include <string.h>
#include <iostream.h>


typedef enum { T_INT, T_FLOAT, T_BOOL, T_STRING, T_DISTANCE, T_CHUNK,
               T_INVALID } VarType;

typedef enum {U_Feet, U_Inches, U_Meters, U_Centimeters, U_BadUnit}
              CfgUnit;

/* note for myself as I'm adding T_DISTANCE - everything gets stored
 * internally as feet.
 */

//: A vjVarValue is an object that knows it's own type even if we don't.
//  More seriously, it's the value storage unit and value return type
//  for a ConfigChunk. <br>
//  Currently, vjVarValues can be of types int, FLOAT, boolean, string
//  (char*), distance(essentially FLOAT), as defined by the VarType
//  enumeration in vjVarValue.h. <br>
//  When you get a vjVarValue, you can do just a few things with it: <br>
//    1. assign it to a variable and then use it.  Note that there is
//       type checking here: if you try assigning a string vjVarValue to
//       an int, you'll get an error. <br>
//    2. Cast it to the right type and use it. <br>
//    3. print it - vjVarValues have overloaded << so you can print them
//       without having to cast to the right value. <br>
//  Note that it's generally incumbent upon the client to know what
//  kind of vjVarValue he's getting and what it can do.  Hey, you're
//  the one who queryed the ConfigChunk, not me. <br>
//
class vjVarValue {

  /* general question of using objects in here.
   * we have to actually store the objects as pointers, in order
   * to be able to use classes that have constructors inside
   * the union.
   * When should we copy?  Seems like the objects we store should
   * be fresh copies of whatever's given to us, and probably so
   * should the objects we return, just for safety's sake.
   * would it be ok to receive and return pointers to objects?
   * What objects would we need to store? mebbe a string class?
   */

private:

  VarType    type;

  // these are the possible storage areas.
  struct {
    int        intval;
    float      floatval;
    char       *strval;
    bool       boolval;
  } val;

public:

  //: Copy constructor.
  vjVarValue (vjVarValue &v) {
    *this = v;
  }


    vjVarValue& operator= (vjVarValue &v) {
	if ((type == T_STRING) || (type == T_CHUNK))
	    if (val.strval) {
		delete val.strval;
		val.strval = NULL;
	    }
	type = v.type;
	switch (type) {
	case T_INVALID:
	    break;
	case T_INT:
	    val.intval = v.val.intval;
	    break;
	case T_FLOAT:
	    val.floatval = v.val.floatval;
	    break;
	case T_BOOL:
	    val.boolval = v.val.boolval;
	    break;
	case T_STRING:
	case T_CHUNK:
	    if (v.val.strval == NULL)
		val.strval = NULL;
	    else
		val.strval = strdup (v.val.strval);
	    break;
	default:
	    //cout << "something's wrong with varvalue assign" << endl;
	    break;
	}
	return *this;
    }


  //: Creates a new vjVarValue of type t.
  //! NOTE: Note that once a vjVarValue object has
  // been created,the type cannot be changed.
  vjVarValue ( VarType t ) {
    type = t;
    switch (type) {
    case T_INT:
      val.intval = 0;
      break;
    case T_BOOL:
      val.boolval = false;
      break;
    case T_FLOAT:
      val.floatval = 0.0;
      break;
    case T_STRING:
    case T_CHUNK:
      val.strval = NULL;
      break;
    }
  }

  //: Destroys self and all associated memory.
  ~vjVarValue() {
    if (((type == T_STRING) || (type == T_CHUNK)) && val.strval)
      delete val.strval;
  }


  /** @name Cast Operators
   *  These operators are used whenever a vjVarValue is cast to another
   *  type.  They do some amount of type checking and coercion,
   *  eventually returning the data stored within the config itself.
   *  Right now, in event of an error we only write a message to cerr
   *  and return a "reasonable" value - 0, 0.0, false, NULL, etc.
   */
  //@{
  /// handles T_INT and T_BOOLs
  operator int() {
    if ((type == T_INT) || (type == T_BOOL))
      return val.intval;
    if (type != T_INVALID)
      cerr << "Type error in cast!\n";
    return 0;
  }

  /*
   * it'd be nice to have a real bool, but we don't.
   */
  operator bool() {
    if ((type == T_BOOL))
      return val.boolval;
    if (type != T_INVALID)
      cerr << "Type error in cast!\n";
    return 0;
  }

  /// handles T_FLOAT and T_DISTANCE
  operator float () {
    if ((type == T_FLOAT) || (type == T_DISTANCE))
      return val.floatval;
    if (type != T_INVALID)
      cerr << "Type error in cast!\n";
    return 0.0;
  }
  /** handles T_STRING.  Note that the char array returned is a freshly
   *  allocated copy, which you're responsible for freeing when you're
   *  done with.  Also note that it's perfectly possible for this string
   *  to be NULL.
   */
  operator char* () {
    if ((type == T_STRING) || (type == T_CHUNK)) {
      if (val.strval) {
	  return strdup (val.strval);
      }
      else
	return strdup("");
    }
    if (type != T_INVALID)
      cerr << "Type error in cast to char*!\n";
    return NULL;
  }
  //@}

  /** @name Assignment Operators.
   *  This is the flip side of the cast: when data is assigned into
   *  a vjVarValue, we do some type checking (at this point only
   *  writing to cerr if an error occurs)
   */
  //@{
  ///
  vjVarValue &operator = (int i) {
    if (type == T_INT) {
      val.intval = i;
      return *this;
    }
    else if (type == T_BOOL) {
      val.boolval = i;
      return *this;
    }
    else {
      cerr << "Type error in assignment!\n";
      return *this;
    }
  }
  vjVarValue& operator = (bool i) {
    if (type == T_BOOL) {
      val.boolval = i;
      return *this;
    }
    else if (type == T_INT) {
      val.intval = i;
      return *this;
    }
    else {
      cerr << "Type error in assignment!\n";
      return *this;
    }
  }
  ///
  vjVarValue &operator = (float i) {
    if ((type == T_FLOAT) || (type == T_DISTANCE)) {
      val.floatval = i;
      return *this;
    } else {
      cerr << "Type error in assignment!\n";
      return *this;
    }
  }
  /** Note that the string assignment makes a copy of the string that
   *  belongs to the vjVarValue - you can do with the original string
   *  what you want.
   */
  vjVarValue &operator = (char *s) {
    if ((type == T_STRING) || (type == T_CHUNK)) {
      if (val.strval)
	delete val.strval;
      if (s)
	  val.strval = strdup(s);
      else
	  val.strval = NULL;
      return *this;
    } else {
      cerr << "Type error in assignment!\n";
      return *this;
    }
  }
  //@}


  /** Writes the value of self to out.  Note that self knows what type
   *  it is, so it makes sure it's printed in a reasonable way.
   *  ints, floats, and distances are printed as numbers (w/out units).
   *  bools are printed as the strings "true" and "false", and
   *  strings are printed as strings.
   */
  friend ostream& operator << (ostream& out, vjVarValue& v) {
//      cerr << "in << func" <<flush;

    switch (v.type) {
    case T_INT:
      out << v.val.intval;
      return out;
    case T_FLOAT:
    case T_DISTANCE:
      out << v.val.floatval;
      return out;
    case T_BOOL:
      out << ((v.val.boolval)?"true":"false");
      return out;
    case T_STRING:
    case T_CHUNK:
      if (v.val.strval)
	out << v.val.strval;
      return out;
    default:
      out << "[can't print value for type " << v.type << " ]";
      return out;
    }
  }

};

#endif


