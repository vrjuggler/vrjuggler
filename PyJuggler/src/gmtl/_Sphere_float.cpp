
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Sphere.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Sphere_float()
{
    class_< gmtl::Sphere<float> >("Spheref", init<  >())
        .def(init< const gmtl::Point<float,3> &, const float & >())
        .def(init< const gmtl::Sphere<float> & >())
        .def_readwrite("mCenter", &gmtl::Sphere<float>::mCenter)
        .def_readwrite("mRadius", &gmtl::Sphere<float>::mRadius)
        .def("getCenter", &gmtl::Sphere<float>::getCenter, return_value_policy< copy_const_reference >())
        .def("getRadius", &gmtl::Sphere<float>::getRadius, return_value_policy< copy_const_reference >())
        .def("setCenter", &gmtl::Sphere<float>::setCenter)
        .def("setRadius", &gmtl::Sphere<float>::setRadius)
    ;

}
