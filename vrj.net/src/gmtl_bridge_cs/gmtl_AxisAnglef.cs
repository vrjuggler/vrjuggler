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

public sealed class AxisAnglef
   : gmtl.VecBase_float_4
{
   // Constructors.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AxisAngle_float__AxisAngle__0();

   public AxisAnglef()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_AxisAngle_float__AxisAngle__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AxisAngle_float__AxisAngle__gmtl_AxisAnglef1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p0);

   public AxisAnglef(gmtl.AxisAnglef p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_AxisAngle_float__AxisAngle__gmtl_AxisAnglef1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AxisAngle_float__AxisAngle__float_float_float_float4(float p0, float p1, float p2, float p3);

   public AxisAnglef(float p0, float p1, float p2, float p3)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_AxisAngle_float__AxisAngle__float_float_float_float4(p0, p1, p2, p3);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_AxisAngle_float__AxisAngle__float_gmtl_Vec3f2(float p0, [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public AxisAnglef(float p0, gmtl.Vec3f p1)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_AxisAngle_float__AxisAngle__float_gmtl_Vec3f2(p0, p1);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal AxisAnglef(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_AxisAnglef(IntPtr obj);

   // Destructor.
   ~AxisAnglef()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_AxisAnglef(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AxisAngle_float__set__float_float_float_float4(IntPtr obj,
	float p0,
	float p1,
	float p2,
	float p3);

   public new void set(float p0, float p1, float p2, float p3)
   {
      gmtl_AxisAngle_float__set__float_float_float_float4(mRawObject, p0, p1, p2, p3);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AxisAngle_float__set__float_gmtl_Vec3f2(IntPtr obj,
	float p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public new void set(float p0, gmtl.Vec3f p1)
   {
      gmtl_AxisAngle_float__set__float_gmtl_Vec3f2(mRawObject, p0, p1);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AxisAngle_float__setAxis__gmtl_Vec3f1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public  void setAxis(gmtl.Vec3f p0)
   {
      gmtl_AxisAngle_float__setAxis__gmtl_Vec3f1(mRawObject, p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_AxisAngle_float__setAngle__float1(IntPtr obj,
	float p0);

   public  void setAngle(float p0)
   {
      gmtl_AxisAngle_float__setAngle__float1(mRawObject, p0);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_AxisAngle_float__getAxis__0(IntPtr obj);

   public  gmtl.Vec3f getAxis()
   {
      gmtl.Vec3f result;
      result = gmtl_AxisAngle_float__getAxis__0(mRawObject);
      return result;
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_AxisAngle_float__getAngle__0(IntPtr obj);

   public  float getAngle()
   {
      float result;
      result = gmtl_AxisAngle_float__getAngle__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.AxisAngle<float>.Params.
   public enum Params
   {
      Size = 4
   };


} // class gmtl.AxisAnglef

/// <summary>
/// Custom marshaler for gmtl.AxisAnglef.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class AxisAnglefMarshaler : ICustomMarshaler
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
      return ((gmtl.AxisAnglef) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.AxisAnglef(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static AxisAnglefMarshaler mInstance = new AxisAnglefMarshaler();
}


} // namespace gmtl
