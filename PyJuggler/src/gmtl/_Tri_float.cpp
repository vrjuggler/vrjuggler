
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Tri.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Tri_float()
{
    class_< gmtl::Tri<float> >("Trif", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Point<float,3> &, const gmtl::Point<float,3> & >())
        .def(init< const gmtl::Tri<float> & >())
        .def("edge", &gmtl::Tri<float>::edge)
        .def("set", &gmtl::Tri<float>::set)
    ;

}
