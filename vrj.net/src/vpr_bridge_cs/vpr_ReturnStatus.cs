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

// Generated from Revision: 1.79 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace vpr
{

public sealed class ReturnStatus
{
   private IntPtr mRawObject = IntPtr.Zero;
   private bool mWeOwnMemory = false;

   /// <summary>
   /// This is needed for the custom marshaler to be able to perform a
   /// reflective lookup.  The custom marshaler also uses this method to get
   /// access to the value of mRawObject when necessary.
   /// </summary>
   public IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_ReturnStatus_ReturnStatus__0();

   public ReturnStatus()
   {
      mRawObject   = vpr_ReturnStatus_ReturnStatus__0();
      mWeOwnMemory = true;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_ReturnStatus_ReturnStatus__vpr_ReturnStatus1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))] vpr.ReturnStatus p0);

   public ReturnStatus(vpr.ReturnStatus p0)
   {
      mRawObject   = vpr_ReturnStatus_ReturnStatus__vpr_ReturnStatus1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_ReturnStatus_ReturnStatus__vpr_ReturnStatus_Code1(vpr.ReturnStatus.Code p0);

   public ReturnStatus(vpr.ReturnStatus.Code p0)
   {
      mRawObject   = vpr_ReturnStatus_ReturnStatus__vpr_ReturnStatus_Code1(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal ReturnStatus(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vpr_ReturnStatus(IntPtr obj);

   // Destructor.
   ~ReturnStatus()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vpr_ReturnStatus(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   public override bool Equals(object obj)
   {
      if ( ! (obj is vpr.ReturnStatus) )
      {
         return false;
      }
      return this == (vpr.ReturnStatus) obj;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ReturnStatus_equal__vpr_ReturnStatus(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))] vpr.ReturnStatus rhs);

   public static bool operator==(vpr.ReturnStatus lhs, vpr.ReturnStatus rhs)
   {
      bool result;
      result = vpr_ReturnStatus_equal__vpr_ReturnStatus(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ReturnStatus_equal__vpr_ReturnStatus_Code(IntPtr lhs,
	vpr.ReturnStatus.Code rhs);

   public static bool operator==(vpr.ReturnStatus lhs, vpr.ReturnStatus.Code rhs)
   {
      bool result;
      result = vpr_ReturnStatus_equal__vpr_ReturnStatus_Code(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ReturnStatus_not_equal__vpr_ReturnStatus(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))] vpr.ReturnStatus rhs);

   public static bool operator!=(vpr.ReturnStatus lhs, vpr.ReturnStatus rhs)
   {
      bool result;
      result = vpr_ReturnStatus_not_equal__vpr_ReturnStatus(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ReturnStatus_not_equal__vpr_ReturnStatus_Code(IntPtr lhs,
	vpr.ReturnStatus.Code rhs);

   public static bool operator!=(vpr.ReturnStatus lhs, vpr.ReturnStatus.Code rhs)
   {
      bool result;
      result = vpr_ReturnStatus_not_equal__vpr_ReturnStatus_Code(lhs.mRawObject, rhs);
      return result;
   }

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ReturnStatus_copy__vpr_ReturnStatus1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))] vpr.ReturnStatus p0);

   public  void copy(vpr.ReturnStatus p0)
   {
      vpr_ReturnStatus_copy__vpr_ReturnStatus1(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ReturnStatus_setCode__vpr_ReturnStatus_Code1(IntPtr obj,
	vpr.ReturnStatus.Code p0);

   public  void setCode(vpr.ReturnStatus.Code p0)
   {
      vpr_ReturnStatus_setCode__vpr_ReturnStatus_Code1(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static vpr.ReturnStatus.Code vpr_ReturnStatus_code__0(IntPtr obj);

   public  vpr.ReturnStatus.Code code()
   {
      vpr.ReturnStatus.Code result;
      result = vpr_ReturnStatus_code__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ReturnStatus_success__0(IntPtr obj);

   public  bool success()
   {
      bool result;
      result = vpr_ReturnStatus_success__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ReturnStatus_failure__0(IntPtr obj);

   public  bool failure()
   {
      bool result;
      result = vpr_ReturnStatus_failure__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ReturnStatus_wouldBlock__0(IntPtr obj);

   public  bool wouldBlock()
   {
      bool result;
      result = vpr_ReturnStatus_wouldBlock__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ReturnStatus_inProgress__0(IntPtr obj);

   public  bool inProgress()
   {
      bool result;
      result = vpr_ReturnStatus_inProgress__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ReturnStatus_timeout__0(IntPtr obj);

   public  bool timeout()
   {
      bool result;
      result = vpr_ReturnStatus_timeout__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration vpr.ReturnStatus.Code.
   public enum Code
   {
      WouldBlock = 2, NotConnected = 5, Succeed = 0, Timeout = 3, Fail = 1, InProgress = 4
   };


} // class vpr.ReturnStatus

/// <summary>
/// Custom marshaler for vpr.ReturnStatus.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ReturnStatusMarshaler : ICustomMarshaler
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
      return ((vpr.ReturnStatus) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vpr.ReturnStatus(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static ReturnStatusMarshaler mInstance = new ReturnStatusMarshaler();
}


} // namespace vpr
