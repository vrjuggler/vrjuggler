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





namespace gmtl
{

public sealed class Quatf
{
   private IntPtr mRawObject = IntPtr.Zero;
   private bool mWeOwnMemory = false;

   /// <summary>
   /// This is needed for the custom marshaler to be able to perform a
   /// reflective lookup.  The custom marshaler also uses this method to get
   /// access to the value of mRawObject when necessary.
   /// </summary>
   public IntPtr RawObject
   {
      get { return mRawObject; }
   }

   // Constructors.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Quat_float__Quat__0();

   public Quatf()
   {
      mRawObject   = gmtl_Quat_float__Quat__0();
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Quat_float__Quat__float_float_float_float4(float p0, float p1, float p2, float p3);

   public Quatf(float p0, float p1, float p2, float p3)
   {
      mRawObject   = gmtl_Quat_float__Quat__float_float_float_float4(p0, p1, p2, p3);
      mWeOwnMemory = true;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static IntPtr gmtl_Quat_float__Quat__gmtl_Quatf1([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.QuatfMarshaler))] gmtl.Quatf p0);

   public Quatf(gmtl.Quatf p0)
   {
      mRawObject   = gmtl_Quat_float__Quat__gmtl_Quatf1(p0);
      mWeOwnMemory = true;
   }

   // Internal constructor needed for marshaling purposes.
   internal Quatf(IntPtr instPtr, bool ownMemory)
   {
      mRawObject   = instPtr;
      mWeOwnMemory = ownMemory;
   }

   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void delete_gmtl_Quatf(IntPtr obj);

   // Destructor.
   ~Quatf()
   {
      if ( mWeOwnMemory && IntPtr.Zero != mRawObject )
      {
         delete_gmtl_Quatf(mRawObject);
         mWeOwnMemory = false;
         mRawObject   = IntPtr.Zero;
      }
   }

   // Operator overloads.

   // Converter operators.

   // Start of non-virtual methods.
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Quat_float__set__float_float_float_float4(IntPtr obj,
	float p0,
	float p1,
	float p2,
	float p3);

   public  void set(float p0, float p1, float p2, float p3)
   {
      gmtl_Quat_float__set__float_float_float_float4(mRawObject, p0, p1, p2, p3);
   }


   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static void gmtl_Quat_float__get__float_float_float_float4(IntPtr obj,
	ref float p0,
	ref float p1,
	ref float p2,
	ref float p3);

   public  void get(ref float p0, ref float p1, ref float p2, ref float p3)
   {
      gmtl_Quat_float__get__float_float_float_float4(mRawObject, ref p0, ref p1, ref p2, ref p3);
   }

/*
   [DllImport("gmtl_bridge", CharSet = CharSet.Ansi)]
   private extern static float gmtl_Quat_float__getData__0(IntPtr obj);

   public  float getData()
   {
      float result;
      result = gmtl_Quat_float__getData__0(mRawObject);
      return result;
   }
*/

   // End of non-virtual methods.

   // Start of virtual methods.
   // End of virtual methods.

   // Nested enumeration gmtl.Quat<float>.Params.
   public enum Params
   {
      Size = 4
   };


} // class gmtl.Quatf

/// <summary>
/// Custom marshaler for gmtl.Quatf.  Use this with P/Invoke
/// calls when a C# object of this type needs to be passed to native code or
/// vice versa.  Essentially, this marshaler hides the existence of mRawObject.
/// </summary>
public class QuatfMarshaler : ICustomMarshaler
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
      return ((gmtl.Quatf) obj).RawObject;
   }

   // Marshaling for native memory coming from C++.
   public Object MarshalNativeToManaged(IntPtr nativeObj)
   {
      return new gmtl.Quatf(nativeObj, false);
   }

   public static ICustomMarshaler GetInstance(string cookie)
   {
      return mInstance;
   }

   private static QuatfMarshaler mInstance = new QuatfMarshaler();
}


} // namespace gmtl
