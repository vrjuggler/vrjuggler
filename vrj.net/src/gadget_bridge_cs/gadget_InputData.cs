#region License
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
#endregion License

// Generated from Revision: 1.78 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gadget
{

public class InputData
{
   protected IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

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
   protected InputData(NoInitTag doInit)
   {
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_InputData_InputData__gadget_InputData1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.InputDataMarshaler))] gadget.InputData p0);

   public InputData(gadget.InputData p0)
   {
      mRawObject   = gadget_InputData_InputData__gadget_InputData1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_InputData_InputData__0();

   public InputData()
   {
      mRawObject   = gadget_InputData_InputData__0();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal InputData(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_InputData(IntPtr obj);

   // Destructor.
   ~InputData()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_InputData(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_InputData_setTime__0(IntPtr obj);

   public  void setTime()
   {
      gadget_InputData_setTime__0(mRawObject);
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_InputData_setTime__vpr_Interval1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.IntervalMarshaler))] vpr.Interval p0);

   public  void setTime(vpr.Interval p0)
   {
      gadget_InputData_setTime__vpr_Interval1(mRawObject, p0);
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.IntervalMarshaler))]
   private extern static vpr.Interval gadget_InputData_getTime__0(IntPtr obj);

   public  vpr.Interval getTime()
   {
      vpr.Interval result;
      result = gadget_InputData_getTime__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.


} // class gadget.InputData

/// <summary>
/// Custom marshaler for gadget.InputData.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class InputDataMarshaler : ICustomMarshaler
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
      return ((gadget.InputData) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.InputData(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static InputDataMarshaler mInstance = new InputDataMarshaler();
}


} // namespace gadget
