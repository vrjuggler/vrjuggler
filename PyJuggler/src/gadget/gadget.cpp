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
void _Export_AnalogData();
void _Export_KeyEvent();
void _Export_Position();
void _Export_Digital();
void _Export_PositionData();
void _Export_Analog();
void _Export_AnalogProxy();
void _Export_BaseDeviceInterface();
void _Export_gadget_Type_EventWindow_Keys_h();
void _Export_DigitalProxy();
void _Export_PositionProxy();
void _Export_DigitalData();
void _Export_Event();
void _Export_gadget_Type_EventWindow_Event_h();
void _Export_AnalogInterface();
void _Export_DigitalInterface();
void _Export_PositionInterface();
void _Export_MouseEvent();

// Module ======================================================================
BOOST_PYTHON_MODULE(gadget)
{
    _Export_AnalogData();
    _Export_Event();
    _Export_KeyEvent();
    _Export_Position();
    _Export_Digital();
    _Export_PositionData();
    _Export_Analog();
    _Export_AnalogProxy();
    _Export_BaseDeviceInterface();
    _Export_gadget_Type_EventWindow_Keys_h();
    _Export_DigitalProxy();
    _Export_PositionProxy();
    _Export_DigitalData();
    _Export_gadget_Type_EventWindow_Event_h();
    _Export_AnalogInterface();
    _Export_DigitalInterface();
    _Export_PositionInterface();
    _Export_MouseEvent();
}
