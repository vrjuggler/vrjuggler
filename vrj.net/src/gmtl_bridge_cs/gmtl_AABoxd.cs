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

public sealed class AABoxd
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
   protected AABoxd(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AABox_double__AABox__0();

   public AABoxd()
   {
      mRawObject   = gmtl_AABox_double__AABox__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AABox_double__AABox__gmtl_Point3d_gmtl_Point3d2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p0, [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p1);

   public AABoxd(gmtl.Point3d p0, gmtl.Point3d p1)
   {
      mRawObject   = gmtl_AABox_double__AABox__gmtl_Point3d_gmtl_Point3d2(p0, p1);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AABox_double__AABox__gmtl_AABoxd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AABoxdMarshaler))] gmtl.AABoxd p0);

   public AABoxd(gmtl.AABoxd p0)
   {
      mRawObject   = gmtl_AABox_double__AABox__gmtl_AABoxd1(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal AABoxd(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_AABoxd(IntPtr obj);

   // Destructor.
   ~AABoxd()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_AABoxd(mRawObject);
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
   private extern static gmtl.Point3d gmtl_AABox_double__getMin__0(IntPtr obj);

   public  gmtl.Point3d getMin()
   {
      gmtl.Point3d result;
      result = gmtl_AABox_double__getMin__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3dMarshaler))]
   private extern static gmtl.Point3d gmtl_AABox_double__getMax__0(IntPtr obj);

   public  gmtl.Point3d getMax()
   {
      gmtl.Point3d result;
      result = gmtl_AABox_double__getMax__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_AABox_double__isEmpty__0(IntPtr obj);

   public  bool isEmpty()
   {
      bool result;
      result = gmtl_AABox_double__isEmpty__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AABox_double__setMin__gmtl_Point3d1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p0);

   public  void setMin(gmtl.Point3d p0)
   {
      gmtl_AABox_double__setMin__gmtl_Point3d1(mRawObject, p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AABox_double__setMax__gmtl_Point3d1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3dMarshaler))] gmtl.Point3d p0);

   public  void setMax(gmtl.Point3d p0)
   {
      gmtl_AABox_double__setMax__gmtl_Point3d1(mRawObject, p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AABox_double__setEmpty__bool1(IntPtr obj,
	bool p0);

   public  void setEmpty(bool p0)
   {
      gmtl_AABox_double__setEmpty__bool1(mRawObject, p0);
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class gmtl.AABoxd

/// <summary>
/// Custom marshaler for gmtl.AABoxd.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class AABoxdMarshaler : ICustomMarshaler
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
      return ((gmtl.AABoxd) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.AABoxd(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static AABoxdMarshaler mInstance = new AABoxdMarshaler();
}


} // namespace gmtl
