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

import java.io.IOException;
import java.util.List;

/**
 * The interface for a connection to a data source containing configuration
 * elements or definitions. The ConfigBroker uses these to abstract where the
 * data it is managing comes from.
 *
 * @see ConfigBroker
 */
public interface DataSource
{
   /**
    * Adds the given configuration element to this resource.
    *
    * @param chunk   the configuration element to add
    */
   public void add(ConfigChunk chunk);

   /**
    * Adds the given configuration definition to this resource.
    *
    * @param desc    the configuration definition to add
    */
   public void add(ChunkDesc desc);

   /**
    * Removes the given configuration element from this resource.
    *
    * @param chunk   the configuration element to remove
    */
   public void remove(ConfigChunk chunk);

   /**
    * Removes the given configuration definition from this resource.
    *
    * @param desc    the configuration definition to remove
    */
   public void remove(ChunkDesc desc);

   /**
    * Tests if the given configuration element is in this resource.
    */
   public boolean contains(ConfigChunk chunk);

   /**
    * Tests if the given configuration definition is in this resource.
    */
   public boolean contains(ChunkDesc desc);

   /**
    * Gets a list of all the elements in this resource.
    *
    * @return  a list of the ConfigChunks in this resource
    */
   public List getElements();

   /**
    * Gets a list of all the definitions in this resource.
    *
    * @return  a list of the ChunkDescs in this resource
    */
   public List getDefinitions();

   /**
    * Commits all pending changes to this resource. This should save the file,
    * send across new data to the running application, etc.
    */
   public void commit()
      throws IOException;

   /**
    * Rolls back all pending changes to this resource. This should reload the
    * file, retrieve data from the application, etc.
    */
   public void rollback()
      throws IOException;

   /**
    * Checks if this data source is read only.
    */
   public boolean isReadOnly();

   /**
    * Checks if this data source can accept configuration elements.
    */
   public boolean acceptsElements();

   /**
    * Checks if this data source can accept configuration definitions.
    */
   public boolean acceptsDefinitions();

   /**
    * Gets a list of the URLs for the resources included by this data source.
    *
    * @return  a list of URLs for the included resources
    */
   public List getIncludes();
}
