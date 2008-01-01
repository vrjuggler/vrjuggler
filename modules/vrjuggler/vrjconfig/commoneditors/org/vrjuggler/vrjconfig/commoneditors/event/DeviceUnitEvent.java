/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

package org.vrjuggler.vrjconfig.commoneditors.event;

import java.util.EventObject;


/**
 * Event object type associated with <code>DeviceUnitListener</code>.
 *
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.UnitConstants
 * @see DeviceUnitListener
 */
public class DeviceUnitEvent
   extends EventObject
{
   /**
    * Creates a device unit event for the given unit type and unit number.
    */
   public DeviceUnitEvent(Object source, Integer unitType,
                          Integer unitNumber)
   {
      super(source);
      this.unitType   = unitType;
      this.unitNumber = unitNumber;
   }

   /**
    * Creates a device unit event for the given unit type and unit number.
    */
   public DeviceUnitEvent(Object source, Integer unitType, int unitNumber)
   {
      this(source, unitType, new Integer(unitNumber));
   }

   /**
    * Returns the type of the unit associated with this event.
    *
    * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.UnitConstants
    */
   public Integer getUnitType()
   {
      return this.unitType;
   }

   /**
    * Returns the number (index) of the unit associated with this event.
    */
   public Integer getUnitNumber()
   {
      return this.unitNumber;
   }

   private Integer unitType   = null;
   private Integer unitNumber = null;
}
