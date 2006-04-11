/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
#include <vpr/vpr.h>
#include <jccl/Config/ConfigTokens.h>


namespace jccl
{

/** Converts a char* to a jccl::VarType.
 *  @param str A non-NULL c string.
 *  @return The VarType named by str (VJ_T_INVALID if no match).
 */
VarType stringToVarType(const std::string& str);

/** Converts a VarType enum elem to a c string.
 *  @param t A VarType (see jccl/Config/VarValue.h)
 *  @return A non-NULL c string.
 */
std::string typeString(const VarType t);

/** Demangle a filename so that it can be passed to open().
 *  @param n A filename.
 *  @param parentfile The name of the file that n is being loaded
 *         relative to.
 *  @return A demangled version of the string n.  All environment variables
 *          are expanded.  If n is not an absolute path and parentfile
 *          is not "", the path part of parentfile will be prepended to
 *          the result.  Note that parentfile is always assumed to refer
 *          to a file, so if n is "included_file" and parentfile is
 *          "mydir/including_file", the result will be "mydir/included_file".
 */
std::string demangleFileName(const std::string& n, std::string parentfile);

/** Looks for a file in a path, and opens it if found.
 *
 *  @param file_name     The name of a file to search for.
 *  @param env_name      The environment variable containing a path.  The
 *                       path is a set of directories separated by a delimiter
 *                       character (';' on windows, ':' on real operating
 *                       systems).
 *  @param absolute_file Storage for the absolute path to the file if it was
 *                       found in the path.
 *
 *  @return vpr::ReturnStatus::Succeed is returned if a file is found in the
 *          path; vpr::ReturnStatus::Fail otherwise.
 */
JCCL_API(vpr::ReturnStatus) findFileUsingPathVar(const std::string& file_name,
                                                 const std::string& env_name,
                                                 std::string& absolute_file);

/** Does the given path have a separator */
bool hasSeparator(const std::string &path);

/** Get the part of the path after the separator */
std::string getRemainder(const std::string &path);

/** Get the part of the path before the separator */
std::string getFirstNameComponent(const std::string& path);

} // End of jccl namespace

#endif
