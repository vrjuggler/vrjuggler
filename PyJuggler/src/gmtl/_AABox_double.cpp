
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/AABox.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_AABox_double()
{
    class_< gmtl::AABox<double> >("AABoxd", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Point<double,3> & >())
        .def(init< const gmtl::AABox<double> & >())
        .def_readwrite("mMin", &gmtl::AABox<double>::mMin)
        .def_readwrite("mMax", &gmtl::AABox<double>::mMax)
        .def_readwrite("mEmpty", &gmtl::AABox<double>::mEmpty)
        .def("getMin", &gmtl::AABox<double>::getMin, return_value_policy< copy_const_reference >())
        .def("getMax", &gmtl::AABox<double>::getMax, return_value_policy< copy_const_reference >())
        .def("isEmpty", &gmtl::AABox<double>::isEmpty)
        .def("setMin", &gmtl::AABox<double>::setMin)
        .def("setMax", &gmtl::AABox<double>::setMax)
        .def("setEmpty", &gmtl::AABox<double>::setEmpty)
    ;

}
