/* vjConfigChunk.cpp */



#include <vjConfig.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjParseUtil.h>
#include <Kernel/vjDebug.h>
#include <Config/vjChunkFactory.h>
#include <Config/vjChunkDesc.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>



vjConfigChunk::vjConfigChunk (vjChunkDesc *d) :props(), type_as_varvalue(T_STRING) {

    desc = d;
    type_as_varvalue = desc->getToken();
    //cout << "creating properties" << endl;
    for (int i = 0; i < desc->plist.size(); i++) {
	vjPropertyDesc* pd = desc->plist[i];
	//cout << "propdesc is " << flush << *pd << endl;
	vjProperty* pr = new vjProperty (pd);
	//cout << "created property " << flush << *pr << endl;
	props.push_back (pr);
    }
    //cout << "done creating properties" << endl;
}



vjConfigChunk::~vjConfigChunk () {
//     cout << "I thought we weren't deleting chunks anymore? -- self is" << endl
// 	 << *this << "-------------" << endl;
    for (int i = 0; i < props.size(); i++)
	delete (props[i]);
}



vjConfigChunk::vjConfigChunk (vjConfigChunk& c):props(), type_as_varvalue(T_STRING) {
    *this = c;
}



vjConfigChunk& vjConfigChunk::operator = (const vjConfigChunk& c) {
    int i;
    if (this == &c)     // ack! same object!
        return *this;

    desc = c.desc;
    type_as_varvalue = c.type_as_varvalue;

    for (i = 0; i < props.size(); i++)
        delete (props[i]);
    props.erase (props.begin(), props.end());
    for (i = 0; i < c.props.size(); i++) {
        props.push_back (new vjProperty(*(c.props[i])));
    }
    return *this;
}


//: tests for equality of two vjConfigChunks
bool vjConfigChunk::operator== (const vjConfigChunk& c) {
    // equality operator - this makes a couple of assumptions:
    // 1. the descs must be the _same_object_, not just equal.
    // 2. the properties will be in the _same_order_.  This is
    //    reasonable if 1. is true.

    if (desc != c.desc)
	return false;
    if (props.size() != c.props.size()) // probably redundant
	return false;
    for (int i = 0; i < props.size(); i++) {
	if (*(props[i]) != *(c.props[i]))
	    return false;
    }
    return true;
}


//: Compares two vjConfigChunks based on their instance names
bool vjConfigChunk::operator< (vjConfigChunk& c) {
    std::string s1 = getProperty ("name");
    std::string s2 = c.getProperty ("name");
    return s1 < s2;
}



//: Return a list of chunk names dependant upon this one
// This is used to sort a db by dependancy.
std::vector<std::string> vjConfigChunk::getDependencies()
{
   std::string chunkname;
   std::vector<std::string> dep_list;     // Create return vector
   int i, j;

   //cout << "Dependency test for " << getProperty ("name") << endl;
   for (i=0;i<props.size();i++)                       // For each property
   {
      if (props[i]->type == T_CHUNK)                  // If it is a chunk ptr
      {
         for (j=0;j<props[i]->getNum();j++)           // For each property
         {
            vjVarValue prop_var_val = props[i]->getValue(j);
            chunkname = static_cast<std::string>(prop_var_val);
            if (!(chunkname == ""))
            {
               dep_list.push_back(chunkname);
            }
         }
      }
      else if (props[i]->type == T_EMBEDDEDCHUNK)
      {
         std::vector<std::string> child_deps;
         for (j = 0; j < props[i]->getNum(); j++)
         {
            // if we ever have cyclic dependencies, we're in trouble
            child_deps = ((vjConfigChunk*)props[i]->getValue(j))->getDependencies();
            dep_list.insert (dep_list.end(), child_deps.begin(), child_deps.end());
         }
      }
   }
   return dep_list;      // Return the list
}


vjProperty* vjConfigChunk::getPropertyPtrFromName (const std::string& property) {
    for (int i = 0; i < props.size(); i++) {
	if (!vjstrcasecmp (props[i]->getName(), property))
	    return props[i];
    }
    return NULL;
}



