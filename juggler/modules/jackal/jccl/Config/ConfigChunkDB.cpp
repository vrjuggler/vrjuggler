
// vjConfigChunkDB.cpp
// Implementation of vjConfigChunk DB
//
// Author: Christopher Just



#include <vjConfig.h>
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



//: copy constructor
vjConfigChunkDB::vjConfigChunkDB (vjConfigChunkDB& db): chunks() {
    *this = db;
}



vjConfigChunkDB& vjConfigChunkDB::operator = (vjConfigChunkDB& db) {
    int i;
    descs = db.descs;
    for (i = 0; i < chunks.size(); i++)
	delete chunks[i];
    chunks.erase (chunks.begin(), chunks.end());
    for (i = 0; i < db.chunks.size(); i++) {
        chunks.push_back (new vjConfigChunk(*(db.chunks[i])));
    }
    return *this;
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

// Return a copy of the chunks vector
vector<vjConfigChunk*> vjConfigChunkDB::getChunks()
{
   return chunks;
}

// Add the given chunks to the end of the chunk list
void vjConfigChunkDB::addChunks(vector<vjConfigChunk*> new_chunks)
{
   chunks.insert(chunks.end(), new_chunks.begin(), new_chunks.end());
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
int vjConfigChunkDB::removeMatching (char *property, int value) {
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

int vjConfigChunkDB::removeMatching (char *property, float value) {
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

int vjConfigChunkDB::removeMatching (char *property, char *value) {
  if (value == NULL)
    return 0;
  int i = 0;
  char* c;
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

//: Sorts the chunks based on dependancies
//! PRE: we need a "good object"
//! MODIFIES: self.  We move the objects around so they are sorted
//! POST: Topologically sorted
// Copy the chunks over to a new list.  Repetatively try to
// find an item in the source list that already has it's dependencies
// copied into the dst list.  Do this iteratively until done or
// until fail.
int vjConfigChunkDB::dependencySort()
{
   // Print out dependancies
   /*
   for(int i=0;i<chunks.size();i++)
   {
      cout << "Chunk:" << chunks[i]->getProperty("name") << endl;
      cout << "\tDepends on:";
      vector<string> deps = chunks[i]->getDependencies();
      if(deps.size() > 0)
      {
         for(int j=0;j<deps.size();j++)
            cout << deps[j] << ", ";
      } else {
         cout << "Nothing.";
      }
      cout << endl;
   }
   */

   // Create new src list to work from
   // Targetting the local data
   vector<vjConfigChunk*> src_chunks = chunks;
   chunks = vector<vjConfigChunk*>(0);             // Chunks is the local data - Zero it out to start

   bool dep_pass(true);             // Pass dependency check
   vector<string> deps;             // Source dependencies
   vector<vjConfigChunk*>::iterator cur_item
               = src_chunks.begin();          // The current src item to look at

   while(cur_item != src_chunks.end())          // While not at end of src list
   {
      deps = (*cur_item)->getDependencies();             // Get src dependencies
      for(int dep_num=0;dep_num<deps.size();dep_num++)   // May have multiple dependencies
         if (getChunk((char*)deps[dep_num].c_str()) == NULL)    // Check dependency
            dep_pass = false;                            // Failed check

      if(dep_pass)        // If src dependencies are accounted for
      {
         chunks.push_back(*cur_item);        // Copy src to dst
         src_chunks.erase(cur_item);         // Erase it from source
         cur_item = src_chunks.begin();      // Goto first item
      }
      else
         cur_item++;             // goto next item

      dep_pass = true;           // Reset to passing
   }

   if(src_chunks.size() > 0)     // Failed to get all dependencies
   {
      // ouput error
      for(int i=0;i<src_chunks.size();i++)
      {
          vjDEBUG(0) << "ERROR: Dependency error:  Chunk:" << chunks[i]->getProperty("name")
                     << "\tDepends on: \n" << vjDEBUG_FLUSH;
         vector<string> deps = chunks[i]->getDependencies();
         if(deps.size() > 0)
         {
            for(int j=0;j<deps.size();j++)
               vjDEBUG(0) << "\tdep " << j << ": " << deps[j] << endl << vjDEBUG_FLUSH;
         } else {
            vjDEBUG(0) << "Nothing.\n" << vjDEBUG_FLUSH;
         }
         vjDEBUG(0) << "Check for undefined devices that others depend upon.\n" << vjDEBUG_FLUSH;
      }
      chunks.insert(chunks.end(), src_chunks.begin(), src_chunks.end());   // Copy over the rest

      return -1;
   }
   else
   {
      // Print out sorted dependancies
      /*
      cout << "---- After sort ----" << endl;
      for(int i=0;i<chunks.size();i++)
      {
         cout << "Chunk:" << chunks[i]->getProperty("name") << endl;
         cout << "\tDepends on:";
         vector<string> deps = chunks[i]->getDependencies();
         if(deps.size() > 0)
         {
            for(int j=0;j<deps.size();j++)
               cout << deps[j] << ", ";
         } else {
            cout << "Nothing.";
         }
         cout << endl;
      }
      */

      return 0;      // Success
   }
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
    vjDEBUG(1) << "ERROR: vjChunkDescDB::>> - No vjChunkDescDB." << endl
	       << vjDEBUG_FLUSH;
    return in;
  }

  do {
    if (0 == readString (in, str, 512))
      break; /* eof */
    if (!strcasecmp (str, "end"))
      break;
    cd = self.descs->getChunkDesc (str);
    if (cd == NULL) {
      vjDEBUG(1) << "ERROR!: Unknown Chunk type: " << str << endl
		 << vjDEBUG_FLUSH;
      // skip to end of chunk
      while (strcasecmp (str, "end")) {
	if (0 == readString (in, str, 512))
	  break;
	//cerr << "read " << str << endl;
      }
    } else {
      ch = new vjConfigChunk(cd, self.descs);
      in >> *ch;
      /* OK.  If this chunk has the same instancename as a chunk
       * already in self, we want to remove the old one
       */
      if (ch->getNum ("name"))
	self.removeMatching ("name", (char*)ch->getProperty ("name"));
      self.chunks.push_back(ch);
    }
  } while (!in.eof());

  vjDEBUG(3) << "vjConfigChunkDB::>> : Finished - " << self.chunks.size() << " chunks read." << endl << vjDEBUG_FLUSH;

  return in;
}



bool vjConfigChunkDB::load (const char *fname) {
  ifstream in(fname);

  vjDEBUG(4) << "vjConfigChunkDB::load(): opening file " << fname << " -- " << vjDEBUG_FLUSH;

  if (descs == NULL) {
    vjDEBUG(1) << "\nERROR: vjConfigChunkDB::load - no vjChunkDescDB" << endl << vjDEBUG_FLUSH;
    return false;
  }
  if (!in) {
    vjDEBUG(1) << "\nvjConfigChunkDB::load(): Unable to open file '"
	       << fname << "'" << endl << vjDEBUG_FLUSH;
    return false;
  }
  vjDEBUG(3) << " succeeded." << endl << vjDEBUG_FLUSH;
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



vjChunkDescDB* vjConfigChunkDB::getChunkDescDB() {
  return descs;
}



bool vjConfigChunkDB::isEmpty() {
  return (chunks.size() == 0);
}



void vjConfigChunkDB::removeAll() {
  // just an alias
  erase();
}



int vjConfigChunkDB::removeNamed (char *name) {
  return removeMatching ("name", name);
}
