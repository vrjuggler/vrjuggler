
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Point.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Point_float_3()
{
    scope* gmtl_Point_float_3_scope = new scope(
    class_< gmtl::Point<float,3>, bases< gmtl::VecBase<float,3> > >("Point3f", init<  >())
        .def(init< const gmtl::Point<float,3> & >())
        .def(init< const gmtl::VecBase<float,3> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
    );

    enum_< gmtl::Point<float,3>::Params >("Params")
        .value("Size", gmtl::Point<float,3>::Size)
    ;

    delete gmtl_Point_float_3_scope;

}
