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

#ifndef _GADGET_ALL_BASE_TYPES_H_
#define _GADGET_ALL_BASE_TYPES_H_

#include <gadget/gadgetConfig.h>

#include <boost/mpl/vector.hpp>

#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/KeyboardMouse.h>
#include <gadget/Type/String.h>
#include <gadget/Type/Command.h>
#include <gadget/Type/Glove.h>
#include <gadget/Devices/Sim/SimInput.h>
#include <gadget/Type/Rumble.h>


namespace gadget
{

/**
 * The type list of all possible base types for input devices. This is
 * mainly for use with gadget::InputDevice. Note that gadget::Input is not
 * included in the list because gadget::Input is an explicit base class of
 * gadget::InputDevice.
 *
 * @note The order of types defined here is what dictates the invocation of
 *       base class methods during device data serialization and
 *       de-serialization.
 *
 * @see gadget::InputDevice::writeObject()
 * @see gadget::InputDevice::readObject()
 * @see gadget::InputDevice::getInputTypeName()
 *
 * @since 2.1.18
 */
typedef boost::mpl::vector<
     Digital
   , Analog
   , Position
   , KeyboardMouse
   , String
   , Command
   , Glove
   , SimInput
   , Rumble
> all_base_types;

}


#endif /* _GADGET_ALL_BASE_TYPES_H_ */
