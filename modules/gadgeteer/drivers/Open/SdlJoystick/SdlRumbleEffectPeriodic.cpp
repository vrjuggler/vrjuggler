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

#include "SdlRumbleEffectPeriodic.h"
#include <gmtl/VecOps.h>

#if SDL_VERSION_ATLEAST(1,3,0)

using gadget::RumbleEffect;

SdlRumbleEffectPeriodic::SdlRumbleEffectPeriodic(SDL_Haptic *haptic, 
      gadget::RumbleEffect::RumbleType type)
   : SdlRumbleEffect(haptic, type)
{
   mEffect.periodic.direction.type = SDL_HAPTIC_POLAR;
   mEffect.periodic.length = 1000;
   mEffect.periodic.period = 200;
   mEffect.periodic.magnitude = MAX_FORCE;
}

SdlRumbleEffectPeriodic::~SdlRumbleEffectPeriodic()
{
}

void SdlRumbleEffectPeriodic::setDirection(DirectionType dirType, const gmtl::Vec3f &direction)
{
   mEffect.periodic.direction.type = dirType;

   float mult;
   if (dirType == CARTESIAN)
   {
      mult = MAX_DIRECTION;
   }
   else
   {
      mult = 100;
   }
   mEffect.periodic.direction.dir[0] = direction[0] * mult;
   mEffect.periodic.direction.dir[1] = direction[1] * mult;
   mEffect.periodic.direction.dir[2] = direction[2] * mult;
}

gmtl::Vec3f SdlRumbleEffectPeriodic::getDirection(DirectionType dirType)
{
   if (mEffect.periodic.direction.type != dirType)
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

   return gmtl::Vec3f(mEffect.periodic.direction.dir[0] / mult,
      mEffect.periodic.direction.dir[1] / mult,
      mEffect.periodic.direction.dir[2] / mult);
}

RumbleEffect::DirectionType SdlRumbleEffectPeriodic::getDirectionType()
{
   return (DirectionType)(mEffect.periodic.direction.type);
}

void SdlRumbleEffectPeriodic::setLength(int milliseconds) 
{
   mEffect.periodic.length = milliseconds;
}

int SdlRumbleEffectPeriodic::getLength()
{
   return mEffect.periodic.length;
}

void SdlRumbleEffectPeriodic::setDelay(int milliseconds)
{
   mEffect.periodic.delay = milliseconds;
}

int SdlRumbleEffectPeriodic::getDelay()
{
   return mEffect.periodic.delay;
}

void SdlRumbleEffectPeriodic::setPeriod(int milliseconds)
{
   mEffect.periodic.period = milliseconds;
}

int SdlRumbleEffectPeriodic::getPeriod()
{
   return mEffect.periodic.period;
}

void SdlRumbleEffectPeriodic::setMagnitude(float val)
{
   mEffect.periodic.magnitude = val * MAX_FORCE;
}

float SdlRumbleEffectPeriodic::getMagnitude()
{
   return ((float) (mEffect.periodic.magnitude) / MAX_FORCE);
}

void SdlRumbleEffectPeriodic::setOffset(float val)
{
   mEffect.periodic.offset = val * MAX_FORCE;
}

float SdlRumbleEffectPeriodic::getOffset()
{
   return ((float) (mEffect.periodic.offset) / MAX_FORCE);
}

void SdlRumbleEffectPeriodic::setPhase(float val)
{
   mEffect.periodic.phase = val * 100.0f;
}

float SdlRumbleEffectPeriodic::getPhase()
{
   return mEffect.periodic.phase / 100.0f;
}

void SdlRumbleEffectPeriodic::setAttackLength(int milliseconds)
{
   mEffect.periodic.attack_length = milliseconds;
}

int SdlRumbleEffectPeriodic::getAttackLength()
{
   return mEffect.periodic.attack_length;
}

void SdlRumbleEffectPeriodic::setAttackLevel(float val)
{
   mEffect.periodic.attack_level = val * MAX_FORCE;
}

float SdlRumbleEffectPeriodic::getAttackLevel()
{
   return ((float) (mEffect.periodic.attack_level) / MAX_FORCE);
}

void SdlRumbleEffectPeriodic::setFadeLength(int milliseconds)
{
   mEffect.periodic.fade_length = milliseconds;
}

int SdlRumbleEffectPeriodic::getFadeLength()
{
   return mEffect.periodic.fade_length;
}

void SdlRumbleEffectPeriodic::setFadeLevel(float val)
{
   mEffect.periodic.fade_level = val * MAX_FORCE;
}

float SdlRumbleEffectPeriodic::getFadeLevel()
{
   return ((float) (mEffect.periodic.fade_level) / MAX_FORCE);
}

#endif
