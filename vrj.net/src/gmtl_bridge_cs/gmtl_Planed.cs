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

// Generated from Revision: 1.73 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public sealed class Planed
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Planed(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_double__Plane__();

   public Planed()
   {
      mRawObject   = gmtl_Plane_double__Plane__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_double__Plane__gmtl_Point3d_gmtl_Point3d_gmtl_Point3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p2);

   public Planed(gmtl.Point3d p0, gmtl.Point3d p1, gmtl.Point3d p2)
   {
      
      
      
      mRawObject   = gmtl_Plane_double__Plane__gmtl_Point3d_gmtl_Point3d_gmtl_Point3d(p0, p1, p2);
      mWeOwnMemory = true;
      
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_double__Plane__gmtl_Vec3d_gmtl_Point3d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public Planed(gmtl.Vec3d p0, gmtl.Point3d p1)
   {
      
      
      mRawObject   = gmtl_Plane_double__Plane__gmtl_Vec3d_gmtl_Point3d(p0, p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_double__Plane__gmtl_Vec3d_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	double p1);

   public Planed(gmtl.Vec3d p0, double p1)
   {
      
      mRawObject   = gmtl_Plane_double__Plane__gmtl_Vec3d_double(p0, p1);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_double__Plane__gmtl_Planed([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanedMarshaler))] gmtl.Planed p0);

   public Planed(gmtl.Planed p0)
   {
      
      mRawObject   = gmtl_Plane_double__Plane__gmtl_Planed(p0);
      mWeOwnMemory = true;
      
   }

   // Internal constructor needed for marshaling purposes.
   internal Planed(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Planed(IntPtr obj);

   // Destructor.
   ~Planed()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Planed(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_Plane_double__getNormal__0(IntPtr obj);

   public  gmtl.Vec3d getNormal()
   {
      gmtl.Vec3d result;
      result = gmtl_Plane_double__getNormal__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Plane_double__setNormal__gmtl_Vec3d1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0);

   public  void setNormal(gmtl.Vec3d p0)
   {
      gmtl_Plane_double__setNormal__gmtl_Vec3d1(mRawObject, p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_Plane_double__getOffset__0(IntPtr obj);

   public  double getOffset()
   {
      double result;
      result = gmtl_Plane_double__getOffset__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Plane_double__setOffset__double1(IntPtr obj,
	double p0);

   public  void setOffset(double p0)
   {
      gmtl_Plane_double__setOffset__double1(mRawObject, p0);
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class gmtl.Planed

/// <summary>
/// Custom marshaler for gmtl.Planed.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class PlanedMarshaler : ICustomMarshaler
{
   public void CleanUpManagedData(Object obj)
   {
   }

   public void CleanUpNativeData(IntPtr nativeData)
   {
   }

   public int GetNativeDataSize()
   {
      return -1;
   }

   // Marshaling for managed data being passed to C++.
   public IntPtr MarshalManagedToNative(Object obj)
   {
      return ((gmtl.Planed) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Planed(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static PlanedMarshaler mInstance = new PlanedMarshaler();
}


} // namespace gmtl
