
#include <vjConfig.h>
#include <ctype.h>
#include <Config/vjVarValue.h>
#include <Config/vjConfigChunk.h>
#include <Kernel/vjDebug.h>

vjVarValue* vjVarValue::invalid_instance = NULL;
std::string vjVarValue::using_invalid_msg = "Using T_INVALID VarValue - something's messed up";

vjVarValue::vjVarValue (vjVarValue &v):strval("") {
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
bool vjVarValue::operator == (const vjVarValue& v) {
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




vjVarValue::operator int() {
    switch (type) {
    case T_INT:
	return intval;
    case T_BOOL:
	return boolval;
    case T_FLOAT:
	return (int)floatval;
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,1) << using_invalid_msg << 1 
			       << endl << vjDEBUG_FLUSH;
	return 0;
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cast to int.\n" 
			      << vjDEBUG_FLUSH;
	return 0;
    }
}



vjVarValue::operator vjConfigChunk*() {
    switch (type) {
    case T_EMBEDDEDCHUNK:
	// we need to make a copy because if the value is deleted, it deletes
	// its embeddedchunk
	return new vjConfigChunk (*embeddedchunkval);
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,1) << using_invalid_msg << 2 
			       << endl << vjDEBUG_FLUSH;
	return NULL;
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cast to vjConfigChunk*.\n" << vjDEBUG_FLUSH;
	return NULL;
    }
}



vjVarValue::operator bool() {
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
	vjDEBUG(vjDBG_CONFIG,1) << using_invalid_msg << 3 
			       << endl << vjDEBUG_FLUSH;
	return false;
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cast to bool.\n"
			      << vjDEBUG_FLUSH;
	return false;
    }
}



vjVarValue::operator float () {
    switch (type) {
    case T_FLOAT:
	return floatval;
    case T_INT:
	return (float)floatval;
    case T_BOOL:
	return (float)boolval;
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,1) << using_invalid_msg << 4 
			       << endl << vjDEBUG_FLUSH;
	return 0.0f;
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cast to float.\n" << vjDEBUG_FLUSH;
	return 0.0f;
    }
}



char* vjVarValue::cstring () {
    switch (type) {
    case T_STRING:
    case T_CHUNK:
	return strdup (strval.c_str());
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,1) << using_invalid_msg << 5 
			       << endl << vjDEBUG_FLUSH;
	return strdup("");
    default:
       vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in cstring().\n" << vjDEBUG_FLUSH << vjDEBUG_FLUSH;
	return strdup("");
    }
}



vjVarValue::operator std::string () {
    switch (type) {
    case T_STRING:
    case T_CHUNK:
	return strval;
    case T_INVALID:
	vjDEBUG(vjDBG_CONFIG,1) << using_invalid_msg << 6 
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
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
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
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
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
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
    }
    return *this;
}



vjVarValue &vjVarValue::operator = (std::string s) {
    switch (type) {
    case T_STRING:
    case T_CHUNK:
	strval = s;
	break;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
    }
    return *this;
}



vjVarValue &vjVarValue::operator = (char *s) {
    switch (type) {
    case T_STRING:
    case T_CHUNK:
	strval = s;
	break;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
    }
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
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
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
	out << v.strval;
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
