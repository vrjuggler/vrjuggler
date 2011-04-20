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
 * version 2.1 of the License, or (at your option) any later version.
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

#include "SdlJoystick.h"
#include "SdlRumbleEffect.h"
#include "SdlRumbleEffectConstant.h"
#include "SdlRumbleEffectPeriodic.h"
#include "SdlRumbleEffectRamp.h"

#include <string>
#include <iostream>

#include <vpr/System.h>

#include <gadget/gadgetParam.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

using gadget::RumbleEffectPtr;
using gadget::RumbleEffect;
using std::string;
using std::cout;
using std::endl;

extern "C"
{
   GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
   {
      return __GADGET_version;
   }
   GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager *inputMgr)
   {
      new gadget::DeviceConstructor<gadget::SdlJoystick>(inputMgr);
   }
}

namespace gadget
{

SdlJoystick::SdlJoystick()
   : mDeviceNumber(0)
   , mJoystick(0)
   , mHaptic(0)
   , mPause(false)
   , mInitialized(false)
{
   mButtons.push_back(gadget::DigitalState::OFF);
   mAxes.push_back(0.0f);
}

SdlJoystick::~SdlJoystick()
{
   stopSampling();
}

string SdlJoystick::getElementType()
{
   return "sdl_joystick";
}

bool SdlJoystick::config(jccl::ConfigElementPtr e)
{
   if (!gadget::Digital::config(e)) return false;
   if (!gadget::Analog::config(e))  return false;
   if (!gadget::Input::config(e))   return false;
   //if (!gadget::Rumble::config(e))  return false;

   mDeviceNumber = e->getProperty<int>("device");

   setMin(-32767.0f);
   setMax(32767.0f);

   return true;
}

void SdlJoystick::controlLoop()
{  
   if (mInitialized)
   {
      // Loop through and keep sampling until stopSampling is called.
      while(!mDone)
      {
         this->sample();

         // Limit sampling to 100 Hz
         vpr::System::msleep(10);
      }

#if SDL_VERSION_ATLEAST(1,3,0)
      if (mHaptic != NULL)
      {
         SDL_HapticClose(mHaptic);
         mHaptic = NULL;
      }
#endif

      if (mJoystick != NULL)
      {
         SDL_JoystickClose(mJoystick);
         mJoystick = NULL;
      }

      SDL_Quit();
   }
}

bool SdlJoystick::sample()
{
   SDL_JoystickUpdate();
   
   for (unsigned int i = 0 ; i < mButtons.size() ; ++i)
   {
      mButtons[i] = gadget::DigitalState::OFF;
      if (SDL_JoystickGetButton(mJoystick, i))
      {
        mButtons[i] = gadget::DigitalState::ON;
      }
      mButtons[i].setTime();
   }

   for (unsigned int i = 0 ; i < mAxes.size() ; ++i)
   {
      mAxes[i].setValue(SDL_JoystickGetAxis(mJoystick, i));
      mAxes[i].setTime();
   }

   addDigitalSample(mButtons);
   addAnalogSample(mAxes);

   return true;
}


bool SdlJoystick::startSampling()
{
   //return init();
   if (!init())
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " gadget::SdlJoystick::startSampling() SDL init failed.\n"
         << vprDEBUG_FLUSH;
      return false;
   }

   if(mThread != NULL)
   {
      vprDEBUG(vprDBG_ERROR,vprDBG_CRITICAL_LVL)
         << clrOutNORM(clrRED,"ERROR:")
         << " gadget::SdlJoystick::startSampling() called, when already "
         << "sampling.\n" << vprDEBUG_FLUSH;
      return false;
   }

   // Set flag that will later allow us to stop the control loop.
   mDone = false;

   bool started(true);

