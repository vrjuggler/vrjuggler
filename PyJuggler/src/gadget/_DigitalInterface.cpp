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
#include <gadgeteer-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_DigitalInterface()
{
    class_< gadgetWrapper::DigitalInterface >("DigitalInterface", init<  >())
        .def(init< const gadgetWrapper::DigitalInterface & >())
        .def("init", &gadgetWrapper::DigitalInterface::init)
        .def("refresh", &gadgetWrapper::DigitalInterface::refresh)
        .def("getProxyName", &gadgetWrapper::DigitalInterface::getProxyName)
        .def("isConnected", &gadgetWrapper::DigitalInterface::isConnected)
        .def("getProxy", &gadgetWrapper::DigitalInterface::getProxy, return_internal_reference< 1 >())
        .def("getData", &gadgetWrapper::DigitalInterface::getData)
    ;

}
