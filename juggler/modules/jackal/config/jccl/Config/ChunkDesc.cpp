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

#include <jccl/jcclConfig.h>

#include <jccl/Config/vjChunkDesc.h>
#include <jccl/Config/vjParseUtil.h>
#include <vpr/Util/Assert.h>
#include <jccl/Config/vjConfigIO.h>
#include <jccl/Config/vjXMLConfigIOHandler.h>


namespace jccl {
   
ChunkDesc::ChunkDesc () :plist() {
    validation = 1;
    name = "unnamed";
    token = "unnamed";
    help = "";
    default_chunk = 0;
    default_node = 0;
    PropertyDesc *d = new PropertyDesc("name",1,T_STRING," ");
    add (d);
}



ChunkDesc::ChunkDesc (const ChunkDesc& desc): plist() {
    validation = 1;
    *this = desc;
}



ChunkDesc::~ChunkDesc() {
    /* XXX: Leave it for now
    for (unsigned int i = 0; i < plist.size(); i++)
        delete plist[i];
    */
    // delete default_chunk;
    validation = 0;
}


#ifdef VJ_DEBUG
void ChunkDesc::assertValid () const {
    vprASSERT (validation == 1 && "Trying to use deleted ChunkDesc");
    for (unsigned int i = 0; i < plist.size(); i++)
        plist[i]->assertValid();
}
#endif



ChunkDesc& ChunkDesc::operator= (const ChunkDesc& other) {
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
    default_node = other.default_node;
    default_chunk = other.default_chunk;

    plist.reserve (other.plist.size());
    for (i = 0; i < other.plist.size(); i++) {
        //plist.push_back ( other.plist[i]);
        plist.push_back ( new PropertyDesc(*(other.plist[i])));
    }

    return *this;
}



void ChunkDesc::setName (const std::string& _name) {
    assertValid();

    name = _name;
}



void ChunkDesc::setToken (const std::string& _token) {
    assertValid();

    token = _token;
}



void ChunkDesc::setHelp (const std::string& _help) {
    assertValid();

    help = _help;
}



const std::string& ChunkDesc::getName () const {
    assertValid();

    return name;
}


const std::string& ChunkDesc::getToken () const {
    assertValid();

    return token;
}


const std::string& ChunkDesc::getHelp () const {
    assertValid();

    return help;
}


void ChunkDesc::add (PropertyDesc *pd) {
    assertValid();

    remove(pd->getToken());
    plist.push_back(pd);
}


void ChunkDesc::setDefaultChunk (DOM_Node* n) {
    default_node = new DOM_Node;
    *default_node = *n;
    default_chunk = 0; // memory leak?
}

ConfigChunk* ChunkDesc::getDefaultChunk() const {
    // thread safety???
    if ((default_chunk == NULL) && (default_node != 0)) {
        XMLConfigIOHandler* handler = (XMLConfigIOHandler*)ConfigIO::instance()->getHandler("xml_config");
        ConfigChunk* ch = handler->buildConfigChunk (*default_node, false);
        if (ch) {
            // this is a cheat
            const_cast<ChunkDesc*>(this)->default_chunk = ch;
        }
        ConfigIO::instance()->releaseHandler (handler);
    }
//      if (default_chunk)
//          std::cout << "returning a default chunk: " << *default_chunk << std::endl;
//      else
//          std::cout << "getDefaultChunk return null" << std::endl;
    return default_chunk;
}


PropertyDesc* ChunkDesc::getPropertyDesc (const std::string& _token) const {
    assertValid();

    for (unsigned int i = 0; i < plist.size(); i++)
        if (!vjstrcasecmp (_token, plist[i]->getToken()))
            return plist[i];
    return NULL;
}



bool ChunkDesc::remove (const std::string& _token)
{
    assertValid();

    std::vector<PropertyDesc*>::iterator cur_desc = plist.begin();
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



VJ_IMPLEMENT(std::ostream&) operator << (std::ostream& out,
                                         const ChunkDesc& self)
{
    self.assertValid();

    out << self.token.c_str() << " \"" << self.name.c_str() << "\" \"" 
        << self.help.c_str() << '"' << std::endl;
    for (unsigned int i = 0; i < self.plist.size(); i++)
        out << "  " << *(self.plist[i]) << std::endl;
    out << "  end" << std::endl;
    return out;
}


VJ_IMPLEMENT(std::istream&) operator >> (std::istream& in, ChunkDesc& self)
{
    self.assertValid();

    const int buflen = 512;
    char str[buflen];
    PropertyDesc *p;
    
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
        p = new PropertyDesc();
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
        self.plist.insert (self.plist.begin(), new PropertyDesc("name",1,T_STRING," "));
    return in;
}



//:equality operator
// a little stricter than it needs to be.. it shouldn't care about the order of
// propertydescs...
bool ChunkDesc::operator== (const ChunkDesc& d) const {
    assertValid();
    d.assertValid();

    if (vjstrcasecmp (token, d.token))
        return false;
    if (vjstrcasecmp (name, d.name))
        return false;
    if (plist.size() != d.plist.size())
        return false;
    for (unsigned int i = 0; i < plist.size(); i++)
        if ((*plist[i]) != (*d.plist[i]))
            return false;
    return true;
}

};
