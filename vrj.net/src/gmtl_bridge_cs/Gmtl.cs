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

// Generated from Revision: 1.1 of RCSfile: free_function_cs.tmpl,v
using System;
using System.Runtime.InteropServices;

namespace gmtl
{

public sealed abstract class Gmtl
{
   [DllImport("gmtl", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invert__gmtl_Matrix44f_gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1);

   public static gmtl.Matrix44f invert(gmtl.Matrix44f p0, gmtl.Matrix44f p1)
   {
      
      
      gmtl.Matrix44f result;
      result = gmtl_invert__gmtl_Matrix44f_gmtl_Matrix44f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invert__gmtl_Matrix33f_gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1);

   public static gmtl.Matrix33f invert(gmtl.Matrix33f p0, gmtl.Matrix33f p1)
   {
      
      
      gmtl.Matrix33f result;
      result = gmtl_invert__gmtl_Matrix33f_gmtl_Matrix33f(p0, p1);
      
      
      return result;
   }

   [DllImport("gmtl", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))]
   private extern static gmtl.Matrix44f gmtl_invert__gmtl_Matrix44f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0);

   public static gmtl.Matrix44f invert(gmtl.Matrix44f p0)
   {
      
      gmtl.Matrix44f result;
      result = gmtl_invert__gmtl_Matrix44f(p0);
      
      return result;
   }

   [DllImport("gmtl", CharSet = CharSet.Ansi)]
   [return : MarshalAs(UnmanagedType.CustomMarshaler,
                       MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))]
   private extern static gmtl.Matrix33f gmtl_invert__gmtl_Matrix33f([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0);

   public static gmtl.Matrix33f invert(gmtl.Matrix33f p0)
   {
      
      gmtl.Matrix33f result;
      result = gmtl_invert__gmtl_Matrix33f(p0);
      
      return result;
   }

   [DllImport("gmtl", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix44fMarshaler))] gmtl.Matrix44f p1,
	ref float p2);

   public static bool isEqual(gmtl.Matrix44f p0, gmtl.Matrix44f p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Matrix44f_gmtl_Matrix44f_float(p0, p1, ref p2);
      
      
      
      return result;
   }

   [DllImport("gmtl", CharSet = CharSet.Ansi)]
   private extern static bool gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p0,
	[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(gmtl.Matrix33fMarshaler))] gmtl.Matrix33f p1,
	ref float p2);

   public static bool isEqual(gmtl.Matrix33f p0, gmtl.Matrix33f p1, ref float p2)
   {
      
      
      
      bool result;
      result = gmtl_isEqual__gmtl_Matrix33f_gmtl_Matrix33f_float(p0, p1, ref p2);
      
      
      
      return result;
   }

}


} // namespace gmtl
