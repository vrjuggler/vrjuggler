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

#ifndef _GADGET_ASCENSION_WANDA_H_
#define _GADGET_ASCENSION_WANDA_H_

#include <gadget/Devices/DriverConfig.h>

#include <boost/mpl/inherit.hpp>

#include <vpr/vpr.h>
#include <vpr/vprDomain.h>

#include <gadget/Type/InputDevice.h>

#include "WandaStandalone.h"


namespace gadget
{

/**
 * Gadgeteer wrapper around wanda::WandaStandalone driver.
 */
class Wanda
   : public InputDevice<boost::mpl::inherit<Digital, Analog>::type>
{
public:
   Wanda(const char* portName = "/dev/ttyS0");

   virtual ~Wanda();

   static std::string getElementType();

   bool isActive()
   {
      return mWanda.isActive();
   }

   /**
    * Configures this device driver.
    *
    * @pre The type of \p elt matches the valued returned by getElementType().
    * @post This object and its base classes are configured.
    *
    * @param elt The config element for this driver.
    *
    * @return \c true is returned if this driver is configured successfully.
    *         Otherwise, \c false is returned.
    */
   virtual bool config(jccl::ConfigElementPtr elt);

   /**
    * Starts communication with the Wanda device and spawns the sample thread.
    *
    * @return \c true is returned if sampling was started successfully.
    *         Otherwise, \c false is returned.
    */
   virtual bool startSampling();

   /**
    * Stops the sample thread and shuts down the Wanda device.
    *
    * @return \c true is returned if sampling was stopped successfully.
    *         Otherwise, \c false is returned.
    */
   virtual bool stopSampling();

   /**
    * Records a sample from the Wanda device.
    *
    * @return \c true is returned if a sample was recorded successfully.
    *         Otherwise, \c false is returned.
    */
   virtual bool sample();

   /**
    * Swaps the analog and digital sample buffers if this device is active.
    * Otherwise, no action is performed.
    */
   virtual void updateData();

private:
   /** The function called by the spawned thread. */
   void controlLoop();

   /** @name Sample Loop Members */
   //@{
   bool mExitFlag;
   //@}

   wanda::WandaStandalone mWanda;
};

}


#endif /* _GADGET_ASCENSION_WANDA_H_ */
