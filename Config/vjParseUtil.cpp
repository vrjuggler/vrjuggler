/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


#include <vjConfig.h>
#include <ctype.h>
#include <Config/vjParseUtil.h>
#include <Kernel/vjDebug.h>
#include <Config/vjConfigTokens.h>

/* a utility function that probably belongs elsewhere */
bool readString (std::istream &in, char *buffer, int size, bool *quoted) {
    /* reads a string from in.  a string is either " delimited
     * or contains no whitespace.
     *
     * returns true if a string is correctly read.  This could be a 0-length
     * quoted string...
     *
     * Quoted is set true if the string read was in quotes.
     */

    size = size-5; // this is just cheezy reassurance
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
            i--; // so that we're writing to the last element of buffer
            while (in.get(buffer[i]) && (buffer[i] != '"'))
                ;
            buffer[i] = '\0';
            vjDEBUG (vjDBG_ERROR,0) << clrOutNORM(clrRED, "ERROR:") << " Truncated string in config file: '"
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



VarType readType (std::istream &in) {
    char str[256];

    if (!readString (in, str, 256))
        return T_INVALID;

    if (!strcasecmp (str, int_TOKEN))
        return T_INT;
    if (!strcasecmp (str, integer_TOKEN))
        return T_INT;
    if (!strcasecmp (str, float_TOKEN))
        return T_FLOAT;
    if (!strcasecmp (str, bool_TOKEN))
        return T_BOOL;
    if (!strcasecmp (str, boolean_TOKEN))
        return T_BOOL;
    if (!strcasecmp (str, string_TOKEN))
        return T_STRING;
    if (!strcasecmp (str, distance_TOKEN))
        return T_DISTANCE;
    if (!strcasecmp (str, chunk_TOKEN))
        return T_CHUNK;
    if (!strcasecmp (str, embeddedchunk_TOKEN))
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
    for (unsigned int i = 0; i < a.size(); i++)
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


/** filename handling routines **/

//: Returns a copy of s with all environment variable names replaced
//+ with their values.
std::string replaceEnvVars (const std::string& s) {
    unsigned int i, j;
    int lastpos = 0;
    std::string result = "";
    for (i = 0; i < s.length(); i++) {
        if (s[i] == '$') {
            //process an env var
            result += std::string(s, lastpos, i - lastpos);
            i++; // skip $
            if (s[i] == '{') {
                for (j = i; j < s.length(); j++)
                    if (s[j] == '}')
                        break;
                std::string var(s,i+1,j-i-1);
                //cout << "searching for env var '" << var.c_str() << '\'' << endl;
                std::string res = getenv (var.c_str());
                result += res;
                i = j+1;
                lastpos = i;
            }
            else {
                for (j = i; j < s.length(); j++)
                    if (s[j] == '/' || s[j] == '\\')
                        break;
                std::string var(s,i,j-i);
                //cout << "searching for env var '" << var.c_str() << '\'' << endl;
                std::string res = getenv (var.c_str());
                result += res;
                i = j;
                lastpos = i;
            }
        }
    }
    result += std::string(s, lastpos, s.length() - lastpos);
    return result;
}



//: is n an absolute path name?
bool isAbsolutePathName (const std::string& n) {
#ifdef WIN32
    return ((n.length() > 0) && (n[0] == '\\'))
        || ((n.length() > 2) && (n[1] == ':') && (n[2] == '\\'));
#else
    return (n.length() > 0) && (n[0] == '/');
#endif
}



std::string demangleFileName (const std::string& n, std::string parentfile) {

    std::string fname = replaceEnvVars (n);

    if (!isAbsolutePathName(fname)) {
        // it's a relative pathname... so we have to add in the path part
        // of parentfile...
//         cout << "demangling relative pathname '" << fname.c_str() << "' with parent dir '"
//              << parentfile.c_str() << "'\n" << endl;
        int lastslash = 0;
        for (unsigned int i = 0; i < parentfile.length(); i++) {
            if (parentfile[i] == '/')
                lastslash = i;
#ifdef WIN32
            if (parentfile[i] == '\\')
                lastslash = i;
#endif
      }
        if (lastslash) {
            std::string s(parentfile, 0, lastslash+1);
            fname = s + n;
        }
    }

    return fname;
}
