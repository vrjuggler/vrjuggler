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

// Generated from Revision: 1.78 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public sealed class Planef
{
   protected IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   /// <summary>
   /// This is needed for the custom marshaler to be able to perform a
   /// reflective lookup.  The custom marshaler also uses this method to get
   /// access to the value of mRawObject when necessary.
   /// </summary>
   public IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Planef(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_float__Plane__0();

   public Planef()
   {
      mRawObject   = gmtl_Plane_float__Plane__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_float__Plane__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p0, [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1, [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p2);

   public Planef(gmtl.Point3f p0, gmtl.Point3f p1, gmtl.Point3f p2)
   {
      mRawObject   = gmtl_Plane_float__Plane__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f3(p0, p1, p2);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_float__Plane__gmtl_Vec3f_gmtl_Point3f2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0, [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1);

   public Planef(gmtl.Vec3f p0, gmtl.Point3f p1)
   {
      mRawObject   = gmtl_Plane_float__Plane__gmtl_Vec3f_gmtl_Point3f2(p0, p1);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_float__Plane__gmtl_Vec3f_float2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0, float p1);

   public Planef(gmtl.Vec3f p0, float p1)
   {
      mRawObject   = gmtl_Plane_float__Plane__gmtl_Vec3f_float2(p0, p1);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Plane_float__Plane__gmtl_Planef1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.PlanefMarshaler))] gmtl.Planef p0);

   public Planef(gmtl.Planef p0)
   {
      mRawObject   = gmtl_Plane_float__Plane__gmtl_Planef1(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Planef(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Planef(IntPtr obj);

   // Destructor.
   ~Planef()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Planef(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_Plane_float__getNormal__0(IntPtr obj);

   public  gmtl.Vec3f getNormal()
   {
      gmtl.Vec3f result;
      result = gmtl_Plane_float__getNormal__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Plane_float__setNormal__gmtl_Vec3f1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public  void setNormal(gmtl.Vec3f p0)
   {
      gmtl_Plane_float__setNormal__gmtl_Vec3f1(mRawObject, p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_Plane_float__getOffset__0(IntPtr obj);

   public  float getOffset()
   {
      float result;
      result = gmtl_Plane_float__getOffset__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Plane_float__setOffset__float1(IntPtr obj,
	float p0);

   public  void setOffset(float p0)
   {
      gmtl_Plane_float__setOffset__float1(mRawObject, p0);
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class gmtl.Planef

/// <summary>
/// Custom marshaler for gmtl.Planef.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class PlanefMarshaler : ICustomMarshaler
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
      return ((gmtl.Planef) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Planef(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static PlanefMarshaler mInstance = new PlanefMarshaler();
}


} // namespace gmtl
