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

// Generated from Revision: 1.79 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public sealed class Matrix33d
{
   private IntPtr mRawObject = IntPtr.Zero;
   private bool mWeOwnMemory = false;

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
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Matrix_double_3_3__Matrix__0();

   public Matrix33d()
   {
      mRawObject   = gmtl_Matrix_double_3_3__Matrix__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Matrix_double_3_3__Matrix__gmtl_Matrix33d1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33dMarshaler))] gmtl.Matrix33d p0);

   public Matrix33d(gmtl.Matrix33d p0)
   {
      mRawObject   = gmtl_Matrix_double_3_3__Matrix__gmtl_Matrix33d1(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Matrix33d(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Matrix33d(IntPtr obj);

   // Destructor.
   ~Matrix33d()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Matrix33d(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_3_3__set__double_double_double_double4(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3);

   public  void set(double p0, double p1, double p2, double p3)
   {
      gmtl_Matrix_double_3_3__set__double_double_double_double4(mRawObject, p0, p1, p2, p3);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_3_3__set__double_double_double_double_double_double6(IntPtr obj,
	double p0,
	double p1,
	double p2,
	double p3,
	double p4,
	double p5);

   public  void set(double p0, double p1, double p2, double p3, double p4, double p5)
   {
      gmtl_Matrix_double_3_3__set__double_double_double_double_double_double6(mRawObject, p0, p1, p2, p3, p4, p5);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_3_3__set__double_double_double_double_double_double_double_double_double9(IntPtr obj,
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
      gmtl_Matrix_double_3_3__set__double_double_double_double_double_double_double_double_double9(mRawObject, p0, p1, p2, p3, p4, p5, p6, p7, p8);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_3_3__set__double_double_double_double_double_double_double_double_double_double_double_double12(IntPtr obj,
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
      gmtl_Matrix_double_3_3__set__double_double_double_double_double_double_double_double_double_double_double_double12(mRawObject, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_3_3__set__double_double_double_double_double_double_double_double_double_double_double_double_double_double_double_double16(IntPtr obj,
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
      gmtl_Matrix_double_3_3__set__double_double_double_double_double_double_double_double_double_double_double_double_double_double_double_double16(mRawObject, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_3_3__set__double1(IntPtr obj,
	double p0);

   public  void set(double p0)
   {
      gmtl_Matrix_double_3_3__set__double1(mRawObject, p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_3_3__setTranspose__double1(IntPtr obj,
	double p0);

   public  void setTranspose(double p0)
   {
      gmtl_Matrix_double_3_3__setTranspose__double1(mRawObject, p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_3_3__getData__0(IntPtr obj,
	[In, Out] double[] arrayHolder);

   public  double[] getData()
   {
      double[] array_holder = new double[9];
      gmtl_Matrix_double_3_3__getData__0(mRawObject, array_holder);
      return array_holder;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_Matrix_double_3_3__isError__0(IntPtr obj);

   public  bool isError()
   {
      bool result;
      result = gmtl_Matrix_double_3_3__isError__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Matrix_double_3_3__setError__0(IntPtr obj);

   public  void setError()
   {
      gmtl_Matrix_double_3_3__setError__0(mRawObject);
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Matrix<double,3,3>.Params.
   public enum Params
   {
      Rows = 3, Cols = 3
   };

   // Nested enumeration gmtl.Matrix<double,3,3>.XformState.
   public enum XformState
   {
      XFORM_ERROR = 128, FULL = 64, ORTHOGONAL = 4, AFFINE = 16, TRANS = 2, NON_UNISCALE = 32, IDENTITY = 1
   };


   // Nested class gmtl.Matrix33d.RowAccessor.
public interface RowAccessor
{
}

/// <summary>
/// Custom marshaler for gmtl.Matrix33d.RowAccessor.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class RowAccessorMarshaler : ICustomMarshaler
{
   private class DummyRowAccessor : gmtl.Matrix33d.RowAccessor
   {
      private IntPtr mRawObject = IntPtr.Zero;

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
         return ((DummyRowAccessor) obj).RawObject;
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


   // Nested class gmtl.Matrix33d.ConstRowAccessor.
public interface ConstRowAccessor
{
}

/// <summary>
/// Custom marshaler for gmtl.Matrix33d.ConstRowAccessor.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ConstRowAccessorMarshaler : ICustomMarshaler
{
   private class DummyConstRowAccessor : gmtl.Matrix33d.ConstRowAccessor
   {
      private IntPtr mRawObject = IntPtr.Zero;

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
         return ((DummyConstRowAccessor) obj).RawObject;
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


} // class gmtl.Matrix33d

/// <summary>
/// Custom marshaler for gmtl.Matrix33d.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class Matrix33dMarshaler : ICustomMarshaler
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
      return ((gmtl.Matrix33d) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Matrix33d(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static Matrix33dMarshaler mInstance = new Matrix33dMarshaler();
}


} // namespace gmtl
