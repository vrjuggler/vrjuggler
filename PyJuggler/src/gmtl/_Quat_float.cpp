
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Quat.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Quat_float()
{
    scope* gmtl_Quat_float_scope = new scope(
    class_< gmtl::Quat<float> >("Quatf", init<  >())
        .def(init< const float, const float, const float, const float >())
        .def(init< const gmtl::Quat<float> & >())
        .def_readwrite("mData", &gmtl::Quat<float>::mData)
        .def("set", &gmtl::Quat<float>::set)
        .def("get", &gmtl::Quat<float>::get)
        .def("__getitem__", (float& (gmtl::Quat<float>::*)(const int)) &gmtl::Quat<float>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::Quatf*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::Quat<float>::Params >("Params")
        .value("Size", gmtl::Quat<float>::Size)
    ;

    delete gmtl_Quat_float_scope;

}
