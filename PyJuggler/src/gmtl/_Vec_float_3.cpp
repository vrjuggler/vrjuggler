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
#include <gmtl/Vec.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Vec_float_3()
{
    scope* gmtl_Vec_float_3_scope = new scope(
    class_< gmtl::Vec<float,3>, bases< gmtl::VecBase<float,3> > >("Vec3f", init<  >())
        .def(init< const gmtl::Vec<float,3> & >())
        .def(init< const gmtl::VecBase<float,3> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
    );

    enum_< gmtl::Vec<float,3>::Params >("Params")
        .value("Size", gmtl::Vec<float,3>::Size)
    ;

    delete gmtl_Vec_float_3_scope;

}
