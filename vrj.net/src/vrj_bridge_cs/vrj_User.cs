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

// Generated from Revision: 1.76 of RCSfile: class_cs.tmpl,v
using System;
using System.Runtime.InteropServices;
using System.Reflection;





namespace vrj
{

public sealed class User
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
   }

   // Constructors.
   protected User(NoInitTag doInit)
   {
      allocDelegates();
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vrj_User_User__0();

   public User()
   {
      allocDelegates();
      mRawObject   = vrj_User_User__0();
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal User(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vrj_User(IntPtr obj);

   // Destructor.
   ~User()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vrj_User(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static int vrj_User_getId__0(IntPtr obj);

   public  int getId()
   {
      int result;
      result = vrj_User_getId__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static string vrj_User_getName__0(IntPtr obj);

   public  string getName()
   {
      string result;
      result = vrj_User_getName__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gadget.PositionProxyMarshaler))]
   private extern static gadget.PositionProxy vrj_User_getHeadPosProxy__0(IntPtr obj);

   public  gadget.PositionProxy getHeadPosProxy()
   {
      gadget.PositionProxy result;
      result = vrj_User_getHeadPosProxy__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.IntervalMarshaler))]
   private extern static vpr.Interval vrj_User_getHeadUpdateTime__0(IntPtr obj);

   public  vpr.Interval getHeadUpdateTime()
   {
      vpr.Interval result;
      result = vrj_User_getHeadUpdateTime__0(mRawObject);
      return result;
   }


   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static float vrj_User_getInterocularDistance__0(IntPtr obj);

   public  float getInterocularDistance()
   {
      float result;
      result = vrj_User_getInterocularDistance__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Start of virtual methods.
   [DllImport("vrj_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vrj_User_config__jccl_ConfigElementPtr1(IntPtr obj,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(jccl.ConfigElementMarshaler))] jccl.ConfigElement p0);

   public  bool config(jccl.ConfigElement p0)
   {
      bool result;
      result = vrj_User_config__jccl_ConfigElementPtr1(mRawObject, p0);
      return result;
   }


   // End of virtual methods.

   // Start of static data.
   // End of static data.


} // class vrj.User

/// <summary>
/// Custom marshaler for vrj.User.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class UserMarshaler : ICustomMarshaler
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
      return ((vrj.User) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vrj.User(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static UserMarshaler mInstance = new UserMarshaler();
}


} // namespace vrj
