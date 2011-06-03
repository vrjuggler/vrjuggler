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

#ifndef _VRPN_RUMBLE_EFFECT_H_
#define _VRPN_RUMBLE_EFFECT_H_

#include <gadget/Type/RumbleEffect.h>
#include <vpr/Thread/Thread.h>
#include "Vrpn.h"

class VrpnRumbleEffect
    : public gadget::RumbleEffect
{
public:
    VrpnRumbleEffect(gadget::Vrpn *vrpn, gadget::RumbleEffect::RumbleType type);
   virtual ~VrpnRumbleEffect();

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

   /**
    * Duration of the effect in milliseconds.
    *
    * Once started, one cycle of the event will last this number of seconds.
    * Supported types include:
    * - CONSTANT
    * - SQUARE
    */
   virtual void setLength(int milliseconds);
   virtual int getLength();

   /**
    * Delay before starting the effect.
    *
    * Number of milliseconds to wait before starting the effect.
    * Supported types include:
    * - CONSTANT
    * - SQUARE
    */
   virtual void setDelay(int milliseconds);
   virtual int getDelay();

   /**
    * Period of the effect.
    *
    * Length of period in milliseconds.
    * Supported types include:
    * - CONSTANT
    * - SQUARE
    */
   virtual void setPeriod(int milliseconds);
   virtual int getPeriod();

protected:
    gadget::Vrpn *mVrpn;      ///< Pointer to Vrpn class
    int mId;                  ///< ID of the effect in mHaptic
    int mLengthMilliseconds;  ///< Length of effect in milliseconds
    int mDelayMilliseconds;   ///< Length of delay in milliseconds
    int mPeriodMilliseconds;  ///< Length of period for square rumble in milliseconds
    gadget::RumbleEffect::RumbleType mRumbleType;
    vpr::Thread*   mThread;
    
    bool isActive() { return mId >= 0; }

private:
   bool mPlaying;
   void playThread();
   bool mExitFlag;
};

#endif
