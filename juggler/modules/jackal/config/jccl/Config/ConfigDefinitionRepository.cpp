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

#include <jccl/jcclConfig.h>
#include <sys/types.h>

#include <vpr/Util/Assert.h>
#include <jccl/Util/Debug.h>
#include <jccl/Config/ConfigDefinition.h>
#include <jccl/Config/ConfigDefinitionRepository.h>

namespace jccl
{
   ConfigDefinitionRepository::ConfigDefinitionRepository()
   {}

   ConfigDefinitionRepository::~ConfigDefinitionRepository()
   {}

   ConfigDefinitionPtr ConfigDefinitionRepository::get(const std::string token,
                                                       unsigned int version)
   {
      // Fail if no version of the definition is in the repository
      DefinitionMap::const_iterator def_itr = mDefs.find(token);
      if (def_itr == mDefs.end())
      {
         return ConfigDefinitionPtr();
      }

      const VersionMap& version_map = def_itr->second;

      // Fail if the version of the definition is not in the repository
      VersionMap::const_iterator ver_itr = version_map.find(version);
      if (ver_itr == version_map.end())
      {
         return ConfigDefinitionPtr();
      }

      // Return the matching config definition
      return ver_itr->second;
   }

   void ConfigDefinitionRepository::add(ConfigDefinitionPtr def)
   {
      mDefs[def->getToken()][def->getVersion()] = def;
   }

   void ConfigDefinitionRepository::remove(ConfigDefinitionPtr def)
   {
      // Check if any version of the definition is in the repository
      DefinitionMap::iterator def_itr = mDefs.find(def->getToken());
      if (def_itr != mDefs.end())
      {
         VersionMap& version_map = def_itr->second;

         // Check if the specific version of the definition is in the repository
         VersionMap::const_iterator ver_itr = version_map.find(def->getVersion());
         if (ver_itr != version_map.end())
         {
            version_map.erase(def->getVersion());
         }
      }
   }
} // namespace jccl
