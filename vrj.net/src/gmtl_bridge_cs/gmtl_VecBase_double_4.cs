#region License
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
#endregion License

// Generated from Revision: 1.78 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public class VecBase_double_4
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
   protected VecBase_double_4(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_4__VecBase__0();

   public VecBase_double_4()
   {
      mRawObject   = gmtl_VecBase_double_4__VecBase__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_4__VecBase__gmtl_VecBase_double_41([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_4Marshaler))] gmtl.VecBase_double_4 p0);

   public VecBase_double_4(gmtl.VecBase_double_4 p0)
   {
      mRawObject   = gmtl_VecBase_double_4__VecBase__gmtl_VecBase_double_41(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_4__VecBase__double_double2(double p0, double p1);

   public VecBase_double_4(double p0, double p1)
   {
      mRawObject   = gmtl_VecBase_double_4__VecBase__double_double2(p0, p1);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_4__VecBase__double_double_double3(double p0, double p1, double p2);

   public VecBase_double_4(double p0, double p1, double p2)
   {
      mRawObject   = gmtl_VecBase_double_4__VecBase__double_double_double3(p0, p1, p2);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_double_4__VecBase__double_double_double_double4(double p0, double p1, double p2, double p3);

   public VecBase_double_4(double p0, double p1, double p2, double p3)
   {
      mRawObject   = gmtl_VecBase_double_4__VecBase__double_double_double_double4(p0, p1, p2, p3);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal VecBase_double_4(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_VecBase_double_4(IntPtr obj);

   // Destructor.
   ~VecBase_double_4()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_VecBase_double_4(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_4__set__double1(IntPtr obj,
	double p0);

   public  void set(double p0)
   {
      gmtl_VecBase_double_4__set__double1(mRawObject, p0);
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_4__set__double1(IntPtr obj,
	double p0);

   public  void set(double p0)
   {
      gmtl_VecBase_double_4__set__double1(mRawObject, p0);
   }
*/

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_4__set__double_double2(IntPtr obj,
	double p0,
	double p1);

   public  void set(double p0, double p1)
   {
      gmtl_VecBase_double_4__set__double_double2(mRawObject, p0, p1);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_4__set__double_double_double3(IntPtr obj,
	double p0,
	double p1,
	double p2);

   public  void set(double p0, double p1, double p2)
   {
      gmtl_VecBase_double_4__set__double_double_double3(mRawObject, p0, p1, p2);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_4__set__double_double_double_double4(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3);

   public  void set(double p0, double p1, double p2, double p3)
   {
      gmtl_VecBase_double_4__set__double_double_double_double4(mRawObject, p0, p1, p2, p3);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_double_4__getData__0(IntPtr obj,
	[In, Out] double[] arrayHolder);

   public  double[] getData()
   {
      double[] array_holder = new double[4];
      gmtl_VecBase_double_4__getData__0(mRawObject, array_holder);
      return array_holder;
   }


   // End of non-virtual methods.

   // Nested enumeration gmtl.VecBase<double,4>.Params.
   public enum Params
   {
      Size = 4
   };


} // class gmtl.VecBase_double_4

/// <summary>
/// Custom marshaler for gmtl.VecBase_double_4.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class VecBase_double_4Marshaler : ICustomMarshaler
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
      return ((gmtl.VecBase_double_4) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.VecBase_double_4(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static VecBase_double_4Marshaler mInstance = new VecBase_double_4Marshaler();
}


} // namespace gmtl
