/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/



#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ChunkDesc.h>
#include <vpr/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <jccl/Config/ConfigTokens.h>

namespace jccl {
   
PropertyDesc::PropertyDesc () : valuelabels(), enumv() {
    validation = 1;
    name = "";
    token = "";
    num = 0;
    type = VJ_T_INVALID;
    help = "";
    enum_val = 0;
}


PropertyDesc::PropertyDesc (const PropertyDesc& d): valuelabels(), enumv() {
    validation = 1;
    *this = d;
}



PropertyDesc::PropertyDesc (const std::string& n, int i, VarType t,
            const std::string& h): valuelabels(), enumv() {
    validation = 1;
    name = n;
    token = n;
    help = h;
    num = i;
    type = t;
    enum_val = 0;
}



PropertyDesc::~PropertyDesc () {
    /* XXX
    unsigned int i;
    for (i = 0; i < enumv.size(); i++)
        delete enumv[i];
    for (i = 0; i < valuelabels.size(); i++)
        delete valuelabels[i];
    */
    validation = 0;
}



#ifdef JCCL_DEBUG
void PropertyDesc::assertValid () const {
    vprASSERT (validation == 1 && "Trying to use deleted PropertyDesc");
}
#endif



void PropertyDesc::appendValueLabel (const std::string& _label) {
    valuelabels.push_back (new EnumEntry (_label, T_STRING));
}

// used as a possible return value below.
static const std::string PropertyDesc_empty_string ("");

const std::string& PropertyDesc::getValueLabel (unsigned int i) const {
    assertValid();

    if (i < valuelabels.size())
        return valuelabels[i]->getName();
    else
        return PropertyDesc_empty_string;
}


void PropertyDesc::appendEnumeration (const std::string& _label, 
                                        const std::string& _value) {
    VarValue *v;
    // this is slightly kludgey.  We make a varvalue to store the enumeration
    // value... except for T_CHUNK and T_EMBEDDEDCHUNK where we store a chunk
    // name type...
    if ((type == T_CHUNK) || (type == T_EMBEDDEDCHUNK))
        v = new VarValue (T_STRING);
    else
        v = new VarValue (type);

    if (type == T_STRING || type == T_CHUNK ||
        type == T_EMBEDDEDCHUNK) {
        *v = (_value == "")?_label : _value;
    }
    else {
        if (_value == "")
            *v = enum_val;
        else 
            *v = _value;
        //*v = (_value == "")?enum_val:_value;
    }
    enum_val++;
    enumv.push_back (new EnumEntry (_label, *v));
}

EnumEntry* PropertyDesc::getEnumEntry (const std::string& s) const {
    assertValid();

    for (unsigned int i = 0; i < enumv.size(); i++) {
        if (!vjstrcasecmp (enumv[i]->getName(), s))
            return enumv[i];
    }
    return NULL;
}


EnumEntry* PropertyDesc::getEnumEntryAtIndex (unsigned int index) const {
    assertValid();

    if (enumv.size() > index)
        return enumv[index];
    else
        return NULL;
}


EnumEntry* PropertyDesc::getEnumEntryWithValue (const VarValue& val) const {
    assertValid();

    for (unsigned int i = 0; i < enumv.size(); i++) {
        if (enumv[i]->getValue() == val)
            return enumv[i];
    }
    return NULL;
}


std::ostream& operator << (std::ostream& out, const PropertyDesc& self) {
    self.assertValid();

    out << self.token.c_str() << " " << typeString(self.type) << " "
        << self.num << " \"" << self.name.c_str() << "\"";

    /* print valuelabels if we have 'em */
    if (self.valuelabels.size() > 0) {
        EnumEntry *e;
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



std::istream& operator >> (std::istream& in, PropertyDesc& self) {
    self.assertValid();


    const int size = 512;
    char str[size];

    const char equal_TOKEN[] = "=";

    /* format of line is: name type size { enums/chunktypes } token. */

    readString (in, str, size);
    //cout << "read propertydesc token " << str << endl;
    self.token = str;
    if (!strcasecmp (str, end_TOKEN))
        return in;

    self.type = readType(in);
    in >> self.num;
    readString (in,str,size);

    self.name = str;

    readString (in, str, size);

    /* parsing value labels, if there are any */
    if (!strcasecmp (str, vj_valuelabels_TOKEN)) {
        //cout << "reading valuelabels" << endl;
        readString (in,str,size);
        if (strcasecmp (str, lbrace_TOKEN))
            vprDEBUG(vprDBG_ERROR,1) << clrOutNORM(clrRED, "ERROR:") << " expected '{'" << std::endl
                                   << vprDEBUG_FLUSH;

        EnumEntry *e;
        readString (in, str, size);
        while (strcasecmp (str, rbrace_TOKEN) && !in.eof()) {
            e = new EnumEntry (str, T_STRING);
            self.valuelabels.push_back (e);
            readString (in, str, size);
        }
        readString (in, str, size);
    }

    /* parsing enumerations, if there are any */
    if (!strcasecmp (str, vj_enumeration_TOKEN))
        readString (in, str, size);
    if (!strcasecmp (str, lbrace_TOKEN)) {
        //cout << "parsing enumerations" << endl;
        int i = 0;
        readString (in, str, size);
        while (strcmp (str, rbrace_TOKEN) && !in.eof()) {
            VarValue *v;
            // this is slightly kludgey.  We make a varvalue to store the enumeration
            // value... except for T_CHUNK and T_EMBEDDEDCHUNK where we store a chunk
            // name type...
            if ((self.type == T_CHUNK) || (self.type == T_EMBEDDEDCHUNK))
                v = new VarValue (T_STRING);
            else
                v = new VarValue (self.type);

            char* c = strstr (str, equal_TOKEN);
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
            self.enumv.push_back (new EnumEntry (str, *v));
            // delete v; XXX
            readString (in, str, size);
        }
        readString (in, str, size);
    }
    self.help = str;

    return in;
}


PropertyDesc& PropertyDesc::operator= (const PropertyDesc& pd) {
    assertValid();

    unsigned int i;
    if (&pd == this)
        return *this;
    name = pd.name;
    token = pd.token;
    help = pd.help;
    type = pd.type;
    num = pd.num;

    /*
    for (i = 0; i < valuelabels.size(); i++)
        delete valuelabels[i];
    for (i = 0; i < enumv.size(); i++)
        delete enumv[i];
        */
    valuelabels.clear();
    enumv.clear();
    for (i = 0; i < pd.valuelabels.size(); i++)
        valuelabels.push_back (new EnumEntry(*(pd.valuelabels[i])));
    for (i = 0; i < pd.enumv.size(); i++)
        enumv.push_back (new EnumEntry(*(pd.enumv[i])));
    return *this;
}


//: Equality Operator
// BUG (IPTHACK) - doesn't check equality of enumerations and valuelabels
bool PropertyDesc::operator== (const PropertyDesc& pd) const {
    assertValid();

    if (vjstrcasecmp (name, pd.name))
        return false;
    if (vjstrcasecmp (token, pd.token))
        return false;
    if (type != pd.type)
        return false;
    if (num != pd.num)
        return false;
    return true;
}


};
