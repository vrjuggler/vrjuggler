
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/VecBase.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_VecBase_float_2()
{
    scope* gmtl_VecBase_float_2_scope = new scope(
    class_< gmtl::VecBase<float,2> >("VecBase2f", init<  >())
        .def(init< const gmtl::VecBase<float,2> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float *) )&gmtl::VecBase<float,2>::set)
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float &) )&gmtl::VecBase<float,2>::set)
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float &, const float &) )&gmtl::VecBase<float,2>::set)
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float &, const float &, const float &) )&gmtl::VecBase<float,2>::set)
        .def("set", (void (gmtl::VecBase<float,2>::*)(const float &, const float &, const float &, const float &) )&gmtl::VecBase<float,2>::set)
        .def("__getitem__", (float& (gmtl::VecBase<float,2>::*)(const unsigned) ) &gmtl::VecBase<float,2>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<float,2>*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<float,2>::Params >("Params")
        .value("Size", gmtl::VecBase<float,2>::Size)
    ;

    delete gmtl_VecBase_float_2_scope;

}
