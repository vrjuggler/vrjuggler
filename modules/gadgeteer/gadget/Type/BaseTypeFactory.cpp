/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#include <gadget/gadgetConfig.h>
#include <typeinfo>

#include <gadget/Type/BaseTypeFactory.h>

// Platform-independent devices.
#include <gadget/Type/Input.h>
#include <gadget/Devices/Sim/SimInput.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Glove.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/KeyboardMouse.h>
#include <gadget/Type/Command.h>
#include <gadget/Type/String.h>
#include <gadget/Type/InputMixer.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/InputBaseTypes.h>

namespace gadget
{

/**
 * Registers a creator for the BaseType base class.
 *
 * @pre Requires that the method std::string getInputTypeName() be defined for
 *      class BaseType.
 *
 * Ex: GADGET_REGISTER_BASE_TYPE_CREATOR(ConnectionAck)
 */
/*const bool reg_ctr_input_ ## BaseType = BaseType::MixedPlaceholderType::create()  \*/
#define GADGET_REGISTER_BASE_TYPE_CREATOR(BaseType)                             \
InputPtr input_ ## BaseType = BaseType::MixedPlaceholderType::create();         \
const bool reg_ctr_ ## BaseType =                                               \
   BaseTypeFactory::instance()->                                        \
      registerCreator(input_ ## BaseType->getInputTypeName(),                   \
                      BaseType::MixedPlaceholderType::create);                  \
   boost::ignore_unused_variable_warning(reg_ctr_ ## BaseType);

vprSingletonImpWithInitFunc( BaseTypeFactory, hackLoadKnownDevices );

/**
 * Registers all the devices that I know about.
 * @note This should really be moved to dynamic library loading code.
 */
void BaseTypeFactory::hackLoadKnownDevices()
{
   // NOTE: These will all given unused variable errors in compiling.
   // That is okay, because the don't actually have to do anything.
   // They just register themselves in their constructor.

   // Platform-independent devices.
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_digital_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_analog_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_position_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_keyboard_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_string_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_command_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_glove_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_digital_analog_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_digital_position_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_analog_position_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_glove_digital_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_glove_digital_analog_position_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(input_digital_analog_position_t);
   GADGET_REGISTER_BASE_TYPE_CREATOR(siminput_input_position);
   GADGET_REGISTER_BASE_TYPE_CREATOR(siminput_input_digital);
   GADGET_REGISTER_BASE_TYPE_CREATOR(siminput_input_analog);
   GADGET_REGISTER_BASE_TYPE_CREATOR(siminput_input_digital_glove_t);
}

} // End of gadget namespace
