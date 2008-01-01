/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#include <gadget/gadgetConfig.h>

#include <iomanip>
#include <typeinfo>

#include <vpr/vpr.h>
#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceFactory.h>
#include <gadget/Type/DeviceConstructor.h>

// Sims
#include <gadget/Devices/Sim/SimAnalog.h>
#include <gadget/Devices/Sim/SimDigital.h>
#include <gadget/Devices/Sim/SimPosition.h>
#include <gadget/Devices/Sim/SimGloveGesture.h>
//#include <gadget/Devices/Sim/SimKeyboardDigital.h>
#include <gadget/Devices/Sim/SimRelativePosition.h>
#include <gadget/Devices/Sim/SimSetablePosition.h>
#include <gadget/Devices/Sim/SimDigitalGlove.h>
#include <gadget/Devices/KeyboardMouseDevice/KeyboardMouseDevice.h>

#if defined(VPR_OS_Windows)
#  include <gadget/Devices/KeyboardMouseDevice/InputWindowWin32.h>
#elif defined(VPR_OS_Darwin) && ! defined(GADGET_USE_X11)
#  include <gadget/Devices/KeyboardMouseDevice/InputWindowCocoa.h>
#else
//#  include <jccl/RTRC/DependencyManager.h>
#  include <gadget/Devices/KeyboardMouseDevice/InputWindowXWin.h>
#endif

/* Physical devices */
#ifdef STATIC_DRIVERS

/* PThread Dependant Driver */
#ifdef GADGET_HAVE_DTK
#   include <gadget/Devices/Open/DTK/DTK.h>
#endif

#endif /* STATIC_DRIVERS */

#include <gadget/Util/Debug.h>

namespace gadget
{

// Initialize the singleton ptr
vprSingletonImpWithInitFunc(DeviceFactory, loadKnownDevices);

DeviceFactory::~DeviceFactory()
{
   typedef std::vector<DeviceConstructorBase*>::iterator iter_type;
   for ( iter_type itr = mConstructors.begin(); itr != mConstructors.end(); ++itr )
   {
      if (NULL != *itr)
      {
         delete *itr;
         *itr = NULL;
      }
   }
   mConstructors.clear();
}

/**
 * Registers all the devices that I know about.
 */
void DeviceFactory::loadKnownDevices()
{
   gadget::InputManager* input_mgr = gadget::InputManager::instance();

#ifdef STATIC_DRIVERS

#ifdef GADGET_HAVE_DTK
   DeviceConstructor<DTK>* dtk_wrapper = new DeviceConstructor<DTK>;
   if( (NULL == dtk_wrapper))
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vprDEBUG_FLUSH;
   }
#endif

#endif /* STATIC_DRIVERS */

   // XXX: Memory leaks!
   DeviceConstructor<SimAnalog>* sim_analog = new DeviceConstructor<SimAnalog>(input_mgr);
   DeviceConstructor<SimDigital>* sim_digital = new DeviceConstructor<SimDigital>(input_mgr);
   DeviceConstructor<SimPosition>* sim_position = new DeviceConstructor<SimPosition>(input_mgr);
   //DeviceConstructor<SimKeyboardDigital>* sim_keyboard_digital = new DeviceConstructor<SimKeyboardDigital>(input_mgr);
   DeviceConstructor<SimSetablePosition>* sim_setable = new DeviceConstructor<SimSetablePosition>(input_mgr);
   DeviceConstructor<SimRelativePosition>* sim_relative = new DeviceConstructor<SimRelativePosition>(input_mgr);

   DeviceConstructor<SimGloveGesture>* sim_glove = new DeviceConstructor<SimGloveGesture>(input_mgr);
   DeviceConstructor<SimDigitalGlove>* simpinch_glove = new DeviceConstructor<SimDigitalGlove>(input_mgr);

