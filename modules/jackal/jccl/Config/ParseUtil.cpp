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


#include <jccl/jcclConfig.h>
#include <ctype.h>
#include <jccl/Config/ParseUtil.h>
#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigTokens.h>
#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>

namespace jccl {
   


VarType stringToVarType (const char* str) {
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

    return VJ_T_INVALID;
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
    int m = a.size();
    int n = b.size();
    n = (n < m)?n:m;
    if (_n >= 0)
        n = (n < _n)?n:_n;
//      int n = Math::Min (a.size(), b.size());
//      if (_n >= 0)
//          n = Math::Min (n, _n);

    for (int i = 0; i < n; i++)
        if (toupper(a[i]) != toupper(b[i]))
            return true;
    return false;
}



bool vjstrncmp (const std::string& a, const std::string& b, int _n) {
    int m = a.size();
    int n = b.size();
    n = (n < m)?n:m;
    if (_n >= 0)
        n = (n < _n)?n:_n;

//      int n = Math::Min (a.size(), b.size());
//      if (_n >= 0)
//          n = Math::Min (n, _n);

    for (int i = 0; i < n; i++)
        if (a[i] != b[i])
            return true;
    return false;
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

    std::string fname = vpr::replaceEnvVars (n);

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



const std::string findFileUsingPathVar (std::ifstream& in, const std::string& file_name, const std::string& env_name) {
    // based on patrick's code to support VJ_CFG_PATH
    // if we find file_name on any path in the variable env_var, we'll
    // open it.  someone calling this function should check in to see
    // if it's an open file afterwards.

    bool found = false;
    
    char* path_string_tmp;
    std::string path_string;
    std::string retval;
    
    // Read the value in the config path environment variable.  If it is
    // non-NULL, store the value in cfg_path since std::string's are
    // easier to use.
    if ( (path_string_tmp = getenv(env_name.c_str())) != NULL ) {
        path_string = path_string_tmp;
        
        vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "Falling back on " << env_name << ": " << path_string << "\n"
            << vprDEBUG_FLUSH;
    }

    // If the user set a value for $VJ_CFG_PATH, parse it, baby!
    if ( path_string.length() > 0 ) {
        std::string::size_type cur_pos = 0, old_pos = 0;
        std::string full_path;
        
// Define the separator character for the elements of $VJ_CFG_PATH.  On Win32,
// we use ";", and on everything else, we use ":".
#ifdef VPR_OS_Win32
        char elem_sep[] = ";";
        char ostype_var[] = "OSTYPE";
        char* ostype;

        // If we are in a Cygwin environment, use ":" as the element
        // separator.
        if ( (ostype = getenv(ostype_var)) != NULL ) {
            if ( strcmp(ostype, "cygwin") == 0 ) {
                elem_sep[0] = ':';
            }
        }
#else
        char elem_sep[] = ":";
#endif
        
        while ( ! found ) {
            // Clear the flags on in so that we can try opening a new file.
            in.clear();
            
            // Find the next occurrence of an element separator.
            cur_pos = path_string.find(elem_sep, old_pos);

            // If cur_pos is greater than the length of the path, there
            // are no more :'s in the path.
            if ( cur_pos > path_string.length() ) {
                // If old_pos is still less than the length of the path,
                // there is one more directory to be read, so set cur_pos
                // to the length of the path string so we can read it.
                // Once it's read, we'll be done.
                if ( old_pos < path_string.length() ) {
                    cur_pos = path_string.length();
                }
                // At this point, both old_pos and cur_pos point beyond
                // the end of the path string.
                else {
                    break;
                }
            }

            // Extract the current directory from the path and point
            // old_pos to be one character past the current position
            // (which points at a ':').
            full_path = path_string.substr(old_pos, cur_pos - old_pos);
            old_pos   = cur_pos + 1;

            // Append "/" + file_name to the current directory.
            full_path += "/";
            full_path += file_name;
            
            vprDEBUG(jcclDBG_CONFIG, vprDBG_STATE_LVL)
                << "ConfigChunkDB::load(): opening file " << full_path
                << "\n" << vprDEBUG_FLUSH;
            
            // Try to open the file name constructed above.
            in.open(full_path.c_str());
            
            if ( in ) {
                found = true;
                retval = full_path;
            }
        }
    }
    return retval;
}


}; // namespace jccl
