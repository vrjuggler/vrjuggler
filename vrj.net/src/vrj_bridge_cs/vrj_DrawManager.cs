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

// Generated from Revision: 1.76 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace vrj
{

public abstract class DrawManager
   : jccl.ConfigElementHandler
{
   private void allocDelegates()
   {
      m_drawDelegate = new drawDelegate(draw);
      m_syncDelegate = new syncDelegate(sync);
      m_setAppDelegate_vrj_App = new setAppDelegate_vrj_App(setApp);
      m_initAPIDelegate = new initAPIDelegate(initAPI);
      m_addDisplayDelegate_vrj_Display = new addDisplayDelegate_vrj_Display(addDisplay);
      m_removeDisplayDelegate_vrj_Display = new removeDisplayDelegate_vrj_Display(removeDisplay);
      m_closeAPIDelegate = new closeAPIDelegate(closeAPI);
   }

   // Constructors.
   protected DrawManager(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_DrawManager_DrawManager__0([MarshalAs(UnmanagedType.FunctionPtr)] drawDelegate d0, [MarshalAs(UnmanagedType.FunctionPtr)] syncDelegate d1, [MarshalAs(UnmanagedType.FunctionPtr)] setAppDelegate_vrj_App d2, [MarshalAs(UnmanagedType.FunctionPtr)] initAPIDelegate d3, [MarshalAs(UnmanagedType.FunctionPtr)] addDisplayDelegate_vrj_Display d4, [MarshalAs(UnmanagedType.FunctionPtr)] removeDisplayDelegate_vrj_Display d5, [MarshalAs(UnmanagedType.FunctionPtr)] closeAPIDelegate d6, [MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_jccl_ConfigElementPtr d7, [MarshalAs(UnmanagedType.FunctionPtr)] configProcessPendingDelegate d8, [MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_jccl_ConfigElementPtr d9, [MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_jccl_ConfigElementPtr d10);

   public DrawManager()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = vrj_DrawManager_DrawManager__0(m_drawDelegate, m_syncDelegate, m_setAppDelegate_vrj_App, m_initAPIDelegate, m_addDisplayDelegate_vrj_Display, m_removeDisplayDelegate_vrj_Display, m_closeAPIDelegate, m_configCanHandleDelegate_jccl_ConfigElementPtr, m_configProcessPendingDelegate, m_configAddDelegate_jccl_ConfigElementPtr, m_configRemoveDelegate_jccl_ConfigElementPtr);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal DrawManager(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vrj_DrawManager(IntPtr obj);

   // Destructor.
   ~DrawManager()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vrj_DrawManager(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   // End of non-virtual methods.

   // Start of virtual methods.
   // Delegate for the draw() callback.
   public delegate void drawDelegate();
   protected drawDelegate m_drawDelegate;

   public abstract void draw();


   // Delegate for the sync() callback.
   public delegate void syncDelegate();
   protected syncDelegate m_syncDelegate;

   public abstract void sync();


   // Delegate for the setApp() callback.
   public delegate void setAppDelegate_vrj_App([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.AppMarshaler))] vrj.App p0);
   protected setAppDelegate_vrj_App m_setAppDelegate_vrj_App;

   public abstract void setApp(vrj.App p0);


   // Delegate for the initAPI() callback.
   public delegate void initAPIDelegate();
   protected initAPIDelegate m_initAPIDelegate;

   public abstract void initAPI();


   // Delegate for the addDisplay() callback.
   public delegate void addDisplayDelegate_vrj_Display([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.DisplayMarshaler))] vrj.Display p0);
   protected addDisplayDelegate_vrj_Display m_addDisplayDelegate_vrj_Display;

   public abstract void addDisplay(vrj.Display p0);


   // Delegate for the removeDisplay() callback.
   public delegate void removeDisplayDelegate_vrj_Display([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.DisplayMarshaler))] vrj.Display p0);
   protected removeDisplayDelegate_vrj_Display m_removeDisplayDelegate_vrj_Display;

   public abstract void removeDisplay(vrj.Display p0);


   // Delegate for the closeAPI() callback.
   public delegate void closeAPIDelegate();
   protected closeAPIDelegate m_closeAPIDelegate;

   public abstract void closeAPI();


   // End of virtual methods.


} // class vrj.DrawManager

/// <summary>
/// Custom marshaler for vrj.DrawManager.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class DrawManagerMarshaler : ICustomMarshaler
{
   private class DummyDrawManager : vrj.DrawManager
   {

      public DummyDrawManager(IntPtr rawObject)
         : base(new NoInitTag())
      {
         mRawObject = rawObject;
         mWeOwnMemory = false;
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static void vrj_DrawManager_draw__0(IntPtr obj);

      public override void draw()
      {
         vrj_DrawManager_draw__0(mRawObject);
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static void vrj_DrawManager_sync__0(IntPtr obj);

      public override void sync()
      {
         vrj_DrawManager_sync__0(mRawObject);
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static void vrj_DrawManager_setApp__vrj_App1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.AppMarshaler))] vrj.App p0);

      public override void setApp(vrj.App p0)
      {
         vrj_DrawManager_setApp__vrj_App1(mRawObject, p0);
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static void vrj_DrawManager_initAPI__0(IntPtr obj);

      public override void initAPI()
      {
         vrj_DrawManager_initAPI__0(mRawObject);
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static void vrj_DrawManager_addDisplay__vrj_Display1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.DisplayMarshaler))] vrj.Display p0);

      public override void addDisplay(vrj.Display p0)
      {
         vrj_DrawManager_addDisplay__vrj_Display1(mRawObject, p0);
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static void vrj_DrawManager_removeDisplay__vrj_Display1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.DisplayMarshaler))] vrj.Display p0);

      public override void removeDisplay(vrj.Display p0)
      {
         vrj_DrawManager_removeDisplay__vrj_Display1(mRawObject, p0);
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static void vrj_DrawManager_closeAPI__0(IntPtr obj);

      public override void closeAPI()
      {
         vrj_DrawManager_closeAPI__0(mRawObject);
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static bool jccl_ConfigElementHandler_configCanHandle__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

      public override bool configCanHandle(jccl.ConfigElement p0)
      {
         bool result;
         result = jccl_ConfigElementHandler_configCanHandle__jccl_ConfigElementPtr1(mRawObject, p0);
         return result;
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static bool jccl_ConfigElementHandler_configAdd__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

      public override bool configAdd(jccl.ConfigElement p0)
      {
         bool result;
         result = jccl_ConfigElementHandler_configAdd__jccl_ConfigElementPtr1(mRawObject, p0);
         return result;
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static bool jccl_ConfigElementHandler_configRemove__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

      public override bool configRemove(jccl.ConfigElement p0)
      {
         bool result;
         result = jccl_ConfigElementHandler_configRemove__jccl_ConfigElementPtr1(mRawObject, p0);
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
      return ((vrj.DrawManager) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyDrawManager(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static DrawManagerMarshaler mInstance = new DrawManagerMarshaler();
}


} // namespace vrj
