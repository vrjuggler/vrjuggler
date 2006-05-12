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
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.vrjconfig.customeditors.intersense;

import java.util.EventObject;
import org.vrjuggler.jccl.config.ConfigElement;

/**
 * This provides detailed information to StationModel and ProxyTreeModel observers as to how 
 * the addition/removal of proxies.
 */
public class IntersenseModelEvent
   extends EventObject
{
   /**
    * Constructs a new IntersenseModelEvent fired from the given source object
    * relating to the given config element, definition token, and unit number.
    *
    * @param source     the source of the event
    * @param elm        the new config element pointing at this device
    * @param token      the definition token
    * @param unit       the number of the unit the proxy points at
    */
   public IntersenseModelEvent(Object source, ConfigElement elm, String token, int unit)
   {
      super(source);
      mElement = elm;
      mToken = token;
      mUnitNumber = unit;
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
   
   /**
    * Gets the config definition token associated with this context event.
    */
   public String getToken()
   {
      return mToken;
   }
   
   /**
    * Gets the unit number associated with this context event.
    */
   public int getUnitNumber()
   {
      return mUnitNumber;
   } 

   /** The element associated with this event. */
   private ConfigElement mElement;
   
   /** The token associated with this event. */
   private String mToken;

   /** The unit number associated with this event. */
   private int mUnitNumber;
}
