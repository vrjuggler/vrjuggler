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

#include <vrj/vrjConfig.h>
#include <vrj/Config/ConfigChunk.h>
#include <vrj/Config/ChunkFactory.h>
#include <vrj/Config/PropertyDesc.h>
#include <vrj/Config/EnumEntry.h>
#include <vrj/Config/ParseUtil.h>
#include <vrj/Config/ConfigTokens.h>
#include <vrj/Util/Debug.h>
#include <vrj/Config/Property.h>

namespace vrj
{

Property::Property (PropertyDesc *pd): value() {
    //cout << "vjProperty(): desc is '" << flush << *pd << "'" << endl;
    int j;
    VarValue *v;

    validation = 1;

    pd->assertValid();
    description = pd;

    num = pd->getNumAllowed();
    type = pd->getType();
    embeddesc = NULL;

    if (type == T_EMBEDDEDCHUNK) {
        EnumEntry *e = description->getEnumEntryAtIndex (0);
        if (e)
            embeddesc = ChunkFactory::instance()->getChunkDesc (e->getName());
    }

    /* the idea here is that if num == -1 we can add values to
     * a property, e.g. add another active wall.
     * otherwise we can just set the extant values.
     */
    if (num != -1) {
        /* we're filling the vector with num copies of a
         * default VarValue */
        for (j = 0; j < num; j++ ) {
            v = createVarValue (j);
            value.push_back(v);
        }
    }
}



VarValue *Property::createVarValue (int i) {
    assertValid();

    // if i == -1, we're just tacking onto the end
    if (i == -1)
        i = value.size();
    if (type == T_EMBEDDEDCHUNK) {
        ConfigChunk *ch = ChunkFactory::instance()->createChunk (embeddesc);
        if (description->getValueLabelsSize() > i)
            ch->setProperty ("Name", description->getValueLabel(i));
        else {
            ch->setProperty ("Name", description->getName());
        }
        return new VarValue (ch);
    }
    else
        return new VarValue (type);
}


Property::~Property () {
    /* XXX
    unsigned int i;

    for (i = 0; i < value.size(); i++)
        delete (value)[i];
        */
    validation = 0;
}



Property::Property (const Property& p):value() {
    description = NULL;
    validation = 1;
    *this = p;
}



#ifdef VJ_DEBUG
void Property::assertValid () const {
    assert (validation == 1 && "Trying to use deleted Property");

    for (unsigned int i = 0; i < value.size(); i++)
        value[i]->assertValid();

    if (description)
        description->assertValid();
}
#endif



Property& Property::operator= (const Property& p) {
    assertValid();
    p.assertValid();

    unsigned int i;

    if (&p == this)
        return *this;

    description = p.description;
    type = p.type;
    units = p.units;
    embeddesc = p.embeddesc;
    num = p.num;

    /*  XXX
    for (i = 0; i < value.size(); i++)
        delete (value[i]);
        */
    value.clear();

    for (i = 0; i < p.value.size(); i++) {
        value.push_back (new VarValue(*(p.value[i])));
    }
    return *this;
}



bool Property::operator== (const Property& p) const {
    assertValid();
    p.assertValid();

    if (description != p.description)
        return false;
    if (value.size() != p.value.size())
        return false;
    for (unsigned int i = 0; i < value.size(); i++)
        if (*(value[i]) != *(p.value[i]))
            return false;
    return true;
}



bool Property::applyUnits (CfgUnit u) {
    assertValid();

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




EnumEntry* Property::getEnumEntryWithValue (int val) const {
    assertValid();

    // gets an enumentry based on the value, instead of the name
    VarValue v(T_INT);
    v = val;
    return description->getEnumEntryWithValue (v);
}



std::ostream& operator << (std::ostream &out, Property& p) {
    p.assertValid();

    out << p.getToken().c_str() << " { ";
    for (unsigned int i = 0; i < p.value.size(); i++) {
        VarValue *v = ((p.value))[i];

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
            EnumEntry *e = p.getEnumEntryWithValue((int)(*v));
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



const VarValue& Property::getValue (unsigned int ind) const {
    assertValid();

    if (ind >= value.size()) {
        return VarValue::getInvalidInstance();
    }
    return *((value)[ind]);
}



int Property::getNum () const {
    assertValid();

    return value.size();
}



const std::string& Property::getName () const {
    assertValid();

    return description->getName();
}


const std::string& Property::getToken () const {
    assertValid();

    return description->getToken();
}


bool Property::preSet (unsigned int ind) {
    assertValid();

    unsigned int i;
    VarValue *v;

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



bool Property::setValue (int val, int ind ) {
    assertValid();

    if (!preSet(ind))
        return false;
    *((value)[ind]) = val;
    return true;
}



bool Property::setValue (float val, int ind ) {
    assertValid();

    if (!preSet(ind))
        return false;
    *((value)[ind]) = val;
    return true;
}



bool Property::setValue (const std::string& val, int ind) {
    assertValid();

    if (!preSet(ind))
        return false;
    *((value)[ind]) = val;
    return true;
}



bool Property::setValue (ConfigChunk* val, int ind) {
    assertValid();

    if (!preSet(ind)) {
        vprDEBUG(vprDBG_ERROR, 1) << "vjProperty::Preset failed!\n" << vprDEBUG_FLUSH;
        return false;
    }
    *(value[ind]) = val;
    return true;
}



bool Property::setValue (const VarValue& val, int ind) {
    assertValid();

    if (!preSet (ind))
        return false;
    *(value[ind]) = val;
    return true;
}



bool Property::tryAssign (int index, const char* val) {
    assertValid();

    /* This does some type-checking and translating before just
     * doing an assign into the right value entry of p. Some of
     * this functionality ought to just be subsumed by VarValue
     * itself, but this way we get back some feedback about
     * wether a type mismatch occurred (ie we return false if
     * a type mismatch occurs ).
     *
     * Incidentally, this is also where string values get
     * mangled into enumeration entries when assigning strings
     * to T_INTs.
     */
    char* endval;
    int i;
    float f;
    bool b;

    if (type != T_CHUNK) {          // T_CHUNKS have enumeration, but they are really strings (or something)
        EnumEntry* e = getEnumEntry (val);
        if (e) {
            setValue (e->getValue());
            return true;
        }
    }

    switch (type) {
    case T_INT:
        i = strtol (val, &endval, 0);
        if (*endval != '\0')
            vprDEBUG (vrjDBG_CONFIG, 0) << clrOutNORM(clrYELLOW, "WARNING:") << " Parser expected int, got '"
                  << val << "'\n" << vprDEBUG_FLUSH;
        setValue (i, index);
        return true;
    case T_FLOAT:
        f = (float)strtod (val, &endval);
        if (*endval != '\0')
            vprDEBUG (vrjDBG_CONFIG, 0) << clrOutNORM(clrYELLOW, "WARNING:") << " Parser expected float, got '"
                                      << val << "'\n" << vprDEBUG_FLUSH;
        setValue (f, index);
        return true;
    case T_BOOL:
        b = false;
        if (!strcasecmp (val, true_TOKEN))
            b = true;
        else if (!strcasecmp (val, false_TOKEN))
            b = false;
        else { // we'll try to accept a numeric value
            b = strtol (val, &endval, 0);
            if (endval != '\0') {
                b = false;
                vprDEBUG (vrjDBG_CONFIG,0) << clrOutNORM(clrYELLOW, "WARNING:") << " Parser expected bool, got '"
                                         << val << "'\n" << vprDEBUG_FLUSH;
            }
        }
        setValue (b, index);
        return true;
    case T_STRING:
    case T_CHUNK:
        setValue (val, index);
        return true;
    case T_EMBEDDEDCHUNK:
        std::cout << "NOT HANDLED HERE!" << std::endl;
        return false;
    default:
        return false;
    }
}

};
