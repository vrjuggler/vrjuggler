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

// Declarations ================================================================


namespace  {


struct vpr_GUID_Wrapper: vpr::GUID
{
    vpr_GUID_Wrapper(PyObject* self_, const vpr::GUID::GenerateTag p0):
        vpr::GUID(p0), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_):
        vpr::GUID(), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_, const vpr::GUID::StdGUID & p0):
        vpr::GUID(p0), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_, const char * p0):
        vpr::GUID(p0), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_, const std::basic_string<char,std::char_traits<char>,std::allocator<char> > & p0):
        vpr::GUID(p0), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_, const vpr::GUID & p0, const std::basic_string<char,std::char_traits<char>,std::allocator<char> > & p1):
        vpr::GUID(p0, p1), self(self_) {}

    vpr_GUID_Wrapper(PyObject* self_, const vpr::GUID & p0):
        vpr::GUID(p0), self(self_) {}

    vpr::ReturnStatus writeObject(vpr::ObjectWriter * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return vpr::GUID::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return vpr::GUID::readObject(p0);
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
void _Export_GUID()
{
    scope* vpr_GUID_scope = new scope(
    class_< vpr::GUID, vpr_GUID_Wrapper >("GUID", init<  >())
        .def(init< const vpr::GUID::GenerateTag & >())
        .def(init< const vpr::GUID::StdGUID & >())
        .def(init< const char * >())
        .def(init< const std::basic_string<char,std::char_traits<char>,std::allocator<char> > & >())
        .def(init< const vpr::GUID &, const std::basic_string<char,std::char_traits<char>,std::allocator<char> > & >())
        .def(init< const vpr::GUID & >())
        .def("writeObject", &vpr::GUID::writeObject, &vpr_GUID_Wrapper::default_writeObject)
        .def("readObject", &vpr::GUID::readObject, &vpr_GUID_Wrapper::default_readObject)
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
