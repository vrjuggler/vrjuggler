/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_CONFIGCHUNK_H_
#define _VJ_CONFIGCHUNK_H_

#include <vjConfig.h>
#include <Config/vjChunkDesc.h>
#include <Config/vjProperty.h>
#include <Config/vjVarValue.h>

#ifdef VJ_OS_HPUX
#   include <float.h>
#   include <stddef.h>
#endif



struct VJCFGToken;


//------------------------------------------------------------------
//: A container for configuration information
//
// A vjConfigChunk stores a number of vjPropertys that describe
// the configuration of a particular component of the system.
// It has an associated vjChunkDesc which describes its type
// and the vjPropertys that belong to it.
//
// @author  Christopher Just
//
//!PUBLIC_API:
//------------------------------------------------------------------
class vjConfigChunk {

private:
    vjChunkDesc* desc;
    std::vector<vjProperty*> props;       // Stores the set of properties
    vjVarValue type_as_varvalue;

public:

    //: Constructs a vjConfigChunk matching the given description.
    //!PRE: desc points to a valid vjChunkDesc
    //!POST: self has been created, and all its vjPropertys
    //+      initialized to their default values.
    vjConfigChunk (vjChunkDesc *_desc);



    //: Destroys a vjConfigChunk and all related memory.
    //!POST:  self has been destroyed, and all memory associated
    //+       with it and its properties is freed (but not the
    //+       memory associated with its vjChunkDesc).
    ~vjConfigChunk ();


    vjConfigChunk (vjConfigChunk& c);


    vjConfigChunk& operator = (const vjConfigChunk& c);



    //: tests for equality of two vjConfigChunks
    bool operator== (const vjConfigChunk& c);

    //: tests for inequality of two vjConfigChunks
    inline bool operator != (const vjConfigChunk& c) {
	return !(*this == c);
    }


    //: Compares two vjConfigChunks based on their instance names
    bool operator< (vjConfigChunk& c);



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
    int getNum (const std::string& property);



    //: Returns one of the values for a given property.
    //!ARGS: property - a non-NULL C string, the token of a property
    //!ARGS: ind - an integer index, in the range from 0 to
    //+      getNum(property) -1.  Determines which of the
    //+      values in the vjProperty to return.
    //!RETURNS: v - a vjVarValue that can be cast directly if
    //+         its type is known (float, int, etc).
    vjVarValue& getProperty (const std::string& property, int ind = 0);


    //: Return all the values for a given property
    // This is just a simple helper function
    //! NOTE: The vector has COPIES of the var values.
    std::vector<vjVarValue*> getAllProperties(const std::string& property);


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
    bool setProperty (const std::string& property, int val, int ind=0);
    bool setProperty (const std::string& property, float val, int ind=0);
    bool setProperty (const std::string& property, const std::string& val,  int ind=0);
    bool setProperty (const std::string& property, vjConfigChunk *val,  int ind=0);



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
    bool addValue (const std::string& property, int val);
    bool addValue (const std::string& property, float val);
    bool addValue (const std::string& property, const std::string& val);
    bool addValue (const std::string& property, vjConfigChunk* val);


    //: Return a list of chunk names dependant upon this one
    // Returns a list of the names of all the chunks
    // that are pointed to by chunk ptrs of this chunk
    std::vector<std::string> getDependencies();

private:
    vjProperty *getPropertyPtrFromName (const std::string& name);
    
    vjProperty *getPropertyPtrFromToken (const std::string& token);

    //: Attempts to assign a value (in tok) to the vjProperty's ith value.
    //!NOTE:  This function does a certain amount of type-mangling, and also
    //+        handles enumeration lookups.  Return value is success/failure.
    bool tryassign (vjProperty *p, int i, const char* val);

};

#endif



