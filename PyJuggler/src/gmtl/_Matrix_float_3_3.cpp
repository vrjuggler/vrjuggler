
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Matrix.h>
#include <gmtl-wrappers.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Matrix_float_3_3()
{
    scope* gmtl_Matrix_float_3_3_scope = new scope(
    class_< gmtl::Matrix<float,3,3> >("Matrix33f", init<  >())
        .def(init< const gmtl::Matrix<float,3,3> & >())
        .def_readwrite("mState", &gmtl::Matrix<float,3,3>::mState)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float, float, float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(float, float, float, float, float, float, float, float, float, float, float, float) )&gmtl::Matrix<float,3,3>::set)
        .def("set", (void (gmtl::Matrix<float,3,3>::*)(const float *) )&gmtl::Matrix<float,3,3>::set)
        .def("setTranspose", &gmtl::Matrix<float,3,3>::setTranspose)
        .def("isError", &gmtl::Matrix<float,3,3>::isError)
        .def("setError", &gmtl::Matrix<float,3,3>::setError)
        .def("__getitem__", &gmtl::Matrix<float,3,3>::operator[])
    );

    class_< gmtl::Matrix<float,3,3>::RowAccessor >("RowAccessor", no_init)
       .def("__getitem__", &gmtl::Matrix<float,3,3>::RowAccessor::operator[], return_value_policy<copy_non_const_reference>())
       .def("__setitem__", (void (*)(gmtl::Matrix33f::RowAccessor*, const unsigned, float)) &gmtlWrapper::setArrayElement)
    ;


    enum_< gmtl::Matrix<float,3,3>::Params >("Params")
        .value("Rows", gmtl::Matrix<float,3,3>::Rows)
        .value("Cols", gmtl::Matrix<float,3,3>::Cols)
    ;


    enum_< gmtl::Matrix<float,3,3>::XformState >("XformState")
        .value("ORTHONORMAL", gmtl::Matrix<float,3,3>::ORTHONORMAL)
        .value("XFORM_ERROR", gmtl::Matrix<float,3,3>::XFORM_ERROR)
        .value("FULL", gmtl::Matrix<float,3,3>::FULL)
        .value("ORTHOGONAL", gmtl::Matrix<float,3,3>::ORTHOGONAL)
        .value("AFFINE", gmtl::Matrix<float,3,3>::AFFINE)
        .value("IDENTITY", gmtl::Matrix<float,3,3>::IDENTITY)
    ;

    delete gmtl_Matrix_float_3_3_scope;

}
