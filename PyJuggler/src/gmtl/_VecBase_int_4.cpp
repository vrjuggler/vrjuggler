
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/VecBase.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_VecBase_int_4()
{
    scope* gmtl_VecBase_int_4_scope = new scope(
    class_< gmtl::VecBase<int,4> >("VecBase4i", init<  >())
        .def(init< const gmtl::VecBase<int,4> & >())
        .def(init< const int &, const int & >())
        .def(init< const int &, const int &, const int & >())
        .def(init< const int &, const int &, const int &, const int & >())
        .def("set", (void (gmtl::VecBase<int,4>::*)(const int *) )&gmtl::VecBase<int,4>::set)
        .def("set", (void (gmtl::VecBase<int,4>::*)(const int &) )&gmtl::VecBase<int,4>::set)
        .def("set", (void (gmtl::VecBase<int,4>::*)(const int &, const int &) )&gmtl::VecBase<int,4>::set)
        .def("set", (void (gmtl::VecBase<int,4>::*)(const int &, const int &, const int &) )&gmtl::VecBase<int,4>::set)
        .def("set", (void (gmtl::VecBase<int,4>::*)(const int &, const int &, const int &, const int &) )&gmtl::VecBase<int,4>::set)
        .def("__getitem__", (int& (gmtl::VecBase<int,4>::*)(const unsigned) ) &gmtl::VecBase<int,4>::operator[], return_value_policy<copy_non_const_reference>())
        .def("__setitem__", (void (*)(gmtl::VecBase<int,4>*, const unsigned, int)) &gmtlWrapper::setArrayElement)
    );

    enum_< gmtl::VecBase<int,4>::Params >("Params")
        .value("Size", gmtl::VecBase<int,4>::Size)
    ;

    delete gmtl_VecBase_int_4_scope;

}
