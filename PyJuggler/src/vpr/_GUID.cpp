/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
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
#include <vpr/Util/GUID.h>

// Using =======================================================================
using namespace boost::python;


// Module ======================================================================
void _Export_GUID()
{
    scope* vpr_GUID_scope = new scope(
    class_< vpr::GUID >("GUID", init<  >())
        .def(init< const vpr::GUID::GenerateTag & >())
        .def(init< const char * >())
        .def(init< const std::basic_string<char,std::char_traits<char>,std::allocator<char> > & >())
        .def(init< const vpr::GUID &, const std::basic_string<char,std::char_traits<char>,std::allocator<char> > & >())
        .def(init< const vpr::GUID & >())
        .def("writeObject", &vpr::GUID::writeObject)
        .def("readObject", &vpr::GUID::readObject)
        .def("toString", &vpr::GUID::toString)
        .def("generate", (void (vpr::GUID::*)() )&vpr::GUID::generate)
        .def("generate", (void (vpr::GUID::*)(const vpr::GUID &, const std::basic_string<char,std::char_traits<char>,std::allocator<char> > &) )&vpr::GUID::generate)
        .def(self_ns::str(self))
        .def( self == self )
        .def( self != self )
        .def( self < self )
    );

    class_< vpr::GUID::GenerateTag >("GenerateTag", init<  >())
        .def(init< const vpr::GUID::GenerateTag & >())
    ;

    vpr_GUID_scope->attr("generateTag") = boost::ref(vpr::GUID::generateTag);
    vpr_GUID_scope->attr("NullGUID") = vpr::GUID::NullGUID;

    class_< vpr::GUID::hash >("hash", init<  >())
        .def(init< const vpr::GUID::hash & >())
        .def("__call__", &vpr::GUID::hash::operator ())
    ;

    delete vpr_GUID_scope;

}
