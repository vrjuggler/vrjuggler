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

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <gadget/Type/PositionData.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_PositionData()
{
    class_< gadget::PositionData >("PositionData", init<  >())
        .def(init< const gadget::PositionData& >())
        .def_readwrite("mPosData", &gadget::PositionData::mPosData)
        .def("getPosition", &gadget::PositionData::getPosition)
        .def("setPosition", &gadget::PositionData::setPosition)
        .def("setTime", (void (gadget::InputData::*)() )&gadget::InputData::setTime)
        .def("setTime", (void (gadget::InputData::*)(const vpr::Interval&) )&gadget::InputData::setTime)
        .def("getTime", &gadget::InputData::getTime)
    ;

}
