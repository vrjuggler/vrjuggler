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

// Generated from Revision: 1.68 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gadget
{

public class Digital
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
      m_configDelegate_boost_shared_ptr_jccl__ConfigElement = new configDelegate_boost_shared_ptr_jccl__ConfigElement(config);
      m_getBaseTypeDelegate = new getBaseTypeDelegate(getBaseType);
      m_writeObjectDelegate_vpr_ObjectWriter = new writeObjectDelegate_vpr_ObjectWriter(writeObjectAdapter);
      m_readObjectDelegate_vpr_ObjectReader = new readObjectDelegate_vpr_ObjectReader(readObjectAdapter);
   }

   // Constructors.
   protected Digital(NoInitTag doInit)
   {
      allocDelegates();
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_Digital_Digital__([MarshalAs(UnmanagedType.FunctionPtr)] configDelegate_boost_shared_ptr_jccl__ConfigElement d0,
	[MarshalAs(UnmanagedType.FunctionPtr)] getBaseTypeDelegate d1,
	[MarshalAs(UnmanagedType.FunctionPtr)] writeObjectDelegate_vpr_ObjectWriter d2,
	[MarshalAs(UnmanagedType.FunctionPtr)] readObjectDelegate_vpr_ObjectReader d3);

   public Digital()
   {
      allocDelegates();
      mRawObject   = gadget_Digital_Digital__(m_configDelegate_boost_shared_ptr_jccl__ConfigElement, m_getBaseTypeDelegate, m_writeObjectDelegate_vpr_ObjectWriter, m_readObjectDelegate_vpr_ObjectReader);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Digital(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_Digital(IntPtr obj);

   // Destructor.
   ~Digital()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_Digital(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.DigitalDataMarshaler))]
   private extern static gadget.DigitalData gadget_Digital_getDigitalData__int(IntPtr obj,
	int p0);

   public  gadget.DigitalData getDigitalData(int p0)
   {
      gadget.DigitalData result;
      result = gadget_Digital_getDigitalData__int(mRawObject, p0);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_Digital_swapDigitalBuffers__(IntPtr obj);

   public  void swapDigitalBuffers()
   {
      gadget_Digital_swapDigitalBuffers__(mRawObject);
   }

   // End of non-virtual methods.

   // Start of virtual methods.
   // Delegate for the config() callback.
   public delegate bool configDelegate_boost_shared_ptr_jccl__ConfigElement([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);
   protected configDelegate_boost_shared_ptr_jccl__ConfigElement m_configDelegate_boost_shared_ptr_jccl__ConfigElement;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_Digital_config__boost_shared_ptr_jccl__ConfigElement(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public virtual bool config(jccl.ConfigElement p0)
   {
      bool result;
      result = gadget_Digital_config__boost_shared_ptr_jccl__ConfigElement(mRawObject, p0);
      return result;
   }

   // Delegate for the getBaseType() callback.
   public delegate string getBaseTypeDelegate();
   protected getBaseTypeDelegate m_getBaseTypeDelegate;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static string gadget_Digital_getBaseType__(IntPtr obj);

   public virtual string getBaseType()
   {
      string result;
      result = gadget_Digital_getBaseType__(mRawObject);
      return result;
   }

   // Delegate for the writeObject() callback.
   protected IntPtr writeObjectAdapter(vpr.ObjectWriter p0)
   {
      return writeObject(p0).mRawObject;
   }

   public delegate IntPtr writeObjectDelegate_vpr_ObjectWriter([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ObjectWriterMarshaler))] vpr.ObjectWriter p0);
   protected writeObjectDelegate_vpr_ObjectWriter m_writeObjectDelegate_vpr_ObjectWriter;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
   private extern static vpr.ReturnStatus gadget_Digital_writeObject__vpr_ObjectWriter(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ObjectWriterMarshaler))] vpr.ObjectWriter p0);

   public virtual vpr.ReturnStatus writeObject(vpr.ObjectWriter p0)
   {
      
      vpr.ReturnStatus result;
      result = gadget_Digital_writeObject__vpr_ObjectWriter(mRawObject, p0);
      
      return result;
   }

   // Delegate for the readObject() callback.
   protected IntPtr readObjectAdapter(vpr.ObjectReader p0)
   {
      return readObject(p0).mRawObject;
   }

   public delegate IntPtr readObjectDelegate_vpr_ObjectReader([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ObjectReaderMarshaler))] vpr.ObjectReader p0);
   protected readObjectDelegate_vpr_ObjectReader m_readObjectDelegate_vpr_ObjectReader;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.ReturnStatusMarshaler))]
   private extern static vpr.ReturnStatus gadget_Digital_readObject__vpr_ObjectReader(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.ObjectReaderMarshaler))] vpr.ObjectReader p0);

   public virtual vpr.ReturnStatus readObject(vpr.ObjectReader p0)
   {
      
      vpr.ReturnStatus result;
      result = gadget_Digital_readObject__vpr_ObjectReader(mRawObject, p0);
      
      return result;
   }

   // End of virtual methods.

   // Nested enumeration gadget.Digital.State.
   public enum State
   {
      TOGGLE_OFF = 3, ON = 1, OFF = 0, TOGGLE_ON = 2
   };


} // class gadget.Digital

/// <summary>
/// Custom marshaler for gadget.Digital.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class DigitalMarshaler : ICustomMarshaler
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
      return ((gadget.Digital) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.Digital(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static DigitalMarshaler mInstance = new DigitalMarshaler();
}


} // namespace gadget
