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
void _Export_DigitalInterface()
{
    class_< gadgetWrapper::DigitalInterface >("DigitalInterface", init<  >())
        .def(init< const gadgetWrapper::DigitalInterface& >())
        .def("init", &gadgetWrapper::DigitalInterface::init)
        .def("refresh", &gadgetWrapper::DigitalInterface::refresh)
        .def("getProxyName", &gadgetWrapper::DigitalInterface::getProxyName)
        .def("isConnected", &gadgetWrapper::DigitalInterface::isConnected)
        .def("getProxy", &gadgetWrapper::DigitalInterface::getProxy, return_internal_reference< 1 >())
        .def("getData", &gadgetWrapper::DigitalInterface::getData)
    ;

}
