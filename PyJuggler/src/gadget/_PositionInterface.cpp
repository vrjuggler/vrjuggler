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
void _Export_PositionInterface()
{
    class_< gadgetWrapper::PositionInterface >("PositionInterface",
         "Wrapper to provide an easier way to access position proxy objects\n"
         "from within user applications.  Users can simply declare a local\n"
         "interface variable and use it as a smart pointer for the proxy."
         ,
         init<  >()
        )
        .def(init< const gadgetWrapper::PositionInterface& >())
        .def("init", &gadgetWrapper::PositionInterface::init,
             "init(proxyName)\n"
             "Initializes this object.\n"
             "Arguments:\n"
             "proxyName -- String name of the position proxy to connect to."
         )
        .def("refresh", &gadgetWrapper::PositionInterface::refresh,
             "refresh()\n"
             "Refreshes the interface based on the current configuration.\n"
             "Post-conditions:\n"
             "(mProxyIndex == -1) ==> Proxy not initialized yet.\n"
             "(mProxyIndex !- -1) ==> mProxyName has name of device and\n"
             "                        local proxy pionter is set to the\n"
             "                        position device."
         )
        .def("getProxyName", &gadgetWrapper::PositionInterface::getProxyName,
             "getProxyName() -> string object\n"
             "Returns the name of the proxy."
         )
        .def("isConnected", &gadgetWrapper::PositionInterface::isConnected,
             "isConnected() -> Boolean\n"
             "Identifies whether this device interface is connected to a\n"
             "proxy."
         )
        .def("getProxy", &gadgetWrapper::PositionInterface::getProxy,
             return_internal_reference< 1 >(),
             "getProxy() -> gadget.PositionProxy object\n"
             "Returns the underlying proxy to which we are connected."
         )
        .def("getTimeStamp", &gadgetWrapper::PositionInterface::getTimeStamp,
             "getTimeStamp() -> vpr.Interval object\n"
             "Returns the time of the last update."
         )
        .def("getData", &gadgetWrapper::PositionInterface::getData,
             (args("scaleFactor") = gadget::PositionUnitConversion::ConvertToFeet),
             "getData(scaleFactor = 3.28) -> gmtl.Matrix44f object\n"
             "Gets the current positional data value from the device as a.\n"
             "matrix.  For example, getData(3.28) will return a matrix in\n"
             "feet.\n"
             "Keyword Arguments:\n"
             "scaleFactor -- Factor to convert from meters to the desired\n"
             "               units.  This paramter is optional.  If not\n"
             "               specified, it defaults to the conversion factor\n"
             "               from meters to feet."
         )
    ;

}
