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

// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl-intersect-wrappers.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_gmtl_intersect_wrappers_h()
{
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::AABox<float>&, const gmtl::AABox<float>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::AABox<double>&, const gmtl::AABox<double>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::AABox<float>&, const gmtl::Point<float,3>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::AABox<double>&, const gmtl::Point<double,3>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::AABox<float>&, const gmtl::Vec<float,3>&, const gmtl::AABox<float>&, const gmtl::Vec<float,3>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::AABox<double>&, const gmtl::Vec<double,3>&, const gmtl::AABox<double>&, const gmtl::Vec<double,3>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<float>&, const gmtl::Vec<float,3>&, const gmtl::Sphere<float>&, const gmtl::Vec<float,3>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<double>&, const gmtl::Vec<double,3>&, const gmtl::Sphere<double>&, const gmtl::Vec<double,3>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::AABox<float>&, const gmtl::Sphere<float>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::AABox<double>&, const gmtl::Sphere<double>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<float>&, const gmtl::AABox<float>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<double>&, const gmtl::AABox<double>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<float>&, const gmtl::Point<float,3>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<double>&, const gmtl::Point<double,3>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<float>&, const gmtl::Ray<float>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<double>&, const gmtl::Ray<double>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<float>&, const gmtl::LineSeg<float>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Sphere<double>&, const gmtl::LineSeg<double>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Plane<float>&, const gmtl::Ray<float>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Plane<double>&, const gmtl::Ray<double>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Tri<float>&, const gmtl::Ray<float>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Tri<double>&, const gmtl::Ray<double>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Tri<float>&, const gmtl::LineSeg<float>&)) &gmtlWrappers::intersect);
   def("intersect", (tuple (gmtlWrappers::*)(const gmtl::Tri<double>&, const gmtl::LineSeg<double>&)) &gmtlWrappers::intersect);
}
