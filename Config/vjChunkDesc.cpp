/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <Config/vjChunkDesc.h>
#include <Config/vjParseUtil.h>

vjChunkDesc::vjChunkDesc () :plist() {
    name = "unnamed";
    token = "unnamed";
    help = "";
    vjPropertyDesc *d = new vjPropertyDesc("name",1,T_STRING," ");
    add (d);
}


vjChunkDesc::~vjChunkDesc() {
    cout << "a chunkdesc is getting deleted!!!" << endl;
    for (int i = 0; i < plist.size(); i++)
	delete plist[i];
}



vjChunkDesc& vjChunkDesc::operator= (const vjChunkDesc& other) {
    int i;

    if (&other == this)
	return *this;

    for (i = 0; i < plist.size(); i++)
	delete plist[i];
    plist.erase (plist.begin(), plist.end());
    
    name = other.name;
    token = other.token;
    help = other.help;
    
    plist.reserve (other.plist.size());
    for (i = 0; i < other.plist.size(); i++)
	plist.push_back ( other.plist[i]);
    
    return *this;
}



void vjChunkDesc::setName (const std::string& _name) {
    name = _name;
}



void vjChunkDesc::setToken (const std::string& _token) {
    token = _token;
}



void vjChunkDesc::setHelp (const std::string& _help) {
    help = _help;
}



std::string vjChunkDesc::getName () {
    return name;
}


std::string vjChunkDesc::getToken () {
    return token;
}


std::string vjChunkDesc::getHelp () {
    return help;
}


void vjChunkDesc::add (vjPropertyDesc *pd) {
    remove(pd->getToken());
    plist.push_back(pd);
}



vjPropertyDesc* vjChunkDesc::getPropertyDesc (const std::string& _token) {
    for (int i = 0; i < plist.size(); i++)
	if (!vjstrcasecmp (_token, plist[i]->getToken()))
	    return plist[i];
    return NULL;
}



bool vjChunkDesc::remove (const std::string& _token) {

    std::vector<vjPropertyDesc*>::iterator begin = plist.begin();
    while (begin != plist.end()) {
	if (!vjstrcasecmp ((*begin)->getToken(), _token)) {
	    delete (*begin);
	    plist.erase(begin);
	    return 1;
	}
	begin++;
    }
    return 0;
}



ostream& operator << (ostream& out, vjChunkDesc& self) {
    out << self.token << " \"" << self.name << "\" \"" 
	<< self.help << '"' << endl;
    for (int i = 0; i < self.plist.size(); i++)
	out << "  " << *(self.plist[i]) << endl;
    out << "  end" <<endl;
    return out;
}



istream& operator >> (istream& in, vjChunkDesc& self) {
    const int buflen = 512;
    char str[buflen];
    vjPropertyDesc *p;

    readString (in, str, buflen);
    self.token = str;

    readString (in, str, buflen);
    self.name = str;

    readString (in, str, buflen);
    self.help = str;

    for (int i = 0; i < self.plist.size(); i++)
	delete self.plist[i];
    self.plist.erase (self.plist.begin(), self.plist.end());

    // this could use improvement
    do {
	p = new vjPropertyDesc();
	in >> *p;
	if (!vjstrcasecmp (p->getToken(),"end")) {
	    delete p;
	    break;
	}
	self.add(p);
    } while (!in.eof());

    if (!self.getPropertyDesc ("name")) 
	self.plist.insert (self.plist.begin(), new vjPropertyDesc("name",1,T_STRING," "));
    return in;
}
