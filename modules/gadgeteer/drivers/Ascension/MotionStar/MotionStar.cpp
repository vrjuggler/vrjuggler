/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#include <gadget/gadgetConfig.h>

#include <vrj/Math/Coord.h>
#include <vrj/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>
#include <gadget/Devices/Ascension/MotionStar.h>


namespace gadget
{

// ============================================================================
// Helper functions speccfic to this file.
// ============================================================================

// ----------------------------------------------------------------------------
// Continuously sample the device.
//
// PRE: The given argument is pointer to a valid MotionStar object.
// POST: MotionStar::sample() is executed repeated until the thread is
//       stopped.
//
// ARGS: arg - A pointer to a MotionStar object cast to a void*.
// ----------------------------------------------------------------------------
static void sampleBirds (void* arg)
{
   MotionStar* devPointer = (MotionStar*) arg;

   vprDEBUG(vrjDBG_INPUT_MGR,3)
      << "vjMotionStar: Spawned SampleBirds starting\n" << vprDEBUG_FLUSH;

   for (;;) {
      devPointer->sample();
   }
}

// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This invokes the MotionStarStandalone constructor and
// initializes member variables.
// ----------------------------------------------------------------------------
MotionStar::MotionStar (const char* address, const unsigned short port,
                            const BIRDNET::protocol proto,
                            const bool master,
                            const FLOCK::hemisphere hemisphere,
                            const FLOCK::data_format bird_format,
                            const BIRDNET::run_mode run_mode,
                            const unsigned char report_rate,
                            const double measurement_rate,
                            const unsigned int birds_required)
   : m_motion_star(address, port, proto, master, hemisphere, bird_format,
                   run_mode, report_rate, measurement_rate, birds_required)
{
   m_my_thread = NULL;
}

// ----------------------------------------------------------------------------
// Destructor.  Sampling is stopped, and the data pool is deallocated.
// ----------------------------------------------------------------------------
MotionStar::~MotionStar ()
{
   stopSampling();

   if (theData != NULL)
   {
      delete theData;
   }
   //getMyMemPool()->deallocate((void*)theData);
}

// ----------------------------------------------------------------------------
// Configure the MotionStar with the given config chunk.
// ----------------------------------------------------------------------------
bool MotionStar::config (jccl::ConfigChunkPtr c)
{
   bool retval;

   retval = false;

   if ( Input::config(c) &&  Position::config(c) )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 3)
         << "       MotionStar::config(jccl::ConfigChunkPtr)\n" << vprDEBUG_FLUSH;

