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

#ifndef _SDL_JOYSTICK_H_
#define _SDL_JOYSTICK_H_

#include <gadget/Devices/DriverConfig.h>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Rumble.h>
#include <gadget/InputManager.h>
#include <vpr/Thread/Thread.h>

#include <SDL/SDL.h>

#if !SDL_VERSION_ATLEAST(1,2,0)
#  error "SDL Version 1.2 is required.  Try configure.pl --with-sdl=no"
#endif

#if SDL_VERSION_ATLEAST(1,3,0)
#  include <SDL/SDL_haptic.h>
#else
#  define SDL_Haptic void
#endif

namespace gadget
{

class SdlJoystick
   : public gadget::input_digital_analog_rumble_t
{
public:
   SdlJoystick();
   virtual ~SdlJoystick();
   static std::string getElementType();

   virtual bool config(jccl::ConfigElementPtr e);

   /** Main thread of control for this active object. */
   void controlLoop();

   virtual bool sample();
   virtual bool startSampling();
   virtual bool stopSampling();
   virtual void updateData();

   virtual const DigitalData getDigitalData(int devNum=0)
   {
      return (mButtons[devNum]);
   }

   virtual const AnalogData getAnalogData(int devNum=0)
   {
      return (mAxes[devNum]);
   }

   bool init();

   /**
    * Gets the supported capabilities in a bitmask.
    *
    * @return bitmask defined in Rumble::RumbleType
    *
    */
   virtual unsigned int getCapabilities();
    
   
   /**
    * Gets the number of directional axes supported by device.
    *
    * @return integer number of axes.
    *
    */
   virtual int getNumAxes();
   
   /**
    * Gets the number of effects the device can store.
    *
    * @return integer number of effects or -1 for error.
    *
    */
   virtual int getMaxStoredEffects();
   
   /**
    * Gets the number of effects the device can play simultaneously.
    *
    * @return integer number of effects or -1 for error.
    *
    */
   virtual int getMaxPlayingEffects();

   /**
    * Stop all effects.
    *
    * @return true for success.
    *
    */
   virtual bool stopAllEffects();
   
   /**
    * Pause/resume effect playback.
    *
    * @return true for success.
    *
    * @param pause True to pause, false to un-pause
    *
    */
   virtual bool setPause(const bool pause);
   
   /**
    * Get pause state.
    *
    * @return true for paused, false for un-paused.
    *
    */
   virtual bool getPaused();
   
   // Less important methods?
   // Replaced by effect->status()
   // int getEffectStatus(int effect_id);  // playing, not playing, error?

   // For implementation later.
   // bool setGain(int gain);  // gain from 0 to 100 to magnify effects?
   // bool setAutoCenter(int autocenter); // value from 0 to 100 meaning??

protected:
   virtual gadget::RumbleEffectPtr createEffectImp(gadget::RumbleEffect::RumbleType type);

private:
   int mDeviceNumber;   ///< What device should we connect too.
   SDL_Joystick *mJoystick;
   SDL_Haptic *mHaptic;
   bool mPause;
   bool mDone;
   bool mInitialized;

   std::vector<gadget::DigitalData> mButtons;
   std::vector<gadget::AnalogData> mAxes;
};

} // End of gadget namespace

#endif
