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





namespace vpr
{

public class GUID
   : vpr.SerializableObject
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
      m_writeObjectDelegate_vpr_ObjectWriter = new writeObjectDelegate_vpr_ObjectWriter(writeObjectAdapter);
      m_readObjectDelegate_vpr_ObjectReader = new readObjectDelegate_vpr_ObjectReader(readObjectAdapter);
   }

   // Constructors.
   protected GUID(NoInitTag doInit)
   {
      allocDelegates();
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_GUID__vpr_GUID_GenerateTag([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUID.GenerateTagMarshaler))] vpr.GUID.GenerateTag p0,
	[MarshalAs(UnmanagedType.FunctionPtr)] writeObjectDelegate_vpr_ObjectWriter d0,
	[MarshalAs(UnmanagedType.FunctionPtr)] readObjectDelegate_vpr_ObjectReader d1);

   public GUID(vpr.GUID.GenerateTag p0)
   {
      allocDelegates();
      
      mRawObject   = vpr_GUID_GUID__vpr_GUID_GenerateTag(p0, m_writeObjectDelegate_vpr_ObjectWriter, m_readObjectDelegate_vpr_ObjectReader);
      mWeOwnMemory = true;
      
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_GUID__([MarshalAs(UnmanagedType.FunctionPtr)] writeObjectDelegate_vpr_ObjectWriter d0,
	[MarshalAs(UnmanagedType.FunctionPtr)] readObjectDelegate_vpr_ObjectReader d1);

   public GUID()
   {
      allocDelegates();
      mRawObject   = vpr_GUID_GUID__(m_writeObjectDelegate_vpr_ObjectWriter, m_readObjectDelegate_vpr_ObjectReader);
      mWeOwnMemory = true;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_GUID__vpr_GUID_StdGUID([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUID.StdGUIDMarshaler))] vpr.GUID.StdGUID p0,
	[MarshalAs(UnmanagedType.FunctionPtr)] writeObjectDelegate_vpr_ObjectWriter d0,
	[MarshalAs(UnmanagedType.FunctionPtr)] readObjectDelegate_vpr_ObjectReader d1);

   public GUID(vpr.GUID.StdGUID p0)
   {
      allocDelegates();
      
      mRawObject   = vpr_GUID_GUID__vpr_GUID_StdGUID(p0, m_writeObjectDelegate_vpr_ObjectWriter, m_readObjectDelegate_vpr_ObjectReader);
      mWeOwnMemory = true;
      
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_GUID__std_basic_string_char_std__char_traits_char__std__allocator_char__(string p0,
	[MarshalAs(UnmanagedType.FunctionPtr)] writeObjectDelegate_vpr_ObjectWriter d0,
	[MarshalAs(UnmanagedType.FunctionPtr)] readObjectDelegate_vpr_ObjectReader d1);

   public GUID(string p0)
   {
      allocDelegates();
      mRawObject   = vpr_GUID_GUID__std_basic_string_char_std__char_traits_char__std__allocator_char__(p0, m_writeObjectDelegate_vpr_ObjectWriter, m_readObjectDelegate_vpr_ObjectReader);
      mWeOwnMemory = true;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_GUID__vpr_GUID_std_basic_string_char_std__char_traits_char__std__allocator_char__([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUIDMarshaler))] vpr.GUID p0,
	string p1,
	[MarshalAs(UnmanagedType.FunctionPtr)] writeObjectDelegate_vpr_ObjectWriter d0,
	[MarshalAs(UnmanagedType.FunctionPtr)] readObjectDelegate_vpr_ObjectReader d1);

   public GUID(vpr.GUID p0, string p1)
   {
      allocDelegates();
      
      mRawObject   = vpr_GUID_GUID__vpr_GUID_std_basic_string_char_std__char_traits_char__std__allocator_char__(p0, p1, m_writeObjectDelegate_vpr_ObjectWriter, m_readObjectDelegate_vpr_ObjectReader);
      mWeOwnMemory = true;
      
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_GUID__vpr_GUID([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUIDMarshaler))] vpr.GUID p0);

   public GUID(vpr.GUID p0)
   {
      
      mRawObject   = vpr_GUID_GUID__vpr_GUID(p0);
      mWeOwnMemory = true;
      
   }

   // Internal constructor needed for marshaling purposes.
   internal GUID(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vpr_GUID(IntPtr obj);

   // Destructor.
   ~GUID()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vpr_GUID(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   public override bool Equals(object obj)
   {
      if ( ! (obj is vpr.GUID) )
      {
         return false;
      }
      return this == (vpr.GUID) obj;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_GUID_equal__vpr_GUID(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUIDMarshaler))] vpr.GUID rhs);

   public static bool operator==(vpr.GUID lhs, vpr.GUID rhs)
   {
      
      bool result;
      result = vpr_GUID_equal__vpr_GUID(lhs.mRawObject, rhs);
      
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_GUID_not_equal__vpr_GUID(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUIDMarshaler))] vpr.GUID rhs);

   public static bool operator!=(vpr.GUID lhs, vpr.GUID rhs)
   {
      
      bool result;
      result = vpr_GUID_not_equal__vpr_GUID(lhs.mRawObject, rhs);
      
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_GUID_less_than__vpr_GUID(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUIDMarshaler))] vpr.GUID rhs);

   public static bool operator<(vpr.GUID lhs, vpr.GUID rhs)
   {
      
      bool result;
      result = vpr_GUID_less_than__vpr_GUID(lhs.mRawObject, rhs);
      
      return result;
   }


   // Start of non-virtual methods.
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static string vpr_GUID_toString__(IntPtr obj);

   public  string toString()
   {
      string result;
      result = vpr_GUID_toString__(mRawObject);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_GUID_generate__(IntPtr obj);

   public  void generate()
   {
      vpr_GUID_generate__(mRawObject);
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_GUID_generate__vpr_GUID_std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUIDMarshaler))] vpr.GUID p0,
	string p1);

   public  void generate(vpr.GUID p0, string p1)
   {
      
      vpr_GUID_generate__vpr_GUID_std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0, p1);
      
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // Delegate for the writeObject() callback.
   protected IntPtr writeObjectAdapter(vpr.ObjectWriter p0)
   {
      return writeObject(p0).mRawObject;
   }

   public delegate IntPtr writeObjectDelegate_vpr_ObjectWriter([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ObjectWriterMarshaler))] vpr.ObjectWriter p0);
   protected writeObjectDelegate_vpr_ObjectWriter m_writeObjectDelegate_vpr_ObjectWriter;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
   private extern static vpr.ReturnStatus vpr_GUID_writeObject__vpr_ObjectWriter(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ObjectWriterMarshaler))] vpr.ObjectWriter p0);

   public virtual vpr.ReturnStatus writeObject(vpr.ObjectWriter p0)
   {
      
      vpr.ReturnStatus result;
      result = vpr_GUID_writeObject__vpr_ObjectWriter(mRawObject, p0);
      
      return result;
   }

   // Delegate for the readObject() callback.
   protected IntPtr readObjectAdapter(vpr.ObjectReader p0)
   {
      return readObject(p0).mRawObject;
   }

   public delegate IntPtr readObjectDelegate_vpr_ObjectReader([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ObjectReaderMarshaler))] vpr.ObjectReader p0);
   protected readObjectDelegate_vpr_ObjectReader m_readObjectDelegate_vpr_ObjectReader;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
   private extern static vpr.ReturnStatus vpr_GUID_readObject__vpr_ObjectReader(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ObjectReaderMarshaler))] vpr.ObjectReader p0);

   public virtual vpr.ReturnStatus readObject(vpr.ObjectReader p0)
   {
      
      vpr.ReturnStatus result;
      result = vpr_GUID_readObject__vpr_ObjectReader(mRawObject, p0);
      
      return result;
   }

   // End of virtual methods.

   // Start of static data.
   // Getter function for vpr.GUID.generateTag
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_generateTag_get();

   // Setter function for vpr.GUID.generateTag
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_GUID_generateTag_set(IntPtr v);

   // Property for vpr.GUID.generateTag
   public static vpr.GUID.GenerateTag generateTag
   {
      get { return new vpr.GUID.GenerateTag(vpr_GUID_generateTag_get(), false); }
      set { vpr_GUID_generateTag_set(value.mRawObject); }
   }

   // Getter function for vpr.GUID.NullGUID
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_NullGUID_get();


   // Property for vpr.GUID.NullGUID
   public static vpr.GUID NullGUID
   {
      get { return new vpr.GUID(vpr_GUID_NullGUID_get(), false); }
   }

   // End of static data.


   // Nested class vpr.GUID.GenerateTag.
