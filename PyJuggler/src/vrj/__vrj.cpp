// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Include =====================================================================
#include <boost/python.hpp>

// Exports =====================================================================
void _Export_GlApp();
void _Export_Kernel();
void _Export_App();
void _Export_Projection();
void _Export_Display();
void _Export_Viewport();
void _Export_SimViewport();
void _Export_Frustum();
void _Export_CameraProjection();
void _Export_User();

#ifdef PYJUGGLER_HAVE_OSG
void _Export_OsgApp();
#endif

// Module ======================================================================
BOOST_PYTHON_MODULE(__vrj)
{
    // Create and acquire the global interpreter lock.
    PyEval_InitThreads();

    _Export_App();
    _Export_GlApp();
    _Export_Kernel();
    _Export_Projection();
    _Export_Display();
    _Export_Viewport();
    _Export_SimViewport();
    _Export_Frustum();
    _Export_CameraProjection();
    _Export_User();
#ifdef PYJUGGLER_HAVE_OSG
    _Export_OsgApp();
#endif
}
