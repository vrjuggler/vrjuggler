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

public sealed class Quatd
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
   protected Quatd(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Quat_double__Quat__0();

   public Quatd()
   {
      mRawObject   = gmtl_Quat_double__Quat__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Quat_double__Quat__double_double_double_double4(double p0, double p1, double p2, double p3);

   public Quatd(double p0, double p1, double p2, double p3)
   {
      mRawObject   = gmtl_Quat_double__Quat__double_double_double_double4(p0, p1, p2, p3);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Quat_double__Quat__gmtl_Quatd1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatdMarshaler))] gmtl.Quatd p0);

   public Quatd(gmtl.Quatd p0)
   {
      mRawObject   = gmtl_Quat_double__Quat__gmtl_Quatd1(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Quatd(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Quatd(IntPtr obj);

   // Destructor.
   ~Quatd()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Quatd(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Quat_double__set__double_double_double_double4(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3);

   public  void set(double p0, double p1, double p2, double p3)
   {
      gmtl_Quat_double__set__double_double_double_double4(mRawObject, p0, p1, p2, p3);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Quat_double__get__double_double_double_double4(IntPtr obj,
	ref double p0,
	ref double p1,
	ref double p2,
	ref double p3);

   public  void get(ref double p0, ref double p1, ref double p2, ref double p3)
   {
      gmtl_Quat_double__get__double_double_double_double4(mRawObject, ref p0, ref p1, ref p2, ref p3);
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_Quat_double__getData__0(IntPtr obj);

   public  double getData()
   {
      double result;
      result = gmtl_Quat_double__getData__0(mRawObject);
      return result;
   }
*/

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Quat<double>.Params.
   public enum Params
   {
      Size = 4
   };


} // class gmtl.Quatd

/// <summary>
/// Custom marshaler for gmtl.Quatd.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class QuatdMarshaler : ICustomMarshaler
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
      return ((gmtl.Quatd) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Quatd(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static QuatdMarshaler mInstance = new QuatdMarshaler();
}


} // namespace gmtl
