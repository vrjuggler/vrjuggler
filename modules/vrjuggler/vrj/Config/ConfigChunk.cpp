/* vjConfigChunk.C */



#include <config.h>
#include <Config/vjConfigChunk.h>
#include <Config/vjParseUtil.h>
#include <Kernel/vjDebug.h>



vjConfigChunk::vjConfigChunk (vjChunkDesc *d) :props() {

  desc = d;
  for (int i = 0; i < desc->plist.size(); i++)
    props.push_back (new vjProperty(desc->plist[i]));
}



vjConfigChunk::~vjConfigChunk () {
  for (int i = 0; i < props.size(); i++)
    delete (props[i]);
}



vjProperty* vjConfigChunk::getPropertyPtr (char *property) {
  for (int i = 0; i < props.size(); i++) {
    if (!strcasecmp(props[i]->name,property))
      return props[i];
  }
  return NULL;
}



vjProperty* vjConfigChunk::getPropertyFromToken (char *token) {
  for (int i = 0; i < props.size(); i++) {
    if (!strcasecmp(props[i]->description->token,token))
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


bool vjConfigChunk::getToken (istream& in, Token& tok) {
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



bool vjConfigChunk::tryassign (vjProperty *p, Token &tok, int i) {
  /* This does some type-checking and translating before just
   * doing an assign into the right value entry of p. Some of
   * this functionality ought to just be subsumed by vjVarValue
   * itself, but this way we get back some feedback about 
   * wether a type mismatch occurred (ie we return false if
   * a type mismatch occurs ).
   * 
   * Incidentally, this is also where string values get mangled into
   * enumeration entries when assigning strings to T_INTs.
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
  Token tok;
  int i;

  self.getToken (in, tok);
  
  while (tok.type != TK_End) {

    if (tok.type != TK_String) {
      vjDEBUG(3) << "ERROR: Unexpected Token #" << tok.type << endl << vjDEBUG_FLUSH;
      self.getToken(in,tok);
      continue;
    }

    // We have a string token; assumably a property name.
    if (!(p = self.getPropertyFromToken (tok.strval))) {
      vjDEBUG(3) << "ERROR: Property '" << tok.strval << "' is not found in"
		 << " Chunk " << self.desc->name << endl << vjDEBUG_FLUSH;
      self.getToken(in,tok);
      continue;
    }

    // We're reading a line of input for a valid Property.
    self.getToken (in, tok);
    if (tok.type == TK_OpenBracket) {
      // We're reading values until we get a TK_CloseBracket.
      i = 0;
      self.getToken (in, tok);
      while ((tok.type != TK_CloseBracket) && (tok.type != TK_End)) {
	if (tok.type == TK_Unit) {
	  p->applyUnits (tok.unitval);
	} 
	else {
	  if (!self.tryassign (p, tok, i++))
	    vjDEBUG(3) << "ERROR: Assigning to property " 
		       << p->name << endl << vjDEBUG_FLUSH;
	}
	self.getToken (in, tok);
      }
      if ((p->num != -1) && (p->num != i))
	vjDEBUG(3) << "ERROR: vjProperty " << p->name << " should have "
		   << p->num << " values; " << i << " found" << endl << vjDEBUG_FLUSH;
      if (tok.type != TK_CloseBracket)
	vjDEBUG(3) << "ERROR: vjProperty " << p->name << ": '}' expected" 
		   << endl << vjDEBUG_FLUSH;
      self.getToken (in,tok);
    }
    else {
      // we're just doing one value.
      if (!self.tryassign (p, tok, 0))
	vjDEBUG(3) << "ERROR: Assigning to property "
		   << p->name << endl << vjDEBUG_FLUSH;
      self.getToken (in,tok);
      if (tok.type == TK_Unit) {
	p->applyUnits (tok.unitval);
	self.getToken (in, tok);
      } 
      if (p->num > 1) {
	vjDEBUG(3) << "ERROR: Property " << p->name
		   << " expects " << p->num << " values." << endl << vjDEBUG_FLUSH;
      }
    }
  }

  return in;
}



int vjConfigChunk::getNum (char *property) {
  for (int i = 0; i < props.size(); i++) 
    if (!strcasecmp(props[i]->name, property)) 
      return props[i]->getNum();
  return 0;
}



vjVarValue vjConfigChunk::getType () {

  vjVarValue v(T_STRING);
  v = desc->token;
  return v;
} 
 


vjVarValue vjConfigChunk::getProperty (char *property, int ind) {
  
  if (!strcasecmp(property,"type")) {
    vjVarValue v(T_STRING);
    v = desc->token;
    return v;
  }

  vjProperty *p = getPropertyPtr (property);
  if (!p) {
    vjVarValue v(T_INVALID);
    return v;
  }
  return p->getValue (ind);
}


/* we're probably gonna need to overload set for each kind of
 * value.  That gets passed on to the vjVarValue assign...
 */

bool vjConfigChunk::setProperty (char *property, int val, int ind) {
  vjProperty *p;
  p = getPropertyPtr (property);
  if (!p)
    return false;
  return p->setValue (val, ind);
}

bool vjConfigChunk::setProperty (char *property, float val, int ind) {
  vjProperty *p;
  p = getPropertyPtr (property);
  if (!p)
    return false;
  return p->setValue (val, ind);
}

bool vjConfigChunk::setProperty (char *property, char* val, int ind) {
  vjProperty *p;
  p = getPropertyPtr (property);
  if (!p)
    return false;
  return p->setValue (val, ind);
}


bool vjConfigChunk::addValue (char *property, int val) {
  vjProperty *p;
  p = getPropertyPtr (property);
  if (p == NULL)
    return false;
  if (p->num != -1)
    return false;
  return setProperty (property, val, p->value.size());
}

bool vjConfigChunk::addValue (char *property, float val) {
  vjProperty *p;
  p = getPropertyPtr (property);
  if (p == NULL)
    return false;
  if (p->num != -1)
    return false;
  return setProperty (property, val, p->value.size());
}

bool vjConfigChunk::addValue (char *property, char* val) {
  vjProperty *p;
  p = getPropertyPtr (property);
  if (p == NULL)
    return false;
  if (p->num != -1)
    return false;
  return setProperty (property, val, p->value.size());
}








