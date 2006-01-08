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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.vrjconfig.customeditors.cave;

import org.vrjuggler.jccl.config.ConfigElement;


public class Corners
{
   private float[] ll_corner = new float[3];
   private float[] lr_corner = new float[3];
   private float[] ur_corner = new float[3];
   private float[] ul_corner = new float[3];
   
   public Corners() 
   {
      System.out.println("Corners Constructor()");
      ll_corner[0] = -5.0f;
      ll_corner[1] = -5.0f;
      ll_corner[2] =  0.0f;
      lr_corner[0] =  5.0f;
      lr_corner[1] = -5.0f;
      lr_corner[2] =  0.0f;
      ur_corner[0] =  5.0f;
      ur_corner[1] =  5.0f;
      ur_corner[2] =  0.0f;
      ul_corner[0] = -5.0f;
      ul_corner[1] =  5.0f;
      ul_corner[2] =  0.0f;
   }

   public Corners(float[] ll, float[] lr, float[] ur, float[] ul)
   {
      System.out.println("Corners Constructor(a,a,a,a)");
      for (int i = 0 ; i < 3 ; i++)
      {
         ll_corner[i] = ll[i];
         lr_corner[i] = lr[i];
         ur_corner[i] = ur[i];
         ul_corner[i] = ul[i];
      }
   }

   public float[] getLL()
   {
      return ll_corner;
   }
   public float[] getLR()
   {
      return lr_corner;
   }
   public float[] getUR()
   {
      return ur_corner;
   }
   public float[] getUL()
   {
      return ul_corner;
   }

   public Corners(ConfigElement elm)
   {
      System.out.println("Corners Constructor(elm)");
      ll_corner[0] = ((Number) elm.getProperty("lower_left_corner", 0)).floatValue();
      ll_corner[1] = ((Number) elm.getProperty("lower_left_corner", 1)).floatValue();
      ll_corner[2] = ((Number) elm.getProperty("lower_left_corner", 2)).floatValue();
      lr_corner[0] = ((Number) elm.getProperty("lower_right_corner", 0)).floatValue();
      lr_corner[1] = ((Number) elm.getProperty("lower_right_corner", 1)).floatValue();
      lr_corner[2] = ((Number) elm.getProperty("lower_right_corner", 2)).floatValue();
      ul_corner[0] = ((Number) elm.getProperty("upper_left_corner", 0)).floatValue();
      ul_corner[1] = ((Number) elm.getProperty("upper_left_corner", 1)).floatValue();
      ul_corner[2] = ((Number) elm.getProperty("upper_left_corner", 2)).floatValue();
      ur_corner[0] = ((Number) elm.getProperty("upper_right_corner", 0)).floatValue();
      ur_corner[1] = ((Number) elm.getProperty("upper_right_corner", 1)).floatValue();
      ur_corner[2] = ((Number) elm.getProperty("upper_right_corner", 2)).floatValue();
   }

   public int hashCode()
   {
      return 0;
      //XXX: Worried about floating point errors because HashMaps use both
      //obj1.hashCode() == obj2hashCode() & obj1.equals(obj2). Maybe Set
      //would be better.
      //return (ll_corner[0] + lr_corner[0] + ul_corner[0] + ur_corner[0]);
   }
   
   public boolean equals(Object obj)
   {
      if (obj instanceof Corners)
      {
         Corners c = (Corners)obj;
         if (ll_corner[0] == c.ll_corner[0] &&
             ll_corner[1] == c.ll_corner[1] &&
             ll_corner[2] == c.ll_corner[2] &&
             lr_corner[0] == c.lr_corner[0] &&
             lr_corner[1] == c.lr_corner[1] &&
             lr_corner[2] == c.lr_corner[2] &&
             ur_corner[0] == c.ur_corner[0] &&
             ur_corner[1] == c.ur_corner[1] &&
             ur_corner[2] == c.ur_corner[2] &&
             ul_corner[0] == c.ul_corner[0] &&
             ul_corner[1] == c.ul_corner[1] &&
             ul_corner[2] == c.ul_corner[2])
         {
            return true;
         }
      }
      return false;
   }

   public String toString()
   {
      StringBuffer buf = new StringBuffer();
      buf.append("{ll:(");
      
      buf.append(Float.toString(ll_corner[0]));
      buf.append(", ");
      buf.append(Float.toString(ll_corner[1]));
      buf.append(", ");
      buf.append(Float.toString(ll_corner[2]));
      buf.append("), lr:(");
      
      buf.append(Float.toString(lr_corner[0]));
      buf.append(", ");
      buf.append(Float.toString(lr_corner[1]));
      buf.append(", ");
      buf.append(Float.toString(lr_corner[2]));
      buf.append("), ur:(");

      buf.append(Float.toString(ur_corner[0]));
      buf.append(", ");
      buf.append(Float.toString(ur_corner[1]));
      buf.append(", ");
      buf.append(Float.toString(ur_corner[2]));
      buf.append("), ul:(");

      buf.append(Float.toString(ul_corner[0]));
      buf.append(", ");
      buf.append(Float.toString(ul_corner[1]));
      buf.append(", ");
      buf.append(Float.toString(ul_corner[2]));
      buf.append(")}");

      return buf.toString();
   }
}
