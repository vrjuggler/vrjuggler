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

// Generated from Revision: 1.70 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace jccl
{

public sealed class ConfigDefinition
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected ConfigDefinition(NoInitTag doInit)
   {
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr jccl_ConfigDefinition_ConfigDefinition__();

   public ConfigDefinition()
   {
      mRawObject   = jccl_ConfigDefinition_ConfigDefinition__();
      mWeOwnMemory = true;
   }

/*
   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr jccl_ConfigDefinition_ConfigDefinition__std_basic_string_char_std__char_traits_char__std__allocator_char___cppdom_boost_shared_ptr_cppdom__Node(string p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(cppdom.NodeMarshaler))] cppdom.Node p1);

   public ConfigDefinition(string p0, cppdom.Node p1)
   {
      mRawObject   = jccl_ConfigDefinition_ConfigDefinition__std_basic_string_char_std__char_traits_char__std__allocator_char___cppdom_boost_shared_ptr_cppdom__Node(p0, p1);
      mWeOwnMemory = true;
   }
*/

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr jccl_ConfigDefinition_ConfigDefinition__jccl_ConfigDefinition([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigDefinitionMarshaler))] jccl.ConfigDefinition p0);

   public ConfigDefinition(jccl.ConfigDefinition p0)
   {
      
      mRawObject   = jccl_ConfigDefinition_ConfigDefinition__jccl_ConfigDefinition(p0);
      mWeOwnMemory = true;
      
   }

   // Internal constructor needed for marshaling purposes.
   internal ConfigDefinition(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_jccl_ConfigDefinition(IntPtr obj);

   // Destructor.
   ~ConfigDefinition()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_jccl_ConfigDefinition(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   public override bool Equals(object obj)
   {
      if ( ! (obj is jccl.ConfigDefinition) )
      {
         return false;
      }
      return this == (jccl.ConfigDefinition) obj;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool jccl_ConfigDefinition_equal__jccl_ConfigDefinition(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigDefinitionMarshaler))] jccl.ConfigDefinition rhs);

   public static bool operator==(jccl.ConfigDefinition lhs, jccl.ConfigDefinition rhs)
   {
      
      bool result;
      result = jccl_ConfigDefinition_equal__jccl_ConfigDefinition(lhs.mRawObject, rhs);
      
      return result;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static bool jccl_ConfigDefinition_not_equal__jccl_ConfigDefinition(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigDefinitionMarshaler))] jccl.ConfigDefinition rhs);

   public static bool operator!=(jccl.ConfigDefinition lhs, jccl.ConfigDefinition rhs)
   {
      
      bool result;
      result = jccl_ConfigDefinition_not_equal__jccl_ConfigDefinition(lhs.mRawObject, rhs);
      
      return result;
   }

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static void jccl_ConfigDefinition_assertValid__(IntPtr obj);

   public  void assertValid()
   {
      jccl_ConfigDefinition_assertValid__(mRawObject);
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static string jccl_ConfigDefinition_getName__(IntPtr obj);

   public  string getName()
   {
      string result;
      result = jccl_ConfigDefinition_getName__(mRawObject);
      return result;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static string jccl_ConfigDefinition_getToken__(IntPtr obj);

   public  string getToken()
   {
      string result;
      result = jccl_ConfigDefinition_getToken__(mRawObject);
      return result;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static uint jccl_ConfigDefinition_getVersion__(IntPtr obj);

   public  uint getVersion()
   {
      uint result;
      result = jccl_ConfigDefinition_getVersion__(mRawObject);
      return result;
   }

   [DllImport("jccl_bridge", CharSet = CharSet.Ansi)]
   private extern static string jccl_ConfigDefinition_getHelp__(IntPtr obj);

   public  string getHelp()
   {
      string result;
      result = jccl_ConfigDefinition_getHelp__(mRawObject);
      return result;
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.


} // class jccl.ConfigDefinition

/// <summary>
/// Custom marshaler for jccl.ConfigDefinition.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ConfigDefinitionMarshaler : ICustomMarshaler
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
      return ((jccl.ConfigDefinition) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new jccl.ConfigDefinition(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static ConfigDefinitionMarshaler mInstance = new ConfigDefinitionMarshaler();
}


} // namespace jccl
