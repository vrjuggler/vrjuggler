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
#include <gadgeteer-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_CommandInterface()
{
    class_< gadgetWrapper::CommandInterface >("CommandInterface",
         "Wrapper to provide an easier way to access command-oriented\n"
         "proxy objects from within user applications.  Users can simply\n"
         "declare a local interface variable and use it as a smart pointer\n"
         "for the proxy."
         ,
         init<  >()
        )
        .def(init< const gadgetWrapper::CommandInterface& >())
        .def("init", &gadgetWrapper::CommandInterface::init,
             "init(proxyName)\n"
             "Initializes this object.\n"
             "Arguments:\n"
             "proxyName -- String name of the command-oriented proxy to\n"
             "             connect to."
         )
        .def("refresh", &gadgetWrapper::CommandInterface::refresh,
             "refresh()\n"
             "Refreshes the interface based on the current configuration.\n"
             "Post-conditions:\n"
             "(mProxyIndex == -1) ==> Proxy not initialized yet.\n"
             "(mProxyIndex !- -1) ==> mProxyName has name of device and\n"
             "                        local proxy pionter is set to the\n"
             "                        command-oriented device."
         )
        .def("getProxyName",
             &gadgetWrapper::CommandInterface::getProxyName,
             "getProxyName() -> string object\n"
             "Returns the name of the proxy."
         )
        .def("isConnected",
             &gadgetWrapper::CommandInterface::isConnected,
             "isConnected() -> Boolean\n"
             "Identifies whether this device interface is connected to a\n"
             "proxy."
         )
        .def("getProxy", &gadgetWrapper::CommandInterface::getProxy,
             return_internal_reference< 1 >(),
             "getProxy() -> gadget.DigitalProxy object\n"
             "Returns the underlying proxy to which we are connected."
         )
        .def("getTimeStamp",
             &gadgetWrapper::CommandInterface::getTimeStamp,
             "getTimeStamp() -> vpr.Interval object\n"
             "Returns the time of the last update."
         )
        .def("getData", &gadgetWrapper::CommandInterface::getData,
             "getData() -> int\n"
             "Gets the current command data value from the device."
         )
    ;

}