   // Create a new thread to handle the control
   try
   {
      mThread = new vpr::Thread(boost::bind(&SdlJoystick::controlLoop,
                                this));
   }
   catch (vpr::Exception& ex)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Failed to spawn thread for SDL Joystick driver!\n"
         << vprDEBUG_FLUSH;
      vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << ex.what() << std::endl << vprDEBUG_FLUSH;
      started = false;
   }

   return started;
}

bool SdlJoystick::stopSampling()
{
   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "[gadget::SdlJoystick::stopSampling()] Stopping the SDL Joystick "
         << "thread...\n" << vprDEBUG_FLUSH;

      // Set the done flag to attempt to cause the control loop to exit
      // naturally.
      mDone = true;

      mThread->join();
      delete mThread;
      mThread = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "*** SDL Joystick has been shutdown. ***" << std::endl
         << vprDEBUG_FLUSH;
   }

   return true;
}

void SdlJoystick::updateData()
{
   if (mInitialized)
   {
      swapAnalogBuffers();
      swapDigitalBuffers();
   }
}

bool SdlJoystick::init()
{
#if SDL_VERSION_ATLEAST(1,3,0)
   SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
#else
   SDL_Init(SDL_INIT_JOYSTICK);
#endif

   if (mDeviceNumber >= SDL_NumJoysticks())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << clrOutBOLD(clrRED, "ERROR")
         << ": Added for joystick number " << mDeviceNumber
         << " but there are only " << SDL_NumJoysticks() << " joysticks.\n"
         << vprDEBUG_FLUSH;
      return false;
   }
   mJoystick = SDL_JoystickOpen(mDeviceNumber);
   if (!mJoystick)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
         << "ERROR: Failed to open joystick " << mDeviceNumber << ": "
         << SDL_GetError();
      return false;
   }

   mButtons.resize(SDL_JoystickNumButtons(mJoystick), gadget::DigitalState::OFF);
   mAxes.resize(SDL_JoystickNumAxes(mJoystick), 0.0f);

   gadget::Digital::addDigitalSample(mButtons);
   gadget::Analog::addAnalogSample(mAxes);

   cout << "Found Joystick " << mDeviceNumber << ": " << SDL_JoystickName(mDeviceNumber) << endl
        << "              Axis: " << mAxes.size() << endl
        << "           Buttons: " << mButtons.size() << endl
        << "              Hats: " << SDL_JoystickNumHats(mJoystick) << " (Unused)" << endl
        << "             Balls: " << SDL_JoystickNumBalls(mJoystick) << " (Unused)" << endl
#if SDL_VERSION_ATLEAST(1,3,0)
        << "           Haptics: " << (SDL_JoystickIsHaptic(mJoystick) ? "YES" : "NO")
#else
        << "           Haptics: Not supported by SDL 1.2"
#endif
        << endl;

#if SDL_VERSION_ATLEAST(1,3,0)
   if (SDL_JoystickIsHaptic(mJoystick))
   {
      mHaptic = SDL_HapticOpenFromJoystick(mJoystick);
      if (!mHaptic)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "ERROR: Failed to initialize haptics on " << mDeviceNumber << ": " << SDL_GetError();
         return true;
      }
   unsigned int cap = getCapabilities();
   cout << "    Loaded Effects: " << getMaxStoredEffects() << endl
        << "    Played Effects: " << getMaxPlayingEffects() << endl
        << "        Axes Count: " << getNumAxes() << endl
        << "       Support for:";
   if (cap & RumbleEffect::CONSTANT)     cout << " constant";
   if (cap & RumbleEffect::SINE)         cout << " sine";
   if (cap & RumbleEffect::SQUARE)       cout << " square";
   if (cap & RumbleEffect::TRIANGLE)     cout << " triangle";
   if (cap & RumbleEffect::SAWTOOTHUP)   cout << " saw-tooth-up";
   if (cap & RumbleEffect::SAWTOOTHDOWN) cout << " saw-tooth-down";
   if (cap & RumbleEffect::RAMP)         cout << " ramp";
   if (cap & RumbleEffect::SPRING)       cout << " spring";
   if (cap & RumbleEffect::DAMPER)       cout << " damper";
   if (cap & RumbleEffect::INERTIA)      cout << " inertia";
   if (cap & RumbleEffect::FRICTION)     cout << " friction";
   if (cap & RumbleEffect::CUSTOM)       cout << " custom";
   if (cap & RumbleEffect::GAIN)         cout << " gain";
   if (cap & RumbleEffect::AUTOCENTER)   cout << " auto-center";
   if (cap & RumbleEffect::STATUS)       cout << " status";
   if (cap & RumbleEffect::PAUSE)        cout << " pause";
   cout << endl;
   }
