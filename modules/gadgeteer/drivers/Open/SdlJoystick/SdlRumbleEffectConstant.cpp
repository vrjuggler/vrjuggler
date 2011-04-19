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

#include "SdlRumbleEffectConstant.h"
#include <gmtl/VecOps.h>

#if SDL_VERSION_ATLEAST(1,3,0)

using gadget::RumbleEffect;

SdlRumbleEffectConstant::SdlRumbleEffectConstant(SDL_Haptic *haptic)
   : SdlRumbleEffect(haptic, RumbleEffect::CONSTANT)
{
   mEffect.constant.direction.type = SDL_HAPTIC_POLAR;
   mEffect.constant.length = 1000;
}

SdlRumbleEffectConstant::~SdlRumbleEffectConstant()
{
}

void SdlRumbleEffectConstant::setDirection(DirectionType dirType, const gmtl::Vec3f &direction)
{
   mEffect.constant.direction.type = dirType;

   float mult;
   if (dirType == CARTESIAN)
      mult = MAX_DIRECTION;
   else
      mult = 100;
   mEffect.constant.direction.dir[0] = direction[0] * mult;
   mEffect.constant.direction.dir[1] = direction[1] * mult;
   mEffect.constant.direction.dir[2] = direction[2] * mult;
}

gmtl::Vec3f SdlRumbleEffectConstant::getDirection(DirectionType dirType)
{
   if (mEffect.constant.direction.type != dirType)
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

   return gmtl::Vec3f(mEffect.constant.direction.dir[0] / mult,
      mEffect.constant.direction.dir[1] / mult,
      mEffect.constant.direction.dir[2] / mult);
}

RumbleEffect::DirectionType SdlRumbleEffectConstant::getDirectionType()
{
   return (DirectionType)(mEffect.constant.direction.type);
}

void SdlRumbleEffectConstant::setLength(int milliseconds) 
{
   mEffect.constant.length = milliseconds;
}

int SdlRumbleEffectConstant::getLength()
{
   return mEffect.constant.length;
}

void SdlRumbleEffectConstant::setDelay(int milliseconds)
{
   mEffect.constant.delay = milliseconds;
}

int SdlRumbleEffectConstant::getDelay()
{
   return mEffect.constant.delay;
}

void SdlRumbleEffectConstant::setLevel(float val)
{
   mEffect.constant.level = val*MAX_FORCE;
}

float SdlRumbleEffectConstant::getLevel()
{
   return ((float) (mEffect.constant.level) / MAX_FORCE);
}

void SdlRumbleEffectConstant::setAttackLength(int milliseconds)
{
   mEffect.constant.attack_length = milliseconds;
}

int SdlRumbleEffectConstant::getAttackLength()
{
   return mEffect.constant.attack_length;
}

void SdlRumbleEffectConstant::setAttackLevel(float val)
{
   mEffect.constant.attack_level = val*MAX_FORCE;
}

float SdlRumbleEffectConstant::getAttackLevel()
{
   return ((float) (mEffect.constant.attack_level) / MAX_FORCE);
}

void SdlRumbleEffectConstant::setFadeLength(int milliseconds)
{
   mEffect.constant.fade_length = milliseconds;
}

int SdlRumbleEffectConstant::getFadeLength()
{
   return mEffect.constant.fade_length;
}

void SdlRumbleEffectConstant::setFadeLevel(float val)
{
   mEffect.constant.fade_level = val*MAX_FORCE;
}

float SdlRumbleEffectConstant::getFadeLevel()
{
   return ((float) (mEffect.constant.fade_level) / MAX_FORCE);
}

#endif
