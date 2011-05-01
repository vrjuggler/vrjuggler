/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _GADGET_RUMBLE_PROXY_H_
#define _GADGET_RUMBLE_PROXY_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Type/Proxy.h>
#include <gadget/Type/Rumble.h>
#include <gadget/Type/RumbleData.h>
#include <gadget/Type/RumbleProxyPtr.h>

namespace gadget
{

/** \class RumbleProxy RumbleProxy.h gadget/Type/RumbleProxy.h
 *
 * @since 2.1.16
 */
class GADGET_CLASS_API RumbleProxy : public TypedProxy<Rumble>
{
public:
   typedef TypedProxy<Rumble> base_type;

protected:
   RumbleProxy();

public:
   static RumbleProxyPtr create();

   virtual ~RumbleProxy();

   /**
    * Gets the supported capabilities in a bitmask.
    *
    * @return bitmask defined in Rumble::RumbleType
    *
    */
   unsigned int getCapabilities()
   {
      if (isNotActive())
      {
         return 0;
      }
      return mTypedDevice->getCapabilities();
   }

   /**
    * Gets the number of directional axes supported by device.
    *
    * @return integer number of axes.
    *
    */
   int getNumAxes()
   {
      if (isNotActive())
      {
         return 0;
      }
      return mTypedDevice->getNumAxes();
   }

   /**
    * Gets the number of effects the device can store.
    *
    * @return integer number of effects or -1 for error.
    *
    */
   int getMaxStoredEffects()
   {
      if (isNotActive())
      {
         return -1;
      }
      return mTypedDevice->getMaxStoredEffects();
   }

   /**
    * Gets the number of effects the device can play simultaneously.
    *
    * @return integer number of effects or -1 for error.
    *
    */
   int getMaxPlayingEffects()
   {
      if (isNotActive())
      {
         return -1;
      }
      return mTypedDevice->getMaxPlayingEffects();
   }

   /**
    * Creates a new effect.
    *
    * @param type The type of effect to create.
    * @param name The name to give the effect.  It can be retreeved again
    *             with getEffect().
    * @return a pointer to the effect or null if the type is not supported or
    *         the name is not available.
    *
    */
   RumbleEffectPtr createEffect(RumbleEffect::RumbleType type)
   {
      if (isNotActive())
      {
         return RumbleEffectPtr();
      }
      return mTypedDevice->createEffect(type);
   }

   /**
    * Stop all effects.
    *
    * @return true for success.
    *
    */
   bool stopAllEffects()
   {
      if (isNotActive())
      {
         return false;
      }
      return mTypedDevice->stopAllEffects();
   }

   /**
    * Pause/resume effect playback.
    *
    * @return true for success.
    *
    * @param pause True to pause, false to un-pause
    *
    */
   bool setPause(const bool pause)
   {
      if (isNotActive()) 
      {
         return false;
      }
      return mTypedDevice->setPause(pause);
   }

   /**
    * Get pause state.
    *
    * @return true for paused, false for un-paused.
    *
    */
   bool getPaused()
   {
      if (isNotActive())
      {
         return false;
      }
      return mTypedDevice->getPaused();
   }

   static std::string getElementType();

   bool config(jccl::ConfigElementPtr element);
private:
   bool isNotActive() { return (isStupefied() || mTypedDevice.get() == NULL); }
};
	
}

#endif
