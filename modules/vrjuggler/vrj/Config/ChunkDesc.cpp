

#include <vjConfig.h>
#include <Config/vjChunkDesc.h>

vjChunkDesc::vjChunkDesc () :plist() {
  name = token = help = NULL;
  setName("");
  setToken("");
  setHelp("");
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



bool vjChunkDesc::add (vjPropertyDesc *pd) {
  remove(pd->token);
  plist.push_back(pd);
  return true;
}



vjPropertyDesc* vjChunkDesc::getPropertyDesc (char *tok) {
  for (int i = 0; i < plist.size(); i++)
    if (!strcasecmp (tok, plist[i]->token))
      return plist[i];
  return NULL;
}



bool vjChunkDesc::remove (char *tok) {
    if (!tok)
	return 0;

  vector<vjPropertyDesc*>::iterator begin = plist.begin();
  while (begin != plist.end()) {
    if (!strcasecmp ((*begin)->token, tok)) {
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
    //cout << "read token token: " << str << endl;
    self.token = new char[strlen(str)+1];
    strcpy (self.token, str);
    readString (in, str, 256);
    //cout << "read name token: " << str << endl;
    self.name = new char[strlen(str)+1];
    strcpy (self.name, str);
    readString (in, str, 256);
    //cout << "read help token: " << str << endl;
    self.help = new char[strlen(str)+1];
    strcpy (self.help, str);

    do {
	p = new vjPropertyDesc();
	in >> *p;
	if (p->token && (!strcasecmp(p->token,"end"))) {
	    delete p;
	    break;
	}
	self.add(p);
    } while (!in.eof());
    return in;
}




