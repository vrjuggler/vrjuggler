
// Includes ====================================================================
#include <boost/python.hpp>
#include <gmtl/Defines.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_gmtl_Defines_h()
{
    enum_< gmtl::PlaneSide >("PlaneSide")
        .value("NEG_SIDE", gmtl::NEG_SIDE)
        .value("POS_SIDE", gmtl::POS_SIDE)
        .value("ON_PLANE", gmtl::ON_PLANE)
    ;

    enum_< gmtl::VectorIndex >("VectorIndex")
        .value("Xelt", gmtl::Xelt)
        .value("Yelt", gmtl::Yelt)
        .value("Welt", gmtl::Welt)
        .value("Zelt", gmtl::Zelt)
    ;

}
