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

#ifndef _GADGET_ASCENSION_MOTION_STAR_H_
#define _GADGET_ASCENSION_MOTION_STAR_H_

#include <gadget/Devices/DriverConfig.h>

#include <string>
#include <vpr/Thread/Thread.h>

#include <gadget/Type/InputDevice.h>
#include <drivers/Ascension/MotionStar/MotionStarStandalone.h>


namespace gadget
{

/**
 *  Position-derived class for running an Ascension MotionStar device that
 *  is connected to a Flock of Birds.  It is a wrapper class for the real
 *  driver class 'MotionStarStandalone'.
 *
 *  MotionStar adds to the MotionStarStandalone class shared memory and
 *  threading.<br>
 *  MotionStar is a positional device driver for the Flock of Birds.  The
 *  config element in the constructor should set up all the settings.  For
 *  these to be changed, the MotionStar has to be deleted and a new instance
 *  created with an updated config element.
 *  <br>
 *
 * @note Some functions still remain for changing the options of the
 *        MotionStar when its not in Sampling mode, but in order to stay
 *        consistent with the gadget::Position functionality these are only
 *        left for building apps without jccl::ConfigElement objects.
 * @note A note on reciever access:
 *   Clients of Juggler should access tracker recievers as [0-n].  For
 *   example, if you have recievers 1,2, and 4 with transmitter on 3, then
 *   you can access the data, in order, as 0, 1, 2.
 *
 * @see Position
 */
class MotionStar
   : public InputDevice<Position>
{
public:

   /**
    * Constructor.  This invokes the MotionStarStandalone constructor and
    * initializes member variables.
    *
    * @post mMotionStar is initialized, and mThread is set to NULL.
    *
    * @param address          The IP address or hostname of the MotionStar
    *                         PC to which we are connecting.  This defaults
    *                         to NULL.
    * @param port             The server port to which we are connecting.
    *                         The default is 6000.
    * @param proto            The transmission protocol to use in
    *                         communicating with the server.  The default
    *                         is BIRDNET::TCP which matches the default
    *                         port number 6000.
    * @param master           A flag stating whether or not this server
    *                         chassis is a master or a slave.  The default
    *                         is slave (false).
    * @param hemisphere       The hemisphere in use on the transmitter.
    *                         The default is FLOCK::FRONT_HEMISPHERE.
    * @param bird_format      The bird format.  The default is
    *                         FLOCK::POSITION_ANGLES.
    * @param run_mode         The run mode for the device.  The default is
    *                         BIRDNET::CONTINUOUS.
    * @param report_rate
    * @param measurement_rate
    */
   MotionStar(const char* address = NULL, const unsigned short port = 6000,
              const enum BIRDNET::protocol proto = BIRDNET::TCP,
              const bool master = false,
              const FLOCK::hemisphere hemisphere = FLOCK::FRONT_HEMISPHERE,
              const FLOCK::data_format bird_format = FLOCK::POSITION_ANGLES,
              const BIRDNET::run_mode run_mode = BIRDNET::CONTINUOUS,
              const unsigned char report_rate = 1,
              const double measurement_rate = 68.3);

   /**
    * Destructor.  Sampling is stopped, and the data pool is deallocated.
    *
    * @pre None.
    * @post Sampling is stopped, and the data pool is deallocated.
    */
   virtual ~MotionStar();

   // ========================================================================
   // Input overrides (or methods with the same name anyway).
   // ========================================================================

   /**
    * Returns what element type is associated with this class.
    *
    * @pre None.
    * @post A std::string constructed from a static constant string is
    *       returned to the caller.
    *
    * @return A std::string containing the element type for this device.
    */
   static std::string getElementType();

   /**
    * Checks to see if the device is active or not.
    *
    * @pre mMotionStar has been initialized.
    * @post The active state of the MotionStar driver is returned to the
    *       caller.
    *
    * @return true if the MotionStar driver is active; false otherwise.
    */
   bool isActive()
   {
      return mMotionStar.isActive();
   }

   // ========================================================================
   // Position overrides.
   // ========================================================================

   /**
    * Configures the MotionStar with the given config element.
    *
    * @pre None.
    * @post If e is a valid config element, the device is configured using its
    *        contents.  Otherwise, configuration fails and false is returned
    *        to the caller.
    *
    * @param e A pointer to a MotionStar config element.
    *
    * @return true if the device was configured succesfully; false otherwise.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /**
    * Begins sampling.
    *
    * @pre mMyThread is either NULL or points to a valid vpr::Thread object.
    * @post If the driver is not already active and a thread has not already
    *       been created, an attempt is made to activate the device.  First,
    *       a connection attempt to the server is made.  If successful, the
    *       device is started.  If that too succeeds, a new threada is
    *       created for sampling the device.
    *
    * @return 0 if sampling was unable to begin for some reason; 1 otherwise.
    */
   virtual bool startSampling();

   /**
    * Stops sampling.
    *
    * @pre mMyThread is either NULL or points to a valid vpr::Thread object.
    * @post If the driver is not currently active, no stop attempt is made.
    *       Otherwise, if mMyThread is non-NULL (assumed to point to a
    *       valid vpr::Thread object), the thread is killed and deleted, and a
    *       stop attempt is made on the device.
    *
    * @return 0 if sampling could not be stopped.  This will occur if the
    *         device is not active or the stop attempt failed.  1 is returned
    *         if sampling was stopped successfully.
    */
   virtual bool stopSampling();

   /**
    * Samples data.
    *
    * @pre mMotionStar has been initia,lized.
    * @post If the device is active, a sample is made from it.  Data are
    *       read from each of the birds into the current read buffer, and
    *       transforms are constructed based on what was received.  Once
    *       this is done, the valid index is swapped for the next round of
    *       sampling.
    *
    * @return 0 if no sample could be made.
    */
   virtual bool sample();

   /**
    * Updates the sampled data.
    *
    * @pre None.
    * @post If the device is active, the valid data for all the birds are
    *       copied into the current data so that both are valid.  The
    *       current indices are then swapped.
    */
   virtual void updateData();


   // ========================================================================
   // MotionStar-specific methods.
   // ========================================================================

   /**
    * Sets the address (either IP address or hostname) for the server.
    *
    * @pre mMotionStar has been initialized.
    * @post If the device is not active, the address is set to the given
    *       value.
    *
    * @param n The new value for the server address.
    */
   void setAddressName(const char* n);

   /**
    * Gets the current server address for the device.
    *
    * @pre mMotionStar has been initialized.
    * @post The current server address is returned to the caller.
    *
    * @return A character array naming the server address.
    */
   std::string getAddressName() const
   {
      return mMotionStar.getAddressName();
   }

   /**
    * Sets the port on the server to which we connect.
    *
    * @pre mMotionStar has been initialized.
    * @post If the device is not active, the port is set to the given value.
    *
    * @param port The new value for the server port.
    */
   void setServerPort(const unsigned short port);

   /**
    * Gets the server port.
    *
    * @pre mMotionStar has been initialized.
    * @post The current server port is returned to the caller.
    *
    * @return An unsigned short integer containing the server port number.
    */
   unsigned short getServerPort() const
   {
      return mMotionStar.getServerPort();
   }

   /**
    * Sets the transmission protocol used between the client and the server.
    *
    * @pre mMotionStar has been initialized.
    * @post If the device is not active, the transmission protocol is set to
    *        the given value.
    *
    * @param protocol  The new value for the transmission protocol.
    */
   void setProtocol(const enum BIRDNET::protocol proto);

   /**
    * Gets the transmission protocol used between the client and the server.
    *
    * @pre mMotionStar has been initialized.
    * @post The current transmission protocol is returned to the caller.
    *
    * @return
    */
   enum BIRDNET::protocol getProtocol() const
   {
      return mMotionStar.getProtocol();
   }

   /**
    * Sets the master status for this server chassis.
    *
    * @pre mMotionStar has been initialized.
    * @post If the device is not active, the master status is set to the
    *        given value.
    *
    * @param msater The new value for the master status.
    */
   void setMasterStatus(const bool master);

   /**
    * Gets the master status for this server chassis.
    *
    * @pre mMotionStar has been initialized.
    * @post The current master status for this chassis is returned to the
    *        caller.
    *
    * @return true if this server chassis is a master; false if it is a slave.
    */
   bool getMasterStatus() const
   {
      return mMotionStar.getMasterStatus();
   }

   /**
    * Changes the hemisphere of the transmitter.
    *
    * @pre mMotionStar has been initialized.
    * @post If the device is not active, the hemisphere for it is reset to
    *        the given value.
    *
    * @param i The new hemisphere to use.
    */
   void setHemisphere(const unsigned char hemisphere);

   /**
    * Gets the transmitter hemisphere currently in use by the device.
    *
    * @pre mMotionStar has been initialized.
    * @post The current hemisphere value is returned to the caller.
    *
    * @return An integer value for the hemisphere in use.
    */
   unsigned char getHemisphere() const
   {
      return mMotionStar.getHemisphere();
   }

   /**
    * Sets the bird format to the given value.
    *
    * @pre mMotionStar has been initialized.
    * @post If the device is not active, the bird format is updated to the
    *        new value.
    *
    * @param n The new bird format to use.
    */
   void setBirdFormat(const unsigned int format);

   /**
    * Gets the bird format currently in use.
    *
    * @pre mMotionStar has been initialized.
    * @post The current bird format value is returned to the caller.
    *
    * @return An unsigned integer value for the bird format.
    */
   unsigned int getBirdFormat() const
   {
      return mMotionStar.getBirdFormat();
   }

   /**
    * Gets the number of sensors currently connected to the flock.
    *
    * @pre mMotionStar has been initialized.
    * @post The current number of connected sensors is returned to the caller.
    *
    * @return An unsigned integer value for the number of connected sensors.
    */
   unsigned int getNumSensors() const
   {
      return mMotionStar.getNumSensors();
   }

   /**
    * Sets the run mode for the device.
    *
    * @pre mMotionStar has been initialized.
    * @post If the device is not active, the run mode is set to the given
    *       value.
    *
    * @param i The new value for the run mode.
    */
   void setRunMode(const unsigned int i);

   /**
    * Gets the current run mode for the device.
    *
    * @pre mMotionStar has been initialized.
    * @post The current run mode is returned to the caller.
    *
    * @return An integer value for run mode.
    */
   int getRunMode() const
   {
      return mMotionStar.getRunMode();
   }

   /**
    * Sets the report rate for the birds.
    *
    * @pre mMotionStar has been initialized.
    * @post If the device is not active, the bird report rate is set to the
    *        given byte value.
    *
    * @param rate The new value for the bird report rate.
    */
   void setReportRate(const unsigned char rate);

   /**
    * Gets the current report rate for the device.
    *
    * @pre mMotionStar has been initialized.
    * @post The current report rate is returned to the caller.
    *
    * @return An unsigned character value for report rate.
    */
   unsigned char getReportRate() const
   {
      return mMotionStar.getReportRate();
   }

   /**
    * Sets the data measurement rate for the chassis.
    *
    * @pre mMotionStar has been initialized.
    * @post If the device is not active, the chassis data measurement rate
    *       is set to the given double value.
    *
    * @param rate The new value for the measurement rate.
    */
   void setMeasurementRate(const double rate);

   /**
    * Gets the current data measurement rate for the chassis.
    *
    * @pre mMotionStar has been initialized.
    * @post The current data measurement rate is returned to the caller.
    *
    * @return A double-precision floating-point value for data measurement
    *         rate.
    */
   double getMeasurementRate() const
   {
      return mMotionStar.getMeasurementRate();
   }

   /**
    * Control loop for sampling. Waits on flag.
    */
   void controlLoop();

private:
   /**
    * Unimplemented!
    */
   void positionCorrect(float& x, float& y, float& z);

   /**
    * Unimplemented!
    */
   void initCorrectionTable(const char*);

   vpr::Thread*         mMyThread;   /**< The thread doing the flock sampling */
   MotionStarStandalone mMotionStar; /**< Actual MotionStar device driver */
   bool                 mExitFlag;

    //PositionData*       mData;
};

} // gadget namespace


#endif   /* _GADGET_ASCENSION_MOTION_STAR_H_ */
