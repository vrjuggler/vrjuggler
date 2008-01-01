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

package org.vrjuggler.vrjconfig.customeditors.display_window;


class Plane
{
   public static final int LL_CORNER = 0;
   public static final int LR_CORNER = 1;
   public static final int UR_CORNER = 2;
   public static final int UL_CORNER = 3;

   public Plane(double width, double height)
   {
      mCorners[0] = new Point3D();
      mCorners[1] = new Point3D(width, 0.0, 0.0);
      mCorners[2] = new Point3D(width, height, 0.0);
      mCorners[3] = new Point3D(0.0, height, 0.0);
   }

   public void setOrientation(Matrix44 mat)
   {
      orientation = mat;
      for ( int i = 0; i < 4; ++i )
      {
         mCorners[i] = orientation.mult(mCorners[i]);
      }
   }

   public void setOrientation(double xAngle, double yAngle, double zAngle)
   {
      setOrientation(new Matrix44(Math.toRadians(xAngle),
                                  Math.toRadians(yAngle),
                                  Math.toRadians(zAngle)));
   }

   public Point3D[] getCorners()
   {
      Point3D[] rot_corners = new Point3D[mCorners.length];
      rot_corners[0] = getLowerLeftCorner();
      rot_corners[1] = getLowerRightCorner();
      rot_corners[2] = getUpperRightCorner();
      rot_corners[3] = getUpperLeftCorner();
      return rot_corners;
   }

   public void setCorner(Point3D value, int cornerID)
   {
      Point3D old_corner = mCorners[cornerID];
      mCorners[cornerID] = value;
//      System.out.println("old_corner: " + old_corner);
//      System.out.println("mCorners[" + cornerID + "]: " + mCorners[cornerID]);

      // Now transform all the other corners relative to the changed one.
      if ( ! mCorners[cornerID].equals(old_corner) )
      {
         double x_diff, y_diff, z_diff;
         x_diff = mCorners[cornerID].x - old_corner.x;
         y_diff = mCorners[cornerID].y - old_corner.y;
         z_diff = mCorners[cornerID].z - old_corner.z;

         int c = (cornerID + 1) % 4;
         for ( int i = 0; i < 3; ++i )
         {
            mCorners[c].x += x_diff;
            mCorners[c].y += y_diff;
            mCorners[c].z += z_diff;
//            System.out.println("mCorners[" + c + "]: " + mCorners[c]);
            c = (c + 1) % 4;
         }
      }
   }

   public Point3D getLowerLeftCorner()
   {
      return mCorners[0];
   }

   public Point3D getLowerRightCorner()
   {
      return mCorners[1];
   }

   public Point3D getUpperRightCorner()
   {
      return mCorners[2];
   }

   public Point3D getUpperLeftCorner()
   {
      return mCorners[3];
   }

   private Point3D[] mCorners = new Point3D[4];
   private Matrix44 orientation = new Matrix44();
}
