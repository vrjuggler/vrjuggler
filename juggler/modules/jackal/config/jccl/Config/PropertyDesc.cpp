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
#include <jccl/Util/Debug.h>
#include <vpr/Util/Assert.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ConfigIO.h>

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
    unsigned int i;
    for (i = 0; i < enumv.size(); i++)
        delete enumv[i];
    for (i = 0; i < valuelabels.size(); i++)
        delete valuelabels[i];
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
    ConfigIO::instance()->writePropertyDesc (out, self);
    return out;
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

    
    for (i = 0; i < valuelabels.size(); i++)
        delete valuelabels[i];
    for (i = 0; i < enumv.size(); i++)
        delete enumv[i];
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
