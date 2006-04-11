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
//#include <gadget/Type/BaseTypes.h>
#include <gadget/Util/Debug.h>


namespace gadget
{

// Initialize the singleton ptr
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

   // Input Digital
   BaseTypeConstructor< InputMixer<Input,Digital>::MixedPlaceholderType >* input_digital
         = new BaseTypeConstructor< InputMixer<Input,Digital>::MixedPlaceholderType >;

   // Input Analog
   BaseTypeConstructor< InputMixer<Input,Analog>::MixedPlaceholderType >* input_analog
         = new BaseTypeConstructor< InputMixer<Input,Analog>::MixedPlaceholderType >;

   // Input Position
   BaseTypeConstructor< InputMixer<Input,Position>::MixedPlaceholderType >* input_position
         = new BaseTypeConstructor< InputMixer<Input,Position>::MixedPlaceholderType >;

   // Input Digital Analog
   // Input Analog Digital
   BaseTypeConstructor< InputMixer< InputMixer<Input,Digital> , Analog>::MixedPlaceholderType >* input_digital_analog
         = new BaseTypeConstructor< InputMixer< InputMixer<Input,Digital> , Analog>::MixedPlaceholderType >;

   // Input Digital Position
   // Input Position Digital
   BaseTypeConstructor< InputMixer< InputMixer<Input,Digital> , Position>::MixedPlaceholderType >* input_digital_position
         = new BaseTypeConstructor< InputMixer< InputMixer<Input,Digital> , Position>::MixedPlaceholderType >;

   // Input Analog Position
   // Input Position Analog
   BaseTypeConstructor< InputMixer< InputMixer<Input,Analog> , Position>::MixedPlaceholderType >* input_analog_position
         = new BaseTypeConstructor< InputMixer< InputMixer<Input,Analog> , Position>::MixedPlaceholderType >;

   // Input Digital Analog Position
   BaseTypeConstructor< InputMixer< InputMixer< InputMixer<Input,Digital> , Analog> , Position>::MixedPlaceholderType >* input_digital_analog_position
         = new BaseTypeConstructor< InputMixer< InputMixer< InputMixer<Input,Digital> , Analog> , Position>::MixedPlaceholderType >;

   // SimInput Input Position
   BaseTypeConstructor< InputMixer< InputMixer<SimInput,Input> , Position>::MixedPlaceholderType >* siminput_input_position
         = new BaseTypeConstructor< InputMixer< InputMixer<SimInput,Input> , Position>::MixedPlaceholderType >;

   // SimInput Input Digital
   BaseTypeConstructor< InputMixer< InputMixer<SimInput,Input> , Digital>::MixedPlaceholderType >* siminput_input_digital
         = new BaseTypeConstructor< InputMixer< InputMixer<SimInput,Input> , Digital>::MixedPlaceholderType >;

   // SimInput Input Analog
   BaseTypeConstructor< InputMixer< InputMixer<SimInput,Input> , Analog>::MixedPlaceholderType >* siminput_input_analog
         = new BaseTypeConstructor< InputMixer< InputMixer<SimInput,Input> , Analog>::MixedPlaceholderType >;

   // Input KeyboardMouse
   BaseTypeConstructor< InputMixer<Input,KeyboardMouse>::MixedPlaceholderType >* input_keyboard
         = new BaseTypeConstructor< InputMixer<Input,KeyboardMouse>::MixedPlaceholderType >;

   // Input String
   BaseTypeConstructor< InputMixer<Input, String>::MixedPlaceholderType >* input_string
         = new BaseTypeConstructor< InputMixer<Input, String>::MixedPlaceholderType >;

   // Input Command
   BaseTypeConstructor< InputMixer<Input, Command>::MixedPlaceholderType >* input_command
         = new BaseTypeConstructor< InputMixer<Input, Command>::MixedPlaceholderType >;

   // Input Glove
   BaseTypeConstructor< InputMixer<Input,Glove>::MixedPlaceholderType >* input_glove
         = new BaseTypeConstructor< InputMixer<Input,Glove>::MixedPlaceholderType >;

   // Input Glove Digital
   BaseTypeConstructor< InputMixer< InputMixer<Input, Glove> , Digital>::MixedPlaceholderType >* input_glove_digital
         = new BaseTypeConstructor< InputMixer< InputMixer<Input, Glove> , Digital>::MixedPlaceholderType >;

   if( (NULL == input_digital) ||
       (NULL == input_analog) ||
       (NULL == input_position) ||
       (NULL == input_digital_analog) ||
       (NULL == input_digital_position) ||
       (NULL == input_analog_position) ||
       (NULL == input_digital_analog_position) ||
       (NULL == siminput_input_position) ||
       (NULL == siminput_input_digital) ||
       (NULL == siminput_input_analog) ||
       (NULL == input_keyboard) ||
       (NULL == input_string) ||
       (NULL == input_command) ||
       (NULL == input_glove) ||
       (NULL == input_glove_digital) )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED,"ERROR:") << " Failed to load a known device\n"
         << vprDEBUG_FLUSH;
   }
}

void BaseTypeFactory::registerNetDevice(BaseTypeConstructorBase* constructor)
{
   vprASSERT(constructor != NULL);
   mConstructors.push_back(constructor);     // Add the constructor to the list
}

// Simply query all device constructors registered looking
// for one that knows how to load the device
bool BaseTypeFactory::recognizeNetDevice(std::string base_type)
{
   return ! (findConstructor(base_type) == -1);
}

/**
 * Loads the specified device.
 */
Input* BaseTypeFactory::loadNetDevice(std::string base_type)
{
   vprASSERT(recognizeNetDevice(base_type));

   int index = findConstructor(base_type);

   Input* new_dev;
   BaseTypeConstructorBase* constructor = mConstructors[index];

   new_dev = constructor->createNetDevice(base_type);
   if(new_dev!=NULL)
   {
      vprDEBUG(gadgetDBG_RIM,vprDBG_VERB_LVL)
         << "[NetDevice Factory] Found the BaseType\n"<< vprDEBUG_FLUSH;
   }
   return new_dev;
}

int BaseTypeFactory::findConstructor(std::string base_type)
{
   //std::string element_type(element->getID());

   for ( unsigned int i = 0; i < mConstructors.size(); ++i )
   {
      // Get next constructor
      BaseTypeConstructorBase* construct = mConstructors[i];
      vprASSERT(construct != NULL);
      if(construct->getInputTypeName() == base_type)
      {
         return i;
      }
   }

   return -1;
}


void BaseTypeFactory::debugDump()
{
   vprDEBUG_OutputGuard(gadgetDBG_RIM, vprDBG_VERB_LVL,
      std::string("gadget::BaseTypeFactory::debugDump\n"),
      std::string("------ END DUMP ------\n"));

   vprDEBUG(gadgetDBG_RIM, vprDBG_VERB_LVL) << "num constructors:"
                             << mConstructors.size() << "\n"
                             << vprDEBUG_FLUSH;

   for ( unsigned int cNum = 0; cNum < mConstructors.size(); ++cNum )
   {
      BaseTypeConstructorBase* dev_constr = mConstructors[cNum];
      vprDEBUG(gadgetDBG_RIM, vprDBG_VERB_LVL)
         << cNum << ": Constructor:" << (void*)dev_constr
         << "   type:" << typeid(*dev_constr).name() << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_RIM, vprDBG_VERB_LVL) << "   recog:"
                                << dev_constr->getInputTypeName() << "\n"
                                << vprDEBUG_FLUSH;
   }
}

} // End of gadget namespace
