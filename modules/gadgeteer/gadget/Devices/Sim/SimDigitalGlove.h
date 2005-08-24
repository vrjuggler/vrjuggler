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

#ifndef _GADGET_SIM_DIGITALGLOVE_H
#define _GADGET_SIM_DIGITALGLOVE_H

#include <gadget/gadgetConfig.h>
#include <vector>

#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Glove.h>
#include <gadget/Type/InputMixer.h>

#include <gadget/Devices/Sim/SimInput.h>

namespace gadget
{

/** \class SimDigitalGlove SimDigitalGlove.h gadget/Devices/Sim/SimDigitalGlove.h
 *
 * Simulated digital glove device.
 * Simulates a digital glove device from a keyboard device.
 * It allows any number of simulated digital glove devices to be created.
 *
 * When the key configured is held down, the digital device is on.
 * When the key is released, the device is no longer set.
 *
 * This class should not be used directly by the user.
 */
class SimDigitalGlove : virtual public InputMixer< InputMixer< InputMixer<Input,Digital>, SimInput>, Glove>
{
public:
   /** Default constructor. */
   SimDigitalGlove();

   /** Destructor. */
   virtual ~SimDigitalGlove() throw ();

   /** Configures this device. */
   virtual bool config(jccl::ConfigElementPtr element);

   enum finger
   {
      LTHUMB = 0, LINDEX = 1, LMIDDLE = 2, LRING = 3, LPINKY = 4,
      RTHUMB = 6, RINDEX = 7, RMIDDLE = 8, RRING = 9, RPINKY = 10
   };

   virtual bool startSampling(){return true;}
   virtual bool stopSampling(){return true;}
   virtual bool sample(){return true;}

   /** Updates the data. */
   virtual void updateData();

   static std::string getElementType();

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
   std::vector<KeyModPair>    mSimKeys;       /**< The keys to press for digital simulation */
};

} // End of gadget namespace

#endif
