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

#ifndef _GADGET_RUMBLE_H_
#define _GADGET_RUMBLE_H_

#include <gadget/gadgetConfig.h>

#include <vector>
#include <boost/concept_check.hpp>   /* for ignore_unused_variable_warning */
#include <boost/noncopyable.hpp>
#include <boost/signal.hpp>

#include <cppdom/cppdom.h>

#include <vpr/Util/SignalProxy.h>
#include <vpr/IO/SerializableObject.h>

#include <jccl/Config/ConfigElementPtr.h>

#include <gadget/Type/RumblePtr.h>
#include <gadget/Type/RumbleEffect.h>
#include <gadget/Type/RumbleEffectPtr.h>

namespace gadget
{

/** \class Rumble Rumble.h gadget/Type/Rumble.h
 *
 * Rumble is the abstract base class from which devices supporting rumble
 * haptic feedback derive.
 *
 * @since 2.1.16
 */
class GADGET_CLASS_API Rumble
   : public vpr::SerializableObject
   , private boost::noncopyable
{
protected:
   /* Constructor/Destructors */
   Rumble();

public:


   /**
    * Creates a Rumble instance and returns it wrapped in a
    * RumblePtr object.
    */
   static RumblePtr create();

   virtual ~Rumble();

   virtual bool config(jccl::ConfigElementPtr e)
   {
      boost::ignore_unused_variable_warning(e);
      return true;
   }
   
   /**
    * Gets the supported capabilities in a bitmask.
    *
    * @return bitmask defined in Rumble::RumbleType
    *
    */
   virtual unsigned int getCapabilities() { return 0; }
    
   
   /**
    * Gets the number of directional axes supported by device.
    *
    * @return integer number of axes.
    *
    */
   virtual int getNumAxes() { return 0; }
   
   /**
    * Gets the number of effects the device can store.
    *
    * @return integer number of effects or -1 for error.
    *
    */
   virtual int getMaxStoredEffects() { return 0; }

   /**
    * Gets the number of effects the device can play simultaneously.
    *
    * @return integer number of effects or -1 for error.
    *
    */
   virtual int getMaxPlayingEffects() { return 0; }

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
   RumbleEffectPtr createEffect(RumbleEffect::RumbleType type);

   /**
    * Stop all effects.
    *
    * @return true for success.
    *
    */
   virtual bool stopAllEffects() { return false; }

   /**
    * Pause/resume effect playback.
    *
    * @return true for success.
    *
    * @param pause True to pause, false to un-pause
    *
    */
   virtual bool setPause(const bool pause) { boost::ignore_unused_variable_warning(pause); return false; }

   /**
    * Get pause state.
    *
    * @return true for paused, false for un-paused.
    *
    */
   virtual bool getPaused() { return false; }


   virtual std::string getInputTypeName()
   {
      return std::string("Rumble");
   }
   
   /**
    * Required to support serialization interface, does nothing.
    *
    * @param writer The object writer to which this object will be
    *               serialized.
    */
   virtual void writeObject(vpr::ObjectWriter* writer) { boost::ignore_unused_variable_warning(writer); }

   /**
    * Required to support serialization interface, does nothing.
    *
    * @param reader The object reader from which this object will be
    *               de-serialized.
    */
   virtual void readObject(vpr::ObjectReader* reader) { boost::ignore_unused_variable_warning(reader); }

protected:
   virtual RumbleEffectPtr createEffectImp(RumbleEffect::RumbleType type)
   {
      boost::ignore_unused_variable_warning(type);
      return RumbleEffectPtr();
   }
  

private:

};

} // End of gadget namespace

#endif   /* _GADGET_RUMBLE_H_ */
