// PyJuggler is (C) Copyright 2002-2005 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Include =====================================================================
#include <boost/python.hpp>

// Exports =====================================================================
void _Export_OsgApp();

// Module ======================================================================
BOOST_PYTHON_MODULE(__osgapp)
{
    boost::python::scope().attr("__doc__") =
       "PyJuggler.__osgapp is an extension module written in Boost.Python\n"
       "that exposes the C++ class vrj::OsgApp.  Upon successful loading,\n"
       "the symbols of this module are imported into the PyJuggler.vrj\n"
       "namespace for use in the authoring of VR Juggler application\n"
       "objects based on Open Scene Graph.  To accomplish this, PyOSG must\n"
       "be used together with vrj.OsgApp.\n\n"
       "Refer to the VR Juggler 2.0 C++ documentation for general usage\n"
       "details:\n"
       "   http://www.vrjuggler.org/vrjuggler/docs.php"
    ;

    _Export_OsgApp();
}
