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
 * version 2 of the License, or (at your option) any later version.
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

#ifndef _GADGET_RUMBLE_EFFECT_H_
#define _GADGET_RUMBLE_EFFECT_H_

#include <gadget/gadgetConfig.h>
#include <gmtl/Vec.h>

namespace gadget 
{

/** \class RumbleEffect RumbleEffect.h gadget/Type/RumbleEffect.h
 *
 * @since 2.1.16
 */
class GADGET_API RumbleEffect
{
public:
   enum RumbleType {
      NONE         = 0,
      ///Constant haptic effect.
      CONSTANT     = (1 <<  0),
      ///Periodic haptic effect that simulates sine waves.
      SINE         = (1 <<  1),
      ///Periodic haptic effect that simulates square waves.
      SQUARE       = (1 <<  2),
      ///Periodic haptic effect that simulates triangular waves.
      TRIANGLE     = (1 <<  3),
      ///Periodic haptic effect that simulates saw tooth up waves.
      SAWTOOTHUP   = (1 <<  4),
      ///Periodic haptic effect that simulates saw tooth down waves.
      SAWTOOTHDOWN = (1 <<  5),
      /// Ramp haptic effect.
      RAMP         = (1 <<  6),
      /// Condition haptic effect that simulates a spring.
      /// Effect is based on the axes position. 
      SPRING       = (1 <<  7),
      /// Condition haptic effect that simulates dampening.
      /// Effect is based on the axes velocity. 
      DAMPER       = (1 <<  8),
      /// Condition haptic effect that simulates inertia.
      /// Effect is based on the axes acceleration. 
      INERTIA      = (1 <<  9),
      /// Condition haptic effect that simulates friction.
      /// Effect is based on the axes movement. 
      FRICTION     = (1 << 10),
      /// User defined custom haptic effect.
      CUSTOM       = (1 << 11),
      /// Device supports setting the global gain.
      GAIN         = (1 << 12),
      /// Device supports setting autocenter.
      AUTOCENTER   = (1 << 13),
      /// Device can be queried for effect status.
      STATUS       = (1 << 14),
      /// Device can be paused.
      PAUSE        = (1 << 15)
   };

   enum DirectionType {
      /// Uses polar coordinates for the direction.
      POLAR = 0,
      /// Uses cartesian coordinates for the direction.
      CARTESIAN = 1,
      /// Uses spherical coordinates for the direction.
      SPHERICAL = 2
   };

   virtual ~RumbleEffect();

   /**
    * Load this effect onto device.
    *
    * @return true for success.
    *
    */
   virtual bool load() = 0;

   /**
    * Update existing effect on device.
    *
    * @return true for success.
    *
    */
   virtual bool update() = 0;

   /**
    * Delete existing effect on device.
    *
    * @return true for success.
    *
    */
   virtual bool unload() = 0;

   /**
    * Play an effect on device.
    *
    * @return true for success.
    *
    * @param num_repeats Number of times to repeat effect, or -1 for infinity
    *
    */
   virtual bool play(int num_repeats) = 0;

   /**
    * Stop playing an effect.
    *  
    * @return true for success.
    *
    */
   virtual bool stop() = 0;

   /** Gets the status of the current effect on the haptic device.
    *
    * @return 0 if it isn't playing, 1 if it is playing and -1 on error.
    */
   virtual int status() = 0;

   /** 
    * Checks if the effect is loaded.
    *
    * @return true if effect is loaded.
    *
    */
   virtual bool isLoaded() = 0;

   /**
    * Returns the type of the effect.
    *
    */
   virtual RumbleType getType() = 0;

   /**
    * Direction of effect.
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
    */
   virtual void setDelay(int milliseconds);
   virtual int getDelay();

   /**
    * Strength of the effect.
    * The value is in the range [0.0, 1.0].
    *
    * Supported on:
    * - CONSTANT
    */
   virtual void setLevel(float val);
   virtual float getLevel();

   /**
    * Period of the wave.
    *
    * Supported on:
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    *
    * \todo What are the units?
    */
   virtual void setPeriod(int milliseconds);
   virtual int getPeriod();

   /**
    * Peak value.
    * The value is in the range [0.0, 1.0].
    *
    * Supported on:
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    */
   virtual void setMagnitude(float val);
   virtual float getMagnitude();

   /**
    * Horizontal shift of the wave.  [0.0, 1.0] where .25 is 25% offset of
    * the start of the waves.
    *
    * Supported on:
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    *
    * \todo What are the units?
    */
   virtual void setPhase(float val);
   virtual float getPhase();

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
    * Mean value of the wave.
    *
    * Supported on:
    * - Periodic (SINE, SQUARE, TRIANGLE, SAWTOOTHUP, SAWTOOTHDOWN)
    */
   virtual void setOffset(float val);
   virtual float getOffset();
   
}; // class RumbleEffect

} // namespace gadget


#endif /* _GADGET_RUMBLE_EFFECT_H_ */
