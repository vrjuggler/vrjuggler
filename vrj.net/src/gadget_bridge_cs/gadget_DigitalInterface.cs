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

public sealed class DigitalInterface
   : gadget.BaseDeviceInterface
{
   private void allocDelegates()
   {
   }

   // Constructors.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_DeviceInterface_gadget_DigitalProxy__DeviceInterface__gadget_DigitalInterface1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.DigitalInterfaceMarshaler))] gadget.DigitalInterface p0);

   public DigitalInterface(gadget.DigitalInterface p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = gadget_DeviceInterface_gadget_DigitalProxy__DeviceInterface__gadget_DigitalInterface1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_DeviceInterface_gadget_DigitalProxy__DeviceInterface__0();

   public DigitalInterface()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      allocDelegates();
      mRawObject   = gadget_DeviceInterface_gadget_DigitalProxy__DeviceInterface__0();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal DigitalInterface(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_DigitalInterface(IntPtr obj);

   // Destructor.
   ~DigitalInterface()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_DigitalInterface(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.DigitalProxyMarshaler))]
   private extern static gadget.DigitalProxy gadget_DeviceInterface_gadget_DigitalProxy__getProxy__0(IntPtr obj);

   public  gadget.DigitalProxy getProxy()
   {
      gadget.DigitalProxy result;
      result = gadget_DeviceInterface_gadget_DigitalProxy__getProxy__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Start of virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_DeviceInterface_gadget_DigitalProxy__refresh__0(IntPtr obj);

   public override void refresh()
   {
      gadget_DeviceInterface_gadget_DigitalProxy__refresh__0(mRawObject);
   }


   // End of virtual methods.


} // class gadget.DigitalInterface

/// <summary>
/// Custom marshaler for gadget.DigitalInterface.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class DigitalInterfaceMarshaler : ICustomMarshaler
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
      return ((gadget.DigitalInterface) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.DigitalInterface(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static DigitalInterfaceMarshaler mInstance = new DigitalInterfaceMarshaler();
}


} // namespace gadget
