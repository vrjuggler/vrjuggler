/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * A configuration represents and contains all the information for a
 * configuration. Note that a configuration can contain other configurations.
 * This allows partial configurations to be reused in multiple high-level
 * configurations.
 */
public class Configuration
{
   /**
    * Creates a new configuration with the given name. The configuration is
    * initially empty.
    */
   public Configuration(String name)
   {
      mName = name;
      mIncludes = new ArrayList();
      mDefinitionPath = new ArrayList();
      mElements = new ArrayList();
   }

   /**
    * Gets the name for this configuration.
    */
   public String getName()
   {
      return mName;
   }

   /**
    * Sets the name for this configuration.
    */
   public synchronized void setName(String name)
   {
      mName = name;
   }

   /**
    * Gets a list of all the included configurations.
    */
   public List getIncludes()
   {
      return mIncludes;
   }

   /**
    * Adds a new include to the end of the includes list.
    *
    * @param filename            the filename of the configuration to add
    *
    * @throws IllegalArgumentException
    *             if the configuration is already included in this configuration
    */
   public synchronized void addInclude(String filename)
      throws IllegalArgumentException
   {
      if (mIncludes.contains(filename))
      {
         throw new IllegalArgumentException("Configuration is already included: " + filename);
      }
      mIncludes.add(filename);
   }

   /**
    * Removes the given configuration from this configuration's includes list.
    *
    * @param filename            the filename of the configuration to remove
    *
    * @throws IllegalArgumentException
    *             if the configuration is not included in this configuration
    */
   public synchronized void removeInclude(String filename)
      throws IllegalArgumentException
   {
      if (!mIncludes.contains(filename))
      {
         throw new IllegalArgumentException("Configuration not included: "+filename);
      }
      mIncludes.remove(filename);
   }

   /**
    * Gets a list of the directories in which configuration definitions are
    * searched for.
    */
   public List getDefinitionPath()
   {
      return mDefinitionPath;
   }

   /**
    * Adds a new directory to the end of the definition search path list.
    *
    * @param dir           the directory to add
    *
    * @throws IllegalArgumentException
    *             if the directory is already in the path
    */
   public synchronized void addDefinitionPath(String dir)
      throws IllegalArgumentException
   {
      if (mDefinitionPath.contains(dir))
      {
         throw new IllegalArgumentException("Directory is already in the path.");
      }
      mDefinitionPath.add(dir);
   }

   /**
    * Removes the given directory from this configuration's definition search
    * path list.
    *
    * @param dir           the directory to remove
    *
    * @throws IllegalArgumentException
    *             if the directory is not in the path
    */
   public synchronized void removeDefinitionPath(String dir)
      throws IllegalArgumentException
   {
      if (!mDefinitionPath.contains(dir))
      {
         throw new IllegalArgumentException("Directory is not in the path");
      }
      mDefinitionPath.remove(dir);
   }

   /**
    * Gets a list of all the the elements in this configuration. This method
    * does not recurse into the included configurations.
    */
   public List getElements()
   {
      return mElements;
   }

   /**
    * Adds the given configuration element to this configuration.
    *
    * @param elt The configuration element to add.
    *
    * @throws IllegalArgumentException
    *             if the element is already a part of this configuration
    */
   public synchronized void addElement(ConfigElement elt)
      throws IllegalArgumentException
   {
      if (mElements.contains(elt))
      {
         throw new IllegalArgumentException("Element '" + elt.getName() +
                                            "' (type " +
                                            elt.getDefinition().getToken() +
                                            ") is already a part of this " +
                                            "configuration.");
      }
      mElements.add(elt);
   }

   /**
    * Removes the given configuration element to this configuration.
    *
    * @param elt The configuration element to remove.
    *
    * @throws IllegalArgumentException
    *             if the element is not a part of this configuration
    */
   public synchronized void removeElement(ConfigElement elt)
      throws IllegalArgumentException
   {
      if (!mElements.contains(elt))
      {
         throw new IllegalArgumentException("Element '" + elt.getName() +
                                            "' (type " +
                                            elt.getDefinition().getToken() +
                                            ") is not a part of this " +
                                            "configuration.");
      }
      mElements.remove(elt);
   }

   /** This configuration's name. */
   private String mName;

   /** A list of all included configurations. */
   private List mIncludes;

   /** A list of the directories that make up the definition search path. */
   private List mDefinitionPath;

   /** A list of all the configuration elements in this configuration. */
   private List mElements;
}
