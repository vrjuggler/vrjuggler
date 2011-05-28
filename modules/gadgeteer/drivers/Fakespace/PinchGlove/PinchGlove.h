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

#ifndef _GADGET_PINCH_GLOVE_H_
#define _GADGET_PINCH_GLOVE_H_

#include <gadget/Devices/DriverConfig.h>

#include <string>
#include <vector>
#include <boost/mpl/inherit.hpp>

#include <gadget/Type/InputDevice.h>

#include "PinchGloveStandalone.h"


namespace gadget
{

/**
 * Fakespace PinchGlove device.
 */
class PinchGlove
   : public InputDevice<boost::mpl::inherit<Glove, Digital>::type>
{
public:
   /**
    * Default constructor. 
    */
   PinchGlove() : mGlove( NULL )
   {;}

   /** 
    * Destructor, shuts down the glove device. 
    */
   virtual ~PinchGlove();

   /**
    * Configure the PinchGlove's serial port and baud.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /**
    * Returns the string rep of the element type used to config this device.
    * This string is used by the device factory to look up device drivers
    * based up the type of element it is trying to load.
    */
   static std::string getElementType();
   
   /**
    * Start the PinchGlove device sampling, this will spawn a thread which will
    * just repeatedly call sample() until stopSampling is called.
    * 
    * @return true - Sucessfully started.
    */
   virtual bool startSampling();
   
   /** 
    * Stops sampling the PinchGlove by setting mExitFlag to true which will
    * cause the spawned thread to exit naturally.
    */
   virtual bool stopSampling();
   
   /**
    * Read the next set of input and place it in the SampleBuffers.
    * 
    * NOTE: This data is not accessable until updataData is called.
    */
   virtual bool sample();
   
   /**
    * Swap the SampleBuffers so that calls to getData() on this device will
    * return the most recently sampled data.
    */ 
   virtual void updateData ();

protected:
   /**
    * The main control loop for this device.
    */
   void controlLoop();

protected:
   PinchGloveStandalone* mGlove;        /**< The PinchGloveStandalone device. */
   std::string           mPortName;     /**< Serial Port to connect to. */
   int                   mBaud;         /**< Baud to communicate with device. */
   bool                  mExitFlag;     /**< Flag used to cleanly shutdown device. */
};

} // End of gadget namespace


#endif   /* _GADGET_PINCH_GLOVE_H_ */
