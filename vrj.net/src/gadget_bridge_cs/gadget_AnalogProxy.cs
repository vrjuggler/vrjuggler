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





namespace gadget
{

public sealed class AnalogProxy
   : gadget.TypedProxy_gadget__Analog
{
   private void allocDelegates()
   {
   }

   // Constructors.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_AnalogProxy_AnalogProxy__gadget_AnalogProxy1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.AnalogProxyMarshaler))] gadget.AnalogProxy p0);

   public AnalogProxy(gadget.AnalogProxy p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = gadget_AnalogProxy_AnalogProxy__gadget_AnalogProxy1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_AnalogProxy_AnalogProxy__0();

   public AnalogProxy()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = gadget_AnalogProxy_AnalogProxy__0();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal AnalogProxy(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_AnalogProxy(IntPtr obj);

   // Destructor.
   ~AnalogProxy()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_AnalogProxy(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static float gadget_AnalogProxy_getData__0(IntPtr obj);

   public  float getData()
   {
      float result;
      result = gadget_AnalogProxy_getData__0(mRawObject);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.AnalogMarshaler))]
   private extern static gadget.Analog gadget_AnalogProxy_getAnalogPtr__0(IntPtr obj);

   public  gadget.Analog getAnalogPtr()
   {
      gadget.Analog result;
      result = gadget_AnalogProxy_getAnalogPtr__0(mRawObject);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static int gadget_AnalogProxy_getUnit__0(IntPtr obj);

   public  int getUnit()
   {
      int result;
      result = gadget_AnalogProxy_getUnit__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Start of virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_AnalogProxy_updateData__0(IntPtr obj);

   public override void updateData()
   {
      gadget_AnalogProxy_updateData__0(mRawObject);
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.IntervalMarshaler))]
   private extern static vpr.Interval gadget_AnalogProxy_getTimeStamp__0(IntPtr obj);

   public override vpr.Interval getTimeStamp()
   {
      vpr.Interval result;
      result = gadget_AnalogProxy_getTimeStamp__0(mRawObject);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_AnalogProxy_config__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool config(jccl.ConfigElement p0)
   {
      bool result;
      result = gadget_AnalogProxy_config__jccl_ConfigElementPtr1(mRawObject, p0);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.InputMarshaler))]
   private extern static gadget.Input gadget_AnalogProxy_getProxiedInputDevice__0(IntPtr obj);

   public override gadget.Input getProxiedInputDevice()
   {
      gadget.Input result;
      result = gadget_AnalogProxy_getProxiedInputDevice__0(mRawObject);
      return result;
   }


   // End of virtual methods.

   // Start of static methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static string gadget_AnalogProxy_getElementType__0();

   public new static string getElementType()
   {
      string result;
      result = gadget_AnalogProxy_getElementType__0();
      return result;
   }


   // End of static methods.


} // class gadget.AnalogProxy

/// <summary>
/// Custom marshaler for gadget.AnalogProxy.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class AnalogProxyMarshaler : ICustomMarshaler
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
      return ((gadget.AnalogProxy) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.AnalogProxy(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static AnalogProxyMarshaler mInstance = new AnalogProxyMarshaler();
}


} // namespace gadget
