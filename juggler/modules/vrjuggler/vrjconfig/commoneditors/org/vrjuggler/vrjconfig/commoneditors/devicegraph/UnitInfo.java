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

package org.vrjuggler.vrjconfig.commoneditors.devicegraph;


public class UnitInfo
   implements UnitConstants
{
   public UnitInfo(Integer unitType, Integer unitNumber)
   {
      this.unitType   = unitType;
      this.unitNumber = unitNumber;
   }

   public Integer getUnitType()
   {
      return this.unitType;
   }

   public Integer getUnitNumber()
   {
      return this.unitNumber;
   }

   public void setUnitNumber(Integer unitNumber)
   {
      this.unitNumber = unitNumber;
   }

   public void decrementUnitNumber()
   {
      this.unitNumber = new Integer(this.unitNumber.intValue() - 1);
   }

   public void incrementUnitNumber()
   {
      this.unitNumber = new Integer(this.unitNumber.intValue() + 1);
   }

   public boolean equals(Object obj)
   {
      boolean result = false;

      if ( this == obj )
      {
         result = true;
      }
      else if ( obj instanceof UnitInfo )
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

   public String toString()
   {
      return "UnitInfo@" + Integer.toHexString(hashCode()) + ": [Type=" +
             UnitTypeHelpers.getUnitTypeName(this.unitType) + "(" +
             this.unitType + "),Number=" + this.unitNumber + "]";
   }

   private Integer unitType   = null;
   private Integer unitNumber = null;
}
