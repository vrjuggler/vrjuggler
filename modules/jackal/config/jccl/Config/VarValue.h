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



#ifndef _JCCL_VARVALUE_H_
#define _JCCL_VARVALUE_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/ConfigChunkPtr.h>
#include <ctype.h>

namespace jccl
{
   
/**********************************************************
typedef enum { T_INT, T_FLOAT, T_BOOL, T_STRING, T_DISTANCE, 
	       T_CHUNK, T_EMBEDDEDCHUNK, T_INVALID } VarType;

Had to rename T_INVALID due to a conflict of names with some
Mac OS X variables.  This could be avoided, but just changing
it here is much much easier.  Places which used T_INVALID have
also been changed to VJ_T_INVALID.
************************************************************/
typedef enum { T_INT, T_FLOAT, T_BOOL, T_STRING, T_DISTANCE, 
	       T_CHUNK, T_EMBEDDEDCHUNK, VJ_T_INVALID} VarType;

typedef enum {U_Feet, U_Inches, U_Meters, U_Centimeters, U_BadUnit}
              CfgUnit;
/* note for myself as I'm adding T_DISTANCE - everything gets stored
 * internally as feet.
 */


    /** VarValue is an object which stores a single typed data element.
     *  VarValues are used as the internal storage for properties of
     *  ConfigChunks.  ConfigChunks also return VarValues to user code.
     *  <p>
     *  Currently, VarValues can be of the primitive types int, float,
     *  boolean, and string.  There are also two special types:  Chunk,
     *  which is the string name of a ConfigChunk, and EmbeddedChunk,
     *  which actually contains an entire jccl::ConfigChunk.  The types
     *  available for VarValues are defined by the jccl::VarType 
     *  enumeration in VarValue.h.
     *  <p>
     *  When you get a VarValue, you can do several things with it:
     *    1.  Assign it to another VarValue variable and then use it.  
     *        Note that this can change the VarValue type of the
     *        variable you are assigning into.
     *    2.  Cast it to the right type and use it.  VarValues can
     *        generally be cast freely to strings, ints, floats, and
     *        booleans.
     *    3.  Print it - VarValues have overloaded << so that they
     *        can be printed directly, without knowing the value or
     *        type of value they contain.
     *  <p>
     *  Note that it's generally incumbent upon the client to know
     *  what kind of VarValue he's getting and what he can do with it.
     *  Hey, you're the one who queried the ConfigChunk, not me.
     */
    class JCCL_CLASS_API VarValue {

    private:

        VarType    type;

        // these are the possible storage areas.
        int          intval;
        float        floatval;
        std::string  strval;
        bool         boolval;
        ConfigChunkPtr embeddedchunkval;
        unsigned int validation;

        static VarValue* invalid_instance;
        static const std::string using_invalid_msg;

    public:

        /** Returns a reference to a global "invalid" VarValue.
         *  This method is mainly useful for returning an invalid
         *  VarValue in case of an error, and is used internally by some
         *  Config/ classes so that they can safely return a VarValue&.
         *  <p>
         *  Note that there is a fairly harmless race condition where an
         *  extra invalid VarValue could be created & not deleted.  Thus
         *  it is not valid to assume that all VJ_T_INVALID varvalues 
         *  are the same object.
         */
        static VarValue& getInvalidInstance ();


        /** Returns the current VarValue type of this object.
         */
        inline VarType getType () const {
            return type;
        }


        /** Copy Constructor.
         */
        VarValue (const VarValue &v);


        /** Convenience constructor for T_EMBEDDEDCHUNK VarValue.
         *  This constructor creates a VarValue with an initial value of
         *  ch.  Note that it stores a smart pointer reference to ch 
         *  itself, and does _not_ make a copy.
         *  <p>
         *  This is explicit for safety's sake.  On some compilers,
         *  the nonexplicit version with a statement like
         *      VarValue v1 = chunk.getProperty(blah)
         *  would cast the result of getProperty to a chunk* and then
         *  call this constructor instead of using the VarValue assignment
         *  operator (because at that time the copy const didn't expect a
         *  const VarValue argument (since fixed)).
         */
        explicit VarValue (const ConfigChunkPtr ch);


