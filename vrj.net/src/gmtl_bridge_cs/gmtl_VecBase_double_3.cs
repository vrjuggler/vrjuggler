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

public class VecBase_double_3
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected VecBase_double_3(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_3__VecBase__();

   public VecBase_double_3()
   {
      mRawObject   = gmtl_VecBase_double_3__VecBase__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_3__VecBase__gmtl_VecBase_double_3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_3Marshaler))] gmtl.VecBase_double_3 p0);

   public VecBase_double_3(gmtl.VecBase_double_3 p0)
   {
      
      mRawObject   = gmtl_VecBase_double_3__VecBase__gmtl_VecBase_double_3(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_3__VecBase__double_double(double p0,
	double p1);

   public VecBase_double_3(double p0, double p1)
   {
      mRawObject   = gmtl_VecBase_double_3__VecBase__double_double(p0, p1);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_3__VecBase__double_double_double(double p0,
	double p1,
	double p2);

   public VecBase_double_3(double p0, double p1, double p2)
   {
      mRawObject   = gmtl_VecBase_double_3__VecBase__double_double_double(p0, p1, p2);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_3__VecBase__double_double_double_double(double p0,
	double p1,
	double p2,
	double p3);

   public VecBase_double_3(double p0, double p1, double p2, double p3)
   {
      mRawObject   = gmtl_VecBase_double_3__VecBase__double_double_double_double(p0, p1, p2, p3);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal VecBase_double_3(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_VecBase_double_3(IntPtr obj);

   // Destructor.
   ~VecBase_double_3()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_VecBase_double_3(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_3__set__double1(IntPtr obj,
	double p0);

   public  void set(double p0)
   {
      gmtl_VecBase_double_3__set__double1(mRawObject, p0);
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_3__set__double1(IntPtr obj,
	double p0);

   public  void set(double p0)
   {
      gmtl_VecBase_double_3__set__double1(mRawObject, p0);
   }
*/

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_3__set__double_double2(IntPtr obj,
	double p0,
	double p1);

   public  void set(double p0, double p1)
   {
      gmtl_VecBase_double_3__set__double_double2(mRawObject, p0, p1);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_3__set__double_double_double3(IntPtr obj,
	double p0,
	double p1,
	double p2);

   public  void set(double p0, double p1, double p2)
   {
      gmtl_VecBase_double_3__set__double_double_double3(mRawObject, p0, p1, p2);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_3__set__double_double_double_double4(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3);

   public  void set(double p0, double p1, double p2, double p3)
   {
      gmtl_VecBase_double_3__set__double_double_double_double4(mRawObject, p0, p1, p2, p3);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_VecBase_double_3__getData__0(IntPtr obj);

   public  double getData()
   {
      double result;
      result = gmtl_VecBase_double_3__getData__0(mRawObject);
      return result;
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_VecBase_double_3__getData__0(IntPtr obj);

   public  double getData()
   {
      double result;
      result = gmtl_VecBase_double_3__getData__0(mRawObject);
      return result;
   }
*/

   // End of non-virtual methods.

   // Nested enumeration gmtl.VecBase<double,3>.Params.
   public enum Params
   {
      Size = 3
   };


} // class gmtl.VecBase_double_3

/// <summary>
/// Custom marshaler for gmtl.VecBase_double_3.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class VecBase_double_3Marshaler : ICustomMarshaler
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
      return ((gmtl.VecBase_double_3) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.VecBase_double_3(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static VecBase_double_3Marshaler mInstance = new VecBase_double_3Marshaler();
}


} // namespace gmtl
