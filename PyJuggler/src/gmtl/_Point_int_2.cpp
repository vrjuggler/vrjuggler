
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Point.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Point_int_2()
{
    scope* gmtl_Point_int_2_scope = new scope(
    class_< gmtl::Point<int,2>, bases< gmtl::VecBase<int,2> > >("Point2i", init<  >())
        .def(init< const gmtl::Point<int,2> & >())
        .def(init< const gmtl::VecBase<int,2> & >())
        .def(init< const int &, const int & >())
        .def(init< const int &, const int &, const int & >())
        .def(init< const int &, const int &, const int &, const int & >())
    );

    enum_< gmtl::Point<int,2>::Params >("Params")
        .value("Size", gmtl::Point<int,2>::Size)
    ;

    delete gmtl_Point_int_2_scope;

}
