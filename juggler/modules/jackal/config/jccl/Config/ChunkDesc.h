/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#ifndef _VJ_CHUNKDESC_H_
#define _VJ_CHUNKDESC_H_

#include <vjConfig.h>
#include <Config/vjPropertyDesc.h>

#ifdef VJ_OS_HPUX
#   include <float.h>
#   include <stddef.h>
#endif



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
class vjChunkDesc {

public:

    //:Identifer for this vjChunkDesc - no spaces allowed
    std::string token;

    //:A longer, friendlier name (for use in GUI displays, etc.)
    std::string name;

    //:A help string of help text
    std::string help;

    //:Container for this vjChunkDesc's vjPropertyDescs
    std::vector<vjPropertyDesc*> plist;


    //:Constructor
    //!POST: Self is created with name and token "Unnamed",
    //+      and with only a single vjPropertyDesc ("Name")
    vjChunkDesc ();



    //:Copy Constructor
    vjChunkDesc (vjChunkDesc& desc);



    //:Desctructor
    //!POST: Destroys self and frees all allocated memory.
    ~vjChunkDesc ();


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



    std::string getName();
    std::string getToken();
    std::string getHelp();


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
    vjPropertyDesc *getPropertyDesc (const std::string& _token);



    //: Writes self to the given output stream
    friend ostream& operator << (ostream& out, vjChunkDesc& self);


    //: Reads self's value from the given input stream
    friend istream& operator >> (istream& in, vjChunkDesc& self);

};

#endif
