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
