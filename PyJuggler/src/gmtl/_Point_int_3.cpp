
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Point.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_Point_int_3()
{
    scope* gmtl_Point_int_3_scope = new scope(
    class_< gmtl::Point<int,3>, bases< gmtl::Point<int,3> > >("Point3i", init<  >())
        .def(init< const gmtl::Point<int,3> & >())
        .def(init< const gmtl::VecBase<int,3> & >())
        .def(init< const int &, const int & >())
        .def(init< const int &, const int &, const int & >())
        .def(init< const int &, const int &, const int &, const int & >())
    );

    enum_< gmtl::Point<int,3>::Params >("Params")
        .value("Size", gmtl::Point<int,3>::Size)
    ;

    delete gmtl_Point_int_3_scope;

}
