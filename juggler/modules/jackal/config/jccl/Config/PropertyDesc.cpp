/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */



#include <Config/vjParseUtil.h>
#include <Config/vjChunkDesc.h>
#include <Kernel/vjDebug.h>



vjPropertyDesc::vjPropertyDesc () : enumv(), valuelabels() {
    name = "";
    token = "";
    num = 0;
    type = T_INVALID;
    help = "";
}



vjPropertyDesc::vjPropertyDesc (const std::string& n, int i, VarType t, 
				const std::string& h):enumv(), valuelabels () {
    name = n;
    token = n;
    help = h;
    num = i;
    type = t;
}



vjPropertyDesc::~vjPropertyDesc () {
    int i;
    for (i = 0; i < enumv.size(); i++)
	delete enumv[i];
    for (i = 0; i < valuelabels.size(); i++)
	delete valuelabels[i];
}



std::string vjPropertyDesc::getValueLabel (int i) {
    if (i < valuelabels.size())
	return (std::string)"";
    else
	return valuelabels[i]->getName();
}



vjEnumEntry* vjPropertyDesc::getEnumEntry (const std::string& s) {
    for (int i = 0; i < enumv.size(); i++) {
	if (!vjstrcasecmp (enumv[i]->getName(), s))
	    return enumv[i];
    }
    return NULL;
}


vjEnumEntry* vjPropertyDesc::getEnumEntryAtIndex (int index) {
    if ((enumv.size() > index) && (index >= 0))
	return enumv[index];
    else
	return NULL;
}

vjEnumEntry* vjPropertyDesc::getEnumEntryWithValue (vjVarValue& val) {
    for (int i = 0; i < enumv.size(); i++) {
	if (enumv[i]->getValue() == val)
	    return enumv[i];
    }
    return NULL;
}


ostream& operator << (ostream& out, vjPropertyDesc& self) {
    out << self.token.c_str() << " " << typeString(self.type) << " "
	<< self.num << " \"" << self.name.c_str() << "\"";

    /* print valuelabels if we have 'em */
    if (self.valuelabels.size() > 0) {
	vjEnumEntry *e;
	out << " vj_valuelabels { ";
	for (int i = 0; i < self.valuelabels.size(); i++) {
	    e = self.valuelabels[i];
	    out << '"' << e->getName().c_str() << "\" ";
	}
	out << "}";
    }

    /* print enumeration only if we have values. */
    if (self.enumv.size() > 0) {
	out << " vj_enumeration { ";
	for (int i = 0; i < self.enumv.size(); i++)
	    out << *(self.enumv[i]) << ' ';
	out << "}";
    }

    /* print help string - always quoted. */
    out << " \"" << self.help.c_str() << '"';
    return out;
}



istream& operator >> (istream& in, vjPropertyDesc& self) {

    const int size = 512;
    char str[size];

    /* format of line is: name type size { enums/chunktypes } token. */

    readString (in, str, size);
    //cout << "read propertydesc token " << str << endl;
    self.token = str;
    if (!strcasecmp (str, "end"))
	return in;

    self.type = readType(in);
    in >> self.num;
    readString (in,str,size);

    self.name = str;

    readString (in, str, size);

    /* parsing value labels, if there are any */
    if (!strcasecmp (str, "vj_valuelabels")) {
	//cout << "reading valuelabels" << endl;
	readString (in,str,size);
	if (strcasecmp (str, "{"))
	    vjDEBUG(vjDBG_ERROR,1) << "ERROR: expected '{'" << endl << vjDEBUG_FLUSH;
	
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
	int j, i = 0;
	vjEnumEntry *e;
	readString (in, str, size);
	while (strcmp (str, "}") && !in.eof()) {
	    vjVarValue v(self.type);
	    char* c = strstr (str, "=");
	    if (c) {
		*c = '\0';
		v = (c+1);
	    }
	    else {
		if (self.type == T_STRING || self.type == T_CHUNK || 
		    self.type == T_EMBEDDEDCHUNK)
		    v = str;
		else
		    v = i++;
	    }
	    self.enumv.push_back (new vjEnumEntry (str, v));
	    readString (in, str, size);
	}
	readString (in, str, size);
    }

    self.help = str;

    return in;
}


vjPropertyDesc& vjPropertyDesc::operator= (vjPropertyDesc& pd) {
    //cout << "propdesc operator= is called!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    if (&pd == this)
    return *this;
    name = pd.name;
    token = pd.token;
    help = pd.help;
    type = pd.type;
    valuelabels.erase (valuelabels.begin(), valuelabels.end());
    enumv.erase (enumv.begin(), enumv.end());
    return *this;
}





