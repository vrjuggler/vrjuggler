

#include <config.h>
#include <Config/vjChunkDesc.h>

vjChunkDesc::vjChunkDesc () :plist() {
  setName("");
  vjPropertyDesc *d = new vjPropertyDesc("name",1,T_STRING," ");
  add (d);
}

vjChunkDesc::~vjChunkDesc() {
  
  delete name;
  for (int i = 0; i < plist.size(); i++)
    delete plist[i];

}


bool vjChunkDesc::add (vjPropertyDesc *pd) {
  if (pd->name)
    remove(pd->name);
  plist.push_back(pd);
  return true;
}

vjPropertyDesc* vjChunkDesc::getPropertyDesc (char *name) {
  for (int i = 0; i < plist.size(); i++)
    if (!strcasecmp (name, plist[i]->name))
      return plist[i];
  return NULL;
}

bool vjChunkDesc::remove (char *n) {

  vector<vjPropertyDesc*>::iterator begin = plist.begin();
  while (begin != plist.end()) {
    if ((*begin)->name && (!strcasecmp ((*begin)->name, n))) {
      delete (*begin);
      plist.erase(begin);
      return 1;
    }
    begin++;
  }
  return 0;
}


ostream& operator << (ostream& out, vjChunkDesc& self) {
  out << self.name << endl;
  for (int i = 0; i < self.plist.size(); i++)
    out << "  " << *(self.plist[i]) << endl;
  out << "  end" <<endl;
  return out;
}

istream& operator >> (istream& in, vjChunkDesc& self) {
  char str[256];
  vjPropertyDesc *p;
  readString (in, str, 256);
  self.name = new char[strlen(str)+1];
  strcpy (self.name, str);
  do {
    p = new vjPropertyDesc();
    in >> *p;
    if (p->name && (!strcasecmp(p->name,"end")))
      break;
    self.add(p);
  } while (!in.eof());
  return in;
}




