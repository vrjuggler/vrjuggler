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

// Generated from Revision: 1.7 of RCSfile: free_types_cs.tmpl,v
using System;
using System.Runtime.InteropServices;

namespace gmtl
{

public sealed class Gmtl
{
   // Prevent instantiation.
   private Gmtl()
   {
   }

   public const float GMTL_EPSILON = 9.999999974752427078783512115478515625e-7f;
   public const float GMTL_MAT_EQUAL_EPSILON = 1.000000047497451305389404296875e-3f;
   public const float GMTL_VEC_EQUAL_EPSILON = 9.99999974737875163555145263671875e-5f;

   public enum VectorIndex
   {
      Xelt = 0, Yelt = 1, Welt = 3, Zelt = 2
   }

   public enum PlaneSide
   {
      NEG_SIDE = 2, POS_SIDE = 1, ON_PLANE = 0
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_add__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2);

   public static gmtl.Quatd add(gmtl.Quatd p0, gmtl.Quatd p1, gmtl.Quatd p2)
   {
      gmtl.Quatd result;
      result = gmtl_add__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_add__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2);

   public static gmtl.Quatf add(gmtl.Quatf p0, gmtl.Quatf p1, gmtl.Quatf p2)
   {
      gmtl.Quatf result;
      result = gmtl_add__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_add__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p2);

   public static gmtl.Matrix44f add(gmtl.Matrix44f p0, gmtl.Matrix44f p1, gmtl.Matrix44f p2)
   {
      gmtl.Matrix44f result;
      result = gmtl_add__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_add__gmtl_Matrix44d_gmtl_Matrix44d_gmtl_Matrix44d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p2);

   public static gmtl.Matrix44d add(gmtl.Matrix44d p0, gmtl.Matrix44d p1, gmtl.Matrix44d p2)
   {
      gmtl.Matrix44d result;
      result = gmtl_add__gmtl_Matrix44d_gmtl_Matrix44d_gmtl_Matrix44d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_add__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p2);

   public static gmtl.Matrix33f add(gmtl.Matrix33f p0, gmtl.Matrix33f p1, gmtl.Matrix33f p2)
   {
      gmtl.Matrix33f result;
      result = gmtl_add__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_add__gmtl_Matrix33d_gmtl_Matrix33d_gmtl_Matrix33d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p2);

   public static gmtl.Matrix33d add(gmtl.Matrix33d p0, gmtl.Matrix33d p1, gmtl.Matrix33d p2)
   {
      gmtl.Matrix33d result;
      result = gmtl_add__gmtl_Matrix33d_gmtl_Matrix33d_gmtl_Matrix33d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3iMarshaler))]
   private extern static gmtl.Point3i gmtl_center__gmtl_Trii1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TriiMarshaler))] gmtl.Trii p0);

   public static gmtl.Point3i center(gmtl.Trii p0)
   {
      gmtl.Point3i result;
      result = gmtl_center__gmtl_Trii1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3dMarshaler))]
   private extern static gmtl.Point3d gmtl_center__gmtl_Trid1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TridMarshaler))] gmtl.Trid p0);

   public static gmtl.Point3d center(gmtl.Trid p0)
   {
      gmtl.Point3d result;
      result = gmtl_center__gmtl_Trid1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3fMarshaler))]
   private extern static gmtl.Point3f gmtl_center__gmtl_Trif1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TrifMarshaler))] gmtl.Trif p0);

   public static gmtl.Point3f center(gmtl.Trif p0)
   {
      gmtl.Point3f result;
      result = gmtl_center__gmtl_Trif1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_conj__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd conj(gmtl.Quatd p0)
   {
      gmtl.Quatd result;
      result = gmtl_conj__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_conj__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf conj(gmtl.Quatf p0)
   {
      gmtl.Quatf result;
      result = gmtl_conj__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_cross__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Vec3f cross(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2)
   {
      gmtl.Vec3f result;
      result = gmtl_cross__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_cross__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.Vec3d cross(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2)
   {
      gmtl.Vec3d result;
      result = gmtl_cross__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))]
   private extern static gmtl.Vec3i gmtl_cross__gmtl_Vec3i_gmtl_Vec3i_gmtl_Vec3i3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))] gmtl.Vec3i p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))] gmtl.Vec3i p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))] gmtl.Vec3i p2);

   public static gmtl.Vec3i cross(gmtl.Vec3i p0, gmtl.Vec3i p1, gmtl.Vec3i p2)
   {
      gmtl.Vec3i result;
      result = gmtl_cross__gmtl_Vec3i_gmtl_Vec3i_gmtl_Vec3i3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_distance__gmtl_LineSegd_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static double distance(gmtl.LineSegd p0, gmtl.Point3d p1)
   {
      double result;
      result = gmtl_distance__gmtl_LineSegd_gmtl_Point3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_distance__gmtl_LineSegf_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static float distance(gmtl.LineSegf p0, gmtl.Point3f p1)
   {
      float result;
      result = gmtl_distance__gmtl_LineSegf_gmtl_Point3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_distance__gmtl_Planef_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static float distance(gmtl.Planef p0, gmtl.Point3f p1)
   {
      float result;
      result = gmtl_distance__gmtl_Planef_gmtl_Point3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_distance__gmtl_Planed_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static double distance(gmtl.Planed p0, gmtl.Point3d p1)
   {
      double result;
      result = gmtl_distance__gmtl_Planed_gmtl_Point3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_distanceSquared__gmtl_LineSegf_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static float distanceSquared(gmtl.LineSegf p0, gmtl.Point3f p1)
   {
      float result;
      result = gmtl_distanceSquared__gmtl_LineSegf_gmtl_Point3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_distanceSquared__gmtl_LineSegd_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static double distanceSquared(gmtl.LineSegd p0, gmtl.Point3d p1)
   {
      double result;
      result = gmtl_distanceSquared__gmtl_LineSegd_gmtl_Point3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_div__gmtl_Quatd_gmtl_Quatd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	double p2);

   public static gmtl.Quatd div(gmtl.Quatd p0, gmtl.Quatd p1, double p2)
   {
      gmtl.Quatd result;
      result = gmtl_div__gmtl_Quatd_gmtl_Quatd_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_div__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	gmtl.Quatd p2);

   public static gmtl.Quatd div(gmtl.Quatd p0, gmtl.Quatd p1, gmtl.Quatd p2)
   {
      gmtl.Quatd result;
      result = gmtl_div__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_div__gmtl_Quatf_gmtl_Quatf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	float p2);

   public static gmtl.Quatf div(gmtl.Quatf p0, gmtl.Quatf p1, float p2)
   {
      gmtl.Quatf result;
      result = gmtl_div__gmtl_Quatf_gmtl_Quatf_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_div__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	gmtl.Quatf p2);

   public static gmtl.Quatf div(gmtl.Quatf p0, gmtl.Quatf p1, gmtl.Quatf p2)
   {
      gmtl.Quatf result;
      result = gmtl_div__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_dot__gmtl_Vec3f_gmtl_Vec3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public static float dot(gmtl.Vec3f p0, gmtl.Vec3f p1)
   {
      float result;
      result = gmtl_dot__gmtl_Vec3f_gmtl_Vec3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_dot__gmtl_Vec3d_gmtl_Vec3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1);

   public static double dot(gmtl.Vec3d p0, gmtl.Vec3d p1)
   {
      double result;
      result = gmtl_dot__gmtl_Vec3d_gmtl_Vec3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_dot__gmtl_Vec4d_gmtl_Vec4d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p1);

   public static double dot(gmtl.Vec4d p0, gmtl.Vec4d p1)
   {
      double result;
      result = gmtl_dot__gmtl_Vec4d_gmtl_Vec4d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_dot__gmtl_Vec4f_gmtl_Vec4f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p1);

   public static float dot(gmtl.Vec4f p0, gmtl.Vec4f p1)
   {
      float result;
      result = gmtl_dot__gmtl_Vec4f_gmtl_Vec4f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_dot__gmtl_Quatd_gmtl_Quatd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1);

   public static double dot(gmtl.Quatd p0, gmtl.Quatd p1)
   {
      double result;
      result = gmtl_dot__gmtl_Quatd_gmtl_Quatd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_dot__gmtl_Quatf_gmtl_Quatf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1);

   public static float dot(gmtl.Quatf p0, gmtl.Quatf p1)
   {
      float result;
      result = gmtl_dot__gmtl_Quatf_gmtl_Quatf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_exp__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf exp(gmtl.Quatf p0)
   {
      gmtl.Quatf result;
      result = gmtl_exp__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_exp__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd exp(gmtl.Quatd p0)
   {
      gmtl.Quatd result;
      result = gmtl_exp__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_findNearestPt__gmtl_Planed_gmtl_Point3d_gmtl_Point3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p2);

   public static double findNearestPt(gmtl.Planed p0, gmtl.Point3d p1, gmtl.Point3d p2)
   {
      double result;
      result = gmtl_findNearestPt__gmtl_Planed_gmtl_Point3d_gmtl_Point3d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_findNearestPt__gmtl_Planef_gmtl_Point3f_gmtl_Point3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p2);

   public static float findNearestPt(gmtl.Planef p0, gmtl.Point3f p1, gmtl.Point3f p2)
   {
      float result;
      result = gmtl_findNearestPt__gmtl_Planef_gmtl_Point3f_gmtl_Point3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3fMarshaler))]
   private extern static gmtl.Point3f gmtl_findNearestPt__gmtl_LineSegf_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static gmtl.Point3f findNearestPt(gmtl.LineSegf p0, gmtl.Point3f p1)
   {
      gmtl.Point3f result;
      result = gmtl_findNearestPt__gmtl_LineSegf_gmtl_Point3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3dMarshaler))]
   private extern static gmtl.Point3d gmtl_findNearestPt__gmtl_LineSegd_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static gmtl.Point3d findNearestPt(gmtl.LineSegd p0, gmtl.Point3d p1)
   {
      gmtl.Point3d result;
      result = gmtl_findNearestPt__gmtl_LineSegd_gmtl_Point3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_identity__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.Matrix44d identity(gmtl.Matrix44d p0)
   {
      gmtl.Matrix44d result;
      result = gmtl_identity__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_identity__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f identity(gmtl.Matrix44f p0)
   {
      gmtl.Matrix44f result;
      result = gmtl_identity__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_identity__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public static gmtl.Matrix33d identity(gmtl.Matrix33d p0)
   {
      gmtl.Matrix33d result;
      result = gmtl_identity__gmtl_Matrix33d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_identity__gmtl_Matrix33f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f identity(gmtl.Matrix33f p0)
   {
      gmtl.Matrix33f result;
      result = gmtl_identity__gmtl_Matrix33f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_invert__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd invert(gmtl.Quatd p0)
   {
      gmtl.Quatd result;
      result = gmtl_invert__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_invert__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf invert(gmtl.Quatf p0)
   {
      gmtl.Quatf result;
      result = gmtl_invert__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invert__gmtl_Matrix44f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invert(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_invert__gmtl_Matrix44f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_invert__gmtl_Matrix44d_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Matrix44d invert(gmtl.Matrix44d p0, gmtl.Matrix44d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_invert__gmtl_Matrix44d_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invert__gmtl_Matrix33f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invert(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_invert__gmtl_Matrix33f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_invert__gmtl_Matrix33d_gmtl_Matrix33d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1);

   public static gmtl.Matrix33d invert(gmtl.Matrix33d p0, gmtl.Matrix33d p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_invert__gmtl_Matrix33d_gmtl_Matrix33d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invert__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f invert(gmtl.Matrix44f p0)
   {
      gmtl.Matrix44f result;
      result = gmtl_invert__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_invert__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.Matrix44d invert(gmtl.Matrix44d p0)
   {
      gmtl.Matrix44d result;
      result = gmtl_invert__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invert__gmtl_Matrix33f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f invert(gmtl.Matrix33f p0)
   {
      gmtl.Matrix33f result;
      result = gmtl_invert__gmtl_Matrix33f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_invert__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public static gmtl.Matrix33d invert(gmtl.Matrix33d p0)
   {
      gmtl.Matrix33d result;
      result = gmtl_invert__gmtl_Matrix33d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_invertAffine__gmtl_Matrix33d_gmtl_Matrix33d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1);

   public static gmtl.Matrix33d invertAffine(gmtl.Matrix33d p0, gmtl.Matrix33d p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_invertAffine__gmtl_Matrix33d_gmtl_Matrix33d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invertAffine__gmtl_Matrix33f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invertAffine(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_invertAffine__gmtl_Matrix33f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invertAffine__gmtl_Matrix44f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invertAffine(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_invertAffine__gmtl_Matrix44f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_invertAffine__gmtl_Matrix44d_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Matrix44d invertAffine(gmtl.Matrix44d p0, gmtl.Matrix44d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_invertAffine__gmtl_Matrix44d_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invertFull__gmtl_Matrix44f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invertFull(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_invertFull__gmtl_Matrix44f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_invertFull__gmtl_Matrix33d_gmtl_Matrix33d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1);

   public static gmtl.Matrix33d invertFull(gmtl.Matrix33d p0, gmtl.Matrix33d p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_invertFull__gmtl_Matrix33d_gmtl_Matrix33d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invertFull__gmtl_Matrix33f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invertFull(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_invertFull__gmtl_Matrix33f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_invertFull__gmtl_Matrix44d_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Matrix44d invertFull(gmtl.Matrix44d p0, gmtl.Matrix44d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_invertFull__gmtl_Matrix44d_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_invertOrthogonal__gmtl_Matrix44d_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Matrix44d invertOrthogonal(gmtl.Matrix44d p0, gmtl.Matrix44d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_invertOrthogonal__gmtl_Matrix44d_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invertOrthogonal__gmtl_Matrix44f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invertOrthogonal(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_invertOrthogonal__gmtl_Matrix44f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_invertOrthogonal__gmtl_Matrix33d_gmtl_Matrix33d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1);

   public static gmtl.Matrix33d invertOrthogonal(gmtl.Matrix33d p0, gmtl.Matrix33d p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_invertOrthogonal__gmtl_Matrix33d_gmtl_Matrix33d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invertOrthogonal__gmtl_Matrix33f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invertOrthogonal(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_invertOrthogonal__gmtl_Matrix33f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invertTrans__gmtl_Matrix33f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invertTrans(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_invertTrans__gmtl_Matrix33f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_invertTrans__gmtl_Matrix33d_gmtl_Matrix33d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1);

   public static gmtl.Matrix33d invertTrans(gmtl.Matrix33d p0, gmtl.Matrix33d p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_invertTrans__gmtl_Matrix33d_gmtl_Matrix33d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invertTrans__gmtl_Matrix44f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invertTrans(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_invertTrans__gmtl_Matrix44f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_invertTrans__gmtl_Matrix44d_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Matrix44d invertTrans(gmtl.Matrix44d p0, gmtl.Matrix44d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_invertTrans__gmtl_Matrix44d_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Planed_gmtl_Planed_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p1,
	double p2);

   public static bool isEqual(gmtl.Planed p0, gmtl.Planed p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Planed_gmtl_Planed_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Planef_gmtl_Planef_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p1,
	float p2);

   public static bool isEqual(gmtl.Planef p0, gmtl.Planef p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Planef_gmtl_Planef_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Spheref_gmtl_Spheref_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p1,
	float p2);

   public static bool isEqual(gmtl.Spheref p0, gmtl.Spheref p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Spheref_gmtl_Spheref_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Sphered_gmtl_Sphered_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p1,
	double p2);

   public static bool isEqual(gmtl.Sphered p0, gmtl.Sphered p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Sphered_gmtl_Sphered_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix44d_gmtl_Matrix44d_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static bool isEqual(gmtl.Matrix44d p0, gmtl.Matrix44d p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Matrix44d_gmtl_Matrix44d_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix44d_gmtl_Matrix44d_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	double p2);

   public static bool isEqual(gmtl.Matrix44d p0, gmtl.Matrix44d p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Matrix44d_gmtl_Matrix44d_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZYXf_gmtl_EulerAngleZYXf_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p1);

   public static bool isEqual(gmtl.EulerAngleZYXf p0, gmtl.EulerAngleZYXf p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZYXf_gmtl_EulerAngleZYXf_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZYXf_gmtl_EulerAngleZYXf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p1,
	float p2);

   public static bool isEqual(gmtl.EulerAngleZYXf p0, gmtl.EulerAngleZYXf p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZYXf_gmtl_EulerAngleZYXf_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZXYf_gmtl_EulerAngleZXYf_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p1);

   public static bool isEqual(gmtl.EulerAngleZXYf p0, gmtl.EulerAngleZXYf p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZXYf_gmtl_EulerAngleZXYf_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZXYf_gmtl_EulerAngleZXYf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p1,
	float p2);

   public static bool isEqual(gmtl.EulerAngleZXYf p0, gmtl.EulerAngleZXYf p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZXYf_gmtl_EulerAngleZXYf_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleXYZd_gmtl_EulerAngleXYZd_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))] gmtl.EulerAngleXYZd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))] gmtl.EulerAngleXYZd p1);

   public static bool isEqual(gmtl.EulerAngleXYZd p0, gmtl.EulerAngleXYZd p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleXYZd_gmtl_EulerAngleXYZd_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleXYZd_gmtl_EulerAngleXYZd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))] gmtl.EulerAngleXYZd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))] gmtl.EulerAngleXYZd p1,
	double p2);

   public static bool isEqual(gmtl.EulerAngleXYZd p0, gmtl.EulerAngleXYZd p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleXYZd_gmtl_EulerAngleXYZd_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZXYd_gmtl_EulerAngleZXYd_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))] gmtl.EulerAngleZXYd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))] gmtl.EulerAngleZXYd p1);

   public static bool isEqual(gmtl.EulerAngleZXYd p0, gmtl.EulerAngleZXYd p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZXYd_gmtl_EulerAngleZXYd_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZXYd_gmtl_EulerAngleZXYd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))] gmtl.EulerAngleZXYd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))] gmtl.EulerAngleZXYd p1,
	double p2);

   public static bool isEqual(gmtl.EulerAngleZXYd p0, gmtl.EulerAngleZXYd p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZXYd_gmtl_EulerAngleZXYd_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleXYZf_gmtl_EulerAngleXYZf_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p1);

   public static bool isEqual(gmtl.EulerAngleXYZf p0, gmtl.EulerAngleXYZf p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleXYZf_gmtl_EulerAngleXYZf_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleXYZf_gmtl_EulerAngleXYZf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p1,
	float p2);

   public static bool isEqual(gmtl.EulerAngleXYZf p0, gmtl.EulerAngleXYZf p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleXYZf_gmtl_EulerAngleXYZf_float3(p0, p1, p2);
      return result;
   }

#if UNIX
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dZXY_gmtl_Coord3dZXY_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZXYMarshaler))] gmtl.Coord3dZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZXYMarshaler))] gmtl.Coord3dZXY p1);

   public static bool isEqual(gmtl.Coord3dZXY p0, gmtl.Coord3dZXY p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dZXY_gmtl_Coord3dZXY_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dZXY_gmtl_Coord3dZXY_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZXYMarshaler))] gmtl.Coord3dZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZXYMarshaler))] gmtl.Coord3dZXY p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dZXY p0, gmtl.Coord3dZXY p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dZXY_gmtl_Coord3dZXY_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fXYZ_gmtl_Coord4fXYZ_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p1);

   public static bool isEqual(gmtl.Coord4fXYZ p0, gmtl.Coord4fXYZ p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fXYZ_gmtl_Coord4fXYZ_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fXYZ_gmtl_Coord4fXYZ_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fXYZ p0, gmtl.Coord4fXYZ p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fXYZ_gmtl_Coord4fXYZ_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dQuat_gmtl_Coord3dQuat_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dQuatMarshaler))] gmtl.Coord3dQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dQuatMarshaler))] gmtl.Coord3dQuat p1);

   public static bool isEqual(gmtl.Coord3dQuat p0, gmtl.Coord3dQuat p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dQuat_gmtl_Coord3dQuat_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dQuat_gmtl_Coord3dQuat_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dQuatMarshaler))] gmtl.Coord3dQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dQuatMarshaler))] gmtl.Coord3dQuat p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dQuat p0, gmtl.Coord3dQuat p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dQuat_gmtl_Coord3dQuat_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fQuat_gmtl_Coord4fQuat_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p1);

   public static bool isEqual(gmtl.Coord4fQuat p0, gmtl.Coord4fQuat p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fQuat_gmtl_Coord4fQuat_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fQuat_gmtl_Coord4fQuat_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fQuat p0, gmtl.Coord4fQuat p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fQuat_gmtl_Coord4fQuat_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dZXY_gmtl_Coord4dZXY_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZXYMarshaler))] gmtl.Coord4dZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZXYMarshaler))] gmtl.Coord4dZXY p1);

   public static bool isEqual(gmtl.Coord4dZXY p0, gmtl.Coord4dZXY p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dZXY_gmtl_Coord4dZXY_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dZXY_gmtl_Coord4dZXY_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZXYMarshaler))] gmtl.Coord4dZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZXYMarshaler))] gmtl.Coord4dZXY p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dZXY p0, gmtl.Coord4dZXY p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dZXY_gmtl_Coord4dZXY_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fQuat_gmtl_Coord3fQuat_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p1);

   public static bool isEqual(gmtl.Coord3fQuat p0, gmtl.Coord3fQuat p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fQuat_gmtl_Coord3fQuat_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fQuat_gmtl_Coord3fQuat_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fQuat p0, gmtl.Coord3fQuat p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fQuat_gmtl_Coord3fQuat_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dXYZ_gmtl_Coord4dXYZ_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dXYZMarshaler))] gmtl.Coord4dXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dXYZMarshaler))] gmtl.Coord4dXYZ p1);

   public static bool isEqual(gmtl.Coord4dXYZ p0, gmtl.Coord4dXYZ p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dXYZ_gmtl_Coord4dXYZ_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dXYZ_gmtl_Coord4dXYZ_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dXYZMarshaler))] gmtl.Coord4dXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dXYZMarshaler))] gmtl.Coord4dXYZ p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dXYZ p0, gmtl.Coord4dXYZ p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dXYZ_gmtl_Coord4dXYZ_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fZYX_gmtl_Coord4fZYX_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p1);

   public static bool isEqual(gmtl.Coord4fZYX p0, gmtl.Coord4fZYX p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fZYX_gmtl_Coord4fZYX_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fZYX_gmtl_Coord4fZYX_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fZYX p0, gmtl.Coord4fZYX p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fZYX_gmtl_Coord4fZYX_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fZXY_gmtl_Coord4fZXY_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p1);

   public static bool isEqual(gmtl.Coord4fZXY p0, gmtl.Coord4fZXY p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fZXY_gmtl_Coord4fZXY_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fZXY_gmtl_Coord4fZXY_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fZXY p0, gmtl.Coord4fZXY p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fZXY_gmtl_Coord4fZXY_float3(p0, p1, p2);
      return result;
   }
