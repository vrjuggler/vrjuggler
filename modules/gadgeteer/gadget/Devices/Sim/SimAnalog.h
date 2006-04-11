/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_SIM_ANALOG_H
#define _GADGET_SIM_ANALOG_H
//#pragma once

#include <gadget/gadgetConfig.h>
#include <vector>

#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Analog.h>
#include <gadget/Devices/Sim/SimInput.h>
#include <gadget/Type/InputMixer.h>


namespace gadget
{

/** \class SimAnalog SimAnalog.h gadget/Devices/Sim/SimAnalog.h
 *
 * Simulated analog device.
 * Simulates an analog device from a keyboard device.
 * It allows any number of simulated analog devices to be created.
 *
 * This class should not be used directly by the user.
 */
//class SimAnalog : public Input, public Analog, public SimInput
class SimAnalog : public InputMixer<InputMixer<SimInput,Input>,Analog>
{
public:
   SimAnalog();

   virtual ~SimAnalog();

   /**
    * Reads the minimum and maximum value configuration information for this
    * analog device.
    *
    * @param element The config element for an analog device.  It must derive
    *                from the base config element type 'analog'.
    */
   virtual bool config(jccl::ConfigElementPtr element);

   /** These functions don't do anything. */
   bool startSampling()
   {
      return true;
   }

   bool stopSampling()
   {
      return true;
   }

   bool sample()
   {
      return true;
   }

   /** Updates the data. */
   virtual void updateData();

   static std::string getElementType();

   virtual std::vector<KeyModPair> getUpKeys()
   {
      return mSimKeysUp;
   }

   virtual std::vector<KeyModPair> getDownKeys()
   {
      return mSimKeysDown;
   }

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

private:
   std::vector<AnalogData>    mAnaData;      /**< The analog data that we have */
   std::vector<KeyModPair>    mSimKeysUp;    /**< The keys to press for moving analog up */
   std::vector<KeyModPair>    mSimKeysDown;  /**< The keys to press for moving analog up */
   float                      mAnaStep;      /**< The analog step size */
   float                      mInitialValue; /**< The inital value for all inputs */
};

} // End of gadget namespace

#endif
