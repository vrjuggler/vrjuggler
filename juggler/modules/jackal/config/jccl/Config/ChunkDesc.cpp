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


#include <Config/vjChunkDesc.h>
#include <Config/vjParseUtil.h>

vjChunkDesc::vjChunkDesc () :plist() {
    validation = 1;
    name = "unnamed";
    token = "unnamed";
    help = "";
    vjPropertyDesc *d = new vjPropertyDesc("name",1,T_STRING," ");
    add (d);
}



vjChunkDesc::vjChunkDesc (const vjChunkDesc& desc): plist() {
    validation = 1;
    *this = desc;
}



vjChunkDesc::~vjChunkDesc() {
    /* XXX: Leave it for now
    for (unsigned int i = 0; i < plist.size(); i++)
        delete plist[i];
    */
    validation = 0;
}


#ifdef VJ_DEBUG
void vjChunkDesc::assertValid () const {
    assert (validation == 1 && "Trying to use deleted vjChunkDesc");
     for (unsigned int i = 0; i < plist.size(); i++)
         plist[i]->assertValid();
}
#endif



vjChunkDesc& vjChunkDesc::operator= (const vjChunkDesc& other) {
    assertValid();
    other.assertValid();

    unsigned int i;

    if (&other == this)
        return *this;

    /* XXX: Leave it alone for now
    for (i = 0; i < plist.size(); i++)
    {
       delete plist[i];
       plist[i] = NULL;       // Overwrite dangling pointer
    }
    */
    plist.clear();

    name = other.name;
    token = other.token;
    help = other.help;

    plist.reserve (other.plist.size());
    for (i = 0; i < other.plist.size(); i++) {
        //plist.push_back ( other.plist[i]);
        plist.push_back ( new vjPropertyDesc(*(other.plist[i])));
    }

    return *this;
}



void vjChunkDesc::setName (const std::string& _name) {
    assertValid();

    name = _name;
}



void vjChunkDesc::setToken (const std::string& _token) {
    assertValid();

    token = _token;
}



void vjChunkDesc::setHelp (const std::string& _help) {
    assertValid();

    help = _help;
}



std::string vjChunkDesc::getName () {
    assertValid();

    return name;
}


std::string vjChunkDesc::getToken () {
    assertValid();

    return token;
}


std::string vjChunkDesc::getHelp () {
    assertValid();

    return help;
}


void vjChunkDesc::add (vjPropertyDesc *pd) {
    assertValid();

    remove(pd->getToken());
    plist.push_back(pd);
}



vjPropertyDesc* vjChunkDesc::getPropertyDesc (const std::string& _token) {
    assertValid();

    for (unsigned int i = 0; i < plist.size(); i++)
        if (!vjstrcasecmp (_token, plist[i]->getToken()))
            return plist[i];
    return NULL;
}



bool vjChunkDesc::remove (const std::string& _token)
{
    assertValid();

    std::vector<vjPropertyDesc*>::iterator cur_desc = plist.begin();
    while (cur_desc != plist.end()) {
        if (!vjstrcasecmp ((*cur_desc)->getToken(), _token)) {
            /* XXX:
               delete (*cur_desc);
               *cur_desc = NULL;
               */
            cur_desc = plist.erase(cur_desc);
            return true;
        }
        cur_desc++;
    }
    return false;
}



std::ostream& operator << (std::ostream& out, vjChunkDesc& self) {
    self.assertValid();

    out << self.token.c_str() << " \"" << self.name.c_str() << "\" \"" 
        << self.help.c_str() << '"' << std::endl;
    for (unsigned int i = 0; i < self.plist.size(); i++)
        out << "  " << *(self.plist[i]) << std::endl;
    out << "  end" << std::endl;
    return out;
}


std::istream& operator >> (std::istream& in, vjChunkDesc& self)
{
    self.assertValid();

    const int buflen = 512;
    char str[buflen];
    vjPropertyDesc *p;
    
    readString (in, str, buflen);
    self.token = str;

    readString (in, str, buflen);
    self.name = str;

    readString (in, str, buflen);
    self.help = str;

    for (unsigned int i = 0; i < self.plist.size(); i++) {
        /* XXX: Leave the memory for now.  Need to fix
           delete self.plist[i];
           self.plist[i] = NULL;                  // Get rid of dangling pointer
        */
    }
    self.plist.clear();
    
    // this could use improvement
    do {
        p = new vjPropertyDesc();
        in >> *p;
        if (!vjstrcasecmp (p->getToken(),std::string("end"))) {
            /* XXX:
               delete p;
            */
            break;
        }
        self.add(p);
    } while (!in.eof());
    
    if (!self.getPropertyDesc ("name"))
        self.plist.insert (self.plist.begin(), new vjPropertyDesc("name",1,T_STRING," "));
    return in;
}



//:equality operator
// a little stricter than it needs to be.. it shouldn't care about the order of
// propertydescs...
bool vjChunkDesc::operator== (const vjChunkDesc& d) {
    assertValid();
    d.assertValid();

    if (vjstrcasecmp (token, d.token))
        return false;
    if (vjstrcasecmp (name, d.name))
        return false;
    if (plist.size() != d.plist.size())
        return false;
    for (unsigned int i = 0; i < plist.size(); i++)
        if ((*plist[i]) != (*plist[i]))
            return false;
    return true;
}
