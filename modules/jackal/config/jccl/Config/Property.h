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



#ifndef _JCCL_PROPERTY_H_
#define _JCCL_PROPERTY_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/VarValue.h>
#include <jccl/Config/PropertyDesc.h>
#include <jccl/Config/ChunkDescPtr.h>
#include <jccl/Config/ConfigChunkPtr.h>

namespace jccl 
{
    
    
    /** Stores a property and all its values.
     *  Unit of storage inside a ConfigChunk.  Has a name, type,
     *  and 0 or more values.  Some Propertys have a fixed number
     *  of values (e.g. to store the three components of a point
     *  in 3space), while others have a variable number (e.g. to
     *  store a list of active devices)
     *  <p>
     *  Each instance is associated with
     *  a PropertyDesc at instantiation; this associated generally
     *  should not be changed.
     *  <p>
     *  Note that the Property instance maintains a pointer to
     *  the PropertyDesc, so be careful.  Generally the ConfigChunk
     *  containing the Property will maintain a ChunkDescPtr to
     *  the ChunkDesc containing the PropertyDesc, ensuring that
     *  memory is managed properly.
     */
class Property {

protected:

    /** Pointer to this Property's description. */
    PropertyDesc *mDescription;

    /** Type of value entries. */
    VarType mType;

    /** A unit, if type is T_DISTANCE. 
     *  @note  The 'unit' code was never fully completed and
     *         is not fully functional.
     */
    CfgUnit mUnits;

    /** Debug flag for testing validity of self. */
    unsigned int mValidation;

    /** Number of values.  -1 for variable number. */
    int mNum;

    /** Vector containing the actual VarValues. */
    std::vector<VarValue*> mValues;

    /** ChunkDesc for embedded chunk (if valtype is EMBEDDEDCHUNK). */
    ChunkDescPtr mEmbedDesc;

public:

    /** Constructor.
     *  @post Self is created.  If num values is not -1, num VarValues are
     *        created and placed in value.  otherwise value is left empty.
     *  @param pd a pointer to a valid PropertyDesc.  Self stores this
     *            pointer, so pd must not be deleted while self exists.
     */
    Property (PropertyDesc *pd);



    /** Copy constructor. */
    Property (const Property& p);



    /** Destructor
     *  @post Self and its stored values are destroyed (but not
     *        its PropertyDesc).
     */
    ~Property ();



    #ifdef JCCL_DEBUG
    void assertValid () const;
    #else
    inline void assertValid () const {
        ;
    }
    #endif


    /** Assignment operator. */
    Property& operator= (const Property& p);



    /** Equality operator.
     *  @return True iff self and p use the same PropertDesc and all values
     *          are equal.
     */
    bool operator== (const Property& p) const;

    /** Inequality operator. */
    inline bool operator != (const Property& p) const {
        return !(*this == p);
    }



    /** Returns the PropertyDesc used to instantiate self. */
    PropertyDesc* getPropertyDesc () const {
        return mDescription;
    }



    /** Returns current number of values in self.
     *  @return Size of values list.
     */
    int getNum () const;



    /** Returns whether self has a fixed number of values.
     *  @return True if self has a fixed number of values; false if values
     *          can be added or removed.
     */
    inline bool hasFixedNumberOfValues () const {
        return (mNum >= 0);
    }



    /** Returns the type of values stored in self.
     *  @return The type of values stored in self - as determined by self's
     *          PropertyDesc.
     */
    inline VarType getType () const {
        return mType;
    }


    /** Returns the name of self.
     *  @return the name used by self's PropertyDesc.
     */
    const std::string& getName () const;


    /** Returns the token of self.
     *  @return the token used by self's PropertyDesc.
     */
    const std::string& getToken () const;


    
    /** Returns a value from self.
     *  @param i Index of the value to return (default = 0).
     *  @return A reference to the ith value in self.  If i is
     *          out of bounds, return a reference to a VJ_T_INVALID
     *          VarValue.  The reference (or pointers to it)
     *          should not be maintained for long periods of time.
     */
    const VarValue& getValue (unsigned int ind = 0) const;



    /** Sets the value at index.
     *  @param val The value to assign.  If a char*, it must be a 
     *             0-terminated C string which is copied.
     *  @param i Index to assign at.
     *  @post If i is in range ( >= 0 and < num), the ith value
     *        in self is set to val.  If self has a variable number
     *        of values, the list of values will be automatically
     *        extended to include i.
     *  @note If the argument can't be assigned because of a type
     *        mismatch, the value at i won't change, though the list
     *        of values may still be extended.  See VarValue::= for
     *        details.
     */
    //@{
    bool setValue (int val, int ind = 0);
    bool setValue (float val, int ind = 0);
    bool setValue (const std::string&  val, int ind = 0);
    bool setValue (ConfigChunkPtr val, int ind = 0);
    bool setValue (const VarValue& val, int ind = 0);
    //@}


    /** Attempts to assign a value to self's ith value.
     *  @note In addition to the usual sorts of padding etc. that setValue
     *        use, this function does a certain amount of type mangling,
     *        and also handles enumeration values lookup.
     *  @return True if we were able to assign a reasonable conversion of
     *          val to self.
     */
    bool tryAssign (int index, const char* val);


    /** Converts the values in this property from units of u to units of feet.
     *  This is used when we're reading in a new distance property - The
     *  values have been read in as raw numbers, then we read in the unit type
     *  and realize what kind of conversion we have to do to put them in
     *  feet (our standard unit for internal storage of distances).
     */
    bool applyUnits (CfgUnit u);


    /** Writes p to out. 
     *  Uses ConfigIO to do the actual output.
     */
    friend std::ostream& operator << (std::ostream &out, Property& p);


protected:

    /** Creates a VarValue of the correct type for this property.
     *  This is used by the setValue/tryAssign methods to pad the
     *  values list.
     *  @param i Position of this value.  Used for giving embedded
     *           chunks default names based on value labels.
     */
    VarValue *createVarValue (int i = -1);


    /** Utility function for setValue(val, ind) functions
     *  @post If self has a variable number of values, and ind
     *        is greater than the current size of the value
     *        vector, the vector is padded with new default-valued
     *        VarValues.
     *  @param index Index into the values vector.
     *  @return True iff ind is a valid index to the values vector
     *          (after padding).
     */
    bool preSet (unsigned int index);

};

};

#endif


