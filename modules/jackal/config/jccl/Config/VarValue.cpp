
#include <vjConfig.h>
#include <ctype.h>
#include <Config/vjVarValue.h>
#include <Config/vjConfigChunk.h>

vjVarValue::vjVarValue (vjVarValue &v) {
    *this = v;
}


vjVarValue::vjVarValue (vjConfigChunk* ch) {
    type = T_EMBEDDEDCHUNK;
    val.embeddedchunkval = ch;
}



vjVarValue& vjVarValue::operator= (vjVarValue &v) {
   if ((type == T_STRING) || (type == T_CHUNK))
      if (val.strval)
      {
         delete val.strval;
         val.strval = NULL;
      }
   type = v.type;
   switch (type)
   {
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
      if (v.val.strval == NULL)
         val.strval = NULL;
      else
         val.strval = strdup (v.val.strval);
      break;
   default:
      //cout << "something's wrong with varvalue assign" << endl;
      break;
   }
   return *this;
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
	val.strval = NULL;
	break;
    }
}



vjVarValue::~vjVarValue() {
    if (((type == T_STRING) || (type == T_CHUNK)) && val.strval)
	delete val.strval;
    if ((type == T_EMBEDDEDCHUNK) && val.embeddedchunkval)
	delete val.embeddedchunkval;
}



vjVarValue::operator int() {
    if ((type == T_INT) || (type == T_BOOL))
	return val.intval;
    if (type != T_INVALID)
	cerr << "Type error in cast!\n";
    return 0;
}



vjVarValue::operator vjConfigChunk*() {
    if ((type == T_EMBEDDEDCHUNK))
	return new vjConfigChunk (*val.embeddedchunkval);
    if (type != T_INVALID)
	cerr << "Type error in cast!\n";
    // this is a very bad thing to do...
    return NULL;
}



vjVarValue::operator bool() {
    if ((type == T_BOOL))
	return val.boolval;
    if (type != T_INVALID)
	cerr << "Type error in cast!\n";
    return 0;
}



vjVarValue::operator float () {
    if ((type == T_FLOAT) || (type == T_DISTANCE))
	return val.floatval;
    if (type != T_INVALID)
	cerr << "Type error in cast!\n";
    return 0.0;
}



vjVarValue::operator char* () {
    if ((type == T_STRING) || (type == T_CHUNK)) {
	if (val.strval) {
	    return strdup (val.strval);
	}
	else
	    return strdup("");
    }
    if (type != T_INVALID)
	cerr << "Type error in cast to char*!\n";
    return NULL;
}

vjVarValue::operator std::string () {
    if ((type == T_STRING) || (type == T_CHUNK)) {
	if (val.strval) {
	    return (std::string)val.strval;
	}
	else
	    return (std::string)"";
    }
    if (type != T_INVALID)
	cerr << "Type error in cast to std::string!\n";
    return (std::string)"";
}



vjVarValue &vjVarValue::operator = (int i) {
    if (type == T_INT) {
	val.intval = i;
	return *this;
    }
    else if (type == T_BOOL) {
	val.boolval = i;
	return *this;
    }
    else {
	cerr << "Type error in assignment!\n";
	return *this;
    }
}



vjVarValue& vjVarValue::operator = (bool i) {
    if (type == T_BOOL) {
	val.boolval = i;
	return *this;
    }
    else if (type == T_INT) {
	val.intval = i;
	return *this;
    }
    else {
	cerr << "Type error in assignment!\n";
	return *this;
    }
}



vjVarValue &vjVarValue::operator = (float i) {
    if ((type == T_FLOAT) || (type == T_DISTANCE)) {
	val.floatval = i;
	return *this;
    } else {
	cerr << "Type error in assignment!\n";
	return *this;
    }
}



vjVarValue &vjVarValue::operator = (char *s) {
    if ((type == T_STRING) || (type == T_CHUNK)) {
	if (val.strval)
	    delete val.strval;
	if (s)
	    val.strval = strdup(s);
	else
	    val.strval = NULL;
	return *this;
    } else {
	cerr << "Type error in assignment!\n";
	return *this;
    }
}



vjVarValue &vjVarValue::operator = (vjConfigChunk *s) {
    if (type == T_EMBEDDEDCHUNK) {
	if (val.embeddedchunkval)
	    delete val.embeddedchunkval;
	if (s)
	    val.embeddedchunkval = new vjConfigChunk (*s);
	else
	    val.embeddedchunkval = NULL;
	return *this;
    } else {
	cerr << "Type error in assignment!\n";
	return *this;
    }
}



ostream& operator << (ostream& out, vjVarValue& v) {
    //      cerr << "in << func" <<flush;

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
	if (v.val.strval)
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
