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


typedef enum {
    TK_String, TK_Float, TK_End, TK_Int, TK_Bool,
    TK_OpenBracket, TK_CloseBracket, TK_Unit, TK_Error
} CfgTok;

struct VJCFGToken {
    CfgTok type;
    int intval;
    float floatval;
    bool boolval;
    CfgUnit unitval;
    char strval[1024];
};


vjConfigChunk::vjConfigChunk (vjChunkDesc *d) :props(), type_as_varvalue(T_STRING) {

    desc = d;
    type_as_varvalue = desc->getToken();
    for (int i = 0; i < desc->plist.size(); i++)
	props.push_back (new vjProperty(desc->plist[i]));
}



vjConfigChunk::~vjConfigChunk () {
  for (int i = 0; i < props.size(); i++)
    delete (props[i]);
}



vjConfigChunk::vjConfigChunk (vjConfigChunk& c):props(), type_as_varvalue(T_STRING) {
    *this = c;
}



vjConfigChunk& vjConfigChunk::operator = (const vjConfigChunk& c) {
    int i;
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
    for(i=0;i<props.size();i++) {
	if(props[i]->type == T_CHUNK)
	    for(j=0;j<props[i]->getNum();j++) {
		chunkname = static_cast<std::string>(props[i]->getValue(j));
		if (!(chunkname == "")) {
		    dep_list.push_back(chunkname);
		}
	    }
	if (props[i]->type == T_EMBEDDEDCHUNK) {
	    std::vector<std::string> child_deps;
	    for (j = 0; j < props[i]->getNum(); j++) {
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



struct TokTableEntry {
  char*   name;
  CfgTok  token;
  CfgUnit unit;
  bool    bval;
};

static TokTableEntry TokTable[] = {
  { "end", TK_End, U_BadUnit, 0 },
  { "{", TK_OpenBracket, U_BadUnit, 0 },
  { "}", TK_CloseBracket, U_BadUnit, 0 },
  { "true", TK_Bool, U_BadUnit, 1},
  { "yes", TK_Bool, U_BadUnit, 1},
  { "false", TK_Bool, U_BadUnit, 0},
  { "no", TK_Bool, U_BadUnit, 0},
  { "feet", TK_Unit, U_Feet, 0},
  { "inches", TK_Unit, U_Inches, 0},
  { "meters", TK_Unit, U_Meters, 0},
  { "centimeters", TK_Unit, U_Meters, 0},
  { "", TK_Error, U_BadUnit, 0},
};


bool vjConfigChunk::getVJCFGToken (istream& in, VJCFGToken& tok) {
    /* this could stand to be reimplemented */

    int i,j;
    bool dbl = false, alpha = false;
    bool quoted;

    if (readString(in,  tok.strval,  1024,  &quoted) == 0) {
	tok.type = TK_End;
	return true;
    }

    // Is it a quoted string?
    if (quoted) {
	tok.type = TK_String;
	return true;
    }

    // Is it one of our actual tokens?
    for (j = 0; TokTable[j].token != TK_Error; j++) {
	if (!strcasecmp (TokTable[j].name, tok.strval)) {
	    tok.type = TokTable[j].token;
	    tok.boolval = TokTable[j].bval;
	    tok.unitval = TokTable[j].unit;
	    return true;
	}
    }

    // Is it a number?
    for (i = 0; i <strlen(tok.strval); i++) {
	if (!(isdigit (tok.strval[i]) || (tok.strval[i] == '.')
	      || (tok.strval[i] == '+')
	      || (tok.strval[i] == '-'))) {
	    alpha = true;
	    break;
	}
	if (tok.strval[i] == '.')
	    dbl = true;
    }
    if (alpha) {
	/* it's a string */
	tok.type = TK_String;
	return true;
    }
    if (dbl) {
	tok.type = TK_Float;
	tok.floatval = (float)strtod (tok.strval,NULL);
	return true;
    }
    /* if all else has failed, it's an int. */
    tok.type = TK_Int;
    tok.intval = atoi (tok.strval);
    return true;

}



bool vjConfigChunk::tryassign (vjProperty *p, VJCFGToken &tok, int i) {
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
    vjEnumEntry *e;

    switch (tok.type) {

    case TK_Int:
	// ints can be assigned to T_INT, T_FLOAT, or T_DISTANCE.
	if (p->type == T_INT) {
	    p->setValue(tok.intval, i);
	    return true;
	}
	else if ((p->type == T_FLOAT) || (p->type == T_DISTANCE)) {
	    p->setValue ((float)tok.intval, i);
	    return true;
	}
	else
	    return false;
	
    case TK_Float:
	// doubles can be assigned to T_FLOAT or T_DISTANCE
	if ((p->type == T_FLOAT) || (p->type == T_DISTANCE)) {
	    p->setValue (tok.floatval, i);
	    return true;
	}
	else
	    return false;
	
    case TK_Bool:
	// bools are bools and only bools
	if (p->type == T_BOOL) {
	    p->setValue (tok.boolval, i);
	    return true;
	}
	else
	    return false;

    case TK_String:
	// Strings can be assigned to T_STRINGs, T_CHUNKS, or T_INTs if there's an enum entry
	if (p->type == T_STRING) {
	    p->setValue (tok.strval, i);
	    return true;
	}
	else if (p->type == T_CHUNK) {
	    p->setValue (tok.strval, i);
	    return true;
	}
	else if ((p->type == T_INT)) {
	    // look it up in the enumerations for p.
	    e = p->getEnumEntry (tok.strval);
	    if (e) {
		p->setValue (e->getVal(), i);
		return true;
	    }
	    return false;
	}
	else
	    return false;
	
    default:
	return false;
    }
}



istream& operator >> (istream& in, vjConfigChunk& self) {
    /* can't really use property >> because we don't know what
     * property to assign into.
     */
    vjProperty *p;
    VJCFGToken tok;
    int i;

    self.getVJCFGToken (in, tok);

    while (tok.type != TK_End) {
	
	if (tok.type != TK_String) {
	    vjDEBUG(vjDBG_ALL,3) << "ERROR: Unexpected Token #" << tok.type << endl << vjDEBUG_FLUSH;
	    self.getVJCFGToken(in,tok);
	    continue;
	}
	
	// We have a string token; assumably a property name.
	if (!(p = self.getPropertyPtrFromToken (tok.strval))) {
	    vjDEBUG(vjDBG_ALL,3) << "ERROR: Property '" << tok.strval << "' is not found in"
		       << " Chunk " << self.desc->name << endl << vjDEBUG_FLUSH;
	    self.getVJCFGToken(in,tok);
	    continue;
	}
	
	// We're reading a line of input for a valid Property.
	self.getVJCFGToken (in, tok);
	if (tok.type == TK_OpenBracket) {
	    // We're reading values until we get a TK_CloseBracket.
	    i = 0;
	    self.getVJCFGToken (in, tok);
	    while ((tok.type != TK_CloseBracket) && (tok.type != TK_End)) {
		if (p->type == T_EMBEDDEDCHUNK) {
		    vjConfigChunk *ch = vjChunkFactory::createChunk (p->embeddesc);
		    in >> *ch;
		    p->setValue (ch, i++);
		}
		else if (tok.type == TK_Unit) {
		    p->applyUnits (tok.unitval);
		}
		else {
		    if (!self.tryassign (p, tok, i++))
			vjDEBUG(vjDBG_ALL,3) << "ERROR: Assigning to property "
				   << p->getName() << endl << vjDEBUG_FLUSH;
		}
		self.getVJCFGToken (in, tok);
	    }
	    if ((p->num != -1) && (p->num != i))
		vjDEBUG(vjDBG_ALL,3) << "ERROR: vjProperty " << p->getName() << " should have "
			   << p->num << " values; " << i << " found" << endl << vjDEBUG_FLUSH;
	    if (tok.type != TK_CloseBracket)
		vjDEBUG(vjDBG_ALL,3) << "ERROR: vjProperty " << p->getName() << ": '}' expected"
			   << endl << vjDEBUG_FLUSH;
	    self.getVJCFGToken (in,tok);
	}
	else {
	    // we're just doing one value.
	    if (!self.tryassign (p, tok, 0))
		vjDEBUG(vjDBG_ALL,3) << "ERROR: Assigning to property "
			   << p->getName() << endl << vjDEBUG_FLUSH;
	    self.getVJCFGToken (in,tok);
	    if (tok.type == TK_Unit) {
		p->applyUnits (tok.unitval);
		self.getVJCFGToken (in, tok);
	    }
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
	vjDEBUG (vjDBG_ALL, 0) << "setProperty: no such property " << property
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








