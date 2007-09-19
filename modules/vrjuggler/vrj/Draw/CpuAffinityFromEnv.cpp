/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#include <vrj/vrjConfig.h>

#include <algorithm>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <vpr/System.h>
#include <vpr/Util/Debug.h>

#include <vrj/Util/Debug.h>
#include <vrj/Draw/CpuAffinityFromEnv.h>


namespace vrj
{

CpuAffinityFromEnv::CpuAffinityFromEnv(const std::string& envVar)
{
   std::string env_var_value;
   vpr::System::getenv(envVar, env_var_value);
   boost::trim_if(env_var_value, boost::is_any_of(" \"'"));

   if ( ! env_var_value.empty() )
   {
      std::vector<std::string> split_vec;
      boost::split(split_vec, env_var_value, boost::is_any_of(" "));

      mCpuList.resize(split_vec.size());

      try
      {
         std::transform(
            split_vec.begin(), split_vec.end(), mCpuList.begin(),
            boost::bind(boost::lexical_cast<int, std::string>, _1)
         );
      }
      catch (boost::bad_lexical_cast& ex)
      {
         vprDEBUG(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Malformed CPU identifier in '" << env_var_value << "':\n"
            << ex.what() << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(vrjDBG_DRAW_MGR, vprDBG_WARNING_LVL)
            << "Draw thread CPU affinity will be disabled.\n"
            << vprDEBUG_FLUSH;
         mCpuList.clear();
      }
   }
}

int CpuAffinityFromEnv::operator()(const unsigned int threadNum)
{
   int cpu(-1);

   if ( ! mCpuList.empty() )
   {
      // Round-robin CPU selection from the list of CPU identifiers read from
      // the environment variable.
      cpu = mCpuList[threadNum % mCpuList.size()];
   }

   return cpu;
}

} // end of vrj namespace
