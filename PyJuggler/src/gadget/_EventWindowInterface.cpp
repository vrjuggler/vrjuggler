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
void _Export_EventWindowInterface()
{
    class_< gadgetWrapper::EventWindowInterface >("EventWindowInterface",
         "Wrapper to provide an easier way to access event window proxy\n"
         "objects from within user applications.  Users can simply declare\n"
         "a local interface variable and use it as a smart pointer for the\n"
         "proxy."
         ,
         init<  >()
        )
        .def(init< const gadgetWrapper::EventWindowInterface& >())
        .def("init", &gadgetWrapper::EventWindowInterface::init,
             "init(proxyName)\n"
             "Initializes this object.\n"
             "Arguments:\n"
             "proxyName -- String name of the event window proxy to connect to."
         )
        .def("refresh", &gadgetWrapper::EventWindowInterface::refresh,
             "refresh()\n"
             "Refreshes the interface based on the current configuration.\n"
             "Post-conditions:\n"
             "(mProxyIndex == -1) ==> Proxy not initialized yet.\n"
             "(mProxyIndex !- -1) ==> mProxyName has name of device and\n"
             "                        local proxy pionter is set to the\n"
             "                        position device."
         )
        .def("getProxyName",
             &gadgetWrapper::EventWindowInterface::getProxyName,
             "getProxyName() -> string object\n"
             "Returns the name of the proxy."
         )
        .def("isConnected", &gadgetWrapper::EventWindowInterface::isConnected,
             "isConnected() -> Boolean\n"
             "Identifies whether this device interface is connected to a\n"
             "proxy."
         )
        .def("getProxy", &gadgetWrapper::EventWindowInterface::getProxy,
             return_internal_reference< 1 >(),
             "getProxy() -> gadget.EventWindowProxy object\n"
             "Returns the underlying proxy to which we are connected."
         )
        .def("getTimeStamp",
             &gadgetWrapper::EventWindowInterface::getTimeStamp,
             "getTimeStamp() -> vpr.Interval object\n"
             "Returns the time of the last update."
         )
        .def("modifierOnly",
             &gadgetWrapper::EventWindowInterface::modifierOnly,
             "modifierOnly(modKey) -> Boolean\n"
             "Determines if the given modifier key is the only modifier\n"
             "pressed.\n"
             "Pre-conditions:\n"
             "The given key identifier must be one of gadget.Keys.NONE,\n"
             "gadget.Keys.KEY_ALT, gadget.Keys.KEY_CTRL, or\n"
             "gadget.Keys.KET_SHIFT.\n"
             "Arguments:\n"
             "modKey -- A gadget.Keys value that must be one of\n"
             "          gadget.Keys.NONE, gadget.Keys.KEY_ALT,\n"
             "          gadget.Keys.KEY_CTRL, or gadget.Keys.KEY_SHIFT.\n"
             "Returns:\n"
             "True is returned if the given modifier is the only modifier\n"
             "key pressed."
         )
        .def("keyPressed", &gadgetWrapper::EventWindowInterface::keyPressed,
             "keyPressed(keyId) -> int\n"
             "Returns the number of times the given key was pressesd during\n"
             "the last frame.  This can be used in a conditional expression\n"
             "to determine if the key was pressed at all."
             "Arguments:\n"
             "keyId -- A value from gadget.Keys identifiing the key whose\n"
             "         state will be queried.\n"
             "Returns:\n"
             "The number of times the named key was pressed since the last\n"
             "update."
         )
        .def("getEventQueue",
             &gadgetWrapper::EventWindowInterface::getEventQueue,
             "getEventQueue() -> list of gadget.Event objects\n"
             "Returns a copy of the current queue of events for the proxied\n"
             "window."
         )
    ;


}