        /** Creates a new VarValue of type t.
         *  The initial value of the VarValue will be 0, false, empty 
         *  string, 0.0f, or a ConfigChunkPtr pointing to NULL.
         */
        VarValue ( VarType t );


        /** Destroys self and all associated memory. */
        ~VarValue();


#ifdef JCCL_DEBUG
        void assertValid () const;
#else
        inline void assertValid () const {
            ;
        }
#endif



        /** Assignment operator. */
        VarValue& operator= (const VarValue &v);



        /** Equality operator.
         *  Returns true only if the two VarValues have the same type
         *  and the same value (or if both are VJ_T_INVALID).
         */
        bool operator == (const VarValue& v) const;
        inline bool operator != (const VarValue& v) const {
            return !(*this == v);
        }


        /** Returns true if the VarValue's type is anything but VJ_T_INVALID.
         */
        inline bool isValid () const {
            return (type != VJ_T_INVALID);
        }


        /*  Cast Operators
         *  These operators are used whenever a VarValue is cast to another
         *  type.  They do some amount of type checking and coercion,
         *  eventually returning the data stored within the config itself.
         *  Right now, in event of an error we only write a message to cerr
         *  and return a "reasonable" value - 0, 0.0, false, "", NULL, etc.
         */

        /** Cast to int.
         *  @return an integer representation of self's value.  Floats are
         *          cast to int, bool to 0 or 1.  Everything else returns 0.
         */
        operator int() const;
    

        /** Cast to ConfigChunkPtr.
         *  @return A ConfigChunkPtr.  If self is not of type 
         *          T_EMBEDDEDCHUNK, the smart pointer points to NULL.
         */
        operator ConfigChunkPtr() const;


        /** Cast to bool.
         *  @return A boolean.  Ints and Floats are true if nonzero.
         *          Everything else returns false.
         */
        operator bool() const;


        /** Cast to float.
         */
        operator float () const;


        /** Converts to a string representation.
         *  @return A freshly-allocated char array that the caller is
         *          responsible for deleting.
         *          Note that EmbeddedChunk VarValues are not currently
         *          converted by this method.
         */
        char* cstring () const;


        /** Cast to std::string.
         *  Note that EmbeddedChunk VarValues are not currently
         *  converted by this method.
         */
        operator std::string () const;


        /** Assignment overload.
         *  Note that none of these assignment operators will change
         *  the VarValue's type.  Instead, the VarValue will coerce the
         *  argument into the desired type.
         *  Note that the std::string and char* assignments are quite
         *  sophisticated, and will attempt to parse strings into
         *  ints, floats, or booleans, as required.
         *  If no reasonable coercion of types exists, the assignment
         *  will fail and print an error message.
         */
        VarValue& operator = (int i);
        VarValue& operator = (bool i);
        VarValue& operator = (float i);
        VarValue& operator = (const std::string& i);


        /** Assignment overload.
         *  Note that none of these assignment operators will change
         *  the VarValue's type.  Instead, the VarValue will coerce the
         *  argument into the desired type.
         *  Note that this assignment is quite
         *  sophisticated, and will attempt to parse strings into
         *  ints, floats, or booleans, as required.
         *  If no reasonable coercion of types exists, the assignment
         *  will fail and print an error message.
         *  In any event, the char* argument is copied, so the caller may
         *  freely delete or overwrite the char* passed to this method.
         */
    VarValue &operator = (const char *s);

    VarValue &operator = (const ConfigChunkPtr s);



        /** Write the value of self to out.
         *  self knows what type it is, and makes sure that it is printed
         *  in a reasonable way.  Ints and floats are printed as numbers,
         *  bools as the strings "true" and "false", strings & chunks as
         *  their string reps, etc.
         */
        friend JCCL_API(std::ostream&) operator << (std::ostream& out, const VarValue& v);
    
    };

}; // namespace jccl
#endif
