#include <vjConfig.h>
#include <strings.h>
#include <ctype.h>
#include <Config/vjParseUtil.h>
#include <Kernel/vjDebug.h>

/* a utility function that probably belongs elsewhere */
bool readString (istream &in, char *buffer, int size, bool *quoted) {
    /* reads a string from in.  a string is either " delimited
     * or contains no whitespace.
     *
     * returns true if a string is correctly read.  This could be a 0-length
     * quoted string...
     */
    /* 
     *       Quoted is set true if the string read was in quoted.
     */

    int i;
    bool retval = false;
    char c, vj;
    buffer[0] = '\0';

    if (quoted)
	*quoted = false;

    // read whitespace, comments
    for (;;) {
	if (!in.get(c))
	    break;
	if (isspace(c))
	    continue;
	if (c == '#') {            // shell type comment
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
	break; // we read the / character and it wasn't part of a comment
    }
    buffer[0] = c;
    
    if ((buffer[0] == '{') || (buffer[0] == '}')) {
	buffer[1] = '\0';
	retval = true;
    }
    else if (buffer[0] == '"') {
	/* do a quoted string */
	if (quoted)
	    *quoted = true;
	for (i = 0; i < size; i++) {
	    in.get(buffer[i]);
	    if (buffer[i] == '"') {
		buffer[i] = '\0';
		break;
	    }
	}
	if (i == size) {
	    while (in.get(buffer[i]) && (buffer[i] != '"'))
		;
	    buffer[i] = '\0';
	    vjDEBUG (vjDBG_ALL,0) << "ERROR: Truncated string in config file: '"
				  << buffer << "'\n" << vjDEBUG_FLUSH;
	}
	retval = true;
    }
    else {
	// should add cleaner overflow handling like above...
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
	retval = true;
    }
    //cout << "readString: read string: '" << buffer << "'" << endl;
    if (!retval)
	buffer[0] = '\0'; // so it's safe to read it...
    return retval;
}



VarType readType (istream &in) {
    char str[256];

    if (!readString (in, str, 256))
	return T_INVALID;

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


bool vjstrncasecmp (const std::string& a, const std::string& b, int _n) {

    int n = VJ_MIN2 (a.size(), b.size());
    if (_n >= 0)
	n = VJ_MIN2 (n, _n);

    for (int i = 0; i < n; i++)
	if (toupper(a[i]) != toupper(b[i]))
	    return true;
    return false;
}



bool vjstrncmp (const std::string& a, const std::string& b, int _n) {

    int n = VJ_MIN2 (a.size(), b.size());
    if (_n >= 0)
	n = VJ_MIN2 (n, _n);

    for (int i = 0; i < n; i++)
	if (a[i] != b[i])
	    return true;
    return false;
}


