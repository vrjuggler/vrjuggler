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

// Generated from $Revision$ of $RCSfile$
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public class VecBase_float_4
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected VecBase_float_4(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_float_4__VecBase__();

   public VecBase_float_4()
   {
      mRawObject   = gmtl_VecBase_float_4__VecBase__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_float_4__VecBase__gmtl_VecBase_float_4([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_float_4Marshaler))] gmtl.VecBase_float_4 p0);

   public VecBase_float_4(gmtl.VecBase_float_4 p0)
   {
      
      mRawObject   = gmtl_VecBase_float_4__VecBase__gmtl_VecBase_float_4(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_float_4__VecBase__float_float(ref float p0,
	ref float p1);

   public VecBase_float_4(ref float p0, ref float p1)
   {
      
      
      mRawObject   = gmtl_VecBase_float_4__VecBase__float_float(ref p0, ref p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_float_4__VecBase__float_float_float(ref float p0,
	ref float p1,
	ref float p2);

   public VecBase_float_4(ref float p0, ref float p1, ref float p2)
   {
      
      
      
      mRawObject   = gmtl_VecBase_float_4__VecBase__float_float_float(ref p0, ref p1, ref p2);
      mWeOwnMemory = true;
      
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_VecBase_float_4__VecBase__float_float_float_float(ref float p0,
	ref float p1,
	ref float p2,
	ref float p3);

   public VecBase_float_4(ref float p0, ref float p1, ref float p2, ref float p3)
   {
      
      
      
      
      mRawObject   = gmtl_VecBase_float_4__VecBase__float_float_float_float(ref p0, ref p1, ref p2, ref p3);
      mWeOwnMemory = true;
      
      
      
      
   }

   // Internal constructor needed for marshaling purposes.
   internal VecBase_float_4(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_VecBase_float_4(IntPtr obj);

   // Destructor.
   ~VecBase_float_4()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_VecBase_float_4(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.


   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_float_4__set__float(IntPtr obj,
	ref float p0);

   public  void set(ref float p0)
   {
      
      gmtl_VecBase_float_4__set__float(mRawObject, ref p0);
      
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_float_4__set__float(IntPtr obj,
	ref float p0);

   public  void set(ref float p0)
   {
      
      gmtl_VecBase_float_4__set__float(mRawObject, ref p0);
      
   }
*/

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_float_4__set__float_float(IntPtr obj,
	ref float p0,
	ref float p1);

   public  void set(ref float p0, ref float p1)
   {
      
      
      gmtl_VecBase_float_4__set__float_float(mRawObject, ref p0, ref p1);
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_float_4__set__float_float_float(IntPtr obj,
	ref float p0,
	ref float p1,
	ref float p2);

   public  void set(ref float p0, ref float p1, ref float p2)
   {
      
      
      
      gmtl_VecBase_float_4__set__float_float_float(mRawObject, ref p0, ref p1, ref p2);
      
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_VecBase_float_4__set__float_float_float_float(IntPtr obj,
	ref float p0,
	ref float p1,
	ref float p2,
	ref float p3);

   public  void set(ref float p0, ref float p1, ref float p2, ref float p3)
   {
      
      
      
      
      gmtl_VecBase_float_4__set__float_float_float_float(mRawObject, ref p0, ref p1, ref p2, ref p3);
      
      
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_VecBase_float_4__getData__(IntPtr obj);

   public  float getData()
   {
      float result;
      result = gmtl_VecBase_float_4__getData__(mRawObject);
      return result;
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_VecBase_float_4__getData__(IntPtr obj);

   public  float getData()
   {
      float result;
      result = gmtl_VecBase_float_4__getData__(mRawObject);
      return result;
   }
*/

   // End of non-virtual methods.

   // Nested enumeration gmtl.VecBase<float,4>.Params.
   public enum Params
   {
      Size = 4
   };


} // class gmtl.VecBase_float_4

/// <summary>
/// Custom marshaler for gmtl.VecBase_float_4.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class VecBase_float_4Marshaler : ICustomMarshaler
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
      return ((gmtl.VecBase_float_4) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.VecBase_float_4(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static VecBase_float_4Marshaler mInstance = new VecBase_float_4Marshaler();
}


} // namespace gmtl
