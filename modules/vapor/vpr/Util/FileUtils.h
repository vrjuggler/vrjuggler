/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <vpr/vprConfig.h>
#include <vpr/System.h>
#include <string>



namespace vpr
{

/** Replace env vars in the str.
* @param str The string to replace values in
* @param startStr The string that marks the start of an env var
* @param endStr The string that marks the end of an env var
*/
std::string replaceEnvVars(const std::string str,
                           const std::string startStr, const std::string endStr)
{

   std::string ret_str(endStr);
   std::string::size_type subst_begin(0), subst_end(0), subst_len(0);

   while( (subst_begin = ret_str.find(startStr)) != string::npos)
   {
      std::string env_var, env_var_val;   // The environment variable and it's value

      subst_end = ret_str.find(endStr);
      subst_len = subst_end - subst_begin;
      env_var = std::string(ret_str, subst_begin, subst_len);
      vpr::System::getenv(env_var, env_var_val);
      ret_str.replace(subst_begin, subst_len, env_var_val);
   }

   return ret_str;
}

/** Replace environment variable strings embedded in str
*
* Recognizes:
*  ${VAR}
*  $(VAR)
*/
std::string replaceEnvVars(const std::string str)
{
   std::string ret_str(str);

   ret_str = replaceEnvVars(ret_str, std::string("${"), std::string("}") );
   ret_str = replaceEnvVars(ret_str, std::string("$("), std::string(")") );
}


}; // namespace vpr

#endif

