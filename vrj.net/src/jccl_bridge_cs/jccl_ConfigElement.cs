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





namespace jccl
{

public sealed class ConfigElement
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

   // Constructors.
   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr jccl_ConfigElement_ConfigElement__0();

   public ConfigElement()
   {
      mRawObject   = jccl_ConfigElement_ConfigElement__0();
      mWeOwnMemory = true;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr jccl_ConfigElement_ConfigElement__jccl_ConfigDefinitionPtr1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigDefinitionMarshaler))] jccl.ConfigDefinition p0);

   public ConfigElement(jccl.ConfigDefinition p0)
   {
      mRawObject   = jccl_ConfigElement_ConfigElement__jccl_ConfigDefinitionPtr1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr jccl_ConfigElement_ConfigElement__jccl_ConfigElement1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public ConfigElement(jccl.ConfigElement p0)
   {
      mRawObject   = jccl_ConfigElement_ConfigElement__jccl_ConfigElement1(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal ConfigElement(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_jccl_ConfigElement(IntPtr obj);

   // Destructor.
   ~ConfigElement()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_jccl_ConfigElement(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   public override bool Equals(object obj)
   {
      if ( ! (obj is jccl.ConfigElement) )
      {
         return false;
      }
      return this == (jccl.ConfigElement) obj;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool jccl_ConfigElement_equal__jccl_ConfigElement(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement rhs);

   public static bool operator==(jccl.ConfigElement lhs, jccl.ConfigElement rhs)
   {
      bool result;
      result = jccl_ConfigElement_equal__jccl_ConfigElement(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool jccl_ConfigElement_not_equal__jccl_ConfigElement(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement rhs);

   public static bool operator!=(jccl.ConfigElement lhs, jccl.ConfigElement rhs)
   {
      bool result;
      result = jccl_ConfigElement_not_equal__jccl_ConfigElement(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool jccl_ConfigElement_less_than__jccl_ConfigElement(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement rhs);

   public static bool operator<(jccl.ConfigElement lhs, jccl.ConfigElement rhs)
   {
      bool result;
      result = jccl_ConfigElement_less_than__jccl_ConfigElement(lhs.mRawObject, rhs);
      return result;
   }

   public static bool operator>(jccl.ConfigElement lhs, jccl.ConfigElement rhs)
   {
      return (! (lhs < rhs) && ! (lhs == rhs));
   }

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool jccl_ConfigElement_isValid__0(IntPtr obj);

   public  bool isValid()
   {
      bool result;
      result = jccl_ConfigElement_isValid__0(mRawObject);
      return result;
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static void jccl_ConfigElement_assertValid__0(IntPtr obj);

   public  void assertValid()
   {
      jccl_ConfigElement_assertValid__0(mRawObject);
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))]
   private extern static jccl.ConfigElement jccl_ConfigElement_getChildElement__std_string1(IntPtr obj,
	string p0);

   public  jccl.ConfigElement getChildElement(string p0)
   {
      jccl.ConfigElement result;
      result = jccl_ConfigElement_getChildElement__std_string1(mRawObject, p0);
      return result;
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static int jccl_ConfigElement_getNum__std_string1(IntPtr obj,
	string p0);

   public  int getNum(string p0)
   {
      int result;
      result = jccl_ConfigElement_getNum__std_string1(mRawObject, p0);
      return result;
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static string jccl_ConfigElement_getName__0(IntPtr obj);

   public  string getName()
   {
      string result;
      result = jccl_ConfigElement_getName__0(mRawObject);
      return result;
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static string jccl_ConfigElement_getFullName__0(IntPtr obj);

   public  string getFullName()
   {
      string result;
      result = jccl_ConfigElement_getFullName__0(mRawObject);
      return result;
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static uint jccl_ConfigElement_getVersion__0(IntPtr obj);

   public  uint getVersion()
   {
      uint result;
      result = jccl_ConfigElement_getVersion__0(mRawObject);
      return result;
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static string jccl_ConfigElement_getID__0(IntPtr obj);

   public  string getID()
   {
      string result;
      result = jccl_ConfigElement_getID__0(mRawObject);
      return result;
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool jccl_ConfigElement_setProperty__std_string_int_bool3(IntPtr obj,
	string p0,
	int p1,
	bool p2);

   public  bool setProperty(string p0, int p1, bool p2)
   {
      bool result;
      result = jccl_ConfigElement_setProperty__std_string_int_bool3(mRawObject, p0, p1, p2);
      return result;
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool jccl_ConfigElement_setProperty__std_string_int_jccl_ConfigElementPtr3(IntPtr obj,
	string p0,
	int p1,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p2);

   public  bool setProperty(string p0, int p1, jccl.ConfigElement p2)
   {
      bool result;
      result = jccl_ConfigElement_setProperty__std_string_int_jccl_ConfigElementPtr3(mRawObject, p0, p1, p2);
      return result;
   }


   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static void jccl_ConfigElement_setDefinition__jccl_ConfigDefinitionPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigDefinitionMarshaler))] jccl.ConfigDefinition p0);

   public  void setDefinition(jccl.ConfigDefinition p0)
   {
      jccl_ConfigElement_setDefinition__jccl_ConfigDefinitionPtr1(mRawObject, p0);
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class jccl.ConfigElement

/// <summary>
/// Custom marshaler for jccl.ConfigElement.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ConfigElementMarshaler : ICustomMarshaler
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
      return ((jccl.ConfigElement) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new jccl.ConfigElement(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static ConfigElementMarshaler mInstance = new ConfigElementMarshaler();
}


} // namespace jccl
