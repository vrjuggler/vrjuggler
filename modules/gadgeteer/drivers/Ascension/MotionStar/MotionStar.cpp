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

#include <gadget/Devices/DriverConfig.h>

#include <vector>
#include <boost/concept_check.hpp>

#include <jccl/Config/ConfigElement.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>
#include <gadget/gadgetParam.h>

#include <drivers/Ascension/MotionStar/MotionStar.h>


extern "C"
{

GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::MotionStar>(inputMgr);
}

}

namespace gadget
{

/**
 * Continuously samples the device.
 *
 * @pre The given argument is pointer to a valid MotionStar object.
 * @post MotionStar::sample() is executed repeated until the thread is
 *       stopped.
 *
 * @param arg A pointer to a MotionStar object cast to a void*.
 */
void MotionStar::controlLoop(void* nullParam)
{
   boost::ignore_unused_variable_warning(nullParam);

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "gadget::MotionStar: Spawned SampleBirds starting\n"
      << vprDEBUG_FLUSH;

   // Keep sampling until the flag is set
   while ( !mExitFlag )
   {
      sample();
      vpr::Thread::yield();
   }
}

// ============================================================================
// Public methods.
// ============================================================================

// Constructor.  This invokes the MotionStarStandalone constructor and
// initializes member variables.
MotionStar::MotionStar(const char* address, const unsigned short port,
                       const BIRDNET::protocol proto, const bool master,
                       const FLOCK::hemisphere hemisphere,
                       const FLOCK::data_format bird_format,
                       const BIRDNET::run_mode run_mode,
                       const unsigned char report_rate,
                       const double measurement_rate)
   : mMyThread(NULL)
   , mMotionStar(address, port, proto, master, hemisphere, bird_format,
                 run_mode, report_rate, measurement_rate)

{
    ;
}

// Destructor.  Sampling is stopped, and the data pool is deallocated.
MotionStar::~MotionStar()
{
   stopSampling();
}

std::string MotionStar::getElementType()
{
   return "motion_star";
}

// Configure the MotionStar with the given config element.
bool MotionStar::config(jccl::ConfigElementPtr e)
{
   bool retval(false);

   if ( Input::config(e) &&  Position::config(e) )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "MotionStar::config(jccl::ConfigElementPtr)\n"
         << vprDEBUG_FLUSH;

      const unsigned int cur_version(2);

      if ( e->getVersion() < cur_version )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutBOLD(clrRED, "ERROR")
            << " [gadget::MotionStar::config()] Element named '"
            << e->getName() << "'" << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "is version " << e->getVersion()
            << ", but we require at least version " << cur_version
            << std::endl << vprDEBUG_FLUSH;
         vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "Ignoring this element and moving on." << std::endl
            << vprDEBUG_FLUSH;
         retval = false;
      }
      else
      {
         // Configure mMotionStar with the config info.
         const unsigned num_filters = e->getNum("position_filters");

         // Sanity check.  There has to be at least one position filter
         // configured.
         if ( num_filters == 0 )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << clrOutBOLD(clrRED, "ERROR")
               << ": [MotionStar::config(jccl::ConfigElementPtr)] No position "
               << "filters configured in " << e->getName() << std::endl
               << vprDEBUG_FLUSH;
            retval = false;
         }
         else
         {
            BIRDNET::units expected_units;

            // Find the first position_transform_filter instance and get its
            // device_units property value.  This will tell us what units we're
            // expecting from the hardware.
            const std::string filter_type("position_transform_filter");
            for ( unsigned i = 0; i < num_filters; ++i )
            {
               jccl::ConfigElementPtr pos_elt =
                  e->getProperty<jccl::ConfigElementPtr>("position_filters", i);

               if ( pos_elt->getID() == filter_type )
               {
                  const float unit_conv =
                     pos_elt->getProperty<float>("device_units");

                  vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
                     << "[gadget::MotionStar::config()] Read " << unit_conv
                     << " as the conversion from device units to meters.\n"
                     << vprDEBUG_FLUSH;

                  // Inches.  This is the most likely configuration as of this
                  // writing.
                  if ( unit_conv == 0.0254f )
                  {
                     expected_units = BIRDNET::INCHES;
                  }
                  // Feet.
                  else if ( unit_conv == 0.3048f )
                  {
                     expected_units = BIRDNET::FEET;
                  }
                  // Meters.
                  else if ( unit_conv == 1.0f )
                  {
                     expected_units = BIRDNET::METERS;
                  }
                  // Unexpected value.
                  else
                  {
                     vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                        << "[MotionStar::config(jccl::ConfigElementPtr)] "
                        << clrOutBOLD(clrRED, "ERROR")
                        << ": Unsupported device unit value " << unit_conv
                        << " in " << pos_elt->getFullName() << std::endl
                        << vprDEBUG_FLUSH;
                     vprDEBUG_NEXT(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                        << "Check your configuration for errors.\n"
                        << vprDEBUG_FLUSH;

                     // Break out of this method early because the
                     // configuration element we were given is bad.
                     return false;
                  }

                  // We're done checking for unit conversion values.
                  break;
               }
            }

            mMotionStar.setExpectedUnits(expected_units);

            setAddressName(e->getProperty<std::string>("address").c_str());
            setServerPort((unsigned short) e->getProperty<int>("server_port"));
            setMasterStatus(e->getProperty<bool>("is_master"));
            setHemisphere((unsigned char) e->getProperty<int>("hemisphere"));
            setBirdFormat((unsigned int) e->getProperty<int>("data_format"));
            setRunMode((unsigned int) e->getProperty<int>("mode"));
            setReportRate((unsigned char) e->getProperty<int>("report_rate"));
            setMeasurementRate(e->getProperty<float>("measurement_rate"));
            retval = true;
         }
      }
   }

   return retval;
}

