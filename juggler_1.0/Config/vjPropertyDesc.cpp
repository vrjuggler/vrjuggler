
#include <Config/vjChunkDesc.h>
#include <Kernel/vjDebug.h>



vjPropertyDesc::vjPropertyDesc () : enumv(), valuelabels() {
  name = NULL;
  token = NULL;
  num = 0;
  type = T_INVALID;
  help = NULL;
}



vjPropertyDesc::vjPropertyDesc (char *n, int i, VarType t, char* h):enumv(), valuelabels () {
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
  int i;
  for (i = 0; i < enumv.size(); i++)
    delete enumv[i];
  for (i = 0; i < valuelabels.size(); i++)
    delete valuelabels[i];
  if (name)
    delete name;
  if (token)
    delete token;
  if (help) {
    delete help;
  }
}



vjEnumEntry* vjPropertyDesc::getEnumEntryAt (int index) {
    if (enumv.size() > index)
	return enumv[index];
    else
	return NULL;
}



ostream& operator << (ostream& out, vjPropertyDesc& self) {
    out << self.token << " " << typeString(self.type) << " "
	<< self.num << " \"" << self.name << "\"";

    if (self.valuelabels.size() > 0) {
	vjEnumEntry *e;
	out << " vj_valuelabels { ";
	for (int i = 0; i < self.valuelabels.size(); i++) {
	    e = self.valuelabels[i];
	    out << '"' << e->getName() << "\" ";
	}
	out << "}";
    }

    /* print enumeration only if we have values. */
    if (self.enumv.size() > 0) {
	vjEnumEntry *e;
	out << " vj_enumeration { ";
	for (int i = 0; i < self.enumv.size(); i++) {
	    e = self.enumv[i];
	    out << '"' << e->getName();
	    if ((self.type != T_STRING) && (self.type != T_CHUNK) &&
		(self.type != T_EMBEDDEDCHUNK))
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

    const int size = 512;
    char str[size];

    /* format of line is: name type size { enums/chunktypes } token. */

    readString (in, str, size);
    //cout << "read propertydesc token " << str << endl;
    if (self.token)
	delete self.token;
    self.token = strdup (str);
    if (!strcasecmp (self.token, "end"))
	return in;

    self.type = readType(in);
    in >> self.num;
    readString (in,str,size);

    if (self.name)
	delete self.name;
    self.name = strdup (str);

    readString (in, str, size);
    
    /* parsing value labels, if there are any */
    if (!strcasecmp (str, "vj_valuelabels")) {
	//cout << "reading valuelabels" << endl;
	readString (in,str,size);
	if (strcasecmp (str, "{"))
	    vjDEBUG(1) << "ERROR: expected '{'" << endl << vjDEBUG_FLUSH;
	
	vjEnumEntry *e;
	readString (in, str, size);
	while (strcasecmp (str, "}") && !in.eof()) {
	    e = new vjEnumEntry (str, 0);
	    self.valuelabels.push_back (e);
	    readString (in, str, size);
	}
	readString (in, str, size);
    }
    
    /* parsing enumerations, if there are any */
    if (!strcasecmp (str, "vj_enumeration"))
	readString (in, str, size);
    if (!strcasecmp (str, "{")) {
	//cout << "parsing enumerations" << endl;
	if (self.type == T_BOOL) {
	    vjDEBUG(1) << "ERROR: " << self.name << ": Enumerations not supported for "
		"boolean types.\n" << vjDEBUG_FLUSH;
	    do {
		readString (in, str, size);
	    } while (!strcasecmp (str, "}") && !in.eof());
	}
	else {
	    int j, i = 0;
	    vjEnumEntry *e;
	    readString (in, str, size);
	    while (strcasecmp (str, "}") && !in.eof()) {
		//cout << "reading enumentry: " << str << endl;
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
		readString (in, str, size);
	    }
	}
	readString (in, str, size);
    }

    self.help = strdup (str);

    return in;
}

