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

package org.vrjuggler.vrjconfig.customeditors.cave;

import java.math.BigDecimal;

class Point3D
{
   public Point3D()
   {
   }

   public Point3D(Point3D pt)
   {
      x = pt.x;
      y = pt.y;
      z = pt.z;
   }

   public Point3D(double[] pt)
   {
      x = pt[0];
      y = pt[1];
      z = pt[2];
   }

   public Point3D(double x, double y, double z)
   {
      this.x = x;
      this.y = y;
      this.z = z;
   }

   public void scale(double scaleFactor)
   {
      x *= scaleFactor;
      y *= scaleFactor;
      z *= scaleFactor;
   }

   public String toString()
   {
      return "(" + x + "," + y + "," + z + ")";
   }

   public boolean equals(Object o)
   {
      if ( this == o )
      {
         return true;
      }

      try
      {
         Point3D p = (Point3D) o;
         return x == p.x && y == p.y && z == p.z;
      }
      catch (ClassCastException e)
      {
         return false;
      }
   }

   public void round()
   {
      x = (new BigDecimal(x).setScale(5, BigDecimal.ROUND_HALF_UP)).floatValue();
      y = (new BigDecimal(y).setScale(5, BigDecimal.ROUND_HALF_UP)).floatValue();
      z = (new BigDecimal(z).setScale(5, BigDecimal.ROUND_HALF_UP)).floatValue();
   }

   public double x;
   public double y;
   public double z;
}
