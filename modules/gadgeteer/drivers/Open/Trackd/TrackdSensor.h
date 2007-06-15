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

#ifndef _GADGET_TRACKD_SENSOR_H_
#define _GADGET_TRACKD_SENSOR_H_

#include <gadget/Devices/DriverConfig.h>
#include <vector>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <drivers/Open/Trackd/TrackdSensorStandalone.h>


namespace gadget
{

/**
 * Position-derived class for interfacing with trackd sensor data
 * located on the local machine in a shared memory arena.
 *
 * @note A note on reciever access:
 *  Clients of Juggler should access tracker recievers as [0-n].  For
 *  example, if you have recievers 1,2, and 4 with transmitter on 3, then
 *  you can access the data, in order, as 0, 1, 2.
 *
 * @see Position
 */
//class TrackdSensor : public Input, public Position
class TrackdSensor
   : public input_position_t
{
public:

   /** Constructor. */
   TrackdSensor();

   /**
    * Destructor.
    *
    * @pre None.
    * @post Shared memory is released.
    */
   ~TrackdSensor();

   /**
    * Configure the trackd sensor with the given configuration element.
    *
    * @pre c must be an element that has Trackd sensor config information
    * @post If c is a valid configuration element, the device is configured
    *       using its contents.  Otherwise, configuration fails and false is
    *       returned to the caller.
    *
    * @param c A pointer to a TrackdSensor configuration element.
    *
    * @return true if the device was configured succesfully.  false is
    *         returned if the configuration element is invalid.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /** Begins sampling. */
   bool startSampling() { return 1; }

   /** Stops sampling. */
   bool stopSampling() { return 1; }

   /** Samples a value. */
   bool sample() { return 1; }

   /**
    * Updates to the sampled data.
    *
    * @pre None.
    * @post Most recent value is copied over to temp area.
    */
   void updateData();

   /** Returns what element type is associated with this class. */
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
    TrackdSensorStandalone* mTrackdSensors; /**< The sensors that we are dealing with. */
    std::vector<PositionData>   mCurSensorValues; /**< The current (up-to-date) values. */
};

} // End of gadget namespace


#endif
