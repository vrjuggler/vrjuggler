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

public sealed class Point2d
   : gmtl.VecBase_double_2
{
   // Constructors.
   protected Point2d(NoInitTag doInit)
      : base(doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_double_2__Point__();

   public Point2d()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_Point_double_2__Point__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_double_2__Point__gmtl_Point2d([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point2dMarshaler))] gmtl.Point2d p0);

   public Point2d(gmtl.Point2d p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      mRawObject   = gmtl_Point_double_2__Point__gmtl_Point2d(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_double_2__Point__gmtl_VecBase_double_2([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_double_2Marshaler))] gmtl.VecBase_double_2 p0);

   public Point2d(gmtl.VecBase_double_2 p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      mRawObject   = gmtl_Point_double_2__Point__gmtl_VecBase_double_2(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_double_2__Point__double_double(ref double p0,
	ref double p1);

   public Point2d(ref double p0, ref double p1)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      mRawObject   = gmtl_Point_double_2__Point__double_double(ref p0, ref p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_double_2__Point__double_double_double(ref double p0,
	ref double p1,
	ref double p2);

   public Point2d(ref double p0, ref double p1, ref double p2)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      
      mRawObject   = gmtl_Point_double_2__Point__double_double_double(ref p0, ref p1, ref p2);
      mWeOwnMemory = true;
      
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_double_2__Point__double_double_double_double(ref double p0,
	ref double p1,
	ref double p2,
	ref double p3);

   public Point2d(ref double p0, ref double p1, ref double p2, ref double p3)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      
      
      mRawObject   = gmtl_Point_double_2__Point__double_double_double_double(ref p0, ref p1, ref p2, ref p3);
      mWeOwnMemory = true;
      
      
      
      
   }

   // Internal constructor needed for marshaling purposes.
   internal Point2d(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Point2d(IntPtr obj);

   // Destructor.
   ~Point2d()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Point2d(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Point<double,2>.Params.
   public enum Params
   {
      Size = 2
   };


} // class gmtl.Point2d

/// <summary>
/// Custom marshaler for gmtl.Point2d.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class Point2dMarshaler : ICustomMarshaler
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
      return ((gmtl.Point2d) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Point2d(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static Point2dMarshaler mInstance = new Point2dMarshaler();
}


} // namespace gmtl
