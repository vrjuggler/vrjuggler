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

package org.vrjuggler.vrjconfig.commoneditors.event;

import java.util.EventObject;


/**
 * Event object type associated with <code>UnitInfoListener</code>.
 *
 * @see org.vrjuggler.vrjconfig.commoneditors.devicegraph.UnitInfo
 * @see UnitInfoListener
 */
public class UnitInfoEvent
   extends EventObject
{
   /**
    * Creates a device unit event for the given unit type and unit number.
    */
   public UnitInfoEvent(Object source, Integer oldUnitNumber,
                        Integer newUnitNumber)
   {
      super(source);
      this.oldUnitNumber = oldUnitNumber;
      this.newUnitNumber = newUnitNumber;
   }

   /**
    * Returns the previous number (index) of the unit associated with this
    * event.
    */
   public Integer getOldUnitNumber()
   {
      return this.oldUnitNumber;
   }

   /**
    * Returns the new number (index) of the unit associated with this event.
    */
   public Integer getNewUnitNumber()
   {
      return this.newUnitNumber;
   }

   private Integer oldUnitNumber = null;
   private Integer newUnitNumber = null;
}
