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

public sealed class PositionProxy
   : gadget.TypedProxy_gadget__Position
{
   private void allocDelegates()
   {
   }

   // Constructors.
   protected PositionProxy(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_PositionProxy_PositionProxy__gadget_PositionProxy([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.PositionProxyMarshaler))] gadget.PositionProxy p0);

   public PositionProxy(gadget.PositionProxy p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      
      mRawObject   = gadget_PositionProxy_PositionProxy__gadget_PositionProxy(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_PositionProxy_PositionProxy__();

   public PositionProxy()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = gadget_PositionProxy_PositionProxy__();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal PositionProxy(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_PositionProxy(IntPtr obj);

   // Destructor.
   ~PositionProxy()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_PositionProxy(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.IntervalMarshaler))]
   private extern static vpr.Interval gadget_PositionProxy_getTimeStamp__0(IntPtr obj);

   public  vpr.Interval getTimeStamp()
   {
      vpr.Interval result;
      result = gadget_PositionProxy_getTimeStamp__0(mRawObject);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gadget_PositionProxy_getData__float0(IntPtr obj);

   public  gmtl.Matrix44f getData()
   {
      gmtl.Matrix44f result;
      result = gadget_PositionProxy_getData__float0(mRawObject);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gadget_PositionProxy_getData__float1(IntPtr obj,
	float p0);

   public  gmtl.Matrix44f getData(float p0)
   {
      gmtl.Matrix44f result;
      result = gadget_PositionProxy_getData__float1(mRawObject, p0);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static int gadget_PositionProxy_getUnit__0(IntPtr obj);

   public  int getUnit()
   {
      int result;
      result = gadget_PositionProxy_getUnit__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Start of virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_PositionProxy_updateData__0(IntPtr obj);

   public override void updateData()
   {
      gadget_PositionProxy_updateData__0(mRawObject);
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_PositionProxy_config__boost_shared_ptr_jccl__ConfigElement1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool config(jccl.ConfigElement p0)
   {
      bool result;
      result = gadget_PositionProxy_config__boost_shared_ptr_jccl__ConfigElement1(mRawObject, p0);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.InputMarshaler))]
   private extern static gadget.Input gadget_PositionProxy_getProxiedInputDevice__0(IntPtr obj);

   public override gadget.Input getProxiedInputDevice()
   {
      gadget.Input result;
      result = gadget_PositionProxy_getProxiedInputDevice__0(mRawObject);
      return result;
   }


   // End of virtual methods.

   // Start of static methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static string gadget_PositionProxy_getElementType__0();

   public new static string getElementType()
   {
      string result;
      result = gadget_PositionProxy_getElementType__0();
      return result;
   }


   // End of static methods.


} // class gadget.PositionProxy

/// <summary>
/// Custom marshaler for gadget.PositionProxy.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class PositionProxyMarshaler : ICustomMarshaler
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
      return ((gadget.PositionProxy) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.PositionProxy(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static PositionProxyMarshaler mInstance = new PositionProxyMarshaler();
}


} // namespace gadget
