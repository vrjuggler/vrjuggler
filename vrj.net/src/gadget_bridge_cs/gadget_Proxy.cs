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

// Generated from $Revision$ of $RCSfile$
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gadget
{

public abstract class Proxy
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
      m_configDelegate_boost_shared_ptr_jccl__ConfigElement = new configDelegate_boost_shared_ptr_jccl__ConfigElement(config);
      m_refreshDelegate = new refreshDelegate(refresh);
      m_updateDataDelegate = new updateDataDelegate(updateData);
      m_getProxiedInputDeviceDelegate = new getProxiedInputDeviceDelegate(getProxiedInputDevice);
      m_isStupifiedDelegate = new isStupifiedDelegate(isStupified);
   }

   // Constructors.
   protected Proxy(NoInitTag doInit)
   {
      allocDelegates();
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_Proxy_Proxy__([MarshalAs(UnmanagedType.FunctionPtr)] configDelegate_boost_shared_ptr_jccl__ConfigElement d0,
	[MarshalAs(UnmanagedType.FunctionPtr)] refreshDelegate d1,
	[MarshalAs(UnmanagedType.FunctionPtr)] updateDataDelegate d2,
	[MarshalAs(UnmanagedType.FunctionPtr)] getProxiedInputDeviceDelegate d3,
	[MarshalAs(UnmanagedType.FunctionPtr)] isStupifiedDelegate d4);

   public Proxy()
   {
      allocDelegates();
      mRawObject   = gadget_Proxy_Proxy__(m_configDelegate_boost_shared_ptr_jccl__ConfigElement, m_refreshDelegate, m_updateDataDelegate, m_getProxiedInputDeviceDelegate, m_isStupifiedDelegate);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Proxy(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_Proxy(IntPtr obj);

   // Destructor.
   ~Proxy()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_Proxy(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.


   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static string gadget_Proxy_getName__(IntPtr obj);

   public  string getName()
   {
      string result;
      result = gadget_Proxy_getName__(mRawObject);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_Proxy_setName__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

   public  void setName(string p0)
   {
      gadget_Proxy_setName__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_Proxy_stupify__bool(IntPtr obj,
	bool p0);

   public  void stupify(bool p0)
   {
      gadget_Proxy_stupify__bool(mRawObject, p0);
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // Delegate for the config() callback.
   public delegate bool configDelegate_boost_shared_ptr_jccl__ConfigElement(jccl.ConfigElement p0);
   protected configDelegate_boost_shared_ptr_jccl__ConfigElement m_configDelegate_boost_shared_ptr_jccl__ConfigElement;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_Proxy_config__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public virtual bool config(jccl.ConfigElement p0)
   {
      bool result;
      result = gadget_Proxy_config__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   // Delegate for the refresh() callback.
   public delegate bool refreshDelegate();
   protected refreshDelegate m_refreshDelegate;

   public abstract bool refresh();

   // Delegate for the updateData() callback.
   public delegate void updateDataDelegate();
   protected updateDataDelegate m_updateDataDelegate;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_Proxy_updateData__(IntPtr obj);

   public virtual void updateData()
   {
      gadget_Proxy_updateData__(mRawObject);
   }

   // Delegate for the getProxiedInputDevice() callback.
   public delegate gadget.Input getProxiedInputDeviceDelegate();
   protected getProxiedInputDeviceDelegate m_getProxiedInputDeviceDelegate;

   public abstract gadget.Input getProxiedInputDevice();

   // Delegate for the isStupified() callback.
   public delegate bool isStupifiedDelegate();
   protected isStupifiedDelegate m_isStupifiedDelegate;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_Proxy_isStupified__(IntPtr obj);

   public virtual bool isStupified()
   {
      bool result;
      result = gadget_Proxy_isStupified__(mRawObject);
      return result;
   }

   // End of virtual methods.

   // Start of static methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static string gadget_Proxy_getElementType__();

   public static string getElementType()
   {
      string result;
      result = gadget_Proxy_getElementType__();
      return result;
   }

   // End of static methods.


} // class gadget.Proxy

/// <summary>
/// Custom marshaler for gadget.Proxy.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ProxyMarshaler : ICustomMarshaler
{
   private class DummyProxy : gadget.Proxy
   {

      public DummyProxy(IntPtr rawObject)
         : base(new NoInitTag())
      {
         mRawObject = rawObject;
         mWeOwnMemory = false;
      }

      [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
      private extern static bool gadget_Proxy_refresh__(IntPtr obj);

      public override bool refresh()
      {
         bool result;
         result = gadget_Proxy_refresh__(mRawObject);
         return result;
      }

      [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
      private extern static gadget.Input gadget_Proxy_getProxiedInputDevice__(IntPtr obj);

      public override gadget.Input getProxiedInputDevice()
      {
         gadget.Input result;
         result = gadget_Proxy_getProxiedInputDevice__(mRawObject);
         return result;
      }

   }

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
      return ((gadget.Proxy) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyProxy(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static ProxyMarshaler mInstance = new ProxyMarshaler();
}


} // namespace gadget
