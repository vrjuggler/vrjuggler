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

public sealed class Matrix44d
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Matrix44d(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Matrix_double_4_4__Matrix__();

   public Matrix44d()
   {
      mRawObject   = gmtl_Matrix_double_4_4__Matrix__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Matrix_double_4_4__Matrix__gmtl_Matrix44d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44dMarshaler))] gmtl.Matrix44d p0);

   public Matrix44d(gmtl.Matrix44d p0)
   {
      
      mRawObject   = gmtl_Matrix_double_4_4__Matrix__gmtl_Matrix44d(p0);
      mWeOwnMemory = true;
      
   }

   // Internal constructor needed for marshaling purposes.
   internal Matrix44d(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Matrix44d(IntPtr obj);

   // Destructor.
   ~Matrix44d()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Matrix44d(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_4_4__set__double_double_double_double4(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3);

   public  void set(double p0, double p1, double p2, double p3)
   {
      gmtl_Matrix_double_4_4__set__double_double_double_double4(mRawObject, p0, p1, p2, p3);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_4_4__set__double_double_double_double_double_double6(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3,
	double p4,
	double p5);

   public  void set(double p0, double p1, double p2, double p3, double p4, double p5)
   {
      gmtl_Matrix_double_4_4__set__double_double_double_double_double_double6(mRawObject, p0, p1, p2, p3, p4, p5);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_4_4__set__double_double_double_double_double_double_double_double_double9(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3,
	double p4,
	double p5,
	double p6,
	double p7,
	double p8);

   public  void set(double p0, double p1, double p2, double p3, double p4, double p5, double p6, double p7, double p8)
   {
      gmtl_Matrix_double_4_4__set__double_double_double_double_double_double_double_double_double9(mRawObject, p0, p1, p2, p3, p4, p5, p6, p7, p8);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_4_4__set__double_double_double_double_double_double_double_double_double_double_double_double12(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3,
	double p4,
	double p5,
	double p6,
	double p7,
	double p8,
	double p9,
	double p10,
	double p11);

   public  void set(double p0, double p1, double p2, double p3, double p4, double p5, double p6, double p7, double p8, double p9, double p10, double p11)
   {
      gmtl_Matrix_double_4_4__set__double_double_double_double_double_double_double_double_double_double_double_double12(mRawObject, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_4_4__set__double_double_double_double_double_double_double_double_double_double_double_double_double_double_double_double16(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3,
	double p4,
	double p5,
	double p6,
	double p7,
	double p8,
	double p9,
	double p10,
	double p11,
	double p12,
	double p13,
	double p14,
	double p15);

   public  void set(double p0, double p1, double p2, double p3, double p4, double p5, double p6, double p7, double p8, double p9, double p10, double p11, double p12, double p13, double p14, double p15)
   {
      gmtl_Matrix_double_4_4__set__double_double_double_double_double_double_double_double_double_double_double_double_double_double_double_double16(mRawObject, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_4_4__set__double1(IntPtr obj,
	ref double p0);

   public  void set(ref double p0)
   {
      gmtl_Matrix_double_4_4__set__double1(mRawObject, ref p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_4_4__setTranspose__double1(IntPtr obj,
	ref double p0);

   public  void setTranspose(ref double p0)
   {
      gmtl_Matrix_double_4_4__setTranspose__double1(mRawObject, ref p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_Matrix_double_4_4__getData__0(IntPtr obj);

   public  double getData()
   {
      double result;
      result = gmtl_Matrix_double_4_4__getData__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_Matrix_double_4_4__isError__0(IntPtr obj);

   public  bool isError()
   {
      bool result;
      result = gmtl_Matrix_double_4_4__isError__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_4_4__setError__0(IntPtr obj);

   public  void setError()
   {
      gmtl_Matrix_double_4_4__setError__0(mRawObject);
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Matrix<double,4,4>.Params.
   public enum Params
   {
      Rows = 4, Cols = 4
   };

   // Nested enumeration gmtl.Matrix<double,4,4>.XformState.
   public enum XformState
   {
      TRANS = 2, FULL = 64, ORTHOGONAL = 4, AFFINE = 16, NON_UNISCALE = 32, XFORM_ERROR = 128, IDENTITY = 1
   };


   // Nested class gmtl.Matrix44d.RowAccessor.
public interface RowAccessor
{
}

/// <summary>
/// Custom marshaler for gmtl.Matrix44d.RowAccessor.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class RowAccessorMarshaler : ICustomMarshaler
{
   private class DummyRowAccessor : gmtl.Matrix44d.RowAccessor
   {
      protected internal IntPtr mRawObject = IntPtr.Zero;

      internal IntPtr RawObject
      {
         get { return mRawObject; }
      }

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
      // Try the fast return method first.  If it fails, catch the exception
      // and use the slow technique instead.
      try
      {
         return ((DummyRowAccessor) obj).mRawObject;
      }
      catch(System.InvalidCastException ex)
      {
         PropertyInfo raw_obj_prop =
            obj.GetType().GetProperty("RawObject",
                                      BindingFlags.NonPublic | BindingFlags.Instance);
         if ( null != raw_obj_prop )
         {
            return (IntPtr) raw_obj_prop.GetValue(obj, null);
         }
      }

      // Marshaling failed.
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


   // Nested class gmtl.Matrix44d.ConstRowAccessor.
public interface ConstRowAccessor
{
}

/// <summary>
/// Custom marshaler for gmtl.Matrix44d.ConstRowAccessor.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ConstRowAccessorMarshaler : ICustomMarshaler
{
   private class DummyConstRowAccessor : gmtl.Matrix44d.ConstRowAccessor
   {
      protected internal IntPtr mRawObject = IntPtr.Zero;

      internal IntPtr RawObject
      {
         get { return mRawObject; }
      }

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
      // Try the fast return method first.  If it fails, catch the exception
      // and use the slow technique instead.
      try
      {
         return ((DummyConstRowAccessor) obj).mRawObject;
      }
      catch(System.InvalidCastException ex)
      {
         PropertyInfo raw_obj_prop =
            obj.GetType().GetProperty("RawObject",
                                      BindingFlags.NonPublic | BindingFlags.Instance);
         if ( null != raw_obj_prop )
         {
            return (IntPtr) raw_obj_prop.GetValue(obj, null);
         }
      }

      // Marshaling failed.
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


} // class gmtl.Matrix44d

/// <summary>
/// Custom marshaler for gmtl.Matrix44d.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class Matrix44dMarshaler : ICustomMarshaler
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
      return ((gmtl.Matrix44d) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Matrix44d(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static Matrix44dMarshaler mInstance = new Matrix44dMarshaler();
}


} // namespace gmtl
