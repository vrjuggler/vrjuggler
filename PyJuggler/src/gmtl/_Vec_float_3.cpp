
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Vec.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Vec_float_3()
{
    scope* gmtl_Vec_float_3_scope = new scope(
    class_< gmtl::Vec<float,3>, bases< gmtl::VecBase<float,3> > >("Vec3f", init<  >())
        .def(init< const gmtl::Vec<float,3> & >())
        .def(init< const gmtl::VecBase<float,3> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
    );

    enum_< gmtl::Vec<float,3>::Params >("Params")
        .value("Size", gmtl::Vec<float,3>::Size)
    ;

    delete gmtl_Vec_float_3_scope;

}
