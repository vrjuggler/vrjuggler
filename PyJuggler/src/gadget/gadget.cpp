// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Include =====================================================================
#include <boost/python.hpp>

// Exports =====================================================================
void _Export_InputManager();
void _Export_AnalogData();
void _Export_DigitalData();
void _Export_PositionData();
void _Export_StringData();
void _Export_Analog();
void _Export_Digital();
void _Export_Position();
void _Export_KeyboardMouse();
void _Export_Command();
void _Export_String();
void _Export_AnalogProxy();
void _Export_DigitalProxy();
void _Export_PositionProxy();
void _Export_KeyboardMouseProxy();
void _Export_CommandProxy();
void _Export_StringProxy();
void _Export_BaseDeviceInterface();
void _Export_AnalogInterface();
void _Export_DigitalInterface();
void _Export_PositionInterface();
void _Export_KeyboardMouseInterface();
void _Export_CommandInterface();
void _Export_StringInterface();
void _Export_gadget_Type_KeyboardMouse_Event_h();
void _Export_gadget_Type_KeyboardMouse_Keys_h();
void _Export_Event();
void _Export_KeyEvent();
void _Export_MouseEvent();
void _Export_PositionUnitConversion();

// Module ======================================================================
BOOST_PYTHON_MODULE(gadget)
{
    boost::python::scope().attr("__doc__") =
       "PyJuggler.gadget is an extension module written in Boost.Python that\n"
       "exposes some C++ classes from the 'gadget' namespace of the\n"
       "Gadgeteer library.  The exposed classes are those that tend to be of\n"
       "the most value for writing VR Juggler application objects.  Some\n"
       "accomodations are made for writing device drivers in Python, but\n"
       "that capability is not the focus of this extension module.\n\n"
       "Refer to the Gadgeteer and VR Juggler 2.0 C++ documentation for\n"
       "general usage details:\n"
       "   http://www.vrjuggler.org/gadgeteer/docs.php\n"
       "   http://www.vrjuggler.org/vrjuggler/docs.php"
    ;
    _Export_InputManager();
    _Export_AnalogData();
    _Export_DigitalData();
    _Export_PositionData();
    _Export_StringData();
    _Export_Analog();
    _Export_Digital();
    _Export_Position();
    _Export_KeyboardMouse();
    _Export_Command();
    _Export_String();
    _Export_AnalogProxy();
    _Export_DigitalProxy();
    _Export_PositionProxy();
    _Export_KeyboardMouseProxy();
    _Export_CommandProxy();
    _Export_StringProxy();
    _Export_BaseDeviceInterface();
    _Export_AnalogInterface();
    _Export_DigitalInterface();
    _Export_PositionInterface();
    _Export_KeyboardMouseInterface();
    _Export_CommandInterface();
    _Export_StringInterface();
    _Export_gadget_Type_KeyboardMouse_Keys_h();
    _Export_gadget_Type_KeyboardMouse_Event_h();
    _Export_Event();
    _Export_KeyEvent();
    _Export_MouseEvent();
    _Export_PositionUnitConversion();
}
