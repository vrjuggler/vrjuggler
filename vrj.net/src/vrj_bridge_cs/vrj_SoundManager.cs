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

// Generated from Revision: 1.78 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace vrj
{

public class SoundManager
   : jccl.ConfigElementHandler
{
   private void allocDelegates()
   {
      m_configAddDelegate_jccl_ConfigElementPtr = new configAddDelegate_jccl_ConfigElementPtr(configAdd);
      m_configRemoveDelegate_jccl_ConfigElementPtr = new configRemoveDelegate_jccl_ConfigElementPtr(configRemove);
      m_configCanHandleDelegate_jccl_ConfigElementPtr = new configCanHandleDelegate_jccl_ConfigElementPtr(configCanHandle);
      m_updateDelegate = new updateDelegate(update);
      m_syncDelegate = new syncDelegate(sync);
   }

   // Constructors.
   protected SoundManager(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_SoundManager_SoundManager__vrj_SoundManager1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.SoundManagerMarshaler))] vrj.SoundManager p0, [MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_jccl_ConfigElementPtr d0, [MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_jccl_ConfigElementPtr d1, [MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_jccl_ConfigElementPtr d2, [MarshalAs(UnmanagedType.FunctionPtr)] updateDelegate d3, [MarshalAs(UnmanagedType.FunctionPtr)] syncDelegate d4, [MarshalAs(UnmanagedType.FunctionPtr)] configProcessPendingDelegate d5);

   public SoundManager(vrj.SoundManager p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = vrj_SoundManager_SoundManager__vrj_SoundManager1(p0, m_configAddDelegate_jccl_ConfigElementPtr, m_configRemoveDelegate_jccl_ConfigElementPtr, m_configCanHandleDelegate_jccl_ConfigElementPtr, m_updateDelegate, m_syncDelegate, m_configProcessPendingDelegate);
      mWeOwnMemory = true;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_SoundManager_SoundManager__0([MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_jccl_ConfigElementPtr d0, [MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_jccl_ConfigElementPtr d1, [MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_jccl_ConfigElementPtr d2, [MarshalAs(UnmanagedType.FunctionPtr)] updateDelegate d3, [MarshalAs(UnmanagedType.FunctionPtr)] syncDelegate d4, [MarshalAs(UnmanagedType.FunctionPtr)] configProcessPendingDelegate d5);

   public SoundManager()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = vrj_SoundManager_SoundManager__0(m_configAddDelegate_jccl_ConfigElementPtr, m_configRemoveDelegate_jccl_ConfigElementPtr, m_configCanHandleDelegate_jccl_ConfigElementPtr, m_updateDelegate, m_syncDelegate, m_configProcessPendingDelegate);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal SoundManager(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vrj_SoundManager(IntPtr obj);

   // Destructor.
   ~SoundManager()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vrj_SoundManager(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of virtual methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_SoundManager_configAdd__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configAdd(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_SoundManager_configAdd__jccl_ConfigElementPtr1(mRawObject, p0);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_SoundManager_configRemove__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configRemove(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_SoundManager_configRemove__jccl_ConfigElementPtr1(mRawObject, p0);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_SoundManager_configCanHandle__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configCanHandle(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_SoundManager_configCanHandle__jccl_ConfigElementPtr1(mRawObject, p0);
      return result;
   }


   // Delegate for the update() callback.
   public delegate void updateDelegate();
   protected updateDelegate m_updateDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_SoundManager_update__0(IntPtr obj);

   public virtual void update()
   {
      vrj_SoundManager_update__0(mRawObject);
   }


   // Delegate for the sync() callback.
   public delegate void syncDelegate();
   protected syncDelegate m_syncDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_SoundManager_sync__0(IntPtr obj);

   public virtual void sync()
   {
      vrj_SoundManager_sync__0(mRawObject);
   }


   // End of virtual methods.


} // class vrj.SoundManager

/// <summary>
/// Custom marshaler for vrj.SoundManager.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class SoundManagerMarshaler : ICustomMarshaler
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
      return ((vrj.SoundManager) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vrj.SoundManager(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static SoundManagerMarshaler mInstance = new SoundManagerMarshaler();
}


} // namespace vrj
