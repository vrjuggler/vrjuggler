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


#ifndef _JCCL_CHUNKDESCDB_H_
#define _JCCL_CHUNKDESCDB_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/ChunkDesc.h>

namespace jccl {
   
//--------------------------------------------------------------
//: Storage class for ChunkDescs used by the ConfigChunkDB.
//
//  A ChunkDescDB is a container for ChunkDescs.  Includes
//  functions to search for particular ChunkDescs, and to read
//  and write ChunkDesc files.
//
// @author Christopher Just
// October 1997
//!PUBLIC_API:
//-----------------------------------------------------------------


class JCCL_CLASS_API ChunkDescDB {
private:

    //:Internal storage of ChunkDescs
    std::vector<ChunkDesc*> descs;

public:

    //:Default Constructor
    //!POST: Self is created with an empty vector of ChunkDescs
    ChunkDescDB ();


    //:Destructor
    //!POST: Self is destroyed; all memory (including contained
    //+      ChunkDescs) is destroyed.
    ~ChunkDescDB ();


    typedef std::vector<ChunkDesc*>::iterator iterator;
    typedef std::vector<ChunkDesc*>::const_iterator const_iterator;

    inline iterator begin() {
        return descs.begin();
    }

    inline const_iterator begin() const {
        return descs.begin();
    }

    inline iterator end() {
        return descs.end();
    }

    inline const_iterator end() const {
        return descs.end();
    }


    //:Searches for a particular ChunkDesc
    //!RETURNS: desc - a ChunkDesc whose token matches _token
    //!RETURNS: NULL - if no such ChunkDesc is found
    ChunkDesc *getChunkDesc (const std::string& _token);


    //:Inserts a ChunkDesc
    //!ARGS: d - non-NULL pointer to ChunkDesc
    //!POST: d is inserted into self, replacing any ChunkDesc
    //+      with the same token.
    //!RETURNS: True - always.
    bool insert (ChunkDesc *d);


    //:Inserts all ChunkDescs in db
    //!ARGS: db - a non-NULL pointer to a ChunkDescDB
    //!POST: all elements of db are inserted into self, replacing any
    //+      existing ChunkDescs with the same token.
    void insert (ChunkDescDB* db);
    

    //:Removes ChunkDesc from self
    //!ARGS: _token - a non-NULL C string
    //!POST: Any ChunkDesc in self whose token equals _token
    //+      is removed and destroyed.
    //!RETURNS: true - if a matching chunk was found
    //!RETURNS: false - if no matching chunk was found
    bool remove (const std::string& name);


    //:Removes all ChunkDescs from self
    //!POST: All ChunkDescs in self have been removed and
    //+      destroyed
    //!NOTE: This <b>can</b> be dangerous, if there exist
    //+      ConfigChunks somewhere that rever to any of
    //+      the deleted ChunkDescs.
    void removeAll ();


    //:Returns the number of ChunkDescs in self.
    //!RETURNS: n - the number of ChunkDescs in self.
    int size () const;


    //:Writes self to out
    //!POST: A text representation of self is appended to out
    //!RETURNS: out
    //!NOTE: The output format is zero or more ChunkDescs
    //+      followed by "end"
    friend std::ostream& operator << (std::ostream& out, const ChunkDescDB& self);


    //:Reads from in
    //!POST: ChunkDescs read from in are appended to self.
    //+      ChunkDescs previously in self are retained.
    //!NOTE: input format is zero or more ChunkDescs followed
    //+      by "End" or eof.
    friend std::istream& operator >> (std::istream& in, ChunkDescDB& self);


    //:Loads a chunkdesc file
    //!ARGS: fname - name of file to load
    //!POST: File is opened and ChunkDescs are read and inserted
    //+      into self (using >>).
    //!RETURNS: true - if file was opened succesfully
    //!RETURNS: false - otherwise
    //!NOTE: Return value only deals with opening the file, and
    //+      true doesn't neccessarily mean no parsing errors
    //+      occurred.
    bool load (const std::string& filename, const std::string& parentfile = "");


    //:Saves a chunkdesc file
    //!ARGS: fname - name of file to load
    //!POST: File is opened and ChunkDescs are written to it
    //+      using << operator.
    //!RETURNS: true - if file was opened succesfully
    //!RETURNS: false - otherwise
    bool save (const char *fname);

};

};
#endif