vjProperty* vjConfigChunk::getPropertyPtrFromToken (const std::string& token) {
  for (int i = 0; i < props.size(); i++) {
    if (!vjstrcasecmp(props[i]->description->getToken(), token))
      return props[i];
  }
  return NULL;
}



ostream& operator << (ostream& out, vjConfigChunk& self) {
  out << self.desc->token << endl;
  for (int i =0; i < self.props.size(); i++) {
    out << "  " << *(self.props[i]) << endl;
  }
  out << "  End" << endl;
  return out;
}



bool vjConfigChunk::tryassign (vjProperty *p, int index, const char* val) {
    /* This does some type-checking and translating before just
     * doing an assign into the right value entry of p. Some of
     * this functionality ought to just be subsumed by vjVarValue
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

    if(p->type != T_CHUNK)          // T_CHUNKS have enumeration, but they are really strings (or something)
    {
       vjEnumEntry* e = p->getEnumEntry (val);
       if (e) {
   	   p->setValue (e->getValue());
   	   return true;
       }
    }

    switch (p->type) {
    case T_INT:
	i = strtol (val, &endval, 0);
	if (*endval != '\0')
	    vjDEBUG (vjDBG_CONFIG, 0) << "WARNING: Parser expected int, got '"
				      << val << "'\n" << vjDEBUG_FLUSH;
	p->setValue (i, index);
	return true;
    case T_FLOAT:
	f = (float)strtod (val, &endval);
	if (*endval != '\0')
	    vjDEBUG (vjDBG_CONFIG, 0) << "WARNING: Parser expected float, got '"
				      << val << "'\n" << vjDEBUG_FLUSH;
	p->setValue (f, index);
	return true;
    case T_BOOL:
	b = false;
	if (!strcasecmp (val, "true"))
	    b = true;
	else if (!strcasecmp (val, "false"))
	    b = false;
	else { // we'll try to accept a numeric value
	    b = strtol (val, &endval, 0);
	    if (endval != '\0') {
		b = false;
		vjDEBUG (vjDBG_CONFIG,0) << "WARNING: Parser expected bool, got '"
					 << val << "'\n" << vjDEBUG_FLUSH;
	    }
	}
	p->setValue (b, index);
	return true;
    case T_STRING:
    case T_CHUNK:
	p->setValue (val, index);
	return true;
    case T_EMBEDDEDCHUNK:
	cout << "NOT HANDLED HERE!" << endl;
	return false;
    default:
	return false;
    }
}





istream& operator >> (istream& in, vjConfigChunk& self) {
    /* can't really use property >> because we don't know what
     * property to assign into.
     */
    const int buflen = 1024;
    char buf[buflen];
    vjProperty *p;
    int i;
    bool quoted;

    while (readString (in, buf, buflen, NULL)) {

	if (!strcasecmp (buf, "end"))
	    break;

	// We have a string token; assumably a property name.
	if (!(p = self.getPropertyPtrFromToken (buf))) {
	    vjDEBUG(vjDBG_ALL,0) << "ERROR: Property '" << buf << "' is not found in"
		       << " Chunk " << self.desc->name << endl << vjDEBUG_FLUSH;
	    continue;
	}
	
	// We're reading a line of input for a valid Property.
	readString (in, buf, buflen, &quoted);

	if (!quoted && (buf[0] == '{')) {
	    // We're reading values until we get a close bracket.
	    i = 0;
	    for (;;) {
		readString (in, buf, buflen, &quoted);
		if (!quoted && (buf[0] == '}'))
		    break;

		// this works because the chunk >> expects the typename to have
		// already been read (which we did when looking for '}')
		if (p->type == T_EMBEDDEDCHUNK) {
		    vjConfigChunk *ch = vjChunkFactory::createChunk (p->embeddesc);
		    in >> *ch;
		    p->setValue (ch, i++);
		}
//  		else if (tok.type == TK_Unit) {
//  		    p->applyUnits (tok.unitval);
//  		}
		else {
		    if (!self.tryassign (p, i++, buf))
			vjDEBUG(vjDBG_ALL,3) << "ERROR: Assigning to property "
				   << p->getName() << endl << vjDEBUG_FLUSH;
		}
	    }

	    if ((p->num != -1) && (p->num != i))
		vjDEBUG(vjDBG_CONFIG,3) << "ERROR: vjProperty " << p->getName() << " should have "
			   << p->num << " values; " << i << " found" << endl << vjDEBUG_FLUSH;
	}
	else {
	    // we're just doing one value.
	    if (!self.tryassign (p, 0, buf))
		vjDEBUG(vjDBG_CONFIG,3) << "ERROR: Assigning to property "
			   << p->getName() << endl << vjDEBUG_FLUSH;
//  	    self.getVJCFGToken (in,tok);
//  	    if (tok.type == TK_Unit) {
//  		p->applyUnits (tok.unitval);
//  		self.getVJCFGToken (in, tok);
//  	    }
	    if (p->num > 1) {
		vjDEBUG(vjDBG_ALL,3) << "ERROR: Property " << p->getName()
			   << " expects " << p->num << " values." << endl << vjDEBUG_FLUSH;
	    }
	}
    }

    return in;
}



