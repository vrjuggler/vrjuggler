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


#include <jccl/jcclConfig.h>
#include <jccl/Config/Property.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ChunkFactory.h>
#include <jccl/Config/PropertyDesc.h>
#include <jccl/Config/EnumEntry.h>
#include <jccl/Config/ParseUtil.h>
#include <jccl/Config/ConfigTokens.h>
#include <jccl/Config/ConfigIO.h>
#include <jccl/Util/Debug.h>
#include <vpr/Util/Assert.h>

namespace jccl {
   
Property::Property (PropertyDesc *pd): mValues() {
    //cout << "Property(): desc is '" << flush << *pd << "'" << endl;
    int j;
    VarValue *v;

    mValidation = 1;

    pd->assertValid();
    mDescription = pd;

    mNum = pd->getNumAllowed();
    mType = pd->getType();
    mEmbedDesc.reset(NULL);

    if (mType == T_EMBEDDEDCHUNK) {
        EnumEntry *e = mDescription->getEnumEntryAtIndex (0);
        if (e)
            mEmbedDesc = ChunkFactory::instance()->getChunkDesc (e->getName());
    }

    /* the idea here is that if num == -1 we can add values to
     * a property, e.g. add another active wall.
     * otherwise we can just set the extant values.
     */
    if (mNum != -1) {
        /* we're filling the vector with num copies of a
         * default VarValue */
        for (j = 0; j < mNum; j++ ) {
            v = createVarValue (j);
            mValues.push_back(v);
        }
    }
}



Property::Property (const Property& p):mValues() {
    mDescription = NULL;
    mValidation = 1;
    *this = p;
}



VarValue *Property::createVarValue (int i) {
    assertValid();

    // if i == -1, we're just tacking onto the end
    if (i == -1)
        i = mValues.size();
    if (mType == T_EMBEDDEDCHUNK) {
        ConfigChunkPtr ch = ChunkFactory::instance()->createChunk (mEmbedDesc);
        if (mDescription->getValueLabelsSize() > i)
            ch->setProperty ("Name", mDescription->getValueLabel(i));
        else {
            ch->setProperty ("Name", mDescription->getName());
        }
        return new VarValue (ch);
    }
    else
        return new VarValue (mType);
}


Property::~Property () {

    unsigned int i;

    for (i = 0; i < mValues.size(); i++)
        delete mValues[i];

    mValidation = 0;
}



#ifdef JCCL_DEBUG
void Property::assertValid () const {
    vprASSERT (mValidation == 1 && "Trying to use deleted Property");

    for (unsigned int i = 0; i < mValues.size(); i++)
        mValues[i]->assertValid();

    if (mDescription)
        mDescription->assertValid();
}
#endif



Property& Property::operator= (const Property& p) {
    assertValid();
    p.assertValid();

    unsigned int i;

    if (&p == this)
        return *this;

    mDescription = p.mDescription;
    mType = p.mType;
    mUnits = p.mUnits;
    mEmbedDesc = p.mEmbedDesc;
    mNum = p.mNum;


    for (i = 0; i < mValues.size(); i++)
        delete (mValues[i]);

    mValues.clear();

    for (i = 0; i < p.mValues.size(); i++) {
        mValues.push_back (new VarValue(*(p.mValues[i])));
    }
    return *this;
}



bool Property::operator== (const Property& p) const {
    assertValid();
    p.assertValid();

    if (mDescription != p.mDescription)
        return false;
    if (mValues.size() != p.mValues.size())
        return false;
    for (unsigned int i = 0; i < mValues.size(); i++)
        if (*(mValues[i]) != *(p.mValues[i]))
            return false;
    return true;
}



bool Property::applyUnits (CfgUnit u) {
    assertValid();

    if (mType == T_DISTANCE) {
        for (unsigned int j = 0; j < mValues.size(); j++)
            setValue( toFeet (getValue(j), u), j);
        return true;
    }
    else {
        //cerr << "Units may only be applied to Distance mValuess." <<endl;
        return false;
    }
}




std::ostream& operator << (std::ostream &out, Property& p) {
    p.assertValid();

    ConfigIO::instance()->writeProperty (out, p);
    return out;
}



const VarValue& Property::getValue (unsigned int ind) const {
    assertValid();

    if (ind >= mValues.size()) {
        return VarValue::getInvalidInstance();
    }
    return *((mValues)[ind]);
}



int Property::getNum () const {
    assertValid();

    return mValues.size();
}



const std::string& Property::getName () const {
    assertValid();

    return mDescription->getName();
}


const std::string& Property::getToken () const {
    assertValid();

    return mDescription->getToken();
}


bool Property::preSet (unsigned int ind) {
    assertValid();

    unsigned int i;
    VarValue *v;

    if (ind >= mValues.size()) {
        if (mNum == -1) {
            for (i = mValues.size(); i <= ind; i++) {
                v = createVarValue();
                mValues.push_back(v);
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
    *((mValues)[ind]) = val;
    return true;
}



bool Property::setValue (float val, int ind ) {
    assertValid();

    if (!preSet(ind))
        return false;
    *((mValues)[ind]) = val;
    return true;
}



bool Property::setValue (const std::string& val, int ind) {
    assertValid();

    if (!preSet(ind))
        return false;
    *((mValues)[ind]) = val;
    return true;
}



bool Property::setValue (ConfigChunkPtr val, int ind) {
    assertValid();

    if (!preSet(ind)) {
        vprDEBUG(vprDBG_ERROR, 1) << "Property::Preset failed!\n" << vprDEBUG_FLUSH;
        return false;
    }
    *(mValues[ind]) = val;
    return true;
}



bool Property::setValue (const VarValue& val, int ind) {
    assertValid();

    if (!preSet (ind))
        return false;
    *(mValues[ind]) = val;
    return true;
}



bool Property::tryAssign (int index, const char* val) {
    assertValid();

    /* This does some type-checking and translating before just
     * doing an assign into the right mValues entry of p. Some of
     * this functionality ought to just be subsumed by VarValue
     * itself, but this way we get back some feedback about
     * wether a type mismatch occurred (ie we return false if
     * a type mismatch occurs ).
     *
     * Incidentally, this is also where string Values get
     * mangled into enumeration entries when assigning strings
     * to T_INTs.
     */
    char* endval;
    int i;
    float f;
    bool b;

    if (mType != T_CHUNK) {          // T_CHUNKS have enumeration, but they are really strings (or something)
        EnumEntry* e = mDescription->getEnumEntry (val);
        if (e) {
            setValue (e->getValue());
            return true;
        }
    }

    switch (mType) {
    case T_INT:
        i = strtol (val, &endval, 0);
        if (*endval != '\0')
            vprDEBUG (jcclDBG_CONFIG, 0) << clrOutNORM(clrYELLOW, "WARNING:") << " Parser expected int, got '"
                  << val << "'\n" << vprDEBUG_FLUSH;
        setValue (i, index);
        return true;
    case T_FLOAT:
        f = (float)strtod (val, &endval);
        if (*endval != '\0')
            vprDEBUG (jcclDBG_CONFIG, 0) << clrOutNORM(clrYELLOW, "WARNING:") << " Parser expected float, got '"
                                      << val << "'\n" << vprDEBUG_FLUSH;
        setValue (f, index);
        return true;
    case T_BOOL:
        b = false;
        if (!strcasecmp (val, true_TOKEN))
            b = true;
        else if (!strcasecmp (val, false_TOKEN))
            b = false;
        else { // we'll try to accept a numeric mValues
            b = strtol (val, &endval, 0) != 0;
            if (endval != '\0') {
                b = false;
                vprDEBUG (jcclDBG_CONFIG,0) << clrOutNORM(clrYELLOW, "WARNING:") << " Parser expected bool, got '"
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