      // Configure m_motion_star with the config info.
      setAddressName(static_cast<std::string>(c->getProperty("address")).c_str());
      setServerPort((unsigned short) static_cast<int>(c->getProperty("serverPort")));
      setMasterStatus(static_cast<bool>(c->getProperty("serverType")));
      setHemisphere((unsigned char) static_cast<int>(c->getProperty("hemisphere")));
      setNumBirds((unsigned int) static_cast<int>(c->getProperty("num")));
      setBirdFormat((unsigned int) static_cast<int>(c->getProperty("bformat")));
      setRunMode((unsigned int) static_cast<int>(c->getProperty("mode")));
      setReportRate((unsigned char) static_cast<int>(c->getProperty("reportRate")));
      setMeasurementRate((float) static_cast<float>(c->getProperty("measurementRate")));
      retval = true;
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Begin sampling.
// ----------------------------------------------------------------------------
int MotionStar::startSampling ()
{
   int retval;

   // Make sure the device isn't already started.
   if ( isActive() )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2) << "vjMotionStar was already started.\n"
                                  << vprDEBUG_FLUSH;
      retval = 0;
   }
   else
   {
      if ( m_my_thread == NULL )
      {
         int num_buffs, start_status;

         if ( theData != NULL )
         {
            //getMyMemPool()->deallocate((void*) theData);
            delete theData;
         }

         // Allocate buffer space for birds.
         num_buffs = (m_motion_star.getNumBirds() + 1) * 3;
         theData   = new vrj::Matrix[num_buffs];

         // Reset current, progress, and valid indices.
         resetIndexes();

         vprDEBUG(vrjDBG_INPUT_MGR, 1) << "    Getting MotionStar ready ...\n"
                                       << vprDEBUG_FLUSH;

         start_status = m_motion_star.start();

         switch (start_status)
         {
            // Proper startup.
            case 0:
               break;
            // Connection to server failed.  MotionStarStandalone prints out
            // the system error message about why.
            case -1:
               vprDEBUG(vrjDBG_INPUT_MGR, 0)
                  << "vjMotionStar failed to connect to server\n"
                  << vprDEBUG_FLUSH;
               retval = 1;
               break;
            // No socket could be created, so no connection can be made.
            case -2:
               vprDEBUG(vrjDBG_INPUT_MGR, 0)
                  << "vjMotionStar could not create a socket\n"
                  << vprDEBUG_FLUSH;
               retval = 1;
               break;
            // No address has been set for the server, so no connection can be
            // made.
            case -3:
               vprDEBUG(vrjDBG_INPUT_MGR, 0)
                  << "No server address set for MotionStar\n"
                  << vprDEBUG_FLUSH;
               retval = 1;
               break;
            // Unkonwn return value from MotionStarStandalone::start().
            default:
               vprDEBUG(vrjDBG_INPUT_MGR, 0)
                  << "Abnormal return from MotionStarStandalone::start()\n"
                  << vprDEBUG_FLUSH;
               retval = 1;
               break;
         }

         // Sanity check.  Make sure the servers were actually started.
         if ( ! isActive() )
         {
            vprDEBUG(vrjDBG_INPUT_MGR, 0) << "vjMotionStar failed to start.\n"
                                          << vprDEBUG_FLUSH;
            retval = 1;
         }
         else
         {
            vprDEBUG(vrjDBG_INPUT_MGR, 1) << "vjMotionStar ready to go.\n"
                                        << vprDEBUG_FLUSH;

            m_my_thread = new vpr::Thread(sampleBirds, (void*) this);

            if ( m_my_thread == NULL )
            {
               vprDEBUG(vrjDBG_INPUT_MGR, 0)
                  << "vjMotionStar could not create sampling thread.\n"
                  << vprDEBUG_FLUSH;
                vprASSERT(false);  // Fail
            }
            else
            {
                retval = 1;   // success
            }
         }
      }
      // The thread has been started, so we are already sampling.
      else
      {
         retval = 0;
      }
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Stop sampling.
// ----------------------------------------------------------------------------
int MotionStar::stopSampling ()
{
   int retval;

   // If we are not active, we cannot stop the device.
   if ( isActive() == false )
   {
      retval = 0;
   }
   // If the sampling thread was started, stop it and the device.
   else if ( m_my_thread != NULL )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 1) << "Stopping the MotionStar thread ...\n"
                                    << vprDEBUG_FLUSH;
      m_my_thread->kill();
      delete m_my_thread;
      m_my_thread = NULL;

      vprDEBUG(vrjDBG_INPUT_MGR, 1) << "  Stopping the MotionStar ...\n"
                                    << vprDEBUG_FLUSH;

      m_motion_star.stop();

      // sanity check: did the device actually stop?
      if ( isActive() == true )
      {
         vprDEBUG(vrjDBG_INPUT_MGR, 1)
            << "MotionStar server did not shut down.\n" << vprDEBUG_FLUSH;
         retval = 0;
      }
      else
      {
         vprDEBUG(vrjDBG_INPUT_MGR, 1) << "MotionStar server shut down.\n"
                                       << vprDEBUG_FLUSH;
         retval = 1;
      }
   }
   // If the thread was not started, then the device is stopped.
   else
   {
      retval = 1;
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Sample data.
// ----------------------------------------------------------------------------
int MotionStar::sample ()
{
   int retval;

   retval = 0;

   if ( isActive() == false )
   {
      vprDEBUG(vprDBG_ALL, 0) << clrSetNORM(clrRED) << "MotionStar ("
                              << getAddressName() << ") NOT ACTIVE IN SAMPLE\n"
                              << clrRESET << vprDEBUG_FLUSH;
   }
   else
   {
      int index;
      vrj::Matrix trans_mat, rot_mat, quat_mat;
      float quat[4], angles[3];
      FLOCK::data_format format;

      // Transforms between the cord frames
      // See transform documentation and VR System pg 146
      // Since we want the reciver in the world system, Rw
      // wTr = wTt*tTr
      vrj::Matrix world_T_transmitter, transmitter_T_reciever, world_T_reciever;

      m_motion_star.sample();

      // For each bird
      for ( unsigned int i = 0; i < m_motion_star.getNumBirds(); i++ )
      {
         // Get the index to the current read buffer
         index = getBirdIndex(i, progress);

         format = m_motion_star.getBirdDataFormat(i);

         switch (format)
         {
            case FLOCK::NO_BIRD_DATA:
            case FLOCK::INVALID:
               break;
            case FLOCK::POSITION:
               theData[index].setTrans(m_motion_star.getXPos(i),
                                       m_motion_star.getYPos(i),
                                       m_motion_star.getZPos(i));
               break;
            case FLOCK::ANGLES:
               theData[index].makeZYXEuler(m_motion_star.getZRot(i),
                                           m_motion_star.getYRot(i),
                                           m_motion_star.getXRot(i));
               break;
            case FLOCK::MATRIX:
               m_motion_star.getMatrixAngles(i, angles);
               theData[index].makeZYXEuler(angles[0], angles[1], angles[2]);
               break;
            case FLOCK::POSITION_ANGLES:
               trans_mat.setTrans(m_motion_star.getXPos(i),
                                  m_motion_star.getYPos(i),
                                  m_motion_star.getZPos(i));
               rot_mat.makeZYXEuler(m_motion_star.getZRot(i),
                                    m_motion_star.getYRot(i),
                                    m_motion_star.getXRot(i));
               theData[index] = trans_mat * rot_mat;
               break;
            case FLOCK::POSITION_MATRIX:
               trans_mat.setTrans(m_motion_star.getXPos(i),
                                  m_motion_star.getYPos(i),
                                  m_motion_star.getZPos(i));
               m_motion_star.getMatrixAngles(i, angles);
               rot_mat.makeXYZEuler(angles[0], angles[1], angles[2]);
               theData[index] = trans_mat * rot_mat;
               break;
            case FLOCK::QUATERNION:
               m_motion_star.getQuaternion(i, quat);
               theData[index].makeQuaternion(quat);
               break;
            case FLOCK::POSITION_QUATERNION:
               trans_mat.setTrans(m_motion_star.getXPos(i),
                                  m_motion_star.getYPos(i),
                                  m_motion_star.getZPos(i));
               m_motion_star.getQuaternion(i, quat);
               quat_mat.makeQuaternion(quat);
               theData[index] = trans_mat * rot_mat;
               break;
         }

         // Set transmitter offset from local info.
         world_T_transmitter = xformMat;

         // Get reciever data from sampled data.
         transmitter_T_reciever = theData[index];

         // Compute total transform.
         world_T_reciever.mult(world_T_transmitter, transmitter_T_reciever);

         // Store corrected xform back into data.
         theData[index] = world_T_reciever;
      }

      // Locks and then swaps the indices.
      swapValidIndexes();

      retval = 1;
   }

   return retval;
}

// ----------------------------------------------------------------------------
// Update to the sampled data.
// ----------------------------------------------------------------------------
void MotionStar::updateData ()
{
   // If the device is not active, we cannot update the data.
   if ( isActive() == false )
   {
      vprDEBUG(vprDBG_ALL, 0) << clrSetNORM(clrRED) << "MotionStar ("
                              << getAddressName()
                              << ") not active in updateData()\n" << clrRESET
                              << vprDEBUG_FLUSH;
   }
   // Otherwise, go through with the update.
   else
   {
      vpr::Guard<vpr::Mutex> updateGuard(lock);

      // Copy the valid data to the current data so that both are valid.
      for ( unsigned int i = 0; i < getNumBirds(); i++ )
      {
         theData[getBirdIndex(i,current)] = theData[getBirdIndex(i,valid)];   // first hand
      }

      // Locks and then swap the indices.
      swapCurrentIndexes();
   }
}

// ----------------------------------------------------------------------------
// Get the reciever transform for the given bird number.  The birds are
// zero-based.
// ----------------------------------------------------------------------------
vrj::Matrix* MotionStar::getPosData (int d)
{
   if ( isActive() == false )
   {
      vprDEBUG(vprDBG_ALL,0) << "Not active in getPosData()\n"
                             << vprDEBUG_FLUSH;
      return NULL;
   }
   return (&theData[getBirdIndex(d,current)]);
}

// ----------------------------------------------------------------------------
// Not used currently -- needed for interface.
// ----------------------------------------------------------------------------
jccl::TimeStamp* MotionStar::getPosUpdateTime (int d)
{
   return (&mDataTimes[getBirdIndex(d,current)]);
}

// ----------------------------------------------------------------------------
// Set the address (either IP address or hostname) for the server.
// ----------------------------------------------------------------------------
void MotionStar::setAddressName (const char* n)
{
   // If the device active, we cannot change the server address.
   if ( isActive() )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change server address while active\n"
         << vprDEBUG_FLUSH;
   } else
   {
      m_motion_star.setAddressName(n);
   }
}

