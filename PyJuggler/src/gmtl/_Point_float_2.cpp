
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Point.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Point_float_2()
{
    scope* gmtl_Point_float_2_scope = new scope(
    class_< gmtl::Point<float,2>, bases< gmtl::VecBase<float,2> > >("Point2f", init<  >())
        .def(init< const gmtl::Point<float,2> & >())
        .def(init< const gmtl::VecBase<float,2> & >())
        .def(init< const float &, const float & >())
        .def(init< const float &, const float &, const float & >())
        .def(init< const float &, const float &, const float &, const float & >())
    );

    enum_< gmtl::Point<float,2>::Params >("Params")
        .value("Size", gmtl::Point<float,2>::Size)
    ;

    delete gmtl_Point_float_2_scope;

}
