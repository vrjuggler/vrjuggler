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


#ifndef _VJ_CHUNKDESC_H_
#define _VJ_CHUNKDESC_H_

#include <vjConfig.h>
#include <Config/vjPropertyDesc.h>
//#include <dom/DOM.hpp>

// forward declare the xerces dom node
class DOM_Node;

namespace vrj
{
   
class ConfigChunk;

//-----------------------------------------------------------------
//:Defines name and properties for a kind of ConfigChunk
//
//       Primarily, it is a name and a list of PropertyDescs.
//       ChunkDescs will probably only need to be used by the
//       ChunkDescDB, ChunkFactory, and the GUI.
//
// @author  Christopher Just
//
//!PUBLIC_API:
//------------------------------------------------------------------------
class VJ_CLASS_API ChunkDesc {

public:

    //:Identifer for this ChunkDesc - no spaces allowed
    std::string token;

    //:A longer, friendlier name (for use in GUI displays, etc.)
    std::string name;

    //:A help string of help text
    std::string help;

    //:Container for this ChunkDesc's PropertyDescs
    std::vector<PropertyDesc*> plist;

    unsigned int validation;

    ConfigChunk* default_chunk;
    // be very careful with this
    DOM_Node *default_node;

    //:Constructor
    //!POST: Self is created with name and token "Unnamed",
    //+      and with only a single PropertyDesc ("Name")
    ChunkDesc ();



    //:Copy Constructor
    ChunkDesc (const ChunkDesc& desc);



    //:Desctructor
    //!POST: Destroys self and frees all allocated memory.
    ~ChunkDesc ();



    #ifdef VJ_DEBUG
    void assertValid () const;
    #else
    inline void assertValid () const {
        ;
    }
    #endif



    typedef std::vector<PropertyDesc*>::iterator iterator;
    typedef std::vector<PropertyDesc*>::const_iterator const_iterator;

    inline iterator begin() {
        return plist.begin();
    }

    inline const_iterator begin() const {
        return plist.begin();
    }

    inline iterator end() {
        return plist.end();
    }

    inline const_iterator end() const {
        return plist.end();
    }


    //:equality operator
    // a little stricter than it needs to be.. it shouldn't care about the order of
    // propertydescs...
    bool operator== (const ChunkDesc& d) const;

    inline bool operator!= (const ChunkDesc& d) const {
        return !(*this == d);
    }


    //:Assignment operator
    //!POST: self copies the value of other
    //!ARGS: other - a ChunkDesc
    //!RETURNS: self
    ChunkDesc& operator= (const ChunkDesc& other);



    //:Sets the user-friendly name of self
    //!ARGS: _name - a string. Whitespace is allowed.
    //!NOTE: self makes a copy of the argument string
    void setName (const std::string& _name);



    //:Sets the token identifier of self
    //!ARGS: _token - a string. Whitespace is not allowed.
    //!NOTE: self makes a copy of the argument string
    void setToken (const std::string& _token);



    //:Sets the help string for self
    //!ARGS: _help - a string. Whitespace is allowed.
    void setHelp (const std::string& _help);



    const std::string& getName() const;
    const std::string& getToken() const;
    const std::string& getHelp() const;

    void setDefaultChunk (DOM_Node* n);
    void setDefaultChunk (ConfigChunk* ch) {
        default_chunk = ch;
    }
    ConfigChunk* getDefaultChunk() const;

    //:Adds a PropertyDesc to self.
    //!NOTE: Any PropertyDesc previously in self with the
    //+      same token as pd is removed.
    void add (PropertyDesc *pd);



    //:Removes named PropertyDesc from self.
    //!ARGS: _token - token to search for
    //!RETURNS: true - a PropertyDesc with that token was found
    //+         and removed.
    //!RETURNS: false - no such PropertyDesc was found.
    bool remove (const std::string& _token);


    //:Gets a PropertyDesc from self with matching token
    //!ARGS: _token - non-NULL token for the desired PropertyDesc
    //!RETURNS: pdesc - Pointer to propertydesc in self with
    //+         matching token
    //!RETURNS: NULL - if no match is found
    PropertyDesc *getPropertyDesc (const std::string& _token) const;


 
    //: Writes self to the given output stream
    friend VJ_API(std::ostream&) operator << (std::ostream& out,
                                              const ChunkDesc& self);


    //: Reads self's value from the given input stream
    friend VJ_API(std::istream&) operator >> (std::istream& in,
                                              ChunkDesc& self);

};
};
#endif
