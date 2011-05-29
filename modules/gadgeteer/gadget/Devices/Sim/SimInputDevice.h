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

#ifndef _GADGET_SIM_INPUT_DEVICE_H_
#define _GADGET_SIM_INPUT_DEVICE_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/InputDevice.h>
#include <gadget/Devices/Sim/SimInput.h>


namespace gadget
{

/** \class SimInputDevice SimInputDevice.h gadget/Devices/Sim/SimInputDevice.h
 *
 * @since 2.1.18
 */
template<typename Base>
class SimInputDevice
   : public SimInput
   , public InputDevice<Base>
{
public:
   typedef typename SimInputDevice::input_device_ base_type;

   /** @name gadget::InputDevice Overrides */
   //@{
   virtual void writeObject(vpr::ObjectWriter* writer)
   {
      base_type::writeObject(writer);
      SimInput::writeObject(writer);
   }

   virtual void readObject(vpr::ObjectReader* reader)
   {
      base_type::readObject(reader);
      SimInput::readObject(reader);
   }

   virtual std::string getInputTypeName()
   {
      return base_type::getInputTypeName() + SimInput::getInputTypeName();
   }
   //@}

};

}


#endif /* _GADGET_SIM_INPUT_DEVICE_H_ */
