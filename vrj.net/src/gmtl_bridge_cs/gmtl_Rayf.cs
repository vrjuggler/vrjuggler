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

public class Rayf
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Rayf(NoInitTag doInit)
   {
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Ray_float__Ray__();

   public Rayf()
   {
      mRawObject   = gmtl_Ray_float__Ray__();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Ray_float__Ray__gmtl_Point3f_gmtl_Vec3f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p1);

   public Rayf(gmtl.Point3f p0, gmtl.Vec3f p1)
   {
      
      
      mRawObject   = gmtl_Ray_float__Ray__gmtl_Point3f_gmtl_Vec3f(p0, p1);
      mWeOwnMemory = true;
      
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Ray_float__Ray__gmtl_Rayf([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.RayfMarshaler))] gmtl.Rayf p0);

   public Rayf(gmtl.Rayf p0)
   {
      
      mRawObject   = gmtl_Ray_float__Ray__gmtl_Rayf(p0);
      mWeOwnMemory = true;
      
   }

   // Internal constructor needed for marshaling purposes.
   internal Rayf(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Rayf(IntPtr obj);

   // Destructor.
   ~Rayf()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Rayf(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Point3fMarshaler))]
   private extern static gmtl.Point3f gmtl_Ray_float__getOrigin__(IntPtr obj);

   public  gmtl.Point3f getOrigin()
   {
      gmtl.Point3f result;
      result = gmtl_Ray_float__getOrigin__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Ray_float__setOrigin__gmtl_Point3f(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Point3fMarshaler))] gmtl.Point3f p0);

   public  void setOrigin(gmtl.Point3f p0)
   {
      
      gmtl_Ray_float__setOrigin__gmtl_Point3f(mRawObject, p0);
      
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))]
   private extern static gmtl.Vec3f gmtl_Ray_float__getDir__(IntPtr obj);

   public  gmtl.Vec3f getDir()
   {
      gmtl.Vec3f result;
      result = gmtl_Ray_float__getDir__(mRawObject);
      return result;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Ray_float__setDir__gmtl_Vec3f(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Vec3fMarshaler))] gmtl.Vec3f p0);

   public  void setDir(gmtl.Vec3f p0)
   {
      
      gmtl_Ray_float__setDir__gmtl_Vec3f(mRawObject, p0);
      
   }

   // End of non-virtual methods.


} // class gmtl.Rayf

/// <summary>
/// Custom marshaler for gmtl.Rayf.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class RayfMarshaler : ICustomMarshaler
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
      return ((gmtl.Rayf) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Rayf(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static RayfMarshaler mInstance = new RayfMarshaler();
}


} // namespace gmtl
