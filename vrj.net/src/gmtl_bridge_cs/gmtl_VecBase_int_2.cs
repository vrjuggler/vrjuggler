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

public class VecBase_int_2
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected VecBase_int_2(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_int_2__VecBase__0();

   public VecBase_int_2()
   {
      mRawObject   = gmtl_VecBase_int_2__VecBase__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_int_2__VecBase__gmtl_VecBase_int_21([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_int_2Marshaler))] gmtl.VecBase_int_2 p0);

   public VecBase_int_2(gmtl.VecBase_int_2 p0)
   {
      mRawObject   = gmtl_VecBase_int_2__VecBase__gmtl_VecBase_int_21(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_int_2__VecBase__int_int2(int p0, int p1);

   public VecBase_int_2(int p0, int p1)
   {
      mRawObject   = gmtl_VecBase_int_2__VecBase__int_int2(p0, p1);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_int_2__VecBase__int_int_int3(int p0, int p1, int p2);

   public VecBase_int_2(int p0, int p1, int p2)
   {
      mRawObject   = gmtl_VecBase_int_2__VecBase__int_int_int3(p0, p1, p2);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_int_2__VecBase__int_int_int_int4(int p0, int p1, int p2, int p3);

   public VecBase_int_2(int p0, int p1, int p2, int p3)
   {
      mRawObject   = gmtl_VecBase_int_2__VecBase__int_int_int_int4(p0, p1, p2, p3);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal VecBase_int_2(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_VecBase_int_2(IntPtr obj);

   // Destructor.
   ~VecBase_int_2()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_VecBase_int_2(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_int_2__set__int1(IntPtr obj,
	int p0);

   public  void set(int p0)
   {
      gmtl_VecBase_int_2__set__int1(mRawObject, p0);
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_int_2__set__int1(IntPtr obj,
	int p0);

   public  void set(int p0)
   {
      gmtl_VecBase_int_2__set__int1(mRawObject, p0);
   }
*/

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_int_2__set__int_int2(IntPtr obj,
	int p0,
	int p1);

   public  void set(int p0, int p1)
   {
      gmtl_VecBase_int_2__set__int_int2(mRawObject, p0, p1);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_int_2__set__int_int_int3(IntPtr obj,
	int p0,
	int p1,
	int p2);

   public  void set(int p0, int p1, int p2)
   {
      gmtl_VecBase_int_2__set__int_int_int3(mRawObject, p0, p1, p2);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_int_2__set__int_int_int_int4(IntPtr obj,
	int p0,
	int p1,
	int p2,
	int p3);

   public  void set(int p0, int p1, int p2, int p3)
   {
      gmtl_VecBase_int_2__set__int_int_int_int4(mRawObject, p0, p1, p2, p3);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_int_2__getData__0(IntPtr obj,
	[In, Out] int[] arrayHolder);

   public  int[] getData()
   {
      int[] array_holder = new int[2];
      gmtl_VecBase_int_2__getData__0(mRawObject, array_holder);
      return array_holder;
   }


   // End of non-virtual methods.

   // Nested enumeration gmtl.VecBase<int,2>.Params.
   public enum Params
   {
      Size = 2
   };


} // class gmtl.VecBase_int_2

/// <summary>
/// Custom marshaler for gmtl.VecBase_int_2.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class VecBase_int_2Marshaler : ICustomMarshaler
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
      return ((gmtl.VecBase_int_2) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.VecBase_int_2(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static VecBase_int_2Marshaler mInstance = new VecBase_int_2Marshaler();
}


} // namespace gmtl
