/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
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



vjPropertyDesc::vjPropertyDesc (vjPropertyDesc& d) {
    *this = d;
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
    unsigned int i;
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
    for (unsigned int i = 0; i < enumv.size(); i++) {
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
   for (unsigned int i = 0; i < self.valuelabels.size(); i++) {
       e = self.valuelabels[i];
       out << '"' << e->getName().c_str() << "\" ";
   }
   out << "}";
    }

    /* print enumeration only if we have values. */
    if (self.enumv.size() > 0) {
   out << " vj_enumeration { ";
   for (unsigned int i = 0; i < self.enumv.size(); i++)
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
       e = new vjEnumEntry (str, T_STRING);
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
       vjVarValue *v;
       // this is slightly kludgey.  We make a varvalue to store the enumeration
       // value... except for T_CHUNK and T_EMBEDDEDCHUNK where we store a chunk
       // name type...
       if ((self.type == T_CHUNK) || (self.type == T_EMBEDDEDCHUNK))
      v = new vjVarValue (T_STRING);
       else
      v = new vjVarValue (self.type);

       char* c = strstr (str, "=");
       if (c) {
      *c = '\0';
      *v = (c+1);
       }
       else {
      if (self.type == T_STRING || self.type == T_CHUNK || 
          self.type == T_EMBEDDEDCHUNK)
          *v = str;
      else
          *v = i++;
       }
       self.enumv.push_back (new vjEnumEntry (str, *v));
       delete v;
       readString (in, str, size);
   }
   readString (in, str, size);
    }
    self.help = str;

    return in;
}


vjPropertyDesc& vjPropertyDesc::operator= (vjPropertyDesc& pd) {
    unsigned int i;
    if (&pd == this)
   return *this;
    name = pd.name;
    token = pd.token;
    help = pd.help;
    type = pd.type;
    num = pd.num;

    for (i = 0; i < valuelabels.size(); i++)
   delete valuelabels[i];
    for (i = 0; i < enumv.size(); i++)
   delete enumv[i];
    valuelabels.erase (valuelabels.begin(), valuelabels.end());
    enumv.erase (enumv.begin(), enumv.end());
    for (i = 0; i < pd.valuelabels.size(); i++)
   valuelabels.push_back (new vjEnumEntry(*(pd.valuelabels[i])));
    for (i = 0; i < pd.enumv.size(); i++)
   enumv.push_back (new vjEnumEntry(*(pd.enumv[i])));
    return *this;
}