// Begin sampling.
bool MotionStar::startSampling()
{
   bool retval(false);     // Initialize to error status

   // Make sure the device isn't already started.
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar was already started.\n" << vprDEBUG_FLUSH;
      retval = true;
   }
   else
   {
      if ( NULL == mMyThread )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "    Getting MotionStar ready ...\n" << vprDEBUG_FLUSH;

         try
         {
            mMotionStar.start();

            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "gadget::MotionStar ready to go.\n" << vprDEBUG_FLUSH;

            // Set exit flag to not exit and start sample thread
            mExitFlag = false;

            vpr::ThreadMemberFunctor<MotionStar>* run_thread =
               new vpr::ThreadMemberFunctor<MotionStar>(this, &MotionStar::controlLoop, NULL);

            mMyThread = new vpr::Thread(run_thread);

            if ( ! mMyThread->valid() )
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << "gadget::MotionStar could not create sampling thread.\n"
                  << vprDEBUG_FLUSH;
                vprASSERT(false);  // Fail
                retval = false;   // failure
            }
            else
            {
                retval = true;   // success
            }
         }
         // Connection to server failed.  MotionStarStandalone prints out
         // the system error message about why.
         catch(mstar::ConnectException& ex)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "gadget::MotionStar failed to connect to server: "
               << ex.getMessage() << std::endl << vprDEBUG_FLUSH;
         }
         // Some network error occurred when trying to start the device.
         catch(mstar::NetworkException&)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "gadget::MotionStar could not create a socket\n"
               << vprDEBUG_FLUSH;
         }
         // Unkonwn exception from MotionStarStandalone::start().
         catch(mstar::MotionStarException& ex)
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "Abnormal return from MotionStarStandalone::start(): "
               << ex.getMessage() << std::endl << vprDEBUG_FLUSH;
         }
      }
      // The thread has been started, so we are already sampling.
      else
      {
         retval = true;
      }
   }

   return retval;
}

// Stop sampling.
bool MotionStar::stopSampling()
{
   bool retval;

   // If we are not active, we cannot stop the device.
   if ( isActive() == false )
   {
      retval = false;
   }
   // If the sampling thread was started, stop it and the device.
   else if ( mMyThread != NULL )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "Stopping the MotionStar thread ...\n" << vprDEBUG_FLUSH;
      mExitFlag = true;
      mMyThread->join();
      delete mMyThread;
      mMyThread = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "  Stopping the MotionStar ...\n" << vprDEBUG_FLUSH;

      try
      {
         mMotionStar.stop();
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "MotionStar server shut down.\n" << vprDEBUG_FLUSH;
         retval = true;
      }
      catch(mstar::CommandException&)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
            << "MotionStar server did not shut down.\n" << vprDEBUG_FLUSH;
         retval = false;
      }
   }
   // If the thread was not started, then the device is stopped.
   else
   {
      retval = true;
   }

   return retval;
}

