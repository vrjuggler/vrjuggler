
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Plane.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Plane_double()
{
    class_< gmtl::Plane<double> >("Planed", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Point<double,3> &, const gmtl::Point<double,3> & >())
        .def(init< const gmtl::Vec<double,3> &, const gmtl::Point<double,3> & >())
        .def(init< const gmtl::Vec<double,3> &, const double & >())
        .def(init< const gmtl::Plane<double> & >())
        .def_readwrite("mNorm", &gmtl::Plane<double>::mNorm)
        .def_readwrite("mOffset", &gmtl::Plane<double>::mOffset)
        .def("getNormal", &gmtl::Plane<double>::getNormal, return_value_policy< copy_const_reference >())
        .def("setNormal", &gmtl::Plane<double>::setNormal)
        .def("getOffset", &gmtl::Plane<double>::getOffset, return_value_policy< copy_const_reference >())
        .def("setOffset", &gmtl::Plane<double>::setOffset)
    ;

}
