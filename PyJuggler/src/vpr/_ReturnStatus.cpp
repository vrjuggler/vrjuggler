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
#include <vpr/Util/ReturnStatus.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_ReturnStatus()
{
    scope* vpr_ReturnStatus_scope = new scope(
    class_< vpr::ReturnStatus >("ReturnStatus", init<  >())
        .def(init< const vpr::ReturnStatus& >())
        .def(init< const vpr::ReturnStatus::Code& >())
        .def("copy", &vpr::ReturnStatus::copy)
        .def("setCode", &vpr::ReturnStatus::setCode)
        .def("success", &vpr::ReturnStatus::success)
        .def("failure", &vpr::ReturnStatus::failure)
        .def("wouldBlock", &vpr::ReturnStatus::wouldBlock)
        .def("inProgress", &vpr::ReturnStatus::inProgress)
        .def("timeout", &vpr::ReturnStatus::timeout)
        .def( self == self )
        .def( self == other< vpr::ReturnStatus::Code >() )
        .def( self != self )
        .def( self != other< vpr::ReturnStatus::Code >() )
    );

    enum_< vpr::ReturnStatus::Code >("Code")
        .value("WouldBlock", vpr::ReturnStatus::WouldBlock)
        .value("NotConnected", vpr::ReturnStatus::NotConnected)
        .value("Succeed", vpr::ReturnStatus::Succeed)
        .value("Timeout", vpr::ReturnStatus::Timeout)
        .value("Fail", vpr::ReturnStatus::Fail)
        .value("InProgress", vpr::ReturnStatus::InProgress)
    ;

    delete vpr_ReturnStatus_scope;

}
