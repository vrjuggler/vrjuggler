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

#ifndef _VRJ_PROPERTY_DESC_H_
#define _VRJ_PROPERTY_DESC_H_

#include <vrj/vrjConfig.h>
#include <vrj/Config/EnumEntry.h>

namespace vrj
{


//------------------------------------------------------------
//: A Description used to instantiate a Property
//
//       Information stored in a PropertyDesc includes Property
//       Name, Type, number of allowed values, and a Help string
//       describing the purpose of the particular property.
//       PropertyDescs also include information for parsing a
//       Property, and (optional) enumeration data for T_INT
//       type Properties.
//       Note: Frequently the docs for this class will refer to 'this
//       Property', which refers to any object of class Property
//       instantiated
//       using this description.
//
// @author:  Christopher Just
//------------------------------------------------------------

class PropertyDesc {

public:

    //: Constructor
    //!POST: name, token, help = NULL, type = T_INVALID, num = 0,
    //+      valuelabels & enumerations are empty.
    PropertyDesc ();



    //:Copy Constructor
    PropertyDesc (const PropertyDesc& d);



    //: Convenience constructor
    //!POST: name = token = n, help = h, num = i, type = t,
    //+      valuelabels & enumerations are empty.
    PropertyDesc (const std::string& n, int i, VarType t, const std::string& h);


    //: Destroys a PropertyDesc, and frees all allocated memory.
    ~PropertyDesc ();



    #ifdef VJ_DEBUG
    void assertValid () const;
    #else
    inline void assertValid () const {
        ;
    }
    #endif



    //: returns the token string for
    inline const std::string& getToken () const {
        return token;
    }

    inline void setToken (const std::string& tok) {
        token = tok;
    }

    inline const std::string& getName () const {
        return name;
    }

    inline void setName (const std::string& _name) {
        name = _name;
    }

    inline const std::string& getHelp () const {
        return help;
    }

    inline void setHelp (const std::string& _help) {
        help = _help;
    }

    // get rid of this soon
    inline VarType getType () const {
        return type;
    }

    inline VarType getVarType () const {
        return type;
    }

    inline void setVarType (VarType _type) {
        type = _type;
    }

    inline int getNumAllowed () const {
        return num;
    }

    // -1 for variable
    inline void setNumAllowed (int _num) {
        num = _num;
    }

    //: Returns the number of individual value labels
    inline int getValueLabelsSize () const {
        return valuelabels.size();
    }

    void appendValueLabel (const std::string& _label);

    //: Returns the ith value label
    const std::string& getValueLabel (unsigned int index) const;


    inline int getEnumerationsSize() const {
        return enumv.size();
    }

    void appendEnumeration (const std::string& _label, const std::string& _val);

    //: Returns the enumeration entry at index ind
    //! ARGS: index - index of EnumEntry to retrieve (0-base)
    //! RETURNS: NULL - if index is < 0 or out of range
    //! RETURNS: enumentry* - otherwise
    EnumEntry* getEnumEntryAtIndex (unsigned int index) const;


    //: Returns an enumentry with val matching val...
    EnumEntry* getEnumEntryWithValue (const VarValue& val) const;


    //: Returns the enumentry named _name
    //! RETURNS: NULL - if no match if found
    //! RETURNS: EnumEntry* - otherwise
    EnumEntry* getEnumEntry (const std::string& _name) const;


    //: Writes a PropertyDesc to the given ostream
    //!NOTE: output format is:
    //+      name typename num token { enum1 enum2=42 } "help string"
    friend std::ostream& operator << (std::ostream& out, const PropertyDesc& self);



    //: Reads a PropertyDesc from the named istream
    //!NOTE: format is the same as that written out by <<
    friend std::istream& operator >> (std::istream& in, PropertyDesc& self);


    PropertyDesc& operator= (const PropertyDesc& pd);

    //: Equality Operator
    // BUG (IPTHACK) - doesn't check equality of enumerations and valuelabels
    bool operator== (const PropertyDesc& pd) const;

    //: Inequality Operator
    inline bool operator!= (const PropertyDesc& pd) const {
        return !(*this == pd);
    }

private:

    //: Descriptive name of the Property this object describes. Used in GUI.
    std::string name;

    //: Short name for this PropertyDesc.  Used in app/library code.
    std::string token;

    //: One line of help information for this PropertyDesc.
    std::string help;

    //: Type of values allowed in this Property.
    VarType type;

    //: Number of value entries allowed for this Property. (-1 for variable)
    //  Typically this is an integer > 0.  For example, a tracker
    //  position offset might be described with 3 Float values (xyz).
    //  A value of -1 indicates that this Property may have a variable
    //  number of values (e.g. for a list of active Walls).
    int  num;

    //: Labels for individual values of this Property (ie. "width", "height")
    std::vector<EnumEntry*> valuelabels;

    //: A list of labeled values that are allowed.
    //  string/int pairs for T_INTs,
    //  valid string values for T_STRINGS, and names of acceptable chunk
    //  types for T_CHUNK.  Note that in the T_CHUNK case, an empty enumv
    //  means _all_ chunk types are accepted
    std::vector<EnumEntry*> enumv;

    int enum_val;

    unsigned int validation;

};

};
#endif
