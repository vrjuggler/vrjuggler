#include <vjConfig.h>
#include <strings.h>
#include <ctype.h>
#include <Config/vjParseUtil.h>

/* a utility function that probably belongs elsewhere */
int readString (istream &in, char *buffer, int size, bool *quoted) {
    /* reads a string from in.  a string is either " delimited
     * or contains no whitespace.
     */
    /* 
     * post: returns the length of the string read (not counting \0)
     *       Quoted is set true if the string read was in quoted.
     */

    int i;
    int retval = 0;
    char c, vj;

    if (quoted)
	*quoted = false;
    for (;;) {
	if (!in.get(c))
	    break;
	if (isspace(c))
	    continue;
	if (c == '#') {
	    while (c != '\n')
		in.get(c);
	    continue;
	}
	if ((c == '/') && in.get(vj)) {
	    /* it might be one or the other type of comment */
	    if (vj == '/') {
		// single line comment.
		while (c != '\n')
		    if (!in.get(c))
			break;
		continue;
	    }
	    else if (vj == '*') {
		// multiline comment.
		while (true) {
		    if (!in.get(vj))
			break;
		    if (vj == '*') {
			in.get(vj);
			if (vj == '/')
			    break;
		    }
		}
		continue;
	    }
	    else 
		in.putback(vj);
	}
	break;
    }
    buffer[0] = c;
    
    if (buffer[0] == '{') {
	buffer[1] = '\0';
	retval = 1;
    }
    else if (buffer[0] == '"') {
	/* do a quoted string */
	if (quoted)
	    *quoted = true;
	for (i = 0; i < size-1; i++) {
	    in.get(buffer[i]);
	    if (buffer[i] == '"')
		break;
	}
	buffer[i] = '\0';
	retval = i;
    }
    else {
	for (i = 1; i < size-1; i++) {
	    in.get(buffer[i]);
	    if (buffer[i] == '}') {
		// wanna push back
		in.putback(buffer[i]);
		break;
	    }
	    if (isspace(buffer[i]))
		break;
	}
	buffer[i] = '\0';
	retval = i;
    }
    //cout << "read string: '" << buffer << "'" << endl;
    return retval;
}



VarType readType (istream &in) {
    char str[256];
    readString (in, str, 256);

    if (!strcasecmp (str, "int"))
	return T_INT;
    if (!strcasecmp (str, "integer"))
	return T_INT;
    if (!strcasecmp (str, "float"))
	return T_FLOAT;
    if (!strcasecmp (str, "bool"))
	return T_BOOL;
    if (!strcasecmp (str, "boolean"))
	return T_BOOL;
    if (!strcasecmp (str, "string"))
	return T_STRING;
    if (!strcasecmp (str, "distance"))
	return T_DISTANCE;
    if (!strcasecmp (str, "chunk"))
	return T_CHUNK;
    if (!strcasecmp (str, "embeddedchunk"))
	return T_EMBEDDEDCHUNK;
    
    return T_INVALID;
}



char *typeString (VarType t) {
    switch (t) {
    case T_INT:
	return "Int";
    case T_BOOL:
	return "Bool";
    case T_FLOAT:
	return "Float";
    case T_STRING:
	return "String";
    case T_CHUNK:
	return "Chunk";
    case T_EMBEDDEDCHUNK:
	return "EmbeddedChunk";
    default:
	return "Unrecognized_Type";
    }
}



char *unitString (CfgUnit t) {
  switch (t) {
  case U_Feet:
    return "Feet";
  case U_Inches:
    return "Inches";
  case U_Meters:
    return "Meters";
  case U_Centimeters:
    return "Centimeters";
  default:
    return "Invalid Unit Type";
  }
}

float toFeet (float val, CfgUnit unit) {
  switch (unit) {
  case U_Feet:
    return val;
  case U_Inches:
    return (val/12);
  case U_Meters:
    return (val*3.28084);
  case U_Centimeters:
    return (val*0.0328084);
  default:
    return val;
  }
}


bool vjstrcasecmp (const std::string& a, const std::string& b) {
  if (a.size() != b.size())
    return true;
  for (int i = 0; i < a.size(); i++)
    if (toupper(a[i]) != toupper(b[i]))
      return true;
  return false;
}