// Sample data.
bool MotionStar::sample()
{
   bool retval;
   std::vector< gadget::PositionData > cur_samples(mMotionStar.getNumSensors());

   retval = false;

   if ( isActive() == false )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrSetNORM(clrRED) << "MotionStar ("
         << getAddressName() << ") NOT ACTIVE IN SAMPLE\n"
         << clrRESET << vprDEBUG_FLUSH;
   }
   else
   {
      try
      {
         mMotionStar.sample();

         // get an initial timestamp for this entire sample. we'll copy it into
         // each PositionData for this sample.
         if ( ! cur_samples.empty() )
         {
            cur_samples[0].setTime();

            // For each bird
            for ( unsigned int i = 0; i < mMotionStar.getNumSensors(); ++i )
            {
               // Get the index to the current read buffer
               cur_samples[i].setTime( cur_samples[0].getTime() );
               cur_samples[i].mPosData = mMotionStar.getDeviceData(i);
            }
         }

         // Add the current data as a sample
         addPositionSample(cur_samples);

         retval = true;
      }
      catch (...)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << clrOutNORM(clrRED, "gadget::MotionStar::sample() caught unknown exception")
            << std::endl << vprDEBUG_FLUSH;
         retval = false;
      }
   }

   return retval;
}

// Update to the sampled data.
void MotionStar::updateData()
{
   // If the device is not active, we cannot update the data.
   if ( isActive() == false )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << clrSetNORM(clrRED) << "MotionStar ("
         << getAddressName()
         << ") not active in updateData()\n" << clrRESET
         << vprDEBUG_FLUSH;
   }
   // Otherwise, go through with the update.
   else
   {
      swapPositionBuffers();
   }
}


// Set the address (either IP address or hostname) for the server.
void MotionStar::setAddressName(const char* n)
{
   // If the device active, we cannot change the server address.
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar: Cannot change server address while active\n"
         << vprDEBUG_FLUSH;
   } else
   {
      mMotionStar.setAddressName(n);
   }
}

// Set the port on the server to which we connect.
void MotionStar::setServerPort(const unsigned short port)
{
   // If the device active, we cannot change the server port.
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar: Cannot change server port while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      mMotionStar.setServerPort(port);

      switch (port)
      {
         case 5000:
            setProtocol(BIRDNET::UDP);
            break;
         case 6000:
            setProtocol(BIRDNET::TCP);
            break;
         default:
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "MotionStar: Unexpected port number " << port << " given!\n"
               << "            Defaulting to TCP port.\n" << vprDEBUG_FLUSH;
            mMotionStar.setProtocol(BIRDNET::TCP);
            break;
      }
   }
}

void MotionStar::setProtocol(const enum BIRDNET::protocol proto)
{
   // If the device active, we cannot change the transmission protocol.
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar: Cannot change transmission protocol while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      mMotionStar.setProtocol(proto);
   }
}

void MotionStar::setMasterStatus(const bool master)
{
   // If the device active, we cannot change the master status.
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar: Cannot change master status while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      mMotionStar.setMasterStatus(master);
   }
}

// Change the hemisphere of the transmitter.
void MotionStar::setHemisphere(const unsigned char hemisphere)
{
   // If the device active, we cannot change the hemisphere.
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar: Cannot change hemisphere while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      switch (hemisphere)
      {
         case 0:
            mMotionStar.setHemisphere(FLOCK::FRONT_HEMISPHERE);
            break;
         case 1:
            mMotionStar.setHemisphere(FLOCK::REAR_HEMISPHERE);
            break;
         case 2:
            mMotionStar.setHemisphere(FLOCK::UPPER_HEMISPHERE);
            break;
         case 3:
            mMotionStar.setHemisphere(FLOCK::LOWER_HEMISPHERE);
            break;
         case 4:
            mMotionStar.setHemisphere(FLOCK::LEFT_HEMISPHERE);
            break;
         case 5:
            mMotionStar.setHemisphere(FLOCK::RIGHT_HEMISPHERE);
            break;
         default:
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "gadget::MotionStar: Unknown hemisphere " << hemisphere
               << " given!\n              Defaulting to front hemisphere.\n"
               << vprDEBUG_FLUSH;
            mMotionStar.setHemisphere(FLOCK::FRONT_HEMISPHERE);
            break;
      }
   }
}

