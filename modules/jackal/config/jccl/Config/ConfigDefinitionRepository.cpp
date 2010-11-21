/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

   ConfigDefinitionPtr ConfigDefinitionRepository::get(const std::string& token)
      const
   {
      // Fail if no version of the definition is in the repository
      DefinitionMap::const_iterator def_itr = mDefs.find(token);
      if (def_itr == mDefs.end())
      {
         return ConfigDefinitionPtr();
      }
      else
      {
         return def_itr->second;
      }
   }

   void ConfigDefinitionRepository::add(ConfigDefinitionPtr def)
   {
      ConfigDefinitionPtr old_def = this->get(def->getToken());

      // If there is an existing definition in the repository with the same
      // ID as def, compare the versions of the two definitions.
      if ( old_def.get() != NULL )
      {
         // If the existing definition has an older version than the new
         // definition, add it to the repository.  Otherwise, discard the new
         // definition.
         if ( old_def->getVersion() < def->getVersion() )
         {
            mDefs[def->getToken()] = def;
         }
      }
      // If there is no existing definition matching the ID of def, add def
      // to the repository.
      else
      {
         mDefs[def->getToken()] = def;
      }
   }

   void ConfigDefinitionRepository::remove(ConfigDefinitionPtr def)
   {
      mDefs.erase(def->getToken());
   }
} // namespace jccl
