/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_DEVICE_TRAITS_H_
#define _GADGET_DEVICE_TRAITS_H_

#include <gadget/gadgetConfig.h>

#include <gadget/Type/AnalogData.h>
#include <gadget/Type/CommandData.h>
#include <gadget/Type/DigitalData.h>
#include <gadget/Type/GloveData.h>
#include <gadget/Type/KeyboardMouseData.h>
#include <gadget/Type/PositionData.h>
#include <gadget/Type/StringData.h>
//#include <gadget/Type/KeyboardMouse.h>


namespace gadget
{

/**
 * @name Device Traits
 *
 * @since 2.1.1
 */
//@{
/** \struct DeviceTraits DeviceTraits.h gadget/Type/DeviceTraits.h
 *
 * The device traits template type. This type must be specialized for each
 * device type.
 *
 * @see gadget::TypedProxy
 */
template<typename T> struct DeviceTraits;

#define DECLARE_DEVICE_TRAITS(DevType)          \
   template<>                                   \
   struct DeviceTraits<class DevType>           \
   {                                            \
      typedef DevType device_type;              \
      typedef DevType ## Data data_type;        \
   };

DECLARE_DEVICE_TRAITS(Analog)
DECLARE_DEVICE_TRAITS(Command)
DECLARE_DEVICE_TRAITS(Digital)
DECLARE_DEVICE_TRAITS(Glove)
DECLARE_DEVICE_TRAITS(KeyboardMouse)
DECLARE_DEVICE_TRAITS(Position)
DECLARE_DEVICE_TRAITS(String)

// XXX: This is not correct. gadget::Gesture does not currently use any
// instantiation of gadget::DeviceData<T> for data handling.
template<>
struct DeviceTraits<class Gesture>
{
   typedef Gesture         device_type;
   typedef DeviceData<int> data_type;
};
//@}

}


#endif /* _GADGET_DEVICE_TRAITS_H_ */
