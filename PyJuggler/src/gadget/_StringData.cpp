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
#include <gadget/Type/StringData.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_StringData()
{
    class_< gadget::StringData >("StringData",
         "gadget.InputData subclass for string data.",
         init<  >()
        )
        .def(init< const gadget::StringData& >())
        .def("getString", &gadget::StringData::getString,
             return_value_policy<copy_const_reference>())
        .def("setString", &gadget::StringData::setString)
        .def("setTime", (void (gadget::InputData::*)() )&gadget::InputData::setTime)
        .def("setTime", (void (gadget::InputData::*)(const vpr::Interval&) )&gadget::InputData::setTime)
        .def("getTime", &gadget::InputData::getTime)
    ;

}
