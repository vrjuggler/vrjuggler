// PyJuggler is (C) Copyright 2002-2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Boost Includes ==============================================================
#include <boost/python.hpp>
#include <boost/cstdint.hpp>

// Includes ====================================================================
#include <gadget/Type/KeyboardMouse/Event.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
void _Export_gadget_Type_KeyboardMouse_Event_h()
{
    enum_< gadget::EventType >("EventType")
        .value("KeyPressEvent", gadget::KeyPressEvent)
        .value("MouseButtonReleaseEvent", gadget::MouseButtonReleaseEvent)
        .value("MouseMoveEvent", gadget::MouseMoveEvent)
        .value("MouseButtonPressEvent", gadget::MouseButtonPressEvent)
        .value("NoEvent", gadget::NoEvent)
        .value("KeyReleaseEvent", gadget::KeyReleaseEvent)
    ;

}
