/* vjPropery.C
 *
 */


#include <config.h>
#include <Config/vjConfigChunk.h>



vjPropery::vjPropery (vjPropertyDesc *pd):value() {
  int j;
  vjVarValue *v;

  description = pd;
  name = pd->name;
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



vjPropery::~vjPropery () {
  int i;

  for (i = 0; i < value.size(); i++) 
    delete (value)[i];
}



bool vjPropery::applyUnits (CfgUnit u) {

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



vjEnumEntry* vjPropery::getEnumEntry (char *n) {
  for (int i = 0; i < description->enumv.size(); i++)
    if (!strcasecmp (n, description->enumv[i]->getName()))
      return description->enumv[i];
  return NULL;
}



vjEnumEntry* vjPropery::getEnumEntry (int val) {
  // gets an enumentry based on the value, instead of the name
  for (int i = 0; i < description->enumv.size(); i++)
    if (val == description->enumv[i]->getVal())
      return description->enumv[i];
  return NULL;
}



ostream& operator << (ostream &out, vjPropery& p) {
  out << p.description->token;
  out << " { ";
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


vjVarValue& vjPropery::getValue (int ind) {
  if ((ind < 0) || (ind >= value.size())) {
    vjVarValue v(T_INVALID);
    return v;
  }
  return *((value)[ind]);
}


int vjPropery::getNum () {
  return value.size();
}



bool vjPropery::preSet (int ind) {
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



bool vjPropery::setValue (int val, int ind ) {
  
  if (!preSet(ind))
    return false;
  *((value)[ind]) = val;
  return true;
}



bool vjPropery::setValue (float val, int ind ) {

  if (!preSet(ind))
    return false;
  *((value)[ind]) = val;
  return true;
}



bool vjPropery::setValue (char* val, int ind) {
  
  if (!preSet(ind))
    return false;
  *((value)[ind]) = val;
  return true;
}

