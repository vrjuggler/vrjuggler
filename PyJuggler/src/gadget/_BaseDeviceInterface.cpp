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
