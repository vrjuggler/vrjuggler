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


#include <vjConfig.h>
#include <Config/vjProperty.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkFactory.h>
#include <Config/vjPropertyDesc.h>
#include <Config/vjEnumEntry.h>
#include <Config/vjParseUtil.h>
#include <Kernel/vjDebug.h>

vjProperty::vjProperty (vjPropertyDesc *pd) {
    //cout << "vjProperty(): desc is '" << flush << *pd << "'" << endl;
    int j;
    vjVarValue *v;


    description = pd;
    num = pd->getNumAllowed();
    type = pd->getType();
    embeddesc = NULL;

    if (type == T_EMBEDDEDCHUNK) {
   vjEnumEntry *e = description->getEnumEntryAtIndex (0);
   if (e) 
       embeddesc = vjChunkFactory::instance()->getChunkDesc (e->getName());
    }

    /* the idea here is that if num == -1 we can add values to 
     * a property, e.g. add another active wall.
     * otherwise we can just set the extant values.
     */
    if (num != -1) {
   /* we're filling the vector with num copies of a 
    * default vjVarValue */
   for (j = 0; j < num; j++ ) {
       v = createVarValue (j);
       value.push_back(v);
   }
    }
}



vjVarValue *vjProperty::createVarValue (int i) {
    // if i == -1, we're just tacking onto the end
    if (i == -1)
   i = value.size();
    if (type == T_EMBEDDEDCHUNK) {
   vjConfigChunk *ch = vjChunkFactory::instance()->createChunk (embeddesc);
   if (description->getValueLabelsSize() > i)
       ch->setProperty ("Name", description->getValueLabel(i));
   else {
       ch->setProperty ("Name", description->getName());
   }
   return new vjVarValue (ch);
    }
    else
   return new vjVarValue (type);
}
 


vjProperty::~vjProperty () {
    unsigned int i;

    for (i = 0; i < value.size(); i++)
   delete (value)[i];
}



vjProperty::vjProperty (const vjProperty& p):value() {
    *this = p;
}



vjProperty& vjProperty::operator= (const vjProperty& p) {
    unsigned int i;

    if (&p == this)
   return *this;

    description = p.description;
    type = p.type;
    units = p.units;

    for (i = 0; i < value.size(); i++)
        delete (value[i]);
    value.erase (value.begin(), value.end());
    for (i = 0; i < p.value.size(); i++) {
        value.push_back (new vjVarValue(*(p.value[i])));
    }
    return *this;
}



bool vjProperty::operator== (const vjProperty& p) const {
    if (description != p.description)
   return false;
    if (value.size() != p.value.size())
   return false;
    for (unsigned int i = 0; i < value.size(); i++)
   if (*(value[i]) != *(p.value[i]))
       return false;
    return true;
}



bool vjProperty::applyUnits (CfgUnit u) {

    if (type == T_DISTANCE) {
   for (unsigned int j = 0; j < value.size(); j++)
       setValue( toFeet (getValue(j), u), j);
   return true;
    }
    else {
   //cerr << "Units may only be applied to Distance values." <<endl;
   return false;
    }
}




vjEnumEntry* vjProperty::getEnumEntryWithValue (int val) const {
    // gets an enumentry based on the value, instead of the name
    vjVarValue v(T_INT);
    v = val;
    return description->getEnumEntryWithValue (v);
}



ostream& operator << (ostream &out, vjProperty& p) {
    out << p.getToken().c_str() << " { ";
    for (unsigned int i = 0; i < p.value.size(); i++) {
        vjVarValue *v = ((p.value))[i];
        
        if ((p.type == T_STRING) || (p.type == T_CHUNK)) {
            out << '"' << *v << '"';
        }
        else if (p.type == T_EMBEDDEDCHUNK) {
            out << "\n" << *v;
        }
        else if ((p.type == T_FLOAT) || (p.type == T_BOOL)) {
            out << *v;
        }
        else {
            vjEnumEntry *e = p.getEnumEntryWithValue((int)(*v));
            if (e)
                out << '"' << e->getName().c_str() << '"';
            else
                out << *v;
        }
        out << " ";
    }
    if (p.type == T_DISTANCE)
        out << " " << unitString (p.units);
    out << " } ";
    return out;
}



vjVarValue& vjProperty::getValue (unsigned int ind) {
    if (ind >= value.size()) {
        return vjVarValue::getInvalidInstance();
    }
    return *((value)[ind]);
}



int vjProperty::getNum () const {
    return value.size();
}



const std::string& vjProperty::getName () const {
    return description->getName();
}


const std::string& vjProperty::getToken () const {
    return description->getToken();
}


bool vjProperty::preSet (unsigned int ind) {
    unsigned int i;
    vjVarValue *v;

    if (ind >= value.size()) {
        if (num == -1) {
            for (i = value.size(); i <= ind; i++) {
                v = createVarValue();
                value.push_back(v);
            }
            return true;
        }
        else
            return false;
    }
    return true;
}



bool vjProperty::setValue (int val, int ind ) {
    if (!preSet(ind))
        return false;
    *((value)[ind]) = val;
    return true;
}



bool vjProperty::setValue (float val, int ind ) {
    if (!preSet(ind))
   return false;
    *((value)[ind]) = val;
    return true;
}



bool vjProperty::setValue (const std::string& val, int ind) {
    if (!preSet(ind))
   return false;
    *((value)[ind]) = val;
    return true;
}



bool vjProperty::setValue (vjConfigChunk* val, int ind) {
    if (!preSet(ind)) {
   vjDEBUG(vjDBG_ERROR, 1) << "vjProperty::Preset failed!\n" << vjDEBUG_FLUSH;
   return false;
    }
    *(value[ind]) = val;
    return true;
}


bool vjProperty::setValue (vjVarValue& val, int ind) {
    if (!preSet (ind))
   return false;
    *(value[ind]) = val;
    return true;
}
