/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_TRACKD_API_SENSOR_H_
#define _GADGET_TRACKD_API_SENSOR_H_

#include <gadget/Devices/DriverConfig.h>
#include <vector>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>

#define TRACKD_DLL_EXPORTS 0
#include GADGET_TRACKD_API_H
#include <gmtl/Matrix.h>


namespace gadget
{

/**
 * Position-derived class for interfacing with TrackdAPI sensor data.
 *
 * @note A note on reciever access:
 *   Clients of Gadgeteer should access tracker recievers as [0-n].  For
 *   example, if you have recievers 1,2, and 4 with transmitter on 3, then
 *   you can access the data, in order, as 0, 1, 2.
 *
 * @see gadget::Position
 */
class TrackdAPISensor
   : public input_position_t
{
public:

   /** Constructor. */
   TrackdAPISensor();

   /**
    * Destructor.
    *
    * @pre None.
    * @post Shared memory is released.
    */
   ~TrackdAPISensor();

   /**
    * Configures the trackd sensor with the given config element.
    *
    * @pre e must be a element that has trackd sensor config information
    * @post If e is a valid config element, the device is configured using its
    *        contents.  Otherwise, configuration fails and false is returned
    *        to the caller.
    *
    * @param c  A pointer to a TrackdSensor config element.
    *
    * @return true if the device was configured succesfully, fales if the
    *              config element is invalid.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /** Begins sampling. */
   bool startSampling() { return 1; }

   /** Stops sampling. */
   bool stopSampling() { return 1; }

   /** Samples a value. */
   bool sample() { return 1; }

   /**
    * Update to the sampled data.
    *
    * @pre None.
    * @post Most recent value is copied over to temp area.
    */
   void updateData();

   /** Returns the current position of the given sensor number. */
   gmtl::Matrix44f getSensorPos(int sensorNum);

   /** Returns what element type is associated with this class. */
   static std::string getElementType();

private:
   TrackerReader* mTrackerReader;   /**< The tracker sensor to read data from */
};

} // End of gadget namespace


#endif
