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

public abstract class GlApp
   : vrj.App
{
   private void allocDelegates()
   {
      m_drawDelegate = new drawDelegate(draw);
      m_contextInitDelegate = new contextInitDelegate(contextInit);
      m_contextCloseDelegate = new contextCloseDelegate(contextClose);
      m_contextPreDrawDelegate = new contextPreDrawDelegate(contextPreDraw);
      m_contextPostDrawDelegate = new contextPostDrawDelegate(contextPostDraw);
      m_bufferPreDrawDelegate = new bufferPreDrawDelegate(bufferPreDraw);
      m_pipePreDrawDelegate = new pipePreDrawDelegate(pipePreDraw);
      m_getDrawManagerDelegate = new getDrawManagerDelegate(getDrawManagerAdapter);
   }

   // Constructors.
   protected GlApp(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_GlApp_GlApp__vrj_Kernel1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.KernelMarshaler))] vrj.Kernel p0, [MarshalAs(UnmanagedType.FunctionPtr)] drawDelegate d0, [MarshalAs(UnmanagedType.FunctionPtr)] contextInitDelegate d1, [MarshalAs(UnmanagedType.FunctionPtr)] contextCloseDelegate d2, [MarshalAs(UnmanagedType.FunctionPtr)] contextPreDrawDelegate d3, [MarshalAs(UnmanagedType.FunctionPtr)] contextPostDrawDelegate d4, [MarshalAs(UnmanagedType.FunctionPtr)] bufferPreDrawDelegate d5, [MarshalAs(UnmanagedType.FunctionPtr)] pipePreDrawDelegate d6, [MarshalAs(UnmanagedType.FunctionPtr)] getDrawManagerDelegate d7, [MarshalAs(UnmanagedType.FunctionPtr)] initDelegate d8, [MarshalAs(UnmanagedType.FunctionPtr)] apiInitDelegate d9, [MarshalAs(UnmanagedType.FunctionPtr)] exitDelegate d10, [MarshalAs(UnmanagedType.FunctionPtr)] preFrameDelegate d11, [MarshalAs(UnmanagedType.FunctionPtr)] latePreFrameDelegate d12, [MarshalAs(UnmanagedType.FunctionPtr)] intraFrameDelegate d13, [MarshalAs(UnmanagedType.FunctionPtr)] postFrameDelegate d14, [MarshalAs(UnmanagedType.FunctionPtr)] resetDelegate d15, [MarshalAs(UnmanagedType.FunctionPtr)] focusChangedDelegate d16, [MarshalAs(UnmanagedType.FunctionPtr)] getDrawScaleFactorDelegate d17, [MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_jccl_ConfigElementPtr d18, [MarshalAs(UnmanagedType.FunctionPtr)] depSatisfiedDelegate d19, [MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_jccl_ConfigElementPtr d20, [MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_jccl_ConfigElementPtr d21, [MarshalAs(UnmanagedType.FunctionPtr)] getSoundManagerDelegate d22);

   public GlApp(vrj.Kernel p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = vrj_GlApp_GlApp__vrj_Kernel1(p0, m_drawDelegate, m_contextInitDelegate, m_contextCloseDelegate, m_contextPreDrawDelegate, m_contextPostDrawDelegate, m_bufferPreDrawDelegate, m_pipePreDrawDelegate, m_getDrawManagerDelegate, m_initDelegate, m_apiInitDelegate, m_exitDelegate, m_preFrameDelegate, m_latePreFrameDelegate, m_intraFrameDelegate, m_postFrameDelegate, m_resetDelegate, m_focusChangedDelegate, m_getDrawScaleFactorDelegate, m_configCanHandleDelegate_jccl_ConfigElementPtr, m_depSatisfiedDelegate, m_configAddDelegate_jccl_ConfigElementPtr, m_configRemoveDelegate_jccl_ConfigElementPtr, m_getSoundManagerDelegate);
      mWeOwnMemory = true;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_GlApp_GlApp__0([MarshalAs(UnmanagedType.FunctionPtr)] drawDelegate d0, [MarshalAs(UnmanagedType.FunctionPtr)] contextInitDelegate d1, [MarshalAs(UnmanagedType.FunctionPtr)] contextCloseDelegate d2, [MarshalAs(UnmanagedType.FunctionPtr)] contextPreDrawDelegate d3, [MarshalAs(UnmanagedType.FunctionPtr)] contextPostDrawDelegate d4, [MarshalAs(UnmanagedType.FunctionPtr)] bufferPreDrawDelegate d5, [MarshalAs(UnmanagedType.FunctionPtr)] pipePreDrawDelegate d6, [MarshalAs(UnmanagedType.FunctionPtr)] getDrawManagerDelegate d7, [MarshalAs(UnmanagedType.FunctionPtr)] initDelegate d8, [MarshalAs(UnmanagedType.FunctionPtr)] apiInitDelegate d9, [MarshalAs(UnmanagedType.FunctionPtr)] exitDelegate d10, [MarshalAs(UnmanagedType.FunctionPtr)] preFrameDelegate d11, [MarshalAs(UnmanagedType.FunctionPtr)] latePreFrameDelegate d12, [MarshalAs(UnmanagedType.FunctionPtr)] intraFrameDelegate d13, [MarshalAs(UnmanagedType.FunctionPtr)] postFrameDelegate d14, [MarshalAs(UnmanagedType.FunctionPtr)] resetDelegate d15, [MarshalAs(UnmanagedType.FunctionPtr)] focusChangedDelegate d16, [MarshalAs(UnmanagedType.FunctionPtr)] getDrawScaleFactorDelegate d17, [MarshalAs(UnmanagedType.FunctionPtr)] configCanHandleDelegate_jccl_ConfigElementPtr d18, [MarshalAs(UnmanagedType.FunctionPtr)] depSatisfiedDelegate d19, [MarshalAs(UnmanagedType.FunctionPtr)] configAddDelegate_jccl_ConfigElementPtr d20, [MarshalAs(UnmanagedType.FunctionPtr)] configRemoveDelegate_jccl_ConfigElementPtr d21, [MarshalAs(UnmanagedType.FunctionPtr)] getSoundManagerDelegate d22);

   public GlApp()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = vrj_GlApp_GlApp__0(m_drawDelegate, m_contextInitDelegate, m_contextCloseDelegate, m_contextPreDrawDelegate, m_contextPostDrawDelegate, m_bufferPreDrawDelegate, m_pipePreDrawDelegate, m_getDrawManagerDelegate, m_initDelegate, m_apiInitDelegate, m_exitDelegate, m_preFrameDelegate, m_latePreFrameDelegate, m_intraFrameDelegate, m_postFrameDelegate, m_resetDelegate, m_focusChangedDelegate, m_getDrawScaleFactorDelegate, m_configCanHandleDelegate_jccl_ConfigElementPtr, m_depSatisfiedDelegate, m_configAddDelegate_jccl_ConfigElementPtr, m_configRemoveDelegate_jccl_ConfigElementPtr, m_getSoundManagerDelegate);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal GlApp(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vrj_GlApp(IntPtr obj);

   // Destructor.
   ~GlApp()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vrj_GlApp(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of virtual methods.
   // Delegate for the draw() callback.
   public delegate void drawDelegate();
   protected drawDelegate m_drawDelegate;

   public abstract void draw();


   // Delegate for the contextInit() callback.
   public delegate void contextInitDelegate();
   protected contextInitDelegate m_contextInitDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_GlApp_contextInit__0(IntPtr obj);

   public virtual void contextInit()
   {
      vrj_GlApp_contextInit__0(mRawObject);
   }


   // Delegate for the contextClose() callback.
   public delegate void contextCloseDelegate();
   protected contextCloseDelegate m_contextCloseDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_GlApp_contextClose__0(IntPtr obj);

   public virtual void contextClose()
   {
      vrj_GlApp_contextClose__0(mRawObject);
   }


   // Delegate for the contextPreDraw() callback.
   public delegate void contextPreDrawDelegate();
   protected contextPreDrawDelegate m_contextPreDrawDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_GlApp_contextPreDraw__0(IntPtr obj);

   public virtual void contextPreDraw()
   {
      vrj_GlApp_contextPreDraw__0(mRawObject);
   }


   // Delegate for the contextPostDraw() callback.
   public delegate void contextPostDrawDelegate();
   protected contextPostDrawDelegate m_contextPostDrawDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_GlApp_contextPostDraw__0(IntPtr obj);

   public virtual void contextPostDraw()
   {
      vrj_GlApp_contextPostDraw__0(mRawObject);
   }


   // Delegate for the bufferPreDraw() callback.
   public delegate void bufferPreDrawDelegate();
   protected bufferPreDrawDelegate m_bufferPreDrawDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_GlApp_bufferPreDraw__0(IntPtr obj);

   public virtual void bufferPreDraw()
   {
      vrj_GlApp_bufferPreDraw__0(mRawObject);
   }


   // Delegate for the pipePreDraw() callback.
   public delegate void pipePreDrawDelegate();
   protected pipePreDrawDelegate m_pipePreDrawDelegate;

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_GlApp_pipePreDraw__0(IntPtr obj);

   public virtual void pipePreDraw()
   {
      vrj_GlApp_pipePreDraw__0(mRawObject);
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vrj.DrawManagerMarshaler))]
   private extern static vrj.DrawManager vrj_GlApp_getDrawManager__0(IntPtr obj);

   public override vrj.DrawManager getDrawManager()
   {
      vrj.DrawManager result;
      result = vrj_GlApp_getDrawManager__0(mRawObject);
      return result;
   }


   // End of virtual methods.


} // class vrj.GlApp

/// <summary>
/// Custom marshaler for vrj.GlApp.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class GlAppMarshaler : ICustomMarshaler
{
   private class DummyGlApp : vrj.GlApp
   {

      public DummyGlApp(IntPtr rawObject)
         : base(new NoInitTag())
      {
         mRawObject = rawObject;
         mWeOwnMemory = false;
      }

      [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
      private extern static void vrj_GlApp_draw__0(IntPtr obj);

      public override void draw()
      {
         vrj_GlApp_draw__0(mRawObject);
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
      return ((vrj.GlApp) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyGlApp(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static GlAppMarshaler mInstance = new GlAppMarshaler();
}


} // namespace vrj
