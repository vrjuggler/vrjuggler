
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Ray.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Ray_float()
{
    class_< gmtl::Ray<float> >("Rayf", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Vec<float,3> & >())
        .def(init< const gmtl::Ray<float> & >())
        .def_readwrite("mOrigin", &gmtl::Ray<float>::mOrigin)
        .def_readwrite("mDir", &gmtl::Ray<float>::mDir)
        .def("getOrigin", &gmtl::Ray<float>::getOrigin, return_value_policy< copy_const_reference >())
        .def("setOrigin", &gmtl::Ray<float>::setOrigin)
        .def("getDir", &gmtl::Ray<float>::getDir, return_value_policy< copy_const_reference >())
        .def("setDir", &gmtl::Ray<float>::setDir)
    ;

}
