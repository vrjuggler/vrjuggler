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

public interface ObjectWriter
{
   vpr.ReturnStatus beginTag(string p0);

   vpr.ReturnStatus endTag();

   vpr.ReturnStatus beginAttribute(string p0);

   vpr.ReturnStatus endAttribute();

   vpr.ReturnStatus writeUint8(byte p0);

   vpr.ReturnStatus writeUint16(ushort p0);

   vpr.ReturnStatus writeUint32(uint p0);

   vpr.ReturnStatus writeUint64(ulong p0);

   vpr.ReturnStatus writeFloat(float p0);

   vpr.ReturnStatus writeDouble(double p0);

   vpr.ReturnStatus writeString(string p0);

   vpr.ReturnStatus writeBool(bool p0);

}

/// <summary>
/// Custom marshaler for vpr.ObjectWriter.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class ObjectWriterMarshaler : ICustomMarshaler
{
   private class DummyObjectWriter : vpr.ObjectWriter
   {
      protected internal IntPtr mRawObject = IntPtr.Zero;

      public DummyObjectWriter(IntPtr rawObject)
      {
         mRawObject = rawObject;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_beginTag__std_basic_string_char_std__char_traits_char__std__allocator_char__(IntPtr obj,
	string p0);

      public vpr.ReturnStatus beginTag(string p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_beginTag__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_endTag__(IntPtr obj);

      public vpr.ReturnStatus endTag()
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

      public vpr.ReturnStatus beginAttribute(string p0)
      {
         vpr.ReturnStatus result;
         result = vpr_ObjectWriter_beginAttribute__std_basic_string_char_std__char_traits_char__std__allocator_char__(mRawObject, p0);
         return result;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_ObjectWriter_endAttribute__(IntPtr obj);

      public vpr.ReturnStatus endAttribute()
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

      public vpr.ReturnStatus writeUint8(byte p0)
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

      public vpr.ReturnStatus writeUint16(ushort p0)
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

      public vpr.ReturnStatus writeUint32(uint p0)
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

      public vpr.ReturnStatus writeUint64(ulong p0)
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

      public vpr.ReturnStatus writeFloat(float p0)
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

      public vpr.ReturnStatus writeDouble(double p0)
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

      public vpr.ReturnStatus writeString(string p0)
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

      public vpr.ReturnStatus writeBool(bool p0)
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
      PropertyInfo raw_obj_prop =
         obj.GetType().GetProperty("mRawObject",
                                   BindingFlags.NonPublic | BindingFlags.Instance);
      if ( null != raw_obj_prop )
      {
         return (IntPtr) raw_obj_prop.GetValue(obj, null);
      }

      return IntPtr.Zero;
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

   private static vpr.ReturnStatusMarshaler mInstance = new vpr.ReturnStatusMarshaler();
}


} // namespace vpr