#endif

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Quatf_gmtl_Quatf_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1);

   public static bool isEqual(gmtl.Quatf p0, gmtl.Quatf p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Quatf_gmtl_Quatf_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Quatf_gmtl_Quatf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	float p2);

   public static bool isEqual(gmtl.Quatf p0, gmtl.Quatf p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Quatf_gmtl_Quatf_float3(p0, p1, p2);
      return result;
   }

#if UNIX
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fZXY_gmtl_Coord3fZXY_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p1);

   public static bool isEqual(gmtl.Coord3fZXY p0, gmtl.Coord3fZXY p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fZXY_gmtl_Coord3fZXY_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fZXY_gmtl_Coord3fZXY_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fZXY p0, gmtl.Coord3fZXY p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fZXY_gmtl_Coord3fZXY_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fZYX_gmtl_Coord3fZYX_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p1);

   public static bool isEqual(gmtl.Coord3fZYX p0, gmtl.Coord3fZYX p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fZYX_gmtl_Coord3fZYX_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fZYX_gmtl_Coord3fZYX_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fZYX p0, gmtl.Coord3fZYX p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fZYX_gmtl_Coord3fZYX_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dZYX_gmtl_Coord3dZYX_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZYXMarshaler))] gmtl.Coord3dZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZYXMarshaler))] gmtl.Coord3dZYX p1);

   public static bool isEqual(gmtl.Coord3dZYX p0, gmtl.Coord3dZYX p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dZYX_gmtl_Coord3dZYX_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dZYX_gmtl_Coord3dZYX_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZYXMarshaler))] gmtl.Coord3dZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZYXMarshaler))] gmtl.Coord3dZYX p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dZYX p0, gmtl.Coord3dZYX p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dZYX_gmtl_Coord3dZYX_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dXYZ_gmtl_Coord3dXYZ_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dXYZMarshaler))] gmtl.Coord3dXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dXYZMarshaler))] gmtl.Coord3dXYZ p1);

   public static bool isEqual(gmtl.Coord3dXYZ p0, gmtl.Coord3dXYZ p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dXYZ_gmtl_Coord3dXYZ_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dXYZ_gmtl_Coord3dXYZ_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dXYZMarshaler))] gmtl.Coord3dXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dXYZMarshaler))] gmtl.Coord3dXYZ p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dXYZ p0, gmtl.Coord3dXYZ p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dXYZ_gmtl_Coord3dXYZ_double3(p0, p1, p2);
      return result;
   }
#endif

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Quatd_gmtl_Quatd_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1);

   public static bool isEqual(gmtl.Quatd p0, gmtl.Quatd p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Quatd_gmtl_Quatd_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Quatd_gmtl_Quatd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	double p2);

   public static bool isEqual(gmtl.Quatd p0, gmtl.Quatd p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Quatd_gmtl_Quatd_double3(p0, p1, p2);
      return result;
   }

#if UNIX
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fXYZ_gmtl_Coord3fXYZ_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p1);

   public static bool isEqual(gmtl.Coord3fXYZ p0, gmtl.Coord3fXYZ p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fXYZ_gmtl_Coord3fXYZ_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fXYZ_gmtl_Coord3fXYZ_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fXYZ p0, gmtl.Coord3fXYZ p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fXYZ_gmtl_Coord3fXYZ_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dZYX_gmtl_Coord4dZYX_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZYXMarshaler))] gmtl.Coord4dZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZYXMarshaler))] gmtl.Coord4dZYX p1);

   public static bool isEqual(gmtl.Coord4dZYX p0, gmtl.Coord4dZYX p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dZYX_gmtl_Coord4dZYX_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dZYX_gmtl_Coord4dZYX_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZYXMarshaler))] gmtl.Coord4dZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZYXMarshaler))] gmtl.Coord4dZYX p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dZYX p0, gmtl.Coord4dZYX p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dZYX_gmtl_Coord4dZYX_double3(p0, p1, p2);
      return result;
   }
#endif

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Trif_gmtl_Trif_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TrifMarshaler))] gmtl.Trif p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TrifMarshaler))] gmtl.Trif p1,
	float p2);

   public static bool isEqual(gmtl.Trif p0, gmtl.Trif p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Trif_gmtl_Trif_float3(p0, p1, p2);
      return result;
   }

#if UNIX
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dAxisAngle_gmtl_Coord4dAxisAngle_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dAxisAngleMarshaler))] gmtl.Coord4dAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dAxisAngleMarshaler))] gmtl.Coord4dAxisAngle p1);

   public static bool isEqual(gmtl.Coord4dAxisAngle p0, gmtl.Coord4dAxisAngle p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dAxisAngle_gmtl_Coord4dAxisAngle_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dAxisAngle_gmtl_Coord4dAxisAngle_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dAxisAngleMarshaler))] gmtl.Coord4dAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dAxisAngleMarshaler))] gmtl.Coord4dAxisAngle p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dAxisAngle p0, gmtl.Coord4dAxisAngle p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dAxisAngle_gmtl_Coord4dAxisAngle_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fAxisAngle_gmtl_Coord4fAxisAngle_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fAxisAngleMarshaler))] gmtl.Coord4fAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fAxisAngleMarshaler))] gmtl.Coord4fAxisAngle p1);

   public static bool isEqual(gmtl.Coord4fAxisAngle p0, gmtl.Coord4fAxisAngle p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fAxisAngle_gmtl_Coord4fAxisAngle_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fAxisAngle_gmtl_Coord4fAxisAngle_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fAxisAngleMarshaler))] gmtl.Coord4fAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fAxisAngleMarshaler))] gmtl.Coord4fAxisAngle p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fAxisAngle p0, gmtl.Coord4fAxisAngle p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fAxisAngle_gmtl_Coord4fAxisAngle_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fAxisAngle_gmtl_Coord3fAxisAngle_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fAxisAngleMarshaler))] gmtl.Coord3fAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fAxisAngleMarshaler))] gmtl.Coord3fAxisAngle p1);

   public static bool isEqual(gmtl.Coord3fAxisAngle p0, gmtl.Coord3fAxisAngle p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fAxisAngle_gmtl_Coord3fAxisAngle_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fAxisAngle_gmtl_Coord3fAxisAngle_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fAxisAngleMarshaler))] gmtl.Coord3fAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fAxisAngleMarshaler))] gmtl.Coord3fAxisAngle p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fAxisAngle p0, gmtl.Coord3fAxisAngle p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fAxisAngle_gmtl_Coord3fAxisAngle_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dQuat_gmtl_Coord4dQuat_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dQuatMarshaler))] gmtl.Coord4dQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dQuatMarshaler))] gmtl.Coord4dQuat p1);

   public static bool isEqual(gmtl.Coord4dQuat p0, gmtl.Coord4dQuat p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dQuat_gmtl_Coord4dQuat_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dQuat_gmtl_Coord4dQuat_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dQuatMarshaler))] gmtl.Coord4dQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dQuatMarshaler))] gmtl.Coord4dQuat p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dQuat p0, gmtl.Coord4dQuat p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dQuat_gmtl_Coord4dQuat_double3(p0, p1, p2);
      return result;
   }
#endif

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Trid_gmtl_Trid_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TridMarshaler))] gmtl.Trid p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TridMarshaler))] gmtl.Trid p1,
	double p2);

   public static bool isEqual(gmtl.Trid p0, gmtl.Trid p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Trid_gmtl_Trid_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZYXd_gmtl_EulerAngleZYXd_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))] gmtl.EulerAngleZYXd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))] gmtl.EulerAngleZYXd p1);

   public static bool isEqual(gmtl.EulerAngleZYXd p0, gmtl.EulerAngleZYXd p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZYXd_gmtl_EulerAngleZYXd_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZYXd_gmtl_EulerAngleZYXd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))] gmtl.EulerAngleZYXd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))] gmtl.EulerAngleZYXd p1,
	double p2);

   public static bool isEqual(gmtl.EulerAngleZYXd p0, gmtl.EulerAngleZYXd p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZYXd_gmtl_EulerAngleZYXd_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static bool isEqual(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	float p2);

   public static bool isEqual(gmtl.Matrix44f p0, gmtl.Matrix44f p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float3(p0, p1, p2);
      return result;
   }

#if UNIX
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dAxisAngle_gmtl_Coord3dAxisAngle_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dAxisAngleMarshaler))] gmtl.Coord3dAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dAxisAngleMarshaler))] gmtl.Coord3dAxisAngle p1);

   public static bool isEqual(gmtl.Coord3dAxisAngle p0, gmtl.Coord3dAxisAngle p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dAxisAngle_gmtl_Coord3dAxisAngle_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dAxisAngle_gmtl_Coord3dAxisAngle_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dAxisAngleMarshaler))] gmtl.Coord3dAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dAxisAngleMarshaler))] gmtl.Coord3dAxisAngle p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dAxisAngle p0, gmtl.Coord3dAxisAngle p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dAxisAngle_gmtl_Coord3dAxisAngle_double3(p0, p1, p2);
      return result;
   }
