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


// Implementation of vjConfigChunk DB
//
// Author: Christopher Just


#include <Config/vjConfigChunkDB.h>
#include <Config/vjChunkFactory.h>
#include <Config/vjParseUtil.h>
#include <Kernel/vjDebug.h>
#include <Config/vjConfigTokens.h>

#include <sys/types.h>


vjConfigChunkDB::vjConfigChunkDB (): chunks() {
    ;
}



vjConfigChunkDB::~vjConfigChunkDB () {
    // if ConfigChunkDBs ever start doing memory management of their
    // ConfigChunks, do it here.
}



//: copy constructor
vjConfigChunkDB::vjConfigChunkDB (vjConfigChunkDB& db): chunks() {
    *this = db;
}



vjConfigChunkDB& vjConfigChunkDB::operator = (vjConfigChunkDB& db) {
    unsigned int i;
    //for (i = 0; i < chunks.size(); i++)
    //    delete chunks[i];
    chunks.clear();
    for (i = 0; i < db.chunks.size(); i++) {
        chunks.push_back (new vjConfigChunk(*(db.chunks[i])));
    }
    return *this;
}



vjConfigChunk* vjConfigChunkDB::getChunk (const std::string& name) {
    /* returns a chunk with the given name, if such exists, or NULL.
     */
    for (unsigned int i = 0; i < chunks.size(); i++) {
        if (!vjstrcasecmp (name, chunks[i]->getProperty("name")))
            return chunks[i];
    }
    return NULL;
}



// Return a copy of the chunks vector
std::vector<vjConfigChunk*> vjConfigChunkDB::getChunks() {
    return chunks;
}



// Add the given chunks to the end of the chunk list
void vjConfigChunkDB::addChunks(std::vector<vjConfigChunk*> new_chunks) {
    // no! must make copies of all chunks. sigh...
    //chunks.insert(chunks.end(), new_chunks.begin(), new_chunks.end());
    for (unsigned int i = 0; i < new_chunks.size(); i++)
        addChunk (new vjConfigChunk (*new_chunks[i]));
}



void vjConfigChunkDB::addChunks(vjConfigChunkDB *db) {
    addChunks (db->chunks);
}



void vjConfigChunkDB::addChunk(vjConfigChunk* new_chunk) {
    removeNamed (new_chunk->getProperty("Name"));
    chunks.push_back (new_chunk);
}




// GetMatching: These functions return a vector of all chunks with a
// property named by the first argument, and a value defined by the
// second argument.  The returned vector may be empty.
// NOTE:  The caller is responsible for delete()ing the vector, but not
// its contents.
std::vector<vjConfigChunk*>* vjConfigChunkDB::getMatching (const std::string& property, const std::string value) {
    std::vector<vjConfigChunk*>* v = new std::vector<vjConfigChunk*>;

    for (unsigned int i = 0; i < chunks.size(); i++) {
        if (!vjstrcasecmp (value, chunks[i]->getProperty(property)))
            v->push_back(chunks[i]);
    }
    return v;
}

std::vector<vjConfigChunk*>* vjConfigChunkDB::getMatching (const std::string& property, int value) {
    int c;
    std::vector<vjConfigChunk*>* v = new std::vector<vjConfigChunk*>;
    for (unsigned int i = 0; i < chunks.size(); i++) {
        c = chunks[i]->getProperty(property);
        if (c == value)
            v->push_back(chunks[i]);
    }
    return v;
}


std::vector<vjConfigChunk*>* vjConfigChunkDB::getMatching (const std::string& property, float value) {
    float c;
    std::vector<vjConfigChunk*>* v = new std::vector<vjConfigChunk*>;
    for (unsigned int i = 0; i < chunks.size(); i++) {
        c = chunks[i]->getProperty(property);
        if (c == value)
            v->push_back(chunks[i]);
    }
    return v;
}



