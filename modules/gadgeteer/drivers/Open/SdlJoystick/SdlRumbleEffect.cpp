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

#include "SdlRumbleEffect.h"
#include <memory.h>
#include <SDL/SDL.h>
#if SDL_VERSION_ATLEAST(1,3,0)

SdlRumbleEffect::SdlRumbleEffect(SDL_Haptic *haptic,
                                 gadget::RumbleEffect::RumbleType type)
   : mHaptic(haptic)
   , mId(-1)
{
   memset(&mEffect, 0, sizeof(SDL_HapticEffect));
   mEffect.type = type;
}

SdlRumbleEffect::~SdlRumbleEffect()
{
   unload();
}

bool SdlRumbleEffect::load()
{
   mId = SDL_HapticNewEffect(mHaptic, &mEffect);
   return isActive();
}

bool SdlRumbleEffect::update()
{
   if (isActive())
   {
      mId = SDL_HapticUpdateEffect(mHaptic, mId, &mEffect);
   }
   return isActive();
}

bool SdlRumbleEffect::unload()
{
   if (isActive())
   {
      SDL_HapticDestroyEffect(mHaptic, mId);
      mId = -1;
      return true;
   }
   return false;
}

bool SdlRumbleEffect::play(int num_repeats)
{
   if (isActive())
   {
      if (num_repeats == -1)
      {
         return (SDL_HapticRunEffect(mHaptic, mId, SDL_HAPTIC_INFINITY) == 0);
      }
      else
      {
         return (SDL_HapticRunEffect(mHaptic, mId, num_repeats) == 0);
      }
   }
   return false;
}

bool SdlRumbleEffect::stop()
{
   if (isActive())
   {
      return (SDL_HapticStopEffect(mHaptic, mId) == 0);
   }
   return false;
}

int SdlRumbleEffect::status()
{
   if (!isActive())
   {
      return -1;
   }

   int ret = SDL_HapticGetEffectStatus(mHaptic, mId);

   if (ret > 0) return 1;
   // The documentation claims that SDL_HapticGetEffectStatus returns
   // SDL_HAPTIC_PLAYING if the effect is playing.  However,
   // SDL_HAPTIC_PLAYING is not defined in the API.  I'm guess that the value
   // is positive and using that to check.  The correct code is:
   //    if (ret == SDL_HAPTIC_PLAYING) return 1;

   if (ret == 0)
   {
      return 0;
   }
   return -1;
}

bool SdlRumbleEffect::isLoaded()
{
   return (mId >= 0);
}

gadget::RumbleEffect::RumbleType SdlRumbleEffect::getType()
{
   return (RumbleType) (mEffect.type);
}

#endif
