/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



#include <vjConfig.h>
#include <sys/types.h>

#include <Kernel/vjDebug.h>
#include <Config/vjChunkDescDB.h>
#include <Config/vjParseUtil.h>


vjChunkDescDB::vjChunkDescDB (): descs() {
    ;
}



vjChunkDescDB::~vjChunkDescDB() {
    removeAll();
}


vjChunkDesc* vjChunkDescDB::getChunkDesc (const std::string& _token) {
    for (int i = 0; i < descs.size(); i++)
        if (!vjstrcasecmp (descs[i]->token, _token))
            return descs[i];
    return NULL;
}



bool vjChunkDescDB::insert (vjChunkDesc *d) {
    for (int i = 0; i < descs.size(); i++)
        if (!vjstrcasecmp (descs[i]->token, d->token)) {
            delete (descs[i]);
            descs[i] = d;
            return true;
        }
    descs.push_back(d);
    return true;
}



void vjChunkDescDB::insert (vjChunkDescDB* db) {
    std::vector<vjChunkDesc*>::iterator begin = db->descs.begin();
    while (begin != db->descs.end()) {
        insert (new vjChunkDesc(**begin));
        begin++;
    }
}



bool vjChunkDescDB::remove (const std::string& tok) {

    std::vector<vjChunkDesc*>::iterator begin = descs.begin();
    while (begin != descs.end()) {
        if (!vjstrcasecmp ((*begin)->token, tok)) {
            delete(*begin);
            descs.erase(begin);
            return true;
        }
        else
            begin++;
    }
    return false;
}



void vjChunkDescDB::removeAll () {
    std::vector<vjChunkDesc*>::iterator i = descs.begin();
    while (i != descs.end()) {
        delete (*i);
        i++;
    }
    descs.erase(descs.begin(), descs.end());
}



int vjChunkDescDB::size () {
    return descs.size();
}



ostream& operator << (ostream& out, vjChunkDescDB& self) {
    for (int i = 0; i < self.descs.size(); i++)
        out << "Chunk " << *(self.descs[i]) << endl;
    out << "End" <<endl;
    return out;
}



istream& operator >> (istream& in, vjChunkDescDB& self) {
    const int buflen = 512;
    char str[buflen];
    vjChunkDesc *ch;

    for (;;) {
        if (readString (in, str, buflen) == 0)
            break; /* eof */
        else if (!strcasecmp (str, "chunk")) {
            ch = new vjChunkDesc();
            in >> *ch;
            self.insert(ch);
        }
        else if (!strcasecmp (str, "end"))
            break;
        else {
            vjDEBUG(vjDBG_ERROR,1) << "Unexpected symbol parsing vjChunkDescDB: '"
                       << str <<"'"<< endl << vjDEBUG_FLUSH;
        }
    }
    vjDEBUG(vjDBG_CONFIG,3) << "vjChunkDescDB::>> : Finished - " << self.descs.size()
               << " descriptions read." << endl << vjDEBUG_FLUSH;
    return in;
}



bool vjChunkDescDB::load (const char *fname) {
    ifstream in(fname);

    if (!in) {
        vjDEBUG(vjDBG_ERROR,0) << "vjChunkDescDB::load(): Unable to open file '"
                   << fname << "'" << endl << vjDEBUG_FLUSH;
        return false;
    }
    in >> *this;
    return true;
}



bool vjChunkDescDB::save (const char *fname) {
    ofstream out(fname);
    if (!out) {
        vjDEBUG(vjDBG_ERROR,0) << "vjChunkDescDB::save(): Unable to open file '"
                   << fname << "'" << endl << vjDEBUG_FLUSH;
        return false;
    }
    out << *this;
    return true;
}
