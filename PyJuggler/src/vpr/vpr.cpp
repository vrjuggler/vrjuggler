// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Include =====================================================================
#include <boost/python.hpp>

// Exports =====================================================================
void _Export_Interval();
void _Export_ReturnStatus();
void _Export_ObjectReader();
void _Export_ObjectWriter();
void _Export_ReadableObject();
void _Export_WriteableObject();
void _Export_SerializableObject();
void _Export_GUID();

// Module ======================================================================
BOOST_PYTHON_MODULE(vpr)
{
    boost::python::scope().attr("__doc__") =
      "PyJuggler.vpr is an extension module written in Boost.Python that\n"
      "exposes some C++ classes of the VR Juggler Portable Runtime (VPR) for\n"
      "use in writing Python code.  The exposed classes are those that tend\n"
      "to be most useful in writing VR Juggler application objects.\n\n"
      "Refer to the VPR C++ documentation for general usage details:\n"
      "   http://www.vrjuggler.org/vapor/docs.php"
    ;
    _Export_Interval();
    _Export_ReturnStatus();
    _Export_ObjectReader();
    _Export_ObjectWriter();
    _Export_ReadableObject();
    _Export_WriteableObject();
    _Export_SerializableObject();
    _Export_GUID();
}
