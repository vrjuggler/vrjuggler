#region License
// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$
#endregion License

// Generated from Revision: 1.78 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace gadget
{

public class BaseDeviceInterface
{
   protected IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   /// <summary>
   /// This is needed for the custom marshaler to be able to perform a
   /// reflective lookup.  The custom marshaler also uses this method to get
   /// access to the value of mRawObject when necessary.
   /// </summary>
   public IntPtr RawObject
   {
      get { return mRawObject; }
   }

   private void allocDelegates()
   {
      m_refreshDelegate = new refreshDelegate(refresh);
   }

   // Constructors.
   protected BaseDeviceInterface(NoInitTag doInit)
   {
      allocDelegates();
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_BaseDeviceInterface_BaseDeviceInterface__gadget_BaseDeviceInterface1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.BaseDeviceInterfaceMarshaler))] gadget.BaseDeviceInterface p0, [MarshalAs(UnmanagedType.FunctionPtr)] refreshDelegate d0);

   public BaseDeviceInterface(gadget.BaseDeviceInterface p0)
   {
      allocDelegates();
      mRawObject   = gadget_BaseDeviceInterface_BaseDeviceInterface__gadget_BaseDeviceInterface1(p0, m_refreshDelegate);
      mWeOwnMemory = true;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_BaseDeviceInterface_BaseDeviceInterface__0([MarshalAs(UnmanagedType.FunctionPtr)] refreshDelegate d0);

   public BaseDeviceInterface()
   {
      allocDelegates();
      mRawObject   = gadget_BaseDeviceInterface_BaseDeviceInterface__0(m_refreshDelegate);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal BaseDeviceInterface(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_BaseDeviceInterface(IntPtr obj);

   // Destructor.
   ~BaseDeviceInterface()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_BaseDeviceInterface(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_BaseDeviceInterface_init__std_string1(IntPtr obj,
	string p0);

   public  void init(string p0)
   {
      gadget_BaseDeviceInterface_init__std_string1(mRawObject, p0);
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static string gadget_BaseDeviceInterface_getProxyName__0(IntPtr obj);

   public  string getProxyName()
   {
      string result;
      result = gadget_BaseDeviceInterface_getProxyName__0(mRawObject);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static bool gadget_BaseDeviceInterface_isConnected__0(IntPtr obj);

   public  bool isConnected()
   {
      bool result;
      result = gadget_BaseDeviceInterface_isConnected__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // Delegate for the refresh() callback.
   public delegate void refreshDelegate();
   protected refreshDelegate m_refreshDelegate;

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_BaseDeviceInterface_refresh__0(IntPtr obj);

   public virtual void refresh()
   {
      gadget_BaseDeviceInterface_refresh__0(mRawObject);
   }


   // End of virtual methods.

   // Start of static methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_BaseDeviceInterface_refreshAllDevices__0();

   public static void refreshAllDevices()
   {
      gadget_BaseDeviceInterface_refreshAllDevices__0();
   }


   // End of static methods.

   // Start of static data.
   // End of static data.


} // class gadget.BaseDeviceInterface

/// <summary>
/// Custom marshaler for gadget.BaseDeviceInterface.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class BaseDeviceInterfaceMarshaler : ICustomMarshaler
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
      return ((gadget.BaseDeviceInterface) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.BaseDeviceInterface(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static BaseDeviceInterfaceMarshaler mInstance = new BaseDeviceInterfaceMarshaler();
}


} // namespace gadget
