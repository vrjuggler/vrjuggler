/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



#ifndef _VJ_PROPERTY_H_
#define _VJ_PROPERTY_H_

#include <vjConfig.h>
#include <Config/vjVarValue.h>
#include <Config/vjPropertyDesc.h>
class vjChunkDesc;


//------------------------------------------------------------------
//: Stores a property and all its values
//
// Unit of storage inside a vjConfigChunk.  Has a name, type,
// and 0 or more values.  Some vjPropertys have a fixed number
// of values (e.g. to store the three components of a point
// in 3space), while others have a variable number (e.g. to
// store a list of active devices)
// <p>Each instance is associated with
// a vjPropertyDesc at instantiation; this associated cannot
// be changed.
// <p>Note that the Property instance maintains a pointer to
// the PropertyDesc, so be careful.
//
// <p>author: Christopher Just
//
//------------------------------------------------------------------
//!PUBLIC_API:
class vjProperty {

public:

    //: Pointer to this vjProperty's description.
    vjPropertyDesc *description;

    //: Number of values.  -1 for variable number (use getNum() )
    int num;

    //: Type of value entries.
    VarType type;

    //: A unit, if type is T_DISTANCE. (not fully functional)
    CfgUnit units;

    //: Vector containing the actual vjVarValues.
    std::vector<vjVarValue*> value;

    //: ChunkDesc for embedded chunk (if valtype is T_EMBEDDEDCHUNK)
    vjChunkDesc *embeddesc;

    //: Constructor
    //! PRE: true
    //! POST: Property is created.  If num values is not -1, num
    //+       vjVarValues are created and placed in value.
    //+       Otherwise, value is left empty.
    //! ARGS: pd - a pointer to a valid vjPropertyDesc.
    //! NOTE: Self stores a pointer to its PropertyDesc pd.  pd
    //+       should not be deleted while self exists.
    vjProperty (vjPropertyDesc *pd);



    //: Destructor
    //! PRE: true
    //! POST: self and its stored values are destroyed (but not
    //+       the PropertyDesc!)
    ~vjProperty ();



    vjProperty (const vjProperty& p);



    vjProperty& operator= (const vjProperty& p);



    bool operator== (const vjProperty& p) const;
    inline bool operator != (const vjProperty& p) const {
        return !(*this == p);
    }


    //: Returns actual current number of values in self
    //! RETURNS: n - size of value vector.
    int getNum () const;


    const std::string& getName () const;


    const std::string& getToken () const;


    //: Returns the VarValue at index
    //! PRE: True
    //! ARGS: ind - integer index of value to return (default 0)
    //! RETURNS: v - indth element of value, or a T_INVALID VarValue
    //+          if ind is out of bounds
    vjVarValue& getValue (int ind = 0);



    //: set the value at ind
    //! PRE: true
    //! POST: the indth value of self is set to val, if ind is
    //+       a valid index to self's value vector. If self has
    //+       a variable number of values and ind is greater than
    //+       the current number of values, the value vector will
    //+       be padded with VarValues of the appropriate type
    //+       (with default values).
    //! NOTE: If the argument can't be assigned because of type
    //+       mismatch, the value at ind won't be changed.
    //+       See vjVarValue::= to see what happens.  Padding
    //+       of the value vector may still occur.
    //! ARGS: val - value to assign.  If char*, must be a valid
    //+       non-NULL C string.
    //! ARGS: ind - integer index to value vector
    bool setValue (int val, int ind = 0);
    bool setValue (float val, int ind = 0);
    bool setValue (const std::string&  val, int ind = 0);
    bool setValue (vjConfigChunk* val, int ind = 0);
    bool setValue (vjVarValue& val, int ind = 0);


    inline vjEnumEntry* getEnumEntry (const std::string& n) const {
        return description->getEnumEntry (n);
    }
    vjEnumEntry* getEnumEntryWithValue (int val) const;


    //: creates a vjVarValue of the correct type for this property
    //! ARGS: i - position of this value.  Useful for giving
    //+           embedded chunks names based on valuelabels.
    vjVarValue *createVarValue (int i = -1);


    /** Converts the values in this property from units of u to units of feet.
     *  This is used when we're reading in a new distance property - The
     *  values have been read in as raw numbers, then we read in the unit type
     *  and realize what kind of conversion we have to do to put them in
     *  feet (our standard unit for internal storage of distances).
     */
    bool applyUnits (CfgUnit u);


    //: writes p to out
    friend ostream& operator << (ostream &out, vjProperty& p);



private:

    //: Utility function for setValue(val, ind) functions
    //! POST: If self has a variable number of values, and ind
    //+       is greater than the current size of the value
    //+       vector, the vector is padded with new default-valued
    //+       vjVarValues.
    //! RETURNS: true if ind is a valid index to the values vector
    //+          (after padding).
    //! RETURNS: false if ind is out of bounds.
    //! ARGS: ind - index into the values vector.
    bool preSet (int ind);

};


#endif


