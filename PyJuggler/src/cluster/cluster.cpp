// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Include =====================================================================
#include <boost/python.hpp>

// Exports =====================================================================
void _Export_UserData();

// Module ======================================================================
BOOST_PYTHON_MODULE(cluster)
{
   boost::python::scope().attr("__doc__") =
      "PyJuggler.cluster is an extension module written in Boost.Python that\n"
      "exposes some C++ classes from the 'cluster' namespace of the\n"
      "Gadgeteer library.  The exposed classes are those that tend to be of\n"
      "the most value for writing cluster-aware VR Juggler application\n"
      "objects.\n\n"
      "Refer to the Gadgeteer and VR Juggler 2.0 C++ documentation for\n"
      "general usage details:\n"
      "   http://www.vrjuggler.org/gadgeteer/docs.php\n"
      "   http://www.vrjuggler.org/vrjuggler/docs.php"
   ;
   _Export_UserData();
}