bool vjConfigChunkDB::erase () {
    /* removes all chunks from self (and frees them)
     */
    //for (unsigned int i = 0; i < chunks.size(); i++)
    //delete (chunks[i]);
    chunks.clear();
    return true;
}



// Removes (and frees all memory associated with) all chunks with a property
// named by the first argument with a value defined by the second argument.
int vjConfigChunkDB::removeMatching (const std::string& property, int value) {
    int i = 0;
    int c;
    std::vector<vjConfigChunk*>::iterator cur_chunk = chunks.begin();
    while (cur_chunk != chunks.end()) {
        c = (*cur_chunk)->getProperty(property);
        if (c == value) {
            //delete (*next);
            cur_chunk = chunks.erase(cur_chunk);
            i++;
        }
        else
            cur_chunk++;
    }
    return i;
}

int vjConfigChunkDB::removeMatching (const std::string& property, float value) {
    int i = 0;
    float c;

    std::vector<vjConfigChunk*>::iterator cur_chunk = chunks.begin();
    while (cur_chunk != chunks.end()) {
        c = (*cur_chunk)->getProperty(property);
        if (c == value) {
            //delete (*cur_chunk);
            cur_chunk = chunks.erase(cur_chunk);
            i++;
        }
        else
            cur_chunk++;
    }
    return i;
}

int vjConfigChunkDB::removeMatching (const std::string& property, const std::string& value) {

    int i = 0;
    std::vector<vjConfigChunk*>::iterator cur_chunk = chunks.begin();
    while (cur_chunk != chunks.end()) {
        vjVarValue v = ((*cur_chunk)->getProperty(property));
        if (((v.getType() == T_STRING) || (v.getType() == T_STRING))
            &&  (!vjstrcasecmp (value, (std::string)v))) {
            //delete (*begin);
            cur_chunk = chunks.erase(cur_chunk);
            i++;
        }
        else
            cur_chunk++;
    }

    return i;
}



