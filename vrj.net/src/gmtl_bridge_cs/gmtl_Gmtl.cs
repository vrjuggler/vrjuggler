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

// Generated from Revision: 1.3 of RCSfile: free_types_cs.tmpl,v
using System;
using System.Runtime.InteropServices;

namespace gmtl
{

public sealed abstract class Gmtl
{
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
   private extern static gmtl.Quatd gmtl_add__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2);

   public static gmtl.Quatd add(gmtl.Quatd p0, gmtl.Quatd p1, gmtl.Quatd p2)
   {
      
      
      
      gmtl.Quatd result;
      result = gmtl_add__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_add__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p2);

   public static gmtl.Matrix44f add(gmtl.Matrix44f p0, gmtl.Matrix44f p1, gmtl.Matrix44f p2)
   {
      
      
      
      gmtl.Matrix44f result;
      result = gmtl_add__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_add__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p2);

   public static gmtl.Matrix33f add(gmtl.Matrix33f p0, gmtl.Matrix33f p1, gmtl.Matrix33f p2)
   {
      
      
      
      gmtl.Matrix33f result;
      result = gmtl_add__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_add__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2);

   public static gmtl.Quatf add(gmtl.Quatf p0, gmtl.Quatf p1, gmtl.Quatf p2)
   {
      
      
      
      gmtl.Quatf result;
      result = gmtl_add__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3dMarshaler))]
   private extern static gmtl.Point3d gmtl_center__gmtl_Trid([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TridMarshaler))] gmtl.Trid p0);

   public static gmtl.Point3d center(gmtl.Trid p0)
   {
      
      gmtl.Point3d result;
      result = gmtl_center__gmtl_Trid(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3fMarshaler))]
   private extern static gmtl.Point3f gmtl_center__gmtl_Trif([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TrifMarshaler))] gmtl.Trif p0);

   public static gmtl.Point3f center(gmtl.Trif p0)
   {
      
      gmtl.Point3f result;
      result = gmtl_center__gmtl_Trif(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3iMarshaler))]
   private extern static gmtl.Point3i gmtl_center__gmtl_Trii([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TriiMarshaler))] gmtl.Trii p0);

   public static gmtl.Point3i center(gmtl.Trii p0)
   {
      
      gmtl.Point3i result;
      result = gmtl_center__gmtl_Trii(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_conj__gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd conj(gmtl.Quatd p0)
   {
      
      gmtl.Quatd result;
      result = gmtl_conj__gmtl_Quatd(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_conj__gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf conj(gmtl.Quatf p0)
   {
      
      gmtl.Quatf result;
      result = gmtl_conj__gmtl_Quatf(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_cross__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.Vec3f cross(gmtl.Vec3f p0, gmtl.Vec3f p1, gmtl.Vec3f p2)
   {
      
      
      
      gmtl.Vec3f result;
      result = gmtl_cross__gmtl_Vec3f_gmtl_Vec3f_gmtl_Vec3f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))]
   private extern static gmtl.Vec3i gmtl_cross__gmtl_Vec3i_gmtl_Vec3i_gmtl_Vec3i([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))] gmtl.Vec3i p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))] gmtl.Vec3i p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3iMarshaler))] gmtl.Vec3i p2);

   public static gmtl.Vec3i cross(gmtl.Vec3i p0, gmtl.Vec3i p1, gmtl.Vec3i p2)
   {
      
      
      
      gmtl.Vec3i result;
      result = gmtl_cross__gmtl_Vec3i_gmtl_Vec3i_gmtl_Vec3i(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_cross__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.Vec3d cross(gmtl.Vec3d p0, gmtl.Vec3d p1, gmtl.Vec3d p2)
   {
      
      
      
      gmtl.Vec3d result;
      result = gmtl_cross__gmtl_Vec3d_gmtl_Vec3d_gmtl_Vec3d(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_distance__gmtl_LineSegd_gmtl_Point3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static double distance(gmtl.LineSegd p0, gmtl.Point3d p1)
   {
      
      
      double result;
      result = gmtl_distance__gmtl_LineSegd_gmtl_Point3d(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_distance__gmtl_LineSegf_gmtl_Point3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static float distance(gmtl.LineSegf p0, gmtl.Point3f p1)
   {
      
      
      float result;
      result = gmtl_distance__gmtl_LineSegf_gmtl_Point3f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_distance__gmtl_Planed_gmtl_Point3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static double distance(gmtl.Planed p0, gmtl.Point3d p1)
   {
      
      
      double result;
      result = gmtl_distance__gmtl_Planed_gmtl_Point3d(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_distance__gmtl_Planef_gmtl_Point3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static float distance(gmtl.Planef p0, gmtl.Point3f p1)
   {
      
      
      float result;
      result = gmtl_distance__gmtl_Planef_gmtl_Point3f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_distanceSquared__gmtl_LineSegd_gmtl_Point3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static double distanceSquared(gmtl.LineSegd p0, gmtl.Point3d p1)
   {
      
      
      double result;
      result = gmtl_distanceSquared__gmtl_LineSegd_gmtl_Point3d(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_distanceSquared__gmtl_LineSegf_gmtl_Point3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static float distanceSquared(gmtl.LineSegf p0, gmtl.Point3f p1)
   {
      
      
      float result;
      result = gmtl_distanceSquared__gmtl_LineSegf_gmtl_Point3f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_div__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	gmtl.Quatf p2);

   public static gmtl.Quatf div(gmtl.Quatf p0, gmtl.Quatf p1, gmtl.Quatf p2)
   {
      
      
      gmtl.Quatf result;
      result = gmtl_div__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_div__gmtl_Quatf_gmtl_Quatf_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	float p2);

   public static gmtl.Quatf div(gmtl.Quatf p0, gmtl.Quatf p1, float p2)
   {
      
      
      gmtl.Quatf result;
      result = gmtl_div__gmtl_Quatf_gmtl_Quatf_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_div__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	gmtl.Quatd p2);

   public static gmtl.Quatd div(gmtl.Quatd p0, gmtl.Quatd p1, gmtl.Quatd p2)
   {
      
      
      gmtl.Quatd result;
      result = gmtl_div__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_div__gmtl_Quatd_gmtl_Quatd_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	double p2);

   public static gmtl.Quatd div(gmtl.Quatd p0, gmtl.Quatd p1, double p2)
   {
      
      
      gmtl.Quatd result;
      result = gmtl_div__gmtl_Quatd_gmtl_Quatd_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_dot__gmtl_Quatf_gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1);

   public static float dot(gmtl.Quatf p0, gmtl.Quatf p1)
   {
      
      
      float result;
      result = gmtl_dot__gmtl_Quatf_gmtl_Quatf(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_dot__gmtl_Quatd_gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1);

   public static double dot(gmtl.Quatd p0, gmtl.Quatd p1)
   {
      
      
      double result;
      result = gmtl_dot__gmtl_Quatd_gmtl_Quatd(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_dot__gmtl_Vec3d_gmtl_Vec3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1);

   public static double dot(gmtl.Vec3d p0, gmtl.Vec3d p1)
   {
      
      
      double result;
      result = gmtl_dot__gmtl_Vec3d_gmtl_Vec3d(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_dot__gmtl_Vec3f_gmtl_Vec3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public static float dot(gmtl.Vec3f p0, gmtl.Vec3f p1)
   {
      
      
      float result;
      result = gmtl_dot__gmtl_Vec3f_gmtl_Vec3f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_dot__gmtl_Vec4f_gmtl_Vec4f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p1);

   public static float dot(gmtl.Vec4f p0, gmtl.Vec4f p1)
   {
      
      
      float result;
      result = gmtl_dot__gmtl_Vec4f_gmtl_Vec4f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_dot__gmtl_Vec4d_gmtl_Vec4d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p1);

   public static double dot(gmtl.Vec4d p0, gmtl.Vec4d p1)
   {
      
      
      double result;
      result = gmtl_dot__gmtl_Vec4d_gmtl_Vec4d(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_exp__gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd exp(gmtl.Quatd p0)
   {
      
      gmtl.Quatd result;
      result = gmtl_exp__gmtl_Quatd(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_exp__gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf exp(gmtl.Quatf p0)
   {
      
      gmtl.Quatf result;
      result = gmtl_exp__gmtl_Quatf(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3dMarshaler))]
   private extern static gmtl.Point3d gmtl_findNearestPt__gmtl_LineSegd_gmtl_Point3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static gmtl.Point3d findNearestPt(gmtl.LineSegd p0, gmtl.Point3d p1)
   {
      
      
      gmtl.Point3d result;
      result = gmtl_findNearestPt__gmtl_LineSegd_gmtl_Point3d(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3fMarshaler))]
   private extern static gmtl.Point3f gmtl_findNearestPt__gmtl_LineSegf_gmtl_Point3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static gmtl.Point3f findNearestPt(gmtl.LineSegf p0, gmtl.Point3f p1)
   {
      
      
      gmtl.Point3f result;
      result = gmtl_findNearestPt__gmtl_LineSegf_gmtl_Point3f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_findNearestPt__gmtl_Planef_gmtl_Point3f_gmtl_Point3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p2);

   public static float findNearestPt(gmtl.Planef p0, gmtl.Point3f p1, gmtl.Point3f p2)
   {
      
      
      
      float result;
      result = gmtl_findNearestPt__gmtl_Planef_gmtl_Point3f_gmtl_Point3f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_findNearestPt__gmtl_Planed_gmtl_Point3d_gmtl_Point3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p2);

   public static double findNearestPt(gmtl.Planed p0, gmtl.Point3d p1, gmtl.Point3d p2)
   {
      
      
      
      double result;
      result = gmtl_findNearestPt__gmtl_Planed_gmtl_Point3d_gmtl_Point3d(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_identity__gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f identity(gmtl.Matrix44f p0)
   {
      
      gmtl.Matrix44f result;
      result = gmtl_identity__gmtl_Matrix44f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_identity__gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f identity(gmtl.Matrix33f p0)
   {
      
      gmtl.Matrix33f result;
      result = gmtl_identity__gmtl_Matrix33f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_invert__gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd invert(gmtl.Quatd p0)
   {
      
      gmtl.Quatd result;
      result = gmtl_invert__gmtl_Quatd(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_invert__gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf invert(gmtl.Quatf p0)
   {
      
      gmtl.Quatf result;
      result = gmtl_invert__gmtl_Quatf(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invert__gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f invert(gmtl.Matrix44f p0)
   {
      
      gmtl.Matrix44f result;
      result = gmtl_invert__gmtl_Matrix44f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invert__gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f invert(gmtl.Matrix33f p0)
   {
      
      gmtl.Matrix33f result;
      result = gmtl_invert__gmtl_Matrix33f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invert__gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invert(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      
      
      gmtl.Matrix44f result;
      result = gmtl_invert__gmtl_Matrix44f_gmtl_Matrix44f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invert__gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invert(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      
      
      gmtl.Matrix33f result;
      result = gmtl_invert__gmtl_Matrix33f_gmtl_Matrix33f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invertAffine__gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invertAffine(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      
      
      gmtl.Matrix44f result;
      result = gmtl_invertAffine__gmtl_Matrix44f_gmtl_Matrix44f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invertAffine__gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invertAffine(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      
      
      gmtl.Matrix33f result;
      result = gmtl_invertAffine__gmtl_Matrix33f_gmtl_Matrix33f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invertFull__gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invertFull(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      
      
      gmtl.Matrix44f result;
      result = gmtl_invertFull__gmtl_Matrix44f_gmtl_Matrix44f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invertFull__gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invertFull(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      
      
      gmtl.Matrix33f result;
      result = gmtl_invertFull__gmtl_Matrix33f_gmtl_Matrix33f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invertOrthogonal__gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invertOrthogonal(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      
      
      gmtl.Matrix33f result;
      result = gmtl_invertOrthogonal__gmtl_Matrix33f_gmtl_Matrix33f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invertOrthogonal__gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invertOrthogonal(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      
      
      gmtl.Matrix44f result;
      result = gmtl_invertOrthogonal__gmtl_Matrix44f_gmtl_Matrix44f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invertTrans__gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invertTrans(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      
      
      gmtl.Matrix33f result;
      result = gmtl_invertTrans__gmtl_Matrix33f_gmtl_Matrix33f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invertTrans__gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invertTrans(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      
      
      gmtl.Matrix44f result;
      result = gmtl_invertTrans__gmtl_Matrix44f_gmtl_Matrix44f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Sphered_gmtl_Sphered_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p1,
	ref double p2);

   public static bool isEqual(gmtl.Sphered p0, gmtl.Sphered p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Sphered_gmtl_Sphered_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Spheref_gmtl_Spheref_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpherefMarshaler))] gmtl.Spheref p1,
	ref float p2);

   public static bool isEqual(gmtl.Spheref p0, gmtl.Spheref p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Spheref_gmtl_Spheref_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AxisAngled_gmtl_AxisAngled_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p1,
	ref double p2);

   public static bool isEqual(gmtl.AxisAngled p0, gmtl.AxisAngled p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_AxisAngled_gmtl_AxisAngled_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AABoxf_gmtl_AABoxf_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxfMarshaler))] gmtl.AABoxf p1,
	ref float p2);

   public static bool isEqual(gmtl.AABoxf p0, gmtl.AABoxf p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_AABoxf_gmtl_AABoxf_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_VecBase_double_4_gmtl_VecBase_double_4_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p1,
	ref double p2);

   public static bool isEqual(gmtl.VecBase_double_4 p0, gmtl.VecBase_double_4 p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_VecBase_double_4_gmtl_VecBase_double_4_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_VecBase_double_3_gmtl_VecBase_double_3_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p1,
	ref double p2);

   public static bool isEqual(gmtl.VecBase_double_3 p0, gmtl.VecBase_double_3 p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_VecBase_double_3_gmtl_VecBase_double_3_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AxisAnglef_gmtl_AxisAnglef_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p1,
	ref float p2);

   public static bool isEqual(gmtl.AxisAnglef p0, gmtl.AxisAnglef p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_AxisAnglef_gmtl_AxisAnglef_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_AABoxd_gmtl_AABoxd_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p1,
	ref double p2);

   public static bool isEqual(gmtl.AABoxd p0, gmtl.AABoxd p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_AABoxd_gmtl_AABoxd_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_VecBase_float_4_gmtl_VecBase_float_4_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p1,
	ref float p2);

   public static bool isEqual(gmtl.VecBase_float_4 p0, gmtl.VecBase_float_4 p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_VecBase_float_4_gmtl_VecBase_float_4_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_VecBase_float_3_gmtl_VecBase_float_3_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p1,
	ref float p2);

   public static bool isEqual(gmtl.VecBase_float_3 p0, gmtl.VecBase_float_3 p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_VecBase_float_3_gmtl_VecBase_float_3_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dAxisAngle_gmtl_Coord4dAxisAngle_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dAxisAngleMarshaler))] gmtl.Coord4dAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dAxisAngleMarshaler))] gmtl.Coord4dAxisAngle p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dAxisAngle p0, gmtl.Coord4dAxisAngle p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dAxisAngle_gmtl_Coord4dAxisAngle_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dAxisAngle_gmtl_Coord3dAxisAngle_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dAxisAngleMarshaler))] gmtl.Coord3dAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dAxisAngleMarshaler))] gmtl.Coord3dAxisAngle p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dAxisAngle p0, gmtl.Coord3dAxisAngle p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dAxisAngle_gmtl_Coord3dAxisAngle_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dXYZ_gmtl_Coord4dXYZ_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dXYZMarshaler))] gmtl.Coord4dXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dXYZMarshaler))] gmtl.Coord4dXYZ p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dXYZ p0, gmtl.Coord4dXYZ p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dXYZ_gmtl_Coord4dXYZ_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fAxisAngle_gmtl_Coord3fAxisAngle_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fAxisAngleMarshaler))] gmtl.Coord3fAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fAxisAngleMarshaler))] gmtl.Coord3fAxisAngle p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fAxisAngle p0, gmtl.Coord3fAxisAngle p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fAxisAngle_gmtl_Coord3fAxisAngle_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dQuat_gmtl_Coord4dQuat_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dQuatMarshaler))] gmtl.Coord4dQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dQuatMarshaler))] gmtl.Coord4dQuat p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dQuat p0, gmtl.Coord4dQuat p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dQuat_gmtl_Coord4dQuat_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fAxisAngle_gmtl_Coord4fAxisAngle_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fAxisAngleMarshaler))] gmtl.Coord4fAxisAngle p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fAxisAngleMarshaler))] gmtl.Coord4fAxisAngle p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fAxisAngle p0, gmtl.Coord4fAxisAngle p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fAxisAngle_gmtl_Coord4fAxisAngle_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fQuat_gmtl_Coord3fQuat_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fQuatMarshaler))] gmtl.Coord3fQuat p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fQuat p0, gmtl.Coord3fQuat p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fQuat_gmtl_Coord3fQuat_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dZXY_gmtl_Coord4dZXY_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZXYMarshaler))] gmtl.Coord4dZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZXYMarshaler))] gmtl.Coord4dZXY p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dZXY p0, gmtl.Coord4dZXY p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dZXY_gmtl_Coord4dZXY_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fQuat_gmtl_Coord4fQuat_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fQuatMarshaler))] gmtl.Coord4fQuat p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fQuat p0, gmtl.Coord4fQuat p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fQuat_gmtl_Coord4fQuat_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dQuat_gmtl_Coord3dQuat_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dQuatMarshaler))] gmtl.Coord3dQuat p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dQuatMarshaler))] gmtl.Coord3dQuat p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dQuat p0, gmtl.Coord3dQuat p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dQuat_gmtl_Coord3dQuat_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4dZYX_gmtl_Coord4dZYX_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZYXMarshaler))] gmtl.Coord4dZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4dZYXMarshaler))] gmtl.Coord4dZYX p1,
	double p2);

   public static bool isEqual(gmtl.Coord4dZYX p0, gmtl.Coord4dZYX p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4dZYX_gmtl_Coord4dZYX_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fXYZ_gmtl_Coord3fXYZ_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fXYZMarshaler))] gmtl.Coord3fXYZ p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fXYZ p0, gmtl.Coord3fXYZ p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fXYZ_gmtl_Coord3fXYZ_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fZYX_gmtl_Coord3fZYX_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZYXMarshaler))] gmtl.Coord3fZYX p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fZYX p0, gmtl.Coord3fZYX p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fZYX_gmtl_Coord3fZYX_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dXYZ_gmtl_Coord3dXYZ_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dXYZMarshaler))] gmtl.Coord3dXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dXYZMarshaler))] gmtl.Coord3dXYZ p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dXYZ p0, gmtl.Coord3dXYZ p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dXYZ_gmtl_Coord3dXYZ_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dZYX_gmtl_Coord3dZYX_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZYXMarshaler))] gmtl.Coord3dZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZYXMarshaler))] gmtl.Coord3dZYX p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dZYX p0, gmtl.Coord3dZYX p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dZYX_gmtl_Coord3dZYX_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3dZXY_gmtl_Coord3dZXY_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZXYMarshaler))] gmtl.Coord3dZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dZXYMarshaler))] gmtl.Coord3dZXY p1,
	double p2);

   public static bool isEqual(gmtl.Coord3dZXY p0, gmtl.Coord3dZXY p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3dZXY_gmtl_Coord3dZXY_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fXYZ_gmtl_Coord4fXYZ_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fXYZMarshaler))] gmtl.Coord4fXYZ p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fXYZ p0, gmtl.Coord4fXYZ p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fXYZ_gmtl_Coord4fXYZ_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fZYX_gmtl_Coord4fZYX_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZYXMarshaler))] gmtl.Coord4fZYX p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fZYX p0, gmtl.Coord4fZYX p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fZYX_gmtl_Coord4fZYX_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord4fZXY_gmtl_Coord4fZXY_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fZXYMarshaler))] gmtl.Coord4fZXY p1,
	float p2);

   public static bool isEqual(gmtl.Coord4fZXY p0, gmtl.Coord4fZXY p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord4fZXY_gmtl_Coord4fZXY_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleXYZd_gmtl_EulerAngleXYZd_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))] gmtl.EulerAngleXYZd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZdMarshaler))] gmtl.EulerAngleXYZd p1,
	ref double p2);

   public static bool isEqual(gmtl.EulerAngleXYZd p0, gmtl.EulerAngleXYZd p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleXYZd_gmtl_EulerAngleXYZd_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZYXd_gmtl_EulerAngleZYXd_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))] gmtl.EulerAngleZYXd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXdMarshaler))] gmtl.EulerAngleZYXd p1,
	ref double p2);

   public static bool isEqual(gmtl.EulerAngleZYXd p0, gmtl.EulerAngleZYXd p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZYXd_gmtl_EulerAngleZYXd_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZYXf_gmtl_EulerAngleZYXf_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p1,
	ref float p2);

   public static bool isEqual(gmtl.EulerAngleZYXf p0, gmtl.EulerAngleZYXf p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZYXf_gmtl_EulerAngleZYXf_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZXYf_gmtl_EulerAngleZXYf_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p1,
	ref float p2);

   public static bool isEqual(gmtl.EulerAngleZXYf p0, gmtl.EulerAngleZXYf p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZXYf_gmtl_EulerAngleZXYf_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleZXYd_gmtl_EulerAngleZXYd_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))] gmtl.EulerAngleZXYd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYdMarshaler))] gmtl.EulerAngleZXYd p1,
	ref double p2);

   public static bool isEqual(gmtl.EulerAngleZXYd p0, gmtl.EulerAngleZXYd p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleZXYd_gmtl_EulerAngleZXYd_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_EulerAngleXYZf_gmtl_EulerAngleXYZf_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleXYZfMarshaler))] gmtl.EulerAngleXYZf p1,
	ref float p2);

   public static bool isEqual(gmtl.EulerAngleXYZf p0, gmtl.EulerAngleXYZf p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_EulerAngleXYZf_gmtl_EulerAngleXYZf_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Trif_gmtl_Trif_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TrifMarshaler))] gmtl.Trif p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TrifMarshaler))] gmtl.Trif p1,
	ref float p2);

   public static bool isEqual(gmtl.Trif p0, gmtl.Trif p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Trif_gmtl_Trif_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Trid_gmtl_Trid_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TridMarshaler))] gmtl.Trid p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TridMarshaler))] gmtl.Trid p1,
	ref double p2);

   public static bool isEqual(gmtl.Trid p0, gmtl.Trid p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Trid_gmtl_Trid_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Quatd_gmtl_Quatd_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	double p2);

   public static bool isEqual(gmtl.Quatd p0, gmtl.Quatd p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Quatd_gmtl_Quatd_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Quatf_gmtl_Quatf_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	float p2);

   public static bool isEqual(gmtl.Quatf p0, gmtl.Quatf p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Quatf_gmtl_Quatf_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Coord3fZXY_gmtl_Coord3fZXY_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3fZXYMarshaler))] gmtl.Coord3fZXY p1,
	float p2);

   public static bool isEqual(gmtl.Coord3fZXY p0, gmtl.Coord3fZXY p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Coord3fZXY_gmtl_Coord3fZXY_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Planef_gmtl_Planef_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p1,
	ref float p2);

   public static bool isEqual(gmtl.Planef p0, gmtl.Planef p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Planef_gmtl_Planef_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Planed_gmtl_Planed_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p1,
	ref double p2);

   public static bool isEqual(gmtl.Planed p0, gmtl.Planed p1, ref double p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Planed_gmtl_Planed_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	ref float p2);

   public static bool isEqual(gmtl.Matrix33f p0, gmtl.Matrix33f p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	ref float p2);

   public static bool isEqual(gmtl.Matrix44f p0, gmtl.Matrix44f p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEquiv__gmtl_Quatd_gmtl_Quatd_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	double p2);

   public static bool isEquiv(gmtl.Quatd p0, gmtl.Quatd p1, double p2)
   {
      
      
      bool result;
      result = gmtl_isEquiv__gmtl_Quatd_gmtl_Quatd_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEquiv__gmtl_Quatf_gmtl_Quatf_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	float p2);

   public static bool isEquiv(gmtl.Quatf p0, gmtl.Quatf p1, float p2)
   {
      
      
      bool result;
      result = gmtl_isEquiv__gmtl_Quatf_gmtl_Quatf_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Quatd_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	double p1);

   public static bool isNormalized(gmtl.Quatd p0, double p1)
   {
      
      bool result;
      result = gmtl_isNormalized__gmtl_Quatd_double(p0, p1);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Quatf_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	float p1);

   public static bool isNormalized(gmtl.Quatf p0, float p1)
   {
      
      bool result;
      result = gmtl_isNormalized__gmtl_Quatf_float(p0, p1);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Vec3f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0,
	float p1);

   public static bool isNormalized(gmtl.Vec3f p0, float p1)
   {
      
      bool result;
      result = gmtl_isNormalized__gmtl_Vec3f_float(p0, p1);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isNormalized__gmtl_Vec4f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0,
	float p1);

   public static bool isNormalized(gmtl.Vec4f p0, float p1)
   {
      
      bool result;
      result = gmtl_isNormalized__gmtl_Vec4f_float(p0, p1);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_length__gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static float length(gmtl.Quatf p0)
   {
      
      float result;
      result = gmtl_length__gmtl_Quatf(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_length__gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static double length(gmtl.Quatd p0)
   {
      
      double result;
      result = gmtl_length__gmtl_Quatd(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_length__gmtl_Vec4f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0);

   public static float length(gmtl.Vec4f p0)
   {
      
      float result;
      result = gmtl_length__gmtl_Vec4f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_length__gmtl_Vec3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public static float length(gmtl.Vec3f p0)
   {
      
      float result;
      result = gmtl_length__gmtl_Vec3f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_length__gmtl_Vec3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0);

   public static double length(gmtl.Vec3d p0)
   {
      
      double result;
      result = gmtl_length__gmtl_Vec3d(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_lengthSquared__gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static float lengthSquared(gmtl.Quatf p0)
   {
      
      float result;
      result = gmtl_lengthSquared__gmtl_Quatf(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_lengthSquared__gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static double lengthSquared(gmtl.Quatd p0)
   {
      
      double result;
      result = gmtl_lengthSquared__gmtl_Quatd(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_lengthSquared__gmtl_Vec4f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0);

   public static float lengthSquared(gmtl.Vec4f p0)
   {
      
      float result;
      result = gmtl_lengthSquared__gmtl_Vec4f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_lengthSquared__gmtl_Vec3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public static float lengthSquared(gmtl.Vec3f p0)
   {
      
      float result;
      result = gmtl_lengthSquared__gmtl_Vec3f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_lengthSquared__gmtl_Vec3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0);

   public static double lengthSquared(gmtl.Vec3d p0)
   {
      
      double result;
      result = gmtl_lengthSquared__gmtl_Vec3d(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))]
   private extern static gmtl.VecBase_float_3 gmtl_lerp__gmtl_VecBase_float_3_float_gmtl_VecBase_float_3_gmtl_VecBase_float_3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p0,
	ref float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p3);

   public static gmtl.VecBase_float_3 lerp(gmtl.VecBase_float_3 p0, ref float p1, gmtl.VecBase_float_3 p2, gmtl.VecBase_float_3 p3)
   {
      
      
      
      
      gmtl.VecBase_float_3 result;
      result = gmtl_lerp__gmtl_VecBase_float_3_float_gmtl_VecBase_float_3_gmtl_VecBase_float_3(p0, ref p1, p2, p3);
      
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))]
   private extern static gmtl.VecBase_float_4 gmtl_lerp__gmtl_VecBase_float_4_float_gmtl_VecBase_float_4_gmtl_VecBase_float_4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p0,
	ref float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p3);

   public static gmtl.VecBase_float_4 lerp(gmtl.VecBase_float_4 p0, ref float p1, gmtl.VecBase_float_4 p2, gmtl.VecBase_float_4 p3)
   {
      
      
      
      
      gmtl.VecBase_float_4 result;
      result = gmtl_lerp__gmtl_VecBase_float_4_float_gmtl_VecBase_float_4_gmtl_VecBase_float_4(p0, ref p1, p2, p3);
      
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))]
   private extern static gmtl.VecBase_double_3 gmtl_lerp__gmtl_VecBase_double_3_double_gmtl_VecBase_double_3_gmtl_VecBase_double_3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p0,
	ref double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p3);

   public static gmtl.VecBase_double_3 lerp(gmtl.VecBase_double_3 p0, ref double p1, gmtl.VecBase_double_3 p2, gmtl.VecBase_double_3 p3)
   {
      
      
      
      
      gmtl.VecBase_double_3 result;
      result = gmtl_lerp__gmtl_VecBase_double_3_double_gmtl_VecBase_double_3_gmtl_VecBase_double_3(p0, ref p1, p2, p3);
      
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))]
   private extern static gmtl.VecBase_double_4 gmtl_lerp__gmtl_VecBase_double_4_double_gmtl_VecBase_double_4_gmtl_VecBase_double_4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p0,
	ref double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p3);

   public static gmtl.VecBase_double_4 lerp(gmtl.VecBase_double_4 p0, ref double p1, gmtl.VecBase_double_4 p2, gmtl.VecBase_double_4 p3)
   {
      
      
      
      
      gmtl.VecBase_double_4 result;
      result = gmtl_lerp__gmtl_VecBase_double_4_double_gmtl_VecBase_double_4_gmtl_VecBase_double_4(p0, ref p1, p2, p3);
      
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_lerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p3);

   public static gmtl.Quatd lerp(gmtl.Quatd p0, double p1, gmtl.Quatd p2, gmtl.Quatd p3)
   {
      
      
      
      gmtl.Quatd result;
      result = gmtl_lerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd(p0, p1, p2, p3);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_lerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p3);

   public static gmtl.Quatf lerp(gmtl.Quatf p0, float p1, gmtl.Quatf p2, gmtl.Quatf p3)
   {
      
      
      
      gmtl.Quatf result;
      result = gmtl_lerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf(p0, p1, p2, p3);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_log__gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf log(gmtl.Quatf p0)
   {
      
      gmtl.Quatf result;
      result = gmtl_log__gmtl_Quatf(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_log__gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd log(gmtl.Quatd p0)
   {
      
      gmtl.Quatd result;
      result = gmtl_log__gmtl_Quatd(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_mult__gmtl_Quatf_gmtl_Quatf_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	float p2);

   public static gmtl.Quatf mult(gmtl.Quatf p0, gmtl.Quatf p1, float p2)
   {
      
      
      gmtl.Quatf result;
      result = gmtl_mult__gmtl_Quatf_gmtl_Quatf_float(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_mult__gmtl_Quatd_gmtl_Quatd_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	double p2);

   public static gmtl.Quatd mult(gmtl.Quatd p0, gmtl.Quatd p1, double p2)
   {
      
      
      gmtl.Quatd result;
      result = gmtl_mult__gmtl_Quatd_gmtl_Quatd_double(p0, p1, p2);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p2);

   public static gmtl.Matrix44f mult(gmtl.Matrix44f p0, gmtl.Matrix44f p1, gmtl.Matrix44f p2)
   {
      
      
      
      gmtl.Matrix44f result;
      result = gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p2);

   public static gmtl.Matrix33f mult(gmtl.Matrix33f p0, gmtl.Matrix33f p1, gmtl.Matrix33f p2)
   {
      
      
      
      gmtl.Matrix33f result;
      result = gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_mult__gmtl_Matrix44f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	float p1);

   public static gmtl.Matrix44f mult(gmtl.Matrix44f p0, float p1)
   {
      
      gmtl.Matrix44f result;
      result = gmtl_mult__gmtl_Matrix44f_float(p0, p1);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	ref float p2);

   public static gmtl.Matrix33f mult(gmtl.Matrix33f p0, gmtl.Matrix33f p1, ref float p2)
   {
      
      
      
      gmtl.Matrix33f result;
      result = gmtl_mult__gmtl_Matrix33f_gmtl_Matrix33f_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	ref float p2);

   public static gmtl.Matrix44f mult(gmtl.Matrix44f p0, gmtl.Matrix44f p1, ref float p2)
   {
      
      
      
      gmtl.Matrix44f result;
      result = gmtl_mult__gmtl_Matrix44f_gmtl_Matrix44f_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_mult__gmtl_Matrix33f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	float p1);

   public static gmtl.Matrix33f mult(gmtl.Matrix33f p0, float p1)
   {
      
      gmtl.Matrix33f result;
      result = gmtl_mult__gmtl_Matrix33f_float(p0, p1);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_mult__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2);

   public static gmtl.Quatd mult(gmtl.Quatd p0, gmtl.Quatd p1, gmtl.Quatd p2)
   {
      
      
      
      gmtl.Quatd result;
      result = gmtl_mult__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_mult__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2);

   public static gmtl.Quatf mult(gmtl.Quatf p0, gmtl.Quatf p1, gmtl.Quatf p2)
   {
      
      
      
      gmtl.Quatf result;
      result = gmtl_mult__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_negate__gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public static gmtl.Quatf negate(gmtl.Quatf p0)
   {
      
      gmtl.Quatf result;
      result = gmtl_negate__gmtl_Quatf(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_negate__gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public static gmtl.Quatd negate(gmtl.Quatd p0)
   {
      
      gmtl.Quatd result;
      result = gmtl_negate__gmtl_Quatd(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_postMult__gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f postMult(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      
      
      gmtl.Matrix44f result;
      result = gmtl_postMult__gmtl_Matrix44f_gmtl_Matrix44f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_postMult__gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f postMult(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      
      
      gmtl.Matrix33f result;
      result = gmtl_postMult__gmtl_Matrix33f_gmtl_Matrix33f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_preMult__gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f preMult(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      
      
      gmtl.Matrix44f result;
      result = gmtl_preMult__gmtl_Matrix44f_gmtl_Matrix44f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_preMult__gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f preMult(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      
      
      gmtl.Matrix33f result;
      result = gmtl_preMult__gmtl_Matrix33f_gmtl_Matrix33f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))]
   private extern static gmtl.VecBase_double_4 gmtl_reflect__gmtl_VecBase_double_4_gmtl_VecBase_double_4_gmtl_Vec4d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4dMarshaler))] gmtl.Vec4d p2);

   public static gmtl.VecBase_double_4 reflect(gmtl.VecBase_double_4 p0, gmtl.VecBase_double_4 p1, gmtl.Vec4d p2)
   {
      
      
      
      gmtl.VecBase_double_4 result;
      result = gmtl_reflect__gmtl_VecBase_double_4_gmtl_VecBase_double_4_gmtl_Vec4d(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))]
   private extern static gmtl.VecBase_double_3 gmtl_reflect__gmtl_VecBase_double_3_gmtl_VecBase_double_3_gmtl_Vec3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p2);

   public static gmtl.VecBase_double_3 reflect(gmtl.VecBase_double_3 p0, gmtl.VecBase_double_3 p1, gmtl.Vec3d p2)
   {
      
      
      
      gmtl.VecBase_double_3 result;
      result = gmtl_reflect__gmtl_VecBase_double_3_gmtl_VecBase_double_3_gmtl_Vec3d(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))]
   private extern static gmtl.VecBase_float_4 gmtl_reflect__gmtl_VecBase_float_4_gmtl_VecBase_float_4_gmtl_Vec4f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p2);

   public static gmtl.VecBase_float_4 reflect(gmtl.VecBase_float_4 p0, gmtl.VecBase_float_4 p1, gmtl.Vec4f p2)
   {
      
      
      
      gmtl.VecBase_float_4 result;
      result = gmtl_reflect__gmtl_VecBase_float_4_gmtl_VecBase_float_4_gmtl_Vec4f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))]
   private extern static gmtl.VecBase_float_3 gmtl_reflect__gmtl_VecBase_float_3_gmtl_VecBase_float_3_gmtl_Vec3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_3Marshaler))] gmtl.VecBase_float_3 p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p2);

   public static gmtl.VecBase_float_3 reflect(gmtl.VecBase_float_3 p0, gmtl.VecBase_float_3 p1, gmtl.Vec3f p2)
   {
      
      
      
      gmtl.VecBase_float_3 result;
      result = gmtl_reflect__gmtl_VecBase_float_3_gmtl_VecBase_float_3_gmtl_Vec3f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_slerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_bool([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p3,
	bool p4);

   public static gmtl.Quatd slerp(gmtl.Quatd p0, double p1, gmtl.Quatd p2, gmtl.Quatd p3, bool p4)
   {
      
      
      
      gmtl.Quatd result;
      result = gmtl_slerp__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_bool(p0, p1, p2, p3, p4);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_slerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_bool([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p3,
	bool p4);

   public static gmtl.Quatf slerp(gmtl.Quatf p0, float p1, gmtl.Quatf p2, gmtl.Quatf p3, bool p4)
   {
      
      
      
      gmtl.Quatf result;
      result = gmtl_slerp__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_bool(p0, p1, p2, p3, p4);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_squad__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	float p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p5);

   public static void squad(gmtl.Quatf p0, float p1, gmtl.Quatf p2, gmtl.Quatf p3, gmtl.Quatf p4, gmtl.Quatf p5)
   {
      
      
      
      
      
      gmtl_squad__gmtl_Quatf_float_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf_gmtl_Quatf(p0, p1, p2, p3, p4, p5);
      
      
      
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_squad__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	double p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p3,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p4,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p5);

   public static void squad(gmtl.Quatd p0, double p1, gmtl.Quatd p2, gmtl.Quatd p3, gmtl.Quatd p4, gmtl.Quatd p5)
   {
      
      
      
      
      
      gmtl_squad__gmtl_Quatd_double_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd_gmtl_Quatd(p0, p1, p2, p3, p4, p5);
      
      
      
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatdMarshaler))]
   private extern static gmtl.Quatd gmtl_sub__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p2);

   public static gmtl.Quatd sub(gmtl.Quatd p0, gmtl.Quatd p1, gmtl.Quatd p2)
   {
      
      
      
      gmtl.Quatd result;
      result = gmtl_sub__gmtl_Quatd_gmtl_Quatd_gmtl_Quatd(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.QuatfMarshaler))]
   private extern static gmtl.Quatf gmtl_sub__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p2);

   public static gmtl.Quatf sub(gmtl.Quatf p0, gmtl.Quatf p1, gmtl.Quatf p2)
   {
      
      
      
      gmtl.Quatf result;
      result = gmtl_sub__gmtl_Quatf_gmtl_Quatf_gmtl_Quatf(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_sub__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p2);

   public static gmtl.Matrix44f sub(gmtl.Matrix44f p0, gmtl.Matrix44f p1, gmtl.Matrix44f p2)
   {
      
      
      
      gmtl.Matrix44f result;
      result = gmtl_sub__gmtl_Matrix44f_gmtl_Matrix44f_gmtl_Matrix44f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_sub__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p2);

   public static gmtl.Matrix33f sub(gmtl.Matrix33f p0, gmtl.Matrix33f p1, gmtl.Matrix33f p2)
   {
      
      
      
      gmtl.Matrix33f result;
      result = gmtl_sub__gmtl_Matrix33f_gmtl_Matrix33f_gmtl_Matrix33f(p0, p1, p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_transpose__gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f transpose(gmtl.Matrix33f p0)
   {
      
      gmtl.Matrix33f result;
      result = gmtl_transpose__gmtl_Matrix33f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_transpose__gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f transpose(gmtl.Matrix44f p0)
   {
      
      gmtl.Matrix44f result;
      result = gmtl_transpose__gmtl_Matrix44f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_transpose__gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f transpose(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      
      
      gmtl.Matrix44f result;
      result = gmtl_transpose__gmtl_Matrix44f_gmtl_Matrix44f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_transpose__gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f transpose(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      
      
      gmtl.Matrix33f result;
      result = gmtl_transpose__gmtl_Matrix33f_gmtl_Matrix33f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static PlaneSide gmtl_whichSide__gmtl_Planed_gmtl_Point3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public static PlaneSide whichSide(gmtl.Planed p0, gmtl.Point3d p1)
   {
      
      
      PlaneSide result;
      result = gmtl_whichSide__gmtl_Planed_gmtl_Point3d(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static PlaneSide gmtl_whichSide__gmtl_Planef_gmtl_Point3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public static PlaneSide whichSide(gmtl.Planef p0, gmtl.Point3f p1)
   {
      
      
      PlaneSide result;
      result = gmtl_whichSide__gmtl_Planef_gmtl_Point3f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static PlaneSide gmtl_whichSide__gmtl_Planef_gmtl_Point3f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1,
	ref float p2);

   public static PlaneSide whichSide(gmtl.Planef p0, gmtl.Point3f p1, ref float p2)
   {
      
      
      
      PlaneSide result;
      result = gmtl_whichSide__gmtl_Planef_gmtl_Point3f_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static PlaneSide gmtl_whichSide__gmtl_Planed_gmtl_Point3d_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1,
	ref double p2);

   public static PlaneSide whichSide(gmtl.Planed p0, gmtl.Point3d p1, ref double p2)
   {
      
      
      
      PlaneSide result;
      result = gmtl_whichSide__gmtl_Planed_gmtl_Point3d_double(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_zero__gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f zero(gmtl.Matrix44f p0)
   {
      
      gmtl.Matrix44f result;
      result = gmtl_zero__gmtl_Matrix44f(p0);
      
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_zero__gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f zero(gmtl.Matrix33f p0)
   {
      
      gmtl.Matrix33f result;
      result = gmtl_zero__gmtl_Matrix33f(p0);
      
      return result;
   }

}


} // namespace gmtl
