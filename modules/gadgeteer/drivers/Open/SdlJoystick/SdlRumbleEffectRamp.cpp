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

#include "SdlRumbleEffectRamp.h"
#include <gmtl/VecOps.h>

#if SDL_VERSION_ATLEAST(1,3,0)

using gadget::RumbleEffect;

SdlRumbleEffectRamp::SdlRumbleEffectRamp(SDL_Haptic *haptic)
   : SdlRumbleEffect(haptic, RumbleEffect::RAMP)
{
   mEffect.ramp.direction.type = SDL_HAPTIC_POLAR;
   mEffect.ramp.length = 1000;
}

SdlRumbleEffectRamp::~SdlRumbleEffectRamp()
{
}

void SdlRumbleEffectRamp::setDirection(DirectionType dirType, const gmtl::Vec3f &direction)
{
   mEffect.ramp.direction.type = dirType;

   float mult;
   if (dirType == CARTESIAN)
   {
      mult = MAX_DIRECTION;
   }
   else
   {
      mult = 100;
   }
   mEffect.ramp.direction.dir[0] = direction[0] * mult;
   mEffect.ramp.direction.dir[1] = direction[1] * mult;
   mEffect.ramp.direction.dir[2] = direction[2] * mult;
}

gmtl::Vec3f SdlRumbleEffectRamp::getDirection(DirectionType dirType)
{
   if (mEffect.ramp.direction.type != dirType)
   {
      return gmtl::Vec3f(0.0f, 0.0f, 0.0f);
   }

   float mult;
   if (dirType == CARTESIAN)
   {
      mult = MAX_DIRECTION;
   }
   else
   {
      mult = 100;
   }

   return gmtl::Vec3f(mEffect.ramp.direction.dir[0] / mult,
      mEffect.ramp.direction.dir[1] / mult,
      mEffect.ramp.direction.dir[2] / mult);
}

RumbleEffect::DirectionType SdlRumbleEffectRamp::getDirectionType()
{
   return (DirectionType)(mEffect.ramp.direction.type);
}

void SdlRumbleEffectRamp::setLength(int milliseconds) 
{
   mEffect.ramp.length = milliseconds;
}

int SdlRumbleEffectRamp::getLength()
{
   return mEffect.ramp.length;
}

void SdlRumbleEffectRamp::setDelay(int milliseconds)
{
   mEffect.ramp.delay = milliseconds;
}

int SdlRumbleEffectRamp::getDelay()
{
   return mEffect.ramp.delay;
}

void SdlRumbleEffectRamp::setStart(float val)
{
   mEffect.ramp.start = val*MAX_FORCE;
}

float SdlRumbleEffectRamp::getStart()
{
   return ((float) (mEffect.ramp.start) / MAX_FORCE);
}

void SdlRumbleEffectRamp::setEnd(float val)
{
   mEffect.ramp.end = val*MAX_FORCE;
}

float SdlRumbleEffectRamp::getEnd()
{
   return ((float) (mEffect.ramp.end) / MAX_FORCE);
}

void SdlRumbleEffectRamp::setAttackLength(int milliseconds)
{
   mEffect.ramp.attack_length = milliseconds;
}

int SdlRumbleEffectRamp::getAttackLength()
{
   return mEffect.ramp.attack_length;
}

void SdlRumbleEffectRamp::setAttackLevel(float val)
{
   mEffect.ramp.attack_level = val*MAX_FORCE;
}

float SdlRumbleEffectRamp::getAttackLevel()
{
   return ((float) (mEffect.ramp.attack_level) / MAX_FORCE);
}

void SdlRumbleEffectRamp::setFadeLength(int milliseconds)
{
   mEffect.ramp.fade_length = milliseconds;
}

int SdlRumbleEffectRamp::getFadeLength()
{
   return mEffect.ramp.fade_length;
}

void SdlRumbleEffectRamp::setFadeLevel(float val)
{
   mEffect.ramp.fade_level = val*MAX_FORCE;
}

float SdlRumbleEffectRamp::getFadeLevel()
{
   return ((float) (mEffect.ramp.fade_level) / MAX_FORCE);
}

#endif
