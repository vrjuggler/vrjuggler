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
#include <vpr/Util/Interval.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Interval()
{
    scope* vpr_Interval_scope = new scope(
    class_< vpr::Interval >("Interval", init<  >())
        .def(init< const vpr::Interval& >())
        .def(init< const vpr::Uint64, const vpr::Interval::Unit >())
        .def_readonly("NoWait", &vpr::Interval::NoWait)
        .def_readonly("NoTimeout", &vpr::Interval::NoTimeout)
        .def_readonly("HalfPeriod", &vpr::Interval::HalfPeriod)
        .def("set", &vpr::Interval::set)
        .def("setf", &vpr::Interval::setf)
        .def("setd", &vpr::Interval::setd)
        .def("setNow", &vpr::Interval::setNow)
        .def("setNowReal", &vpr::Interval::setNowReal)
        .def("sec", (void (vpr::Interval::*)(const vpr::Uint64) )&vpr::Interval::sec)
        .def("sec", (vpr::Uint64 (vpr::Interval::*)() const)&vpr::Interval::sec)
        .def("secf", (void (vpr::Interval::*)(const float) )&vpr::Interval::secf)
        .def("secf", (float (vpr::Interval::*)() const)&vpr::Interval::secf)
        .def("secd", (void (vpr::Interval::*)(const double) )&vpr::Interval::secd)
        .def("secd", (double (vpr::Interval::*)() const)&vpr::Interval::secd)
        .def("msec", (void (vpr::Interval::*)(const vpr::Uint64) )&vpr::Interval::msec)
        .def("msec", (vpr::Uint64 (vpr::Interval::*)() const)&vpr::Interval::msec)
        .def("msecf", (void (vpr::Interval::*)(const float) )&vpr::Interval::msecf)
        .def("msecf", (float (vpr::Interval::*)() const)&vpr::Interval::msecf)
        .def("msecd", (void (vpr::Interval::*)(const double) )&vpr::Interval::msecd)
        .def("msecd", (double (vpr::Interval::*)() const)&vpr::Interval::msecd)
        .def("usec", (void (vpr::Interval::*)(const vpr::Uint64) )&vpr::Interval::usec)
        .def("usec", (vpr::Uint64 (vpr::Interval::*)() const)&vpr::Interval::usec)
        .def("usecf", (void (vpr::Interval::*)(const float) )&vpr::Interval::usecf)
        .def("usecf", (float (vpr::Interval::*)() const)&vpr::Interval::usecf)
        .def("usecd", (void (vpr::Interval::*)(const double) )&vpr::Interval::usecd)
        .def("usecd", (double (vpr::Interval::*)() const)&vpr::Interval::usecd)
        .def("getBaseVal", &vpr::Interval::getBaseVal)
        .def("now", &vpr::Interval::now)
        .staticmethod("now")
        .def( self == self )
        .def( self != self )
        .def( self < self )
        .def( self <= self )
        .def( self > self )
        .def( self += self )
        .def( self + self )
        .def( self -= self )
        .def( self - self )
    );

    enum_< vpr::Interval::Unit >("Unit")
        .value("Msec", vpr::Interval::Msec)
        .value("Base", vpr::Interval::Base)
        .value("Sec", vpr::Interval::Sec)
        .value("Usec", vpr::Interval::Usec)
    ;

    delete vpr_Interval_scope;

}
