/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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
#include <vpr/vpr.h>
#include <gadget/Type/DeviceFactory.h>

// Platform-independent devices.
#include <gadget/Devices/Ascension/Flock.h>
#include <gadget/Devices/Ascension/MotionStar.h>

// XXX: Undo this once the Intersense driver is ported to VPR serial ports.
#if !defined(VPR_OS_FreeBSD) && !defined(VPR_OS_Darwin)
#include <gadget/Devices/Intersense/Intersense.h>
#endif

#include <gadget/Devices/Immersion/Ibox.h>
#include <gadget/Devices/Fakespace/PinchGlove.h>
#include <gadget/Devices/5DT/DataGlove.h>
#include <gadget/Devices/Polhemus/Fastrack.h>

// Sims
#include <gadget/Devices/Sim/SimAnalog.h>
#include <gadget/Devices/Sim/SimDigital.h>
#include <gadget/Devices/Sim/SimPosition.h>
#include <gadget/Devices/Sim/SimGloveGesture.h>
//#include <gadget/Devices/Sim/SimKeyboardDigital.h>
#include <gadget/Devices/Sim/SimRelativePosition.h>
#include <gadget/Devices/Sim/SimSetablePosition.h>
#include <gadget/Devices/Sim/SimDigitalGlove.h>

/* Physical devices */
#ifndef VPR_OS_Win32

#   ifdef VPR_OS_Darwin
#      include <gadget/Devices/Keyboard/KeyboardOSX.h>
#   else
//#      include <gadget/Devices/VirtualTechnologies/CyberGlove.h>
#      include <gadget/Devices/Keyboard/KeyboardXWin.h>
#      include <gadget/Devices/Keyboard/KeyboardDepCheckerXWin.h>
#      include <gadget/Devices/Open/Trackd/TrackdController.h>
#      include <gadget/Devices/Open/Trackd/TrackdSensor.h>
#   endif

#   include <gadget/Devices/Logitech/logiclass.h>
#else
#   include <gadget/Devices/Keyboard/KeyboardWin32.h>
#endif

/* PThread Dependant Driver */
#ifdef GADGET_HAVE_DTK
#   include <gadget/Devices/Open/DTK/DTK.h>
#endif

#include <typeinfo>

namespace gadget
{

// Initialize the singleton ptr
//vjDeviceFactory* DeviceFactory::mInstance = NULL;
//vjSingletonImp( DeviceFactory ); //kevin
vprSingletonImpWithInitFunc( DeviceFactory, hackLoadKnownDevices );

/**
 * Registers all the devices that I know about.
 * @note This should really be moved to dynamic library loading code.
 */
void DeviceFactory::hackLoadKnownDevices()
{
   // NOTE: These will all given unused variable errors in compiling.
   // That is okay, because the don't actually have to do anything.
   // They just register themselves in their constructor.

   // Platform-independent devices.
   DeviceConstructor<Flock>* flock = new DeviceConstructor<Flock>;
   DeviceConstructor<MotionStar>* motion_star = new DeviceConstructor<MotionStar>;

// XXX: Undo this once the Intersense driver is ported to VPR serial ports.
#if !defined(VPR_OS_FreeBSD) && !defined(VPR_OS_Darwin)
   DeviceConstructor<Intersense>* intersense = new DeviceConstructor<Intersense>;
#endif

   DeviceConstructor<IBox>* ibox = new DeviceConstructor<IBox>;
   DeviceConstructor<PinchGlove>* pinch_glove = new DeviceConstructor<PinchGlove>;
   DeviceConstructor<DataGlove>* data_glove = new DeviceConstructor<DataGlove>;
   DeviceConstructor<Fastrack>* fastrack = new DeviceConstructor<Fastrack>;

   if( (NULL == flock)        ||
// XXX: Undo this once the Intersense driver is ported to VPR serial ports.
#if !defined(VPR_OS_FreeBSD) && !defined(VPR_OS_Darwin)
       (NULL == intersense)   ||
#endif
       (NULL == ibox)         ||
       (NULL == pinch_glove)  ||
       (NULL == data_glove)   ||
       (NULL == fastrack)     ||
       (NULL == motion_star)   )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vprDEBUG_FLUSH;
   }

   DeviceConstructor<SimAnalog>* sim_analog = new DeviceConstructor<SimAnalog>;
   DeviceConstructor<SimDigital>* sim_digital = new DeviceConstructor<SimDigital>;
   DeviceConstructor<SimPosition>* sim_position = new DeviceConstructor<SimPosition>;
   //DeviceConstructor<SimKeyboardDigital>* sim_keyboard_digital = new DeviceConstructor<SimKeyboardDigital>;
   DeviceConstructor<SimSetablePosition>* sim_setable = new DeviceConstructor<SimSetablePosition>;
   DeviceConstructor<SimRelativePosition>* sim_relative = new DeviceConstructor<SimRelativePosition>;

