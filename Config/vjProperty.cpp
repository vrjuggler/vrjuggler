/* vjProperty.C
 *
 */


#include <vjConfig.h>
#include <Config/vjProperty.h>



vjProperty::vjProperty (vjPropertyDesc *pd):value() {
  int j;
  vjVarValue *v;

  description = pd;
  name = pd->token;
  num = pd->num;
  type = pd->type;

  /* the idea here is that if num == -1 we can add values to a property,
   * e.g. add another active wall.
   * otherwise we can just set the extant values.
   */
  if (num != -1) {
    /* we're filling the vector with num copies of a default vjVarValue */
    for (j = 0; j < num; j++ ) {
      v = new vjVarValue(type);
      value.push_back(v);
    }
  }
}



vjProperty::~vjProperty () {
  int i;

  for (i = 0; i < value.size(); i++) 
    delete (value)[i];
}



vjProperty::vjProperty (vjProperty& p):value() {
    *this = p;
}



vjProperty& vjProperty::operator= (vjProperty& p) {
    int i;

    description = p.description;
    name = description->token;
    num = p.num;
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



vjEnumEntry* vjProperty::getEnumEntry (char *n) {
  for (int i = 0; i < description->enumv.size(); i++)
    if (!strcasecmp (n, description->enumv[i]->getName()))
      return description->enumv[i];
  return NULL;
}



vjEnumEntry* vjProperty::getEnumEntry (int val) {
  // gets an enumentry based on the value, instead of the name
  for (int i = 0; i < description->enumv.size(); i++)
    if (val == description->enumv[i]->getVal())
      return description->enumv[i];
  return NULL;
}



ostream& operator << (ostream &out, vjProperty& p) {
  out << p.name << " { ";
  for (int i = 0; i < p.value.size(); i++) {
    vjVarValue *v = ((p.value))[i];

    if ((p.type == T_STRING) || (p.type == T_CHUNK)) {
      out << '"' << *v << '"';
    }
    else if (p.description->enumv.size() > 0) {
      vjEnumEntry *e = p.getEnumEntry((int)(*v));
      if (e)
	out << e->getName();
      else
	out << *v;
    } 
    else {
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
    vjVarValue v(T_INVALID);
    return v;
  }
  return *((value)[ind]);
}


int vjProperty::getNum () {
  return value.size();
}



bool vjProperty::preSet (int ind) {
  int i;
  vjVarValue *v;
  
  if (ind < 0)
    return false;
  if (ind >= value.size()) {
    if (num == -1) {
      for (i = value.size(); i <= ind; i++) {
	v = new vjVarValue(type);
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



bool vjProperty::setValue (char* val, int ind) {
  
  if (!preSet(ind))
    return false;
  *((value)[ind]) = val;
  return true;
}

