
// vjConfigChunkDB.C
// Implementation of vjConfigChunk DB
//
// Author: Christopher Just



#include <config.h>
#include <fstream.h>
#include <sys/types.h>

#include <Kernel/vjDebug.h>
#include <Config/vjConfigChunkDB.h>



vjConfigChunkDB::vjConfigChunkDB (vjChunkDescDB *d): chunks() {
  descs = d;
}



vjConfigChunkDB::~vjConfigChunkDB () {
  erase();
}



void vjConfigChunkDB::setChunkDescDB (vjChunkDescDB *d) {
  descs = d;
}



vjConfigChunk* vjConfigChunkDB::getChunk (char *name) {
  /* returns a chunk with the given name, if such exists, or NULL.
   */
  char *c;
  for (int i = 0; i < chunks.size(); i++) {
    c = chunks[i]->getProperty ("name");
    if (c && !strcasecmp (c, name))
      return chunks[i];
  }
  return NULL;
}



// GetMatching: These functions return a vector of all chunks with a
// property named by the first argument, and a value defined by the
// second argument.  The returned vector may be empty.
// NOTE:  The caller is responsible for delete()ing the vector, but not
// its contents.
vector<vjConfigChunk*>* vjConfigChunkDB::getMatching (char *property, char *value) {
  char *c;
  vector<vjConfigChunk*>* v = new vector<vjConfigChunk*>;

  for (int i = 0; i < chunks.size(); i++) {
    c = chunks[i]->getProperty(property);
    if (!strcasecmp (c, value))
      v->push_back(chunks[i]);
  } 
  return v;
}

vector<vjConfigChunk*>* vjConfigChunkDB::getMatching (char *property, int value) {
  int c;
  vector<vjConfigChunk*>* v = new vector<vjConfigChunk*>;
  for (int i = 0; i < chunks.size(); i++) {
    c = chunks[i]->getProperty(property);
    if (c == value)
      v->push_back(chunks[i]);
  }
  return v;
}


vector<vjConfigChunk*>* vjConfigChunkDB::getMatching (char *property, float value) {
  float c;
  vector<vjConfigChunk*>* v = new vector<vjConfigChunk*>;
  for (int i = 0; i < chunks.size(); i++) {
    c = chunks[i]->getProperty(property);
    if (c == value)
      v->push_back(chunks[i]);
  }
  return v;
}



bool vjConfigChunkDB::erase () {
  /* removes all chunks from self (and frees them)
   */
  for (int i = 0; i < chunks.size(); i++)
    delete (chunks[i]);
  chunks.erase (chunks.begin(), chunks.end() );
  return true;
}



// Removes (and frees all memory associated with) all chunks with a property
// named by the first argument with a value defined by the second argument.
int vjConfigChunkDB::remove (char *property, int value) {
  int i = 0;
  int c;
  vector<vjConfigChunk*>::iterator begin = chunks.begin();
  while (begin != chunks.end()) {
    c = (*begin)->getProperty(property);
    if (c == value) {
      delete (*begin);
      chunks.erase(begin);
    }
    else
      begin++;
  }
  return i;
}

int vjConfigChunkDB::remove (char *property, float value) {
  int i = 0;
  float c;

  vector<vjConfigChunk*>::iterator begin = chunks.begin();
  while (begin != chunks.end()) {
    c = (*begin)->getProperty(property);
    if (c == value) {
      delete (*begin);
      chunks.erase(begin);
    }
    else
      begin++;
  }
  return i;
}

int vjConfigChunkDB::remove (char *property, char *value) {
  if (value == NULL)
    return 0;
  int i = 0;
  char* c;
  cout << "vjConfigChunkDB remove (" << property << ", " << value << ")" << endl;
  vector<vjConfigChunk*>::iterator begin = chunks.begin();
  while (begin != chunks.end()) {
    c = (*begin)->getProperty(property);
    if (c) {
      if (!strcasecmp (value, c)) {
	delete (*begin);
	chunks.erase(begin);
      }
      else
	begin++;
      delete c;
    }
    else
      begin++;
  }
  return i;
}



/* IO functions: */

ostream& operator << (ostream& out, vjConfigChunkDB& self) {
  for (int i = 0; i < self.chunks.size(); i++) {
    out << *(self.chunks[i]) << endl;
  }
  out << "End" << endl;
  return out;
}



istream& operator >> (istream& in, vjConfigChunkDB& self) {
  char str[512];
  vjChunkDesc *cd;
  vjConfigChunk *ch;

  if (self.descs == NULL) {
    cerr << "ERROR: vjChunkDescDB::>> - No vjChunkDescDB." << endl;
    return in;
  }

  do {
    if (0 == readString (in, str, 512))
      break; /* eof */
    if (!strcasecmp (str, "end"))
      break;
    //cout << "reading chunk " << str << endl;
    cd = self.descs->getChunkDesc (str);
    if (cd == NULL) {
      cerr << "ERROR!: Unknown Chunk type: " << str << endl;
      // skip to end of chunk
      while (strcasecmp (str, "end")) {
	if (0 == readString (in, str, 512))
	  break;
	//cerr << "read " << str << endl;
      }
    } else {
      ch = new vjConfigChunk(cd);
      in >> *ch;
      /* OK.  If this chunk has the same instancename as a chunk
       * already in self, we want to remove the old one
       */
      if (ch->getNum ("name"))
	self.remove ("name", (char*)ch->getProperty ("name"));
      self.chunks.push_back(ch);
    }
  } while (!in.eof());    
  
  cerr << "vjConfigChunkDB::>> : Finished - " << self.chunks.size() << " chunks read." << endl; 

  return in;
}



bool vjConfigChunkDB::load (const char *fname) {
  ifstream in(fname);

  vjDEBUG(4) << "vjConfigChunkDB::load(): opening file " << fname << endl << vjDEBUG_FLUSH;

  if (descs == NULL) {
    vjDEBUG(1) << "ERROR: vjConfigChunkDB::load - no vjChunkDescDB" << endl << vjDEBUG_FLUSH;
    return false;
  }
  if (!in) {
    vjDEBUG(1) << "vjConfigChunkDB::load(): Unable to open file '" 
	       << fname << "'" << endl << vjDEBUG_FLUSH;
    return false;
  }
  in >> *this;
  return true;
}

bool vjConfigChunkDB::save (const char *fname) {
  ofstream out(fname);
  if (!out) {
    vjDEBUG(1) << "ERROR: vjConfigChunkDB::save() - Unable to open file '"
	       << fname << "'" << endl << vjDEBUG_FLUSH;
    return false;
  }
  out << *this;
  return true;
}


