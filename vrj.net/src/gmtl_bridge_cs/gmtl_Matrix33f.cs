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

public class Matrix33f
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Matrix33f(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Matrix_float_3_3__Matrix__();

   public Matrix33f()
   {
      mRawObject   = gmtl_Matrix_float_3_3__Matrix__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Matrix_float_3_3__Matrix__gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public Matrix33f(gmtl.Matrix33f p0)
   {
      
      mRawObject   = gmtl_Matrix_float_3_3__Matrix__gmtl_Matrix33f(p0);
      mWeOwnMemory = true;
      
   }

   // Internal constructor needed for marshaling purposes.
   internal Matrix33f(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Matrix33f(IntPtr obj);

   // Destructor.
   ~Matrix33f()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Matrix33f(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.


   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_float_3_3__set__float_float_float_float(IntPtr obj,
	float p0,
	float p1,
	float p2,
	float p3);

   public  void set(float p0, float p1, float p2, float p3)
   {
      gmtl_Matrix_float_3_3__set__float_float_float_float(mRawObject, p0, p1, p2, p3);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_float_3_3__set__float_float_float_float_float_float(IntPtr obj,
	float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5);

   public  void set(float p0, float p1, float p2, float p3, float p4, float p5)
   {
      gmtl_Matrix_float_3_3__set__float_float_float_float_float_float(mRawObject, p0, p1, p2, p3, p4, p5);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_float_3_3__set__float_float_float_float_float_float_float_float_float(IntPtr obj,
	float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5,
	float p6,
	float p7,
	float p8);

   public  void set(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8)
   {
      gmtl_Matrix_float_3_3__set__float_float_float_float_float_float_float_float_float(mRawObject, p0, p1, p2, p3, p4, p5, p6, p7, p8);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_float_3_3__set__float_float_float_float_float_float_float_float_float_float_float_float(IntPtr obj,
	float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5,
	float p6,
	float p7,
	float p8,
	float p9,
	float p10,
	float p11);

   public  void set(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11)
   {
      gmtl_Matrix_float_3_3__set__float_float_float_float_float_float_float_float_float_float_float_float(mRawObject, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_float_3_3__set__float_float_float_float_float_float_float_float_float_float_float_float_float_float_float_float(IntPtr obj,
	float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5,
	float p6,
	float p7,
	float p8,
	float p9,
	float p10,
	float p11,
	float p12,
	float p13,
	float p14,
	float p15);

   public  void set(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10, float p11, float p12, float p13, float p14, float p15)
   {
      gmtl_Matrix_float_3_3__set__float_float_float_float_float_float_float_float_float_float_float_float_float_float_float_float(mRawObject, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_float_3_3__set__float(IntPtr obj,
	ref float p0);

   public  void set(ref float p0)
   {
      
      gmtl_Matrix_float_3_3__set__float(mRawObject, ref p0);
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_float_3_3__setTranspose__float(IntPtr obj,
	ref float p0);

   public  void setTranspose(ref float p0)
   {
      
      gmtl_Matrix_float_3_3__setTranspose__float(mRawObject, ref p0);
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_Matrix_float_3_3__getData__(IntPtr obj);

   public  float getData()
   {
      float result;
      result = gmtl_Matrix_float_3_3__getData__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_Matrix_float_3_3__isError__(IntPtr obj);

   public  bool isError()
   {
      bool result;
      result = gmtl_Matrix_float_3_3__isError__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_float_3_3__setError__(IntPtr obj);

   public  void setError()
   {
      gmtl_Matrix_float_3_3__setError__(mRawObject);
   }

   // End of non-virtual methods.

   // Nested enumeration gmtl.Matrix<float,3,3>.Params.
   public enum Params
   {
      Rows = 3, Cols = 3
   };

   // Nested enumeration gmtl.Matrix<float,3,3>.XformState.
   public enum XformState
   {
      TRANS = 2, FULL = 64, ORTHOGONAL = 4, AFFINE = 16, NON_UNISCALE = 32, XFORM_ERROR = 128, IDENTITY = 1
   };


   // Nested class gmtl.Matrix33f.RowAccessor.
public interface RowAccessor
{
}

/// <summary>
/// Custom marshaler for gmtl.Matrix33f.RowAccessor.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class RowAccessorMarshaler : ICustomMarshaler
{
   private class DummyRowAccessor : gmtl.Matrix33f.RowAccessor
   {
      protected internal IntPtr mRawObject = IntPtr.Zero;

      public DummyRowAccessor(IntPtr rawObject)
      {
         mRawObject = rawObject;
      }

   }

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
      PropertyInfo raw_obj_prop =
         obj.GetType().GetProperty("RawObject",
                                   BindingFlags.NonPublic | BindingFlags.Instance);
      if ( null != raw_obj_prop )
      {
         return (IntPtr) raw_obj_prop.GetValue(obj, null);
      }

      return IntPtr.Zero;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyRowAccessor(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static RowAccessorMarshaler mInstance = new RowAccessorMarshaler();
}


   // Nested class gmtl.Matrix33f.ConstRowAccessor.
public interface ConstRowAccessor
{
}

/// <summary>
/// Custom marshaler for gmtl.Matrix33f.ConstRowAccessor.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ConstRowAccessorMarshaler : ICustomMarshaler
{
   private class DummyConstRowAccessor : gmtl.Matrix33f.ConstRowAccessor
   {
      protected internal IntPtr mRawObject = IntPtr.Zero;

      public DummyConstRowAccessor(IntPtr rawObject)
      {
         mRawObject = rawObject;
      }

   }

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
      PropertyInfo raw_obj_prop =
         obj.GetType().GetProperty("RawObject",
                                   BindingFlags.NonPublic | BindingFlags.Instance);
      if ( null != raw_obj_prop )
      {
         return (IntPtr) raw_obj_prop.GetValue(obj, null);
      }

      return IntPtr.Zero;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyConstRowAccessor(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static ConstRowAccessorMarshaler mInstance = new ConstRowAccessorMarshaler();
}


} // class gmtl.Matrix33f

/// <summary>
/// Custom marshaler for gmtl.Matrix33f.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class Matrix33fMarshaler : ICustomMarshaler
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
      return ((gmtl.Matrix33f) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Matrix33f(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static Matrix33fMarshaler mInstance = new Matrix33fMarshaler();
}


} // namespace gmtl
