/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <ctype.h>
#include <Config/vjVarValue.h>
#include <Config/vjConfigChunk.h>
#include <Kernel/vjDebug.h>
#include <Config/vjParseUtil.h>

vjVarValue* vjVarValue::invalid_instance = NULL;
const std::string vjVarValue::using_invalid_msg = "Casting from T_INVALID VarValue - this may mean we're confused";


/*static*/ vjVarValue& vjVarValue::getInvalidInstance () {
    if (invalid_instance == NULL)
	invalid_instance = new vjVarValue (T_INVALID);
    return *invalid_instance;
}


vjVarValue::vjVarValue (const vjVarValue &v):strval("") {
    intval = 0;
    floatval = 0.0;
    embeddedchunkval = NULL;
    boolval = false;
    *this = v;
}


vjVarValue::vjVarValue (vjConfigChunk* ch):strval("") {
    intval = 0;
    floatval = 0.0;
    embeddedchunkval = NULL;
    boolval = false;
    type = T_EMBEDDEDCHUNK;
    if (ch)
	embeddedchunkval = new vjConfigChunk(*ch);
}


vjVarValue::vjVarValue ( VarType t ):strval("") {
    type = t;
    intval = 0;
    floatval = 0.0;
    embeddedchunkval = NULL;
    boolval = false;
}



vjVarValue::~vjVarValue() {
    if (embeddedchunkval)
	delete embeddedchunkval;
}



vjVarValue& vjVarValue::operator= (const vjVarValue &v) {
    if (&v == this)
	return *this;

    type = v.type;

    if (embeddedchunkval) {
	delete embeddedchunkval;
	embeddedchunkval = NULL;
    }
    intval = v.intval;
    floatval = v.floatval;
    boolval = v.boolval;
    strval = v.strval;
    if (v.embeddedchunkval)
	embeddedchunkval = new vjConfigChunk (*v.embeddedchunkval);

    return *this;
}



//: Equality Operator
bool vjVarValue::operator == (const vjVarValue& v) const {
    if (type != v.type)
        return false;
    switch (type) {
    case T_INT:
	return (intval == v.intval);
    case T_FLOAT:
	return (floatval == v.floatval);
    case T_STRING:
    case T_CHUNK:
	return (strval == v.strval);
    case T_BOOL:
	return (boolval == v.boolval);
    case T_EMBEDDEDCHUNK:
	if (embeddedchunkval) {
	    if (v.embeddedchunkval)
		return (*embeddedchunkval == *(v.embeddedchunkval));
	    else
		return false;
	}
	else
	    return (!v.embeddedchunkval);
    default:
	return false;
    }
}




vjVarValue::operator int() const {
    switch (type) {
    case T_INT:
	return intval;
    case T_BOOL:
	return boolval;
    case T_FLOAT:
	return (int)floatval;
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,4) << using_invalid_msg.c_str() << 1 
			       << endl << vjDEBUG_FLUSH;
	return 0;
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cast to int.\n" 
			      << vjDEBUG_FLUSH;
	return 0;
    }
}



vjVarValue::operator vjConfigChunk*() const {
    switch (type) {
    case T_EMBEDDEDCHUNK:
	// we need to make a copy because if the value is deleted, it deletes
	// its embeddedchunk
	return new vjConfigChunk (*embeddedchunkval);
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,4) << using_invalid_msg.c_str() << 2 
			       << endl << vjDEBUG_FLUSH;
	return NULL;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cast to vjConfigChunk* - real type is " << typeString(type) << ".\n" << vjDEBUG_FLUSH;
	return NULL;
    }
}



vjVarValue::operator bool() const {
    if ((type == T_BOOL))
	return boolval;
    switch (type) {
    case T_BOOL:
	return boolval;
    case T_INT:
	return (bool)intval;
    case T_FLOAT:
	return (bool)floatval;
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,4) << using_invalid_msg.c_str() << 3 
			       << endl << vjDEBUG_FLUSH;
	return false;
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cast to bool.\n"
			      << vjDEBUG_FLUSH;
	return false;
    }
}



vjVarValue::operator float () const {
    switch (type) {
    case T_FLOAT:
	return floatval;
    case T_INT:
	return (float)floatval;
    case T_BOOL:
	return (float)boolval;
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,4) <<  using_invalid_msg.c_str() << 4 
			       << endl << vjDEBUG_FLUSH;
	return 0.0f;
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cast to float.\n" << vjDEBUG_FLUSH;
	return 0.0f;
    }
}



