
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/EulerAngle.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_EulerAngle_float_gmtl_ZXY()
{
    scope* gmtl_EulerAngle_float_gmtl_ZXY_scope = new scope(
    class_< gmtl::EulerAngle<float,gmtl::ZXY> >("EulerAngleZXYf", init<  >())
        .def(init< const gmtl::EulerAngle<float,gmtl::ZXY> & >())
        .def(init< float, float, float >())
        .def("set", &gmtl::EulerAngle<float,gmtl::ZXY>::set)
        .def("__getitem__", (float& (gmtl::EulerAngle<float,gmtl::ZXY>::*)(const unsigned) ) &gmtl::EulerAngle<float,gmtl::ZXY>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::EulerAngle<float,gmtl::ZXY>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::EulerAngle<float,gmtl::ZXY>::Params >("Params")
        .value("Order", gmtl::EulerAngle<float,gmtl::ZXY>::Order)
        .value("Size", gmtl::EulerAngle<float,gmtl::ZXY>::Size)
    ;

    delete gmtl_EulerAngle_float_gmtl_ZXY_scope;

}