// ----------------------------------------------------------------------------
// Set the port on the server to which we connect.
// ----------------------------------------------------------------------------
void MotionStar::setServerPort (const unsigned short port)
{
   // If the device active, we cannot change the server port.
   if ( isActive() )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change server port while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      m_motion_star.setServerPort(port);

      switch (port)
      {
         case 5000:
            setProtocol(BIRDNET::UDP);
            break;
         case 6000:
            setProtocol(BIRDNET::TCP);
            break;
         default:
            vprDEBUG(vrjDBG_INPUT_MGR, 1)
               << "vjMotionStar: Unexpected port number " << port << " given!\n"
               << "              Defaulting to TCP port.\n" << vprDEBUG_FLUSH;
            m_motion_star.setProtocol(BIRDNET::TCP);
            break;
      }
   }
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void MotionStar::setProtocol (const enum BIRDNET::protocol proto)
{
   // If the device active, we cannot change the transmission protocol.
   if ( isActive() )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change transmission protocol while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      m_motion_star.setProtocol(proto);
   }
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void MotionStar::setMasterStatus (const bool master)
{
   // If the device active, we cannot change the master status.
   if ( isActive() )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change master status while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      m_motion_star.setMasterStatus(master);
   }
}

// ----------------------------------------------------------------------------
// Change the hemisphere of the transmitter.
// ----------------------------------------------------------------------------
void MotionStar::setHemisphere (const unsigned char hemisphere)
{
   // If the device active, we cannot change the hemisphere.
   if ( isActive() )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change hemisphere while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      switch (hemisphere)
      {
         case 0:
            m_motion_star.setHemisphere(FLOCK::FRONT_HEMISPHERE);
            break;
         case 1:
            m_motion_star.setHemisphere(FLOCK::REAR_HEMISPHERE);
            break;
         case 2:
            m_motion_star.setHemisphere(FLOCK::UPPER_HEMISPHERE);
            break;
         case 3:
            m_motion_star.setHemisphere(FLOCK::LOWER_HEMISPHERE);
            break;
         case 4:
            m_motion_star.setHemisphere(FLOCK::LEFT_HEMISPHERE);
            break;
         case 5:
            m_motion_star.setHemisphere(FLOCK::RIGHT_HEMISPHERE);
            break;
         default:
            vprDEBUG(vrjDBG_INPUT_MGR, 1)
               << "vjMotionStar: Unknown hemisphere " << hemisphere
               << " given!\n              Defaulting to front hemisphere.\n"
               << vprDEBUG_FLUSH;
            m_motion_star.setHemisphere(FLOCK::FRONT_HEMISPHERE);
            break;
      }
   }
}

