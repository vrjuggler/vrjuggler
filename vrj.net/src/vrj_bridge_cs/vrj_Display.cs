#region License
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
#endregion License

// Generated from Revision: 1.79 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace vrj
{

public sealed class Display
{
   private IntPtr mRawObject = IntPtr.Zero;
   private bool mWeOwnMemory = false;

   /// <summary>
   /// This is needed for the custom marshaler to be able to perform a
   /// reflective lookup.  The custom marshaler also uses this method to get
   /// access to the value of mRawObject when necessary.
   /// </summary>
   public IntPtr RawObject
   {
      get { return mRawObject; }
   }

   private void allocDelegates()
   {
   }

   // Constructors.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_Display_Display__vrj_Display1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vrj.DisplayMarshaler))] vrj.Display p0);

   public Display(vrj.Display p0)
   {
      allocDelegates();
      mRawObject   = vrj_Display_Display__vrj_Display1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_Display_Display__0();

   public Display()
   {
      allocDelegates();
      mRawObject   = vrj_Display_Display__0();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Display(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vrj_Display(IntPtr obj);

   // Destructor.
   ~Display()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vrj_Display(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Display_configDisplayWindow__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public  void configDisplayWindow(jccl.ConfigElement p0)
   {
      vrj_Display_configDisplayWindow__jccl_ConfigElementPtr1(mRawObject, p0);
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Display_configViewports__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public  void configViewports(jccl.ConfigElement p0)
   {
      vrj_Display_configViewports__jccl_ConfigElementPtr1(mRawObject, p0);
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Display_updateProjections__float1(IntPtr obj,
	float p0);

   public  void updateProjections(float p0)
   {
      vrj_Display_updateProjections__float1(mRawObject, p0);
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Display_isActive__0(IntPtr obj);

   public  bool isActive()
   {
      bool result;
      result = vrj_Display_isActive__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Display_setName__std_string1(IntPtr obj,
	string p0);

   public  void setName(string p0)
   {
      vrj_Display_setName__std_string1(mRawObject, p0);
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static string vrj_Display_getName__0(IntPtr obj);

   public  string getName()
   {
      string result;
      result = vrj_Display_getName__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Display_shouldDrawBorder__0(IntPtr obj);

   public  bool shouldDrawBorder()
   {
      bool result;
      result = vrj_Display_shouldDrawBorder__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Display_shouldHideMouse__0(IntPtr obj);

   public  bool shouldHideMouse()
   {
      bool result;
      result = vrj_Display_shouldHideMouse__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Display_setOriginAndSize__int_int_int_int_bool4(IntPtr obj,
	int p0,
	int p1,
	int p2,
	int p3);

   public  void setOriginAndSize(int p0, int p1, int p2, int p3)
   {
      vrj_Display_setOriginAndSize__int_int_int_int_bool4(mRawObject, p0, p1, p2, p3);
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Display_setOriginAndSize__int_int_int_int_bool5(IntPtr obj,
	int p0,
	int p1,
	int p2,
	int p3,
	bool p4);

   public  void setOriginAndSize(int p0, int p1, int p2, int p3, bool p4)
   {
      vrj_Display_setOriginAndSize__int_int_int_int_bool5(mRawObject, p0, p1, p2, p3, p4);
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Display_getOriginAndSize__int_int_int_int4(IntPtr obj,
	ref int p0,
	ref int p1,
	ref int p2,
	ref int p3);

   public  void getOriginAndSize(ref int p0, ref int p1, ref int p2, ref int p3)
   {
      vrj_Display_getOriginAndSize__int_int_int_int4(mRawObject, ref p0, ref p1, ref p2, ref p3);
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Display_setPipe__int1(IntPtr obj,
	int p0);

   public  void setPipe(int p0)
   {
      vrj_Display_setPipe__int1(mRawObject, p0);
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static int vrj_Display_getPipe__0(IntPtr obj);

   public  int getPipe()
   {
      int result;
      result = vrj_Display_getPipe__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_Display_isStereoRequested__0(IntPtr obj);

   public  bool isStereoRequested()
   {
      bool result;
      result = vrj_Display_isStereoRequested__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))]
   private extern static jccl.ConfigElement vrj_Display_getConfigElement__0(IntPtr obj);

   public  jccl.ConfigElement getConfigElement()
   {
      jccl.ConfigElement result;
      result = vrj_Display_getConfigElement__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))]
   private extern static jccl.ConfigElement vrj_Display_getGlFrameBufferConfig__0(IntPtr obj);

   public  jccl.ConfigElement getGlFrameBufferConfig()
   {
      jccl.ConfigElement result;
      result = vrj_Display_getGlFrameBufferConfig__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static uint vrj_Display_getNumViewports__0(IntPtr obj);

   public  uint getNumViewports()
   {
      uint result;
      result = vrj_Display_getNumViewports__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Start of virtual methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void vrj_Display_config__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public  void config(jccl.ConfigElement p0)
   {
      vrj_Display_config__jccl_ConfigElementPtr1(mRawObject, p0);
   }


   // End of virtual methods.


} // class vrj.Display

/// <summary>
/// Custom marshaler for vrj.Display.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class DisplayMarshaler : ICustomMarshaler
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
      return ((vrj.Display) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vrj.Display(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static DisplayMarshaler mInstance = new DisplayMarshaler();
}


} // namespace vrj
