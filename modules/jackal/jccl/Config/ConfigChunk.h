/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#ifndef _VJ_CONFIGCHUNK_H_
#define _VJ_CONFIGCHUNK_H_

#include <vjConfig.h>
#include <Config/vjChunkDesc.h>
#include <Config/vjProperty.h>
#include <Config/vjVarValue.h>


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
class VJ_CLASS_API vjConfigChunk {

private:
    vjChunkDesc* desc;
    std::vector<vjProperty*> props;       // Stores the set of properties
    vjVarValue type_as_varvalue;
    unsigned int validation;  // flag for testing validity of self

    static const std::string embedded_separator;


public:

    static bool hasSeparator (const std::string &path) {
        return (path.find(embedded_separator) != path.npos);
    }

    static std::string getRemainder (const std::string &path) {
        std::string::size_type i = path.find (embedded_separator);
        if (i == path.npos)
            return path;
        else 
            return path.substr (i + embedded_separator.length());
    }

    static std::string getFirstNameComponent (const std::string& path) {
        std::string::size_type i = path.find (embedded_separator);
        if (i == path.npos)
            return path;
        else 
            return path.substr (0, i);
    }



public:

    //: Constructs a vjConfigChunk with no given description.
    //! NOTE: a ConfigChunk created with this function is essentially
    //+       useless until a description has been assigned with
    //+       associateDesc() or the Chunk is assigned into with =, 
    //+       but I want this so I can have vectors of
    //+       ConfigChunks, instead of ptrs...
    vjConfigChunk ();


    //: Constructs a vjConfigChunk matching the given description.
    //!PRE: desc points to a valid vjChunkDesc
    //!POST: self has been created, and all its vjPropertys
    //+      initialized to their default values.
    vjConfigChunk (vjChunkDesc *_desc, bool use_defaults = true);



    //: Destroys a vjConfigChunk and all related memory.
    //!POST:  self has been destroyed, and all memory associated
    //+       with it and its properties is freed (but not the
    //+       memory associated with its vjChunkDesc).
    ~vjConfigChunk ();


    vjConfigChunk (const vjConfigChunk& c);


    #ifdef VJ_DEBUG
    void assertValid () const;
    #else
    inline void assertValid () const {
        ;
    }
    #endif

    //: Associates the description d with this Chunk
    //!NOTE:  When this function is called, any previous properties etc.
    //+       of this Chunk are destroyed, and new (blank) properties are
    //+       created.
    void associateDesc (vjChunkDesc* d, bool use_defaults = true);


    vjConfigChunk& operator = (const vjConfigChunk& c);



    //: tests for equality of two vjConfigChunks
    bool operator == (const vjConfigChunk& c) const;

    //: tests for inequality of two vjConfigChunks
    inline bool operator != (const vjConfigChunk& c) const {
        return !(*this == c);
    }


    //: Compares two vjConfigChunks based on their instance names
    bool operator < (const vjConfigChunk& c) const;



    typedef std::vector<vjProperty*>::iterator iterator;
    typedef std::vector<vjProperty*>::const_iterator const_iterator;

    inline iterator begin() {
        return props.begin();
    }

    inline const_iterator begin() const {
        return props.begin();
    }

    inline iterator end() {
        return props.end();
    }

    inline const_iterator end() const {
        return props.end();
    }


    // used for dependency resolution
    vjConfigChunk* getEmbeddedChunk (const std::string &path);


    //: writes self to out
    //!POST: self is written to out.  Format is as defined
    //+      in the ConfigFileFormats document.
    //!RETURNS: out - the output stream
    //!ARGS: out - a valid ostream.
    //!ARGS: self - a valid vjConfigChunk
    friend VJ_API(std::ostream&) operator << (std::ostream& out,
                                              const vjConfigChunk& self);



    //: reads a value from in
    //!POST: the value of self is changed based on what is
    //+      read from in.
    //!RETURNS: in - the input stream
    //!ARGS: in - a valid input stream
    //!ARGS: self - a valid vjConfigChunk
    friend VJ_API(std::istream&) operator >> (std::istream& in,
                                              vjConfigChunk& self);



    //: Returns the name of a chunk's type.
    //!RETURNS: s - a C string containing the token for this
    //+          chunk's vjChunkDesc.
    //!NOTE: this is the same as a call of getProperty ("type",0)
    const vjVarValue& getType () const;



    //: returns number of values for the named property
    //!ARGS: property - a non-NULL C string; the name of a property
    //!RETURNS: n - the number of values contained by the named
    //+         property. (could be 0)
    //!RETURNS: 0 - if self does not contain a property with the
    //+         given name.
    //!NOTE: This should always be used before looking at any
    //+      vjProperty that can have a variable number of values.
    int getNum (const std::string& property) const;



    //: Returns the name of this ConfigChunk.
    const std::string getName () const {
        return getProperty("Name");
    }

    //: Returns token of this ConfigChunk's ChunkDesc.
    const std::string& getDescToken () const {
        return desc->getToken();
    }


    //: Returns one of the values for a given property.
    //!ARGS: property - a non-NULL C string, the token of a property
    //!ARGS: ind - an integer index, in the range from 0 to
    //+      getNum(property) -1.  Determines which of the
    //+      values in the vjProperty to return.
    //!RETURNS: v - a vjVarValue that can be cast directly if
    //+         its type is known (float, int, etc).
    const vjVarValue& getProperty (const std::string& property, int ind = 0) const;


    //: Return all the values for a given property
    // This is just a simple helper function
    //! NOTE: The vector has COPIES of the var values.
    std::vector<vjVarValue*> getAllProperties (const std::string& property) const;


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


    //: check to see if a property exists within a config chunk.
    inline bool doesPropertyExistFromToken( const std::string& token ) const
    {
       return (this->getPropertyPtrFromToken( token ) != NULL);
    }    
    
    //: Return a list of chunk names dependant based on chunk ptrs
    // Returns a list of the names of all the chunks
    // that are pointed to by chunk ptrs of this chunk
    std::vector<std::string> getChunkPtrDependencies() const;

    vjProperty *getPropertyPtrFromName (const std::string& name) const;
    
    vjProperty *getPropertyPtrFromToken (const std::string& token) const;


};

#endif



