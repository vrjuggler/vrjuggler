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

public sealed class LineSegf
   : gmtl.Rayf
{
   // Constructors.
   protected LineSegf(NoInitTag doInit)
      : base(doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_LineSeg_float__LineSeg__();

   public LineSegf()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_LineSeg_float__LineSeg__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_LineSeg_float__LineSeg__gmtl_Point3f_gmtl_Vec3f(ref int p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public LineSegf(ref int p0, gmtl.Vec3f p1)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      mRawObject   = gmtl_LineSeg_float__LineSeg__gmtl_Point3f_gmtl_Vec3f(ref p0, p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_LineSeg_float__LineSeg__gmtl_LineSegf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.LineSegfMarshaler))] gmtl.LineSegf p0);

   public LineSegf(gmtl.LineSegf p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      mRawObject   = gmtl_LineSeg_float__LineSeg__gmtl_LineSegf(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_LineSeg_float__LineSeg__gmtl_Point3f_gmtl_Point3f(ref int p0,
	ref int p1);

   public LineSegf(ref int p0, ref int p1)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      mRawObject   = gmtl_LineSeg_float__LineSeg__gmtl_Point3f_gmtl_Point3f(ref p0, ref p1);
      mWeOwnMemory = true;
      
      
   }

   // Internal constructor needed for marshaling purposes.
   internal LineSegf(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_LineSegf(IntPtr obj);

   // Destructor.
   ~LineSegf()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_LineSegf(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_LineSeg_float__getLength__(IntPtr obj);

   public  float getLength()
   {
      float result;
      result = gmtl_LineSeg_float__getLength__(mRawObject);
      return result;
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class gmtl.LineSegf

/// <summary>
/// Custom marshaler for gmtl.LineSegf.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class LineSegfMarshaler : ICustomMarshaler
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
      return ((gmtl.LineSegf) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.LineSegf(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static LineSegfMarshaler mInstance = new LineSegfMarshaler();
}


} // namespace gmtl
