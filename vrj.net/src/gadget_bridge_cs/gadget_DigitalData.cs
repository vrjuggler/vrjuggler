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

// Generated from Revision: 1.73 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gadget
{

public class DigitalData
   : gadget.InputData
{
   // Constructors.
   protected DigitalData(NoInitTag doInit)
      : base(doInit)
   {
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_DigitalData_DigitalData__gadget_DigitalData([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.DigitalDataMarshaler))] gadget.DigitalData p0);

   public DigitalData(gadget.DigitalData p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      mRawObject   = gadget_DigitalData_DigitalData__gadget_DigitalData(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_DigitalData_DigitalData__();

   public DigitalData()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gadget_DigitalData_DigitalData__();
      mWeOwnMemory = true;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_DigitalData_DigitalData__int(int p0);

   public DigitalData(int p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gadget_DigitalData_DigitalData__int(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal DigitalData(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_DigitalData(IntPtr obj);

   // Destructor.
   ~DigitalData()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_DigitalData(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static int gadget_DigitalData_getDigital__0(IntPtr obj);

   public  int getDigital()
   {
      int result;
      result = gadget_DigitalData_getDigital__0(mRawObject);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_DigitalData_setDigital__int1(IntPtr obj,
	int p0);

   public  void setDigital(int p0)
   {
      gadget_DigitalData_setDigital__int1(mRawObject, p0);
   }


   // End of non-virtual methods.


} // class gadget.DigitalData

/// <summary>
/// Custom marshaler for gadget.DigitalData.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class DigitalDataMarshaler : ICustomMarshaler
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
      return ((gadget.DigitalData) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.DigitalData(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static DigitalDataMarshaler mInstance = new DigitalDataMarshaler();
}


} // namespace gadget
