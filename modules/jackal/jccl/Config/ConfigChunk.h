
// vjConfigChunk.h
// Header for vjConfigChunk class
//
// Author: Christopher Just


#ifndef _VJ_CONFIGCHUNK_H_
#define _VJ_CONFIGCHUNK_H_

#include <vjConfig.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <iostream.h>

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

#include <Config/vjProperty.h>


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



//------------------------------------------------------------------
//: A container for configuration information
//
// A vjConfigChunk stores a number of vjPropertys that describe
// the configuration of a particular component of the system.
// It has an associated vjChunkDesc which describes its type
// and the vjPropertys that belong to it.
//
//------------------------------------------------------------------
class vjConfigChunk {

private:
    vjChunkDesc* desc;
    vector<vjProperty*> props;       // Stores the set of properties


public:

    //: Constructs a vjConfigChunk matching the given description.
    //!PRE: desc points to a valid vjChunkDesc
    //!POST: self has been created, and all its vjPropertys 
    //+      initialized to their default values.
    vjConfigChunk (vjChunkDesc *desc);



    //: Destroys a vjConfigChunk and all related memory.
    //!POST:  self has been destroyed, and all memory associated
    //+       with it and its properties is freed (but not the
    //+       memory associated with its vjChunkDesc).
    ~vjConfigChunk ();



    vjConfigChunk (vjConfigChunk& c);



    vjConfigChunk& operator = (vjConfigChunk& c);



    //: writes self to out
    //!POST: self is written to out.  Format is as defined
    //+      in the ConfigFileFormats document.
    //!RETURNS: out - the output stream
    //!ARGS: out - a valid ostream.
    //!ARGS: self - a valid vjConfigChunk
    friend ostream& operator << (ostream& out, vjConfigChunk& self);



    //: reads a value from in
    //!POST: the value of self is changed based on what is
    //+      read from in.
    //!RETURNS: in - the input stream
    //!ARGS: in - a valid input stream
    //!ARGS: self - a valid vjConfigChunk
    friend istream& operator >> (istream& in, vjConfigChunk& self);



    //: Returns the name of a chunk's type.
    //!RETURNS: s - a C string containing the token for this
    //+          chunk's vjChunkDesc.
    //!NOTE: this is the same as a call of getProperty ("type",0)
    vjVarValue& getType ();



    //: returns number of values for the named property
    //!ARGS: property - a non-NULL C string; the name of a property
    //!RETURNS: n - the number of values contained by the named
    //+         property. (could be 0)
    //!RETURNS: 0 - if self does not contain a property with the
    //+         given name.
    //!NOTE: This should always be used before looking at any
    //+      vjProperty that can have a variable number of values.
    int getNum (char *property);



    //: Returns one of the values for a given property.
    //!ARGS: property - a non-NULL C string, the token of a property
    //!ARGS: ind - an integer index, in the range from 0 to
    //+      getNum(property) -1.  Determines which of the
    //+      values in the vjProperty to return.
    //!RETURNS: v - a vjVarValue that can be cast directly if
    //+         its type is known (float, int, etc).
    vjVarValue& getProperty (char *property, int ind = 0);



    //: Sets a value for the given property.
    //!PRE:  property is a non-null string, ind >= 0.
    //!POST: If the named property is in self,  and if the index
    //+      given is valid, and the types match, then val is 
    //+      assigned to that value of that property
    //!RETURNS: true - succesful assignment
    //!RETURNS: false - failed assignment (type mismatch,
    //+         no such property, invalid index, etc.)
    //!NOTE: if ind is higher than the number of values in the
    //+      property, but the property allows a variable number
    //+      of values, the property will be padded with values
    //+      so that the assignment can occure.
    //+      <p>The char* version of setProperty allocates its
    //+      own copy of the string value argument.
    bool setProperty (char *property, int val, int ind=0);
    bool setProperty (char *property, float val, int ind=0);
    bool setProperty (char *property, char *val,  int ind=0);



    //: Appends val to the set of values for the named vjProperty
    //!NOTE: This can be considered a convenience function for
    //+      "setValue (property, val, getNum(property))"
    //!ARGS: property - non-NULL C string, token for a property.
    //!ARGS: val - a value to be appended to the named property.
    //!POST: The given vjProperty has the new value appended to 
    //+      the end of its list of values.  
    //!RETURNS: true - success
    //!RETURNS: false - failure. (no such property, or it 
    //+         does exist but has a fixed number of values
    bool addValue (char *property, int val);
    bool addValue (char *property, float val);
    bool addValue (char *property, char* val);



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