   if( (NULL == sim_analog)   ||
       (NULL == sim_digital)  ||
       (NULL == sim_position) ||
       (NULL == sim_glove)    ||
       (NULL == simpinch_glove) ||
       (NULL == sim_setable) ||
       (NULL == sim_relative) )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vprDEBUG_FLUSH;
   }

   DeviceConstructor<KeyboardMouseDevice>* keyboard_device =
      new DeviceConstructor<KeyboardMouseDevice>(input_mgr);
   if( (NULL == keyboard_device))
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED,"ERROR:") << "Failed to load the known device KeyboardMouseDevice."
         << std::endl << vprDEBUG_FLUSH;
   }
#if defined(VPR_OS_Windows)
   DeviceConstructor<InputWindowWin32>* key_win32 =
      new DeviceConstructor<InputWindowWin32>(input_mgr);
   if( (NULL == key_win32))
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n"
         << vprDEBUG_FLUSH;
   }
#elif defined(VPR_OS_Darwin) && ! defined(GADGET_USE_X11)
   DeviceConstructor<InputWindowCocoa>* osx_keyboard =
      new DeviceConstructor<InputWindowCocoa>(input_mgr);
   if( (NULL == osx_keyboard) )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n"
         << vprDEBUG_FLUSH;
   }
#else
   DeviceConstructor<InputWindowXWin>* xwin_key =
      new DeviceConstructor<InputWindowXWin>(input_mgr);
   if( (NULL == xwin_key) )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n"
         << vprDEBUG_FLUSH;
   }
#endif

}

void DeviceFactory::registerDevice(DeviceConstructorBase* constructor)
{
   vprASSERT(constructor != NULL);
   mConstructors.push_back(constructor);     // Add the constructor to the list
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "gadget::DeviceFactory: Registered: "
      << std::setiosflags(std::ios::right) << std::setw(25)
      << std::setfill(' ') << constructor->getElementType()
      << std::setiosflags(std::ios::right)
      //<< "   :" << (void*)constructor
      << "  type: " << typeid(*constructor).name() << std::endl
      << vprDEBUG_FLUSH;
}

// Simply query all device constructors registered looking
// for one that knows how to load the device
bool DeviceFactory::recognizeDevice(jccl::ConfigElementPtr element) const
{
   return ! (findConstructor(element) == -1);
}

/**
 * Loads the specified device.
 */
Input* DeviceFactory::loadDevice(jccl::ConfigElementPtr element)
{
   vprASSERT(recognizeDevice(element));

   int index(findConstructor(element));

   Input* new_dev;
   DeviceConstructorBase* constructor = mConstructors[index];

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "gadget::DeviceFactory::loadDevice: Loading device: "
      << element->getID() << "  with: " << typeid(*constructor).name()
      << std::endl << vprDEBUG_FLUSH;

   new_dev = constructor->createDevice(element);
   return new_dev;
}

int DeviceFactory::findConstructor(jccl::ConfigElementPtr element) const
{
   const std::string element_type(element->getID());

   for ( unsigned int i = 0; i < mConstructors.size(); ++i )
   {
      // Get next constructor
      DeviceConstructorBase* construct = mConstructors[i];
      vprASSERT(construct != NULL);

      if(construct->getElementType() == element_type)
      {
         return i;
      }
   }

   return -1;
}


void DeviceFactory::debugDump()
{
   vprDEBUG_OutputGuard(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL,
      std::string("gadget::DeviceFactory::debugDump\n"),
      std::string("------ END DUMP ------\n"));
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL) << "num constructors:"
                                   << mConstructors.size() << "\n"
                                   << vprDEBUG_FLUSH;

   for(unsigned int cNum=0;cNum<mConstructors.size();cNum++)
   {
      DeviceConstructorBase* dev_constr = mConstructors[cNum];
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
         << cNum << ": Constructor:" << (void*)dev_constr
         << "   type:" << typeid(*dev_constr).name() << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
         << "   recog:" << dev_constr->getElementType() << "\n"
         << vprDEBUG_FLUSH;
   }
}

} // End of gadget namespace
