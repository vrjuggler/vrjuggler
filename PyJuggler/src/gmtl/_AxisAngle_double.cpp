
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/AxisAngle.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_AxisAngle_double()
{
    scope* gmtl_AxisAngle_double_scope = new scope(
    class_< gmtl::AxisAngle<double>, bases< gmtl::VecBase<double, 4> > >("AxisAngled", init<  >())
        .def(init< const gmtl::AxisAngle<double> & >())
        .def(init< const double &, const double &, const double &, const double & >())
        .def(init< const double &, const gmtl::Vec<double,3> & >())
        .def("set", (void (gmtl::AxisAngle<double>::*)(const double &, const double &, const double &, const double &) )&gmtl::AxisAngle<double>::set)
        .def("set", (void (gmtl::AxisAngle<double>::*)(const double &, const gmtl::Vec<double,3> &) )&gmtl::AxisAngle<double>::set)
        .def("setAxis", &gmtl::AxisAngle<double>::setAxis)
        .def("setAngle", &gmtl::AxisAngle<double>::setAngle)
        .def("getAxis", &gmtl::AxisAngle<double>::getAxis)
        .def("getAngle", &gmtl::AxisAngle<double>::getAngle, return_value_policy< copy_const_reference >())
    );

    enum_< gmtl::AxisAngle<double>::Params >("Params")
        .value("Size", gmtl::AxisAngle<double>::Size)
    ;

    delete gmtl_AxisAngle_double_scope;

}
