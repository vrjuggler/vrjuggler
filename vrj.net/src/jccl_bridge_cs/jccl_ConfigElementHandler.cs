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





namespace jccl
{

public abstract class ConfigElementHandler
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
      m_configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement = new configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement(configCanHandle);
      m_configProcessPendingDelegate = new configProcessPendingDelegate(configProcessPending);
      m_configAddDelegate_boost_shared_ptr_jccl__ConfigElement = new configAddDelegate_boost_shared_ptr_jccl__ConfigElement(configAdd);
      m_configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement = new configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement(configRemove);
   }

   // Constructors.
   protected ConfigElementHandler(NoInitTag doInit)
   {
      allocDelegates();
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr jccl_ConfigElementHandler_ConfigElementHandler__0([MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement d0, [MarshalAs(UnmanagedType.FunctionPtr)] configProcessPendingDelegate d1, [MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_boost_shared_ptr_jccl__ConfigElement d2, [MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement d3);

   public ConfigElementHandler()
   {
      allocDelegates();
      mRawObject   = jccl_ConfigElementHandler_ConfigElementHandler__0(m_configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement, m_configProcessPendingDelegate, m_configAddDelegate_boost_shared_ptr_jccl__ConfigElement, m_configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal ConfigElementHandler(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_jccl_ConfigElementHandler(IntPtr obj);

   // Destructor.
   ~ConfigElementHandler()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_jccl_ConfigElementHandler(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of virtual methods.
   // Delegate for the configCanHandle() callback.
   public delegate bool configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);
   protected configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement m_configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement;

   public abstract bool configCanHandle(jccl.ConfigElement p0);


   // Delegate for the configProcessPending() callback.
   public delegate int configProcessPendingDelegate();
   protected configProcessPendingDelegate m_configProcessPendingDelegate;

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static int jccl_ConfigElementHandler_configProcessPending__0(IntPtr obj);

   public virtual int configProcessPending()
   {
      int result;
      result = jccl_ConfigElementHandler_configProcessPending__0(mRawObject);
      return result;
   }


   // Delegate for the configAdd() callback.
   public delegate bool configAddDelegate_boost_shared_ptr_jccl__ConfigElement([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);
   protected configAddDelegate_boost_shared_ptr_jccl__ConfigElement m_configAddDelegate_boost_shared_ptr_jccl__ConfigElement;

   public abstract bool configAdd(jccl.ConfigElement p0);


   // Delegate for the configRemove() callback.
   public delegate bool configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);
   protected configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement m_configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement;

   public abstract bool configRemove(jccl.ConfigElement p0);


   // End of virtual methods.


} // class jccl.ConfigElementHandler

/// <summary>
/// Custom marshaler for jccl.ConfigElementHandler.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ConfigElementHandlerMarshaler : ICustomMarshaler
{
   private class DummyConfigElementHandler : jccl.ConfigElementHandler
   {

      public DummyConfigElementHandler(IntPtr rawObject)
         : base(new NoInitTag())
      {
         mRawObject = rawObject;
         mWeOwnMemory = false;
      }

      [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
      private extern static bool jccl_ConfigElementHandler_configCanHandle__boost_shared_ptr_jccl__ConfigElement1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

      public override bool configCanHandle(jccl.ConfigElement p0)
      {
         bool result;
         result = jccl_ConfigElementHandler_configCanHandle__boost_shared_ptr_jccl__ConfigElement1(mRawObject, p0);
         return result;
      }

      [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
      private extern static bool jccl_ConfigElementHandler_configAdd__boost_shared_ptr_jccl__ConfigElement1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

      public override bool configAdd(jccl.ConfigElement p0)
      {
         bool result;
         result = jccl_ConfigElementHandler_configAdd__boost_shared_ptr_jccl__ConfigElement1(mRawObject, p0);
         return result;
      }

      [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
      private extern static bool jccl_ConfigElementHandler_configRemove__boost_shared_ptr_jccl__ConfigElement1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

      public override bool configRemove(jccl.ConfigElement p0)
      {
         bool result;
         result = jccl_ConfigElementHandler_configRemove__boost_shared_ptr_jccl__ConfigElement1(mRawObject, p0);
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
      return ((jccl.ConfigElementHandler) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyConfigElementHandler(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static ConfigElementHandlerMarshaler mInstance = new ConfigElementHandlerMarshaler();
}


} // namespace jccl
