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
package org.vrjuggler.jccl.config;

import java.util.*;

/**
 * Stores a set of configuration definitions that can be retrieved by name and
 * version. This allows configuration definitions to be noncopyable and yet
 * still easily retrievable.
 */
public class ConfigDefinitionRepository
{
   /**
    * Creates a new repository of configuration definitions that is empty.
    */
   public ConfigDefinitionRepository()
   {
      mDefs = new TreeMap();
   }

   /**
    * Adds the given definition to this repository.
    *
    * @param def        the definition to add
    */
   public synchronized void add(ConfigDefinition def)
   {
      // Get the mapping of versions to definitions for this definition
      if (!mDefs.containsKey(def.getToken()))
      {
         Map version_map = new TreeMap();
         mDefs.put(def.getToken(), version_map);
      }
      Map def_version_map = (Map)mDefs.get(def.getToken());

      // Add the definition into the repository
      def_version_map.put(new Integer(def.getVersion()), def);
   }

   /**
    * Gets the latest version of the definition with the given token.
    *
    * @param token      the token of the definition to get
    */
   public synchronized ConfigDefinition get(String token)
   {
      // Fail if no version of the definition is in this repository
      if (!mDefs.containsKey(token))
      {
         return null;
      }

      // Get the maximal key
      Map version_map = (Map)mDefs.get(token);
      Integer latest_version = (Integer)Collections.max(version_map.keySet());

      // Return the latest version
      return (ConfigDefinition)version_map.get(latest_version);
   }

   /**
    * Gets the given version of the definition with the given token.
    *
    * @param token      the token of the definition to get
    * @param version    the version of the definition to get
    *
    * @return null if the definition is not in this repository
    */
   public synchronized ConfigDefinition get(String token, int version)
   {
      // Fail if no version of the definition is in this repository
      if (!mDefs.containsKey(token))
      {
         return null;
      }

      Map version_map = (Map)mDefs.get(token);

      // Fail if the version of the definition is not in this repository
      if (!version_map.containsKey(new Integer(version)))
      {
         return null;
      }

      return (ConfigDefinition)version_map.get(new Integer(version));
   }

   /**
    * A map of definition names to a map of definition versions to the
    * definitions themselves.
    */
   private Map mDefs;
}
