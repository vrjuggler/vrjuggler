#region License
// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$
#endregion License

// Generated from Revision: 1.78 of RCSfile: class_cs.tmpl,v
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
      m_configCanHandleDelegate_jccl_ConfigElementPtr = new configCanHandleDelegate_jccl_ConfigElementPtr(configCanHandle);
      m_depSatisfiedDelegate = new depSatisfiedDelegate(depSatisfied);
      m_configAddDelegate_jccl_ConfigElementPtr = new configAddDelegate_jccl_ConfigElementPtr(configAdd);
      m_configRemoveDelegate_jccl_ConfigElementPtr = new configRemoveDelegate_jccl_ConfigElementPtr(configRemove);
      m_getDrawManagerDelegate = new getDrawManagerDelegate(getDrawManagerAdapter);
      m_getSoundManagerDelegate = new getSoundManagerDelegate(getSoundManagerAdapter);
   }

   // Constructors.
   protected App(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_App_App__vrj_Kernel1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.KernelMarshaler))] vrj.Kernel p0, [MarshalAs(UnmanagedType.FunctionPtr)] initDelegate d0, [MarshalAs(UnmanagedType.FunctionPtr)] apiInitDelegate d1, [MarshalAs(UnmanagedType.FunctionPtr)] exitDelegate d2, [MarshalAs(UnmanagedType.FunctionPtr)] preFrameDelegate d3, [MarshalAs(UnmanagedType.FunctionPtr)] latePreFrameDelegate d4, [MarshalAs(UnmanagedType.FunctionPtr)] intraFrameDelegate d5, [MarshalAs(UnmanagedType.FunctionPtr)] postFrameDelegate d6, [MarshalAs(UnmanagedType.FunctionPtr)] resetDelegate d7, [MarshalAs(UnmanagedType.FunctionPtr)] focusChangedDelegate d8, [MarshalAs(UnmanagedType.FunctionPtr)] getDrawScaleFactorDelegate d9, [MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_jccl_ConfigElementPtr d10, [MarshalAs(UnmanagedType.FunctionPtr)] depSatisfiedDelegate d11, [MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_jccl_ConfigElementPtr d12, [MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_jccl_ConfigElementPtr d13, [MarshalAs(UnmanagedType.FunctionPtr)] getDrawManagerDelegate d14, [MarshalAs(UnmanagedType.FunctionPtr)] getSoundManagerDelegate d15, [MarshalAs(UnmanagedType.FunctionPtr)] configProcessPendingDelegate d16);

   public App(vrj.Kernel p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = vrj_App_App__vrj_Kernel1(p0, m_initDelegate, m_apiInitDelegate, m_exitDelegate, m_preFrameDelegate, m_latePreFrameDelegate, m_intraFrameDelegate, m_postFrameDelegate, m_resetDelegate, m_focusChangedDelegate, m_getDrawScaleFactorDelegate, m_configCanHandleDelegate_jccl_ConfigElementPtr, m_depSatisfiedDelegate, m_configAddDelegate_jccl_ConfigElementPtr, m_configRemoveDelegate_jccl_ConfigElementPtr, m_getDrawManagerDelegate, m_getSoundManagerDelegate, m_configProcessPendingDelegate);
      mWeOwnMemory = true;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_App_App__0([MarshalAs(UnmanagedType.FunctionPtr)] initDelegate d0, [MarshalAs(UnmanagedType.FunctionPtr)] apiInitDelegate d1, [MarshalAs(UnmanagedType.FunctionPtr)] exitDelegate d2, [MarshalAs(UnmanagedType.FunctionPtr)] preFrameDelegate d3, [MarshalAs(UnmanagedType.FunctionPtr)] latePreFrameDelegate d4, [MarshalAs(UnmanagedType.FunctionPtr)] intraFrameDelegate d5, [MarshalAs(UnmanagedType.FunctionPtr)] postFrameDelegate d6, [MarshalAs(UnmanagedType.FunctionPtr)] resetDelegate d7, [MarshalAs(UnmanagedType.FunctionPtr)] focusChangedDelegate d8, [MarshalAs(UnmanagedType.FunctionPtr)] getDrawScaleFactorDelegate d9, [MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_jccl_ConfigElementPtr d10, [MarshalAs(UnmanagedType.FunctionPtr)] depSatisfiedDelegate d11, [MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_jccl_ConfigElementPtr d12, [MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_jccl_ConfigElementPtr d13, [MarshalAs(UnmanagedType.FunctionPtr)] getDrawManagerDelegate d14, [MarshalAs(UnmanagedType.FunctionPtr)] getSoundManagerDelegate d15, [MarshalAs(UnmanagedType.FunctionPtr)] configProcessPendingDelegate d16);

   public App()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = vrj_App_App__0(m_initDelegate, m_apiInitDelegate, m_exitDelegate, m_preFrameDelegate, m_latePreFrameDelegate, m_intraFrameDelegate, m_postFrameDelegate, m_resetDelegate, m_focusChangedDelegate, m_getDrawScaleFactorDelegate, m_configCanHandleDelegate_jccl_ConfigElementPtr, m_depSatisfiedDelegate, m_configAddDelegate_jccl_ConfigElementPtr, m_configRemoveDelegate_jccl_ConfigElementPtr, m_getDrawManagerDelegate, m_getSoundManagerDelegate, m_configProcessPendingDelegate);
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

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_haveFocus__0(IntPtr obj);

   public  bool haveFocus()
   {
      bool result;
      result = vrj_App_haveFocus__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_setFocus__bool1(IntPtr obj,
	bool p0);

   public  void setFocus(bool p0)
   {
      vrj_App_setFocus__bool1(mRawObject, p0);
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // Delegate for the init() callback.
   public delegate void initDelegate();
   protected initDelegate m_initDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_init__0(IntPtr obj);

   public virtual void init()
   {
      vrj_App_init__0(mRawObject);
   }


   // Delegate for the apiInit() callback.
   public delegate void apiInitDelegate();
   protected apiInitDelegate m_apiInitDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_apiInit__0(IntPtr obj);

   public virtual void apiInit()
   {
      vrj_App_apiInit__0(mRawObject);
   }


   // Delegate for the exit() callback.
   public delegate void exitDelegate();
   protected exitDelegate m_exitDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_exit__0(IntPtr obj);

   public virtual void exit()
   {
      vrj_App_exit__0(mRawObject);
   }


   // Delegate for the preFrame() callback.
   public delegate void preFrameDelegate();
   protected preFrameDelegate m_preFrameDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_preFrame__0(IntPtr obj);

   public virtual void preFrame()
   {
      vrj_App_preFrame__0(mRawObject);
   }


   // Delegate for the latePreFrame() callback.
   public delegate void latePreFrameDelegate();
   protected latePreFrameDelegate m_latePreFrameDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_latePreFrame__0(IntPtr obj);

   public virtual void latePreFrame()
   {
      vrj_App_latePreFrame__0(mRawObject);
   }


   // Delegate for the intraFrame() callback.
   public delegate void intraFrameDelegate();
   protected intraFrameDelegate m_intraFrameDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_intraFrame__0(IntPtr obj);

   public virtual void intraFrame()
   {
      vrj_App_intraFrame__0(mRawObject);
   }


   // Delegate for the postFrame() callback.
   public delegate void postFrameDelegate();
   protected postFrameDelegate m_postFrameDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_postFrame__0(IntPtr obj);

   public virtual void postFrame()
   {
      vrj_App_postFrame__0(mRawObject);
   }


   // Delegate for the reset() callback.
   public delegate void resetDelegate();
   protected resetDelegate m_resetDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_reset__0(IntPtr obj);

   public virtual void reset()
   {
      vrj_App_reset__0(mRawObject);
   }


   // Delegate for the focusChanged() callback.
   public delegate void focusChangedDelegate();
   protected focusChangedDelegate m_focusChangedDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_App_focusChanged__0(IntPtr obj);

   public virtual void focusChanged()
   {
      vrj_App_focusChanged__0(mRawObject);
   }


   // Delegate for the getDrawScaleFactor() callback.
   public delegate float getDrawScaleFactorDelegate();
   protected getDrawScaleFactorDelegate m_getDrawScaleFactorDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static float vrj_App_getDrawScaleFactor__0(IntPtr obj);

   public virtual float getDrawScaleFactor()
   {
      float result;
      result = vrj_App_getDrawScaleFactor__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_configCanHandle__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configCanHandle(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_App_configCanHandle__jccl_ConfigElementPtr1(mRawObject, p0);
      return result;
   }


   // Delegate for the depSatisfied() callback.
   public delegate bool depSatisfiedDelegate();
   protected depSatisfiedDelegate m_depSatisfiedDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_depSatisfied__0(IntPtr obj);

   public virtual bool depSatisfied()
   {
      bool result;
      result = vrj_App_depSatisfied__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_configAdd__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configAdd(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_App_configAdd__jccl_ConfigElementPtr1(mRawObject, p0);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_App_configRemove__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configRemove(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_App_configRemove__jccl_ConfigElementPtr1(mRawObject, p0);
      return result;
   }


   // Delegate for the getDrawManager() callback.
   protected IntPtr getDrawManagerAdapter()
   {
      return getDrawManager().RawObject;
   }

   public delegate IntPtr getDrawManagerDelegate();
   protected getDrawManagerDelegate m_getDrawManagerDelegate;

   public abstract vrj.DrawManager getDrawManager();


   // Delegate for the getSoundManager() callback.
   protected IntPtr getSoundManagerAdapter()
   {
      return getSoundManager().RawObject;
   }

   public delegate IntPtr getSoundManagerDelegate();
   protected getSoundManagerDelegate m_getSoundManagerDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vrj.SoundManagerMarshaler))]
   private extern static vrj.SoundManager vrj_App_getSoundManager__0(IntPtr obj);

   public virtual vrj.SoundManager getSoundManager()
   {
      vrj.SoundManager result;
      result = vrj_App_getSoundManager__0(mRawObject);
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
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vrj.DrawManagerMarshaler))]
      private extern static vrj.DrawManager vrj_App_getDrawManager__0(IntPtr obj);

      public override vrj.DrawManager getDrawManager()
      {
         vrj.DrawManager result;
         result = vrj_App_getDrawManager__0(mRawObject);
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
      return ((vrj.App) obj).RawObject;
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
