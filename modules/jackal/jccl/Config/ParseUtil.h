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



#ifndef _JCCL_PARSEUTIL_H_
#define _JCCL_PARSEUTIL_H_

#include <jccl/jcclConfig.h>
#include <jccl/Config/VarValue.h>

namespace jccl
{
   

/** Converts a char* to a jccl::VarType.
 *  @param str A non-NULL c string.
 *  @return The VarType named by str (VJ_T_INVALID if no match).
 */
VarType stringToVarType (const char* str);


/** Converts a VarType enum elem to a c string.
 *  @param t A VarType (see jccl/Config/VarValue.h)
 *  @return A non-NULL c string.
 */
char *typeString (VarType t);



/** Converts a CfgUnit to a c string. */
char *unitString (CfgUnit t);



/** Converts a value to feet.
 *  @param val A distance in some unit.
 *  @param unit The units that val is measured in.
 *  @return The distance val converted to feet.
 */
float toFeet (float val, CfgUnit unit);



/** A variant of stdlib's strcasecmp, but with std::string args.
 *  @param a An std::string
 *  @param b An std::string
 *  @return 0 if the strings are equal, else nonzero.  The comparison
 *            is not case sensitive.
 */
bool vjstrcasecmp (const std::string& a, const std::string& b);



/** A variant of stdlib's strncasecmp, but with std::string args.
 *  @param a An std::string
 *  @param b An std::string
 *  @param n Maximum number of characters to compare.  If n < 0 or is
 *           omitted, n is set to the length of the shorter string.
 *  @return 0 if the strings are equal, else nonzero.  The comparison
 *            is not case sensitive.
 */
bool vjstrncasecmp (const std::string& a, const std::string& b, int _n = -1);



/** A variant of stdlib's strncmp, but with std::string args.
 *  @param a An std::string
 *  @param b An std::string
 *  @param n Maximum number of characters to compare.  If n < 0 or is
 *           omitted, n is set to the length of the shorter string.
 *  @return 0 if the strings are equal, else nonzero.  The comparison
 *            is case sensitive.
 */
bool vjstrncmp (const std::string& a, const std::string& b, int _n = -1);



/** Mangles a filename so that it can be passed to open().
 *  @param n A filename.
 *  @param parentfile The name of the file that n is being loaded
 *         relative to.
 *  @return A "mangled" version of the string n.  All environment variables
 *          are expanded.  If n is not an absolute path and parentfile
 *          is not "", the path part of parentfile will be prepended to 
 *          the result.  Note that parentfile is always assumed to refer
 *          to a file, so if n is "included_file" and parentfile is
 *          "mydir/including_file", the result will be "mydir/included_file".
 */
std::string demangleFileName (const std::string& n, std::string parentfile);



/** Looks for a file in a path, and opens it if found.
 *  @param in An input stream.  If the file is found during the path 
 *            search, the in will be open for reading.
 *  @param file_name The name of a file to search for.
 *  @param env_name The environment variable containing a path.  The
 *                  path is a set of directories separated by a delimiter
 *                  character (';' on windows, ':' on real operating 
 *                  systems).
 *  @return The full name of the file if it was found, or empty string
 *          if it was not found.
 */
const std::string findFileUsingPathVar (std::ifstream& in, const std::string& file_name, const std::string& env_name);


};

#endif
