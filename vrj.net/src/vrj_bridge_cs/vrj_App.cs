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





namespace vrj
{

public abstract class App
   : jccl.ConfigElementHandler
{
   private void allocDelegates()
   {
      m_initDelegate = new initDelegate(init);
      m_apiInitDelegate = new apiInitDelegate(apiInit);
      m_exitDelegate = new exitDelegate(exit);
      m_preFrameDelegate = new preFrameDelegate(preFrame);
      m_latePreFrameDelegate = new latePreFrameDelegate(latePreFrame);
      m_intraFrameDelegate = new intraFrameDelegate(intraFrame);
      m_postFrameDelegate = new postFrameDelegate(postFrame);
      m_resetDelegate = new resetDelegate(reset);
      m_focusChangedDelegate = new focusChangedDelegate(focusChanged);
      m_getDrawScaleFactorDelegate = new getDrawScaleFactorDelegate(getDrawScaleFactor);
      m_configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement = new configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement(configCanHandle);
      m_depSatisfiedDelegate = new depSatisfiedDelegate(depSatisfied);
      m_configAddDelegate_boost_shared_ptr_jccl__ConfigElement = new configAddDelegate_boost_shared_ptr_jccl__ConfigElement(configAdd);
      m_configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement = new configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement(configRemove);
      m_getDrawManagerDelegate = new getDrawManagerDelegate(getDrawManager);
      m_getSoundManagerDelegate = new getSoundManagerDelegate(getSoundManager);
   }

   // Constructors.
   protected App(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_App_App__vrj_Kernel([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.KernelMarshaler))] vrj.Kernel p0,
	[MarshalAs(UnmanagedType.FunctionPtr)] initDelegate d0,
	[MarshalAs(UnmanagedType.FunctionPtr)] apiInitDelegate d1,
	[MarshalAs(UnmanagedType.FunctionPtr)] exitDelegate d2,
	[MarshalAs(UnmanagedType.FunctionPtr)] preFrameDelegate d3,
	[MarshalAs(UnmanagedType.FunctionPtr)] latePreFrameDelegate d4,
	[MarshalAs(UnmanagedType.FunctionPtr)] intraFrameDelegate d5,
	[MarshalAs(UnmanagedType.FunctionPtr)] postFrameDelegate d6,
	[MarshalAs(UnmanagedType.FunctionPtr)] resetDelegate d7,
	[MarshalAs(UnmanagedType.FunctionPtr)] focusChangedDelegate d8,
	[MarshalAs(UnmanagedType.FunctionPtr)] getDrawScaleFactorDelegate d9,
	[MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement d10,
	[MarshalAs(UnmanagedType.FunctionPtr)] depSatisfiedDelegate d11,
	[MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_boost_shared_ptr_jccl__ConfigElement d12,
	[MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement d13,
	[MarshalAs(UnmanagedType.FunctionPtr)] getDrawManagerDelegate d14,
	[MarshalAs(UnmanagedType.FunctionPtr)] getSoundManagerDelegate d15,
	[MarshalAs(UnmanagedType.FunctionPtr)] configProcessPendingDelegate d16);

   public App(vrj.Kernel p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      
      mRawObject   = vrj_App_App__vrj_Kernel(p0, m_initDelegate, m_apiInitDelegate, m_exitDelegate, m_preFrameDelegate, m_latePreFrameDelegate, m_intraFrameDelegate, m_postFrameDelegate, m_resetDelegate, m_focusChangedDelegate, m_getDrawScaleFactorDelegate, m_configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement, m_depSatisfiedDelegate, m_configAddDelegate_boost_shared_ptr_jccl__ConfigElement, m_configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement, m_getDrawManagerDelegate, m_getSoundManagerDelegate, m_configProcessPendingDelegate);
      mWeOwnMemory = true;
      
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_App_App__([MarshalAs(UnmanagedType.FunctionPtr)] initDelegate d0,
	[MarshalAs(UnmanagedType.FunctionPtr)] apiInitDelegate d1,
	[MarshalAs(UnmanagedType.FunctionPtr)] exitDelegate d2,
	[MarshalAs(UnmanagedType.FunctionPtr)] preFrameDelegate d3,
	[MarshalAs(UnmanagedType.FunctionPtr)] latePreFrameDelegate d4,
	[MarshalAs(UnmanagedType.FunctionPtr)] intraFrameDelegate d5,
	[MarshalAs(UnmanagedType.FunctionPtr)] postFrameDelegate d6,
	[MarshalAs(UnmanagedType.FunctionPtr)] resetDelegate d7,
	[MarshalAs(UnmanagedType.FunctionPtr)] focusChangedDelegate d8,
	[MarshalAs(UnmanagedType.FunctionPtr)] getDrawScaleFactorDelegate d9,
	[MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement d10,
	[MarshalAs(UnmanagedType.FunctionPtr)] depSatisfiedDelegate d11,
	[MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_boost_shared_ptr_jccl__ConfigElement d12,
	[MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement d13,
	[MarshalAs(UnmanagedType.FunctionPtr)] getDrawManagerDelegate d14,
	[MarshalAs(UnmanagedType.FunctionPtr)] getSoundManagerDelegate d15,
	[MarshalAs(UnmanagedType.FunctionPtr)] configProcessPendingDelegate d16);

   public App()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = vrj_App_App__(m_initDelegate, m_apiInitDelegate, m_exitDelegate, m_preFrameDelegate, m_latePreFrameDelegate, m_intraFrameDelegate, m_postFrameDelegate, m_resetDelegate, m_focusChangedDelegate, m_getDrawScaleFactorDelegate, m_configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement, m_depSatisfiedDelegate, m_configAddDelegate_boost_shared_ptr_jccl__ConfigElement, m_configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement, m_getDrawManagerDelegate, m_getSoundManagerDelegate, m_configProcessPendingDelegate);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal App(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vrj_App(IntPtr obj);

   // Destructor.
   ~App()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vrj_App(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.


   // Start of non-virtual methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_haveFocus__(IntPtr obj);

   public  bool haveFocus()
   {
      bool result;
      result = vrj_App_haveFocus__(mRawObject);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_setFocus__bool(IntPtr obj,
	bool p0);

   public  void setFocus(bool p0)
   {
      vrj_App_setFocus__bool(mRawObject, p0);
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // Delegate for the init() callback.
   public delegate void initDelegate();
   protected initDelegate m_initDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_init__(IntPtr obj);

   public virtual void init()
   {
      vrj_App_init__(mRawObject);
   }

   // Delegate for the apiInit() callback.
   public delegate void apiInitDelegate();
   protected apiInitDelegate m_apiInitDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_apiInit__(IntPtr obj);

   public virtual void apiInit()
   {
      vrj_App_apiInit__(mRawObject);
   }

   // Delegate for the exit() callback.
   public delegate void exitDelegate();
   protected exitDelegate m_exitDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_exit__(IntPtr obj);

   public virtual void exit()
   {
      vrj_App_exit__(mRawObject);
   }

   // Delegate for the preFrame() callback.
   public delegate void preFrameDelegate();
   protected preFrameDelegate m_preFrameDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_preFrame__(IntPtr obj);

   public virtual void preFrame()
   {
      vrj_App_preFrame__(mRawObject);
   }

   // Delegate for the latePreFrame() callback.
   public delegate void latePreFrameDelegate();
   protected latePreFrameDelegate m_latePreFrameDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_latePreFrame__(IntPtr obj);

   public virtual void latePreFrame()
   {
      vrj_App_latePreFrame__(mRawObject);
   }

   // Delegate for the intraFrame() callback.
   public delegate void intraFrameDelegate();
   protected intraFrameDelegate m_intraFrameDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_intraFrame__(IntPtr obj);

   public virtual void intraFrame()
   {
      vrj_App_intraFrame__(mRawObject);
   }

   // Delegate for the postFrame() callback.
   public delegate void postFrameDelegate();
   protected postFrameDelegate m_postFrameDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_postFrame__(IntPtr obj);

   public virtual void postFrame()
   {
      vrj_App_postFrame__(mRawObject);
   }

   // Delegate for the reset() callback.
   public delegate void resetDelegate();
   protected resetDelegate m_resetDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_reset__(IntPtr obj);

   public virtual void reset()
   {
      vrj_App_reset__(mRawObject);
   }

   // Delegate for the focusChanged() callback.
   public delegate void focusChangedDelegate();
   protected focusChangedDelegate m_focusChangedDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_focusChanged__(IntPtr obj);

   public virtual void focusChanged()
   {
      vrj_App_focusChanged__(mRawObject);
   }

   // Delegate for the getDrawScaleFactor() callback.
   public delegate float getDrawScaleFactorDelegate();
   protected getDrawScaleFactorDelegate m_getDrawScaleFactorDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static float vrj_App_getDrawScaleFactor__(IntPtr obj);

   public virtual float getDrawScaleFactor()
   {
      float result;
      result = vrj_App_getDrawScaleFactor__(mRawObject);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_configCanHandle__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configCanHandle(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_App_configCanHandle__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   // Delegate for the depSatisfied() callback.
   public delegate bool depSatisfiedDelegate();
   protected depSatisfiedDelegate m_depSatisfiedDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_depSatisfied__(IntPtr obj);

   public virtual bool depSatisfied()
   {
      bool result;
      result = vrj_App_depSatisfied__(mRawObject);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_configAdd__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configAdd(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_App_configAdd__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_configRemove__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configRemove(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_App_configRemove__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   // Delegate for the getDrawManager() callback.
   public delegate vrj.DrawManager getDrawManagerDelegate();
   protected getDrawManagerDelegate m_getDrawManagerDelegate;

   public abstract vrj.DrawManager getDrawManager();

   // Delegate for the getSoundManager() callback.
   public delegate vrj.SoundManager getSoundManagerDelegate();
   protected getSoundManagerDelegate m_getSoundManagerDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static vrj.SoundManager vrj_App_getSoundManager__(IntPtr obj);

   public virtual vrj.SoundManager getSoundManager()
   {
      vrj.SoundManager result;
      result = vrj_App_getSoundManager__(mRawObject);
      return result;
   }

   // End of virtual methods.


} // class vrj.App

/// <summary>
/// Custom marshaler for vrj.App.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class AppMarshaler : ICustomMarshaler
{
   private class DummyApp : vrj.App
   {

      public DummyApp(IntPtr rawObject)
         : base(new NoInitTag())
      {
         mRawObject = rawObject;
         mWeOwnMemory = false;
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static vrj.DrawManager vrj_App_getDrawManager__(IntPtr obj);

      public override vrj.DrawManager getDrawManager()
      {
         vrj.DrawManager result;
         result = vrj_App_getDrawManager__(mRawObject);
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
      return ((vrj.App) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyApp(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static AppMarshaler mInstance = new AppMarshaler();
}


} // namespace vrj
