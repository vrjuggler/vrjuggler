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

// Generated from Revision: 1.75 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public class Rayd
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Rayd(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Ray_double__Ray__0();

   public Rayd()
   {
      mRawObject   = gmtl_Ray_double__Ray__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Ray_double__Ray__gmtl_Point3d_gmtl_Vec3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p0, [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1);

   public Rayd(gmtl.Point3d p0, gmtl.Vec3d p1)
   {
      mRawObject   = gmtl_Ray_double__Ray__gmtl_Point3d_gmtl_Vec3d2(p0, p1);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Ray_double__Ray__gmtl_Rayd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RaydMarshaler))] gmtl.Rayd p0);

   public Rayd(gmtl.Rayd p0)
   {
      mRawObject   = gmtl_Ray_double__Ray__gmtl_Rayd1(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Rayd(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Rayd(IntPtr obj);

   // Destructor.
   ~Rayd()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Rayd(mRawObject);
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
   private extern static gmtl.Point3d gmtl_Ray_double__getOrigin__0(IntPtr obj);

   public  gmtl.Point3d getOrigin()
   {
      gmtl.Point3d result;
      result = gmtl_Ray_double__getOrigin__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Ray_double__setOrigin__gmtl_Point3d1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p0);

   public  void setOrigin(gmtl.Point3d p0)
   {
      gmtl_Ray_double__setOrigin__gmtl_Point3d1(mRawObject, p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_Ray_double__getDir__0(IntPtr obj);

   public  gmtl.Vec3d getDir()
   {
      gmtl.Vec3d result;
      result = gmtl_Ray_double__getDir__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Ray_double__setDir__gmtl_Vec3d1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0);

   public  void setDir(gmtl.Vec3d p0)
   {
      gmtl_Ray_double__setDir__gmtl_Vec3d1(mRawObject, p0);
   }


   // End of non-virtual methods.


} // class gmtl.Rayd

/// <summary>
/// Custom marshaler for gmtl.Rayd.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class RaydMarshaler : ICustomMarshaler
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
      return ((gmtl.Rayd) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Rayd(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static RaydMarshaler mInstance = new RaydMarshaler();
}


} // namespace gmtl
