// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <vrj/Display/Projection.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================
namespace pyj {

struct vrj_Projection_Wrapper: vrj::Projection
{
    vrj_Projection_Wrapper(PyObject* self_):
        vrj::Projection(), self(self_) {}

    vrj_Projection_Wrapper(PyObject* self_, const vrj::Projection& p0):
        vrj::Projection(p0), self(self_) {}

    void calcViewMatrix(gmtl::Matrix44f& p0, const float p1) {
        try
        {
            call_method< void >(self, "calcViewMatrix", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    PyObject* self;
};

}// namespace 


// Module ======================================================================
void _Export_Projection()
{
    scope* vrj_Projection_scope = new scope(
    class_< vrj::Projection, boost::noncopyable, pyj::vrj_Projection_Wrapper >("Projection",
         "Abstract base class for viewport definitions.  This class is\n"
         "responsible for storing and computing projection information\n"
         "based upon eye positions.  This class is an abstract base class\n"
         "for other classes that actually compute the projections."
         ,
         init<  >()
        )
        .def("calcViewMatrix", pure_virtual(&vrj::Projection::calcViewMatrix),
             "calcViewMatrix(eyePos, scaleFactor)\n"
             "Calculates the view matrix.\n\n"
             "Pre-conditions:\n"
             "eyePos is scaled by the position scaling factor.  scaleFactor\n"
             "is the scale currently used.\n\n"
             "Arguments:\n"
             "eyePos      -- A gmtl.Matrix44f object that represents the eye\n"
             "               position.\n"
             "scaleFactor -- The scale factor."
         )
        .def("getEye", &vrj::Projection::getEye,
             "getEye() -> vrj.Projection.Eye value\n"
             "Returns the eye for this projection."
         )
        .def("getViewport", &vrj::Projection::getViewport,
             return_internal_reference< 1 >(),
             "getViewport() -> vrj.Viewport object\n"
             "Returns the viewport associated with this projection."
         )
        .def("getFrustumApexAndCorners",
             &vrj::Projection::getFrustumApexAndCorners,
             "getFrustumApexAndCorners(apex, ur, lr, ul, ll)\n"
             "Helper to get the frustum apex and corners in model\n"
             "coordinates.\n\n"
             "Post-conditions:\n"
             "The given variables contain the values of the frusta corners\n"
             "in model space.\n\n"
             "Arguments:\n"
             "apex -- A gmtl.Vec3f used for storing the apex.\n"
             "ur   -- A gmtl.Vec3f used for storing the upper right corner of\n"
             "        the frustum.\n"
             "lr   -- A gmtl.Vec3f used for storing the lower right corner of\n"
             "        the frustum.\n"
             "ul   -- A gmtl.Vec3f used for storing the upper left corner of\n"
             "        the frustum.\n"
             "ll   -- A gmtl.Vec3f used for storing the lower left corner of\n"
             "        the frustum.\n\n"
             "Note:\n"
             "This function is meant for debugging purposes."
         )
        .def("getViewMatrix", &vrj::Projection::getViewMatrix,
             return_value_policy< copy_const_reference >(),
             "getViewMatrix() -> gmtl.Matrix44f object\n"
             "Returns this projection's view matrix."
         )
        .def("getFrustum", &vrj::Projection::getFrustum,
             "getFrustum() -> vrj.Frustum object\n"
             "Returns a copy of this projection's frustum."
         )
//        .def(self_ns::str(self))
    );

    enum_< vrj::Projection::Eye >("Eye")
        .value("RIGHT", vrj::Projection::RIGHT)
        .value("LEFT", vrj::Projection::LEFT)
    ;

    delete vrj_Projection_scope;

}