// ----------------------------------------------------------------------------
// Set the bird format to the given value.
// ----------------------------------------------------------------------------
void MotionStar::setBirdFormat (const unsigned int format)
{
   // If the device active, we cannot change the bird format.
   if ( isActive() )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change format while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      switch (format)
      {
         case 0:
            m_motion_star.setBirdFormat(FLOCK::NO_BIRD_DATA);
            break;
         case 1:
            m_motion_star.setBirdFormat(FLOCK::POSITION);
            break;
         case 2:
            m_motion_star.setBirdFormat(FLOCK::ANGLES);
            break;
         case 3:
            m_motion_star.setBirdFormat(FLOCK::MATRIX);
            break;
         case 4:
            m_motion_star.setBirdFormat(FLOCK::POSITION_ANGLES);
            break;
         case 5:
            m_motion_star.setBirdFormat(FLOCK::POSITION_MATRIX);
            break;
         case 6:
            vprDEBUG(vrjDBG_INPUT_MGR, 1)
               << "vjMotionStar: Invalid bird format 6 given!\n"
               << "              Defaulting to position/angles.\n"
               << vprDEBUG_FLUSH;
            m_motion_star.setBirdFormat(FLOCK::POSITION_ANGLES);
            break;
         case 7:
            m_motion_star.setBirdFormat(FLOCK::QUATERNION);
            break;
         case 8:
            m_motion_star.setBirdFormat(FLOCK::POSITION_QUATERNION);
            break;
         default:
            vprDEBUG(vrjDBG_INPUT_MGR, 1)
               << "vjMotionStar: Unexpected bird format " << format
               << " given!\n              Defaulting to position/angles.\n"
               << vprDEBUG_FLUSH;
            m_motion_star.setBirdFormat(FLOCK::POSITION_ANGLES);
            break;
      }
   }
}

