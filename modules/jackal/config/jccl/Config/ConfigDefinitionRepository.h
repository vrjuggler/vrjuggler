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

#ifndef _JCCL_CONFIG_DEFINITION_REPOSITORY_H_
#define _JCCL_CONFIG_DEFINITION_REPOSITORY_H_

#include <jccl/jcclConfig.h>
#include <string>
#include <map>
#include <jccl/Config/ConfigDefinitionPtr.h>

namespace jccl
{
   /** \class ConfigDefinitionRepository ConfigDefinitionRepository.h jccl/Config/ConfigDefinitionRepository.h
    *
    * Stores a set of configuration definitions that can be retrieved by name.
    * Only the newest version of a given definition is available.
    */
   class JCCL_CLASS_API ConfigDefinitionRepository
   {
   public:

      /** Constructor.  Creates an empty ConfigDefinitionRepository. */
      ConfigDefinitionRepository();

      /** Destructor.  Doesn't do anything. */
      ~ConfigDefinitionRepository();

      /**
       * Gets the config definition for the given token.
       * @returns NULL ptr if not found, else a shared ptr to the desc.
       */
      ConfigDefinitionPtr get(const std::string& token) const;

      /**
       * Adds the given configuration definition to this repository.
       */
      void add(ConfigDefinitionPtr def);

      /**
       * Removes the given configuration definition from this repository.
       */
      void remove(ConfigDefinitionPtr def);

   private:
      typedef std::map<std::string, ConfigDefinitionPtr> DefinitionMap;
      DefinitionMap mDefs;
   };
} // End of jccl namespace

#endif
