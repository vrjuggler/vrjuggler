#include <config.h>
#include <iostream.h>
#include <fstream.h>
#include <sys/types.h>

#include <Kernel/vjDebug.h>
#include <Config/vjChunkDescDB.h>



vjChunkDescDB::vjChunkDescDB (): descs() {
  ;
}



vjChunkDescDB::~vjChunkDescDB() {
  removeAll();
}


vjChunkDesc* vjChunkDescDB::getChunkDesc (char *name) {
  for (int i = 0; i < descs.size(); i++)
    if (!strcasecmp (descs[i]->token, name))
      return descs[i];
  return NULL;
}



bool vjChunkDescDB::insert (vjChunkDesc *d) {
  for (int i = 0; i < descs.size(); i++) 
    if (!strcasecmp (descs[i]->token, d->token)) {
      delete (descs[i]);
      descs[i] = d;
      return true;
    }
  descs.push_back(d);
  return true;
}



bool vjChunkDescDB::remove (char *tok) {
  
  vector<vjChunkDesc*>::iterator begin = descs.begin();
  while (begin != descs.end()) {
    if (!strcasecmp ((*begin)->token, tok)) {
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
  vector<vjChunkDesc*>::iterator i = descs.begin();
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
  char str[256];
  
  for (;;) {
    vjChunkDesc *ch;
    if (readString (in, str, 256) == 0)
      break; /* eof */
    else if (!strcasecmp (str, "chunk")) {
      ch = new vjChunkDesc();
      in >> *ch;
      self.insert(ch);
    } 
    else if (!strcasecmp (str, "end"))
      break;
    else {
      vjDEBUG(1) << "Unexpected symbol parsing vjChunkDescDB: '" 
		           << str <<"'"<< endl << vjDEBUG_FLUSH;
    }
  }
  vjDEBUG(4) << "vjChunkDescDB::>> : Finished - " << self.descs.size()
       << " descriptions read." << endl << vjDEBUG_FLUSH;
  return in;
}



bool vjChunkDescDB::load (char *fname) {
  ifstream in(fname);

  if (!in) {
    vjDEBUG(1) << "vjChunkDescDB::load(): Unable to open file '"
	       << fname << "'" << endl << vjDEBUG_FLUSH;
    return false;
  }
  in >> *this;
  return true;
}



bool vjChunkDescDB::save (char *fname) {
  ofstream out(fname);
  if (!out) {
    vjDEBUG(1) << "vjChunkDescDB::save(): Unable to open file '"
	       << fname << "'" << endl << vjDEBUG_FLUSH;
    return false;
  }
  out << *this;
  return true;
}
