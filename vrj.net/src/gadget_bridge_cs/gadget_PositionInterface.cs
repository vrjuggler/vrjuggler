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

// Generated from Revision: 1.65 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gadget
{

public class PositionInterface
   : gadget.BaseDeviceInterface
{
   private void allocDelegates()
   {
      m_refreshDelegate = new refreshDelegate(refresh);
   }

   // Constructors.
   protected PositionInterface(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_DeviceInterface_gadget_PositionProxy__DeviceInterface__gadget_PositionInterface([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.PositionInterfaceMarshaler))] gadget.PositionInterface p0,
	[MarshalAs(UnmanagedType.FunctionPtr)] refreshDelegate d0);

   public PositionInterface(gadget.PositionInterface p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      
      mRawObject   = gadget_DeviceInterface_gadget_PositionProxy__DeviceInterface__gadget_PositionInterface(p0, m_refreshDelegate);
      mWeOwnMemory = true;
      
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_DeviceInterface_gadget_PositionProxy__DeviceInterface__([MarshalAs(UnmanagedType.FunctionPtr)] refreshDelegate d0);

   public PositionInterface()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = gadget_DeviceInterface_gadget_PositionProxy__DeviceInterface__(m_refreshDelegate);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal PositionInterface(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_PositionInterface(IntPtr obj);

   // Destructor.
   ~PositionInterface()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_PositionInterface(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.PositionProxyMarshaler))]
   private extern static gadget.PositionProxy gadget_DeviceInterface_gadget_PositionProxy__getProxy__(IntPtr obj);

   public  gadget.PositionProxy getProxy()
   {
      gadget.PositionProxy result;
      result = gadget_DeviceInterface_gadget_PositionProxy__getProxy__(mRawObject);
      return result;
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_DeviceInterface_gadget_PositionProxy__refresh__(IntPtr obj);

   public override void refresh()
   {
      gadget_DeviceInterface_gadget_PositionProxy__refresh__(mRawObject);
   }

   // End of virtual methods.


} // class gadget.PositionInterface

/// <summary>
/// Custom marshaler for gadget.PositionInterface.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class PositionInterfaceMarshaler : ICustomMarshaler
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
      return ((gadget.PositionInterface) obj).mRawObject;
   }

   [DllImport("gadget_bridge")]
   private extern static IntPtr new_gadget_PositionInterface_Holder(IntPtr obj);

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.PositionInterface(new_gadget_PositionInterface_Holder(nativeObj), false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static PositionInterfaceMarshaler mInstance = new PositionInterfaceMarshaler();
}


} // namespace gadget
