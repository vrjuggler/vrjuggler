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


#ifndef _JCCL_CONFIGCHUNK_H_
#define _JCCL_CONFIGCHUNK_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/ChunkDescPtr.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <jccl/Config/Property.h>
#include <jccl/Config/VarValue.h>


namespace jccl
{
   

/** A container of configuration information.
 *  A ConfigChunk stores a number of Propertys that describe
 *  the configuration of a particular component of the system.
 *  It has an associated ChunkDesc which describes its type
 *  and the Propertys that belong to it.
 *
 *  @version $Revision$, $Date$
 */
class JCCL_CLASS_API ConfigChunk {

protected:
    ChunkDescPtr mDesc;                /**< Description for this Chunk. */
    std::vector<Property*> mProps;     /**< Properties of this Chunk. */

    unsigned int mValidation;          /**< Flag for testing memory use.*/

    static const std::string embedded_separator; 


public:

    static bool hasSeparator (const std::string &path);

    static std::string getRemainder (const std::string &path);

    static std::string getFirstNameComponent (const std::string& path);

public:

    /** Constructs a ConfigChunk with no given description.
     *  @note A ConfigChunk created with this function is essentially
     *        useless until a description has been assigned with
     *        associateDesc() or the Chunk is assigned into with =, 
     *        but I want this so I can have vectors of
     *        ConfigChunks, instead of ptrs...
     */
    ConfigChunk ();



    /** Constructs a ConfigChunk matching the given description.
     *  @param desc points to a valid ChunkDesc
     *  @param use_defaults (default = true) determines whether to use
     *         the default values supplied with this ChunkDesc.  This
     *         is almost always true (except when creating the default
     *         values chunk itself).
     */
    ConfigChunk (ChunkDescPtr _desc, bool use_defaults = true);



    /** Copy constructor. */
    ConfigChunk (const ConfigChunk& c);



    /** Destroys a ConfigChunk and frees associated memory.
     *  @post Self has been destroyed, along with all properties
     *        and values associated with it (but _not_ the memory
     *        associated with its ChunkDesc).
     */
    ~ConfigChunk ();



    #ifdef JCCL_DEBUG
    void assertValid () const;
    #else 
    inline void assertValid () const {;}
    #endif



    /** Assignment operator. */
    ConfigChunk& operator = (const ConfigChunk& c);



    /** Tests for value equality of two ConfigChunks.
     *  @return True iff self and c have the same values for all properties.
     */
    bool operator == (const ConfigChunk& c) const;



    /** Tests for inequality of two ConfigChunks. 
     *  @return True iff self and c don't have the same value.
     **/
    inline bool operator != (const ConfigChunk& c) const {
        return !(*this == c);
    }



    /** Lexical comparison based on chunks' instance names.
     *  @return True if the instance name of self lexically
     *          precedes the name of c.
     */
    bool operator < (const ConfigChunk& c) const;



    /** Iterator type for self's properties. */
    typedef std::vector<Property*>::iterator iterator;
    /** Const iterator type for self's properties. */
    typedef std::vector<Property*>::const_iterator const_iterator;


    /** Iterator to beginning of self's properties list. */
    inline iterator begin() {
        return mProps.begin();
    }

    /** Const iterator to beginning of self's properties list. */
    inline const_iterator begin() const {
        return mProps.begin();
    }

    /** Iterator to end of self's properties list. */
    inline iterator end() {
        return mProps.end();
    }

    /** Const iterator to end of self's properties list. */
    inline const_iterator end() const {
        return mProps.end();
    }



    /** Gets an embedded chunk from a property of self.
     *  This is primarily used for dependency resolution.
     *  @param path The complete name of an embedded chunk in self.
     *              The form is, for example, 
     *              "base_name->prop_name->instance_name".
     *  @return A ConfigChunkPtr to the embedded chunk.  This may be
     *          A NULL ConfigChunkPtr, if the embedded chunk name was
     *          not found.  Check before use.
     */
    ConfigChunkPtr getEmbeddedChunk (const std::string &path);



