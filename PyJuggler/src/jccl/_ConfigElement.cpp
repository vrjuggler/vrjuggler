// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Includes ====================================================================
#include <boost/python.hpp>
#include <jccl/Config/ConfigElement.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_ConfigElement()
{
    scope* jccl_ConfigElement_scope = new scope(
    class_< jccl::ConfigElement >("ConfigElement", init<  >())
        .def(init< jccl::ConfigDefinitionPtr >())
        .def(init< const jccl::ConfigElement& >())
        .def("isValid", &jccl::ConfigElement::isValid)
        .def("assertValid", &jccl::ConfigElement::assertValid)
        .def("initFromNode", &jccl::ConfigElement::initFromNode)
        .def("getChildElement", &jccl::ConfigElement::getChildElement)
        .def("getNum", &jccl::ConfigElement::getNum)
        .def("getName", &jccl::ConfigElement::getName)
        .def("getFullName", &jccl::ConfigElement::getFullName)
        .def("getVersion", &jccl::ConfigElement::getVersion)
        .def("getID", &jccl::ConfigElement::getID)
        .def("setProperty", (bool (jccl::ConfigElement::*)(const std::string&, const int, bool) )&jccl::ConfigElement::setProperty)
        .def("setProperty", (bool (jccl::ConfigElement::*)(const std::string&, int, jccl::ConfigElementPtr) )&jccl::ConfigElement::setProperty)
        .def("getElementPtrDependencies", &jccl::ConfigElement::getElementPtrDependencies)
        .def("getChildElements", &jccl::ConfigElement::getChildElements)
        .def("getNode", &jccl::ConfigElement::getNode)
        .def(self_ns::str(self))
        .def( self == self )
        .def( self != self )
        .def( self < self )
    );
    register_ptr_to_python< boost::shared_ptr< jccl::ConfigElement > >();
    delete jccl_ConfigElement_scope;
}
