
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Math.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_gmtl_Math_h()
{
    def("deg2Rad", (double (*)(double))&gmtl::Math::deg2Rad);
    def("deg2Rad", (float (*)(float))&gmtl::Math::deg2Rad);
    def("rad2Deg", (float (*)(float))&gmtl::Math::rad2Deg);
    def("rad2Deg", (double (*)(double))&gmtl::Math::rad2Deg);
}
