
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Quat.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Quat_double()
{
    scope* gmtl_Quat_double_scope = new scope(
    class_< gmtl::Quat<double> >("Quatd", init<  >())
        .def(init< const double, const double, const double, const double >())
        .def(init< const gmtl::Quat<double> & >())
        .def_readwrite("mData", &gmtl::Quat<double>::mData)
        .def("set", &gmtl::Quat<double>::set)
        .def("get", &gmtl::Quat<double>::get)
        .def("__getitem__", (double& (gmtl::Quat<double>::*)(const int)) &gmtl::Quat<double>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::Quatd*, const unsigned, double)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::Quat<double>::Params >("Params")
        .value("Size", gmtl::Quat<double>::Size)
    ;

    delete gmtl_Quat_double_scope;

}
