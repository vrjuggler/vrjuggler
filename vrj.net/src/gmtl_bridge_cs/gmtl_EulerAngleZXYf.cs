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

public sealed class EulerAngleZXYf
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected EulerAngleZXYf(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__0();

   public EulerAngleZXYf()
   {
      mRawObject   = gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__gmtl_EulerAngleZXYf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZXYfMarshaler))] gmtl.EulerAngleZXYf p0);

   public EulerAngleZXYf(gmtl.EulerAngleZXYf p0)
   {
      mRawObject   = gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__gmtl_EulerAngleZXYf1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__float_float_float3(float p0, float p1, float p2);

   public EulerAngleZXYf(float p0, float p1, float p2)
   {
      mRawObject   = gmtl_EulerAngle_float_gmtl_ZXY__EulerAngle__float_float_float3(p0, p1, p2);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal EulerAngleZXYf(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_EulerAngleZXYf(IntPtr obj);

   // Destructor.
   ~EulerAngleZXYf()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_EulerAngleZXYf(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_EulerAngle_float_gmtl_ZXY__set__float_float_float3(IntPtr obj,
	float p0,
	float p1,
	float p2);

   public  void set(float p0, float p1, float p2)
   {
      gmtl_EulerAngle_float_gmtl_ZXY__set__float_float_float3(mRawObject, p0, p1, p2);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_EulerAngle_float_gmtl_ZXY__getData__0(IntPtr obj,
	[In, Out] float[] arrayHolder);

   public  float[] getData()
   {
      float[] array_holder = new float[3];
      gmtl_EulerAngle_float_gmtl_ZXY__getData__0(mRawObject, array_holder);
      return array_holder;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.EulerAngle<float,gmtl.ZXY>.Params.
   public enum Params
   {
      Order = 2, Size = 3
   };


} // class gmtl.EulerAngleZXYf

/// <summary>
/// Custom marshaler for gmtl.EulerAngleZXYf.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class EulerAngleZXYfMarshaler : ICustomMarshaler
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
      return ((gmtl.EulerAngleZXYf) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.EulerAngleZXYf(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static EulerAngleZXYfMarshaler mInstance = new EulerAngleZXYfMarshaler();
}


} // namespace gmtl
