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

// Module ======================================================================
void _Export_AnalogInterface()
{
    class_< gadgetWrapper::AnalogInterface >("AnalogInterface", init<  >())
        .def(init< const gadgetWrapper::AnalogInterface& >())
        .def("init", &gadgetWrapper::AnalogInterface::init)
        .def("refresh", &gadgetWrapper::AnalogInterface::refresh)
        .def("getProxyName", &gadgetWrapper::AnalogInterface::getProxyName)
        .def("isConnected", &gadgetWrapper::AnalogInterface::isConnected)
        .def("getProxy", &gadgetWrapper::AnalogInterface::getProxy, return_internal_reference< 1 >())
        .def("getData", &gadgetWrapper::AnalogInterface::getData)
    ;

}
