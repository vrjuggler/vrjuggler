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
#include <gadget/Type/EventWindow/KeyEvent.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace pyj {


struct gadget_KeyEvent_Wrapper: gadget::KeyEvent
{
    gadget_KeyEvent_Wrapper(PyObject* self_, const gadget::KeyEvent & p0):
        gadget::KeyEvent(p0), self(self_) {}

    gadget_KeyEvent_Wrapper(PyObject* self_, const gadget::EventType & p0, const gadget::Keys & p1, const int & p2, const long unsigned int & p3):
        gadget::KeyEvent(p0, p1, p2, p3), self(self_) {}

    gadget_KeyEvent_Wrapper(PyObject* self_, const gadget::EventType & p0, const gadget::Keys & p1, const int & p2, const long unsigned int & p3, char p4):
        gadget::KeyEvent(p0, p1, p2, p3, p4), self(self_) {}

    gadget_KeyEvent_Wrapper(PyObject* self_):
        gadget::KeyEvent(), self(self_) {}

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
        return gadget::KeyEvent::writeObject(p0);
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
        return gadget::KeyEvent::readObject(p0);
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
void _Export_KeyEvent()
{
    scope* gadget_KeyEvent_scope = new scope(
    class_< gadget::KeyEvent, bases< gadget::Event >, pyj::gadget_KeyEvent_Wrapper >("KeyEvent", init<  >())
        .def(init< const gadget::KeyEvent & >())
        .def(init< const gadget::EventType &, const gadget::Keys &, const int &, const long unsigned int &, optional< char > >())
        .def("writeObject", &gadget::KeyEvent::writeObject, &pyj::gadget_KeyEvent_Wrapper::default_writeObject)
        .def("readObject", &gadget::KeyEvent::readObject, &pyj::gadget_KeyEvent_Wrapper::default_readObject)
        .def("getKey", &gadget::KeyEvent::getKey, return_value_policy< copy_const_reference >())
        .def("getModifierMask", &gadget::KeyEvent::getModifierMask, return_value_policy< copy_const_reference >())
        .def("getKeyChar", &gadget::KeyEvent::getKeyChar, return_value_policy< copy_const_reference >())
        .def("type", &gadget::Event::type, return_value_policy< copy_const_reference >())
        .def("setType", &gadget::Event::setType)
        .def("time", &gadget::Event::time, return_value_policy< copy_const_reference >())
    );
    // Temporary code for smart pointers
    objects::class_value_wrapper< 
      boost::shared_ptr< gadget::KeyEvent >, objects::make_ptr_instance< 
        gadget::KeyEvent, objects::pointer_holder< 
          boost::shared_ptr< gadget::KeyEvent >, gadget::KeyEvent >
      >
    >();
    delete gadget_KeyEvent_scope;

}
