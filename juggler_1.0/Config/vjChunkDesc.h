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


class vjConfigChunk;
class DOM_Node;

//-----------------------------------------------------------------
//:Defines name and properties for a kind of vjConfigChunk
//
//       Primarily, it is a name and a list of vjPropertyDescs.
//       vjChunkDescs will probably only need to be used by the
//       vjChunkDescDB, vjChunkFactory, and the GUI.
//
// @author  Christopher Just
//
//!PUBLIC_API:
//------------------------------------------------------------------------
class VJ_CLASS_API vjChunkDesc {

public:

    //:Identifer for this vjChunkDesc - no spaces allowed
    std::string token;

    //:A longer, friendlier name (for use in GUI displays, etc.)
    std::string name;

    //:A help string of help text
    std::string help;

    //:Container for this vjChunkDesc's vjPropertyDescs
    std::vector<vjPropertyDesc*> plist;

    unsigned int validation;

    vjConfigChunk* default_chunk;
    // be very careful with this
    DOM_Node *default_node;

    //:Constructor
    //!POST: Self is created with name and token "Unnamed",
    //+      and with only a single vjPropertyDesc ("Name")
    vjChunkDesc ();



    //:Copy Constructor
    vjChunkDesc (const vjChunkDesc& desc);



    //:Desctructor
    //!POST: Destroys self and frees all allocated memory.
    ~vjChunkDesc ();



    #ifdef VJ_DEBUG
    void assertValid () const;
    #else
    inline void assertValid () const {
        ;
    }
    #endif



    typedef std::vector<vjPropertyDesc*>::iterator iterator;
    typedef std::vector<vjPropertyDesc*>::const_iterator const_iterator;

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
    bool operator== (const vjChunkDesc& d) const;

    inline bool operator!= (const vjChunkDesc& d) const {
        return !(*this == d);
    }


    //:Assignment operator
    //!POST: self copies the value of other
    //!ARGS: other - a vjChunkDesc
    //!RETURNS: self
    vjChunkDesc& operator= (const vjChunkDesc& other);



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
    void setDefaultChunk (vjConfigChunk* ch) {
        default_chunk = ch;
    }
    vjConfigChunk* getDefaultChunk() const;

    //:Adds a vjPropertyDesc to self.
    //!NOTE: Any vjPropertyDesc previously in self with the
    //+      same token as pd is removed.
    void add (vjPropertyDesc *pd);



    //:Removes named vjPropertyDesc from self.
    //!ARGS: _token - token to search for
    //!RETURNS: true - a vjPropertyDesc with that token was found
    //+         and removed.
    //!RETURNS: false - no such vjPropertyDesc was found.
    bool remove (const std::string& _token);


    //:Gets a vjPropertyDesc from self with matching token
    //!ARGS: _token - non-NULL token for the desired vjPropertyDesc
    //!RETURNS: pdesc - Pointer to propertydesc in self with
    //+         matching token
    //!RETURNS: NULL - if no match is found
    vjPropertyDesc *getPropertyDesc (const std::string& _token) const;


 
    //: Writes self to the given output stream
    friend VJ_API(std::ostream&) operator << (std::ostream& out,
                                              const vjChunkDesc& self);


    //: Reads self's value from the given input stream
    friend VJ_API(std::istream&) operator >> (std::istream& in,
                                              vjChunkDesc& self);

};

#endif