   DeviceConstructor<SimGloveGesture>* sim_glove = new DeviceConstructor<SimGloveGesture>;
   DeviceConstructor<SimDigitalGlove>* simpinch_glove = new DeviceConstructor<SimDigitalGlove>;

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

#ifndef VPR_OS_Win32
#ifdef VPR_OS_Darwin
   DeviceConstructor<KeyboardOSX>* osx_keyboard = new DeviceConstructor<KeyboardOSX>;
   if( (NULL == osx_keyboard) )
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vprDEBUG_FLUSH;
   }

#else
   DeviceConstructor<TrackdSensor>* trackd_sensor = new DeviceConstructor<TrackdSensor>;
   DeviceConstructor<TrackdController>* trackd_controller = new DeviceConstructor<TrackdController>;
//   DeviceConstructor<CyberGlove>* cyber_glove = new DeviceConstructor<CyberGlove>;
   DeviceConstructor<KeyboardXWin>* xwin_key = new DeviceConstructor<KeyboardXWin>;
   jccl::DependencyManager::instance()->registerChecker(new KeyboardDepCheckerXWin());
   DeviceConstructor<ThreeDMouse>* threed_mouse = new DeviceConstructor<ThreeDMouse>;
   if( (NULL == trackd_sensor)      ||
       (NULL == trackd_controller)  ||
       (NULL == data_glove)   ||
//       (NULL == cyber_glove)  ||
       (NULL == xwin_key)     ||
       (NULL == threed_mouse))
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vprDEBUG_FLUSH;
   }

#endif
#else

   DeviceConstructor<KeyboardWin32>* key_win32 = new DeviceConstructor<KeyboardWin32>;
   if( (NULL == key_win32))
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vprDEBUG_FLUSH;
   }

#endif
#ifdef GADGET_HAVE_DTK
   DeviceConstructor<DTK>* dtk_wrapper = new DeviceConstructor<DTK>;
   if( (NULL == dtk_wrapper))
   {
      vprDEBUG(vprDBG_ALL,vprDBG_CRITICAL_LVL) << clrOutBOLD(clrRED,"ERROR:") << "Failed to load a known device\n" << vprDEBUG_FLUSH;
   }
#endif
}

void DeviceFactory::registerDevice(DeviceConstructorBase* constructor)
{
   vprASSERT(constructor != NULL);
   mConstructors.push_back(constructor);     // Add the constructor to the list
   vprDEBUG(gadgetDBG_INPUT_MGR,1)
      << "gadget::DeviceFactory: Registered: "
      << std::setiosflags(std::ios::right) << std::setw(25)
      << std::setfill(' ') << constructor->getChunkType()
      << std::setiosflags(std::ios::right)
      //<< "   :" << (void*)constructor
      << "  type: " << typeid(*constructor).name() << std::endl
      << vprDEBUG_FLUSH;
}

// Simply query all device constructors registered looking
// for one that knows how to load the device
bool DeviceFactory::recognizeDevice(jccl::ConfigChunkPtr chunk)
{
   if(findConstructor(chunk) == -1)
      return false;
   else
      return true;
}

/**
 * Loads the specified device.
 */
Input* DeviceFactory::loadDevice(jccl::ConfigChunkPtr chunk)
{
   vprASSERT(recognizeDevice(chunk));

   int index = findConstructor(chunk);

   Input* new_dev;
   DeviceConstructorBase* constructor = mConstructors[index];

   vprDEBUG(gadgetDBG_INPUT_MGR,3)
      << "gadget::DeviceFactory::loadDevice: Loading device: "
      << chunk->getDescToken() << "  with: " << typeid(*constructor).name()
      << std::endl << vprDEBUG_FLUSH;

   new_dev = constructor->createDevice(chunk);
   return new_dev;
}

int DeviceFactory::findConstructor(jccl::ConfigChunkPtr chunk)
{
   std::string chunk_type;
   chunk_type = chunk->getDescToken();

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
   vprDEBUG_OutputGuard(gadgetDBG_INPUT_MGR, 0,
      std::string("gadget::DeviceFactory::debugDump\n"),
      std::string("------ END DUMP ------\n"));
   vprDEBUG(gadgetDBG_INPUT_MGR,0) << "num constructors:"
                                   << mConstructors.size() << "\n"
                                   << vprDEBUG_FLUSH;

   for(unsigned int cNum=0;cNum<mConstructors.size();cNum++)
   {
      DeviceConstructorBase* dev_constr = mConstructors[cNum];
      vprDEBUG(gadgetDBG_INPUT_MGR,0)
         << cNum << ": Constructor:" << (void*)dev_constr
         << "   type:" << typeid(*dev_constr).name() << "\n" << vprDEBUG_FLUSH;
      vprDEBUG(gadgetDBG_INPUT_MGR,0) << "   recog:"
                                      << dev_constr->getChunkType() << "\n"
                                      << vprDEBUG_FLUSH;
   }
}

};
