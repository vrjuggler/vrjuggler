
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Sphere.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Sphere_double()
{
    class_< gmtl::Sphere<double> >("Sphered", init<  >())
        .def(init< const gmtl::Point<double,3> &, const double & >())
        .def(init< const gmtl::Sphere<double> & >())
        .def_readwrite("mCenter", &gmtl::Sphere<double>::mCenter)
        .def_readwrite("mRadius", &gmtl::Sphere<double>::mRadius)
        .def("getCenter", &gmtl::Sphere<double>::getCenter, return_value_policy< copy_const_reference >())
        .def("getRadius", &gmtl::Sphere<double>::getRadius, return_value_policy< copy_const_reference >())
        .def("setCenter", &gmtl::Sphere<double>::setCenter)
        .def("setRadius", &gmtl::Sphere<double>::setRadius)
    ;

}
