
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/AxisAngle.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_AxisAngle_float()
{
    scope* gmtl_AxisAngle_float_scope = new scope(
    class_< gmtl::AxisAngle<float>, bases< gmtl::VecBase<float,4> > >("AxisAnglef", init<  >())
        .def(init< const gmtl::AxisAngle<float> & >())
        .def(init< const float &, const float &, const float &, const float & >())
        .def(init< const float &, const gmtl::Vec<float,3> & >())
        .def("set", (void (gmtl::AxisAngle<float>::*)(const float &, const float &, const float &, const float &) )&gmtl::AxisAngle<float>::set)
        .def("set", (void (gmtl::AxisAngle<float>::*)(const float &, const gmtl::Vec<float,3> &) )&gmtl::AxisAngle<float>::set)
        .def("setAxis", &gmtl::AxisAngle<float>::setAxis)
        .def("setAngle", &gmtl::AxisAngle<float>::setAngle)
        .def("getAxis", &gmtl::AxisAngle<float>::getAxis)
        .def("getAngle", &gmtl::AxisAngle<float>::getAngle, return_value_policy< copy_const_reference >())
    );

    enum_< gmtl::AxisAngle<float>::Params >("Params")
        .value("Size", gmtl::AxisAngle<float>::Size)
    ;

    delete gmtl_AxisAngle_float_scope;

}
