/*
 *  File:	    $Name$
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



#ifndef _VJ_PROPERTY_DESC_H_
#define _VJ_PROPERTY_DESC_H_

#include <vjConfig.h>
#include <Config/vjEnumEntry.h>
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

    //: Constructor
    //!POST: name, token, help = NULL, type = T_INVALID, num = 0,
    //+      valuelabels & enumerations are empty.
    vjPropertyDesc ();

    //: Convenience constructor
    //!POST: name = token = n, help = h, num = i, type = t,
    //+      valuelabels & enumerations are empty.
    vjPropertyDesc (const std::string& n, int i, VarType t, const std::string& h);


    //: Destroys a vjPropertyDesc, and frees all allocated memory.
    ~vjPropertyDesc ();



    //: returns the token string for
    inline std::string& getToken () {
	return token;
    }


    inline std::string& getName () {
	return name;
    }



    inline VarType getType () {
	return type;
    }


    inline int getNumAllowed () {
	return num;
    }


    //: Returns the number of individual value labels
    inline int getValueLabelsSize () {
	return valuelabels.size();
    }


    //: Returns the ith value label
    std::string getValueLabel (int i);



    //: Returns the enumeration entry at index ind
    //! ARGS: index - index of EnumEntry to retrieve (0-base)
    //! RETURNS: NULL - if index is < 0 or out of range
    //! RETURNS: enumentry* - otherwise 
    vjEnumEntry* getEnumEntryAtIndex (int index);


    //: Returns an enumentry with val matching val...
    vjEnumEntry* getEnumEntryWithValue (vjVarValue& val);


    //: Returns the enumentry named _name
    //! RETURNS: NULL - if no match if found
    //! RETURNS: vjEnumEntry* - otherwise
    vjEnumEntry* getEnumEntry (const std::string& _name);

    
    //: Writes a vjPropertyDesc to the given ostream
    //!NOTE: output format is:
    //+      name typename num token { enum1 enum2=42 } "help string"
    friend ostream& operator << (ostream& out, vjPropertyDesc& self);



    //: Reads a vjPropertyDesc from the named istream
    //!NOTE: format is the same as that written out by <<
    friend istream& operator >> (istream& in, vjPropertyDesc& self);


    vjPropertyDesc& operator= (vjPropertyDesc& pd) {
	cout << "propdesc operator= is called!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	if (&pd == this)
	    return *this;
	name = pd.name;
	token = pd.token;
	help = pd.help;
	type = pd.type;
	valuelabels.erase (valuelabels.begin(), valuelabels.end());
	enumv.erase (enumv.begin(), enumv.end());
	return *this;
    }



private:

    //: Descriptive name of the vjProperty this object describes. Used in GUI.
    std::string name;

    //: Short name for this vjPropertyDesc.  Used in app/library code.
    std::string token;

    //: One line of help information for this vjPropertyDesc.
    std::string help;

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
    
};


#endif
