# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VRJ.NET is (C) Copyright 2004 by Patrick Hartling
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# -----------------------------------------------------------------
# File:          $RCSfile$
# Date modified: $Date$
# Version:       $Revision$
# -----------------------------------------------------------------
#
# *************** <auto-copyright.pl END do not edit this line> ***************

mod = 'gadget'

InputManager = ReferenceType(mod, 'gadget::InputManager',
                             'gadget/InputManager.h')
sealed(InputManager)
exclude(InputManager.getDeviceFactory)
exclude(InputManager.getInputLogger)

Input = ReferenceType(mod, 'gadget::Input', 'gadget/Type/Input.h')

Analog = ReferenceType(mod, 'gadget::Analog', 'gadget/Type/Analog.h')
exclude(Analog.addAnalogSample)
exclude(Analog.getAnalogDataBuffer)

Digital = ReferenceType(mod, 'gadget::Digital', 'gadget/Type/Digital.h')
exclude(Digital.addDigitalSample)
exclude(Digital.getDigitalDataBuffer)

Position = ReferenceType(mod, 'gadget::Position', 'gadget/Type/Position.h')
exclude(Position.addPositionSample)
exclude(Position.getPositionDataBuffer)

InputData = ReferenceType(mod, 'gadget::InputData', 'gadget/Type/InputData.h')
AnalogData = ReferenceType(mod, 'gadget::AnalogData',
                           'gadget/Type/AnalogData.h')
DigitalData = ReferenceType(mod, 'gadget::DigitalData',
                            'gadget/Type/DigitalData.h')
PositionData = ReferenceType(mod, 'gadget::PositionData',
                             'gadget/Type/PositionData.h')

Proxy = ReferenceType(mod, 'gadget::Proxy', 'gadget/Type/Proxy.h')
exclude(Proxy.set)

TypedProxy = ReferenceTemplate(mod, 'gadget::TypedProxy', 'gadget/Type/Proxy.h')
TypedProxy_ana = TypedProxy('gadget::Analog', ['gadget/Type/Analog.h'])
#rename(TypedProxy_ana, 'TypedProxy_Analog')
exclude(TypedProxy_ana.set)
TypedProxy_dig = TypedProxy('gadget::Digital', ['gadget/Type/Digital.h'])
#rename(TypedProxy_dig, 'TypedProxy_Digital')
exclude(TypedProxy_dig.set)
TypedProxy_pos = TypedProxy('gadget::Position', ['gadget/Type/Position.h'])
#rename(TypedProxy_pos, 'TypedProxy_Position')
exclude(TypedProxy_pos.set)

AnalogProxy = ReferenceType(mod, 'gadget::AnalogProxy',
                             'gadget/Type/AnalogProxy.h')
sealed(AnalogProxy)
exclude(AnalogProxy.set)

DigitalProxy = ReferenceType(mod, 'gadget::DigitalProxy',
                             'gadget/Type/DigitalProxy.h')
sealed(DigitalProxy)
exclude(DigitalProxy.set)

PositionProxy = ReferenceType(mod, 'gadget::PositionProxy',
                              'gadget/Type/PositionProxy.h')
sealed(PositionProxy)
exclude(PositionProxy.set)
exclude(PositionProxy.getPositionData)
exclude(PositionProxy.getPositionPtr)

BaseDeviceInterface = ReferenceType(mod, 'gadget::BaseDeviceInterface',
                                    'gadget/Type/DeviceInterface.h')

DeviceInterface = ReferenceTemplate(mod, 'gadget::DeviceInterface',
                                    'gadget/Type/DeviceInterface.h')

AnalogInterface = DeviceInterface('gadget::AnalogProxy',
                                   ['gadget/Type/AnalogInterface.h'],
                                   rename = 'gadget::AnalogInterface')
sealed(AnalogInterface)
use_smart_ptr(AnalogInterface)
no_smart_ptr(AnalogInterface.getProxy)
no_smart_ptr(AnalogInterface.refresh)
exclude(AnalogInterface.setProxy)

DigitalInterface = DeviceInterface('gadget::DigitalProxy',
                                   ['gadget/Type/DigitalInterface.h'],
                                   rename = 'gadget::DigitalInterface')
sealed(DigitalInterface)
use_smart_ptr(DigitalInterface)
no_smart_ptr(DigitalInterface.getProxy)
no_smart_ptr(DigitalInterface.refresh)
exclude(DigitalInterface.setProxy)

PositionInterface = DeviceInterface('gadget::PositionProxy',
                                    ['gadget/Type/PositionInterface.h'],
                                    rename = 'gadget::PositionInterface')
sealed(PositionInterface)
use_smart_ptr(PositionInterface)
no_smart_ptr(PositionInterface.getProxy)
no_smart_ptr(PositionInterface.refresh)
exclude(PositionInterface.setProxy)

PositionUnitConv = FreeTypesHolder(mod, 'PositionUnitConversion',
                                   'gadget/Type/Position/PositionUnitConversion.h')
PositionUnitConv.addConstant('gadget::PositionUnitConversion::ConvertToFeet')
PositionUnitConv.addConstant('gadget::PositionUnitConversion::ConvertToInches')
PositionUnitConv.addConstant('gadget::PositionUnitConversion::ConvertToMeters')
PositionUnitConv.addConstant('gadget::PositionUnitConversion::ConvertToCentimeters')
