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

public interface WriteableObject
{
   vpr.ReturnStatus writeObject(vpr.ObjectWriter p0);

}

/// <summary>
/// Custom marshaler for vpr.WriteableObject.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class WriteableObjectMarshaler : ICustomMarshaler
{
   private class DummyWriteableObject : vpr.WriteableObject
   {
      protected internal IntPtr mRawObject = IntPtr.Zero;

      public DummyWriteableObject(IntPtr rawObject)
      {
         mRawObject = rawObject;
      }

      [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
      [return : MarshalAs(UnmanagedType.CustomMarshaler,
                          MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
      private extern static vpr.ReturnStatus vpr_WriteableObject_writeObject__vpr_ObjectWriter(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ObjectWriterMarshaler))] vpr.ObjectWriter p0);

      public vpr.ReturnStatus writeObject(vpr.ObjectWriter p0)
      {
         
         vpr.ReturnStatus result;
         result = vpr_WriteableObject_writeObject__vpr_ObjectWriter(mRawObject, p0);
         
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
         obj.GetType().GetProperty("RawObject",
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
      return new DummyWriteableObject(nativeObj);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static vpr.ReturnStatusMarshaler mInstance = new vpr.ReturnStatusMarshaler();
}


} // namespace vpr
