/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_INPUT_BASE_TYPES_H_
#define _GADGET_INPUT_BASE_TYPES_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/InputMixer.h>

namespace gadget
{

class Input;
class Digital;
class Analog;
class Position;
class KeyboardMouse;
class String;
class Command;
class Glove;
class SimInput;
class Rumble;

/**
 * @name Device Driver Base Types
 *
 * The possible input base types for device drivers. A device driver
 * implementation must derive from one of these in order to function in a
 * cluster configuration.
 *
 * @since 1.3.8
 */
//@{
typedef InputMixer< Input, Digital > input_digital_t;
typedef InputMixer< Input, Analog > input_analog_t;
typedef InputMixer< Input, Position > input_position_t;
typedef InputMixer< Input, KeyboardMouse > input_keyboard_t;
typedef InputMixer< Input, String > input_string_t;
typedef InputMixer< Input, Command > input_command_t;
typedef InputMixer< Input, Glove > input_glove_t;
typedef InputMixer< InputMixer<Input, Digital>, Analog> input_digital_analog_t;
typedef InputMixer< InputMixer<Input, Digital>, Position> input_digital_position_t;
typedef InputMixer< InputMixer<Input, Analog>, Position> input_analog_position_t;
typedef InputMixer< InputMixer<Input, Glove>, Digital> input_glove_digital_t;
typedef InputMixer< InputMixer< InputMixer< InputMixer<Input, Glove>, Digital>, Analog>, Position> input_glove_digital_analog_position_t;
typedef InputMixer< InputMixer< InputMixer<Input, Digital>, Analog>, Position> input_digital_analog_position_t;
typedef InputMixer< InputMixer<SimInput, Input>, Position> siminput_input_position;
typedef InputMixer< InputMixer<SimInput, Input>, Digital> siminput_input_digital;
typedef InputMixer< InputMixer<SimInput, Input>, Analog> siminput_input_analog;
typedef InputMixer< InputMixer< InputMixer<SimInput,Input>, Digital>, Glove> siminput_input_digital_glove_t;
typedef InputMixer< InputMixer<Input, Command>, Analog> input_command_analog_t;
typedef InputMixer< InputMixer< InputMixer<Input, Digital>, Analog>, Rumble> input_digital_analog_rumble_t;
//@}

} // end namespace

#endif   /* GADGET_INPUT_BASE_TYPES_H */