//: Sorts the chunks based on dependancies of chunk ptrs
//! PRE: we need a "good object"
//! MODIFIES: self.  We move the objects around so they are sorted
//! ARGS: auxChunks - Auxilary chunks that have been loaded already
//! POST: Topologically sorted
// Copy the chunks over to a new list.  Repetatively try to
// find an item in the source list that already has it's dependencies
// copied into the dst list.  Do this iteratively until done or
// until fail.
int vjConfigChunkDB::dependencySort(vjConfigChunkDB* auxChunks)
{
    // Print out dependancies
#ifdef VJ_DEBUG
    vjDEBUG_BEGIN(vjDBG_CONFIG,4) << "---- Dependencies -----------\n" << vjDEBUG_FLUSH;
    for (unsigned int i=0;i<chunks.size();i++) {
        vjDEBUG(vjDBG_CONFIG,4) << "Chunk:" << chunks[i]->getProperty("name")
                                << std::endl << "\tDepends on:\n"
                                << vjDEBUG_FLUSH;
        std::vector<std::string> deps = chunks[i]->getChunkPtrDependencies();
        if (deps.size() > 0) {
            for (unsigned int j=0;j<deps.size();j++)
                vjDEBUG(vjDBG_CONFIG,4) << "   " << j << ": "
                                        << deps[j].c_str() << std::endl
                                        << vjDEBUG_FLUSH;
        } else {
            vjDEBUG(vjDBG_CONFIG,4) << "   Nothing.\n" << vjDEBUG_FLUSH;
        }
    }
    vjDEBUG_END(vjDBG_CONFIG,4) << "-----------------------------\n" << vjDEBUG_FLUSH;
#endif


    // --- THE SORT --- //
    // Create new src list to work from
    // Targetting the local data
    // So basically, we take an element from the src list one at a time
    // If it's dependencies are already in the local list, add it to the local list
    // else go on to the next one
    // Kinda like an insertion sort
    std::vector<vjConfigChunk*> src_chunks = chunks;
    chunks = std::vector<vjConfigChunk*>(0);        // Chunks is the local data - Zero it out to start

    bool dep_pass(true);             // Flag for Pass dependency check
    std::vector<std::string> deps;   // Dependencies of current item
    std::vector<vjConfigChunk*>::iterator cur_item = src_chunks.begin();          // The current src item to look at

    while (cur_item != src_chunks.end()) {          // While not at end of src list
        vjDEBUG(vjDBG_CONFIG,4) << "Checking depencies for: " << (*cur_item)->getProperty("name") << "\n" << vjDEBUG_FLUSH;

        deps = (*cur_item)->getChunkPtrDependencies();             // Get src dependencies
        for (unsigned int dep_num=0;dep_num<deps.size();dep_num++) {  // For each dependency
            bool dep_not_found = (getChunk(deps[dep_num]) == NULL);
            bool aux_dep_not_found = ((auxChunks == NULL) ||
                                      (auxChunks->getChunk(deps[dep_num]) == NULL));

            // If dependency not in list yet or in aux buffer
            // If (not in src && (!aux exists || not in aux))
            if ( dep_not_found && aux_dep_not_found )
                dep_pass = false;                                   // Failed check (we don't pass)
        }

        if (dep_pass) {        // If all dependencies are accounted for
            addChunk(*cur_item);        // Copy src to dst
            src_chunks.erase(cur_item);         // Erase it from source
            cur_item = src_chunks.begin();      // Goto first item
        } else
            cur_item++;             // Try next item

        dep_pass = true;           // Reset to passing
    }

    // ASSERT: (Done with sort)
    //   Either, all depencies have been accounted for and the src list is empty
    //   OR we went all the way through list without finding an item that passes

    if (src_chunks.size() > 0) {     // Items left, so we failed to get all dependencies
        // ouput error
        for (unsigned int i=0;i<src_chunks.size();i++) {
            vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << " Dependency error:  Chunk:" << src_chunks[i]->getProperty("name")
                                   << "\tDepends on: \n" << vjDEBUG_FLUSH;
            std::vector<std::string> deps = src_chunks[i]->getChunkPtrDependencies();
            if (deps.size() > 0) {
                for (unsigned int j=0;j<deps.size();j++)
                    vjDEBUG(vjDBG_ERROR,0) << "\tdep " << j << ": "
                                           << deps[j].c_str() << std::endl
                                           << vjDEBUG_FLUSH;
            } else {
                vjDEBUG(vjDBG_ERROR,0) << "Nothing.\n" << vjDEBUG_FLUSH;
            }
            vjDEBUG(vjDBG_ERROR,0) << "Check for undefined devices that others depend upon.\n" << vjDEBUG_FLUSH;
        }
        chunks.insert(chunks.end(), src_chunks.begin(), src_chunks.end());   // Copy over the rest anyway

        return -1;
    } else {
        // Print out sorted dependancies
#ifdef VJ_DEBUG

        vjDEBUG_BEGIN(vjDBG_CONFIG,4) << "---- After sort ----" << std::endl << vjDEBUG_FLUSH;
        for (unsigned int i=0;i<chunks.size();i++) {
            vjDEBUG(vjDBG_CONFIG,4) << "Chunk:" << chunks[i]->getProperty("name") << std::endl
                                    << "\tDepends on:\n" << vjDEBUG_FLUSH;
            std::vector<std::string> deps = chunks[i]->getChunkPtrDependencies();
            if (deps.size() > 0) {
                for (unsigned int j=0;j<deps.size();j++)
                    vjDEBUG(vjDBG_CONFIG,4) << "   " << j << ": " << deps[j].c_str() << std::endl << vjDEBUG_FLUSH;
            } else {
                vjDEBUG(vjDBG_CONFIG,4) << "   Nothing.\n" << vjDEBUG_FLUSH;
            }
        }
        vjDEBUG_END(vjDBG_CONFIG,4) << "-----------------\n" << vjDEBUG_FLUSH;
#endif

        return 0;      // Success
    }
}



