
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Plane.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Plane_float()
{
    class_< gmtl::Plane<float> >("Planef", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Point<float,3> &, const gmtl::Point<float,3> & >())
        .def(init< const gmtl::Vec<float,3> &, const gmtl::Point<float,3> & >())
        .def(init< const gmtl::Vec<float,3> &, const float & >())
        .def(init< const gmtl::Plane<float> & >())
        .def_readwrite("mNorm", &gmtl::Plane<float>::mNorm)
        .def_readwrite("mOffset", &gmtl::Plane<float>::mOffset)
        .def("getNormal", &gmtl::Plane<float>::getNormal, return_value_policy< copy_const_reference >())
        .def("setNormal", &gmtl::Plane<float>::setNormal)
        .def("getOffset", &gmtl::Plane<float>::getOffset, return_value_policy< copy_const_reference >())
        .def("setOffset", &gmtl::Plane<float>::setOffset)
    ;

}
