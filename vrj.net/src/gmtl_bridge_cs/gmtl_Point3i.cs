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

public sealed class Point3i
   : gmtl.VecBase_int_3
{
   // Constructors.
   protected Point3i(NoInitTag doInit)
      : base(doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_int_3__Point__();

   public Point3i()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_Point_int_3__Point__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_int_3__Point__gmtl_Point3i([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3iMarshaler))] gmtl.Point3i p0);

   public Point3i(gmtl.Point3i p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      mRawObject   = gmtl_Point_int_3__Point__gmtl_Point3i(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_int_3__Point__gmtl_VecBase_int_3([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_int_3Marshaler))] gmtl.VecBase_int_3 p0);

   public Point3i(gmtl.VecBase_int_3 p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      mRawObject   = gmtl_Point_int_3__Point__gmtl_VecBase_int_3(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_int_3__Point__int_int(ref int p0,
	ref int p1);

   public Point3i(ref int p0, ref int p1)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      mRawObject   = gmtl_Point_int_3__Point__int_int(ref p0, ref p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_int_3__Point__int_int_int(ref int p0,
	ref int p1,
	ref int p2);

   public Point3i(ref int p0, ref int p1, ref int p2)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      
      mRawObject   = gmtl_Point_int_3__Point__int_int_int(ref p0, ref p1, ref p2);
      mWeOwnMemory = true;
      
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Point_int_3__Point__int_int_int_int(ref int p0,
	ref int p1,
	ref int p2,
	ref int p3);

   public Point3i(ref int p0, ref int p1, ref int p2, ref int p3)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      
      
      
      mRawObject   = gmtl_Point_int_3__Point__int_int_int_int(ref p0, ref p1, ref p2, ref p3);
      mWeOwnMemory = true;
      
      
      
      
   }

   // Internal constructor needed for marshaling purposes.
   internal Point3i(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Point3i(IntPtr obj);

   // Destructor.
   ~Point3i()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Point3i(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Point<int,3>.Params.
   public enum Params
   {
      Size = 3
   };


} // class gmtl.Point3i

/// <summary>
/// Custom marshaler for gmtl.Point3i.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class Point3iMarshaler : ICustomMarshaler
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
      return ((gmtl.Point3i) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Point3i(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static Point3iMarshaler mInstance = new Point3iMarshaler();
}


} // namespace gmtl
