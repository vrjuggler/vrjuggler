/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
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

#ifndef _PYJUGGLER_GMTL_INTERSECT_WRAPPERS_H_
#define _PYJUGGLER_GMTL_INTERSECT_WRAPPERS_H_

// This file makes declarations of the templated function instantiations
// needed for Boost.Python to do its thing.  More specifically, the functions
// defined here wrap instantiations of gmtl::intersect() and change the return
// type to be a Python tuple.

#include <boost/python/tuple.hpp>

#include <gmtl/Intersection.h>


namespace gmtlWrappers
{
   using namespace boost::python;

   template<typename T>
   tuple intersect(const gmtl::AABox<T>& b1, const gmtl::AABox<T>& b2)
   {
      return make_tuple(gmtl::intersect(b1, b2));
   }

   template tuple intersect(const gmtl::AABoxf&, const gmtl::AABoxf&);
   template tuple intersect(const gmtl::AABoxd&, const gmtl::AABoxd&);

   template<typename T>
   tuple intersect(const gmtl::AABox<T>& b, const gmtl::Point<T, 3>& p)
   {
      return make_tuple(gmtl::intersect(b, p));
   }

   template tuple intersect(const gmtl::AABoxf&, const gmtl::Point3f&);
   template tuple intersect(const gmtl::AABoxd&, const gmtl::Point3d&);

   template<typename T>
   tuple intersect(const gmtl::AABox<T>& b1, const gmtl::Vec<T, 3>& v1,
                   const gmtl::AABox<T>& b2, const gmtl::Vec<T, 3>& v2)
   {
      T val1, val2;
      bool result = gmtl::intersect(b1, v1, b2, v2, val1, val2);
      return make_tuple(result, val1, val2);
   }

   template tuple intersect(const gmtl::AABoxf&, const gmtl::Vec3f&,
                            const gmtl::AABoxf&, const gmtl::Vec3f&);
   template tuple intersect(const gmtl::AABoxd&, const gmtl::Vec3d&,
                            const gmtl::AABoxd&, const gmtl::Vec3d&);

   template<typename T>
   tuple intersect(const gmtl::Sphere<T>& s1, const gmtl::Vec<T, 3>& v1,
                   const gmtl::Sphere<T>& s2, const gmtl::Vec<T, 3>& v2)
   {
      T val1, val2;
      bool result = gmtl::intersect(s1, v1, s2, v2, val1, val2);
      return make_tuple(result, val1, val2);
   }

   template tuple intersect(const gmtl::Spheref&, const gmtl::Vec3f&,
                            const gmtl::Spheref&, const gmtl::Vec3f&);
   template tuple intersect(const gmtl::Sphered&, const gmtl::Vec3d&,
                            const gmtl::Sphered&, const gmtl::Vec3d&);

   template<typename T>
   tuple intersect(const gmtl::AABox<T>& b, const gmtl::Sphere<T>& s)
   {
      return make_tuple(gmtl::intersect(b, s));
   }

   template tuple intersect(const gmtl::AABoxf&, const gmtl::Spheref&);
   template tuple intersect(const gmtl::AABoxd&, const gmtl::Sphered&);

   template<typename T>
   tuple intersect(const gmtl::Sphere<T>& s, const gmtl::AABox<T>& b)
   {
      return make_tuple(gmtl::intersect(s, b));
   }

   template tuple intersect(const gmtl::Spheref&, const gmtl::AABoxf&);
   template tuple intersect(const gmtl::Sphered&, const gmtl::AABoxd&);

   template<typename T>
   tuple intersect(const gmtl::Sphere<T>& s, const gmtl::Point<T, 3>& p)
   {
      return make_tuple(gmtl::intersect(s, p));
   }

   template tuple intersect(const gmtl::Spheref&, const gmtl::Point3f&);
   template tuple intersect(const gmtl::Sphered&, const gmtl::Point3d&);

   template<typename T>
   tuple intersect(const gmtl::Sphere<T>& s, const gmtl::Ray<T>& r)
   {
      int c;
      float val1, val2;
      bool result = gmtl::intersect(s, r, c, val1, val2);
      return make_tuple(result, c, val1, val2);
   }

   template tuple intersect(const gmtl::Spheref&, const gmtl::Rayf&);
   template tuple intersect(const gmtl::Sphered&, const gmtl::Rayd&);

   template<typename T>
   tuple intersect(const gmtl::Sphere<T>& s, const gmtl::LineSeg<T>& l)
   {
      int c;
      float val1, val2;
      bool result = gmtl::intersect(s, l, c, val1, val2);
      return make_tuple(result, c, val1, val2);
   }

   template tuple intersect(const gmtl::Spheref&, const gmtl::LineSegf&);
   template tuple intersect(const gmtl::Sphered&, const gmtl::LineSegd&);

   template<typename T>
   tuple intersect(const gmtl::Plane<T>& p, const gmtl::Ray<T>& r)
   {
      T val;
      bool result = gmtl::intersect(p, r, val);
      return make_tuple(result, val);
   }

   template tuple intersect(const gmtl::Planef&, const gmtl::Rayf&);
   template tuple intersect(const gmtl::Planed&, const gmtl::Rayd&);

   template<typename T>
   tuple intersect(const gmtl::Tri<T>& t, const gmtl::Ray<T>& r)
   {
      float val1, val2, val3;
      bool result = gmtl::intersect(t, r, val1, val2, val3);
      return make_tuple(result, val1, val2, val3);
   }

   template tuple intersect(const gmtl::Trif&, const gmtl::Rayf&);
   template tuple intersect(const gmtl::Trid&, const gmtl::Rayd&);

   template<typename T>
   tuple intersect(const gmtl::Tri<T>& t, const gmtl::LineSeg<T>& l)
   {
      float val1, val2, val3;
      bool result = gmtl::intersect(t, l, val1, val2, val3);
      return make_tuple(result, val1, val2, val3);
   }

   template tuple intersect(const gmtl::Trif&, const gmtl::LineSegf&);
   template tuple intersect(const gmtl::Trid&, const gmtl::LineSegd&);
}


#endif /* _PYJUGGLER_GMTL_INTERSECT_WRAPPERS_H_ */