/* IO functions: */

std::ostream& operator << (std::ostream& out, vjConfigChunkDB& self) {
    for (unsigned int i = 0; i < self.chunks.size(); i++) {
        out << *(self.chunks[i]) << std::endl;
    }
    out << "End" << std::endl;
    return out;
}



std::istream& operator >> (std::istream& in, vjConfigChunkDB& self) {

    const int bufsize = 512;
    char str[bufsize];
    vjConfigChunk *ch;

    do {
        if (!readString (in, str, bufsize))
            break; /* eof */
        if (!strcasecmp (str, end_TOKEN))
            break;

        std::string newstr = str;
        ch = vjChunkFactory::instance()->createChunk (newstr);
        if (ch == NULL) {
            vjDEBUG(vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << " Unknown Chunk type: " << str << std::endl
                                   << vjDEBUG_FLUSH;
            // skip to end of chunk
            while (strcasecmp (str, end_TOKEN)) {
                if (0 == readString (in, str, bufsize))
                    break;
                //std::cerr << "read " << str << std::endl;
            }
        }
        else {
            in >> *ch;

            //std::cerr << "read chunk: " << *ch << std::endl;

            if (!vjstrcasecmp (ch->getType(), "vjIncludeFile")) {
                // this is another one of those bits of code where we need to make some
                // real decisions about memory management of ConfigChunks
                std::string s = ch->getProperty ("Name");
                vjConfigChunkDB newdb;
                newdb.load (s, self.file_name);
                self.addChunks(&newdb);
            }
            else if (!vjstrcasecmp (ch->getType(), "vjIncludeDescFile")) {
                // the descs could be needed by everybody else in this file,
                // so load 'em now...
                std::string s = ch->getProperty ("Name");
                vjChunkDescDB newdb;
                newdb.load (s, self.file_name);
                vjChunkFactory::instance()->addDescs (&newdb);
            }
            else {
                // just a plain old chunk to add in...
                self.addChunk(ch);
            }
        }
    } while (!in.eof());

    vjDEBUG(vjDBG_CONFIG,3) << "vjConfigChunkDB::>> : Finished - "
                            << self.chunks.size() << " chunks read."
                            << std::endl << vjDEBUG_FLUSH;

    return in;
}






bool vjConfigChunkDB::load (const std::string& filename, const std::string& parentfile) {
    file_name = demangleFileName (filename, parentfile);

    std::ifstream in(file_name.c_str());

    vjDEBUG(vjDBG_CONFIG,3) << "vjConfigChunkDB::load(): opening file " << file_name.c_str() << " -- " << vjDEBUG_FLUSH;


    if (!in) {
        vjDEBUG(vjDBG_ALL,0) << "\nvjConfigChunkDB::load(): Unable to open file '"
                             << file_name.c_str() << "'" << std::endl << vjDEBUG_FLUSH;
        return false;
    }
    vjDEBUG(vjDBG_CONFIG,5) << " succeeded.\n" << vjDEBUG_FLUSH;
    in >> *this;
    vjDEBUG(vjDBG_CONFIG,3) << " finished.. read " << chunks.size() << " chunks\n"
                            << vjDEBUG_FLUSH;
    return true;
}



bool vjConfigChunkDB::save (const std::string& fname) {

    std::ofstream out(fname.c_str());
    if (!out) {
        vjDEBUG(vjDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") << " vjConfigChunkDB::save() - Unable to open file '"
                               << fname.c_str() << "'\n" << vjDEBUG_FLUSH;
        return false;
    }
    out << *this;
    return true;
}



bool vjConfigChunkDB::isEmpty() {
    return (chunks.size() == 0);
}



void vjConfigChunkDB::removeAll() {
    // just an alias
    erase();
}



int vjConfigChunkDB::removeNamed (const std::string& name) {
    return removeMatching ("Name", name);
}

