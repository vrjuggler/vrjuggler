

#include <Config/vjChunkDesc.h>

vjChunkDesc::vjChunkDesc () :plist() {
    name = token = help = NULL;
    setName ("unnamed");
    setToken ("unnamed");
    setHelp ("");
    vjPropertyDesc *d = new vjPropertyDesc("name",1,T_STRING," ");
    add (d);
}


vjChunkDesc::~vjChunkDesc() {

    if (name)
	delete name;
    if (token)
	delete token;
    if (help)
	delete help;
    for (int i = 0; i < plist.size(); i++)
	delete plist[i];
}



vjChunkDesc& vjChunkDesc::operator= (const vjChunkDesc& other) {
    int i;

    if (name)
	delete name;
    if (help)
	delete help;
    if (token)
	delete token;
    for (i = 0; i < plist.size(); i++)
	delete plist[i];
    plist.erase (plist.begin(), plist.end());
    
    name = strdup (other.name);
    token = strdup (other.token);
    help = strdup (other.help);
    
    plist.reserve (other.plist.size());
    for (i = 0; i < other.plist.size(); i++)
	plist.push_back ( other.plist[i]);
    
    return *this;
}



bool vjChunkDesc::setName (char *_name) {
    if (name)
	delete name;
    name = strdup (_name);
    return (name != NULL);
}



bool vjChunkDesc::setToken (char *_token) {
    if (token)
	delete token;
    token = strdup (_token);
    return (token != NULL);
}



bool vjChunkDesc::setHelp (char *_help) {
    if (help)
	delete help;
    help = strdup (_help);
    return (token != NULL);
}



void vjChunkDesc::add (vjPropertyDesc *pd) {
    remove(pd->token);
    plist.push_back(pd);
}



vjPropertyDesc* vjChunkDesc::getPropertyDesc (char *_token) {
    for (int i = 0; i < plist.size(); i++)
	if (!strcasecmp (_token, plist[i]->token))
	    return plist[i];
    return NULL;
}



bool vjChunkDesc::remove (char *_token) {
    if (!_token)
	return 0;

    std::vector<vjPropertyDesc*>::iterator begin = plist.begin();
    while (begin != plist.end()) {
	if (!strcasecmp ((*begin)->token, _token)) {
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
    char str[256];
    vjPropertyDesc *p;

    readString (in, str, 256);
    self.setToken (str);

    readString (in, str, 256);
    self.setName (str);

    readString (in, str, 256);
    self.setHelp (str);

    for (int i = 0; i < self.plist.size(); i++)
	delete self.plist[i];
    self.plist.erase (self.plist.begin(), self.plist.end());

    // this could use improvement
    do {
	p = new vjPropertyDesc();
	in >> *p;
	if (p->token && (!strcasecmp(p->token,"end"))) {
	    delete p;
	    break;
	}
	self.add(p);
    } while (!in.eof());

    if (!self.getPropertyDesc ("name")) 
	self.plist.insert (self.plist.begin(), new vjPropertyDesc("name",1,T_STRING," "));
    return in;
}




