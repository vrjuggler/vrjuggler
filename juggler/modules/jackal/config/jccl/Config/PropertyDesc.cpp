
#include <Config/vjChunkDesc.h>



vjPropertyDesc::vjPropertyDesc () : enumv() {
  name = NULL;
  token = NULL;
  num = 0;
  type = T_INVALID;
  help = NULL;
}



vjPropertyDesc::vjPropertyDesc (char *n, int i, VarType t, char* h):enumv() {
  name = new char[strlen(n)+1];
  strcpy (name, n);
  token = new char[strlen(n)+1];
  strcpy (token, n);
  num = i;
  type = t;
  help = new char[strlen(h)+1];
  strcpy (help, h);
}



vjPropertyDesc::~vjPropertyDesc () {
  for (int i = 0; i < enumv.size(); i++)
    delete enumv[i];
  if (name)
    delete name;
  if (token)
    delete token;
  if (help) {
    delete help;
  }
}


ostream& operator << (ostream& out, vjPropertyDesc& self) {
  out << self.name << " " << typeString(self.type) << " "
      << self.num << " " << self.token;
  
  /* print enumeration only if we have values. */
  if (self.enumv.size() > 0) {
    vjEnumEntry *e;
    out << " { ";
    for (int i = 0; i < self.enumv.size(); i++) {
      e = self.enumv[i];
      out << '"' << e->getName();
      if ((self.type != T_STRING) && (self.type != T_CHUNK))
	out << "=" << e->getVal();
      out << "\" ";
    }
    out << "}";
  }
  
  /* print help string - always quoted. */
  out << " \"" << self.help << '"';
  return out;
}



istream& operator >> (istream& in, vjPropertyDesc& self) {

  char str[512];
  int size;
  
  /* format of line is: name type size { enums/chunktypes } token. */
  
  readString (in, str, 512);
  if (self.name)
    delete self.name;
  size = strlen(str)+1;
  if (!(self.name = new char[size]))
    cout << "Unable to allocate ram" << endl;
  strcpy (self.name, str);
  if (!strcasecmp (self.name, "end"))
    return in;
  
  self.type = readType(in);
  in >> self.num;
  readString (in,str,512);
  if (self.token)
    delete self.token;
  size = strlen(str)+1;
  self.token = new char[size];
  strcpy (self.token, str);
  /* crude initial parsing of enumerations.
   */
  readString (in, str, 512);
  if (!strcasecmp (str, "{")) {
    if (self.type == T_BOOL) {
      cout << "ERROR: " << self.name << ": Enumerations not supported for "
	"boolean types.\n";
      do {
	readString (in, str, 512);
      } while (!strcasecmp (str, "}") && !in.eof());
    } 
    else {
      int j, i = 0;
      vjEnumEntry *e;
      readString (in, str, 512);
      while (strcasecmp (str, "}") && !in.eof()) {
	if (self.type == T_INT) 
	  for (j = 0; j < strlen(str); j++) {
	    if (str[j] == '=') {
	      i = atoi (str+j+1);
	      str[j] = '\0';
	      break;
	    }
	  }
	e = new vjEnumEntry (str, i++);
	self.enumv.push_back (e);
	readString (in, str, 512);
      }
    }
    readString (in, str, 512);
  }
  
  self.help = new char [strlen(str) +1];
  strcpy (self.help, str);
  
  return in;
}

