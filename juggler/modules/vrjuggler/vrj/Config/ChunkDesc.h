/*
 * vjChunkDesc.h 
 *
 * Author: Christopher Just
 *
 * A vjChunkDesc is a description for a kind of ConfigChunk.  The vjChunkDesc
 * includes a typename and a list of property descriptions.
 *
 */

#ifndef _VJ_CHUNKDESC_H_
#define _VJ_CHUNKDESC_H_

#include <vjConfig.h>
#include <strings.h>

#ifdef VJ_OS_HPUX
#   include <float.h>
#   include <stddef.h>
#   include <vector>
#else
#   if defined(VJ_USE_PTHREADS) && ! defined(_PTHREADS)
#       define _PTHREADS
#   endif

#   include <vector.h>
#endif	/* ifdef VJ_OS_HPUX */

#include <Config/vjVarValue.h>
#include <Config/vjParseUtil.h>
#include <Config/vjEnumEntry.h>
#include <Config/vjPropertyDesc.h>


//-----------------------------------------------------------------
//:Defines name and properties for a kind of vjConfigChunk
//
// Primarily, it is a name and a list of vjPropertyDescs.
// vjChunkDescs will probably only need to be used by the 
// vjChunkDescDB, vjChunkFactory, and the GUI.
//-----------------------------------------------------------------


class vjChunkDesc {

public:

    //:Identifer for this vjChunkDesc - no spaces allowed
    char *token;

    //:A longer, friendlier name (for use in GUI displays, etc.)
    char *name;

    //:A help string of help text
    char *help;

    //:Container for this vjChunkDesc's vjPropertyDescs
    vector<vjPropertyDesc*> plist;


    //:Constructor
    //!POST: Self is created with name and token "Unnamed",
    //+      and with only a single vjPropertyDesc ("Name")
    vjChunkDesc ();


    //:Desctructor
    //!POST: Destroys self and frees all allocated memory.
    ~vjChunkDesc ();


    //:Assignment operator
    //!POST: self copies the value of other
    //!ARGS: other - a vjChunkDesc
    //!RETURNS: self
    vjChunkDesc& operator= (const vjChunkDesc& other);



    //:Sets the user-friendly name of self
    //!ARGS: _name - a non-NULL string. Whitespace is allowed.
    //!RETURNS: True - success
    //!RETURNS: False - failure (probably memory-allocation)
    //!NOTE: self makes a copy of the argument string
    bool setName (char *_name);



    //:Sets the token identifier of self
    //!ARGS: _token - a non-NULL string. Whitespace is not allowed.
    //!RETURNS: True - success
    //!RETURNS: False - failure (probably memory-allocation)
    //!NOTE: self makes a copy of the argument string
    bool setToken (char *_token);



    //:Sets the help string for self
    //!ARGS: _help - a non-NULL string. Whitespace is allowed.
    //!RETURNS: True - success
    //!RETURNS: False - failure (probably memory-allocation)
    //!NOTE: self makes a copy of the argument string
    bool setHelp (char *_help);



    //:Adds a vjPropertyDesc to self.
    //!NOTE: Any vjPropertyDesc previously in self with the
    //+      same token as pd is removed.
    void add (vjPropertyDesc *pd);



    //:Removes named vjPropertyDesc from self.
    //!ARGS: _token - token to search for
    //!RETURNS: true - a vjPropertyDesc with that token was found
    //+         and removed.
    //!RETURNS: false - no such vjPropertyDesc was found.
    bool remove (char *_token);


    //:Gets a vjPropertyDesc from self with matching token
    //!ARGS: _token - non-NULL token for the desired vjPropertyDesc
    //!RETURNS: pdesc - Pointer to propertydesc in self with
    //+         matching token
    //!RETURNS: NULL - if no match is found
    vjPropertyDesc *getPropertyDesc (char *_token);



    //: Writes self to the given output stream
    friend ostream& operator << (ostream& out, vjChunkDesc& self);


    //: Reads self's value from the given input stream
    friend istream& operator >> (istream& in, vjChunkDesc& self);

};

#endif
