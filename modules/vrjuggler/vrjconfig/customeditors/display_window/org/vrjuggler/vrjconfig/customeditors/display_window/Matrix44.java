/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

   public void invert(Matrix44 _m)
   {
      double[][] src_data = _m.data;

      int n = 4;
      int i, j;
      int[] r = new int[n], c = new int[n], row = new int[n], col = new int[n];
      double[][] m = new double[n][n * 2];
      double pivot, max_m, tmp_m, fac;

      // Set working matrix.
      for ( i = 0; i < n; ++i )
      {
         for ( j = 0; j < n; ++j )
         {
            m[i][j] = src_data[i][j];
            m[i][j + n] = (i == j) ? 1.0 : 0.0;
         }
      }

      // Beginning of loop.
      for ( int k = 0; k < n; ++k )
      {
         // Choosing the pivot.
         for ( i = 0, max_m = 0; i < n; ++i )
         {
            if ( row[i] != 0.0 )
            {
               continue;
            }

            for ( j = 0; j < n; ++j )
            {
               if ( col[j] != 0.0 )
               {
                  continue;
               }

               tmp_m = Math.abs(m[i][j]);
               if ( tmp_m > max_m )
               {
                  max_m = tmp_m;
                  r[k] = i;
                  c[k] = j;
               }
            }
         }
         row[r[k]] = col[c[k]] = 1;
         pivot = m[r[k]][c[k]];

         // Normalization.
         for ( j = 0; j < 2 * n; ++j )
         {
            if ( j == c[k] )
            {
               m[r[k]][j] = 1.0;
            }
            else
            {
               m[r[k]][j] /= pivot;
            }
         }

         // Reduction.
         for ( i = 0; i < n; ++i )
         {
            if ( i == r[k] )
            {
               continue;
            }

            for ( j = 0, fac = m[i][c[k]]; j < 2 * n; ++j )
            {
               if ( j == c[k] )
               {
                  m[i][j] = 0.0;
               }
               else
               {
                  m[i][j] -= fac * m[r[k]][j];
               }
            }
         }
      }

      // Assign inverse to a matrix.
      for ( i = 0; i < n; ++i )
      {
         for ( j = 0; j < n; ++j )
         {
            row[i] = (c[j] == i) ? r[j] : row[i];
         }
      }

      for ( i = 0; i < n; ++i )
      {
         for ( j = 0; j < n; ++j )
         {
            data[i][j] = m[row[i]][j + n];
         }
      }
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
