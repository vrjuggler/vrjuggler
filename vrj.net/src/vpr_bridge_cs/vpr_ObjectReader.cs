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

public abstract class ObjectReader
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
      m_resetReadingDelegate = new resetReadingDelegate(resetReading);
      m_pushStateDelegate = new pushStateDelegate(pushState);
      m_popStateDelegate = new popStateDelegate(popState);
      m_readUint8Delegate = new readUint8Delegate(readUint8);
      m_readUint16Delegate = new readUint16Delegate(readUint16);
      m_readUint32Delegate = new readUint32Delegate(readUint32);
      m_readUint64Delegate = new readUint64Delegate(readUint64);
      m_readFloatDelegate = new readFloatDelegate(readFloat);
      m_readDoubleDelegate = new readDoubleDelegate(readDouble);
      m_readStringDelegate = new readStringDelegate(readString);
      m_readBoolDelegate = new readBoolDelegate(readBool);
      m_readUint8Delegate_unsigned_char = new readUint8Delegate_unsigned_char(readUint8);
      m_readUint16Delegate_short_unsigned_int = new readUint16Delegate_short_unsigned_int(readUint16);
      m_readUint32Delegate_unsigned_int = new readUint32Delegate_unsigned_int(readUint32);
      m_readUint64Delegate_long_long_unsigned_int = new readUint64Delegate_long_long_unsigned_int(readUint64);
      m_readFloatDelegate_float = new readFloatDelegate_float(readFloat);
      m_readDoubleDelegate_double = new readDoubleDelegate_double(readDouble);
      m_readStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__ = new readStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__(readString);
      m_readBoolDelegate_bool = new readBoolDelegate_bool(readBool);
   }

   // Constructors.
   protected ObjectReader(NoInitTag doInit)
   {
      allocDelegates();
   }

   // Internal constructor needed for marshaling purposes.
   internal ObjectReader(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vpr_ObjectReader(IntPtr obj);

   // Destructor.
   ~ObjectReader()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vpr_ObjectReader(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.


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

   // Delegate for the resetReading() callback.
   public delegate void resetReadingDelegate();
   protected resetReadingDelegate m_resetReadingDelegate;

   public abstract void resetReading();

   // Delegate for the pushState() callback.
   public delegate void pushStateDelegate();
   protected pushStateDelegate m_pushStateDelegate;

   public abstract void pushState();

   // Delegate for the popState() callback.
   public delegate void popStateDelegate();
   protected popStateDelegate m_popStateDelegate;

   public abstract void popState();

   // Delegate for the readUint8() callback.
   public delegate byte readUint8Delegate();
   protected readUint8Delegate m_readUint8Delegate;

   public abstract byte readUint8();

   // Delegate for the readUint16() callback.
   public delegate ushort readUint16Delegate();
   protected readUint16Delegate m_readUint16Delegate;

   public abstract ushort readUint16();

   // Delegate for the readUint32() callback.
   public delegate uint readUint32Delegate();
   protected readUint32Delegate m_readUint32Delegate;

   public abstract uint readUint32();

   // Delegate for the readUint64() callback.
   public delegate ulong readUint64Delegate();
   protected readUint64Delegate m_readUint64Delegate;

   public abstract ulong readUint64();

   // Delegate for the readFloat() callback.
   public delegate float readFloatDelegate();
   protected readFloatDelegate m_readFloatDelegate;

   public abstract float readFloat();

   // Delegate for the readDouble() callback.
   public delegate double readDoubleDelegate();
   protected readDoubleDelegate m_readDoubleDelegate;

   public abstract double readDouble();

   // Delegate for the readString() callback.
   public delegate string readStringDelegate();
   protected readStringDelegate m_readStringDelegate;

   public abstract string readString();

   // Delegate for the readBool() callback.
   public delegate bool readBoolDelegate();
   protected readBoolDelegate m_readBoolDelegate;

   public abstract bool readBool();

   // Delegate for the readUint8() callback.
   public delegate void readUint8Delegate_unsigned_char(ref byte p0);
   protected readUint8Delegate_unsigned_char m_readUint8Delegate_unsigned_char;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ObjectReader_readUint8__unsigned_char(IntPtr obj,
	ref byte p0);

   public virtual void readUint8(ref byte p0)
   {
      
      vpr_ObjectReader_readUint8__unsigned_char(mRawObject, ref p0);
      
   }

   // Delegate for the readUint16() callback.
   public delegate void readUint16Delegate_short_unsigned_int(ref ushort p0);
   protected readUint16Delegate_short_unsigned_int m_readUint16Delegate_short_unsigned_int;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ObjectReader_readUint16__short_unsigned_int(IntPtr obj,
	ref ushort p0);

   public virtual void readUint16(ref ushort p0)
   {
      
      vpr_ObjectReader_readUint16__short_unsigned_int(mRawObject, ref p0);
      
   }

   // Delegate for the readUint32() callback.
   public delegate void readUint32Delegate_unsigned_int(ref uint p0);
   protected readUint32Delegate_unsigned_int m_readUint32Delegate_unsigned_int;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ObjectReader_readUint32__unsigned_int(IntPtr obj,
	ref uint p0);

   public virtual void readUint32(ref uint p0)
   {
      
      vpr_ObjectReader_readUint32__unsigned_int(mRawObject, ref p0);
      
   }

   // Delegate for the readUint64() callback.
   public delegate void readUint64Delegate_long_long_unsigned_int(ref ulong p0);
   protected readUint64Delegate_long_long_unsigned_int m_readUint64Delegate_long_long_unsigned_int;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ObjectReader_readUint64__long_long_unsigned_int(IntPtr obj,
	ref ulong p0);

   public virtual void readUint64(ref ulong p0)
   {
      
      vpr_ObjectReader_readUint64__long_long_unsigned_int(mRawObject, ref p0);
      
   }

   // Delegate for the readFloat() callback.
   public delegate void readFloatDelegate_float(ref float p0);
   protected readFloatDelegate_float m_readFloatDelegate_float;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ObjectReader_readFloat__float(IntPtr obj,
	ref float p0);

   public virtual void readFloat(ref float p0)
   {
      
      vpr_ObjectReader_readFloat__float(mRawObject, ref p0);
      
   }

   // Delegate for the readDouble() callback.
   public delegate void readDoubleDelegate_double(ref double p0);
   protected readDoubleDelegate_double m_readDoubleDelegate_double;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ObjectReader_readDouble__double(IntPtr obj,
	ref double p0);

   public virtual void readDouble(ref double p0)
   {
      
      vpr_ObjectReader_readDouble__double(mRawObject, ref p0);
      
   }

   // Delegate for the readString() callback.
   public delegate void readStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__(string p0);
   protected readStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__ m_readStringDelegate_std_basic_string_char_std__char_traits_char__std__allocator_char__;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ObjectReader_readString__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

   public virtual void readString(string p0)
   {
      vpr_ObjectReader_readString__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
   }

   // Delegate for the readBool() callback.
   public delegate void readBoolDelegate_bool(ref bool p0);
   protected readBoolDelegate_bool m_readBoolDelegate_bool;

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_ObjectReader_readBool__bool(IntPtr obj,
	ref bool p0);

   public virtual void readBool(ref bool p0)
   {
      
      vpr_ObjectReader_readBool__bool(mRawObject, ref p0);
      
   }

   // End of virtual methods.


} // class vpr.ObjectReader

/// <summary>
/// Custom marshaler for vpr.ObjectReader.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ObjectReaderMarshaler : ICustomMarshaler
{
   private class DummyObjectReader : vpr.ObjectReader
   {

      public DummyObjectReader(IntPtr rawObject)
         : base(new NoInitTag())
      {
         mRawObject = rawObject;
         mWeOwnMemory = false;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectReader_beginTag__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

      public override vpr.ReturnStatus beginTag(string p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectReader_beginTag__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectReader_endTag__(IntPtr obj);

      public override vpr.ReturnStatus endTag()
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectReader_endTag__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectReader_beginAttribute__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

      public override vpr.ReturnStatus beginAttribute(string p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectReader_beginAttribute__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectReader_endAttribute__(IntPtr obj);

      public override vpr.ReturnStatus endAttribute()
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectReader_endAttribute__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static void vpr_ObjectReader_resetReading__(IntPtr obj);

      public override void resetReading()
      {
         vpr_ObjectReader_resetReading__(mRawObject);
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static void vpr_ObjectReader_pushState__(IntPtr obj);

      public override void pushState()
      {
         vpr_ObjectReader_pushState__(mRawObject);
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static void vpr_ObjectReader_popState__(IntPtr obj);

      public override void popState()
      {
         vpr_ObjectReader_popState__(mRawObject);
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static byte vpr_ObjectReader_readUint8__(IntPtr obj);

      public override byte readUint8()
      {
         byte result;
         result = vpr_ObjectReader_readUint8__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static ushort vpr_ObjectReader_readUint16__(IntPtr obj);

      public override ushort readUint16()
      {
         ushort result;
         result = vpr_ObjectReader_readUint16__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static uint vpr_ObjectReader_readUint32__(IntPtr obj);

      public override uint readUint32()
      {
         uint result;
         result = vpr_ObjectReader_readUint32__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static ulong vpr_ObjectReader_readUint64__(IntPtr obj);

      public override ulong readUint64()
      {
         ulong result;
         result = vpr_ObjectReader_readUint64__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static float vpr_ObjectReader_readFloat__(IntPtr obj);

      public override float readFloat()
      {
         float result;
         result = vpr_ObjectReader_readFloat__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static double vpr_ObjectReader_readDouble__(IntPtr obj);

      public override double readDouble()
      {
         double result;
         result = vpr_ObjectReader_readDouble__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static string vpr_ObjectReader_readString__(IntPtr obj);

      public override string readString()
      {
         string result;
         result = vpr_ObjectReader_readString__(mRawObject);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      private extern static bool vpr_ObjectReader_readBool__(IntPtr obj);

      public override bool readBool()
      {
         bool result;
         result = vpr_ObjectReader_readBool__(mRawObject);
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
      return ((vpr.ObjectReader) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new DummyObjectReader(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static vpr.ReturnStatusMarshaler mInstance = new vpr.ReturnStatusMarshaler();
}


} // namespace vpr
