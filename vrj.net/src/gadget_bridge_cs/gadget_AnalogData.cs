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

public class AnalogData
   : gadget.InputData
{
   // Constructors.
   protected AnalogData(NoInitTag doInit)
      : base(doInit)
   {
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_AnalogData_AnalogData__gadget_AnalogData1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.AnalogDataMarshaler))] gadget.AnalogData p0);

   public AnalogData(gadget.AnalogData p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gadget_AnalogData_AnalogData__gadget_AnalogData1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_AnalogData_AnalogData__0();

   public AnalogData()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gadget_AnalogData_AnalogData__0();
      mWeOwnMemory = true;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_AnalogData_AnalogData__float1(float p0);

   public AnalogData(float p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gadget_AnalogData_AnalogData__float1(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal AnalogData(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_AnalogData(IntPtr obj);

   // Destructor.
   ~AnalogData()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_AnalogData(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static float gadget_AnalogData_getAnalog__0(IntPtr obj);

   public  float getAnalog()
   {
      float result;
      result = gadget_AnalogData_getAnalog__0(mRawObject);
      return result;
   }


   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_AnalogData_setAnalog__float1(IntPtr obj,
	float p0);

   public  void setAnalog(float p0)
   {
      gadget_AnalogData_setAnalog__float1(mRawObject, p0);
   }


   // End of non-virtual methods.


} // class gadget.AnalogData

/// <summary>
/// Custom marshaler for gadget.AnalogData.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class AnalogDataMarshaler : ICustomMarshaler
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
      return ((gadget.AnalogData) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.AnalogData(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static AnalogDataMarshaler mInstance = new AnalogDataMarshaler();
}


} // namespace gadget
