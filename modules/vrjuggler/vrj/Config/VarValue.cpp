
#include <vjConfig.h>
#include <ctype.h>
#include <Config/vjVarValue.h>
#include <Config/vjConfigChunk.h>
#include <Kernel/vjDebug.h>

vjVarValue* vjVarValue::invalid_instance = NULL;
std::string vjVarValue::using_invalid_msg = "Using T_INVALID VarValue - something's messed up";

vjVarValue::vjVarValue (vjVarValue &v) {
    *this = v;
}


vjVarValue::vjVarValue (vjConfigChunk* ch) {
    type = T_EMBEDDEDCHUNK;
    val.embeddedchunkval = ch;
}


vjVarValue::vjVarValue ( VarType t ) {
    type = t;
    switch (type) {
    case T_INT:
	val.intval = 0;
	break;
    case T_EMBEDDEDCHUNK:
	val.embeddedchunkval = NULL;
	break;
    case T_BOOL:
	val.boolval = false;
	break;
    case T_FLOAT:
	val.floatval = 0.0;
	break;
    case T_STRING:
    case T_CHUNK:
	val.strval = "";
	break;
    }
}



vjVarValue::~vjVarValue() {
    if ((type == T_EMBEDDEDCHUNK) && val.embeddedchunkval)
	delete val.embeddedchunkval;
}



vjVarValue& vjVarValue::operator= (const vjVarValue &v) {
    type = v.type;
    switch (type) {
	case T_INVALID:
	break;
	case T_INT:
	val.intval = v.val.intval;
	break;
	case T_FLOAT:
	val.floatval = v.val.floatval;
	break;
	case T_BOOL:
	val.boolval = v.val.boolval;
	break;
	case T_EMBEDDEDCHUNK:
	val.embeddedchunkval = new vjConfigChunk (*v.val.embeddedchunkval);
	break;
	case T_STRING:
	case T_CHUNK:
	val.strval = v.val.strval;
	break;
	default:
	//cout << "something's wrong with varvalue assign\n" << vjDEBUG_FLUSH;
	break;
    }
    return *this;
}



//: Equality Operator
bool vjVarValue::operator == (const vjVarValue& v) {
    if (type != v.type)
        return false;
    switch (type) {
    case T_INT:
	return (val.intval == v.val.intval);
    case T_FLOAT:
	return (val.floatval == v.val.floatval);
    case T_STRING:
    case T_CHUNK:
	return (val.strval == v.val.strval);
    case T_BOOL:
	return (val.boolval == v.val.boolval);
    case T_EMBEDDEDCHUNK:
	return (*(val.embeddedchunkval) == *(v.val.embeddedchunkval));
    default:
	return false;
    }
}




vjVarValue::operator int() {
    switch (type) {
    case T_INT:
	return val.intval;
    case T_BOOL:
	return val.boolval;
    case T_FLOAT:
	return (int)val.floatval;
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
	return new vjConfigChunk (*val.embeddedchunkval);
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
	return val.boolval;
    switch (type) {
    case T_BOOL:
	return val.boolval;
    case T_INT:
	return (bool)val.intval;
    case T_FLOAT:
	return (bool)val.floatval;
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
	return val.floatval;
    case T_INT:
	return (float)val.floatval;
    case T_BOOL:
	return (float)val.boolval;
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
	return strdup (val.strval.c_str());
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
	return val.strval;
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
	val.intval = i;
	break;
    case T_FLOAT:
	val.floatval = (float)i;
	break;
    case T_BOOL:
	val.boolval = (bool)i;
	break;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
    }
    return *this;
}



vjVarValue& vjVarValue::operator = (bool i) {
    switch (type) {
    case T_INT:
	val.intval = (int)i;
	break;
    case T_BOOL:
	val.boolval = i;
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
	val.floatval = i;
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
	val.strval = s;
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
	val.strval = s;
	break;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
    }
    return *this;
}


vjVarValue &vjVarValue::operator = (vjConfigChunk *s) {
    switch (type) {
    case T_EMBEDDEDCHUNK:
	if (val.embeddedchunkval)
	    delete val.embeddedchunkval;
	if (s)
	    val.embeddedchunkval = new vjConfigChunk (*s);
	else
	    val.embeddedchunkval = NULL;
	break;
    default:
	vjDEBUG(vjDBG_ERROR,0) << "vjVarValue: type mismatch in assignment.\n" << vjDEBUG_FLUSH;
    }
    return *this;
}



ostream& operator << (ostream& out, vjVarValue& v) {
    //      vjDEBUG(vjDBG_ERROR,0) << "in << func" <<flush;

    switch (v.type) {
    case T_INT:
	out << v.val.intval;
	return out;
    case T_FLOAT:
    case T_DISTANCE:
	out << v.val.floatval;
	return out;
    case T_BOOL:
	out << ((v.val.boolval)?"true":"false");
	return out;
    case T_STRING:
    case T_CHUNK:
	out << v.val.strval;
	return out;
    case T_EMBEDDEDCHUNK:
	if (v.val.embeddedchunkval)
	    out << *(v.val.embeddedchunkval);
	return out;
    default:
	out << "[can't print value for type " << v.type << " ]";
	return out;
    }
}
