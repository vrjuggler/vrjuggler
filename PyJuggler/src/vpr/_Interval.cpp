// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <vpr/Util/Interval.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct IntervalPickle : pickle_suite
{
   static tuple getstate(const vpr::Interval& i)
   {
      return make_tuple(i.usec());
   }

   static void setstate(vpr::Interval& i, tuple state)
   {
      vpr::Uint64 usec = extract<vpr::Uint64>(state[0]);
      i.usec(usec);
   }
};
                                                                                
}

// Module ======================================================================
void _Export_Interval()
{
    scope* vpr_Interval_scope = new scope(
    class_< vpr::Interval >("Interval",
         "This class captures an high-resolution interval.\n\n"
         "This interval is based off an unsigned always increasing\n"
         "counter.  This means that the interval is only accurate for about\n"
         "12 hours and that it is able to be useful for a range of roughly\n"
         "6 hours.  (This is because, internally, everything is stored as\n"
         "tens of microseconds.)\n\n"
         "The interval overflows whenever the counter maxes out.\n"
         "The __sub__() routine will take care of the overflow automatically\n"
         "to make it possible to compare to interval values."
         ,
         init<  >(
            "__init__()\n"
            "Default constructor sets the internal timer to 0.\n\n"
            "__init__(num, timeUnit)\n"
            "Constructor that takes an integer number and a unit of time and\n"
            "stores it in the internal timer as the interval.\n\n"
         )
        )
        .def(init< const vpr::Interval& >())
        .def(init< const vpr::Uint64, const vpr::Interval::Unit >())
        .def_readonly("NoWait", &vpr::Interval::NoWait)
        .def_readonly("NoTimeout", &vpr::Interval::NoTimeout)
        .def_readonly("HalfPeriod", &vpr::Interval::HalfPeriod)
        .def("set", &vpr::Interval::set,
             "set(num, timeUnit)\n"
             "Sets this interval using an integer (num) and a unit type\n"
             "(timeUnit).\n"
             "Example: x.set(1, vpr.Interval.Sec)"
         )
        .def("setf", &vpr::Interval::setf,
             "setf(num, timeUnit)\n"
             "Sets this interval using a single-precision floating-point\n"
             "number (num) and a unit type (timeUnit).\n"
             "Example: x.setf(1.0, vpr.Interval.Sec)"
         )
        .def("setd", &vpr::Interval::setd,
             "setd(num, timeUnit)\n"
             "Sets this interval using a double-precision floating-point\n"
             "number (num) and a unit type (timeUnit).\n"
             "Example: x.setd(1.0, vpr.Interval.Sec)"
         )
        .def("setNow", &vpr::Interval::setNow,
             "setNow()\n"
             "Sets this interval to the current time.  This can then be used\n"
             "to compute a time interval by subtracting two intervals from\n"
             "each other."
         )
        .def("setNowReal", &vpr::Interval::setNowReal,
             "setNowReal()\n"
             "Set now that is gauranteed to be the time based on the system\n"
             "clock which can then be used to compute a time interval by\n"
             "subracting two intervals from each other."
         )
        .def("sec",
             (void (vpr::Interval::*)(const vpr::Uint64) )&vpr::Interval::sec,
             "sec(value)\n"
             "Given an amount of time in seconds as an integer, store it\n"
             "as this the value of interval.\n\n"
             "sec() -> int\n"
             "Returns the value of this interval in seconds as an integer.")
        .def("sec", (vpr::Uint64 (vpr::Interval::*)() const)&vpr::Interval::sec)
        .def("secf",
             (void (vpr::Interval::*)(const float) )&vpr::Interval::secf,
             "secf(value)\n"
             "Given an amount of time in seconds as a single-precision\n"
             "floating-point value, store it as the value of this interval.\n\n"
             "secf() -> float\n"
             "Returns the value of this interval in seconds as\n"
             "single-precision floating-point value.")
        .def("secf", (float (vpr::Interval::*)() const)&vpr::Interval::secf)
        .def("secd",
             (void (vpr::Interval::*)(const double) )&vpr::Interval::secd,
             "secd(value)\n"
             "Given an amount of time in seconds as a double-precision\n"
             "floating-point value, store it as the value of this\n"
             "interval.\n\n"
             "secd() -> double\n"
             "Returns the value of this interval in seconds as\n"
             "double-precision floating-point value.")
        .def("secd", (double (vpr::Interval::*)() const)&vpr::Interval::secd)
        .def("msec",
             (void (vpr::Interval::*)(const vpr::Uint64) )&vpr::Interval::msec,
             "msec(value)\n"
             "Given an amount of time in milliseconds as an integer, store\n"
             "it as this the value of interval.\n\n"
             "msec() -> int\n"
             "Returns the value of this interval in milliseconds as an\n"
             "integer.")
        .def("msec", (vpr::Uint64 (vpr::Interval::*)() const)&vpr::Interval::msec)
        .def("msecf",
             (void (vpr::Interval::*)(const float) )&vpr::Interval::msecf,
             "msecf(value)\n"
             "Given an amount of time in seconds as a single-precision\n"
             "floating-point value, store it as the value of this interval.\n\n"
             "msecf() -> float\n"
             "Returns the value of this interval in milliseconds as a\n"
             "single-precision floating-point value.")
        .def("msecf", (float (vpr::Interval::*)() const)&vpr::Interval::msecf)
        .def("msecd",
             (void (vpr::Interval::*)(const double) )&vpr::Interval::msecd,
             "msecd(value)\n"
             "Given an amount of time in seconds as a double-precision\n"
             "floating-point value, store it as the value of this interval.\n\n"
             "msecd() -> float\n"
             "Returns the value of this interval in milliseconds as a\n"
             "double-precision floating-point value.")
        .def("msecd", (double (vpr::Interval::*)() const)&vpr::Interval::msecd)
        .def("usec",
             (void (vpr::Interval::*)(const vpr::Uint64) )&vpr::Interval::usec,
             "usec(value)\n"
             "Given an amount of time in microseconds as an integer, store\n"
             "it as this the value of interval.\n\n"
             "usec() -> int\n"
             "Returns the value of this interval in microseconds as an\n"
             "integer.")
        .def("usec", (vpr::Uint64 (vpr::Interval::*)() const)&vpr::Interval::usec)
        .def("usecf",
             (void (vpr::Interval::*)(const float) )&vpr::Interval::usecf,
             "usecf(value)\n"
             "Given an amount of time in microseconds as a single-precision\n"
             "floating-point value, store it as this the value of interval.\n\n"
             "usecf() -> float\n"
             "Returns the value of this interval in microseconds as a\n"
             "single-precision floating-point value.")
        .def("usecf", (float (vpr::Interval::*)() const)&vpr::Interval::usecf)
        .def("usecd",
             (void (vpr::Interval::*)(const double) )&vpr::Interval::usecd,
             "usecd(value)\n"
             "Given an amount of time in microseconds as a double-precision\n"
             "floating-point value, store it as this the value of interval.\n\n"
             "usecd() -> float\n"
             "Returns the value of this interval in microseconds as a\n"
             "double-precision floating-point value.")
        .def("usecd", (double (vpr::Interval::*)() const)&vpr::Interval::usecd)
        .def("getBaseVal", &vpr::Interval::getBaseVal,
             "getBaseVal() -> int\n"
             "Returns the value of this interval in microseconds as an\n"
             "integer.")
        .def("now", &vpr::Interval::now,
             "PyJuggler.vpr.Interval.now() -> PyJuggler.vpr.Interval\n"
             "Helper function that returns an Interval representing the\n"
             "current time.")
        .def_pickle(pyj::IntervalPickle())
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