#endif

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AABoxf_gmtl_AABoxf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p1,
	float p2);

   public static bool isEqual(gmtl.AABoxf p0, gmtl.AABoxf p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_AABoxf_gmtl_AABoxf_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AABoxd_gmtl_AABoxd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p1,
	double p2);

   public static bool isEqual(gmtl.AABoxd p0, gmtl.AABoxd p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_AABoxd_gmtl_AABoxd_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_VecBase_float_4_gmtl_VecBase_float_4_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p1,
	float p2);

   public static bool isEqual(gmtl.VecBase_float_4 p0, gmtl.VecBase_float_4 p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_VecBase_float_4_gmtl_VecBase_float_4_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_VecBase_double_4_gmtl_VecBase_double_4_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p1,
	double p2);

   public static bool isEqual(gmtl.VecBase_double_4 p0, gmtl.VecBase_double_4 p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_VecBase_double_4_gmtl_VecBase_double_4_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_VecBase_float_3_gmtl_VecBase_float_3_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p1,
	float p2);

   public static bool isEqual(gmtl.VecBase_float_3 p0, gmtl.VecBase_float_3 p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_VecBase_float_3_gmtl_VecBase_float_3_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_VecBase_double_3_gmtl_VecBase_double_3_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p1,
	double p2);

   public static bool isEqual(gmtl.VecBase_double_3 p0, gmtl.VecBase_double_3 p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_VecBase_double_3_gmtl_VecBase_double_3_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix33d_gmtl_Matrix33d_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1);

   public static bool isEqual(gmtl.Matrix33d p0, gmtl.Matrix33d p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Matrix33d_gmtl_Matrix33d_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix33d_gmtl_Matrix33d_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1,
	double p2);

   public static bool isEqual(gmtl.Matrix33d p0, gmtl.Matrix33d p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Matrix33d_gmtl_Matrix33d_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static bool isEqual(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	float p2);

   public static bool isEqual(gmtl.Matrix33f p0, gmtl.Matrix33f p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AxisAngled_gmtl_AxisAngled_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p1);

   public static bool isEqual(gmtl.AxisAngled p0, gmtl.AxisAngled p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_AxisAngled_gmtl_AxisAngled_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AxisAngled_gmtl_AxisAngled_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p1,
	double p2);

   public static bool isEqual(gmtl.AxisAngled p0, gmtl.AxisAngled p1, double p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_AxisAngled_gmtl_AxisAngled_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AxisAnglef_gmtl_AxisAnglef_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p1);

   public static bool isEqual(gmtl.AxisAnglef p0, gmtl.AxisAnglef p1)
   {
      bool result;
      result = gmtl_isEqual__gmtl_AxisAnglef_gmtl_AxisAnglef_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AxisAnglef_gmtl_AxisAnglef_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p1,
	float p2);

   public static bool isEqual(gmtl.AxisAnglef p0, gmtl.AxisAnglef p1, float p2)
   {
      bool result;
      result = gmtl_isEqual__gmtl_AxisAnglef_gmtl_AxisAnglef_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEquiv__gmtl_Quatf_gmtl_Quatf_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1);

   public static bool isEquiv(gmtl.Quatf p0, gmtl.Quatf p1)
   {
      bool result;
      result = gmtl_isEquiv__gmtl_Quatf_gmtl_Quatf_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEquiv__gmtl_Quatf_gmtl_Quatf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	float p2);

   public static bool isEquiv(gmtl.Quatf p0, gmtl.Quatf p1, float p2)
   {
      bool result;
      result = gmtl_isEquiv__gmtl_Quatf_gmtl_Quatf_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEquiv__gmtl_Quatd_gmtl_Quatd_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1);

   public static bool isEquiv(gmtl.Quatd p0, gmtl.Quatd p1)
   {
      bool result;
      result = gmtl_isEquiv__gmtl_Quatd_gmtl_Quatd_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEquiv__gmtl_Quatd_gmtl_Quatd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	double p2);

   public static bool isEquiv(gmtl.Quatd p0, gmtl.Quatd p1, double p2)
   {
      bool result;
      result = gmtl_isEquiv__gmtl_Quatd_gmtl_Quatd_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Vec3f_float1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public static bool isNormalized(gmtl.Vec3f p0)
   {
      bool result;
      result = gmtl_isNormalized__gmtl_Vec3f_float1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Vec3f_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	float p1);

   public static bool isNormalized(gmtl.Vec3f p0, float p1)
   {
      bool result;
      result = gmtl_isNormalized__gmtl_Vec3f_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Vec4f_float1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0);

   public static bool isNormalized(gmtl.Vec4f p0)
   {
      bool result;
      result = gmtl_isNormalized__gmtl_Vec4f_float1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Vec4f_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0,
	float p1);

   public static bool isNormalized(gmtl.Vec4f p0, float p1)
   {
      bool result;
      result = gmtl_isNormalized__gmtl_Vec4f_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Quatd_double1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static bool isNormalized(gmtl.Quatd p0)
   {
      bool result;
      result = gmtl_isNormalized__gmtl_Quatd_double1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Quatd_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	double p1);

   public static bool isNormalized(gmtl.Quatd p0, double p1)
   {
      bool result;
      result = gmtl_isNormalized__gmtl_Quatd_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Quatf_float1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static bool isNormalized(gmtl.Quatf p0)
   {
      bool result;
      result = gmtl_isNormalized__gmtl_Quatf_float1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Quatf_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	float p1);

   public static bool isNormalized(gmtl.Quatf p0, float p1)
   {
      bool result;
      result = gmtl_isNormalized__gmtl_Quatf_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_length__gmtl_Vec4f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0);

   public static float length(gmtl.Vec4f p0)
   {
      float result;
      result = gmtl_length__gmtl_Vec4f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_length__gmtl_Vec3d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0);

   public static double length(gmtl.Vec3d p0)
   {
      double result;
      result = gmtl_length__gmtl_Vec3d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_length__gmtl_Vec3f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public static float length(gmtl.Vec3f p0)
   {
      float result;
      result = gmtl_length__gmtl_Vec3f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_length__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static double length(gmtl.Quatd p0)
   {
      double result;
      result = gmtl_length__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_length__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static float length(gmtl.Quatf p0)
   {
      float result;
      result = gmtl_length__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_lengthSquared__gmtl_Vec3d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0);

   public static double lengthSquared(gmtl.Vec3d p0)
   {
      double result;
      result = gmtl_lengthSquared__gmtl_Vec3d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_lengthSquared__gmtl_Vec3f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public static float lengthSquared(gmtl.Vec3f p0)
   {
      float result;
      result = gmtl_lengthSquared__gmtl_Vec3f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_lengthSquared__gmtl_Vec4f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0);

   public static float lengthSquared(gmtl.Vec4f p0)
   {
      float result;
      result = gmtl_lengthSquared__gmtl_Vec4f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_lengthSquared__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static float lengthSquared(gmtl.Quatf p0)
   {
      float result;
      result = gmtl_lengthSquared__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_lengthSquared__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static double lengthSquared(gmtl.Quatd p0)
   {
      double result;
      result = gmtl_lengthSquared__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))]
   private extern static gmtl.VecBase_double_3 gmtl_lerp__gmtl_VecBase_double_3_double_gmtl_VecBase_double_3_gmtl_VecBase_double_34([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p0,
	double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p3);

   public static gmtl.VecBase_double_3 lerp(gmtl.VecBase_double_3 p0, double p1, gmtl.VecBase_double_3 p2, gmtl.VecBase_double_3 p3)
   {
      gmtl.VecBase_double_3 result;
      result = gmtl_lerp__gmtl_VecBase_double_3_double_gmtl_VecBase_double_3_gmtl_VecBase_double_34(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))]
   private extern static gmtl.VecBase_float_3 gmtl_lerp__gmtl_VecBase_float_3_float_gmtl_VecBase_float_3_gmtl_VecBase_float_34([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p0,
	float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p3);

   public static gmtl.VecBase_float_3 lerp(gmtl.VecBase_float_3 p0, float p1, gmtl.VecBase_float_3 p2, gmtl.VecBase_float_3 p3)
   {
      gmtl.VecBase_float_3 result;
      result = gmtl_lerp__gmtl_VecBase_float_3_float_gmtl_VecBase_float_3_gmtl_VecBase_float_34(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_lerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p3);

   public static gmtl.Quatd lerp(gmtl.Quatd p0, double p1, gmtl.Quatd p2, gmtl.Quatd p3)
   {
      gmtl.Quatd result;
      result = gmtl_lerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_lerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p3);

   public static gmtl.Quatf lerp(gmtl.Quatf p0, float p1, gmtl.Quatf p2, gmtl.Quatf p3)
   {
      gmtl.Quatf result;
      result = gmtl_lerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))]
   private extern static gmtl.VecBase_float_4 gmtl_lerp__gmtl_VecBase_float_4_float_gmtl_VecBase_float_4_gmtl_VecBase_float_44([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p0,
	float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p3);

   public static gmtl.VecBase_float_4 lerp(gmtl.VecBase_float_4 p0, float p1, gmtl.VecBase_float_4 p2, gmtl.VecBase_float_4 p3)
   {
      gmtl.VecBase_float_4 result;
      result = gmtl_lerp__gmtl_VecBase_float_4_float_gmtl_VecBase_float_4_gmtl_VecBase_float_44(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))]
   private extern static gmtl.VecBase_double_4 gmtl_lerp__gmtl_VecBase_double_4_double_gmtl_VecBase_double_4_gmtl_VecBase_double_44([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p0,
	double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p3);

   public static gmtl.VecBase_double_4 lerp(gmtl.VecBase_double_4 p0, double p1, gmtl.VecBase_double_4 p2, gmtl.VecBase_double_4 p3)
   {
      gmtl.VecBase_double_4 result;
      result = gmtl_lerp__gmtl_VecBase_double_4_double_gmtl_VecBase_double_4_gmtl_VecBase_double_44(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_log__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf log(gmtl.Quatf p0)
   {
      gmtl.Quatf result;
      result = gmtl_log__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_log__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd log(gmtl.Quatd p0)
   {
      gmtl.Quatd result;
      result = gmtl_log__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_mult__gmtl_Quatf_gmtl_Quatf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	float p2);

   public static gmtl.Quatf mult(gmtl.Quatf p0, gmtl.Quatf p1, float p2)
   {
      gmtl.Quatf result;
      result = gmtl_mult__gmtl_Quatf_gmtl_Quatf_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_mult__gmtl_Quatd_gmtl_Quatd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	double p2);

   public static gmtl.Quatd mult(gmtl.Quatd p0, gmtl.Quatd p1, double p2)
   {
      gmtl.Quatd result;
      result = gmtl_mult__gmtl_Quatd_gmtl_Quatd_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_mult__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2);

   public static gmtl.Quatf mult(gmtl.Quatf p0, gmtl.Quatf p1, gmtl.Quatf p2)
   {
      gmtl.Quatf result;
      result = gmtl_mult__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p2);

   public static gmtl.Matrix44f mult(gmtl.Matrix44f p0, gmtl.Matrix44f p1, gmtl.Matrix44f p2)
   {
      gmtl.Matrix44f result;
      result = gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_mult__gmtl_Matrix44d_gmtl_Matrix44d_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	double p2);

   public static gmtl.Matrix44d mult(gmtl.Matrix44d p0, gmtl.Matrix44d p1, double p2)
   {
      gmtl.Matrix44d result;
      result = gmtl_mult__gmtl_Matrix44d_gmtl_Matrix44d_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	float p2);

   public static gmtl.Matrix44f mult(gmtl.Matrix44f p0, gmtl.Matrix44f p1, float p2)
   {
      gmtl.Matrix44f result;
      result = gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p2);

   public static gmtl.Matrix33f mult(gmtl.Matrix33f p0, gmtl.Matrix33f p1, gmtl.Matrix33f p2)
   {
      gmtl.Matrix33f result;
      result = gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_mult__gmtl_Matrix33d_gmtl_Matrix33d_gmtl_Matrix33d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p2);

   public static gmtl.Matrix33d mult(gmtl.Matrix33d p0, gmtl.Matrix33d p1, gmtl.Matrix33d p2)
   {
      gmtl.Matrix33d result;
      result = gmtl_mult__gmtl_Matrix33d_gmtl_Matrix33d_gmtl_Matrix33d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_mult__gmtl_Matrix44d_gmtl_Matrix44d_gmtl_Matrix44d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p2);

   public static gmtl.Matrix44d mult(gmtl.Matrix44d p0, gmtl.Matrix44d p1, gmtl.Matrix44d p2)
   {
      gmtl.Matrix44d result;
      result = gmtl_mult__gmtl_Matrix44d_gmtl_Matrix44d_gmtl_Matrix44d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_mult__gmtl_Matrix44d_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	double p1);

   public static gmtl.Matrix44d mult(gmtl.Matrix44d p0, double p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_mult__gmtl_Matrix44d_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_mult__gmtl_Matrix44f_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	float p1);

   public static gmtl.Matrix44f mult(gmtl.Matrix44f p0, float p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_mult__gmtl_Matrix44f_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_mult__gmtl_Matrix33d_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	double p1);

   public static gmtl.Matrix33d mult(gmtl.Matrix33d p0, double p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_mult__gmtl_Matrix33d_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_mult__gmtl_Matrix33f_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	float p1);

   public static gmtl.Matrix33f mult(gmtl.Matrix33f p0, float p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_mult__gmtl_Matrix33f_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_mult__gmtl_Matrix33d_gmtl_Matrix33d_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1,
	double p2);

   public static gmtl.Matrix33d mult(gmtl.Matrix33d p0, gmtl.Matrix33d p1, double p2)
   {
      gmtl.Matrix33d result;
      result = gmtl_mult__gmtl_Matrix33d_gmtl_Matrix33d_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	float p2);

   public static gmtl.Matrix33f mult(gmtl.Matrix33f p0, gmtl.Matrix33f p1, float p2)
   {
      gmtl.Matrix33f result;
      result = gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_mult__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2);

   public static gmtl.Quatd mult(gmtl.Quatd p0, gmtl.Quatd p1, gmtl.Quatd p2)
   {
      gmtl.Quatd result;
      result = gmtl_mult__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_negate__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf negate(gmtl.Quatf p0)
   {
      gmtl.Quatf result;
      result = gmtl_negate__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_negate__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd negate(gmtl.Quatd p0)
   {
      gmtl.Quatd result;
      result = gmtl_negate__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_postMult__gmtl_Matrix33d_gmtl_Matrix33d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1);

   public static gmtl.Matrix33d postMult(gmtl.Matrix33d p0, gmtl.Matrix33d p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_postMult__gmtl_Matrix33d_gmtl_Matrix33d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_postMult__gmtl_Matrix33f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f postMult(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_postMult__gmtl_Matrix33f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_postMult__gmtl_Matrix44d_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Matrix44d postMult(gmtl.Matrix44d p0, gmtl.Matrix44d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_postMult__gmtl_Matrix44d_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_postMult__gmtl_Matrix44f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f postMult(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_postMult__gmtl_Matrix44f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_preMult__gmtl_Matrix33d_gmtl_Matrix33d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1);

   public static gmtl.Matrix33d preMult(gmtl.Matrix33d p0, gmtl.Matrix33d p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_preMult__gmtl_Matrix33d_gmtl_Matrix33d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_preMult__gmtl_Matrix33f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f preMult(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_preMult__gmtl_Matrix33f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_preMult__gmtl_Matrix44d_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Matrix44d preMult(gmtl.Matrix44d p0, gmtl.Matrix44d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_preMult__gmtl_Matrix44d_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_preMult__gmtl_Matrix44f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f preMult(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_preMult__gmtl_Matrix44f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))]
   private extern static gmtl.VecBase_double_4 gmtl_reflect__gmtl_VecBase_double_4_gmtl_VecBase_double_4_gmtl_Vec4d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p2);

   public static gmtl.VecBase_double_4 reflect(gmtl.VecBase_double_4 p0, gmtl.VecBase_double_4 p1, gmtl.Vec4d p2)
   {
      gmtl.VecBase_double_4 result;
      result = gmtl_reflect__gmtl_VecBase_double_4_gmtl_VecBase_double_4_gmtl_Vec4d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))]
   private extern static gmtl.VecBase_float_4 gmtl_reflect__gmtl_VecBase_float_4_gmtl_VecBase_float_4_gmtl_Vec4f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p2);

   public static gmtl.VecBase_float_4 reflect(gmtl.VecBase_float_4 p0, gmtl.VecBase_float_4 p1, gmtl.Vec4f p2)
   {
      gmtl.VecBase_float_4 result;
      result = gmtl_reflect__gmtl_VecBase_float_4_gmtl_VecBase_float_4_gmtl_Vec4f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))]
   private extern static gmtl.VecBase_double_3 gmtl_reflect__gmtl_VecBase_double_3_gmtl_VecBase_double_3_gmtl_Vec3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.VecBase_double_3 reflect(gmtl.VecBase_double_3 p0, gmtl.VecBase_double_3 p1, gmtl.Vec3d p2)
   {
      gmtl.VecBase_double_3 result;
      result = gmtl_reflect__gmtl_VecBase_double_3_gmtl_VecBase_double_3_gmtl_Vec3d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))]
   private extern static gmtl.VecBase_float_3 gmtl_reflect__gmtl_VecBase_float_3_gmtl_VecBase_float_3_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.VecBase_float_3 reflect(gmtl.VecBase_float_3 p0, gmtl.VecBase_float_3 p1, gmtl.Vec3f p2)
   {
      gmtl.VecBase_float_3 result;
      result = gmtl_reflect__gmtl_VecBase_float_3_gmtl_VecBase_float_3_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_slerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_bool4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p3);

   public static gmtl.Quatf slerp(gmtl.Quatf p0, float p1, gmtl.Quatf p2, gmtl.Quatf p3)
   {
      gmtl.Quatf result;
      result = gmtl_slerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_bool4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_slerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_bool5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p3,
	bool p4);

   public static gmtl.Quatf slerp(gmtl.Quatf p0, float p1, gmtl.Quatf p2, gmtl.Quatf p3, bool p4)
   {
      gmtl.Quatf result;
      result = gmtl_slerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_bool5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_slerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_bool4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p3);

   public static gmtl.Quatd slerp(gmtl.Quatd p0, double p1, gmtl.Quatd p2, gmtl.Quatd p3)
   {
      gmtl.Quatd result;
      result = gmtl_slerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_bool4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_slerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_bool5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p3,
	bool p4);

   public static gmtl.Quatd slerp(gmtl.Quatd p0, double p1, gmtl.Quatd p2, gmtl.Quatd p3, bool p4)
   {
      gmtl.Quatd result;
      result = gmtl_slerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_bool5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_squad__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p5);

   public static void squad(gmtl.Quatd p0, double p1, gmtl.Quatd p2, gmtl.Quatd p3, gmtl.Quatd p4, gmtl.Quatd p5)
   {
      gmtl_squad__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd6(p0, p1, p2, p3, p4, p5);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_squad__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p5);

   public static void squad(gmtl.Quatf p0, float p1, gmtl.Quatf p2, gmtl.Quatf p3, gmtl.Quatf p4, gmtl.Quatf p5)
   {
      gmtl_squad__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf6(p0, p1, p2, p3, p4, p5);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_sub__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2);

   public static gmtl.Quatd sub(gmtl.Quatd p0, gmtl.Quatd p1, gmtl.Quatd p2)
   {
      gmtl.Quatd result;
      result = gmtl_sub__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_sub__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2);

   public static gmtl.Quatf sub(gmtl.Quatf p0, gmtl.Quatf p1, gmtl.Quatf p2)
   {
      gmtl.Quatf result;
      result = gmtl_sub__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_sub__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p2);

   public static gmtl.Matrix44f sub(gmtl.Matrix44f p0, gmtl.Matrix44f p1, gmtl.Matrix44f p2)
   {
      gmtl.Matrix44f result;
      result = gmtl_sub__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_sub__gmtl_Matrix44d_gmtl_Matrix44d_gmtl_Matrix44d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p2);

   public static gmtl.Matrix44d sub(gmtl.Matrix44d p0, gmtl.Matrix44d p1, gmtl.Matrix44d p2)
   {
      gmtl.Matrix44d result;
      result = gmtl_sub__gmtl_Matrix44d_gmtl_Matrix44d_gmtl_Matrix44d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_sub__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p2);

   public static gmtl.Matrix33f sub(gmtl.Matrix33f p0, gmtl.Matrix33f p1, gmtl.Matrix33f p2)
   {
      gmtl.Matrix33f result;
      result = gmtl_sub__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_sub__gmtl_Matrix33d_gmtl_Matrix33d_gmtl_Matrix33d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p2);

   public static gmtl.Matrix33d sub(gmtl.Matrix33d p0, gmtl.Matrix33d p1, gmtl.Matrix33d p2)
   {
      gmtl.Matrix33d result;
      result = gmtl_sub__gmtl_Matrix33d_gmtl_Matrix33d_gmtl_Matrix33d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_transpose__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.Matrix44d transpose(gmtl.Matrix44d p0)
   {
      gmtl.Matrix44d result;
      result = gmtl_transpose__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_transpose__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f transpose(gmtl.Matrix44f p0)
   {
      gmtl.Matrix44f result;
      result = gmtl_transpose__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_transpose__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public static gmtl.Matrix33d transpose(gmtl.Matrix33d p0)
   {
      gmtl.Matrix33d result;
      result = gmtl_transpose__gmtl_Matrix33d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_transpose__gmtl_Matrix33f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f transpose(gmtl.Matrix33f p0)
   {
      gmtl.Matrix33f result;
      result = gmtl_transpose__gmtl_Matrix33f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_transpose__gmtl_Matrix33f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f transpose(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_transpose__gmtl_Matrix33f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_transpose__gmtl_Matrix33d_gmtl_Matrix33d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1);

   public static gmtl.Matrix33d transpose(gmtl.Matrix33d p0, gmtl.Matrix33d p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_transpose__gmtl_Matrix33d_gmtl_Matrix33d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_transpose__gmtl_Matrix44f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f transpose(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_transpose__gmtl_Matrix44f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_transpose__gmtl_Matrix44d_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Matrix44d transpose(gmtl.Matrix44d p0, gmtl.Matrix44d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_transpose__gmtl_Matrix44d_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static PlaneSide gmtl_whichSide__gmtl_Planed_gmtl_Point3d_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1,
	double p2);

   public static PlaneSide whichSide(gmtl.Planed p0, gmtl.Point3d p1, double p2)
   {
      PlaneSide result;
      result = gmtl_whichSide__gmtl_Planed_gmtl_Point3d_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static PlaneSide gmtl_whichSide__gmtl_Planef_gmtl_Point3f_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1,
	float p2);

   public static PlaneSide whichSide(gmtl.Planef p0, gmtl.Point3f p1, float p2)
   {
      PlaneSide result;
      result = gmtl_whichSide__gmtl_Planef_gmtl_Point3f_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static PlaneSide gmtl_whichSide__gmtl_Planef_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static PlaneSide whichSide(gmtl.Planef p0, gmtl.Point3f p1)
   {
      PlaneSide result;
      result = gmtl_whichSide__gmtl_Planef_gmtl_Point3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static PlaneSide gmtl_whichSide__gmtl_Planed_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static PlaneSide whichSide(gmtl.Planed p0, gmtl.Point3d p1)
   {
      PlaneSide result;
      result = gmtl_whichSide__gmtl_Planed_gmtl_Point3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_zero__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.Matrix44d zero(gmtl.Matrix44d p0)
   {
      gmtl.Matrix44d result;
      result = gmtl_zero__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_zero__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f zero(gmtl.Matrix44f p0)
   {
      gmtl.Matrix44f result;
      result = gmtl_zero__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_zero__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public static gmtl.Matrix33d zero(gmtl.Matrix33d p0)
   {
      gmtl.Matrix33d result;
      result = gmtl_zero__gmtl_Matrix33d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_zero__gmtl_Matrix33f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f zero(gmtl.Matrix33f p0)
   {
      gmtl.Matrix33f result;
      result = gmtl_zero__gmtl_Matrix33f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtlWrappers_makeRotMatrix33__gmtl_EulerAngleXYZf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p0);

   public static gmtl.Matrix33f makeRotMatrix33(gmtl.EulerAngleXYZf p0)
   {
      gmtl.Matrix33f result;
      result = gmtlWrappers_makeRotMatrix33__gmtl_EulerAngleXYZf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtlWrappers_makeRotMatrix33__gmtl_EulerAngleZYXf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p0);

   public static gmtl.Matrix33f makeRotMatrix33(gmtl.EulerAngleZYXf p0)
   {
      gmtl.Matrix33f result;
      result = gmtlWrappers_makeRotMatrix33__gmtl_EulerAngleZYXf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtlWrappers_makeRotMatrix33__gmtl_EulerAngleZXYf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p0);

   public static gmtl.Matrix33f makeRotMatrix33(gmtl.EulerAngleZXYf p0)
   {
      gmtl.Matrix33f result;
      result = gmtlWrappers_makeRotMatrix33__gmtl_EulerAngleZXYf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtlWrappers_makeRotMatrix33__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Matrix33f makeRotMatrix33(gmtl.Quatf p0)
   {
      gmtl.Matrix33f result;
      result = gmtlWrappers_makeRotMatrix33__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtlWrappers_makeRotMatrix33__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Matrix33d makeRotMatrix33(gmtl.Quatd p0)
   {
      gmtl.Matrix33d result;
      result = gmtlWrappers_makeRotMatrix33__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeRotMatrix44__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Matrix44d makeRotMatrix44(gmtl.Quatd p0)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeRotMatrix44__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeRotMatrix44__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Matrix44f makeRotMatrix44(gmtl.Quatf p0)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeRotMatrix44__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeRotMatrix44__gmtl_AxisAngled1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p0);

   public static gmtl.Matrix44d makeRotMatrix44(gmtl.AxisAngled p0)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeRotMatrix44__gmtl_AxisAngled1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeRotMatrix44__gmtl_AxisAnglef1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p0);

   public static gmtl.Matrix44f makeRotMatrix44(gmtl.AxisAnglef p0)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeRotMatrix44__gmtl_AxisAnglef1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeRotMatrix44__gmtl_EulerAngleZXYf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p0);

   public static gmtl.Matrix44f makeRotMatrix44(gmtl.EulerAngleZXYf p0)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeRotMatrix44__gmtl_EulerAngleZXYf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeRotMatrix44__gmtl_EulerAngleZYXf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p0);

   public static gmtl.Matrix44f makeRotMatrix44(gmtl.EulerAngleZYXf p0)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeRotMatrix44__gmtl_EulerAngleZYXf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeRotMatrix44__gmtl_EulerAngleXYZf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p0);

   public static gmtl.Matrix44f makeRotMatrix44(gmtl.EulerAngleXYZf p0)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeRotMatrix44__gmtl_EulerAngleXYZf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeRotMatrix44__gmtl_Vec3f_gmtl_Vec3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public static gmtl.Matrix44f makeRotMatrix44(gmtl.Vec3f p0, gmtl.Vec3f p1)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeRotMatrix44__gmtl_Vec3f_gmtl_Vec3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtlWrappers_makeRotQuat__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Quatf makeRotQuat(gmtl.Matrix44f p0)
   {
      gmtl.Quatf result;
      result = gmtlWrappers_makeRotQuat__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtlWrappers_makeRotQuat__gmtl_AxisAngled1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p0);

   public static gmtl.Quatd makeRotQuat(gmtl.AxisAngled p0)
   {
      gmtl.Quatd result;
      result = gmtlWrappers_makeRotQuat__gmtl_AxisAngled1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtlWrappers_makeRotQuat__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.Quatd makeRotQuat(gmtl.Matrix44d p0)
   {
      gmtl.Quatd result;
      result = gmtlWrappers_makeRotQuat__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtlWrappers_makeRotQuat__gmtl_AxisAnglef1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p0);

   public static gmtl.Quatf makeRotQuat(gmtl.AxisAnglef p0)
   {
      gmtl.Quatf result;
      result = gmtlWrappers_makeRotQuat__gmtl_AxisAnglef1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtlWrappers_makeRotQuat__gmtl_Vec3f_gmtl_Vec3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public static gmtl.Quatf makeRotQuat(gmtl.Vec3f p0, gmtl.Vec3f p1)
   {
      gmtl.Quatf result;
      result = gmtlWrappers_makeRotQuat__gmtl_Vec3f_gmtl_Vec3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtlWrappers_makeRotQuat__gmtl_Vec3d_gmtl_Vec3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1);

   public static gmtl.Quatd makeRotQuat(gmtl.Vec3d p0, gmtl.Vec3d p1)
   {
      gmtl.Quatd result;
      result = gmtlWrappers_makeRotQuat__gmtl_Vec3d_gmtl_Vec3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))]
   private extern static gmtl.EulerAngleXYZf gmtlWrappers_makeRotEulerAngleXYZ__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.EulerAngleXYZf makeRotEulerAngleXYZ(gmtl.Matrix44f p0)
   {
      gmtl.EulerAngleXYZf result;
      result = gmtlWrappers_makeRotEulerAngleXYZ__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))]
   private extern static gmtl.EulerAngleXYZd gmtlWrappers_makeRotEulerAngleXYZ__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.EulerAngleXYZd makeRotEulerAngleXYZ(gmtl.Matrix44d p0)
   {
      gmtl.EulerAngleXYZd result;
      result = gmtlWrappers_makeRotEulerAngleXYZ__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))]
   private extern static gmtl.EulerAngleZYXf gmtlWrappers_makeRotEulerAngleZYX__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.EulerAngleZYXf makeRotEulerAngleZYX(gmtl.Matrix44f p0)
   {
      gmtl.EulerAngleZYXf result;
      result = gmtlWrappers_makeRotEulerAngleZYX__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))]
   private extern static gmtl.EulerAngleZYXd gmtlWrappers_makeRotEulerAngleZYX__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.EulerAngleZYXd makeRotEulerAngleZYX(gmtl.Matrix44d p0)
   {
      gmtl.EulerAngleZYXd result;
      result = gmtlWrappers_makeRotEulerAngleZYX__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))]
   private extern static gmtl.EulerAngleZXYd gmtlWrappers_makeRotEulerAngleZXY__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.EulerAngleZXYd makeRotEulerAngleZXY(gmtl.Matrix44d p0)
   {
      gmtl.EulerAngleZXYd result;
      result = gmtlWrappers_makeRotEulerAngleZXY__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))]
   private extern static gmtl.EulerAngleZXYf gmtlWrappers_makeRotEulerAngleZXY__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.EulerAngleZXYf makeRotEulerAngleZXY(gmtl.Matrix44f p0)
   {
      gmtl.EulerAngleZXYf result;
      result = gmtlWrappers_makeRotEulerAngleZXY__gmtl_Matrix44f1(p0);
      return result;
   }

#if UNIX
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Matrix33f makeDirCosMatrix33(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2)
   {
      gmtl.Matrix33f result;
      result = gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3);

   public static gmtl.Matrix33f makeDirCosMatrix33(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3)
   {
      gmtl.Matrix33f result;
      result = gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4);

   public static gmtl.Matrix33f makeDirCosMatrix33(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4)
   {
      gmtl.Matrix33f result;
      result = gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p5);

   public static gmtl.Matrix33f makeDirCosMatrix33(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4, gmtl.Vec3f p5)
   {
      gmtl.Matrix33f result;
      result = gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f6(p0, p1, p2, p3, p4, p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.Matrix33d makeDirCosMatrix33(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2)
   {
      gmtl.Matrix33d result;
      result = gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3);

   public static gmtl.Matrix33d makeDirCosMatrix33(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3)
   {
      gmtl.Matrix33d result;
      result = gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4);

   public static gmtl.Matrix33d makeDirCosMatrix33(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4)
   {
      gmtl.Matrix33d result;
      result = gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p5);

   public static gmtl.Matrix33d makeDirCosMatrix33(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4, gmtl.Vec3d p5)
   {
      gmtl.Matrix33d result;
      result = gmtlWrappers_makeDirCosMatrix33__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d6(p0, p1, p2, p3, p4, p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.Matrix44d makeDirCosMatrix44(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3);

   public static gmtl.Matrix44d makeDirCosMatrix44(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4);

   public static gmtl.Matrix44d makeDirCosMatrix44(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p5);

   public static gmtl.Matrix44d makeDirCosMatrix44(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4, gmtl.Vec3d p5)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d6(p0, p1, p2, p3, p4, p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Matrix44f makeDirCosMatrix44(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3);

   public static gmtl.Matrix44f makeDirCosMatrix44(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4);

   public static gmtl.Matrix44f makeDirCosMatrix44(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p5);

   public static gmtl.Matrix44f makeDirCosMatrix44(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4, gmtl.Vec3f p5)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeDirCosMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f6(p0, p1, p2, p3, p4, p5);
      return result;
   }
#endif

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeTransMatrix44__gmtl_Vec3d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0);

   public static gmtl.Matrix44d makeTransMatrix44(gmtl.Vec3d p0)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeTransMatrix44__gmtl_Vec3d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeTransMatrix44__gmtl_Vec3f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public static gmtl.Matrix44f makeTransMatrix44(gmtl.Vec3f p0)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeTransMatrix44__gmtl_Vec3f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtlWrappers_makeTransVec3__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.Vec3d makeTransVec3(gmtl.Matrix44d p0)
   {
      gmtl.Vec3d result;
      result = gmtlWrappers_makeTransVec3__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtlWrappers_makeTransVec3__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Vec3f makeTransVec3(gmtl.Matrix44f p0)
   {
      gmtl.Vec3f result;
      result = gmtlWrappers_makeTransVec3__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeScaleMatrix44__gmtl_Vec3d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0);

   public static gmtl.Matrix44d makeScaleMatrix44(gmtl.Vec3d p0)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeScaleMatrix44__gmtl_Vec3d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeScaleMatrix44__gmtl_Vec3f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public static gmtl.Matrix44f makeScaleMatrix44(gmtl.Vec3f p0)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeScaleMatrix44__gmtl_Vec3f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeScaleMatrix44__gmtl_Vec4d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p0);

   public static gmtl.Matrix44d makeScaleMatrix44(gmtl.Vec4d p0)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeScaleMatrix44__gmtl_Vec4d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeScaleMatrix44__gmtl_Vec4f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0);

   public static gmtl.Matrix44f makeScaleMatrix44(gmtl.Vec4f p0)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeScaleMatrix44__gmtl_Vec4f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeScaleMatrix44__float1(float p0);

   public static gmtl.Matrix44f makeScaleMatrix44(float p0)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeScaleMatrix44__float1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeScaleMatrix44__double1(double p0);

   public static gmtl.Matrix44d makeScaleMatrix44(double p0)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeScaleMatrix44__double1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtlWrappers_makeAxesMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Matrix44f makeAxesMatrix44(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2)
   {
      gmtl.Matrix44f result;
      result = gmtlWrappers_makeAxesMatrix44__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtlWrappers_makeAxesMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.Matrix44d makeAxesMatrix44(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2)
   {
      gmtl.Matrix44d result;
      result = gmtlWrappers_makeAxesMatrix44__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_setRot__gmtl_Quatf_gmtl_EulerAngleXYZf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p1);

   public static gmtl.Quatf setRot(gmtl.Quatf p0, gmtl.EulerAngleXYZf p1)
   {
      gmtl.Quatf result;
      result = gmtl_setRot__gmtl_Quatf_gmtl_EulerAngleXYZf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_setRot__gmtl_Quatf_gmtl_EulerAngleZYXf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p1);

   public static gmtl.Quatf setRot(gmtl.Quatf p0, gmtl.EulerAngleZYXf p1)
   {
      gmtl.Quatf result;
      result = gmtl_setRot__gmtl_Quatf_gmtl_EulerAngleZYXf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_setRot__gmtl_Quatf_gmtl_EulerAngleZXYf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p1);

   public static gmtl.Quatf setRot(gmtl.Quatf p0, gmtl.EulerAngleZXYf p1)
   {
      gmtl.Quatf result;
      result = gmtl_setRot__gmtl_Quatf_gmtl_EulerAngleZXYf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_setRot__gmtl_Quatd_gmtl_AxisAngled2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p1);

   public static gmtl.Quatd setRot(gmtl.Quatd p0, gmtl.AxisAngled p1)
   {
      gmtl.Quatd result;
      result = gmtl_setRot__gmtl_Quatd_gmtl_AxisAngled2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))]
   private extern static gmtl.EulerAngleZYXf gmtl_setRot__gmtl_EulerAngleZYXf_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.EulerAngleZYXf setRot(gmtl.EulerAngleZYXf p0, gmtl.Matrix44f p1)
   {
      gmtl.EulerAngleZYXf result;
      result = gmtl_setRot__gmtl_EulerAngleZYXf_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))]
   private extern static gmtl.EulerAngleXYZf gmtl_setRot__gmtl_EulerAngleXYZf_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.EulerAngleXYZf setRot(gmtl.EulerAngleXYZf p0, gmtl.Matrix44f p1)
   {
      gmtl.EulerAngleXYZf result;
      result = gmtl_setRot__gmtl_EulerAngleXYZf_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_setRot__gmtl_Quatd_gmtl_Vec3d_gmtl_Vec3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.Quatd setRot(gmtl.Quatd p0, gmtl.Vec3d p1, gmtl.Vec3d p2)
   {
      gmtl.Quatd result;
      result = gmtl_setRot__gmtl_Quatd_gmtl_Vec3d_gmtl_Vec3d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_setRot__gmtl_Quatd_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Quatd setRot(gmtl.Quatd p0, gmtl.Matrix44d p1)
   {
      gmtl.Quatd result;
      result = gmtl_setRot__gmtl_Quatd_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_setRot__gmtl_Quatf_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Quatf setRot(gmtl.Quatf p0, gmtl.Matrix44f p1)
   {
      gmtl.Quatf result;
      result = gmtl_setRot__gmtl_Quatf_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_setRot__gmtl_Quatf_gmtl_Vec3f_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Quatf setRot(gmtl.Quatf p0, gmtl.Vec3f p1, gmtl.Vec3f p2)
   {
      gmtl.Quatf result;
      result = gmtl_setRot__gmtl_Quatf_gmtl_Vec3f_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setRot__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Matrix44f setRot(gmtl.Matrix44f p0, gmtl.Vec3f p1, gmtl.Vec3f p2)
   {
      gmtl.Matrix44f result;
      result = gmtl_setRot__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))]
   private extern static gmtl.Coord4fZYX gmtl_setRot__gmtl_Coord4fZYX_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord4fZYX setRot(gmtl.Coord4fZYX p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord4fZYX result;
      result = gmtl_setRot__gmtl_Coord4fZYX_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))]
   private extern static gmtl.Coord4fZXY gmtl_setRot__gmtl_Coord4fZXY_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord4fZXY setRot(gmtl.Coord4fZXY p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord4fZXY result;
      result = gmtl_setRot__gmtl_Coord4fZXY_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setRot__gmtl_Matrix33f_gmtl_EulerAngleZXYf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p1);

   public static gmtl.Matrix33f setRot(gmtl.Matrix33f p0, gmtl.EulerAngleZXYf p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_setRot__gmtl_Matrix33f_gmtl_EulerAngleZXYf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setRot__gmtl_Matrix33f_gmtl_EulerAngleXYZf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p1);

   public static gmtl.Matrix33f setRot(gmtl.Matrix33f p0, gmtl.EulerAngleXYZf p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_setRot__gmtl_Matrix33f_gmtl_EulerAngleXYZf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setRot__gmtl_Matrix33f_gmtl_EulerAngleZYXf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p1);

   public static gmtl.Matrix33f setRot(gmtl.Matrix33f p0, gmtl.EulerAngleZYXf p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_setRot__gmtl_Matrix33f_gmtl_EulerAngleZYXf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setRot__gmtl_Matrix44d_gmtl_EulerAngleXYZd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))] gmtl.EulerAngleXYZd p1);

   public static gmtl.Matrix44d setRot(gmtl.Matrix44d p0, gmtl.EulerAngleXYZd p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_setRot__gmtl_Matrix44d_gmtl_EulerAngleXYZd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setRot__gmtl_Matrix44f_gmtl_EulerAngleZXYf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p1);

   public static gmtl.Matrix44f setRot(gmtl.Matrix44f p0, gmtl.EulerAngleZXYf p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setRot__gmtl_Matrix44f_gmtl_EulerAngleZXYf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setRot__gmtl_Matrix44d_gmtl_EulerAngleZXYd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))] gmtl.EulerAngleZXYd p1);

   public static gmtl.Matrix44d setRot(gmtl.Matrix44d p0, gmtl.EulerAngleZXYd p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_setRot__gmtl_Matrix44d_gmtl_EulerAngleZXYd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setRot__gmtl_Matrix44d_gmtl_EulerAngleZYXd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))] gmtl.EulerAngleZYXd p1);

   public static gmtl.Matrix44d setRot(gmtl.Matrix44d p0, gmtl.EulerAngleZYXd p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_setRot__gmtl_Matrix44d_gmtl_EulerAngleZYXd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setRot__gmtl_Matrix33f_gmtl_AxisAnglef2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p1);

   public static gmtl.Matrix33f setRot(gmtl.Matrix33f p0, gmtl.AxisAnglef p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_setRot__gmtl_Matrix33f_gmtl_AxisAnglef2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setRot__gmtl_Matrix44d_gmtl_AxisAngled2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p1);

   public static gmtl.Matrix44d setRot(gmtl.Matrix44d p0, gmtl.AxisAngled p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_setRot__gmtl_Matrix44d_gmtl_AxisAngled2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setRot__gmtl_Matrix44f_gmtl_EulerAngleZYXf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p1);

   public static gmtl.Matrix44f setRot(gmtl.Matrix44f p0, gmtl.EulerAngleZYXf p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setRot__gmtl_Matrix44f_gmtl_EulerAngleZYXf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setRot__gmtl_Matrix44f_gmtl_EulerAngleXYZf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p1);

   public static gmtl.Matrix44f setRot(gmtl.Matrix44f p0, gmtl.EulerAngleXYZf p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setRot__gmtl_Matrix44f_gmtl_EulerAngleXYZf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setRot__gmtl_Matrix44f_gmtl_AxisAnglef2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p1);

   public static gmtl.Matrix44f setRot(gmtl.Matrix44f p0, gmtl.AxisAnglef p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setRot__gmtl_Matrix44f_gmtl_AxisAnglef2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_setRot__gmtl_Quatf_gmtl_AxisAnglef2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p1);

   public static gmtl.Quatf setRot(gmtl.Quatf p0, gmtl.AxisAnglef p1)
   {
      gmtl.Quatf result;
      result = gmtl_setRot__gmtl_Quatf_gmtl_AxisAnglef2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))]
   private extern static gmtl.Coord4fZYX gmtl_setRot__gmtl_Coord4fZYX_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord4fZYX setRot(gmtl.Coord4fZYX p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord4fZYX result;
      result = gmtl_setRot__gmtl_Coord4fZYX_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))]
   private extern static gmtl.Coord4fZXY gmtl_setRot__gmtl_Coord4fZXY_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord4fZXY setRot(gmtl.Coord4fZXY p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord4fZXY result;
      result = gmtl_setRot__gmtl_Coord4fZXY_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))]
   private extern static gmtl.Coord3fQuat gmtl_setRot__gmtl_Coord3fQuat_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord3fQuat setRot(gmtl.Coord3fQuat p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord3fQuat result;
      result = gmtl_setRot__gmtl_Coord3fQuat_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))]
   private extern static gmtl.Coord4fQuat gmtl_setRot__gmtl_Coord4fQuat_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord4fQuat setRot(gmtl.Coord4fQuat p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord4fQuat result;
      result = gmtl_setRot__gmtl_Coord4fQuat_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))]
   private extern static gmtl.Coord3fXYZ gmtl_setRot__gmtl_Coord3fXYZ_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord3fXYZ setRot(gmtl.Coord3fXYZ p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord3fXYZ result;
      result = gmtl_setRot__gmtl_Coord3fXYZ_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))]
   private extern static gmtl.Coord3fZYX gmtl_setRot__gmtl_Coord3fZYX_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord3fZYX setRot(gmtl.Coord3fZYX p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord3fZYX result;
      result = gmtl_setRot__gmtl_Coord3fZYX_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))]
   private extern static gmtl.Coord3fZXY gmtl_setRot__gmtl_Coord3fZXY_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord3fZXY setRot(gmtl.Coord3fZXY p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord3fZXY result;
      result = gmtl_setRot__gmtl_Coord3fZXY_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))]
   private extern static gmtl.Coord4fXYZ gmtl_setRot__gmtl_Coord4fXYZ_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord4fXYZ setRot(gmtl.Coord4fXYZ p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord4fXYZ result;
      result = gmtl_setRot__gmtl_Coord4fXYZ_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))]
   private extern static gmtl.Coord4fQuat gmtl_setRot__gmtl_Coord4fQuat_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord4fQuat setRot(gmtl.Coord4fQuat p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord4fQuat result;
      result = gmtl_setRot__gmtl_Coord4fQuat_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))]
   private extern static gmtl.Coord3fQuat gmtl_setRot__gmtl_Coord3fQuat_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord3fQuat setRot(gmtl.Coord3fQuat p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord3fQuat result;
      result = gmtl_setRot__gmtl_Coord3fQuat_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))]
   private extern static gmtl.EulerAngleZXYf gmtl_setRot__gmtl_EulerAngleZXYf_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.EulerAngleZXYf setRot(gmtl.EulerAngleZXYf p0, gmtl.Matrix44f p1)
   {
      gmtl.EulerAngleZXYf result;
      result = gmtl_setRot__gmtl_EulerAngleZXYf_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))]
   private extern static gmtl.Coord3fZYX gmtl_setRot__gmtl_Coord3fZYX_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord3fZYX setRot(gmtl.Coord3fZYX p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord3fZYX result;
      result = gmtl_setRot__gmtl_Coord3fZYX_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))]
   private extern static gmtl.Coord3fXYZ gmtl_setRot__gmtl_Coord3fXYZ_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord3fXYZ setRot(gmtl.Coord3fXYZ p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord3fXYZ result;
      result = gmtl_setRot__gmtl_Coord3fXYZ_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))]
   private extern static gmtl.Coord4fXYZ gmtl_setRot__gmtl_Coord4fXYZ_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord4fXYZ setRot(gmtl.Coord4fXYZ p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord4fXYZ result;
      result = gmtl_setRot__gmtl_Coord4fXYZ_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))]
   private extern static gmtl.Coord3fZXY gmtl_setRot__gmtl_Coord3fZXY_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord3fZXY setRot(gmtl.Coord3fZXY p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord3fZXY result;
      result = gmtl_setRot__gmtl_Coord3fZXY_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))]
   private extern static gmtl.AxisAngled gmtl_setRot__gmtl_AxisAngled_gmtl_Quatd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p0,
	gmtl.Quatd p1);

   public static gmtl.AxisAngled setRot(gmtl.AxisAngled p0, gmtl.Quatd p1)
   {
      gmtl.AxisAngled result;
      result = gmtl_setRot__gmtl_AxisAngled_gmtl_Quatd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))]
   private extern static gmtl.AxisAnglef gmtl_setRot__gmtl_AxisAnglef_gmtl_Quatf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p0,
	gmtl.Quatf p1);

   public static gmtl.AxisAnglef setRot(gmtl.AxisAnglef p0, gmtl.Quatf p1)
   {
      gmtl.AxisAnglef result;
      result = gmtl_setRot__gmtl_AxisAnglef_gmtl_Quatf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setRot__gmtl_Matrix44f_gmtl_Quatf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1);

   public static gmtl.Matrix44f setRot(gmtl.Matrix44f p0, gmtl.Quatf p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setRot__gmtl_Matrix44f_gmtl_Quatf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setRot__gmtl_Matrix33f_gmtl_Quatf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1);

   public static gmtl.Matrix33f setRot(gmtl.Matrix33f p0, gmtl.Quatf p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_setRot__gmtl_Matrix33f_gmtl_Quatf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setRot__gmtl_Matrix44d_gmtl_Quatd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1);

   public static gmtl.Matrix44d setRot(gmtl.Matrix44d p0, gmtl.Quatd p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_setRot__gmtl_Matrix44d_gmtl_Quatd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_setRot__gmtl_Matrix33d_gmtl_Quatd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1);

   public static gmtl.Matrix33d setRot(gmtl.Matrix33d p0, gmtl.Quatd p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_setRot__gmtl_Matrix33d_gmtl_Quatd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_makeVec__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Vec3f makeVec(gmtl.Quatf p0)
   {
      gmtl.Vec3f result;
      result = gmtl_makeVec__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_makeVec__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Vec3d makeVec(gmtl.Quatd p0)
   {
      gmtl.Vec3d result;
      result = gmtl_makeVec__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_makeNormal__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf makeNormal(gmtl.Quatf p0)
   {
      gmtl.Quatf result;
      result = gmtl_makeNormal__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))]
   private extern static gmtl.Vec4f gmtl_makeNormal__gmtl_Vec4f1(gmtl.Vec4f p0);

   public static gmtl.Vec4f makeNormal(gmtl.Vec4f p0)
   {
      gmtl.Vec4f result;
      result = gmtl_makeNormal__gmtl_Vec4f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_makeNormal__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd makeNormal(gmtl.Quatd p0)
   {
      gmtl.Quatd result;
      result = gmtl_makeNormal__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_makeNormal__gmtl_Vec3f1(gmtl.Vec3f p0);

   public static gmtl.Vec3f makeNormal(gmtl.Vec3f p0)
   {
      gmtl.Vec3f result;
      result = gmtl_makeNormal__gmtl_Vec3f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_makeNormal__gmtl_Vec3d1(gmtl.Vec3d p0);

   public static gmtl.Vec3d makeNormal(gmtl.Vec3d p0)
   {
      gmtl.Vec3d result;
      result = gmtl_makeNormal__gmtl_Vec3d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))]
   private extern static gmtl.AxisAngled gmtl_makeNormal__gmtl_AxisAngled1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p0);

   public static gmtl.AxisAngled makeNormal(gmtl.AxisAngled p0)
   {
      gmtl.AxisAngled result;
      result = gmtl_makeNormal__gmtl_AxisAngled1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))]
   private extern static gmtl.AxisAnglef gmtl_makeNormal__gmtl_AxisAnglef1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p0);

   public static gmtl.AxisAnglef makeNormal(gmtl.AxisAnglef p0)
   {
      gmtl.AxisAnglef result;
      result = gmtl_makeNormal__gmtl_AxisAnglef1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))]
   private extern static gmtl.Vec3i gmtl_makeCross__gmtl_Vec3i_gmtl_Vec3i2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))] gmtl.Vec3i p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))] gmtl.Vec3i p1);

   public static gmtl.Vec3i makeCross(gmtl.Vec3i p0, gmtl.Vec3i p1)
   {
      gmtl.Vec3i result;
      result = gmtl_makeCross__gmtl_Vec3i_gmtl_Vec3i2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_makeCross__gmtl_Vec3f_gmtl_Vec3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public static gmtl.Vec3f makeCross(gmtl.Vec3f p0, gmtl.Vec3f p1)
   {
      gmtl.Vec3f result;
      result = gmtl_makeCross__gmtl_Vec3f_gmtl_Vec3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_makeCross__gmtl_Vec3d_gmtl_Vec3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1);

   public static gmtl.Vec3d makeCross(gmtl.Vec3d p0, gmtl.Vec3d p1)
   {
      gmtl.Vec3d result;
      result = gmtl_makeCross__gmtl_Vec3d_gmtl_Vec3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))]
   private extern static gmtl.Vec4f gmtl_setTrans__gmtl_Vec4f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Vec4f setTrans(gmtl.Vec4f p0, gmtl.Matrix44f p1)
   {
      gmtl.Vec4f result;
      result = gmtl_setTrans__gmtl_Vec4f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))]
   private extern static gmtl.Vec4f gmtl_setTrans__gmtl_Vec4f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Vec4f setTrans(gmtl.Vec4f p0, gmtl.Matrix33f p1)
   {
      gmtl.Vec4f result;
      result = gmtl_setTrans__gmtl_Vec4f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_setTrans__gmtl_Vec3f_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Vec3f setTrans(gmtl.Vec3f p0, gmtl.Matrix33f p1)
   {
      gmtl.Vec3f result;
      result = gmtl_setTrans__gmtl_Vec3f_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_setTrans__gmtl_Vec3d_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Vec3d setTrans(gmtl.Vec3d p0, gmtl.Matrix44d p1)
   {
      gmtl.Vec3d result;
      result = gmtl_setTrans__gmtl_Vec3d_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setTrans__gmtl_Matrix44d_gmtl_Vec3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1);

   public static gmtl.Matrix44d setTrans(gmtl.Matrix44d p0, gmtl.Vec3d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_setTrans__gmtl_Matrix44d_gmtl_Vec3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setTrans__gmtl_Matrix44f_gmtl_Vec3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public static gmtl.Matrix44f setTrans(gmtl.Matrix44f p0, gmtl.Vec3f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setTrans__gmtl_Matrix44f_gmtl_Vec3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setTrans__gmtl_Matrix44f_gmtl_Vec4f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p1);

   public static gmtl.Matrix44f setTrans(gmtl.Matrix44f p0, gmtl.Vec4f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setTrans__gmtl_Matrix44f_gmtl_Vec4f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setTrans__gmtl_Matrix33f_gmtl_Vec4f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p1);

   public static gmtl.Matrix33f setTrans(gmtl.Matrix33f p0, gmtl.Vec4f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_setTrans__gmtl_Matrix33f_gmtl_Vec4f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setTrans__gmtl_Matrix33f_gmtl_Vec3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public static gmtl.Matrix33f setTrans(gmtl.Matrix33f p0, gmtl.Vec3f p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_setTrans__gmtl_Matrix33f_gmtl_Vec3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_setTrans__gmtl_Vec3f_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Vec3f setTrans(gmtl.Vec3f p0, gmtl.Matrix44f p1)
   {
      gmtl.Vec3f result;
      result = gmtl_setTrans__gmtl_Vec3f_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_setPure__gmtl_Quatd_gmtl_Vec3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1);

   public static gmtl.Quatd setPure(gmtl.Quatd p0, gmtl.Vec3d p1)
   {
      gmtl.Quatd result;
      result = gmtl_setPure__gmtl_Quatd_gmtl_Vec3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_setPure__gmtl_Quatf_gmtl_Vec3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public static gmtl.Quatf setPure(gmtl.Quatf p0, gmtl.Vec3f p1)
   {
      gmtl.Quatf result;
      result = gmtl_setPure__gmtl_Quatf_gmtl_Vec3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_makePure__gmtl_Vec3d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0);

   public static gmtl.Quatd makePure(gmtl.Vec3d p0)
   {
      gmtl.Quatd result;
      result = gmtl_makePure__gmtl_Vec3d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_makePure__gmtl_Vec3f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public static gmtl.Quatf makePure(gmtl.Vec3f p0)
   {
      gmtl.Quatf result;
      result = gmtl_makePure__gmtl_Vec3f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_makeConj__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd makeConj(gmtl.Quatd p0)
   {
      gmtl.Quatd result;
      result = gmtl_makeConj__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_makeConj__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf makeConj(gmtl.Quatf p0)
   {
      gmtl.Quatf result;
      result = gmtl_makeConj__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_makeInvert__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd makeInvert(gmtl.Quatd p0)
   {
      gmtl.Quatd result;
      result = gmtl_makeInvert__gmtl_Quatd1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_makeInvert__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf makeInvert(gmtl.Quatf p0)
   {
      gmtl.Quatf result;
      result = gmtl_makeInvert__gmtl_Quatf1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_makeInvert__gmtl_Matrix33f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f makeInvert(gmtl.Matrix33f p0)
   {
      gmtl.Matrix33f result;
      result = gmtl_makeInvert__gmtl_Matrix33f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_makeInvert__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f makeInvert(gmtl.Matrix44f p0)
   {
      gmtl.Matrix44f result;
      result = gmtl_makeInvert__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_makeInvert__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.Matrix44d makeInvert(gmtl.Matrix44d p0)
   {
      gmtl.Matrix44d result;
      result = gmtl_makeInvert__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_makeInvert__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public static gmtl.Matrix33d makeInvert(gmtl.Matrix33d p0)
   {
      gmtl.Matrix33d result;
      result = gmtl_makeInvert__gmtl_Matrix33d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))]
   private extern static gmtl.AxisAngled gmtl_set__gmtl_AxisAngled_gmtl_Quatd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p0,
	gmtl.Quatd p1);

   public static gmtl.AxisAngled set(gmtl.AxisAngled p0, gmtl.Quatd p1)
   {
      gmtl.AxisAngled result;
      result = gmtl_set__gmtl_AxisAngled_gmtl_Quatd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))]
   private extern static gmtl.AxisAnglef gmtl_set__gmtl_AxisAnglef_gmtl_Quatf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p0,
	gmtl.Quatf p1);

   public static gmtl.AxisAnglef set(gmtl.AxisAnglef p0, gmtl.Quatf p1)
   {
      gmtl.AxisAnglef result;
      result = gmtl_set__gmtl_AxisAnglef_gmtl_Quatf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_set__gmtl_Quatf_gmtl_EulerAngleZYXf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p1);

   public static gmtl.Quatf set(gmtl.Quatf p0, gmtl.EulerAngleZYXf p1)
   {
      gmtl.Quatf result;
      result = gmtl_set__gmtl_Quatf_gmtl_EulerAngleZYXf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_set__gmtl_Quatf_gmtl_EulerAngleZXYf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p1);

   public static gmtl.Quatf set(gmtl.Quatf p0, gmtl.EulerAngleZXYf p1)
   {
      gmtl.Quatf result;
      result = gmtl_set__gmtl_Quatf_gmtl_EulerAngleZXYf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_set__gmtl_Quatf_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Quatf set(gmtl.Quatf p0, gmtl.Matrix44f p1)
   {
      gmtl.Quatf result;
      result = gmtl_set__gmtl_Quatf_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_set__gmtl_Quatd_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.Quatd set(gmtl.Quatd p0, gmtl.Matrix44d p1)
   {
      gmtl.Quatd result;
      result = gmtl_set__gmtl_Quatd_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_set__gmtl_Quatf_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Quatf set(gmtl.Quatf p0, gmtl.Matrix33f p1)
   {
      gmtl.Quatf result;
      result = gmtl_set__gmtl_Quatf_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_set__gmtl_Quatf_gmtl_EulerAngleXYZf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p1);

   public static gmtl.Quatf set(gmtl.Quatf p0, gmtl.EulerAngleXYZf p1)
   {
      gmtl.Quatf result;
      result = gmtl_set__gmtl_Quatf_gmtl_EulerAngleXYZf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_set__gmtl_Quatf_gmtl_AxisAnglef2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p1);

   public static gmtl.Quatf set(gmtl.Quatf p0, gmtl.AxisAnglef p1)
   {
      gmtl.Quatf result;
      result = gmtl_set__gmtl_Quatf_gmtl_AxisAnglef2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord4fXYZ2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord4fXYZ p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord4fXYZ2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord4fZYX2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord4fZYX p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord4fZYX2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))]
   private extern static gmtl.EulerAngleZXYf gmtl_set__gmtl_EulerAngleZXYf_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.EulerAngleZXYf set(gmtl.EulerAngleZXYf p0, gmtl.Matrix44f p1)
   {
      gmtl.EulerAngleZXYf result;
      result = gmtl_set__gmtl_EulerAngleZXYf_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))]
   private extern static gmtl.EulerAngleXYZf gmtl_set__gmtl_EulerAngleXYZf_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.EulerAngleXYZf set(gmtl.EulerAngleXYZf p0, gmtl.Matrix33f p1)
   {
      gmtl.EulerAngleXYZf result;
      result = gmtl_set__gmtl_EulerAngleXYZf_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))]
   private extern static gmtl.EulerAngleXYZd gmtl_set__gmtl_EulerAngleXYZd_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))] gmtl.EulerAngleXYZd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.EulerAngleXYZd set(gmtl.EulerAngleXYZd p0, gmtl.Matrix44d p1)
   {
      gmtl.EulerAngleXYZd result;
      result = gmtl_set__gmtl_EulerAngleXYZd_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))]
   private extern static gmtl.EulerAngleZYXd gmtl_set__gmtl_EulerAngleZYXd_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))] gmtl.EulerAngleZYXd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.EulerAngleZYXd set(gmtl.EulerAngleZYXd p0, gmtl.Matrix44d p1)
   {
      gmtl.EulerAngleZYXd result;
      result = gmtl_set__gmtl_EulerAngleZYXd_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))]
   private extern static gmtl.EulerAngleZXYf gmtl_set__gmtl_EulerAngleZXYf_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.EulerAngleZXYf set(gmtl.EulerAngleZXYf p0, gmtl.Matrix33f p1)
   {
      gmtl.EulerAngleZXYf result;
      result = gmtl_set__gmtl_EulerAngleZXYf_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))]
   private extern static gmtl.EulerAngleZXYd gmtl_set__gmtl_EulerAngleZXYd_gmtl_Matrix44d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))] gmtl.EulerAngleZXYd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1);

   public static gmtl.EulerAngleZXYd set(gmtl.EulerAngleZXYd p0, gmtl.Matrix44d p1)
   {
      gmtl.EulerAngleZXYd result;
      result = gmtl_set__gmtl_EulerAngleZXYd_gmtl_Matrix44d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))]
   private extern static gmtl.EulerAngleXYZf gmtl_set__gmtl_EulerAngleXYZf_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.EulerAngleXYZf set(gmtl.EulerAngleXYZf p0, gmtl.Matrix44f p1)
   {
      gmtl.EulerAngleXYZf result;
      result = gmtl_set__gmtl_EulerAngleXYZf_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_set__gmtl_Matrix44d_gmtl_EulerAngleXYZd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))] gmtl.EulerAngleXYZd p1);

   public static gmtl.Matrix44d set(gmtl.Matrix44d p0, gmtl.EulerAngleXYZd p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_set__gmtl_Matrix44d_gmtl_EulerAngleXYZd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_set__gmtl_Matrix44d_gmtl_EulerAngleZYXd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))] gmtl.EulerAngleZYXd p1);

   public static gmtl.Matrix44d set(gmtl.Matrix44d p0, gmtl.EulerAngleZYXd p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_set__gmtl_Matrix44d_gmtl_EulerAngleZYXd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_EulerAngleZYXf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.EulerAngleZYXf p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_EulerAngleZYXf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_EulerAngleZXYf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.EulerAngleZXYf p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_EulerAngleZXYf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_set__gmtl_Matrix44d_gmtl_AxisAngled2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p1);

   public static gmtl.Matrix44d set(gmtl.Matrix44d p0, gmtl.AxisAngled p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_set__gmtl_Matrix44d_gmtl_AxisAngled2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_EulerAngleXYZf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.EulerAngleXYZf p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_EulerAngleXYZf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_AxisAnglef2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.AxisAnglef p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_AxisAnglef2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_set__gmtl_Matrix33d_gmtl_Quatd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1);

   public static gmtl.Matrix33d set(gmtl.Matrix33d p0, gmtl.Quatd p1)
   {
      gmtl.Matrix33d result;
      result = gmtl_set__gmtl_Matrix33d_gmtl_Quatd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord3fAxisAngle2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fAxisAngleMarshaler))] gmtl.Coord3fAxisAngle p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord3fAxisAngle p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord3fAxisAngle2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord4fQuat2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord4fQuat p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord4fQuat2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_EulerAngleXYZf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.EulerAngleXYZf p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_EulerAngleXYZf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_set__gmtl_Matrix44d_gmtl_EulerAngleZXYd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))] gmtl.EulerAngleZXYd p1);

   public static gmtl.Matrix44d set(gmtl.Matrix44d p0, gmtl.EulerAngleZXYd p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_set__gmtl_Matrix44d_gmtl_EulerAngleZXYd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_EulerAngleZXYf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.EulerAngleZXYf p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_EulerAngleZXYf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_EulerAngleZYXf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.EulerAngleZYXf p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_EulerAngleZYXf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord3fQuat2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord3fQuat p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord3fQuat2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord3fZYX2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord3fZYX p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord3fZYX2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_set__gmtl_Quatd_gmtl_AxisAngled2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p1);

   public static gmtl.Quatd set(gmtl.Quatd p0, gmtl.AxisAngled p1)
   {
      gmtl.Quatd result;
      result = gmtl_set__gmtl_Quatd_gmtl_AxisAngled2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))]
   private extern static gmtl.Coord3fXYZ gmtl_set__gmtl_Coord3fXYZ_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord3fXYZ set(gmtl.Coord3fXYZ p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord3fXYZ result;
      result = gmtl_set__gmtl_Coord3fXYZ_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))]
   private extern static gmtl.Coord3fZYX gmtl_set__gmtl_Coord3fZYX_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord3fZYX set(gmtl.Coord3fZYX p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord3fZYX result;
      result = gmtl_set__gmtl_Coord3fZYX_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))]
   private extern static gmtl.Coord3fZXY gmtl_set__gmtl_Coord3fZXY_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord3fZXY set(gmtl.Coord3fZXY p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord3fZXY result;
      result = gmtl_set__gmtl_Coord3fZXY_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))]
   private extern static gmtl.Coord4fXYZ gmtl_set__gmtl_Coord4fXYZ_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord4fXYZ set(gmtl.Coord4fXYZ p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord4fXYZ result;
      result = gmtl_set__gmtl_Coord4fXYZ_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))]
   private extern static gmtl.Coord3fXYZ gmtl_set__gmtl_Coord3fXYZ_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord3fXYZ set(gmtl.Coord3fXYZ p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord3fXYZ result;
      result = gmtl_set__gmtl_Coord3fXYZ_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))]
   private extern static gmtl.Coord3fZYX gmtl_set__gmtl_Coord3fZYX_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord3fZYX set(gmtl.Coord3fZYX p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord3fZYX result;
      result = gmtl_set__gmtl_Coord3fZYX_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))]
   private extern static gmtl.Coord4fZYX gmtl_set__gmtl_Coord4fZYX_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord4fZYX set(gmtl.Coord4fZYX p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord4fZYX result;
      result = gmtl_set__gmtl_Coord4fZYX_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))]
   private extern static gmtl.Coord4fZXY gmtl_set__gmtl_Coord4fZXY_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord4fZXY set(gmtl.Coord4fZXY p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord4fZXY result;
      result = gmtl_set__gmtl_Coord4fZXY_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))]
   private extern static gmtl.Coord3fZXY gmtl_set__gmtl_Coord3fZXY_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord3fZXY set(gmtl.Coord3fZXY p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord3fZXY result;
      result = gmtl_set__gmtl_Coord3fZXY_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))]
   private extern static gmtl.Coord4fXYZ gmtl_set__gmtl_Coord4fXYZ_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord4fXYZ set(gmtl.Coord4fXYZ p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord4fXYZ result;
      result = gmtl_set__gmtl_Coord4fXYZ_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))]
   private extern static gmtl.EulerAngleZYXf gmtl_set__gmtl_EulerAngleZYXf_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.EulerAngleZYXf set(gmtl.EulerAngleZYXf p0, gmtl.Matrix33f p1)
   {
      gmtl.EulerAngleZYXf result;
      result = gmtl_set__gmtl_EulerAngleZYXf_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))]
   private extern static gmtl.Coord4fZXY gmtl_set__gmtl_Coord4fZXY_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord4fZXY set(gmtl.Coord4fZXY p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord4fZXY result;
      result = gmtl_set__gmtl_Coord4fZXY_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))]
   private extern static gmtl.Coord4fZYX gmtl_set__gmtl_Coord4fZYX_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord4fZYX set(gmtl.Coord4fZYX p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord4fZYX result;
      result = gmtl_set__gmtl_Coord4fZYX_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))]
   private extern static gmtl.Coord4fQuat gmtl_set__gmtl_Coord4fQuat_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord4fQuat set(gmtl.Coord4fQuat p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord4fQuat result;
      result = gmtl_set__gmtl_Coord4fQuat_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))]
   private extern static gmtl.Coord3fQuat gmtl_set__gmtl_Coord3fQuat_gmtl_Matrix33f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Coord3fQuat set(gmtl.Coord3fQuat p0, gmtl.Matrix33f p1)
   {
      gmtl.Coord3fQuat result;
      result = gmtl_set__gmtl_Coord3fQuat_gmtl_Matrix33f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))]
   private extern static gmtl.Coord4fQuat gmtl_set__gmtl_Coord4fQuat_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord4fQuat set(gmtl.Coord4fQuat p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord4fQuat result;
      result = gmtl_set__gmtl_Coord4fQuat_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))]
   private extern static gmtl.Coord3fQuat gmtl_set__gmtl_Coord3fQuat_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Coord3fQuat set(gmtl.Coord3fQuat p0, gmtl.Matrix44f p1)
   {
      gmtl.Coord3fQuat result;
      result = gmtl_set__gmtl_Coord3fQuat_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord3fXYZ2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord3fXYZ p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord3fXYZ2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord3fZXY2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord3fZXY p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord3fZXY2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord4fZYX2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord4fZYX p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord4fZYX2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord4fZXY2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord4fZXY p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord4fZXY2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord3fQuat2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord3fQuat p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord3fQuat2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord4fQuat2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord4fQuat p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord4fQuat2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord4fZXY2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord4fZXY p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord4fZXY2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))]
   private extern static gmtl.EulerAngleZYXf gmtl_set__gmtl_EulerAngleZYXf_gmtl_Matrix44f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.EulerAngleZYXf set(gmtl.EulerAngleZYXf p0, gmtl.Matrix44f p1)
   {
      gmtl.EulerAngleZYXf result;
      result = gmtl_set__gmtl_EulerAngleZYXf_gmtl_Matrix44f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_set__gmtl_Matrix44d_gmtl_Quatd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1);

   public static gmtl.Matrix44d set(gmtl.Matrix44d p0, gmtl.Quatd p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_set__gmtl_Matrix44d_gmtl_Quatd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Quatf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Quatf p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Quatf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Quatf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Quatf p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Quatf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord4fAxisAngle2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fAxisAngleMarshaler))] gmtl.Coord4fAxisAngle p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord4fAxisAngle p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord4fAxisAngle2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_set__gmtl_Matrix44f_gmtl_Coord3fAxisAngle2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fAxisAngleMarshaler))] gmtl.Coord3fAxisAngle p1);

   public static gmtl.Matrix44f set(gmtl.Matrix44f p0, gmtl.Coord3fAxisAngle p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_set__gmtl_Matrix44f_gmtl_Coord3fAxisAngle2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord4fAxisAngle2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fAxisAngleMarshaler))] gmtl.Coord4fAxisAngle p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord4fAxisAngle p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord4fAxisAngle2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord3fZYX2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord3fZYX p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord3fZYX2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord3fXYZ2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord3fXYZ p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord3fXYZ2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord4fXYZ2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord4fXYZ p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord4fXYZ2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_set__gmtl_Matrix33f_gmtl_Coord3fZXY2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p1);

   public static gmtl.Matrix33f set(gmtl.Matrix33f p0, gmtl.Coord3fZXY p1)
   {
      gmtl.Matrix33f result;
      result = gmtl_set__gmtl_Matrix33f_gmtl_Coord3fZXY2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setScale__gmtl_Matrix44d_gmtl_Vec3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1);

   public static gmtl.Matrix44d setScale(gmtl.Matrix44d p0, gmtl.Vec3d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_setScale__gmtl_Matrix44d_gmtl_Vec3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setScale__gmtl_Matrix44f_gmtl_Vec4f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p1);

   public static gmtl.Matrix44f setScale(gmtl.Matrix44f p0, gmtl.Vec4f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setScale__gmtl_Matrix44f_gmtl_Vec4f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setScale__gmtl_Matrix44d_gmtl_Vec4d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p1);

   public static gmtl.Matrix44d setScale(gmtl.Matrix44d p0, gmtl.Vec4d p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_setScale__gmtl_Matrix44d_gmtl_Vec4d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setScale__gmtl_Matrix44f_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	float p1);

   public static gmtl.Matrix44f setScale(gmtl.Matrix44f p0, float p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setScale__gmtl_Matrix44f_float2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setScale__gmtl_Matrix44d_double2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	double p1);

   public static gmtl.Matrix44d setScale(gmtl.Matrix44d p0, double p1)
   {
      gmtl.Matrix44d result;
      result = gmtl_setScale__gmtl_Matrix44d_double2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setScale__gmtl_Matrix44f_gmtl_Vec3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public static gmtl.Matrix44f setScale(gmtl.Matrix44f p0, gmtl.Vec3f p1)
   {
      gmtl.Matrix44f result;
      result = gmtl_setScale__gmtl_Matrix44f_gmtl_Vec3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_makeYRot__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static double makeYRot(gmtl.Matrix44d p0)
   {
      double result;
      result = gmtl_makeYRot__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_makeYRot__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public static double makeYRot(gmtl.Matrix33d p0)
   {
      double result;
      result = gmtl_makeYRot__gmtl_Matrix33d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_makeYRot__gmtl_Matrix33f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static float makeYRot(gmtl.Matrix33f p0)
   {
      float result;
      result = gmtl_makeYRot__gmtl_Matrix33f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_makeYRot__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static float makeYRot(gmtl.Matrix44f p0)
   {
      float result;
      result = gmtl_makeYRot__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_makeXRot__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static float makeXRot(gmtl.Matrix44f p0)
   {
      float result;
      result = gmtl_makeXRot__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_makeXRot__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public static double makeXRot(gmtl.Matrix33d p0)
   {
      double result;
      result = gmtl_makeXRot__gmtl_Matrix33d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_makeXRot__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static double makeXRot(gmtl.Matrix44d p0)
   {
      double result;
      result = gmtl_makeXRot__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_makeXRot__gmtl_Matrix33f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static float makeXRot(gmtl.Matrix33f p0)
   {
      float result;
      result = gmtl_makeXRot__gmtl_Matrix33f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_makeZRot__gmtl_Matrix33f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static float makeZRot(gmtl.Matrix33f p0)
   {
      float result;
      result = gmtl_makeZRot__gmtl_Matrix33f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_makeZRot__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public static double makeZRot(gmtl.Matrix33d p0)
   {
      double result;
      result = gmtl_makeZRot__gmtl_Matrix33d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_makeZRot__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static float makeZRot(gmtl.Matrix44f p0)
   {
      float result;
      result = gmtl_makeZRot__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_makeZRot__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static double makeZRot(gmtl.Matrix44d p0)
   {
      double result;
      result = gmtl_makeZRot__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setDirCos__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3);

   public static gmtl.Matrix44f setDirCos(gmtl.Matrix44f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3)
   {
      gmtl.Matrix44f result;
      result = gmtl_setDirCos__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setDirCos__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4);

   public static gmtl.Matrix44f setDirCos(gmtl.Matrix44f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4)
   {
      gmtl.Matrix44f result;
      result = gmtl_setDirCos__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setDirCos__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p5);

   public static gmtl.Matrix44f setDirCos(gmtl.Matrix44f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4, gmtl.Vec3f p5)
   {
      gmtl.Matrix44f result;
      result = gmtl_setDirCos__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f6(p0, p1, p2, p3, p4, p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setDirCos__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f7([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p5,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p6);

   public static gmtl.Matrix44f setDirCos(gmtl.Matrix44f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4, gmtl.Vec3f p5, gmtl.Vec3f p6)
   {
      gmtl.Matrix44f result;
      result = gmtl_setDirCos__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f7(p0, p1, p2, p3, p4, p5, p6);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setDirCos__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3);

   public static gmtl.Matrix44d setDirCos(gmtl.Matrix44d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3)
   {
      gmtl.Matrix44d result;
      result = gmtl_setDirCos__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setDirCos__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4);

   public static gmtl.Matrix44d setDirCos(gmtl.Matrix44d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4)
   {
      gmtl.Matrix44d result;
      result = gmtl_setDirCos__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setDirCos__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p5);

   public static gmtl.Matrix44d setDirCos(gmtl.Matrix44d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4, gmtl.Vec3d p5)
   {
      gmtl.Matrix44d result;
      result = gmtl_setDirCos__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d6(p0, p1, p2, p3, p4, p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setDirCos__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d7([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p5,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p6);

   public static gmtl.Matrix44d setDirCos(gmtl.Matrix44d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4, gmtl.Vec3d p5, gmtl.Vec3d p6)
   {
      gmtl.Matrix44d result;
      result = gmtl_setDirCos__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d7(p0, p1, p2, p3, p4, p5, p6);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setDirCos__gmtl_Matrix33f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3);

   public static gmtl.Matrix33f setDirCos(gmtl.Matrix33f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3)
   {
      gmtl.Matrix33f result;
      result = gmtl_setDirCos__gmtl_Matrix33f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setDirCos__gmtl_Matrix33f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4);

   public static gmtl.Matrix33f setDirCos(gmtl.Matrix33f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4)
   {
      gmtl.Matrix33f result;
      result = gmtl_setDirCos__gmtl_Matrix33f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setDirCos__gmtl_Matrix33f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p5);

   public static gmtl.Matrix33f setDirCos(gmtl.Matrix33f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4, gmtl.Vec3f p5)
   {
      gmtl.Matrix33f result;
      result = gmtl_setDirCos__gmtl_Matrix33f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f6(p0, p1, p2, p3, p4, p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_setDirCos__gmtl_Matrix33f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f7([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p5,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p6);

   public static gmtl.Matrix33f setDirCos(gmtl.Matrix33f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3, gmtl.Vec3f p4, gmtl.Vec3f p5, gmtl.Vec3f p6)
   {
      gmtl.Matrix33f result;
      result = gmtl_setDirCos__gmtl_Matrix33f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f7(p0, p1, p2, p3, p4, p5, p6);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_setDirCos__gmtl_Matrix33d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3);

   public static gmtl.Matrix33d setDirCos(gmtl.Matrix33d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3)
   {
      gmtl.Matrix33d result;
      result = gmtl_setDirCos__gmtl_Matrix33d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_setDirCos__gmtl_Matrix33d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4);

   public static gmtl.Matrix33d setDirCos(gmtl.Matrix33d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4)
   {
      gmtl.Matrix33d result;
      result = gmtl_setDirCos__gmtl_Matrix33d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_setDirCos__gmtl_Matrix33d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p5);

   public static gmtl.Matrix33d setDirCos(gmtl.Matrix33d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4, gmtl.Vec3d p5)
   {
      gmtl.Matrix33d result;
      result = gmtl_setDirCos__gmtl_Matrix33d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d6(p0, p1, p2, p3, p4, p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_setDirCos__gmtl_Matrix33d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d7([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p5,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p6);

   public static gmtl.Matrix33d setDirCos(gmtl.Matrix33d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3, gmtl.Vec3d p4, gmtl.Vec3d p5, gmtl.Vec3d p6)
   {
      gmtl.Matrix33d result;
      result = gmtl_setDirCos__gmtl_Matrix33d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d7(p0, p1, p2, p3, p4, p5, p6);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setAxes__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3);

   public static gmtl.Matrix44f setAxes(gmtl.Matrix44f p0, gmtl.Vec3f p1, gmtl.Vec3f p2, gmtl.Vec3f p3)
   {
      gmtl.Matrix44f result;
      result = gmtl_setAxes__gmtl_Matrix44f_gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setAxes__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3);

   public static gmtl.Matrix44d setAxes(gmtl.Matrix44d p0, gmtl.Vec3d p1, gmtl.Vec3d p2, gmtl.Vec3d p3)
   {
      gmtl.Matrix44d result;
      result = gmtl_setAxes__gmtl_Matrix44d_gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d4(p0, p1, p2, p3);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_makeTranspose__gmtl_Matrix33f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f makeTranspose(gmtl.Matrix33f p0)
   {
      gmtl.Matrix33f result;
      result = gmtl_makeTranspose__gmtl_Matrix33f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_makeTranspose__gmtl_Matrix44f1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f makeTranspose(gmtl.Matrix44f p0)
   {
      gmtl.Matrix44f result;
      result = gmtl_makeTranspose__gmtl_Matrix44f1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))]
   private extern static gmtl.Matrix33d gmtl_makeTranspose__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public static gmtl.Matrix33d makeTranspose(gmtl.Matrix33d p0)
   {
      gmtl.Matrix33d result;
      result = gmtl_makeTranspose__gmtl_Matrix33d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_makeTranspose__gmtl_Matrix44d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public static gmtl.Matrix44d makeTranspose(gmtl.Matrix44d p0)
   {
      gmtl.Matrix44d result;
      result = gmtl_makeTranspose__gmtl_Matrix44d1(p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setFrustum__gmtl_Matrix44d_double_double_double_double_double_double7([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	double p1,
	double p2,
	double p3,
	double p4,
	double p5,
	double p6);

   public static gmtl.Matrix44d setFrustum(gmtl.Matrix44d p0, double p1, double p2, double p3, double p4, double p5, double p6)
   {
      gmtl.Matrix44d result;
      result = gmtl_setFrustum__gmtl_Matrix44d_double_double_double_double_double_double7(p0, p1, p2, p3, p4, p5, p6);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setFrustum__gmtl_Matrix44f_float_float_float_float_float_float7([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5,
	float p6);

   public static gmtl.Matrix44f setFrustum(gmtl.Matrix44f p0, float p1, float p2, float p3, float p4, float p5, float p6)
   {
      gmtl.Matrix44f result;
      result = gmtl_setFrustum__gmtl_Matrix44f_float_float_float_float_float_float7(p0, p1, p2, p3, p4, p5, p6);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))]
   private extern static gmtl.Matrix44d gmtl_setPerspective__gmtl_Matrix44d_double_double_double_double5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0,
	double p1,
	double p2,
	double p3,
	double p4);

   public static gmtl.Matrix44d setPerspective(gmtl.Matrix44d p0, double p1, double p2, double p3, double p4)
   {
      gmtl.Matrix44d result;
      result = gmtl_setPerspective__gmtl_Matrix44d_double_double_double_double5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_setPerspective__gmtl_Matrix44f_float_float_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	float p1,
	float p2,
	float p3,
	float p4);

   public static gmtl.Matrix44f setPerspective(gmtl.Matrix44f p0, float p1, float p2, float p3, float p4)
   {
      gmtl.Matrix44f result;
      result = gmtl_setPerspective__gmtl_Matrix44f_float_float_float_float5(p0, p1, p2, p3, p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isInVolume__gmtl_Sphered_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static bool isInVolume(gmtl.Sphered p0, gmtl.Point3d p1)
   {
      bool result;
      result = gmtl_isInVolume__gmtl_Sphered_gmtl_Point3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isInVolume__gmtl_Spheref_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static bool isInVolume(gmtl.Spheref p0, gmtl.Point3f p1)
   {
      bool result;
      result = gmtl_isInVolume__gmtl_Spheref_gmtl_Point3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isInVolume__gmtl_Sphered_gmtl_Sphered2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p1);

   public static bool isInVolume(gmtl.Sphered p0, gmtl.Sphered p1)
   {
      bool result;
      result = gmtl_isInVolume__gmtl_Sphered_gmtl_Sphered2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isInVolume__gmtl_Spheref_gmtl_Spheref2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p1);

   public static bool isInVolume(gmtl.Spheref p0, gmtl.Spheref p1)
   {
      bool result;
      result = gmtl_isInVolume__gmtl_Spheref_gmtl_Spheref2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_extendVolume__gmtl_AABoxf_gmtl_AABoxf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p1);

   public static void extendVolume(gmtl.AABoxf p0, gmtl.AABoxf p1)
   {
      gmtl_extendVolume__gmtl_AABoxf_gmtl_AABoxf2(p0, p1);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_extendVolume__gmtl_AABoxd_gmtl_AABoxd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p1);

   public static void extendVolume(gmtl.AABoxd p0, gmtl.AABoxd p1)
   {
      gmtl_extendVolume__gmtl_AABoxd_gmtl_AABoxd2(p0, p1);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_extendVolume__gmtl_AABoxf_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static void extendVolume(gmtl.AABoxf p0, gmtl.Point3f p1)
   {
      gmtl_extendVolume__gmtl_AABoxf_gmtl_Point3f2(p0, p1);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_extendVolume__gmtl_AABoxd_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static void extendVolume(gmtl.AABoxd p0, gmtl.Point3d p1)
   {
      gmtl_extendVolume__gmtl_AABoxd_gmtl_Point3d2(p0, p1);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_extendVolume__gmtl_Spheref_gmtl_Spheref2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p1);

   public static void extendVolume(gmtl.Spheref p0, gmtl.Spheref p1)
   {
      gmtl_extendVolume__gmtl_Spheref_gmtl_Spheref2(p0, p1);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_extendVolume__gmtl_Sphered_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static void extendVolume(gmtl.Sphered p0, gmtl.Point3d p1)
   {
      gmtl_extendVolume__gmtl_Sphered_gmtl_Point3d2(p0, p1);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_extendVolume__gmtl_Spheref_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static void extendVolume(gmtl.Spheref p0, gmtl.Point3f p1)
   {
      gmtl_extendVolume__gmtl_Spheref_gmtl_Point3f2(p0, p1);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_extendVolume__gmtl_Sphered_gmtl_Sphered2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p1);

   public static void extendVolume(gmtl.Sphered p0, gmtl.Sphered p1)
   {
      gmtl_extendVolume__gmtl_Sphered_gmtl_Sphered2(p0, p1);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isOnVolume__gmtl_Sphered_gmtl_Point3d_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1,
	double p2);

   public static bool isOnVolume(gmtl.Sphered p0, gmtl.Point3d p1, double p2)
   {
      bool result;
      result = gmtl_isOnVolume__gmtl_Sphered_gmtl_Point3d_double3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isOnVolume__gmtl_Spheref_gmtl_Point3f_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1,
	float p2);

   public static bool isOnVolume(gmtl.Spheref p0, gmtl.Point3f p1, float p2)
   {
      bool result;
      result = gmtl_isOnVolume__gmtl_Spheref_gmtl_Point3f_float3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Trif_gmtl_Rayf_float_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TrifMarshaler))] gmtl.Trif p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RayfMarshaler))] gmtl.Rayf p1,
	ref float p2,
	ref float p3,
	ref float p4);

   public static bool intersect(gmtl.Trif p0, gmtl.Rayf p1, ref float p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersect__gmtl_Trif_gmtl_Rayf_float_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Trif_gmtl_LineSegf_float_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TrifMarshaler))] gmtl.Trif p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p1,
	ref float p2,
	ref float p3,
	ref float p4);

   public static bool intersect(gmtl.Trif p0, gmtl.LineSegf p1, ref float p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersect__gmtl_Trif_gmtl_LineSegf_float_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Trid_gmtl_LineSegd_float_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TridMarshaler))] gmtl.Trid p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p1,
	ref float p2,
	ref float p3,
	ref float p4);

   public static bool intersect(gmtl.Trid p0, gmtl.LineSegd p1, ref float p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersect__gmtl_Trid_gmtl_LineSegd_float_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Sphered_gmtl_AABoxd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p1);

   public static bool intersect(gmtl.Sphered p0, gmtl.AABoxd p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_Sphered_gmtl_AABoxd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Spheref_gmtl_AABoxf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p1);

   public static bool intersect(gmtl.Spheref p0, gmtl.AABoxf p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_Spheref_gmtl_AABoxf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Spheref_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static bool intersect(gmtl.Spheref p0, gmtl.Point3f p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_Spheref_gmtl_Point3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_AABoxf_gmtl_Vec3f_gmtl_AABoxf_gmtl_Vec3f_float_float6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	ref float p4,
	ref float p5);

   public static bool intersect(gmtl.AABoxf p0, gmtl.Vec3f p1, gmtl.AABoxf p2, gmtl.Vec3f p3, ref float p4, ref float p5)
   {
      bool result;
      result = gmtl_intersect__gmtl_AABoxf_gmtl_Vec3f_gmtl_AABoxf_gmtl_Vec3f_float_float6(p0, p1, p2, p3, ref p4, ref p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_AABoxd_gmtl_Vec3d_gmtl_AABoxd_gmtl_Vec3d_double_double6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	ref double p4,
	ref double p5);

   public static bool intersect(gmtl.AABoxd p0, gmtl.Vec3d p1, gmtl.AABoxd p2, gmtl.Vec3d p3, ref double p4, ref double p5)
   {
      bool result;
      result = gmtl_intersect__gmtl_AABoxd_gmtl_Vec3d_gmtl_AABoxd_gmtl_Vec3d_double_double6(p0, p1, p2, p3, ref p4, ref p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_AABoxf_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static bool intersect(gmtl.AABoxf p0, gmtl.Point3f p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_AABoxf_gmtl_Point3f2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_AABoxd_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static bool intersect(gmtl.AABoxd p0, gmtl.Point3d p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_AABoxd_gmtl_Point3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_AABoxf_gmtl_Spheref2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p1);

   public static bool intersect(gmtl.AABoxf p0, gmtl.Spheref p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_AABoxf_gmtl_Spheref2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_AABoxd_gmtl_Sphered2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p1);

   public static bool intersect(gmtl.AABoxd p0, gmtl.Sphered p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_AABoxd_gmtl_Sphered2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Spheref_gmtl_Vec3f_gmtl_Spheref_gmtl_Vec3f_float_float6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p3,
	ref float p4,
	ref float p5);

   public static bool intersect(gmtl.Spheref p0, gmtl.Vec3f p1, gmtl.Spheref p2, gmtl.Vec3f p3, ref float p4, ref float p5)
   {
      bool result;
      result = gmtl_intersect__gmtl_Spheref_gmtl_Vec3f_gmtl_Spheref_gmtl_Vec3f_float_float6(p0, p1, p2, p3, ref p4, ref p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Sphered_gmtl_Vec3d_gmtl_Sphered_gmtl_Vec3d_double_double6([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p3,
	ref double p4,
	ref double p5);

   public static bool intersect(gmtl.Sphered p0, gmtl.Vec3d p1, gmtl.Sphered p2, gmtl.Vec3d p3, ref double p4, ref double p5)
   {
      bool result;
      result = gmtl_intersect__gmtl_Sphered_gmtl_Vec3d_gmtl_Sphered_gmtl_Vec3d_double_double6(p0, p1, p2, p3, ref p4, ref p5);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_AABoxf_gmtl_AABoxf2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p1);

   public static bool intersect(gmtl.AABoxf p0, gmtl.AABoxf p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_AABoxf_gmtl_AABoxf2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_AABoxd_gmtl_AABoxd2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p1);

   public static bool intersect(gmtl.AABoxd p0, gmtl.AABoxd p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_AABoxd_gmtl_AABoxd2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Planef_gmtl_Rayf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RayfMarshaler))] gmtl.Rayf p1,
	ref float p2);

   public static bool intersect(gmtl.Planef p0, gmtl.Rayf p1, ref float p2)
   {
      bool result;
      result = gmtl_intersect__gmtl_Planef_gmtl_Rayf_float3(p0, p1, ref p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Planed_gmtl_Rayd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RaydMarshaler))] gmtl.Rayd p1,
	ref double p2);

   public static bool intersect(gmtl.Planed p0, gmtl.Rayd p1, ref double p2)
   {
      bool result;
      result = gmtl_intersect__gmtl_Planed_gmtl_Rayd_double3(p0, p1, ref p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Planed_gmtl_LineSegd_double3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p1,
	ref double p2);

   public static bool intersect(gmtl.Planed p0, gmtl.LineSegd p1, ref double p2)
   {
      bool result;
      result = gmtl_intersect__gmtl_Planed_gmtl_LineSegd_double3(p0, p1, ref p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Planef_gmtl_LineSegf_float3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p1,
	ref float p2);

   public static bool intersect(gmtl.Planef p0, gmtl.LineSegf p1, ref float p2)
   {
      bool result;
      result = gmtl_intersect__gmtl_Planef_gmtl_LineSegf_float3(p0, p1, ref p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Trid_gmtl_Rayd_float_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TridMarshaler))] gmtl.Trid p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RaydMarshaler))] gmtl.Rayd p1,
	ref float p2,
	ref float p3,
	ref float p4);

   public static bool intersect(gmtl.Trid p0, gmtl.Rayd p1, ref float p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersect__gmtl_Trid_gmtl_Rayd_float_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Sphered_gmtl_LineSegd_int_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p1,
	ref int p2,
	ref float p3,
	ref float p4);

   public static bool intersect(gmtl.Sphered p0, gmtl.LineSegd p1, ref int p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersect__gmtl_Sphered_gmtl_LineSegd_int_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Spheref_gmtl_LineSegf_int_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p1,
	ref int p2,
	ref float p3,
	ref float p4);

   public static bool intersect(gmtl.Spheref p0, gmtl.LineSegf p1, ref int p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersect__gmtl_Spheref_gmtl_LineSegf_int_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Sphered_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static bool intersect(gmtl.Sphered p0, gmtl.Point3d p1)
   {
      bool result;
      result = gmtl_intersect__gmtl_Sphered_gmtl_Point3d2(p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Sphered_gmtl_Rayd_int_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RaydMarshaler))] gmtl.Rayd p1,
	ref int p2,
	ref float p3,
	ref float p4);

   public static bool intersect(gmtl.Sphered p0, gmtl.Rayd p1, ref int p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersect__gmtl_Sphered_gmtl_Rayd_int_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersect__gmtl_Spheref_gmtl_Rayf_int_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RayfMarshaler))] gmtl.Rayf p1,
	ref int p2,
	ref float p3,
	ref float p4);

   public static bool intersect(gmtl.Spheref p0, gmtl.Rayf p1, ref int p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersect__gmtl_Spheref_gmtl_Rayf_int_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersectVolume__gmtl_Spheref_gmtl_Rayf_int_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RayfMarshaler))] gmtl.Rayf p1,
	ref int p2,
	ref float p3,
	ref float p4);

   public static bool intersectVolume(gmtl.Spheref p0, gmtl.Rayf p1, ref int p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersectVolume__gmtl_Spheref_gmtl_Rayf_int_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersectVolume__gmtl_Sphered_gmtl_Rayd_int_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RaydMarshaler))] gmtl.Rayd p1,
	ref int p2,
	ref float p3,
	ref float p4);

   public static bool intersectVolume(gmtl.Sphered p0, gmtl.Rayd p1, ref int p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersectVolume__gmtl_Sphered_gmtl_Rayd_int_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersectVolume__gmtl_Spheref_gmtl_LineSegf_int_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p1,
	ref int p2,
	ref float p3,
	ref float p4);

   public static bool intersectVolume(gmtl.Spheref p0, gmtl.LineSegf p1, ref int p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersectVolume__gmtl_Spheref_gmtl_LineSegf_int_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_intersectVolume__gmtl_Sphered_gmtl_LineSegd_int_float_float5([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p1,
	ref int p2,
	ref float p3,
	ref float p4);

   public static bool intersectVolume(gmtl.Sphered p0, gmtl.LineSegd p1, ref int p2, ref float p3, ref float p4)
   {
      bool result;
      result = gmtl_intersectVolume__gmtl_Sphered_gmtl_LineSegd_int_float_float5(p0, p1, ref p2, ref p3, ref p4);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))]
   private extern static gmtl.Vec4d gmtl_xform__gmtl_Vec4d_gmtl_Matrix44d_gmtl_Vec4d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p2);

   public static gmtl.Vec4d xform(gmtl.Vec4d p0, gmtl.Matrix44d p1, gmtl.Vec4d p2)
   {
      gmtl.Vec4d result;
      result = gmtl_xform__gmtl_Vec4d_gmtl_Matrix44d_gmtl_Vec4d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_xform__gmtl_Vec3d_gmtl_Matrix44d_gmtl_Vec3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.Vec3d xform(gmtl.Vec3d p0, gmtl.Matrix44d p1, gmtl.Vec3d p2)
   {
      gmtl.Vec3d result;
      result = gmtl_xform__gmtl_Vec3d_gmtl_Matrix44d_gmtl_Vec3d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_xform__gmtl_Vec3d_gmtl_Matrix33d_gmtl_Vec3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.Vec3d xform(gmtl.Vec3d p0, gmtl.Matrix33d p1, gmtl.Vec3d p2)
   {
      gmtl.Vec3d result;
      result = gmtl_xform__gmtl_Vec3d_gmtl_Matrix33d_gmtl_Vec3d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))]
   private extern static gmtl.Vec4f gmtl_xform__gmtl_Vec4f_gmtl_Matrix44f_gmtl_Vec4f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p2);

   public static gmtl.Vec4f xform(gmtl.Vec4f p0, gmtl.Matrix44f p1, gmtl.Vec4f p2)
   {
      gmtl.Vec4f result;
      result = gmtl_xform__gmtl_Vec4f_gmtl_Matrix44f_gmtl_Vec4f3(p0, p1, p2);
      return result;
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))]
   private extern static gmtl.Vec4d gmtl_xform__gmtl_Vec4d_gmtl_Matrix44d_gmtl_Vec4d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p2);

   public static gmtl.Vec4d xform(gmtl.Vec4d p0, gmtl.Matrix44d p1, gmtl.Vec4d p2)
   {
      gmtl.Vec4d result;
      result = gmtl_xform__gmtl_Vec4d_gmtl_Matrix44d_gmtl_Vec4d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_xform__gmtl_Vec3d_gmtl_Matrix33d_gmtl_Vec3d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.Vec3d xform(gmtl.Vec3d p0, gmtl.Matrix33d p1, gmtl.Vec3d p2)
   {
      gmtl.Vec3d result;
      result = gmtl_xform__gmtl_Vec3d_gmtl_Matrix33d_gmtl_Vec3d3(p0, p1, p2);
      return result;
   }
*/

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_xform__gmtl_Vec3f_gmtl_Matrix33f_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Vec3f xform(gmtl.Vec3f p0, gmtl.Matrix33f p1, gmtl.Vec3f p2)
   {
      gmtl.Vec3f result;
      result = gmtl_xform__gmtl_Vec3f_gmtl_Matrix33f_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point4dMarshaler))]
   private extern static gmtl.Point4d gmtl_xform__gmtl_Point4d_gmtl_Matrix44d_gmtl_Point4d3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point4dMarshaler))] gmtl.Point4d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point4dMarshaler))] gmtl.Point4d p2);

   public static gmtl.Point4d xform(gmtl.Point4d p0, gmtl.Matrix44d p1, gmtl.Point4d p2)
   {
      gmtl.Point4d result;
      result = gmtl_xform__gmtl_Point4d_gmtl_Matrix44d_gmtl_Point4d3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point4fMarshaler))]
   private extern static gmtl.Point4f gmtl_xform__gmtl_Point4f_gmtl_Matrix44f_gmtl_Point4f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point4fMarshaler))] gmtl.Point4f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point4fMarshaler))] gmtl.Point4f p2);

   public static gmtl.Point4f xform(gmtl.Point4f p0, gmtl.Matrix44f p1, gmtl.Point4f p2)
   {
      gmtl.Point4f result;
      result = gmtl_xform__gmtl_Point4f_gmtl_Matrix44f_gmtl_Point4f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.RayfMarshaler))]
   private extern static gmtl.Rayf gmtl_xform__gmtl_Rayf_gmtl_Matrix44f_gmtl_Rayf3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RayfMarshaler))] gmtl.Rayf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RayfMarshaler))] gmtl.Rayf p2);

   public static gmtl.Rayf xform(gmtl.Rayf p0, gmtl.Matrix44f p1, gmtl.Rayf p2)
   {
      gmtl.Rayf result;
      result = gmtl_xform__gmtl_Rayf_gmtl_Matrix44f_gmtl_Rayf3(p0, p1, p2);
      return result;
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_xform__gmtl_Vec3f_gmtl_Matrix33f_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Vec3f xform(gmtl.Vec3f p0, gmtl.Matrix33f p1, gmtl.Vec3f p2)
   {
      gmtl.Vec3f result;
      result = gmtl_xform__gmtl_Vec3f_gmtl_Matrix33f_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))]
   private extern static gmtl.Vec4f gmtl_xform__gmtl_Vec4f_gmtl_Matrix44f_gmtl_Vec4f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p2);

   public static gmtl.Vec4f xform(gmtl.Vec4f p0, gmtl.Matrix44f p1, gmtl.Vec4f p2)
   {
      gmtl.Vec4f result;
      result = gmtl_xform__gmtl_Vec4f_gmtl_Matrix44f_gmtl_Vec4f3(p0, p1, p2);
      return result;
   }
