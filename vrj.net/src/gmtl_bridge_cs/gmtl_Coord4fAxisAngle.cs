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

// Generated from Revision: 1.70 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public sealed class Coord4fAxisAngle
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Coord4fAxisAngle(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__();

   public Coord4fAxisAngle()
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__gmtl_Coord4fAxisAngle([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord4fAxisAngleMarshaler))] gmtl.Coord4fAxisAngle p0);

   public Coord4fAxisAngle(gmtl.Coord4fAxisAngle p0)
   {
      
      mRawObject   = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__gmtl_Coord4fAxisAngle(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__gmtl_Vec4f_gmtl_AxisAnglef([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))] gmtl.Vec4f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))] gmtl.AxisAnglef p1);

   public Coord4fAxisAngle(gmtl.Vec4f p0, gmtl.AxisAnglef p1)
   {
      
      
      mRawObject   = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__gmtl_Vec4f_gmtl_AxisAnglef(p0, p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__float_float_float_float_float_float(float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5);

   public Coord4fAxisAngle(float p0, float p1, float p2, float p3, float p4, float p5)
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__float_float_float_float_float_float(p0, p1, p2, p3, p4, p5);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__float_float_float_float_float_float_float(float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5,
	float p6);

   public Coord4fAxisAngle(float p0, float p1, float p2, float p3, float p4, float p5, float p6)
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__float_float_float_float_float_float_float(p0, p1, p2, p3, p4, p5, p6);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__float_float_float_float_float_float_float_float(float p0,
	float p1,
	float p2,
	float p3,
	float p4,
	float p5,
	float p6,
	float p7);

   public Coord4fAxisAngle(float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7)
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____Coord__float_float_float_float_float_float_float_float(p0, p1, p2, p3, p4, p5, p6, p7);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Coord4fAxisAngle(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Coord4fAxisAngle(IntPtr obj);

   // Destructor.
   ~Coord4fAxisAngle()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Coord4fAxisAngle(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))]
   private extern static gmtl.Vec4f gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____getPos__(IntPtr obj);

   public  gmtl.Vec4f getPos()
   {
      gmtl.Vec4f result;
      result = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____getPos__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))]
   private extern static gmtl.AxisAnglef gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____getRot__(IntPtr obj);

   public  gmtl.AxisAnglef getRot()
   {
      gmtl.AxisAnglef result;
      result = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____getRot__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec4fMarshaler))]
   private extern static gmtl.Vec4f gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____pos__(IntPtr obj);

   public  gmtl.Vec4f pos()
   {
      gmtl.Vec4f result;
      result = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____pos__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAnglefMarshaler))]
   private extern static gmtl.AxisAnglef gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____rot__(IntPtr obj);

   public  gmtl.AxisAnglef rot()
   {
      gmtl.AxisAnglef result;
      result = gmtl_Coord_gmtl_Vec_float_4__gmtl_AxisAngle_float____rot__(mRawObject);
      return result;
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Coord<gmtl.Vec<float,4>,gmtl.AxisAngle<float> >.Params.
   public enum Params
   {
      RotSize = 4, PosSize = 4
   };


} // class gmtl.Coord4fAxisAngle

/// <summary>
/// Custom marshaler for gmtl.Coord4fAxisAngle.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class Coord4fAxisAngleMarshaler : ICustomMarshaler
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
      return ((gmtl.Coord4fAxisAngle) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Coord4fAxisAngle(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static Coord4fAxisAngleMarshaler mInstance = new Coord4fAxisAngleMarshaler();
}


} // namespace gmtl
