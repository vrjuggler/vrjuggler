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
#include <vpr/Util/ReturnStatus.h>
#include <vpr/Util/Interval.h>

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
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return vpr::GUID::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        return call_method< vpr::ReturnStatus >(self, "readObject", p0);
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return vpr::GUID::readObject(p0);
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(vpr)
{
    scope* vpr_Interval_scope = new scope(
    class_< vpr::Interval >("Interval", init<  >())
        .def(init< const vpr::Interval & >())
        .def(init< const long long unsigned int, const vpr::Interval::Unit >())
        .def("set", &vpr::Interval::set)
        .def("setf", &vpr::Interval::setf)
        .def("setd", &vpr::Interval::setd)
        .def("setNow", &vpr::Interval::setNow)
        .def("setNowReal", &vpr::Interval::setNowReal)
        .def("sec", (void (vpr::Interval::*)(const long long unsigned int) )&vpr::Interval::sec)
        .def("sec", (long long unsigned int (vpr::Interval::*)() const)&vpr::Interval::sec)
        .def("secf", (void (vpr::Interval::*)(const float) )&vpr::Interval::secf)
        .def("secf", (float (vpr::Interval::*)() const)&vpr::Interval::secf)
        .def("secd", (void (vpr::Interval::*)(const double) )&vpr::Interval::secd)
        .def("secd", (double (vpr::Interval::*)() const)&vpr::Interval::secd)
        .def("msec", (void (vpr::Interval::*)(const long long unsigned int) )&vpr::Interval::msec)
        .def("msec", (long long unsigned int (vpr::Interval::*)() const)&vpr::Interval::msec)
        .def("msecf", (void (vpr::Interval::*)(const float) )&vpr::Interval::msecf)
        .def("msecf", (float (vpr::Interval::*)() const)&vpr::Interval::msecf)
        .def("msecd", (void (vpr::Interval::*)(const double) )&vpr::Interval::msecd)
        .def("msecd", (double (vpr::Interval::*)() const)&vpr::Interval::msecd)
        .def("usec", (void (vpr::Interval::*)(const long long unsigned int) )&vpr::Interval::usec)
        .def("usec", (long long unsigned int (vpr::Interval::*)() const)&vpr::Interval::usec)
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
        .def( self - self )
    );
    vpr_Interval_scope->attr("NoWait") = vpr::Interval::NoWait;
    vpr_Interval_scope->attr("NoTimeout") = vpr::Interval::NoTimeout;
    vpr_Interval_scope->attr("HalfPeriod") = vpr::Interval::HalfPeriod;

    enum_< vpr::Interval::Unit >("Unit")
        .value("Msec", vpr::Interval::Msec)
        .value("Base", vpr::Interval::Base)
        .value("Sec", vpr::Interval::Sec)
        .value("Usec", vpr::Interval::Usec)
    ;

    delete vpr_Interval_scope;

    scope* vpr_ReturnStatus_scope = new scope(
    class_< vpr::ReturnStatus >("ReturnStatus", init<  >())
        .def(init< const vpr::ReturnStatus & >())
        .def(init< const vpr::ReturnStatus::Code & >())
        .def("copy", &vpr::ReturnStatus::copy)
        .def("setCode", &vpr::ReturnStatus::setCode)
        .def("success", &vpr::ReturnStatus::success)
        .def("failure", &vpr::ReturnStatus::failure)
        .def("wouldBlock", &vpr::ReturnStatus::wouldBlock)
        .def("inProgress", &vpr::ReturnStatus::inProgress)
        .def("timeout", &vpr::ReturnStatus::timeout)
        .def( self == self )
        .def( self == other< vpr::ReturnStatus::Code >() )
        .def( self != self )
        .def( self != other< vpr::ReturnStatus::Code >() )
    );

    enum_< vpr::ReturnStatus::Code >("Code")
        .value("WouldBlock", vpr::ReturnStatus::WouldBlock)
        .value("NotConnected", vpr::ReturnStatus::NotConnected)
        .value("Succeed", vpr::ReturnStatus::Succeed)
        .value("Timeout", vpr::ReturnStatus::Timeout)
        .value("Fail", vpr::ReturnStatus::Fail)
        .value("InProgress", vpr::ReturnStatus::InProgress)
    ;

    delete vpr_ReturnStatus_scope;

    scope* vpr_GUID_scope = new scope(
    class_< vpr::GUID, vpr_GUID_Wrapper >("GUID", init<  >())
        .def(init< const vpr::GUID::GenerateTag >())
        .def(init< const vpr::GUID::StdGUID & >())
        .def(init< const char * >())
        .def(init< const std::basic_string<char,std::char_traits<char>,std::allocator<char> > & >())
        .def(init< const vpr::GUID &, const std::basic_string<char,std::char_traits<char>,std::allocator<char> > & >())
        .def(init< const vpr::GUID & >())
        .def("toString", &vpr::GUID::toString)
        .def("generate", (void (vpr::GUID::*)() )&vpr::GUID::generate)
        .def("generate", (void (vpr::GUID::*)(const vpr::GUID &, const std::basic_string<char,std::char_traits<char>,std::allocator<char> > &) )&vpr::GUID::generate)
        .def("writeObject", &vpr::GUID::writeObject, &vpr_GUID_Wrapper::default_writeObject)
        .def("readObject", &vpr::GUID::readObject, &vpr_GUID_Wrapper::default_readObject)
        .def(self_ns::str(self))
        .def( self == self )
        .def( self != self )
        .def( self < self )
    );
    vpr_GUID_scope->attr("generateTag") = vpr::GUID::generateTag;
    vpr_GUID_scope->attr("NullGUID") = vpr::GUID::NullGUID;

    class_< vpr::GUID::GenerateTag >("GenerateTag", init<  >())
        .def(init< const vpr::GUID::GenerateTag & >())
    ;


    class_< vpr::GUID::hash >("hash", init<  >())
        .def(init< const vpr::GUID::hash & >())
        .def("__call__", &vpr::GUID::hash::operator ())
    ;

    delete vpr_GUID_scope;

}
