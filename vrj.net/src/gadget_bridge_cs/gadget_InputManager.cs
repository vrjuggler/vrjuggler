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

public sealed class InputManager
   : jccl.ConfigElementHandler
{
   private void allocDelegates()
   {
   }

   // Constructors.
   protected InputManager(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   // Internal constructor needed for marshaling purposes.
   internal InputManager(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_InputManager(IntPtr obj);

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))]
   private extern static jccl.ConfigElement gadget_InputManager_getDisplaySystemElement__(IntPtr obj);

   public  jccl.ConfigElement getDisplaySystemElement()
   {
      jccl.ConfigElement result;
      result = gadget_InputManager_getDisplaySystemElement__(mRawObject);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_InputManager_configureDevice__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public  bool configureDevice(jccl.ConfigElement p0)
   {
      bool result;
      result = gadget_InputManager_configureDevice__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_InputManager_updateAllData__(IntPtr obj);

   public  void updateAllData()
   {
      gadget_InputManager_updateAllData__(mRawObject);
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.InputMarshaler))]
   private extern static gadget.Input gadget_InputManager_getDevice__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

   public  gadget.Input getDevice(string p0)
   {
      gadget.Input result;
      result = gadget_InputManager_getDevice__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_InputManager_addDevice__gadget_Input(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.InputMarshaler))] gadget.Input p0);

   public  bool addDevice(gadget.Input p0)
   {
      
      bool result;
      result = gadget_InputManager_addDevice__gadget_Input(mRawObject, p0);
      
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_InputManager_addRemoteDevice__gadget_Input_std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.InputMarshaler))] gadget.Input p0,
	string p1);

   public  bool addRemoteDevice(gadget.Input p0, string p1)
   {
      
      bool result;
      result = gadget_InputManager_addRemoteDevice__gadget_Input_std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0, p1);
      
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_InputManager_removeDevice__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

   public  bool removeDevice(string p0)
   {
      bool result;
      result = gadget_InputManager_removeDevice__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_InputManager_removeDevice__gadget_Input(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.InputMarshaler))] gadget.Input p0);

   public  bool removeDevice(gadget.Input p0)
   {
      
      bool result;
      result = gadget_InputManager_removeDevice__gadget_Input(mRawObject, p0);
      
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_InputManager_addProxy__gadget_Proxy(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.ProxyMarshaler))] gadget.Proxy p0);

   public  bool addProxy(gadget.Proxy p0)
   {
      
      bool result;
      result = gadget_InputManager_addProxy__gadget_Proxy(mRawObject, p0);
      
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.ProxyMarshaler))]
   private extern static gadget.Proxy gadget_InputManager_getProxy__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

   public  gadget.Proxy getProxy(string p0)
   {
      gadget.Proxy result;
      result = gadget_InputManager_getProxy__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_InputManager_refreshAllProxies__(IntPtr obj);

   public  void refreshAllProxies()
   {
      gadget_InputManager_refreshAllProxies__(mRawObject);
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Start of virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_InputManager_configAdd__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configAdd(jccl.ConfigElement p0)
   {
      bool result;
      result = gadget_InputManager_configAdd__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_InputManager_configRemove__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configRemove(jccl.ConfigElement p0)
   {
      bool result;
      result = gadget_InputManager_configRemove__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_InputManager_configCanHandle__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configCanHandle(jccl.ConfigElement p0)
   {
      bool result;
      result = gadget_InputManager_configCanHandle__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   // End of virtual methods.

   // Start of static methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.InputManagerMarshaler))]
   private extern static gadget.InputManager gadget_InputManager_instance__();

   public static gadget.InputManager instance()
   {
      gadget.InputManager result;
      result = gadget_InputManager_instance__();
      return result;
   }

   // End of static methods.


} // class gadget.InputManager

/// <summary>
/// Custom marshaler for gadget.InputManager.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class InputManagerMarshaler : ICustomMarshaler
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
      return ((gadget.InputManager) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.InputManager(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static InputManagerMarshaler mInstance = new InputManagerMarshaler();
}


} // namespace gadget
