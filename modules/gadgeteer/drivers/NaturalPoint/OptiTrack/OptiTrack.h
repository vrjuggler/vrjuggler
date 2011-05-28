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

#ifndef _GADGET_OPTITRACK_H_
#define _GADGET_OPTITRACK_H_

#include <gadget/Devices/DriverConfig.h>
#include <vector>
#include <vpr/Thread/Thread.h>

#include <gadget/Type/InputDevice.h>

#include "OptiTrackStandalone.h"


namespace gadget
{

/**
 * Position derived class for OptiTrack mocap system.
 *
 * Wraps the OptiTrackStandalone driver.
 *
 * @see Position
 */
class OptiTrack
   : public InputDevice<Position>
{
public:
   OptiTrack();
   virtual ~OptiTrack();

   /**
    * Configures the sensor with the given config element.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /** Begins sampling. */
   bool startSampling();

   /** Main thread of control for this active object. */
   void controlLoop();
   
   /** Stops sampling. */
   bool stopSampling();

   /** Samples data. */
   bool sample();

   /** Updates the sampled data. */
   void updateData();

   /** Returns the current position of the given rigid body number. */
   gmtl::Matrix44f getSensorPos(int sensorNum);

   /** Returns what element type is associated with this class. */
   static std::string getElementType();

   /** Checks if the device is active. */
   bool isActive()
   {
      return mTracker.isActive();
   }

private:
   OptiTrackStandalone mTracker;    /**< The tracker class to read data from. */
   int mPort;                       /**< Port to bind to. */
   bool mDone;                      /**< Boolean to control thread exit. */
   std::vector<int> mRigidBodyIDs;  /**< Rigid body IDs assigned by configuration. */
   std::vector<int> mMarkerIDs;     /**< Marker IDs assigned by configuration. */
};

} // End of gadget namespace

#endif
