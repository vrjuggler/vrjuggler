
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Tri.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Tri_double()
{
    class_< gmtl::Tri<double> >("Trid", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Point<double,3> &, const gmtl::Point<double,3> & >())
        .def(init< const gmtl::Tri<double> & >())
        .def("edge", &gmtl::Tri<double>::edge)
        .def("set", &gmtl::Tri<double>::set)
    ;

}
