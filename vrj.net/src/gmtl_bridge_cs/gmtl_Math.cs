#region License
// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$
#endregion License

// Generated from Revision: 1.7 of RCSfile: free_types_cs.tmpl,v
using System;
using System.Runtime.InteropServices;

namespace gmtl
{

public sealed class Math
{
   // Prevent instantiation.
   private Math()
   {
   }


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
