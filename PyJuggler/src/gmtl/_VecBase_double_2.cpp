
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/VecBase.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_VecBase_double_2()
{
    scope* gmtl_VecBase_double_2_scope = new scope(
    class_< gmtl::VecBase<double,2> >("VecBase2d", init<  >())
        .def(init< const gmtl::VecBase<double,2> & >())
        .def(init< const double &, const double & >())
        .def(init< const double &, const double &, const double & >())
        .def(init< const double &, const double &, const double &, const double & >())
        .def("set", (void (gmtl::VecBase<double,2>::*)(const double *) )&gmtl::VecBase<double,2>::set)
        .def("set", (void (gmtl::VecBase<double,2>::*)(const double &) )&gmtl::VecBase<double,2>::set)
        .def("set", (void (gmtl::VecBase<double,2>::*)(const double &, const double &) )&gmtl::VecBase<double,2>::set)
        .def("set", (void (gmtl::VecBase<double,2>::*)(const double &, const double &, const double &) )&gmtl::VecBase<double,2>::set)
        .def("set", (void (gmtl::VecBase<double,2>::*)(const double &, const double &, const double &, const double &) )&gmtl::VecBase<double,2>::set)
        .def("__getitem__", (double& (gmtl::VecBase<double,2>::*)(const unsigned) ) &gmtl::VecBase<double,2>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<double,2>*, const unsigned, double)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<double,2>::Params >("Params")
        .value("Size", gmtl::VecBase<double,2>::Size)
    ;

    delete gmtl_VecBase_double_2_scope;

}
