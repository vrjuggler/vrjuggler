/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
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

// Generated from Revision: 1.6 of RCSfile: free_types_cs.tmpl,v
using System;
using System.Runtime.InteropServices;

namespace gmtl
{

public sealed abstract class Math
{

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_Math_deg2Rad__double1(double p0);

   public static double deg2Rad(double p0)
   {
      double result;
      result = gmtl_Math_deg2Rad__double1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_Math_deg2Rad__float1(float p0);

   public static float deg2Rad(float p0)
   {
      float result;
      result = gmtl_Math_deg2Rad__float1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_Math_rad2Deg__double1(double p0);

   public static double rad2Deg(double p0)
   {
      double result;
      result = gmtl_Math_rad2Deg__double1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_Math_rad2Deg__float1(float p0);

   public static float rad2Deg(float p0)
   {
      float result;
      result = gmtl_Math_rad2Deg__float1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_Math_ceil__double1(double p0);

   public static double ceil(double p0)
   {
      double result;
      result = gmtl_Math_ceil__double1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_Math_ceil__float1(float p0);

   public static float ceil(float p0)
   {
      float result;
      result = gmtl_Math_ceil__float1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_Math_floor__double1(double p0);

   public static double floor(double p0)
   {
      double result;
      result = gmtl_Math_floor__double1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_Math_floor__float1(float p0);

   public static float floor(float p0)
   {
      float result;
      result = gmtl_Math_floor__float1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Math_seedRandom__unsigned_int1(uint p0);

   public static void seedRandom(uint p0)
   {
      gmtl_Math_seedRandom__unsigned_int1(p0);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_Math_unitRandom__0();

   public static float unitRandom()
   {
      float result;
      result = gmtl_Math_unitRandom__0();
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_Math_rangeRandom__float_float2(float p0,
	float p1);

   public static float rangeRandom(float p0, float p1)
   {
      float result;
      result = gmtl_Math_rangeRandom__float_float2(p0, p1);
      return result;
   }

}


} // namespace gmtl
