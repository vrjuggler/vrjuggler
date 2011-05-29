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

#include <boost/mpl/inherit.hpp>

#include <gadget/Type/InputDevice.h>
#include <gadget/Devices/Sim/SimInputDevice.h>
#include <gadget/Type/BaseTypeFactory.h>


namespace
{

template<typename Base>
class PlaceholderDevice
   : public Base
{
private:
   PlaceholderDevice()
   {
      /* Do nothing. */ ;
   }

   /** @name gadget::Input Interface Implementation */
   //@{
   virtual bool sample()
   {
      return false;
   }

   virtual bool startSampling()
   {
      return false;
   }

   virtual bool stopSampling()
   {
      return false;
   }

   virtual void updateData()
   {
      /* Do nothing. */ ;
   }
   //@}

public:
   static gadget::InputPtr create()
   {
      return gadget::InputPtr(new PlaceholderDevice);
   }
};

template<template<class> class DeviceType, typename T>
void registerBaseType()
{
   typedef DeviceType<T> base_type;
   std::cout << PlaceholderDevice<base_type>::create()->getInputTypeName()
             << std::endl;
   gadget::BaseTypeFactory::instance()->registerCreator(
      PlaceholderDevice<base_type>::create()->getInputTypeName(),
      PlaceholderDevice<base_type>::create
   );
}

}

namespace gadget
{

vprSingletonImpWithInitFunc(BaseTypeFactory, registerBaseDeviceTypes);

/**
 * Registers all the input device base types.
 */
void BaseTypeFactory::registerBaseDeviceTypes()
{
   // While it would be nice to generate this registration code at compile
   // time, there are 255 k-combinations of the members of
   // gadget::all_base_types. That is far more than the few that we actually
   // need.
   registerBaseType<InputDevice, Digital>();
   registerBaseType<InputDevice, Analog>();
   registerBaseType<InputDevice, Position>();
   registerBaseType<InputDevice, KeyboardMouse>();
   registerBaseType<InputDevice, String>();
   registerBaseType<InputDevice, Command>();
   registerBaseType<InputDevice, Glove>();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Command, Analog>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Digital, Analog>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Digital, Position>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Analog, Position>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Glove, Digital>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Glove, Digital, Analog, Position>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Digital, Analog, Position>::type
   >();
   registerBaseType<
      InputDevice, boost::mpl::inherit<Digital, Analog, Rumble>::type
   >();
   registerBaseType<SimInputDevice, Position>();
   registerBaseType<SimInputDevice, Digital>();
   registerBaseType<SimInputDevice, Analog>();
   registerBaseType<
      SimInputDevice, boost::mpl::inherit<Digital, Glove>::type
   >();
}

} // End of gadget namespace
