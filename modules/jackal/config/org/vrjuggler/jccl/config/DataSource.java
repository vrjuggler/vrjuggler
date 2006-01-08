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

import java.io.IOException;
import java.util.List;

/**
 * The interface for a connection to a data source containing configuration
 * elements. The ConfigBroker uses these to abstract where the data it is
 * managing comes from.
 *
 * @see ConfigBroker
 */
public interface DataSource
{
   /** Adds the given configuration element to this resource.
    *
    * @param elt     the configuration element to add
    */
   public void add(ConfigElement elt);

   /**
    * Removes the given configuration element from this resource.
    *
    * @param elt     the configuration element to remove
    */
   public void remove(ConfigElement elt);

   /**
    * Tests if the given configuration element is in this resource.
    */
   public boolean contains(ConfigElement elt);

   /**
    * Gets a list of all the elements in this resource.
    *
    * @return  a list of the ConfigElements in this resource
    */
   public List getElements();

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
    * Gets a list of the URLs for the resources included by this data source.
    *
    * @return  a list of URLs for the included resources
    */
   public List getIncludes();
}
