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

// Generated from Revision: 1.78 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gmtl
{

public sealed class Vec4i
   : gmtl.VecBase_int_4
{
   // Constructors.
   protected Vec4i(NoInitTag doInit)
      : base(doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Vec_int_4__Vec__0();

   public Vec4i()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_Vec_int_4__Vec__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Vec_int_4__Vec__gmtl_Vec4i1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec4iMarshaler))] gmtl.Vec4i p0);

   public Vec4i(gmtl.Vec4i p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_Vec_int_4__Vec__gmtl_Vec4i1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Vec_int_4__Vec__gmtl_VecBase_int_41([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.VecBase_int_4Marshaler))] gmtl.VecBase_int_4 p0);

   public Vec4i(gmtl.VecBase_int_4 p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_Vec_int_4__Vec__gmtl_VecBase_int_41(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Vec_int_4__Vec__int_int2(int p0, int p1);

   public Vec4i(int p0, int p1)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_Vec_int_4__Vec__int_int2(p0, p1);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Vec_int_4__Vec__int_int_int3(int p0, int p1, int p2);

   public Vec4i(int p0, int p1, int p2)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_Vec_int_4__Vec__int_int_int3(p0, p1, p2);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Vec_int_4__Vec__int_int_int_int4(int p0, int p1, int p2, int p3);

   public Vec4i(int p0, int p1, int p2, int p3)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gmtl_Vec_int_4__Vec__int_int_int_int4(p0, p1, p2, p3);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Vec4i(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Vec4i(IntPtr obj);

   // Destructor.
   ~Vec4i()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Vec4i(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Vec<int,4>.Params.
   public enum Params
   {
      Size = 4
   };


} // class gmtl.Vec4i

/// <summary>
/// Custom marshaler for gmtl.Vec4i.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class Vec4iMarshaler : ICustomMarshaler
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
      return ((gmtl.Vec4i) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Vec4i(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static Vec4iMarshaler mInstance = new Vec4iMarshaler();
}


} // namespace gmtl
