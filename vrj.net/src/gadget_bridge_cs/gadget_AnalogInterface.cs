#region License
// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$
#endregion License

// Generated from Revision: 1.79 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gadget
{

public sealed class AnalogInterface
   : gadget.BaseDeviceInterface
{
   private void allocDelegates()
   {
   }

   // Constructors.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_DeviceInterface_gadget_AnalogProxy__DeviceInterface__gadget_AnalogInterface1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.AnalogInterfaceMarshaler))] gadget.AnalogInterface p0);

   public AnalogInterface(gadget.AnalogInterface p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = gadget_DeviceInterface_gadget_AnalogProxy__DeviceInterface__gadget_AnalogInterface1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_DeviceInterface_gadget_AnalogProxy__DeviceInterface__0();

   public AnalogInterface()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = gadget_DeviceInterface_gadget_AnalogProxy__DeviceInterface__0();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal AnalogInterface(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_AnalogInterface(IntPtr obj);

   // Destructor.
   ~AnalogInterface()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_AnalogInterface(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.AnalogProxyMarshaler))]
   private extern static gadget.AnalogProxy gadget_DeviceInterface_gadget_AnalogProxy__getProxy__0(IntPtr obj);

   public  gadget.AnalogProxy getProxy()
   {
      gadget.AnalogProxy result;
      result = gadget_DeviceInterface_gadget_AnalogProxy__getProxy__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Start of virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_DeviceInterface_gadget_AnalogProxy__refresh__0(IntPtr obj);

   public override void refresh()
   {
      gadget_DeviceInterface_gadget_AnalogProxy__refresh__0(mRawObject);
   }


   // End of virtual methods.


} // class gadget.AnalogInterface

/// <summary>
/// Custom marshaler for gadget.AnalogInterface.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class AnalogInterfaceMarshaler : ICustomMarshaler
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
      return ((gadget.AnalogInterface) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.AnalogInterface(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static AnalogInterfaceMarshaler mInstance = new AnalogInterfaceMarshaler();
}


} // namespace gadget
