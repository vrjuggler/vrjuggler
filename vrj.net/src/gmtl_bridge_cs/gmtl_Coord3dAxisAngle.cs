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

public sealed class Coord3dAxisAngle
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Coord3dAxisAngle(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__();

   public Coord3dAxisAngle()
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__gmtl_Coord3dAxisAngle([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Coord3dAxisAngleMarshaler))] gmtl.Coord3dAxisAngle p0);

   public Coord3dAxisAngle(gmtl.Coord3dAxisAngle p0)
   {
      
      mRawObject   = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__gmtl_Coord3dAxisAngle(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__gmtl_Vec3d_gmtl_AxisAngled([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))] gmtl.AxisAngled p1);

   public Coord3dAxisAngle(gmtl.Vec3d p0, gmtl.AxisAngled p1)
   {
      
      
      mRawObject   = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__gmtl_Vec3d_gmtl_AxisAngled(p0, p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__double_double_double_double_double_double(double p0,
	double p1,
	double p2,
	double p3,
	double p4,
	double p5);

   public Coord3dAxisAngle(double p0, double p1, double p2, double p3, double p4, double p5)
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__double_double_double_double_double_double(p0, p1, p2, p3, p4, p5);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__double_double_double_double_double_double_double(double p0,
	double p1,
	double p2,
	double p3,
	double p4,
	double p5,
	double p6);

   public Coord3dAxisAngle(double p0, double p1, double p2, double p3, double p4, double p5, double p6)
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__double_double_double_double_double_double_double(p0, p1, p2, p3, p4, p5, p6);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__double_double_double_double_double_double_double_double(double p0,
	double p1,
	double p2,
	double p3,
	double p4,
	double p5,
	double p6,
	double p7);

   public Coord3dAxisAngle(double p0, double p1, double p2, double p3, double p4, double p5, double p6, double p7)
   {
      mRawObject   = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____Coord__double_double_double_double_double_double_double_double(p0, p1, p2, p3, p4, p5, p6, p7);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Coord3dAxisAngle(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Coord3dAxisAngle(IntPtr obj);

   // Destructor.
   ~Coord3dAxisAngle()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Coord3dAxisAngle(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____getPos__(IntPtr obj);

   public  gmtl.Vec3d getPos()
   {
      gmtl.Vec3d result;
      result = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____getPos__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))]
   private extern static gmtl.AxisAngled gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____getRot__(IntPtr obj);

   public  gmtl.AxisAngled getRot()
   {
      gmtl.AxisAngled result;
      result = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____getRot__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))]
   private extern static gmtl.Vec3d gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____pos__(IntPtr obj);

   public  gmtl.Vec3d pos()
   {
      gmtl.Vec3d result;
      result = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____pos__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.AxisAngledMarshaler))]
   private extern static gmtl.AxisAngled gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____rot__(IntPtr obj);

   public  gmtl.AxisAngled rot()
   {
      gmtl.AxisAngled result;
      result = gmtl_Coord_gmtl_Vec_double_3__gmtl_AxisAngle_double____rot__(mRawObject);
      return result;
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Coord<gmtl.Vec<double,3>,gmtl.AxisAngle<double> >.Params.
   public enum Params
   {
      RotSize = 4, PosSize = 3
   };


} // class gmtl.Coord3dAxisAngle

/// <summary>
/// Custom marshaler for gmtl.Coord3dAxisAngle.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class Coord3dAxisAngleMarshaler : ICustomMarshaler
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
      return ((gmtl.Coord3dAxisAngle) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Coord3dAxisAngle(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static Coord3dAxisAngleMarshaler mInstance = new Coord3dAxisAngleMarshaler();
}


} // namespace gmtl
