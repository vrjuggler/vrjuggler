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





namespace gadget
{

public class PositionData
   : gadget.InputData
{
   // Constructors.
   protected PositionData(NoInitTag doInit)
      : base(doInit)
   {
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_PositionData_PositionData__gadget_PositionData([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gadget.PositionDataMarshaler))] gadget.PositionData p0);

   public PositionData(gadget.PositionData p0)
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      
      mRawObject   = gadget_PositionData_PositionData__gadget_PositionData(p0);
      mWeOwnMemory = true;
      
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gadget_PositionData_PositionData__();

   public PositionData()
      : base(new NoInitTag())   // Do not initialize mRawObject in base class
   {
      mRawObject   = gadget_PositionData_PositionData__();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal PositionData(IntPtr instPtr, bool ownMemory)
      : base(new NoInitTag())
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gadget_PositionData(IntPtr obj);

   // Destructor.
   ~PositionData()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gadget_PositionData(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.


   // Start of non-virtual methods.
   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gadget_PositionData_getPosition__(IntPtr obj);

   public  gmtl.Matrix44f getPosition()
   {
      gmtl.Matrix44f result;
      result = gadget_PositionData_getPosition__(mRawObject);
      return result;
   }

   [DllImport("gadget_bridge", CharSet = CharSet.Ansi)]
   private extern static void gadget_PositionData_setPosition__gmtl_Matrix44f(IntPtr obj,
	gmtl.Matrix44f p0);

   public  void setPosition(gmtl.Matrix44f p0)
   {
      gadget_PositionData_setPosition__gmtl_Matrix44f(mRawObject, p0);
   }

   // End of non-virtual methods.


} // class gadget.PositionData

/// <summary>
/// Custom marshaler for gadget.PositionData.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class PositionDataMarshaler : ICustomMarshaler
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
      return ((gadget.PositionData) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gadget.PositionData(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static PositionDataMarshaler mInstance = new PositionDataMarshaler();
}


} // namespace gadget
