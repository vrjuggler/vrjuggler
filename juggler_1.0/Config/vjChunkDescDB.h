/*
 * vjChunkDescDB.h
 *
 * Author: Christopher Just
 *
<<<<<<< vjChunkDescDB.h
 *
 *
=======
>>>>>>> 1.5
 */


#ifndef _VJ_CHUNKDESCDB_H_
#define _VJ_CHUNKDESCDB_H_

#include <vjConfig.h>
#include <iostream.h>
#include <Config/vjChunkDesc.h>

//--------------------------------------------------------------
//: Storage class for vjChunkDescs used by the ConfigChunkDB.
//
//  A vjChunkDescDB is a container for vjChunkDescs.  Includes
//  functions to search for particular vjChunkDescs, and to read
//  and write ChunkDesc files.
//
//!PUBLIC_API
//-----------------------------------------------------------------


class vjChunkDescDB {
private:

    //:Internal storage of vjChunkDescs
    vector<vjChunkDesc*> descs;

public:

    //:Default Constructor
    //!POST: Self is created with an empty vector of vjChunkDescs
    vjChunkDescDB ();


    //:Destructor
    //!POST: Self is destroyed; all memory (including contained
    //+      vjChunkDescs) is destroyed.
    ~vjChunkDescDB ();


    //:Searches for a particular vjChunkDesc
    //!RETURNS: desc - a vjChunkDesc whose token matches _token
    //!RETURNS: NULL - if no such vjChunkDesc is found
    vjChunkDesc *getChunkDesc (char *_token);


    //:Inserts a vjChunkDesc
    //!ARGS: d - non-NULL pointer to vjChunkDesc
    //!POST: d is inserted into self, replacing any vjChunkDesc
    //+      with the same token.
    //!RETURNS: True - always.
    bool insert (vjChunkDesc *d);


    //:Removes vjChunkDesc from self
    //!ARGS: _token - a non-NULL C string
    //!POST: Any vjChunkDesc in self whose token equals _token
    //+      is removed and destroyed.
    //!RETURNS: true - if a matching chunk was found
    //!RETURNS: false - if no matching chunk was found
    bool remove (char *name);


    //:Removes all vjChunkDescs from self
    //!POST: All vjChunkDescs in self have been removed and
    //+      destroyed
    //!NOTE: This <b>can</b> be dangerous, if there exist
    //+      vjConfigChunks somewhere that rever to any of
    //+      the deleted vjChunkDescs.
    void removeAll ();


    //:Returns the number of vjChunkDescs in self.
    //!RETURNS: n - the number of vjChunkDescs in self.
    int size ();


    //:Writes self to out
    //!POST: A text representation of self is appended to out
    //!RETURNS: out
    //!NOTE: The output format is zero or more vjChunkDescs
    //+      followed by "end"
    friend ostream& operator << (ostream& out, vjChunkDescDB& self);


    //:Reads from in
    //!POST: vjChunkDescs read from in are appended to self.
    //+      vjChunkDescs previously in self are retained.
    //!NOTE: input format is zero or more vjChunkDescs followed
    //+      by "End" or eof.
    friend istream& operator >> (istream& in, vjChunkDescDB& self);


    //:Loads a chunkdesc file
    //!ARGS: fname - name of file to load
    //!POST: File is opened and vjChunkDescs are read and inserted
    //+      into self (using >>).
    //!RETURNS: true - if file was opened succesfully
    //!RETURNS: false - otherwise
    //!NOTE: Return value only deals with opening the file, and
    //+      true doesn't neccessarily mean no parsing errors
    //+      occurred.
    bool load (char *fname);


    //:Saves a chunkdesc file
    //!ARGS: fname - name of file to load
    //!POST: File is opened and vjChunkDescs are written to it
    //+      using << operator.
    //!RETURNS: true - if file was opened succesfully
    //!RETURNS: false - otherwise
    bool save (char *fname);

};

#endif




