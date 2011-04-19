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

#ifndef _SDL_RUMBLE_EFFECT_RAMP_H_
#define _SDL_RUMBLE_EFFECT_RAMP_H_

#include "SdlRumbleEffect.h"

#if SDL_VERSION_ATLEAST(1,3,0)

class SdlRumbleEffectRamp
    : public SdlRumbleEffect
{
public:
   SdlRumbleEffectRamp(SDL_Haptic *haptic);
   virtual ~SdlRumbleEffectRamp();

   /**
    * Returns the direction.
    *
    * Supported on:
    * - CONSTANT
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    * - Condition (SPRING, DAMPER, INERTIA, FRICTION)
    * - RAMP
    */
   virtual void setDirection(DirectionType dirType, const gmtl::Vec3f &direction);
   virtual gmtl::Vec3f getDirection(DirectionType dirType);
   virtual DirectionType getDirectionType();

   /**
    * Duration of the effect in milliseconds.
    *
    * Once started, one cycle of the event will last this number of seconds.
    * Supported types include:
    * - CONSTANT
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    * - Condition (SPRING, DAMPER, INERTIA, FRICTION)
    * - RAMP
    * - CUSTOM
    */
   virtual void setLength(int milliseconds);
   virtual int getLength();

   /**
    * Delay before starting the effect.
    *
    * Number of seconds to wait before starting the effect.
    * Supported types include:
    * - CONSTANT
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    * - Condition (SPRING, DAMPER, INERTIA, FRICTION)
    * - RAMP
    * - CUSTOM
    *
    * \todo Verify units.
    */
   virtual void setDelay(int milliseconds);
   virtual int getDelay();

   /**
    * Beginning strength level.
    *
    * Value is in [0.0, 1.0]
    * Supported on:
    * - RAMP
    */
   virtual void setStart(float val);
   virtual float getStart();
    
   /**
    * Ending strength level.
    *
    * Value is in [0.0, 1.0]
    * Supported on:
    * - RAMP
    */
   virtual void setEnd(float val);
   virtual float getEnd();

   /**
    * Duration of the attack in milliseconds.
    *
    * Supported on:
    * - CONSTANT
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    * - RAMP
    * - CUSTOM
    *
    * \todo Verify units.
    */
   virtual void setAttackLength(int milliseconds);
   virtual int getAttackLength();
    
   /**
    * Level at the start of the attack.
    *
    * Value is in [0.0, 1.0]
    * Supported on:
    * - CONSTANT
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    * - RAMP
    * - CUSTOM
    */
   virtual void setAttackLevel(float val);
   virtual float getAttackLevel();

   /**
    * Duration of the fade in milliseconds.
    *
    * Supported on:
    * - CONSTANT
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    * - RAMP
    * - CUSTOM
    *
    * \todo Verify units.
    */
   virtual void setFadeLength(int milliseconds);
   virtual int getFadeLength();

   /**
    * Level at the end of the fade.
    *
    * Value is in [0.0, 1.0]
    * Supported on:
    * - CONSTANT
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    * - RAMP
    * - CUSTOM
    */
   virtual void setFadeLevel(float val);
   virtual float getFadeLevel();
};

#endif
#endif
