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

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <gadgeteer-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadgetWrapper_PositionInterface_getData_overloads_0_1, getData, 0, 1)

}// namespace 


// Module ======================================================================
void _Export_PositionInterface()
{
    class_< gadgetWrapper::PositionInterface >("PositionInterface", init<  >())
        .def(init< const gadgetWrapper::PositionInterface& >())
        .def("init", &gadgetWrapper::PositionInterface::init)
        .def("refresh", &gadgetWrapper::PositionInterface::refresh)
        .def("getProxyName", &gadgetWrapper::PositionInterface::getProxyName)
        .def("isConnected", &gadgetWrapper::PositionInterface::isConnected)
        .def("getProxy", &gadgetWrapper::PositionInterface::getProxy, return_internal_reference< 1 >())
        .def("getTimeStamp", &gadgetWrapper::PositionInterface::getTimeStamp)
        .def("getData", &gadgetWrapper::PositionInterface::getData, pyj::gadgetWrapper_PositionInterface_getData_overloads_0_1())
    ;

}
