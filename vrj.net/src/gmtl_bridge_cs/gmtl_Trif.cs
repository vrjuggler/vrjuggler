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

public sealed class Trif
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Trif(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Tri_float__Tri__();

   public Trif()
   {
      mRawObject   = gmtl_Tri_float__Tri__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Tri_float__Tri__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p2);

   public Trif(gmtl.Point3f p0, gmtl.Point3f p1, gmtl.Point3f p2)
   {
      
      
      
      mRawObject   = gmtl_Tri_float__Tri__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f(p0, p1, p2);
      mWeOwnMemory = true;
      
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Tri_float__Tri__gmtl_Trif([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.TrifMarshaler))] gmtl.Trif p0);

   public Trif(gmtl.Trif p0)
   {
      
      mRawObject   = gmtl_Tri_float__Tri__gmtl_Trif(p0);
      mWeOwnMemory = true;
      
   }

   // Internal constructor needed for marshaling purposes.
   internal Trif(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Trif(IntPtr obj);

   // Destructor.
   ~Trif()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Trif(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_Tri_float__edge__int(IntPtr obj,
	int p0);

   public  gmtl.Vec3f edge(int p0)
   {
      gmtl.Vec3f result;
      result = gmtl_Tri_float__edge__int(mRawObject, p0);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_Tri_float__edge__int_int(IntPtr obj,
	int p0,
	int p1);

   public  gmtl.Vec3f edge(int p0, int p1)
   {
      gmtl.Vec3f result;
      result = gmtl_Tri_float__edge__int_int(mRawObject, p0, p1);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Tri_float__set__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p2);

   public  void set(gmtl.Point3f p0, gmtl.Point3f p1, gmtl.Point3f p2)
   {
      
      
      
      gmtl_Tri_float__set__gmtl_Point3f_gmtl_Point3f_gmtl_Point3f(mRawObject, p0, p1, p2);
      
      
      
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class gmtl.Trif

/// <summary>
/// Custom marshaler for gmtl.Trif.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class TrifMarshaler : ICustomMarshaler
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
      return ((gmtl.Trif) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Trif(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static TrifMarshaler mInstance = new TrifMarshaler();
}


} // namespace gmtl
