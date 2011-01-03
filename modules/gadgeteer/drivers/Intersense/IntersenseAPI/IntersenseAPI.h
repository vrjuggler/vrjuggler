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

#ifndef _GADGET_INTERSENSE_H_
#define _GADGET_INTERSENSE_H_

#include <gadget/Devices/DriverConfig.h>
#include <vector>
#include <vpr/Thread/Thread.h>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Position.h>
#include <drivers/Intersense/IntersenseAPI/IntersenseAPIStandalone.h>


namespace gadget
{

/**
 * Position derived class for running an IS900 tracking system.
 *
 * Wraps the IntersenseStandalone driver.
 *
 * Intersense is a positional device driver for the Flock of Birds, the config
 * element in the constructor should set up all the settings, for these to be
 * changed the Flock has to be deleted and a new instance created with an
 * updated config element.
 *
 * @note Intersense inherits from Digital and Analog.  These base classes,
 *       however, can not handle multiple receivers in the same way as
 *       gadget::Position.  Therefore, to access buttons 0-3 on station 0,
 *       call for button 0-3. but to access buttons 0-3 on station 1,
 *       you must ask for units 4-7, and so on.
 * @note Some functions still remain for changing the options of
 *       the flock when its not in Sampling mode, but in order to stay
 *       consistent with the gadget::Position functionality these
 *       are only left for building apps without jccl::ConfigElement objects.
 * @note A note on receiver access:
 *   Clients of juggler should access tracker recievers as [0-n]
 *   For example, if you have recievers 1,2, and 4 with transmitter on 3,
 *   then you can access the data, in order, as 0,1,2.
 *
 * @see Position
 */
class IntersenseAPI
   : public input_digital_analog_position_t
{
public:
   IntersenseAPI();
   virtual ~IntersenseAPI();

   /** Configures the device with a config element. */
   virtual bool config(jccl::ConfigElementPtr e);

   /** Begins sampling. */
   bool startSampling();

   /** Main thread of control for this active object. */
   void controlLoop();

   /** Stops sampling. */
   bool stopSampling();

   /** Samples data. */
   bool sample();

   /** Updates to the sampled data. */
   void updateData();

   /** Returns what element type is associated with this class. */
   static std::string getElementType();

   /** Checks if the device is active. */
   bool isActive()
   {
      return mTracker.isActive();
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
   struct ISStationConfig
   {
      int stationIndex;
      bool enabled;

      int dig_num;
      int ana_num;

      bool useDigital;
      bool useAnalog;
   };

   IntersenseAPIStandalone mTracker;

   std::vector<ISStationConfig> mStations;

   std::vector<DigitalData> mDigitalData;
   std::vector<AnalogData>  mAnalogData;

   std::string    mISenseDriverLocation;
   bool           mDone;
};

} // End of gadget namespace

#endif
