
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/LineSeg.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_LineSeg_float()
{
    class_< gmtl::LineSeg<float> >("LineSegf", init<  >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Vec<float,3> & >())
        .def(init< const gmtl::LineSeg<float> & >())
        .def(init< const gmtl::Point<float,3> &, const gmtl::Point<float,3> & >())
        .def("getLength", &gmtl::LineSeg<float>::getLength)
    ;

}
