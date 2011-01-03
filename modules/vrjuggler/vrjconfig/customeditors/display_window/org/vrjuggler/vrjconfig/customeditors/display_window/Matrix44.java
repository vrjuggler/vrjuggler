/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

package org.vrjuggler.vrjconfig.customeditors.display_window;


class Matrix44
{
   public Matrix44()
   {
      identity();
   }

   public Matrix44(double xAngle, double yAngle, double zAngle)
   {
      this();
      setRot(xAngle, yAngle, zAngle);
   }

   public void identity()
   {
      for ( int i = 0; i < 4; ++i )
      {
         for ( int j = 0; j < 4; ++j )
         {
            data[i][j] = 0.0;
         }
      }

      data[0][0] = 1.0;
      data[1][1] = 1.0;
      data[2][2] = 1.0;
      data[3][3] = 1.0;
   }

   public void setRot(double xAngle, double yAngle, double zAngle)
   {
      double sin_x = Math.sin(xAngle);
      double sin_y = Math.sin(yAngle);
      double sin_z = Math.sin(zAngle);
      double cos_x = Math.cos(xAngle);
      double cos_y = Math.cos(yAngle);
      double cos_z = Math.cos(zAngle);

      data[0][0] = cos_y * cos_z;
      data[0][1] = -cos_y * sin_z;
      data[0][2] = sin_y;
      data[1][0] = sin_x * sin_y * cos_z + cos_x * sin_z;
      data[1][1] = -sin_x * sin_y * sin_z + cos_x * cos_z;
      data[1][2] = -sin_x * cos_y;
      data[2][0] = -cos_x * sin_y * cos_z + sin_x * sin_z;
      data[2][1] = cos_x * sin_y * sin_z + sin_x * cos_z;
      data[2][2] = cos_x * cos_y;
   }

   public void setTrans(double x, double y, double z)
   {
      data[3][0] = x;
      data[3][1] = y;
      data[3][2] = z;
   }

   public Point3D mult(Point3D pt)
   {
      double[] result = new double[]{0.0, 0.0, 0.0, 1.0};
      double[] input  = new double[]{pt.x, pt.y, pt.z, 1.0};

      for ( int i = 0; i < 4; ++i )
      {
         for ( int j = 0; j < 4; ++j )
         {
            result[i] += data[i][j] * input[j];
         }
      }

      return new Point3D(result[0], result[1], result[2]);
   }

   public String toString()
   {
      String output = "";
      for ( int i = 0; i < 4; ++i )
      {
         output += "[";
         for ( int j = 0; j < 4; ++j )
         {
            output += " " + data[i][j];
         }

         output += " ]\n";
      }

      return output;
   }

   public double[][] data = new double[4][4];
}
