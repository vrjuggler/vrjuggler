/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

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
BOOST_PYTHON_MODULE(vrj)
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
