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





namespace vpr
{

public sealed class Interval
{
   protected internal IntPtr mRawObject = IntPtr.Zero;
   protected bool mWeOwnMemory = false;
   protected class NoInitTag {}

   internal IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   protected Interval(NoInitTag doInit)
   {
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_Interval_Interval__vpr_Interval1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.IntervalMarshaler))] vpr.Interval p0);

   public Interval(vpr.Interval p0)
   {
      mRawObject   = vpr_Interval_Interval__vpr_Interval1(p0);
      mWeOwnMemory = true;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_Interval_Interval__0();

   public Interval()
   {
      mRawObject   = vpr_Interval_Interval__0();
      mWeOwnMemory = true;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_Interval_Interval__vpr_Uint64_vpr_Interval_Unit2(ulong p0, vpr.Interval.Unit p1);

   public Interval(ulong p0, vpr.Interval.Unit p1)
   {
      mRawObject   = vpr_Interval_Interval__vpr_Uint64_vpr_Interval_Unit2(p0, p1);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Interval(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_vpr_Interval(IntPtr obj);

   // Destructor.
   ~Interval()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_vpr_Interval(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   public override bool Equals(object obj)
   {
      if ( ! (obj is vpr.Interval) )
      {
         return false;
      }
      return this == (vpr.Interval) obj;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_Interval_equal__vpr_Interval(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.IntervalMarshaler))] vpr.Interval rhs);

   public static bool operator==(vpr.Interval lhs, vpr.Interval rhs)
   {
      bool result;
      result = vpr_Interval_equal__vpr_Interval(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_Interval_not_equal__vpr_Interval(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.IntervalMarshaler))] vpr.Interval rhs);

   public static bool operator!=(vpr.Interval lhs, vpr.Interval rhs)
   {
      bool result;
      result = vpr_Interval_not_equal__vpr_Interval(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_Interval_less_than__vpr_Interval(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.IntervalMarshaler))] vpr.Interval rhs);

   public static bool operator<(vpr.Interval lhs, vpr.Interval rhs)
   {
      bool result;
      result = vpr_Interval_less_than__vpr_Interval(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_Interval_less_than_or_equal__vpr_Interval(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.IntervalMarshaler))] vpr.Interval rhs);

   public static bool operator<=(vpr.Interval lhs, vpr.Interval rhs)
   {
      bool result;
      result = vpr_Interval_less_than_or_equal__vpr_Interval(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static bool vpr_Interval_greater_than__vpr_Interval(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.IntervalMarshaler))] vpr.Interval rhs);

   public static bool operator>(vpr.Interval lhs, vpr.Interval rhs)
   {
      bool result;
      result = vpr_Interval_greater_than__vpr_Interval(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.IntervalMarshaler))]
   private extern static vpr.Interval vpr_Interval_add__vpr_Interval(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.IntervalMarshaler))] vpr.Interval rhs);

   public static vpr.Interval operator+(vpr.Interval lhs, vpr.Interval rhs)
   {
      vpr.Interval result;
      result = vpr_Interval_add__vpr_Interval(lhs.mRawObject, rhs);
      return result;
   }

   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.IntervalMarshaler))]
   private extern static vpr.Interval vpr_Interval_subtract__vpr_Interval(IntPtr lhs,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(vpr.IntervalMarshaler))] vpr.Interval rhs);

   public static vpr.Interval operator-(vpr.Interval lhs, vpr.Interval rhs)
   {
      vpr.Interval result;
      result = vpr_Interval_subtract__vpr_Interval(lhs.mRawObject, rhs);
      return result;
   }

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_set__vpr_Uint64_vpr_Interval_Unit2(IntPtr obj,
	ulong p0,
	vpr.Interval.Unit p1);

   public  void set(ulong p0, vpr.Interval.Unit p1)
   {
      vpr_Interval_set__vpr_Uint64_vpr_Interval_Unit2(mRawObject, p0, p1);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_setf__float_vpr_Interval_Unit2(IntPtr obj,
	float p0,
	vpr.Interval.Unit p1);

   public  void setf(float p0, vpr.Interval.Unit p1)
   {
      vpr_Interval_setf__float_vpr_Interval_Unit2(mRawObject, p0, p1);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_setd__double_vpr_Interval_Unit2(IntPtr obj,
	double p0,
	vpr.Interval.Unit p1);

   public  void setd(double p0, vpr.Interval.Unit p1)
   {
      vpr_Interval_setd__double_vpr_Interval_Unit2(mRawObject, p0, p1);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_setNow__0(IntPtr obj);

   public  void setNow()
   {
      vpr_Interval_setNow__0(mRawObject);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_setNowReal__0(IntPtr obj);

   public  void setNowReal()
   {
      vpr_Interval_setNowReal__0(mRawObject);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_sec__vpr_Uint641(IntPtr obj,
	ulong p0);

   public  void sec(ulong p0)
   {
      vpr_Interval_sec__vpr_Uint641(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static ulong vpr_Interval_sec__0(IntPtr obj);

   public  ulong sec()
   {
      ulong result;
      result = vpr_Interval_sec__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_secf__float1(IntPtr obj,
	float p0);

   public  void secf(float p0)
   {
      vpr_Interval_secf__float1(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static float vpr_Interval_secf__0(IntPtr obj);

   public  float secf()
   {
      float result;
      result = vpr_Interval_secf__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_secd__double1(IntPtr obj,
	double p0);

   public  void secd(double p0)
   {
      vpr_Interval_secd__double1(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static double vpr_Interval_secd__0(IntPtr obj);

   public  double secd()
   {
      double result;
      result = vpr_Interval_secd__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_msec__vpr_Uint641(IntPtr obj,
	ulong p0);

   public  void msec(ulong p0)
   {
      vpr_Interval_msec__vpr_Uint641(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static ulong vpr_Interval_msec__0(IntPtr obj);

   public  ulong msec()
   {
      ulong result;
      result = vpr_Interval_msec__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_msecf__float1(IntPtr obj,
	float p0);

   public  void msecf(float p0)
   {
      vpr_Interval_msecf__float1(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static float vpr_Interval_msecf__0(IntPtr obj);

   public  float msecf()
   {
      float result;
      result = vpr_Interval_msecf__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_msecd__double1(IntPtr obj,
	double p0);

   public  void msecd(double p0)
   {
      vpr_Interval_msecd__double1(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static double vpr_Interval_msecd__0(IntPtr obj);

   public  double msecd()
   {
      double result;
      result = vpr_Interval_msecd__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_usec__vpr_Uint641(IntPtr obj,
	ulong p0);

   public  void usec(ulong p0)
   {
      vpr_Interval_usec__vpr_Uint641(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static ulong vpr_Interval_usec__0(IntPtr obj);

   public  ulong usec()
   {
      ulong result;
      result = vpr_Interval_usec__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_usecf__float1(IntPtr obj,
	float p0);

   public  void usecf(float p0)
   {
      vpr_Interval_usecf__float1(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static float vpr_Interval_usecf__0(IntPtr obj);

   public  float usecf()
   {
      float result;
      result = vpr_Interval_usecf__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static void vpr_Interval_usecd__double1(IntPtr obj,
	double p0);

   public  void usecd(double p0)
   {
      vpr_Interval_usecd__double1(mRawObject, p0);
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static double vpr_Interval_usecd__0(IntPtr obj);

   public  double usecd()
   {
      double result;
      result = vpr_Interval_usecd__0(mRawObject);
      return result;
   }


   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static ulong vpr_Interval_getBaseVal__0(IntPtr obj);

   public  ulong getBaseVal()
   {
      ulong result;
      result = vpr_Interval_getBaseVal__0(mRawObject);
      return result;
   }


   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Start of static methods.
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(vpr.IntervalMarshaler))]
   private extern static vpr.Interval vpr_Interval_now__0();

   public static vpr.Interval now()
   {
      vpr.Interval result;
      result = vpr_Interval_now__0();
      return result;
   }


   // End of static methods.

   // Start of static data.
   // Getter function for vpr.Interval.NoWait
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_Interval_NoWait_get();


   // Property for vpr.Interval.NoWait
   public static vpr.Interval NoWait
   {
      get { return new vpr.Interval(vpr_Interval_NoWait_get(), false); }
   }

   // Getter function for vpr.Interval.NoTimeout
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_Interval_NoTimeout_get();


   // Property for vpr.Interval.NoTimeout
   public static vpr.Interval NoTimeout
   {
      get { return new vpr.Interval(vpr_Interval_NoTimeout_get(), false); }
   }

   // Getter function for vpr.Interval.HalfPeriod
   [DllImport("vpr_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr vpr_Interval_HalfPeriod_get();


   // Property for vpr.Interval.HalfPeriod
   public static vpr.Interval HalfPeriod
   {
      get { return new vpr.Interval(vpr_Interval_HalfPeriod_get(), false); }
   }

   // End of static data.

   // Nested enumeration vpr.Interval.Unit.
   public enum Unit
   {
      Msec = 1, Base = 3, Sec = 0, Usec = 2
   };


} // class vpr.Interval

/// <summary>
/// Custom marshaler for vpr.Interval.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class IntervalMarshaler : ICustomMarshaler
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
      return ((vpr.Interval) obj).mRawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new vpr.Interval(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static IntervalMarshaler mInstance = new IntervalMarshaler();
}


} // namespace vpr
