/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vpr/vprConfig.h>
#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>


namespace vpr
{

const std::string replaceEnvVars(const std::string& str,
                                 const std::string& startStr,
                                 const std::string& endStr)
{

   std::string ret_str(str);
   std::string::size_type subst_begin(0), subst_end(0), subst_len(0);

   while( (subst_begin = ret_str.find(startStr)) != std::string::npos)
   {
      std::string env_var, env_var_val;   // The environment variable and its value

      subst_end = ret_str.find(endStr);
      subst_len = subst_end - subst_begin;
      env_var = std::string(ret_str, subst_begin+startStr.size(), subst_len-startStr.size());
      vpr::System::getenv(env_var, env_var_val);
      ret_str.replace(subst_begin, subst_len+1, env_var_val);
   }

   return ret_str;
}

const std::string replaceEnvVars(const std::string& str)
{
   std::string ret_str(str);

   ret_str = replaceEnvVars(ret_str, std::string("${"), std::string("}") );
   ret_str = replaceEnvVars(ret_str, std::string("$("), std::string(")") );

   return ret_str;
}

} // End of vpr namespace
