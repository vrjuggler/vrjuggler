
#ifndef _VJ_PROPERTY_DESC_H_
#define _VJ_PROPERTY_DESC_H_

#include <vjConfig.h>
#include <Config/vjParseUtil.h>

//------------------------------------------------------------
//: A Description used to instantiate a vjProperty
//
//       Information stored in a vjPropertyDesc includes vjProperty 
//       Name, Type, number of allowed values, and a Help string 
//       describing the purpose of the particular property.  
//       vjPropertyDescs also include information for parsing a 
//       vjProperty, and (optional) enumeration data for T_INT
//       type Properties.
//       Note: Frequently the docs for this class will refer to 'this
//       vjProperty', which refers to any object of class vjProperty 
//       instantiated
//       using this description.
//
// @author:  Christopher Just
//------------------------------------------------------------


class vjPropertyDesc {

public:

    //: Descriptive name of the vjProperty this object describes. Used in GUI.
    char *name;

    //: Short name for this vjPropertyDesc.  Used in app/library code.
    char* token;

    //: One line of help information for this vjPropertyDesc.
    char* help;

    //: Type of values allowed in this vjProperty.
    VarType type;

    //: Number of value entries allowed for this vjProperty. (-1 for variable)
    //  Typically this is an integer > 0.  For example, a tracker
    //  position offset might be described with 3 Float values (xyz).
    //  A value of -1 indicates that this vjProperty may have a variable
    //  number of values (e.g. for a list of active Walls).
    int  num; 

    //: Labels for individual values of this Property (ie. "width", "height")
    std::vector<vjEnumEntry*> valuelabels;

    //: A list of labeled values that are allowed.
    //  string/int pairs for T_INTs,
    //  valid string values for T_STRINGS, and names of acceptable chunk
    //  types for T_CHUNK.  Note that in the T_CHUNK case, an empty enumv
    //  means _all_ chunk types are accepted
    std::vector<vjEnumEntry*> enumv;
    
    //: Constructor
    //!POST: name, token, help = NULL, type = T_INVALID, num = 0,
    //+      valuelabels & enumerations are empty.
    vjPropertyDesc ();

    //: Convenience constructor
    //!POST: name = token = n, help = h, num = i, type = t,
    //+      valuelabels & enumerations are empty.
    vjPropertyDesc (char *n, int i, VarType t, char* h);


    //: Destroys a vjPropertyDesc, and frees all allocated memory.
    ~vjPropertyDesc ();


    //: Returns the enumeration entry at index ind
    //! ARGS: index - index of EnumEntry to retrieve (0-base)
    //! RETURNS: NULL - if index is < 0 or out of range
    //! RETURNS: enumentry* - otherwise 
    vjEnumEntry *getEnumEntryAt (int index);


    
    //: Writes a vjPropertyDesc to the given ostream
    //!NOTE: output format is:
    //+      name typename num token { enum1 enum2=42 } "help string"
    friend ostream& operator << (ostream& out, vjPropertyDesc& self);



    //: Reads a vjPropertyDesc from the named istream
    //!NOTE: format is the same as that written out by <<
    friend istream& operator >> (istream& in, vjPropertyDesc& self);

};


#endif
