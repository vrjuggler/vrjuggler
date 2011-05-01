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

#ifndef _SDL_RUMBLE_EFFECT_H_
#define _SDL_RUMBLE_EFFECT_H_

#include <SDL/SDL.h>
#if SDL_VERSION_ATLEAST(1,3,0)

#include <gadget/Type/RumbleEffect.h>
#include <SDL/SDL_haptic.h>

class SdlRumbleEffect
    : public gadget::RumbleEffect
{
public:
   SdlRumbleEffect(SDL_Haptic *haptic, gadget::RumbleEffect::RumbleType type);
   virtual ~SdlRumbleEffect();

   /**
    * Load this effect onto device.
    *
    * @return true for success.
    *
    */
   virtual bool load();

   /**
    * Update existing effect on device.
    *
    * @return true for success.
    *
    */
   virtual bool update();

   /**
    * Delete existing effect on device.
    *
    * @return true for success.
    *
    */
   virtual bool unload();
    
   /**
    * Play an effect on device.
    *
    * @return true for success.
    *
    * @param num_repeats Number of times to repeat effect, or -1 for infinity
    *
    */
   virtual bool play(int num_repeats);

   /**
    * Stop playing an effect.
    *  
    * @return true for success.
    *
    */
   virtual bool stop();

   /** Gets the status of the current effect on the haptic device.
    *
    * @return 0 if it isn't playing, 1 if it is playing and -1 on error.
    */
   virtual int status();
    
   /** 
    * Checks if the effect is loaded.
    *
    * @return true if effect is loaded.
    *
    */
   virtual bool isLoaded();

   /**
    * Returns the type of the effect.
    *
    */
   virtual gadget::RumbleEffect::RumbleType getType();

protected:
   SDL_Haptic *mHaptic;      ///< Pointer to an active SDL Haptic Class.
   int mId;                  ///< ID of the effect in mHaptic
   SDL_HapticEffect mEffect; ///< The Haptic Effect

   bool isActive() { return mId >= 0; }

   static const Uint16 MAX_FORCE = 32767; ///< The largest value for level values.
   /// The largest value used for SDL_HapticDirection.dir
   static const Uint32 MAX_DIRECTION = 32767; 
};

#endif
#endif
