// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

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
