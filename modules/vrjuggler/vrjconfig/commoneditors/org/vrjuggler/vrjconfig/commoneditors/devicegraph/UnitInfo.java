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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;

import javax.swing.event.EventListenerList;

import org.vrjuggler.vrjconfig.commoneditors.event.UnitInfoEvent;
import org.vrjuggler.vrjconfig.commoneditors.event.UnitInfoListener;


/**
 * A holder for information about input sources (units) on some input device.
 * An input source in a given device is identified uniquely by its type and
 * its number.  This class makes the input source type immutable but allows
 * the number to be changed to account for changes in how the device is
 * configured.
 */
public class UnitInfo
   implements UnitConstants
{
   /**
    * Creates a new input source (unit) representation with the given type
    * and number.
    *
    * @param unitType   the type of the unit (as specified in
    *                   <code>UnitConstants</code>) associated with this object
    * @param unitNumber the number (index) of the unit associated with this
    *                   object
    *
    * @see UnitConstants
    */
   public UnitInfo(Integer unitType, int unitNumber)
   {
      this(unitType, new Integer(unitNumber));
   }

   /**
    * Creates a new input source (unit) representation with the given type
    * and number.
    *
    * @param unitType   the type of the unit (as specified in
    *                   <code>UnitConstants</code>) associated with this object
    * @param unitNumber the number (index) of the unit associated with this
    *                   object
    *
    * @see UnitConstants
    */
   public UnitInfo(Integer unitType, Integer unitNumber)
   {
      this.unitType   = unitType;
      this.unitNumber = unitNumber;
   }

   /**
    * Returns the type of this input source as specified by
    * <code>UnitConstants</code>.
    *
    * @see UnitConstants
    */
   public Integer getUnitType()
   {
      return this.unitType;
   }

   /**
    * Returns the unit number (index) of this input source.
    */
   public Integer getUnitNumber()
   {
      return this.unitNumber;
   }

   /**
    * Changes the unit number (index) of this input source to the given value.
    */
   public void setUnitNumber(int unitNumber)
   {
      setUnitNumber(new Integer(unitNumber));
   }

   /**
    * Changes the unit number (index) of this input source to the given value.
    */
   public void setUnitNumber(Integer unitNumber)
   {
      Integer old_value = this.unitNumber;
      this.unitNumber = unitNumber;

      fireUnitNumberChanged(old_value);
   }

   /**
    * Decrements this unit's number (index) by 1.
    */
   public void decrementUnitNumber()
   {
      decrementUnitNumber(1);
   }

   /**
    * Decrements this unit's number (index) by the given value.
    *
    * @param amount     the amount to decrement the unit number
    */
   public void decrementUnitNumber(int amount)
   {
      Integer old_value = this.unitNumber;
      this.unitNumber = new Integer(this.unitNumber.intValue() - amount);

      fireUnitNumberChanged(old_value);
   }

   /**
    * Increments this unit's number (index) by 1.
    */
   public void incrementUnitNumber()
   {
      incrementUnitNumber(1);
   }

   /**
    * Increments this unit's number (index) by the given value.
    *
    * @param amount     the amount to increment the unit number
    */
   public void incrementUnitNumber(int amount)
   {
      Integer old_value = this.unitNumber;
      this.unitNumber = new Integer(this.unitNumber.intValue() + amount);

      fireUnitNumberChanged(old_value);
   }

   /**
    * Equality overload to ensure that units can be compared easily and
    * correctly.
    */
   public boolean equals(Object obj)
   {
      boolean result = false;

      if ( this == obj )
      {
         result = true;
      }
      else if ( obj != null && obj.getClass().equals(this.getClass()) )
      {
         UnitInfo info_obj = (UnitInfo) obj;

         if ( this.unitType.equals(info_obj.unitType) &&
              this.unitNumber.equals(info_obj.unitNumber) )
         {
            result = true;
         }
      }

      return result;
   }

   public int hashCode()
   {
      return 0;
   }

   /**
    * Adds a listener for events fired by this object.
    */
   public void addUnitInfoListener(UnitInfoListener l)
   {
      mListeners.add(UnitInfoListener.class, l);
   }

   /**
    * Removes the given object as a listener for events fired by this object.
    */
   public void removeUnitInfoListener(UnitInfoListener l)
   {
      mListeners.remove(UnitInfoListener.class, l);
   }

   /**
    * Strig-ifies this object.
    */
   public String toString()
   {
      return "UnitInfo: [Type=" +
             UnitTypeHelpers.getUnitTypeName(this.unitType) + "(" +
             this.unitType + "),Number=" + this.unitNumber + "]";
   }

   /**
    * Fires a <code>UnitInfoEvent</code> for a change in this object's unit
    * number.
    */
   protected void fireUnitNumberChanged(Integer oldValue)
   {
      UnitInfoEvent event = null;
      Object[] listeners = mListeners.getListenerList();

      for ( int i = listeners.length - 2; i >= 0; i -= 2 )
      {
         if ( listeners[i] == UnitInfoListener.class )
         {
            if ( event == null )
            {
               event = new UnitInfoEvent(this, oldValue, this.unitNumber);
            }

            ((UnitInfoListener) listeners[i + 1]).unitNumberChanged(event);
         }
      }
   }

   private Integer unitType   = null;
   private Integer unitNumber = null;

   private EventListenerList mListeners = new EventListenerList();
}
