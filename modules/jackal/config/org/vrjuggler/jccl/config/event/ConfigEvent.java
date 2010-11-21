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

package org.vrjuggler.jccl.config.event;

import java.util.EventObject;
import org.vrjuggler.jccl.config.ConfigElement;

/**
 * This provides detailed information to ConfigBroker observers as to how the
 * configuration elements therein have changed.
 */
public class ConfigEvent
   extends EventObject
{
   /**
    * Constructs a new ConfigEvent fired from the given source object
    * relating to the given resource and configuration element.
    *
    * @param source     the source of the event
    * @param resource   the resource that changed
    * @param elt        the element affected by the change
    */
   public ConfigEvent(Object source, String resource, ConfigElement elt)
   {
      super(source);
      mResource = resource;
      mElement = elt;
   }

   /**
    * Gets the resource associated with this context event.
    */
   public String getResource()
   {
      return mResource;
   }

   /**
    * Gets the configuration element associated with this event if there was
    * one.
    *
    * @return  the affected element ; null if this event does not relate to a
    *          element
    */
   public ConfigElement getElement()
   {
      return mElement;
   }

   /** The resource associated with this event. */
   private String mResource;

   /** The element associated with this event. */
   private ConfigElement mElement;
}
