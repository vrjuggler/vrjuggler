
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Tri.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Tri_int()
{
    class_< gmtl::Tri<int> >("Trii", init<  >())
        .def(init< const gmtl::Point<int,3> &, const gmtl::Point<int,3> &, const gmtl::Point<int,3> & >())
        .def(init< const gmtl::Tri<int> & >())
        .def("edge", &gmtl::Tri<int>::edge)
        .def("set", &gmtl::Tri<int>::set)
    ;

}
