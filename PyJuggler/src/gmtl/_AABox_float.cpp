
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/AABox.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_AABox_float()
{
    class_< gmtl::AABox<float> >("AABoxf", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Point<float,3> & >())
        .def(init< const gmtl::AABox<float> & >())
        .def_readwrite("mMin", &gmtl::AABox<float>::mMin)
        .def_readwrite("mMax", &gmtl::AABox<float>::mMax)
        .def_readwrite("mEmpty", &gmtl::AABox<float>::mEmpty)
        .def("getMin", &gmtl::AABox<float>::getMin, return_value_policy< copy_const_reference >())
        .def("getMax", &gmtl::AABox<float>::getMax, return_value_policy< copy_const_reference >())
        .def("isEmpty", &gmtl::AABox<float>::isEmpty)
        .def("setMin", &gmtl::AABox<float>::setMin)
        .def("setMax", &gmtl::AABox<float>::setMax)
        .def("setEmpty", &gmtl::AABox<float>::setEmpty)
    ;

}
