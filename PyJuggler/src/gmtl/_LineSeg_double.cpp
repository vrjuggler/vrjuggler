
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/LineSeg.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_LineSeg_double()
{
    class_< gmtl::LineSeg<double> >("LineSegd", init<  >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Vec<double,3> & >())
        .def(init< const gmtl::LineSeg<double> & >())
        .def(init< const gmtl::Point<double,3> &, const gmtl::Point<double,3> & >())
        .def("getLength", &gmtl::LineSeg<double>::getLength)
    ;

}
