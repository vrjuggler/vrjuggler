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
#include <gadget/Type/EventWindow/Event.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace pyj {


struct gadget_Event_Wrapper: gadget::Event
{
    gadget_Event_Wrapper(PyObject* self_, const gadget::Event & p0):
        gadget::Event(p0), self(self_) {}

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
        return gadget::Event::writeObject(p0);
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
        return gadget::Event::readObject(p0);
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
void _Export_Event()
{
    scope* gadget_Event_scope = new scope(
    class_< gadget::Event, pyj::gadget_Event_Wrapper >("Event", init< const gadget::Event & >())
        .def("writeObject", &gadget::Event::writeObject, &pyj::gadget_Event_Wrapper::default_writeObject)
        .def("readObject", &gadget::Event::readObject, &pyj::gadget_Event_Wrapper::default_readObject)
        .def("type", &gadget::Event::type, return_value_policy< copy_const_reference >())
        .def("setType", &gadget::Event::setType)
        .def("time", &gadget::Event::time, return_value_policy< copy_const_reference >())
    );
    // Temporary code for smart pointers
    objects::class_value_wrapper< 
      boost::shared_ptr< gadget::Event >, objects::make_ptr_instance< 
        gadget::Event, objects::pointer_holder< 
          boost::shared_ptr< gadget::Event >, gadget::Event >
      >
    >();
    delete gadget_Event_scope;

}