    /** Writes a representation of self to out.
     *  @param out An ostream.
     *  @param self A ConfigChunk.
     *  @return out.
     *  @note This method uses ConfigIO to perform the actual output.
     */
    friend JCCL_API(std::ostream&) operator << (std::ostream& out,
                                                const ConfigChunk& self);



    /** Alias for getDescToken().
     *  @deprecated
     */
    const std::string& getType () const;



    /** Returns number of values for the specified property.
     *  @param property The token of a property.
     *  @return The number of values that exist for the given property,
     *          or 0 if the property does not exist in self.
     */
    int getNum (const std::string& property) const;



    /** Returns the instance name of this ConfigChunk. */
    const std::string getName () const {
        return getProperty("Name");
    }



    /** Returns the token string that identifies self's ChunkDesc. */
    const std::string& getDescToken () const;



    /** Returns a value from one of self's properties. 
     *  @param property The token string for a property.
     *  @param ind The index to the property's list of values.  Use
     *             getNum() to determine the number of values for a 
     *             given property.  The valid range is 0 to getNum()-1.
     *  @return The ind'th value in the named property's list of values,
     *          or an INVALID VarValue if the property doesn't exist or
     *          the index is out of range.
     *  @see VarValue
     */
    const VarValue& getProperty (const std::string& property, int ind = 0) const;



    /** @name Property setter values. */
    //@{
    /** Sets a value for the given property.
     *  @param property The token string for a property.
     *  @param ind The index to the property's list of values.  Use
     *             getNum() to determine the number of values for a 
     *             given property.  The valid range is 0 to getNum()-1.
     *             The default value is 0.
     *  @param val The value to set.
     *  @post The ind'th value of the specified property (if it exists)
     *        is set to val.  If the property allows a variable number
     *        of values, the values list may be extended so that the ind'th
     *        value exists.
     *  @return True if the set was successful, otherwise false.  Failure
     *          can indicate that the property wasn't found, the index 
     *          was out of range, or a type mismatch on the assignment.
     */
    bool setProperty (const std::string& property, int val, int ind=0);
    bool setProperty (const std::string& property, float val, int ind=0);
    bool setProperty (const std::string& property, const std::string& val, 
                      int ind=0);
    bool setProperty (const std::string& property, 
                      ConfigChunkPtr val, int ind=0);
    //@}



    /** Checks if the given property actually exists within self.
     *  @return True iff self has a Property with the given token.
     */
    inline bool doesPropertyExistFromToken( const std::string& token ) const
    {
       return (this->getPropertyPtrFromToken( token ) != NULL);
    }    



    /** Return a list of self's depenencies.
     *  Dependencies are any ConfigChunks named by a "Chunk Pointer" property
     *  of self (or any chunk embedded in self).
     *  @return A vector of the names of all ConfigChunks referenced by self,
     *          which can be used for dependency checking.
     */
    std::vector<std::string> getChunkPtrDependencies() const;



    /** Returns the requested property from self.
     *  @param property The name string for a property.
     *  @return A pointer to the matching property, or NULL if none was
     *          found.
     */
    Property *getPropertyPtrFromName (const std::string& name) const;
    


    /** Returns the requested property from self.
     *  @param property The token string for a property.
     *  @return A pointer to the matching property, or NULL if none was
     *          found.
     */
    Property *getPropertyPtrFromToken (const std::string& token) const;


public:

    /** Associates the description d with this Chunk
     *  @post When this function is called, any previous properties etc.
     *        of this Chunk are destroyed, and new (blank) properties are
     *        created.
     */ 
    void associateDesc (ChunkDescPtr d, bool use_defaults = true);



    /** Returns copies of all the values for a given property.
     *  @param property The token string for a property.
     *  @return A vector of VarValue*.  These are copies of the VarValues in
     *          self and should be deleted when the caller is finished with
     *          the vector.  If the specified property was not found, the
     *          vector will be empty.
     */
    std::vector<VarValue*> getAllProperties (const std::string& property) const;




};


}; // namespace jccl

#endif

