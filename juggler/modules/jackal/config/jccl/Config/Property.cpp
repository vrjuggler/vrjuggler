
// vjProperty.cpp


#include <vjConfig.h>
#include <Config/vjProperty.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjChunkFactory.h>
#include <Config/vjPropertyDesc.h>
#include <Config/vjEnumEntry.h>
#include <Kernel/vjDebug.h>

vjProperty::vjProperty (vjPropertyDesc *pd):value() {
    int j;
    vjVarValue *v;

    description = pd;
    num = pd->getNumAllowed();
    type = pd->getType();
    embeddesc = NULL;

    if (type == T_EMBEDDEDCHUNK) {
	vjEnumEntry *e = description->getEnumEntryAtIndex (0);
	if (e) 
	    embeddesc = vjChunkFactory::getChunkDesc (e->getName());
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
	vjConfigChunk *ch = vjChunkFactory::createChunk (embeddesc);
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
    int i;

    for (i = 0; i < value.size(); i++)
	delete (value)[i];
}



vjProperty::vjProperty (vjProperty& p):value() {
    *this = p;
}



vjProperty& vjProperty::operator= (const vjProperty& p) {
    int i;

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

bool vjProperty::operator== (const vjProperty& p) {
    if (description != p.description)
	return false;
    if (value.size() != p.value.size())
	return false;
    for (int i = 0; i < value.size(); i++)
	if (*(value[i]) != *(p.value[i]))
	    return false;
    return true;
}


bool vjProperty::applyUnits (CfgUnit u) {

    if (type == T_DISTANCE) {
	for (int j = 0; j < value.size(); j++)
	    setValue( toFeet (getValue(j), u), j);
	return true;
    }
    else {
	cerr << "Units may only be applied to Distance values." <<endl;
	return false;
    }
}




vjEnumEntry* vjProperty::getEnumEntryWithValue (int val) {
    // gets an enumentry based on the value, instead of the name
    vjVarValue v(T_INT);
    v = val;
    return description->getEnumEntryWithValue (v);
}



ostream& operator << (ostream &out, vjProperty& p) {
    out << p.getToken () << " { ";
    for (int i = 0; i < p.value.size(); i++) {
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
		out << e->getName();
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



vjVarValue& vjProperty::getValue (int ind) {
    if ((ind < 0) || (ind >= value.size())) {
	return vjVarValue::getInvalidInstance();
    }
    return *((value)[ind]);
}



int vjProperty::getNum () {
    return value.size();
}



std::string& vjProperty::getName () {
    return description->getName();
}


std::string& vjProperty::getToken () {
    return description->getToken();
}


bool vjProperty::preSet (int ind) {
    int i;
    vjVarValue *v;

    if (ind < 0)
	return false;
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
	vjDEBUG(vjDBG_ALL, 0) << "Preset failed\n" << vjDEBUG_FLUSH;
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