*/

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_xform__gmtl_Vec3f_gmtl_Matrix44f_gmtl_Vec3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Vec3f xform(gmtl.Vec3f p0, gmtl.Matrix44f p1, gmtl.Vec3f p2)
   {
      gmtl.Vec3f result;
      result = gmtl_xform__gmtl_Vec3f_gmtl_Matrix44f_gmtl_Vec3f3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.RaydMarshaler))]
   private extern static gmtl.Rayd gmtl_xform__gmtl_Rayd_gmtl_Matrix44d_gmtl_Rayd3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RaydMarshaler))] gmtl.Rayd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RaydMarshaler))] gmtl.Rayd p2);

   public static gmtl.Rayd xform(gmtl.Rayd p0, gmtl.Matrix44d p1, gmtl.Rayd p2)
   {
      gmtl.Rayd result;
      result = gmtl_xform__gmtl_Rayd_gmtl_Matrix44d_gmtl_Rayd3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))]
   private extern static gmtl.LineSegf gmtl_xform__gmtl_LineSegf_gmtl_Matrix44f_gmtl_LineSegf3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p2);

   public static gmtl.LineSegf xform(gmtl.LineSegf p0, gmtl.Matrix44f p1, gmtl.LineSegf p2)
   {
      gmtl.LineSegf result;
      result = gmtl_xform__gmtl_LineSegf_gmtl_Matrix44f_gmtl_LineSegf3(p0, p1, p2);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))]
   private extern static gmtl.LineSegd gmtl_xform__gmtl_LineSegd_gmtl_Matrix44d_gmtl_LineSegd3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p2);

   public static gmtl.LineSegd xform(gmtl.LineSegd p0, gmtl.Matrix44d p1, gmtl.LineSegd p2)
   {
      gmtl.LineSegd result;
      result = gmtl_xform__gmtl_LineSegd_gmtl_Matrix44d_gmtl_LineSegd3(p0, p1, p2);
      return result;
   }

}


} // namespace gmtl