public class GenerateTag
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected GenerateTag(NoInitTag doInit)
   {
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_GenerateTag_GenerateTag__vpr_GUID_GenerateTag([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUID.GenerateTagMarshaler))] vpr.GUID.GenerateTag p0);

   public GenerateTag(vpr.GUID.GenerateTag p0)
   {
      
      mRawObject   = vpr_GUID_GenerateTag_GenerateTag__vpr_GUID_GenerateTag(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_GenerateTag_GenerateTag__();

   public GenerateTag()
   {
      mRawObject   = vpr_GUID_GenerateTag_GenerateTag__();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal GenerateTag(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vpr_GUID_GenerateTag(IntPtr obj);

   // Destructor.
   ~GenerateTag()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vpr_GUID_GenerateTag(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.



} // class vpr.GUID.GenerateTag

/// <summary>
/// Custom marshaler for vpr.GUID.GenerateTag.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class GenerateTagMarshaler : ICustomMarshaler
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
      return ((vpr.GUID.GenerateTag) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vpr.GUID.GenerateTag(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static GenerateTagMarshaler mInstance = new GenerateTagMarshaler();
}


   // Nested class vpr.GUID.StdGUID.
public class StdGUID
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected StdGUID(NoInitTag doInit)
   {
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_StdGUID_StdGUID__vpr_GUID_StdGUID([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUID.StdGUIDMarshaler))] vpr.GUID.StdGUID p0);

   public StdGUID(vpr.GUID.StdGUID p0)
   {
      
      mRawObject   = vpr_GUID_StdGUID_StdGUID__vpr_GUID_StdGUID(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_StdGUID_StdGUID__();

   public StdGUID()
   {
      mRawObject   = vpr_GUID_StdGUID_StdGUID__();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal StdGUID(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vpr_GUID_StdGUID(IntPtr obj);

   // Destructor.
   ~StdGUID()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vpr_GUID_StdGUID(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.



} // class vpr.GUID.StdGUID

/// <summary>
/// Custom marshaler for vpr.GUID.StdGUID.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class StdGUIDMarshaler : ICustomMarshaler
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
      return ((vpr.GUID.StdGUID) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vpr.GUID.StdGUID(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static StdGUIDMarshaler mInstance = new StdGUIDMarshaler();
}


   // Nested class vpr.GUID.hash.
public class hash
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected hash(NoInitTag doInit)
   {
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_hash_hash__vpr_GUID_hash([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.GUID.hashMarshaler))] vpr.GUID.hash p0);

   public hash(vpr.GUID.hash p0)
   {
      
      mRawObject   = vpr_GUID_hash_hash__vpr_GUID_hash(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_GUID_hash_hash__();

   public hash()
   {
      mRawObject   = vpr_GUID_hash_hash__();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal hash(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vpr_GUID_hash(IntPtr obj);

   // Destructor.
   ~hash()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vpr_GUID_hash(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.



} // class vpr.GUID.hash

/// <summary>
/// Custom marshaler for vpr.GUID.hash.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class hashMarshaler : ICustomMarshaler
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
      return ((vpr.GUID.hash) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vpr.GUID.hash(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static hashMarshaler mInstance = new hashMarshaler();
}


} // class vpr.GUID

/// <summary>
/// Custom marshaler for vpr.GUID.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class GUIDMarshaler : ICustomMarshaler
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
      return ((vpr.GUID) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vpr.GUID(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static GUIDMarshaler mInstance = new GUIDMarshaler();
}


} // namespace vpr
