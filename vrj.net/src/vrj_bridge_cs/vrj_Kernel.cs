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

public class Kernel
   : jccl.ConfigElementHandler
{
   private void allocDelegates()
   {
      m_configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement = new configCanHandleDelegate_boost_shared_ptr_jccl__ConfigElement(configCanHandle);
      m_configAddDelegate_boost_shared_ptr_jccl__ConfigElement = new configAddDelegate_boost_shared_ptr_jccl__ConfigElement(configAdd);
      m_configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement = new configRemoveDelegate_boost_shared_ptr_jccl__ConfigElement(configRemove);
   }

   // Constructors.
   protected Kernel(NoInitTag doInit)
      : base(doInit)
   {
      allocDelegates();
   }

   // Internal constructor needed for marshaling purposes.
   internal Kernel(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vrj_Kernel(IntPtr obj);

   // Destructor.
   ~Kernel()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vrj_Kernel(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.


   // Start of non-virtual methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static int vrj_Kernel_start__(IntPtr obj);

   public  int start()
   {
      int result;
      result = vrj_Kernel_start__(mRawObject);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_stop__(IntPtr obj);

   public  void stop()
   {
      vrj_Kernel_stop__(mRawObject);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Kernel_isRunning__(IntPtr obj);

   public  bool isRunning()
   {
      bool result;
      result = vrj_Kernel_isRunning__(mRawObject);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_waitForKernelStop__(IntPtr obj);

   public  void waitForKernelStop()
   {
      vrj_Kernel_waitForKernelStop__(mRawObject);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_initConfig__(IntPtr obj);

   public  void initConfig()
   {
      vrj_Kernel_initConfig__(mRawObject);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_setApplication__vrj_App(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.AppMarshaler))] vrj.App p0);

   public  void setApplication(vrj.App p0)
   {
      
      vrj_Kernel_setApplication__vrj_App(mRawObject, p0);
      
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_loadConfigFile__char(IntPtr obj,
	ref sbyte p0);

   public  void loadConfigFile(ref sbyte p0)
   {
      
      vrj_Kernel_loadConfigFile__char(mRawObject, ref p0);
      
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_loadConfigFile__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

   public  void loadConfigFile(string p0)
   {
      vrj_Kernel_loadConfigFile__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_scanForConfigDefinitions__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

   public  void scanForConfigDefinitions(string p0)
   {
      vrj_Kernel_scanForConfigDefinitions__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Kernel_addUser__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   protected  bool addUser(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_Kernel_addUser__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Kernel_removeUser__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   protected  bool removeUser(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_Kernel_removeUser__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_updateFrameData__(IntPtr obj);

   protected  void updateFrameData()
   {
      vrj_Kernel_updateFrameData__(mRawObject);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_checkForReconfig__(IntPtr obj);

   protected  void checkForReconfig()
   {
      vrj_Kernel_checkForReconfig__(mRawObject);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_changeApplication__vrj_App(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.AppMarshaler))] vrj.App p0);

   protected  void changeApplication(vrj.App p0)
   {
      
      vrj_Kernel_changeApplication__vrj_App(mRawObject, p0);
      
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_initSignalButtons__(IntPtr obj);

   protected  void initSignalButtons()
   {
      vrj_Kernel_initSignalButtons__(mRawObject);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_checkSignalButtons__(IntPtr obj);

   protected  void checkSignalButtons()
   {
      vrj_Kernel_checkSignalButtons__(mRawObject);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_startDrawManager__bool(IntPtr obj,
	bool p0);

   protected  void startDrawManager(bool p0)
   {
      vrj_Kernel_startDrawManager__bool(mRawObject, p0);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Kernel_stopDrawManager__(IntPtr obj);

   protected  void stopDrawManager()
   {
      vrj_Kernel_stopDrawManager__(mRawObject);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static gadget.InputManager vrj_Kernel_getInputManager__(IntPtr obj);

   public  gadget.InputManager getInputManager()
   {
      gadget.InputManager result;
      result = vrj_Kernel_getInputManager__(mRawObject);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static vrj.User vrj_Kernel_getUser__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

   public  vrj.User getUser(string p0)
   {
      vrj.User result;
      result = vrj_Kernel_getUser__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
      return result;
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Kernel_configCanHandle__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configCanHandle(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_Kernel_configCanHandle__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Kernel_configAdd__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configAdd(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_Kernel_configAdd__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Kernel_configRemove__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public override bool configRemove(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_Kernel_configRemove__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   // End of virtual methods.

   // Start of static methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static vrj.Kernel vrj_Kernel_instance__();

   public static vrj.Kernel instance()
   {
      vrj.Kernel result;
      result = vrj_Kernel_instance__();
      return result;
   }

   // End of static methods.


} // class vrj.Kernel

/// <summary>
/// Custom marshaler for vrj.Kernel.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class KernelMarshaler : ICustomMarshaler
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
      return ((vrj.Kernel) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vrj.Kernel(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static KernelMarshaler mInstance = new KernelMarshaler();
}


} // namespace vrj