#endif
   mInitialized = true;
   return true;
}

unsigned int SdlJoystick::getCapabilities()
{
#if SDL_VERSION_ATLEAST(1,3,0)
   if (mHaptic) return SDL_HapticQuery(mHaptic);
#endif
   return 0;
}

int SdlJoystick::getNumAxes()
{
#if SDL_VERSION_ATLEAST(1,3,0)
   if (mHaptic) return SDL_HapticNumAxes(mHaptic);
#endif
   return 0;
}

int SdlJoystick::getMaxStoredEffects()
{
#if SDL_VERSION_ATLEAST(1,3,0)
   if (mHaptic) return SDL_HapticNumEffects(mHaptic);
#endif
   return 0;
}

int SdlJoystick::getMaxPlayingEffects()
{
#if SDL_VERSION_ATLEAST(1,3,0)
   if (mHaptic) return SDL_HapticNumEffectsPlaying(mHaptic);
#endif
   return 0;
}

bool SdlJoystick::stopAllEffects()
{
#if SDL_VERSION_ATLEAST(1,3,0)
   if (mHaptic) return (SDL_HapticStopAll(mHaptic) == 0);
#endif
   return false;
}

bool SdlJoystick::setPause(const bool pause)
{
#if SDL_VERSION_ATLEAST(1,3,0)
   if (mPause == pause) return true;

   bool ret = false;
   if (mHaptic && mPause != pause)
   {
      if (mPause)
         ret = (SDL_HapticUnpause(mHaptic) == 0);
      else
         ret = (SDL_HapticPause(mHaptic) == 0);
      if (ret) mPause = !mPause;
   }
   return ret;
#else
   mPause = pause;
   return true;
#endif
}

bool SdlJoystick::getPaused()
{
   return mPause;
}

RumbleEffectPtr SdlJoystick::createEffectImp(RumbleEffect::RumbleType type)
{
   boost::ignore_unused_variable_warning(type);
   RumbleEffectPtr ret;
#if SDL_VERSION_ATLEAST(1,3,0)
   bool available = (getCapabilities() & type);
   switch (type)
   {
   case RumbleEffect::CONSTANT:
      if (available)
         ret.reset(new SdlRumbleEffectConstant(mHaptic));
      break;
   case RumbleEffect::SINE:
      if (available)
         ret.reset(new SdlRumbleEffectPeriodic(mHaptic, type));
      break;
   case RumbleEffect::SQUARE:
   case RumbleEffect::TRIANGLE:
   case RumbleEffect::SAWTOOTHUP:
   case RumbleEffect::SAWTOOTHDOWN:
      if (available)
         ret.reset(new SdlRumbleEffectPeriodic(mHaptic, type));
      else // See if we can get a SINE Effect.
         //ret = createEffect(RumbleEffect::SINE);
      break;
   case RumbleEffect::RAMP:
      if (available)
         ret.reset(new SdlRumbleEffectConstant(mHaptic));
      break;
   case RumbleEffect::SPRING:
      break;
   case RumbleEffect::DAMPER:
      break;
   case RumbleEffect::INERTIA:
      break;
   case RumbleEffect::FRICTION:
      break;
   case RumbleEffect::CUSTOM:
      break;
   default:
      break;
   }
#endif
   return ret;
}

} // End of gadget namespace