// ----------------------------------------------------------------------------
// Set the number of birds connected to the flock.
// ----------------------------------------------------------------------------
void MotionStar::setNumBirds (unsigned int i)
{
   // If the device active, we cannot change the number of birds.
   if ( isActive() )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change number of birds while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      m_motion_star.setNumBirds(i);
   }
}

// ----------------------------------------------------------------------------
// Set the run mode for the device.
// ----------------------------------------------------------------------------
void MotionStar::setRunMode (const unsigned int mode)
{
   // If the device active, we cannot change the run mode.
   if ( isActive() )
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change run mode while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      switch (mode)
      {
         case 0:
            m_motion_star.setRunMode(BIRDNET::CONTINUOUS);
            break;
         case 1:
            m_motion_star.setRunMode(BIRDNET::SINGLE_SHOT);
            break;
         default:
            vprDEBUG(vrjDBG_INPUT_MGR, 1)
               << "vjMotionStar: Unexpected run mode " << mode << " given!\n"
               << "              Defaulting to continuous.\n" << vprDEBUG_FLUSH;
            m_motion_star.setRunMode(BIRDNET::CONTINUOUS);
            break;
      }
   }
}

// ----------------------------------------------------------------------------
// Set the report rate for the device.
// ----------------------------------------------------------------------------
void MotionStar::setReportRate (const unsigned char rate)
{
   // If the device active, we cannot change the report rate.
   if (isActive())
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change bird report rate while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      m_motion_star.setReportRate(rate);
   }
}

// ----------------------------------------------------------------------------
// Set the measurement rate for the chassis.
// ----------------------------------------------------------------------------
void MotionStar::setMeasurementRate (const double rate)
{
   // If the device active, we cannot change the measurement rate.
   if (isActive())
   {
      vprDEBUG(vrjDBG_INPUT_MGR, 2)
         << "vjMotionStar: Cannot change chassis measurement rate while active\n"
         << vprDEBUG_FLUSH;
   }
   else
   {
      m_motion_star.setMeasurementRate(rate);
   }
}

// ============================================================================
// Private methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Unimplemented!
// ----------------------------------------------------------------------------
void MotionStar::positionCorrect (float& x, float& y, float& z)
{
   /* Implement me! */ ;
}

// ----------------------------------------------------------------------------
// Unimplemented!
// ----------------------------------------------------------------------------
void MotionStar::initCorrectionTable (const char* table_file)
{
   /* Implement me! */ ;
}

// ----------------------------------------------------------------------------
// Helper to return the index for theData array given the birdNum we are
// dealing with and the bufferIndex to read.
//
// XXX: We are going to say the birds are 0 based.
// ----------------------------------------------------------------------------
unsigned int MotionStar::getBirdIndex (int bird_num, int buffer_index)
{
   unsigned int ret_val = (bird_num * 3) + buffer_index;
   vprASSERT(ret_val < ((getNumBirds() + 1) * 3));

   return ret_val;
}

} // End of vrj namespace
