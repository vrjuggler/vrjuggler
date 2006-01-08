/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
import javax.swing.event.EventListenerList;
import org.vrjuggler.jccl.config.event.*;

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

      // Notify listeners
      fireDefinitionAdded(def);
   }

   /**
    * Removes the given definition and all its versions from this repository.
    *
    * @param def the definition to remove
    */
   public synchronized void remove(ConfigDefinition def)
      throws DefinitionLookupException
   {
      if ( mDefs.containsKey(def.getToken()) )
      {
         mDefs.remove(def.getToken());
         fireDefinitionRemoved(def);
      }
      else
      {
         throw new DefinitionLookupException("No definition with token '" +
                                             def.getToken() + "' exists");
      }
   }

   /**
    * Removes the named definition and all its versions from this repository.
    *
    * @param token the identifier of the definition to remove
    */
   public synchronized void remove(String token)
      throws DefinitionLookupException
   {
      if ( mDefs.containsKey(token) )
      {
         ConfigDefinition def = (ConfigDefinition) mDefs.remove(token);
         fireDefinitionRemoved(def);
      }
      else
      {
         throw new DefinitionLookupException("No definition with token '" +
                                             token + "' exists");
      }
   }

   /**
    * Re-inserts the named definition into the repository under the new name.
    *
    * @param oldToken the old identifier under which the definition is
    *                 registered
    * @param newToken the new identifier under which the definition will be
    *                 registered
    */
/*
   public synchronized void rename(String oldToken, String newToken)
      throws DefinitionLookupException
   {
      if ( mDefs.containsKey(oldToken) )
      {
         Map def_versions = (Map) mDefs.remove(oldToken);
         mDefs.put(newToken, def_versions);
      }
      else
      {
         throw new DefinitionLookupException("No definition with token '" +
                                             oldToken + "' exists");
      }
   }
*/

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

      // Get the maximal version number
      try
      {
         Integer latest_version = getNewestVersionNumber(token);

         Map version_map = (Map)mDefs.get(token);
         // Return the latest version
         return (ConfigDefinition)version_map.get(latest_version);
      }
      catch(DefinitionLookupException dle)
      {
         return null;
      }
   }

   /**
    * Gets the latest version number for the given token.
    *
    * @param token      the token of the definition in question.
    */
   public synchronized Integer getNewestVersionNumber(String token)
      throws DefinitionLookupException
   {
      // Get the maximal key
      Map version_map = (Map)mDefs.get(token);

      if(null == version_map)
      {
         throw new DefinitionLookupException("Failed to find definition " +
                                             "of type '" + token + "'");
      }

      return (Integer) Collections.max(version_map.keySet());
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
    * Gets a list of the latest version all the definitions in the repository.
    *
    * @return  a list of configuration definitions
    */
   public synchronized List getAllLatest()
   {
      List defs = new ArrayList();

      // Get the lastes version for each definition
      for (Iterator itr = mDefs.keySet().iterator(); itr.hasNext(); )
      {
         ConfigDefinition def = get((String)itr.next());
         defs.add(def);
      }

      return defs;
   }

   /**
    * Adds the given object as a listener for changes to this repository.
    */
   public void addConfigDefinitionRepositoryListener(ConfigDefinitionRepositoryListener listener)
   {
      mListeners.add(ConfigDefinitionRepositoryListener.class, listener);
   }

   /**
    * Removes the given object as a listener for changes to this repository.
    */
   public void removeConfigDefinitionRepositoryListener(ConfigDefinitionRepositoryListener listener)
   {
      mListeners.remove(ConfigDefinitionRepositoryListener.class, listener);
   }

   /**
    * Notifies listeners that the given configuration definition has been added
    * to the repository.
    */
   protected void fireDefinitionAdded(ConfigDefinition def)
   {
      ConfigDefinitionRepositoryEvent evt = null;
      Object[] listeners = mListeners.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionRepositoryListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionRepositoryEvent(this, def);
            }
            ((ConfigDefinitionRepositoryListener)listeners[i+1]).configDefinitionAdded(evt);
         }
      }
   }

   /**
    * Notifies listeners that the given configuration definition has been
    * removed from the repository.
    */
   protected void fireDefinitionRemoved(ConfigDefinition def)
   {
      ConfigDefinitionRepositoryEvent evt = null;
      Object[] listeners = mListeners.getListenerList();
      for (int i=listeners.length-2; i>=0; i-=2)
      {
         if (listeners[i] == ConfigDefinitionRepositoryListener.class)
         {
            if (evt == null)
            {
               evt = new ConfigDefinitionRepositoryEvent(this, def);
            }
            ((ConfigDefinitionRepositoryListener)listeners[i+1]).configDefinitionRemoved(evt);
         }
      }
   }

   public synchronized List getSubDefinitions(List tokens)
   {
      List result = new ArrayList();

      // We can start by adding the tokens for all listed items.
      result.addAll(tokens);

      for (Iterator itr = tokens.iterator(); itr.hasNext(); )
      {
         result.addAll(getSubDefinitions((String)itr.next()));
      }
      return result;
   }

   public synchronized List getSubDefinitions(String token)
   {
      List result = new ArrayList();
      ConfigDefinition def = get(token);
      
      // Test to make sure that the definition actually exists.
      if(null == def)
      {
         System.out.println("Could not find definition: " + token);
      }
      else
      {
         List sub_defs = def.getSubDefinitions();
         // For each sub definition we must check to make sure we do not already
         // have it.
         for (Iterator sub_itr = sub_defs.iterator(); sub_itr.hasNext(); )
         {
            ConfigDefinition sub_def = (ConfigDefinition)sub_itr.next();
            if(!result.contains(sub_def.getToken()))
            {
               result.add(sub_def.getToken());
            }
         }
      }
      return result;
   }

   /**
    * A map of definition names to a map of definition versions to the
    * definitions themselves.
    */
   private Map mDefs;

   /** All listeners interested in this repository. */
   private EventListenerList mListeners = new EventListenerList();
}
