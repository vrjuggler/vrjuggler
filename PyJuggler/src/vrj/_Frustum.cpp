/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
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
#include <vrj/Display/Frustum.h>
#include <vrj-helpers.h>

// Using =======================================================================
using namespace boost::python;

/*
namespace vrjHelpers
{
    template void setArrayElement(vrj::Frustum*, const unsigned, float);
}
*/

// Module ======================================================================
void _Export_Frustum()
{
    scope* vrj_Frustum_scope = new scope(
    class_< vrj::Frustum >("Frustum", init<  >())
        .def(init< const vrj::Frustum & >())
        .def("setBottomLeftTopRight", &vrj::Frustum::setBottomLeftTopRight)
        .def("setNearFar", &vrj::Frustum::setNearFar)
        .def("set", &vrj::Frustum::set)
        .def("__getitem__", (float& (vrj::Frustum::*)(int)) &vrj::Frustum::operator[], return_value_policy<copy_non_const_reference>())
//        .def(self_ns::str(self))
    );

    enum_< vrj::Frustum::entry >("entry")
        .value("VJ_TOP", vrj::Frustum::VJ_TOP)
        .value("VJ_BOTTOM", vrj::Frustum::VJ_BOTTOM)
        .value("VJ_LEFT", vrj::Frustum::VJ_LEFT)
        .value("VJ_FAR", vrj::Frustum::VJ_FAR)
        .value("VJ_NEAR", vrj::Frustum::VJ_NEAR)
        .value("VJ_RIGHT", vrj::Frustum::VJ_RIGHT)
    ;

    delete vrj_Frustum_scope;

}