int vjConfigChunk::getNum (const std::string& property_token) {
    vjProperty* p = getPropertyPtrFromToken (property_token);
    if (p)
	return p->getNum();
    else
	return 0;
}



vjVarValue& vjConfigChunk::getType () {
    return type_as_varvalue;
}



vjVarValue& vjConfigChunk::getProperty (const std::string& property_token, int ind) {
   if (!vjstrcasecmp(property_token,"type"))
   {
      return type_as_varvalue;
   }

   vjProperty *p = getPropertyPtrFromToken (property_token);
   if (!p)
   {
       vjDEBUG(vjDBG_CONFIG,2) << "request for property " << property_token << " in chunk named "
	    << getProperty("Name") << " returning invalid instance" << endl;
      return vjVarValue::getInvalidInstance();
   }
   return p->getValue (ind);
}


/* we're probably gonna need to overload set for each kind of
 * value.  That gets passed on to the vjVarValue assign...
 */

bool vjConfigChunk::setProperty (const std::string& property, int val, int ind) {
    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (!p)
	return false;
    return p->setValue (val, ind);
}

bool vjConfigChunk::setProperty (const std::string& property, float val, int ind) {
    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (!p)
	return false;
    return p->setValue (val, ind);
}

bool vjConfigChunk::setProperty (const std::string& property, const std::string& val, int ind) {
    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (!p)
	return false;
    return p->setValue (val, ind);
}

bool vjConfigChunk::setProperty (const std::string& property, vjConfigChunk* val, int ind) {
    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (!p) {
	vjDEBUG (vjDBG_ALL, 1) << "setProperty: no such property " << property
			       << "\n" << vjDEBUG_FLUSH;
	return false;
    }
    return p->setValue (val, ind);
}


bool vjConfigChunk::addValue (const std::string& property, int val) {
    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (p == NULL)
	return false;
    if (p->num != -1)
	return false;
    return setProperty (property, val, p->value.size());
}

bool vjConfigChunk::addValue (const std::string& property, float val) {
    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (p == NULL)
	return false;
    if (p->num != -1)
	return false;
    return setProperty (property, val, p->value.size());
}

bool vjConfigChunk::addValue (const std::string& property, const std::string& val) {
    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (p == NULL)
	return false;
    if (p->num != -1)
	return false;
    return setProperty (property, val, p->value.size());
}

bool vjConfigChunk::addValue (const std::string& property, vjConfigChunk* val) {
    vjProperty *p;
    p = getPropertyPtrFromToken (property);
    if (p == NULL)
	return false;
    if (p->num != -1)
	return false;
    return setProperty (property, val, p->value.size());
}