// Set the bird format to the given value.
void MotionStar::setBirdFormat(const unsigned int format)
{
   // If the device active, we cannot change the bird format.
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar: Cannot change format while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      switch (format)
      {
         case 0:
            mMotionStar.setBirdFormat(FLOCK::NO_BIRD_DATA);
            break;
         case 1:
            mMotionStar.setBirdFormat(FLOCK::POSITION);
            break;
         case 2:
            mMotionStar.setBirdFormat(FLOCK::ANGLES);
            break;
         case 3:
            mMotionStar.setBirdFormat(FLOCK::MATRIX);
            break;
         case 4:
            mMotionStar.setBirdFormat(FLOCK::POSITION_ANGLES);
            break;
         case 5:
            mMotionStar.setBirdFormat(FLOCK::POSITION_MATRIX);
            break;
         case 6:
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "gadget::MotionStar: Invalid bird format 6 given!\n"
               << "                    Defaulting to position/angles.\n"
               << vprDEBUG_FLUSH;
            mMotionStar.setBirdFormat(FLOCK::POSITION_ANGLES);
            break;
         case 7:
            mMotionStar.setBirdFormat(FLOCK::QUATERNION);
            break;
         case 8:
            mMotionStar.setBirdFormat(FLOCK::POSITION_QUATERNION);
            break;
         default:
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
               << "gadget::MotionStar: Unexpected bird format " << format
               << " given!\n"
               << "                    Defaulting to position/angles.\n"
               << vprDEBUG_FLUSH;
            mMotionStar.setBirdFormat(FLOCK::POSITION_ANGLES);
            break;
      }
   }
}

// Set the run mode for the device.
void MotionStar::setRunMode(const unsigned int mode)
{
   // If the device active, we cannot change the run mode.
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar: Cannot change run mode while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      try
      {
         switch (mode)
         {
            case 0:
               mMotionStar.setRunMode(BIRDNET::CONTINUOUS);
               break;
            case 1:
               mMotionStar.setRunMode(BIRDNET::SINGLE_SHOT);
               break;
            default:
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
                  << "gadget::MotionStar: Unexpected run mode " << mode
                  << " given!\n"
                  << "                    Defaulting to continuous.\n"
                  << vprDEBUG_FLUSH;
               mMotionStar.setRunMode(BIRDNET::CONTINUOUS);
               break;
         }
      }
      catch(mstar::CommandException&)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
            << "gadget::MotionStar: Failed to change run mode to " << mode
            << std::endl << vprDEBUG_FLUSH;
      }
   }
}

// Set the report rate for the device.
void MotionStar::setReportRate(const unsigned char rate)
{
   // If the device active, we cannot change the report rate.
   if (isActive())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar: Cannot change bird report rate while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      mMotionStar.setReportRate(rate);
   }
}

// Set the measurement rate for the chassis.
void MotionStar::setMeasurementRate(const double rate)
{
   // If the device active, we cannot change the measurement rate.
   if (isActive())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::MotionStar: Cannot change chassis measurement rate while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      mMotionStar.setMeasurementRate(rate);
   }
}

// ============================================================================
// Private methods.
// ============================================================================

// Unimplemented!
void MotionStar::positionCorrect(float& x, float& y, float& z)
{
   boost::ignore_unused_variable_warning(x);
   boost::ignore_unused_variable_warning(y);
   boost::ignore_unused_variable_warning(z);

   // XXX Implement me!
}

// Unimplemented!
void MotionStar::initCorrectionTable(const char* table_file)
{
   boost::ignore_unused_variable_warning(table_file);

   // XXX Implement me!
}

} // End of gadget namespace
