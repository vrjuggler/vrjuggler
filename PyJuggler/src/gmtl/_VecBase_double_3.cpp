
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/VecBase.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_VecBase_double_3()
{
    scope* gmtl_VecBase_double_3_scope = new scope(
    class_< gmtl::VecBase<double,3> >("VecBase3d", init<  >())
        .def(init< const gmtl::VecBase<double,3> & >())
        .def(init< const double &, const double & >())
        .def(init< const double &, const double &, const double & >())
        .def(init< const double &, const double &, const double &, const double & >())
        .def("set", (void (gmtl::VecBase<double,3>::*)(const double *) )&gmtl::VecBase<double,3>::set)
        .def("set", (void (gmtl::VecBase<double,3>::*)(const double &) )&gmtl::VecBase<double,3>::set)
        .def("set", (void (gmtl::VecBase<double,3>::*)(const double &, const double &) )&gmtl::VecBase<double,3>::set)
        .def("set", (void (gmtl::VecBase<double,3>::*)(const double &, const double &, const double &) )&gmtl::VecBase<double,3>::set)
        .def("set", (void (gmtl::VecBase<double,3>::*)(const double &, const double &, const double &, const double &) )&gmtl::VecBase<double,3>::set)
        .def("__getitem__", (double& (gmtl::VecBase<double,3>::*)(const unsigned) ) &gmtl::VecBase<double,3>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<double,3>*, const unsigned, double)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<double,3>::Params >("Params")
        .value("Size", gmtl::VecBase<double,3>::Size)
    ;

    delete gmtl_VecBase_double_3_scope;

}
