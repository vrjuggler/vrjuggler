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

// Generated from Revision: 1.70 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public sealed class Sphered
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Sphered(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Sphere_double__Sphere__();

   public Sphered()
   {
      mRawObject   = gmtl_Sphere_double__Sphere__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Sphere_double__Sphere__gmtl_Point3d_double([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p0,
	ref double p1);

   public Sphered(gmtl.Point3d p0, ref double p1)
   {
      
      
      mRawObject   = gmtl_Sphere_double__Sphere__gmtl_Point3d_double(p0, ref p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Sphere_double__Sphere__gmtl_Sphered([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.SpheredMarshaler))] gmtl.Sphered p0);

   public Sphered(gmtl.Sphered p0)
   {
      
      mRawObject   = gmtl_Sphere_double__Sphere__gmtl_Sphered(p0);
      mWeOwnMemory = true;
      
   }

   // Internal constructor needed for marshaling purposes.
   internal Sphered(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Sphered(IntPtr obj);

   // Destructor.
   ~Sphered()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Sphered(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3dMarshaler))]
   private extern static gmtl.Point3d gmtl_Sphere_double__getCenter__(IntPtr obj);

   public  gmtl.Point3d getCenter()
   {
      gmtl.Point3d result;
      result = gmtl_Sphere_double__getCenter__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_Sphere_double__getRadius__(IntPtr obj);

   public  double getRadius()
   {
      double result;
      result = gmtl_Sphere_double__getRadius__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Sphere_double__setCenter__gmtl_Point3d(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p0);

   public  void setCenter(gmtl.Point3d p0)
   {
      
      gmtl_Sphere_double__setCenter__gmtl_Point3d(mRawObject, p0);
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Sphere_double__setRadius__double(IntPtr obj,
	ref double p0);

   public  void setRadius(ref double p0)
   {
      
      gmtl_Sphere_double__setRadius__double(mRawObject, ref p0);
      
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class gmtl.Sphered

/// <summary>
/// Custom marshaler for gmtl.Sphered.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class SpheredMarshaler : ICustomMarshaler
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
      return ((gmtl.Sphered) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Sphered(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static SpheredMarshaler mInstance = new SpheredMarshaler();
}


} // namespace gmtl
