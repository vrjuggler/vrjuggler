/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <vrj/vrjConfig.h>
#include <vrj/Input/Type/DeviceFactory.h>

// Sims
#include <vrj/Input/Devices/Sim/SimAnalog.h>
#include <vrj/Input/Devices/Sim/SimDigital.h>
#include <vrj/Input/Devices/Sim/SimPosition.h>
#include <vrj/Input/Devices/Sim/SimGloveGesture.h>
//#include <vrj/Input/Devices/Sim/SimKeyboardDigital.h>
#include <vrj/Input/Devices/Sim/SimRelativePosition.h>
#include <vrj/Input/Devices/Sim/SimDigitalGlove.h>

/* Physical devices */
#ifndef WIN32
#   include <vrj/Input/Devices/Ascension/Flock.h>
#   include <vrj/Input/Devices/Intersense/Intersense.h>

#   ifdef VJ_OS_Darwin
#      include <vrj/Input/Devices/Keyboard/OSXKeyboard.h>
#   else
#      include <vrj/Input/Devices/Immersion/Ibox.h>
#      include <vrj/Input/Devices/VirtualTechnologies/CyberGlove.h>
#      include <vrj/Input/Devices/Fakespace/PinchGlove.h>
#      include <vrj/Input/Devices/Keyboard/KeyboardXWin.h>
#      include <vrj/Input/Devices/Keyboard/KeyboardDepCheckerXWin.h>
#      include <vrj/Input/Devices/Open/Trackd/TrackdController.h>
#      include <vrj/Input/Devices/Open/Trackd/TrackdSensor.h>
#   endif

#   include <vrj/Input/Devices/Logitech/logiclass.h>
#   include <vrj/Input/Devices/Ascension/MotionStar.h>
#else
#   include <vrj/Input/Devices/Keyboard/KeyboardWin32.h>
#endif

/* PThread Dependant Driver */
#ifdef VJ_HAVE_DTK
#   include <vrj/Input/Devices/Open/DTK/DTK.h>
#endif

#include <typeinfo>

