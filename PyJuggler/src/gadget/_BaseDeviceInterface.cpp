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
#include <gadget/Type/DeviceInterface.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct gadget_BaseDeviceInterface_Wrapper: gadget::BaseDeviceInterface
{
    gadget_BaseDeviceInterface_Wrapper(PyObject* self_, const gadget::BaseDeviceInterface& p0):
        gadget::BaseDeviceInterface(p0), self(self_) {}

    gadget_BaseDeviceInterface_Wrapper(PyObject* self_):
        gadget::BaseDeviceInterface(), self(self_) {}

    void refresh() {
        try
        {
            call_method< void >(self, "refresh");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_refresh() {
        gadget::BaseDeviceInterface::refresh();
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_BaseDeviceInterface()
{
    class_< gadget::BaseDeviceInterface, pyj::gadget_BaseDeviceInterface_Wrapper >("BaseDeviceInterface", init<  >())
        .def(init< const gadget::BaseDeviceInterface& >())
        .def("refresh", &gadget::BaseDeviceInterface::refresh, &pyj::gadget_BaseDeviceInterface_Wrapper::default_refresh)
        .def("init", &gadget::BaseDeviceInterface::init)
        .def("getProxyName", &gadget::BaseDeviceInterface::getProxyName)
        .def("isConnected", &gadget::BaseDeviceInterface::isConnected)
        .def("refreshAllDevices", &gadget::BaseDeviceInterface::refreshAllDevices)
        .staticmethod("refreshAllDevices")
    ;

}
