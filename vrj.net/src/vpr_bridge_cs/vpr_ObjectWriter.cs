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





namespace vpr
{

public abstract class ObjectWriter
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
      m_beginTagDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__ = new beginTagDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__(beginTagAdapter);
      m_endTagDelegate = new endTagDelegate(endTagAdapter);
      m_beginAttributeDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__ = new beginAttributeDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__(beginAttributeAdapter);
      m_endAttributeDelegate = new endAttributeDelegate(endAttributeAdapter);
      m_writeUint8Delegate_unsigned_char = new writeUint8Delegate_unsigned_char(writeUint8Adapter);
      m_writeUint16Delegate_short_unsigned_int = new writeUint16Delegate_short_unsigned_int(writeUint16Adapter);
      m_writeUint32Delegate_unsigned_int = new writeUint32Delegate_unsigned_int(writeUint32Adapter);
      m_writeUint64Delegate_long_long_unsigned_int = new writeUint64Delegate_long_long_unsigned_int(writeUint64Adapter);
      m_writeFloatDelegate_float = new writeFloatDelegate_float(writeFloatAdapter);
      m_writeDoubleDelegate_double = new writeDoubleDelegate_double(writeDoubleAdapter);
      m_writeStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__ = new writeStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__(writeStringAdapter);
      m_writeBoolDelegate_bool = new writeBoolDelegate_bool(writeBoolAdapter);
   }

   // Constructors.
   protected ObjectWriter(NoInitTag doInit)
   {
      allocDelegates();
   }

   // Internal constructor needed for marshaling purposes.
   internal ObjectWriter(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vpr_ObjectWriter(IntPtr obj);

   // Destructor.
   ~ObjectWriter()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vpr_ObjectWriter(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_ObjectWriter_isBinary__(IntPtr obj);

   public  bool isBinary()
   {
      bool result;
      result = vpr_ObjectWriter_isBinary__(mRawObject);
      return result;
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // Delegate for the beginTag() callback.
   protected IntPtr beginTagAdapter(string p0)
   {
      return beginTag(p0).mRawObject;
   }

   public delegate IntPtr beginTagDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__(string p0);
   protected beginTagDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__ m_beginTagDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   public abstract vpr.ReturnStatus beginTag(string p0);

   // Delegate for the endTag() callback.
   protected IntPtr endTagAdapter()
   {
      return endTag().mRawObject;
   }

   public delegate IntPtr endTagDelegate();
   protected endTagDelegate m_endTagDelegate;

   public abstract vpr.ReturnStatus endTag();

   // Delegate for the beginAttribute() callback.
   protected IntPtr beginAttributeAdapter(string p0)
   {
      return beginAttribute(p0).mRawObject;
   }

   public delegate IntPtr beginAttributeDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__(string p0);
   protected beginAttributeDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__ m_beginAttributeDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   public abstract vpr.ReturnStatus beginAttribute(string p0);

   // Delegate for the endAttribute() callback.
   protected IntPtr endAttributeAdapter()
   {
      return endAttribute().mRawObject;
   }

   public delegate IntPtr endAttributeDelegate();
   protected endAttributeDelegate m_endAttributeDelegate;

   public abstract vpr.ReturnStatus endAttribute();

   // Delegate for the writeUint8() callback.
   protected IntPtr writeUint8Adapter(byte p0)
   {
      return writeUint8(p0).mRawObject;
   }

   public delegate IntPtr writeUint8Delegate_unsigned_char(byte p0);
   protected writeUint8Delegate_unsigned_char m_writeUint8Delegate_unsigned_char;

   public abstract vpr.ReturnStatus writeUint8(byte p0);

   // Delegate for the writeUint16() callback.
   protected IntPtr writeUint16Adapter(ushort p0)
   {
      return writeUint16(p0).mRawObject;
   }

   public delegate IntPtr writeUint16Delegate_short_unsigned_int(ushort p0);
   protected writeUint16Delegate_short_unsigned_int m_writeUint16Delegate_short_unsigned_int;

   public abstract vpr.ReturnStatus writeUint16(ushort p0);

   // Delegate for the writeUint32() callback.
   protected IntPtr writeUint32Adapter(uint p0)
   {
      return writeUint32(p0).mRawObject;
   }

   public delegate IntPtr writeUint32Delegate_unsigned_int(uint p0);
   protected writeUint32Delegate_unsigned_int m_writeUint32Delegate_unsigned_int;

   public abstract vpr.ReturnStatus writeUint32(uint p0);

   // Delegate for the writeUint64() callback.
   protected IntPtr writeUint64Adapter(ulong p0)
   {
      return writeUint64(p0).mRawObject;
   }

   public delegate IntPtr writeUint64Delegate_long_long_unsigned_int(ulong p0);
   protected writeUint64Delegate_long_long_unsigned_int m_writeUint64Delegate_long_long_unsigned_int;

   public abstract vpr.ReturnStatus writeUint64(ulong p0);

   // Delegate for the writeFloat() callback.
   protected IntPtr writeFloatAdapter(float p0)
   {
      return writeFloat(p0).mRawObject;
   }

   public delegate IntPtr writeFloatDelegate_float(float p0);
   protected writeFloatDelegate_float m_writeFloatDelegate_float;

   public abstract vpr.ReturnStatus writeFloat(float p0);

   // Delegate for the writeDouble() callback.
   protected IntPtr writeDoubleAdapter(double p0)
   {
      return writeDouble(p0).mRawObject;
   }

   public delegate IntPtr writeDoubleDelegate_double(double p0);
   protected writeDoubleDelegate_double m_writeDoubleDelegate_double;

   public abstract vpr.ReturnStatus writeDouble(double p0);

   // Delegate for the writeString() callback.
   protected IntPtr writeStringAdapter(string p0)
   {
      return writeString(p0).mRawObject;
   }

   public delegate IntPtr writeStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__(string p0);
   protected writeStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__ m_writeStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   public abstract vpr.ReturnStatus writeString(string p0);

   // Delegate for the writeBool() callback.
   protected IntPtr writeBoolAdapter(bool p0)
   {
      return writeBool(p0).mRawObject;
   }

   public delegate IntPtr writeBoolDelegate_bool(bool p0);
   protected writeBoolDelegate_bool m_writeBoolDelegate_bool;

   public abstract vpr.ReturnStatus writeBool(bool p0);

   // End of virtual methods.


} // class vpr.ObjectWriter

/// <summary>
/// Custom marshaler for vpr.ObjectWriter.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ObjectWriterMarshaler : ICustomMarshaler
{
   private class DummyObjectWriter : vpr.ObjectWriter
   {

      public DummyObjectWriter(IntPtr rawObject)
         : base(new NoInitTag())
      {
         mRawObject = rawObject;
         mWeOwnMemory = false;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_beginTag__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

      public override vpr.ReturnStatus beginTag(string p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_beginTag__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_endTag__(IntPtr obj);

      public override vpr.ReturnStatus endTag()
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_endTag__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_beginAttribute__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

      public override vpr.ReturnStatus beginAttribute(string p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_beginAttribute__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_endAttribute__(IntPtr obj);

      public override vpr.ReturnStatus endAttribute()
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_endAttribute__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_writeUint8__unsigned_char(IntPtr obj,
	byte p0);

      public override vpr.ReturnStatus writeUint8(byte p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_writeUint8__unsigned_char(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_writeUint16__short_unsigned_int(IntPtr obj,
	ushort p0);

      public override vpr.ReturnStatus writeUint16(ushort p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_writeUint16__short_unsigned_int(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_writeUint32__unsigned_int(IntPtr obj,
	uint p0);

      public override vpr.ReturnStatus writeUint32(uint p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_writeUint32__unsigned_int(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_writeUint64__long_long_unsigned_int(IntPtr obj,
	ulong p0);

      public override vpr.ReturnStatus writeUint64(ulong p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_writeUint64__long_long_unsigned_int(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_writeFloat__float(IntPtr obj,
	float p0);

      public override vpr.ReturnStatus writeFloat(float p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_writeFloat__float(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_writeDouble__double(IntPtr obj,
	double p0);

      public override vpr.ReturnStatus writeDouble(double p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_writeDouble__double(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_writeString__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

      public override vpr.ReturnStatus writeString(string p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_writeString__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_writeBool__bool(IntPtr obj,
	bool p0);

      public override vpr.ReturnStatus writeBool(bool p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_writeBool__bool(mRawObject, p0);
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
      return ((vpr.ObjectWriter) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyObjectWriter(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static ObjectWriterMarshaler mInstance = new ObjectWriterMarshaler();
}


} // namespace vpr
