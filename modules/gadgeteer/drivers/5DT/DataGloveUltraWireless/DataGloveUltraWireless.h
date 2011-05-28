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

#ifndef _GADGET_DATA_GLOVE_ULTRA_H_
#define _GADGET_DATA_GLOVE_ULTRA_H_

#include <gadget/Devices/DriverConfig.h>

#include <vector>
#include <boost/mpl/inherit.hpp>

#include <vpr/Thread/Thread.h>

#include <gadget/Type/InputDevice.h>

#include "DataGloveUltraWirelessStandalone.h"


namespace gadget
{

/**
 * Driver for Wireless 5DT Data Glove Ultra Wireless 5 and 14 with Serial Port Kit.
 *
 * @note The 5DT wireless DataGloveUltraWireless is a transmit-only device.
 * @note The glove continuously emits a glove data packet.
 *
 * @date 06-15-08
 */

class DataGloveUltraWireless
   : public InputDevice<boost::mpl::inherit<Command, Analog>::type>
{
public:
   /** Construct */
   DataGloveUltraWireless();

   /** Destroy the glove */
   virtual ~DataGloveUltraWireless();

  /** Configures the device with a config element. */
  virtual bool config(jccl::ConfigElementPtr e);

   /** Returns what element type is associated with this class. */
   static std::string getElementType();

   /** Begins sampling. */
   virtual bool startSampling();

   /** Ends sampling. */
   virtual bool stopSampling();

   /** Samples data. */
   virtual bool sample();

   /** Updates to the sampled data. */
   virtual void updateData ();

   /** The main control loop for the object. */
   void controlLoop();

   bool isActive()
   {
      return mGlove.isActive();
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
   DataGloveUltraWirelessStandalone mGlove;           /**< The actual glove */

   std::string    mPortName;
   int            mBaudRate;
   bool           mPortAEnabled;
   bool           mPortBEnabled;

   // Port A, B;
   std::vector<AnalogData> mAnalogData;   /**< analogs for each finger */

   bool           mDone;

};

} // End of gadget namespace


#endif   /* _GADGET_DATA_GLOVE_H_ */
