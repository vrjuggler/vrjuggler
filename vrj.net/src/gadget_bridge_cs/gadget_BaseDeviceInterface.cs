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

// Generated from Revision: 1.69 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gadget
{

public class BaseDeviceInterface
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   private void allocDelegates()
   {
      m_refreshDelegate = new refreshDelegate(refresh);
   }

   // Constructors.
   protected BaseDeviceInterface(NoInitTag doInit)
   {
      allocDelegates();
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_BaseDeviceInterface_BaseDeviceInterface__gadget_BaseDeviceInterface([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.BaseDeviceInterfaceMarshaler))] gadget.BaseDeviceInterface p0,
	[MarshalAs(UnmanagedType.FunctionPtr)] refreshDelegate d0);

   public BaseDeviceInterface(gadget.BaseDeviceInterface p0)
   {
      allocDelegates();
      
      mRawObject   = gadget_BaseDeviceInterface_BaseDeviceInterface__gadget_BaseDeviceInterface(p0, m_refreshDelegate);
      mWeOwnMemory = true;
      
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_BaseDeviceInterface_BaseDeviceInterface__([MarshalAs(UnmanagedType.FunctionPtr)] refreshDelegate d0);

   public BaseDeviceInterface()
   {
      allocDelegates();
      mRawObject   = gadget_BaseDeviceInterface_BaseDeviceInterface__(m_refreshDelegate);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal BaseDeviceInterface(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_BaseDeviceInterface(IntPtr obj);

   // Destructor.
   ~BaseDeviceInterface()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_BaseDeviceInterface(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_BaseDeviceInterface_init__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

   public  void init(string p0)
   {
      gadget_BaseDeviceInterface_init__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static string gadget_BaseDeviceInterface_getProxyName__(IntPtr obj);

   public  string getProxyName()
   {
      string result;
      result = gadget_BaseDeviceInterface_getProxyName__(mRawObject);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_BaseDeviceInterface_isConnected__(IntPtr obj);

   public  bool isConnected()
   {
      bool result;
      result = gadget_BaseDeviceInterface_isConnected__(mRawObject);
      return result;
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // Delegate for the refresh() callback.
   public delegate void refreshDelegate();
   protected refreshDelegate m_refreshDelegate;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_BaseDeviceInterface_refresh__(IntPtr obj);

   public virtual void refresh()
   {
      gadget_BaseDeviceInterface_refresh__(mRawObject);
   }

   // End of virtual methods.

   // Start of static methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_BaseDeviceInterface_refreshAllDevices__();

   public static void refreshAllDevices()
   {
      gadget_BaseDeviceInterface_refreshAllDevices__();
   }

   // End of static methods.

   // Start of static data.
   // End of static data.


} // class gadget.BaseDeviceInterface

/// <summary>
/// Custom marshaler for gadget.BaseDeviceInterface.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class BaseDeviceInterfaceMarshaler : ICustomMarshaler
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
      return ((gadget.BaseDeviceInterface) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.BaseDeviceInterface(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static BaseDeviceInterfaceMarshaler mInstance = new BaseDeviceInterfaceMarshaler();
}


} // namespace gadget
