// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

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
