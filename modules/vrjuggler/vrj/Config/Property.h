
#ifndef _VJ_PROPERTY_H_
#define _VJ_PROPERTY_H_

#include <Config/vjVarValue.h>
#include <Config/vjChunkDesc.h>


//------------------------------------------------------------------
//: Stores a property and all its values
//
// Unit of storage inside a vjConfigChunk.  Has a name, type,
// and 0 or more values.  Each instance is associated with
// a PropertyDesc at instantiation; this associated cannot
// be changed.
// Note that the Property instance maintains a pointer to
// the PropertyDesc, so be careful.
//
// @author Christopher Just
//
//------------------------------------------------------------------
class vjProperty {

public:

  vjPropertyDesc *description;      
    //: Pointer to this vjProperty's description.
  char *name;
    //: Name of the vjProperty (shortcut for description->name)
  int num;
    //: Number of values.  -1 for variable number (use getNum() )
  VarType type;                   
    //: Type of value entries.
  CfgUnit units;                  
    //: A unit, if type is T_DISTANCE.
  vector<vjVarValue*> value;        
    //: Vector containing the actual vjVarValues.


  vjProperty (vjPropertyDesc *pd);
  //: Constructor
  //! PRE: true
  //! POST: Property is created.  If num values is not -1, num
  //+       vjVarValues are created and placed in value.
  //+       Otherwise, value is left empty.
  //! ARGS: pd - a pointer to a valid vjPropertyDesc. 
  //! NOTE: Self stores a pointer to its PropertyDesc pd.  pd
  //+       should not be deleted while self exists.

  ~vjProperty ();
  //: Destructor
  //! PRE: true
  //! POST: self and its stored values are destroyed (but not
  //+       the PropertyDesc!)


  int getNum ();
  //: Returns actual current number of values in self
  //! RETURNS: size of value vector.



  vjVarValue& getValue (int ind = 0);
  //: Returns the VarValue at index
  //! PRE: True
  //! ARGS: ind - integer index of value to return (default 0)
  //! RETURNS: indth element of value, or a T_INVALID VarValue
  //+          if ind is out of bounds



  bool setValue (int val, int ind = 0);
  bool setValue (float val, int ind = 0);
  bool setValue (char* val, int ind = 0);
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
  bool preSet (int ind);
  //: Utility function for setValue(val, ind)
  //! POST: If self has a variable number of values, and ind
  //+       is greater than the current size of the value
  //+       vector, the vector is padded with new default-valued
  //+       vjVarValues.
  //! RETURNS: true if ind is a valid index to the values vector
  //+          (after padding).
  //! RETURNS: false if ind is out of bounds.
  //! ARGS: ind - index into the values vector.

};


#endif
