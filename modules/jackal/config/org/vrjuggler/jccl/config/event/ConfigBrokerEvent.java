/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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
package org.vrjuggler.jccl.config.event;

import java.util.EventObject;
import org.vrjuggler.jccl.config.DataSource;

/**
 * This provides detailed information to ConfigBroker observers as to how a
 * ConfigBroker changed.
 */
public class ConfigBrokerEvent
   extends EventObject
{
   /**
    * Constructs a new ConfigBrokerEvent fired from the given source object
    * relating to the given resource.
    *
    * @param source     the source of the event
    * @param resource   the resource that changed
    */
   public ConfigBrokerEvent(Object source, String resource, DataSource dataSource)
   {
      super(source);
      this.resource = resource;
      this.dataSource = dataSource;
   }

   /**
    * Gets the resource associated with this event.
    */
   public String getResource()
   {
      return resource;
   }

   /**
    * Gets the data source associated with this event.
    */
   public DataSource getDataSource()
   {
      return dataSource;
   }

   /**
    * The resources associated with this event.
    */
   private String resource;

   /**
    * The DataSource associated with this event.
    */
   private DataSource dataSource;
}
