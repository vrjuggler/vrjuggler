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

public sealed class EulerAngleZYXf
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected EulerAngleZYXf(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__();

   public EulerAngleZYXf()
   {
      mRawObject   = gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__gmtl_EulerAngleZYXf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.EulerAngleZYXfMarshaler))] gmtl.EulerAngleZYXf p0);

   public EulerAngleZYXf(gmtl.EulerAngleZYXf p0)
   {
      
      mRawObject   = gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__gmtl_EulerAngleZYXf(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__float_float_float(float p0,
	float p1,
	float p2);

   public EulerAngleZYXf(float p0, float p1, float p2)
   {
      mRawObject   = gmtl_EulerAngle_float_gmtl_ZYX__EulerAngle__float_float_float(p0, p1, p2);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal EulerAngleZYXf(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_EulerAngleZYXf(IntPtr obj);

   // Destructor.
   ~EulerAngleZYXf()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_EulerAngleZYXf(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_EulerAngle_float_gmtl_ZYX__set__float_float_float3(IntPtr obj,
	float p0,
	float p1,
	float p2);

   public  void set(float p0, float p1, float p2)
   {
      gmtl_EulerAngle_float_gmtl_ZYX__set__float_float_float3(mRawObject, p0, p1, p2);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_EulerAngle_float_gmtl_ZYX__getData__0(IntPtr obj);

   public  float getData()
   {
      float result;
      result = gmtl_EulerAngle_float_gmtl_ZYX__getData__0(mRawObject);
      return result;
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_EulerAngle_float_gmtl_ZYX__getData__0(IntPtr obj);

   public  float getData()
   {
      float result;
      result = gmtl_EulerAngle_float_gmtl_ZYX__getData__0(mRawObject);
      return result;
   }
*/

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.EulerAngle<float,gmtl.ZYX>.Params.
   public enum Params
   {
      Order = 1, Size = 3
   };


} // class gmtl.EulerAngleZYXf

/// <summary>
/// Custom marshaler for gmtl.EulerAngleZYXf.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class EulerAngleZYXfMarshaler : ICustomMarshaler
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
      return ((gmtl.EulerAngleZYXf) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.EulerAngleZYXf(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static EulerAngleZYXfMarshaler mInstance = new EulerAngleZYXfMarshaler();
}


} // namespace gmtl