namespace vrj
{

// Initialize the singleton ptr
//vjDeviceFactory* DeviceFactory::mInstance = NULL;
//vjSingletonImp( DeviceFactory ); //kevin
vprSingletonImpWithInitFunc( DeviceFactory, hackLoadKnownDevices );

template <class DEV>
DeviceConstructor<DEV>::DeviceConstructor()
{
   vprASSERT(DeviceFactory::instance() != NULL);
   DeviceFactory::instance()->registerDevice(this);
}


// Register all the devices that I know about
//!NOTE: This should really be moved to dynamic library loading code.
void DeviceFactory::hackLoadKnownDevices()
{
   // NOTE: These will all given unused variable errors in compiling.
   // That is okay, because the don't actually have to do anything.
   // They just register themselves in their constructor.
   DeviceConstructor<SimAnalog>* sim_analog = new DeviceConstructor<SimAnalog>;
   DeviceConstructor<SimDigital>* sim_digital = new DeviceConstructor<SimDigital>;
   DeviceConstructor<SimPosition>* sim_position = new DeviceConstructor<SimPosition>;
   DeviceConstructor<SimGloveGesture>* sim_glove = new DeviceConstructor<SimGloveGesture>;
   //vjDeviceConstructor<SimKeyboardDigital>* sim_keyboard_digital = new DeviceConstructor<SimKeyboardDigital>;
   DeviceConstructor<SimRelativePosition>* sim_relative = new DeviceConstructor<SimRelativePosition>;
   DeviceConstructor<SimDigitalGlove>* simpinch_glove = new DeviceConstructor<SimDigitalGlove>;

   if( (NULL == sim_analog)   ||
       (NULL == sim_digital)  ||
       (NULL == sim_position) ||
       (NULL == sim_glove)    ||
       (NULL == sim_relative) ||
       (NULL == simpinch_glove))
   {
      vjDEBUG(vjDBG_ALL,vjDBG_ERROR) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vjDEBUG_FLUSH;
   }

#ifndef WIN32
#ifdef VJ_OS_Darwin
   DeviceConstructor<OSXKeyboard>* osx_keyboard = new DeviceConstructor<OSXKeyboard>;
   if( (NULL == osx_keyboard) )
   {
      vjDEBUG(vjDBG_ALL,vjDBG_ERROR) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vjDEBUG_FLUSH;
   }

#else
   DeviceConstructor<TrackdSensor>* trackd_sensor = new DeviceConstructor<TrackdSensor>;
   DeviceConstructor<TrackdController>* trackd_controller = new DeviceConstructor<TrackdController>;
   DeviceConstructor<IBox>* ibox = new DeviceConstructor<IBox>;
   DeviceConstructor<PinchGlove>* pinch_glove = new DeviceConstructor<PinchGlove>;
   DeviceConstructor<CyberGlove>* cyber_glove = new DeviceConstructor<CyberGlove>;
   DeviceConstructor<KeyboardXWin>* xwin_key = new DeviceConstructor<KeyboardXWin>;
   DependencyManager::instance()->registerChecker(new KeyboardDepCheckerXWin());
   DeviceConstructor<ThreeDMouse>* threed_mouse = new DeviceConstructor<ThreeDMouse>;
   if( (NULL == trackd_sensor)      ||
       (NULL == trackd_controller)  ||
       (NULL == ibox)         ||
       (NULL == pinch_glove)  ||
       (NULL == cyber_glove)  ||
       (NULL == xwin_key)     ||
       (NULL == threed_mouse))
   {
      vjDEBUG(vjDBG_ALL,vjDBG_ERROR) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vjDEBUG_FLUSH;
   }

#endif
   DeviceConstructor<Flock>* flock = new DeviceConstructor<Flock>;
   DeviceConstructor<Intersense>* intersense = new DeviceConstructor<Intersense>;
   DeviceConstructor<MotionStar>* motion_star = new DeviceConstructor<MotionStar>;

   if( (NULL == flock)        ||
       (NULL == intersense)   ||
       (NULL == motion_star)   )
   {
      vjDEBUG(vjDBG_ALL,vjDBG_ERROR) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vjDEBUG_FLUSH;
   }
#else

   DeviceConstructor<KeyboardWin32>* key_win32 = new DeviceConstructor<KeyboardWin32>;
   if( (NULL == key_win32))
   {
      vjDEBUG(vjDBG_ALL,vjDBG_ERROR) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vjDEBUG_FLUSH;
   }

#endif
#ifdef VJ_HAVE_DTK
   DeviceConstructor<DTK>* dtk_wrapper = new DeviceConstructor<DTK>;
   if( (NULL == dtk_wrapper))
   {
      vjDEBUG(vjDBG_ALL,vjDBG_ERROR) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vjDEBUG_FLUSH;
   }
#endif
}

void DeviceFactory::registerDevice(DeviceConstructorBase* constructor)
{
   vprASSERT(constructor != NULL);
   mConstructors.push_back(constructor);     // Add the constructor to the list
   vjDEBUG(vjDBG_INPUT_MGR,1) << "vjDeviceFactory: Registered: "
              << std::setiosflags(std::ios::right) << std::setw(25) << std::setfill(' ') << constructor->getChunkType() << std::setiosflags(std::ios::right)
              //<< "   :" << (void*)constructor
              << "  type: " << typeid(*constructor).name() << std::endl
              << vjDEBUG_FLUSH;
}

// Simply query all device constructors registered looking
// for one that knows how to load the device
bool DeviceFactory::recognizeDevice(ConfigChunk* chunk)
{
   if(findConstructor(chunk) == -1)
      return false;
   else
      return true;
}

//: Load the specified device
//!PRE: recognizeDevice(chunk) == true
//!ARGS: chunk - specification of the device to load
//!RETURNS: null - Device failed to load
//+         other - Pointer to the loaded device
Input* DeviceFactory::loadDevice(ConfigChunk* chunk)
{
   vprASSERT(recognizeDevice(chunk));

   int index = findConstructor(chunk);

   Input* new_dev;
   DeviceConstructorBase* constructor = mConstructors[index];

   vjDEBUG(vjDBG_INPUT_MGR,3) << "vjDeviceFactory::loadDevice: Loading device: "
              << chunk->getType() << "  with: "
              << typeid(*constructor).name() << std::endl << vjDEBUG_FLUSH;

   new_dev = constructor->createDevice(chunk);
   return new_dev;
}

int DeviceFactory::findConstructor(ConfigChunk* chunk)
{
   std::string chunk_type;
   chunk_type = (std::string)chunk->getType();

   for(unsigned int i=0;i<mConstructors.size();i++)
   {
      // Get next constructor
      DeviceConstructorBase* construct = mConstructors[i];
      vprASSERT(construct != NULL);

      if(construct->getChunkType() == chunk_type)
         return i;
   }

   return -1;
}


void DeviceFactory::debugDump()
{
   vjDEBUG_BEGIN(vjDBG_INPUT_MGR,0) << "vjDeviceFactory::debugDump\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_INPUT_MGR,0) << "num constructors:" << mConstructors.size() << "\n" << vjDEBUG_FLUSH;

   for(unsigned int cNum=0;cNum<mConstructors.size();cNum++)
   {
      DeviceConstructorBase* dev_constr = mConstructors[cNum];
      vjDEBUG(vjDBG_INPUT_MGR,0) << cNum << ": Constructor:" << (void*)dev_constr
                 << "   type:" << typeid(*dev_constr).name() << "\n" << vjDEBUG_FLUSH;
      vjDEBUG(vjDBG_INPUT_MGR,0) << "   recog:" << dev_constr->getChunkType() << "\n" << vjDEBUG_FLUSH;
   }

   vjDEBUG_END(vjDBG_INPUT_MGR,0) << "------ END DUMP ------\n" << vjDEBUG_FLUSH;
}

};