char* vjVarValue::cstring () const {
    switch (type) {
    case T_STRING:
    case T_CHUNK:
	return strdup (strval.c_str());
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,4) <<  using_invalid_msg.c_str() << 5 
			       << endl << vjDEBUG_FLUSH;
	return strdup("");
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cstring().\n" << vjDEBUG_FLUSH << vjDEBUG_FLUSH;
	return strdup("");
    }
}



vjVarValue::operator std::string () const {
    switch (type) {
    case T_STRING:
    case T_CHUNK:
	return strval;
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,4) <<  using_invalid_msg.c_str() << 6 
			       << endl << vjDEBUG_FLUSH;
	return (std::string)"";
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cast to std::string.\n" << vjDEBUG_FLUSH;
	return (std::string)"";
    }
}



vjVarValue &vjVarValue::operator = (int i) {
    switch (type) {
    case T_INT:
	intval = i;
	break;
    case T_FLOAT:
	floatval = (float)i;
	break;
    case T_BOOL:
	boolval = (bool)i;
	break;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - vjVarValue(" << typeString(type) << ") = int.\n" << vjDEBUG_FLUSH;
    }
    return *this;
}



vjVarValue& vjVarValue::operator = (bool i) {
    switch (type) {
    case T_INT:
	intval = (int)i;
	break;
    case T_BOOL:
	boolval = i;
	break;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - vjVarValue(" << typeString(type) << ") = bool.\n" << vjDEBUG_FLUSH;
    }
    return *this;
}



vjVarValue &vjVarValue::operator = (float i) {
    switch (type) {
    case T_FLOAT:
    case T_DISTANCE:
	floatval = i;
	break;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - vjVarValue(" << typeString(type) << ") = float.\n" << vjDEBUG_FLUSH;
    }
    return *this;
}



vjVarValue &vjVarValue::operator = (const std::string& s) {
    return *this = s.c_str();
//      switch (type) {
//      case T_STRING:
//      case T_CHUNK:
//          strval = s;
//  	break;
//      default:
//  	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
//      }
//      return *this;
}



vjVarValue &vjVarValue::operator = (const char *val) {
    bool err = false;
    char* endval;
    int i;
    float f;
    bool b;

    switch (type) {
    case T_STRING:
    case T_CHUNK:
        strval = val;
	break;
    case T_INT:
	i = strtol (val, &endval, 0);
        if (*endval == '\0')
            intval = i;
        else
            err = true;
	break;
    case T_FLOAT:
	f = (float)strtod (val, &endval);
	if (*endval == '\0')
            floatval = f;
        else
            err = true;
	break;
    case T_BOOL:
	if (!strcasecmp (val, "true"))
	    boolval = true;
	else if (!strcasecmp (val, "false"))
	    boolval = false;
	else { // we'll try to accept a numeric value
	    b = strtol (val, &endval, 0);
	    if (*endval == '\0')
	        boolval = b;
	    else
		err = true;
	}
	break; 
    default:
        vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - vjVarValue(" << typeString(type) << ") = string/char*.\n" << vjDEBUG_FLUSH;
    for (;;);
    }
    if (err) 
        vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: couldn't assign string '" 
                               << val << "'.\n" << vjDEBUG_FLUSH;
    return *this;
}


vjVarValue &vjVarValue::operator = (vjConfigChunk *s) {
    switch (type) {
    case T_EMBEDDEDCHUNK:
	if (embeddedchunkval)
	    delete embeddedchunkval;
	if (s)
	    embeddedchunkval = new vjConfigChunk (*s);
	else
	    embeddedchunkval = NULL;
	break;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment - vjVarValue(" << typeString(type) << ") = vjConfigChunk*.\n" << vjDEBUG_FLUSH;
    }
    return *this;
}



ostream& operator << (ostream& out, const vjVarValue& v) {
    //      vjDEBUG(vjDBG_ERROR,0) << "in << func" <<flush;

    switch (v.type) {
    case T_INT:
	out << v.intval;
	return out;
    case T_FLOAT:
    case T_DISTANCE:
	out << v.floatval;
	return out;
    case T_BOOL:
	out << ((v.boolval)?"true":"false");
	return out;
    case T_STRING:
    case T_CHUNK:
	out << v.strval.c_str();
	return out;
    case T_EMBEDDEDCHUNK:
	if (v.embeddedchunkval)
	    out << *(v.embeddedchunkval);
	return out;
    default:
	out << "[can't print value for type " << v.type << " ]";
	return out;
    }
}
