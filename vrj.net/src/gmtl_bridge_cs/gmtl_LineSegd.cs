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

public sealed class LineSegd
   : gmtl.Rayd
{
   // Constructors.
   protected LineSegd(NoInitTag doInit)
      : base(doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_LineSeg_double__LineSeg__();

   public LineSegd()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_LineSeg_double__LineSeg__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_LineSeg_double__LineSeg__gmtl_Point_double_3_gmtl_Vec3d(ref int p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3dMarshaler))] gmtl.Vec3d p1);

   public LineSegd(ref int p0, gmtl.Vec3d p1)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      mRawObject   = gmtl_LineSeg_double__LineSeg__gmtl_Point_double_3_gmtl_Vec3d(ref p0, p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_LineSeg_double__LineSeg__gmtl_LineSegd([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegdMarshaler))] gmtl.LineSegd p0);

   public LineSegd(gmtl.LineSegd p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      mRawObject   = gmtl_LineSeg_double__LineSeg__gmtl_LineSegd(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_LineSeg_double__LineSeg__gmtl_Point_double_3_gmtl_Point_double_3(ref int p0,
	ref int p1);

   public LineSegd(ref int p0, ref int p1)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      mRawObject   = gmtl_LineSeg_double__LineSeg__gmtl_Point_double_3_gmtl_Point_double_3(ref p0, ref p1);
      mWeOwnMemory = true;
      
      
   }

   // Internal constructor needed for marshaling purposes.
   internal LineSegd(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_LineSegd(IntPtr obj);

   // Destructor.
   ~LineSegd()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_LineSegd(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static double gmtl_LineSeg_double__getLength__(IntPtr obj);

   public  double getLength()
   {
      double result;
      result = gmtl_LineSeg_double__getLength__(mRawObject);
      return result;
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class gmtl.LineSegd

/// <summary>
/// Custom marshaler for gmtl.LineSegd.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class LineSegdMarshaler : ICustomMarshaler
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
      return ((gmtl.LineSegd) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.LineSegd(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static LineSegdMarshaler mInstance = new LineSegdMarshaler();
}


} // namespace gmtl
