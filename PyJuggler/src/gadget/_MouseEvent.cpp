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
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <gadget/Type/EventWindow/MouseEvent.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace pyj {


struct gadget_MouseEvent_Wrapper: gadget::MouseEvent
{
    gadget_MouseEvent_Wrapper(PyObject* self_, const gadget::MouseEvent & p0):
        gadget::MouseEvent(p0), self(self_) {}

    gadget_MouseEvent_Wrapper(PyObject* self_, const gadget::EventType & p0, const gadget::Keys & p1, const int & p2, const int & p3, const int & p4, const int & p5, const int & p6, const long unsigned int & p7):
        gadget::MouseEvent(p0, p1, p2, p3, p4, p5, p6, p7), self(self_) {}

    gadget_MouseEvent_Wrapper(PyObject* self_):
        gadget::MouseEvent(), self(self_) {}

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
        return gadget::MouseEvent::writeObject(p0);
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
        return gadget::MouseEvent::readObject(p0);
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
void _Export_MouseEvent()
{
    scope* gadget_MouseEvent_scope = new scope(
    class_< gadget::MouseEvent, bases< gadget::Event >, pyj::gadget_MouseEvent_Wrapper >("MouseEvent", init<  >())
        .def(init< const gadget::MouseEvent & >())
        .def(init< const gadget::EventType &, const gadget::Keys &, const int &, const int &, const int &, const int &, const int &, const long unsigned int & >())
        .def("writeObject", &gadget::MouseEvent::writeObject, &pyj::gadget_MouseEvent_Wrapper::default_writeObject)
        .def("readObject", &gadget::MouseEvent::readObject, &pyj::gadget_MouseEvent_Wrapper::default_readObject)
        .def("getButton", &gadget::MouseEvent::getButton, return_value_policy< copy_const_reference >())
        .def("getX", &gadget::MouseEvent::getX, return_value_policy< copy_const_reference >())
        .def("getY", &gadget::MouseEvent::getY, return_value_policy< copy_const_reference >())
        .def("getGlobalX", &gadget::MouseEvent::getGlobalX, return_value_policy< copy_const_reference >())
        .def("getGlobalY", &gadget::MouseEvent::getGlobalY, return_value_policy< copy_const_reference >())
        .def("getState", &gadget::MouseEvent::getState, return_value_policy< copy_const_reference >())
        .def("type", &gadget::Event::type, return_value_policy< copy_const_reference >())
        .def("setType", &gadget::Event::setType)
        .def("time", &gadget::Event::time, return_value_policy< copy_const_reference >())
    );
    // Temporary code for smart pointers
    objects::class_value_wrapper< 
      boost::shared_ptr< gadget::MouseEvent >, objects::make_ptr_instance< 
        gadget::MouseEvent, objects::pointer_holder< 
          boost::shared_ptr< gadget::MouseEvent >, gadget::MouseEvent >
      >
    >();
    delete gadget_MouseEvent_scope;

}
