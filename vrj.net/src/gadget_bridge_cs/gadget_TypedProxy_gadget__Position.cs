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

// Generated from Revision: 1.75 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gadget
{

public abstract class TypedProxy_gadget__Position
   : gadget.Proxy
{
   private void allocDelegates()
   {
      m_refreshDelegate = new refreshDelegate(refresh);
      m_getDeviceNameDelegate = new getDeviceNameDelegate(getDeviceName);
   }

   // Constructors.
   protected TypedProxy_gadget__Position(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_TypedProxy_gadget_Position__TypedProxy__0([MarshalAs(UnmanagedType.FunctionPtr)] refreshDelegate d0, [MarshalAs(UnmanagedType.FunctionPtr)] getDeviceNameDelegate d1, [MarshalAs(UnmanagedType.FunctionPtr)] configDelegate_boost_shared_ptr_jccl__ConfigElement d2, [MarshalAs(UnmanagedType.FunctionPtr)] updateDataDelegate d3, [MarshalAs(UnmanagedType.FunctionPtr)] getProxiedInputDeviceDelegate d4, [MarshalAs(UnmanagedType.FunctionPtr)] isStupifiedDelegate d5);

   public TypedProxy_gadget__Position()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = gadget_TypedProxy_gadget_Position__TypedProxy__0(m_refreshDelegate, m_getDeviceNameDelegate, m_configDelegate_boost_shared_ptr_jccl__ConfigElement, m_updateDataDelegate, m_getProxiedInputDeviceDelegate, m_isStupifiedDelegate);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal TypedProxy_gadget__Position(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_TypedProxy_gadget__Position(IntPtr obj);

   // Destructor.
   ~TypedProxy_gadget__Position()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_TypedProxy_gadget__Position(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_TypedProxy_gadget_Position__refresh__0(IntPtr obj);

   public override bool refresh()
   {
      bool result;
      result = gadget_TypedProxy_gadget_Position__refresh__0(mRawObject);
      return result;
   }


   // Delegate for the getDeviceName() callback.
   public delegate string getDeviceNameDelegate();
   protected getDeviceNameDelegate m_getDeviceNameDelegate;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static string gadget_TypedProxy_gadget_Position__getDeviceName__0(IntPtr obj);

   public virtual string getDeviceName()
   {
      string result;
      result = gadget_TypedProxy_gadget_Position__getDeviceName__0(mRawObject);
      return result;
   }


   // End of virtual methods.


} // class gadget.TypedProxy_gadget__Position

/// <summary>
/// Custom marshaler for gadget.TypedProxy_gadget__Position.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class TypedProxy_gadget__PositionMarshaler : ICustomMarshaler
{
   private class DummyTypedProxy_gadget__Position : gadget.TypedProxy_gadget__Position
   {

      public DummyTypedProxy_gadget__Position(IntPtr rawObject)
         : base(new NoInitTag())
      {
         mRawObject = rawObject;
         mWeOwnMemory = false;
      }

      [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(gadget.InputMarshaler))]
      private extern static gadget.Input gadget_Proxy_getProxiedInputDevice__0(IntPtr obj);

      public override gadget.Input getProxiedInputDevice()
      {
         gadget.Input result;
         result = gadget_Proxy_getProxiedInputDevice__0(mRawObject);
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
      return ((gadget.TypedProxy_gadget__Position) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyTypedProxy_gadget__Position(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static TypedProxy_gadget__PositionMarshaler mInstance = new TypedProxy_gadget__PositionMarshaler();
}


} // namespace gadget
