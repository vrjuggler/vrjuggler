
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Ray.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Ray_double()
{
    class_< gmtl::Ray<double> >("Rayd", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Vec<double,3> & >())
        .def(init< const gmtl::Ray<double> & >())
        .def_readwrite("mOrigin", &gmtl::Ray<double>::mOrigin)
        .def_readwrite("mDir", &gmtl::Ray<double>::mDir)
        .def("getOrigin", &gmtl::Ray<double>::getOrigin, return_value_policy< copy_const_reference >())
        .def("setOrigin", &gmtl::Ray<double>::setOrigin)
        .def("getDir", &gmtl::Ray<double>::getDir, return_value_policy< copy_const_reference >())
        .def("setDir", &gmtl::Ray<double>::setDir)
    ;

}
