
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/EulerAngle.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_EulerAngle_float_gmtl_XYZ()
{
    scope* gmtl_EulerAngle_float_gmtl_XYZ_scope = new scope(
    class_< gmtl::EulerAngle<float,gmtl::XYZ> >("EulerAngleXYZf", init<  >())
        .def(init< const gmtl::EulerAngle<float,gmtl::XYZ> & >())
        .def(init< float, float, float >())
        .def("set", &gmtl::EulerAngle<float,gmtl::XYZ>::set)
        .def("__getitem__", (float& (gmtl::EulerAngle<float,gmtl::XYZ>::*)(const unsigned) ) &gmtl::EulerAngle<float,gmtl::XYZ>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::EulerAngle<float,gmtl::XYZ>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::EulerAngle<float,gmtl::XYZ>::Params >("Params")
        .value("Order", gmtl::EulerAngle<float,gmtl::XYZ>::Order)
        .value("Size", gmtl::EulerAngle<float,gmtl::XYZ>::Size)
    ;

    delete gmtl_EulerAngle_float_gmtl_XYZ_scope;

}
