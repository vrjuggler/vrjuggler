/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#include <gadget/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>
#include <gadget/Devices/Ascension/MotionStar.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>


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

   vprDEBUG(gadgetDBG_INPUT_MGR,3)
      << "gadget::MotionStar: Spawned SampleBirds starting\n"
      << vprDEBUG_FLUSH;

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
      vprDEBUG(gadgetDBG_INPUT_MGR, 3)
         << "       MotionStar::config(jccl::ConfigChunkPtr)\n"
         << vprDEBUG_FLUSH;

      // Configure m_motion_star with the config info.
      setAddressName(c->getProperty<std::string>("address").c_str());
      setServerPort((unsigned short) c->getProperty<int>("serverPort"));
      setMasterStatus(c->getProperty<bool>("serverType"));
      setHemisphere((unsigned char) c->getProperty<int>("hemisphere"));
      setNumBirds((unsigned int) c->getProperty<int>("num"));
      setBirdFormat((unsigned int) c->getProperty<int>("bformat"));
      setRunMode((unsigned int) c->getProperty<int>("mode"));
      setReportRate((unsigned char) c->getProperty<int>("reportRate"));
      setMeasurementRate(c->getProperty<float>("measurementRate"));
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar was already started.\n" << vprDEBUG_FLUSH;
      retval = 0;
   }
   else
   {
      if ( m_my_thread == NULL )
      {
         int start_status;

         vprDEBUG(gadgetDBG_INPUT_MGR, 1)
            << "    Getting MotionStar ready ...\n" << vprDEBUG_FLUSH;

         start_status = m_motion_star.start();

         switch (start_status)
         {
            // Proper startup.
            case 0:
               break;
            // Connection to server failed.  MotionStarStandalone prints out
            // the system error message about why.
            case -1:
               vprDEBUG(gadgetDBG_INPUT_MGR, 0)
                  << "gadget::MotionStar failed to connect to server\n"
                  << vprDEBUG_FLUSH;
               retval = 1;
               break;
            // No socket could be created, so no connection can be made.
            case -2:
               vprDEBUG(gadgetDBG_INPUT_MGR, 0)
                  << "gadget::MotionStar could not create a socket\n"
                  << vprDEBUG_FLUSH;
               retval = 1;
               break;
            // No address has been set for the server, so no connection can be
            // made.
            case -3:
               vprDEBUG(gadgetDBG_INPUT_MGR, 0)
                  << "No server address set for MotionStar\n"
                  << vprDEBUG_FLUSH;
               retval = 1;
               break;
            // Unkonwn return value from MotionStarStandalone::start().
            default:
               vprDEBUG(gadgetDBG_INPUT_MGR, 0)
                  << "Abnormal return from MotionStarStandalone::start()\n"
                  << vprDEBUG_FLUSH;
               retval = 1;
               break;
         }

         // Sanity check.  Make sure the servers were actually started.
         if ( ! isActive() )
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, 0)
               << "gadget::MotionStar failed to start.\n" << vprDEBUG_FLUSH;
            retval = 1;
         }
         else
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, 1)
               << "gadget::MotionStar ready to go.\n" << vprDEBUG_FLUSH;

            m_my_thread = new vpr::Thread(sampleBirds, (void*) this);

            if ( m_my_thread == NULL )
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, 0)
                  << "gadget::MotionStar could not create sampling thread.\n"
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 1) << "Stopping the MotionStar thread ...\n"
                                       << vprDEBUG_FLUSH;
      m_my_thread->kill();
      delete m_my_thread;
      m_my_thread = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, 1) << "  Stopping the MotionStar ...\n"
                                       << vprDEBUG_FLUSH;

      m_motion_star.stop();

      // sanity check: did the device actually stop?
      if ( isActive() == true )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, 1)
            << "MotionStar server did not shut down.\n" << vprDEBUG_FLUSH;
         retval = 0;
      }
      else
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, 1) << "MotionStar server shut down.\n"
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
   std::vector< gadget::PositionData > cur_samples(m_motion_star.getNumBirds());

   retval = 0;

   if ( isActive() == false )
   {
      vprDEBUG(vprDBG_ALL, 0) << clrSetNORM(clrRED) << "MotionStar ("
                              << getAddressName() << ") NOT ACTIVE IN SAMPLE\n"
                              << clrRESET << vprDEBUG_FLUSH;
   }
   else
   {
      gmtl::Matrix44f trans_mat, rot_mat;
      float quat[4], angles[3];
      FLOCK::data_format format;

      // Transforms between the cord frames
      // See transform documentation and VR System pg 146
      // Since we want the reciver in the world system, Rw
      // wTr = wTt*tTr
      gmtl::Matrix44f world_T_transmitter, transmitter_T_receiver, world_T_receiver;
      m_motion_star.sample();

      // get an initial timestamp for this entire sample. we'll copy it into
      // each PositionData for this sample.
      if (!cur_samples.empty())
      {
         cur_samples[0].setTime();
      }

      // For each bird
      for ( unsigned int i = 0; i < m_motion_star.getNumBirds(); ++i )
      {
         // Get the index to the current read buffer
         cur_samples[i].setTime( cur_samples[i].getTime() );
         gmtl::identity(transmitter_T_receiver);

         format = m_motion_star.getBirdDataFormat(i);
         gmtl::EulerAngleZYXf euler;
         
         switch (format)
         {
            case FLOCK::NO_BIRD_DATA:
            case FLOCK::INVALID:
               break;
            case FLOCK::POSITION:
               gmtl::setTrans (transmitter_T_receiver,
                               gmtl::Vec3f( m_motion_star.getXPos(i), m_motion_star.getYPos(i), m_motion_star.getZPos(i)) );
               break;
            case FLOCK::ANGLES:
               euler.set( gmtl::Math::deg2Rad(m_motion_star.getZRot(i)),
                          gmtl::Math::deg2Rad(m_motion_star.getYRot(i)),
                          gmtl::Math::deg2Rad(m_motion_star.getXRot(i)) );
               gmtl::setRot( transmitter_T_receiver, euler);
               break;
            case FLOCK::MATRIX:
               m_motion_star.getMatrixAngles(i, angles);

               euler.set( gmtl::Math::deg2Rad(angles[0]),
                          gmtl::Math::deg2Rad(angles[1]),
                          gmtl::Math::deg2Rad(angles[2]) );
               gmtl::setRot( transmitter_T_receiver, euler );

               break;
            case FLOCK::POSITION_ANGLES:
               gmtl::setTrans( trans_mat,
                                gmtl::Vec3f( m_motion_star.getXPos(i), m_motion_star.getYPos(i), m_motion_star.getZPos(i) ) );
               
               euler.set( gmtl::Math::deg2Rad(m_motion_star.getZRot(i)),
                          gmtl::Math::deg2Rad(m_motion_star.getYRot(i)),
                          gmtl::Math::deg2Rad(m_motion_star.getXRot(i)) );
               gmtl::setRot( rot_mat, euler );
               transmitter_T_receiver = (trans_mat * rot_mat);
               break;
            case FLOCK::POSITION_MATRIX:
               gmtl::setTrans( trans_mat,
                                gmtl::Vec3f( m_motion_star.getXPos(i), m_motion_star.getYPos(i), m_motion_star.getZPos(i) ) );

               m_motion_star.getMatrixAngles(i, angles);

               euler.set( gmtl::Math::deg2Rad(angles[2]),
                          gmtl::Math::deg2Rad(angles[1]),
                          gmtl::Math::deg2Rad(angles[0]) );
               gmtl::setRot( rot_mat, euler );
               transmitter_T_receiver = trans_mat * rot_mat;
               break;
            case FLOCK::QUATERNION:
               m_motion_star.getQuaternion(i, quat);
               gmtl::set( transmitter_T_receiver, gmtl::Quatf(quat[1], quat[2], quat[3], quat[0]));
               break;
            case FLOCK::POSITION_QUATERNION:
               gmtl::setTrans( trans_mat, gmtl::Vec3f(m_motion_star.getXPos(i),
                                                      m_motion_star.getYPos(i),
                                                      m_motion_star.getZPos(i)) );

               m_motion_star.getQuaternion(i, quat);
               gmtl::set( rot_mat, gmtl::Quatf(quat[1], quat[2], quat[3], quat[0]));

               transmitter_T_receiver = trans_mat * rot_mat;
               break;
         }

         // Set transmitter offset from local info.
         world_T_transmitter = xformMat;

         // Get receiver data from sampled data.
         //transmitter_T_receiver = *(cur_samples[index].getPosition());

         // Compute total transform.
         // wTr = wTt * tTr
         gmtl::mult(world_T_receiver, world_T_transmitter, transmitter_T_receiver);

         // Store corrected xform back into data.
         *(cur_samples[i].getPosition()) = world_T_receiver;
      }

      // Locks and then swaps the indices.
      mPosSamples.lock();
      mPosSamples.addSample(cur_samples);
      mPosSamples.unlock();

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
      mPosSamples.swapBuffers();
   }
}


// ----------------------------------------------------------------------------
// Set the address (either IP address or hostname) for the server.
// ----------------------------------------------------------------------------
void MotionStar::setAddressName (const char* n)
{
   // If the device active, we cannot change the server address.
   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change server address while active\n"
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change server port while active\n"
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
            vprDEBUG(gadgetDBG_INPUT_MGR, 1)
               << "MotionStar: Unexpected port number " << port << " given!\n"
               << "            Defaulting to TCP port.\n" << vprDEBUG_FLUSH;
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change transmission protocol while active\n"
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change master status while active\n"
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change hemisphere while active\n"
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
            vprDEBUG(gadgetDBG_INPUT_MGR, 1)
               << "gadget::MotionStar: Unknown hemisphere " << hemisphere
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change format while active\n"
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
            vprDEBUG(gadgetDBG_INPUT_MGR, 1)
               << "gadget::MotionStar: Invalid bird format 6 given!\n"
               << "                    Defaulting to position/angles.\n"
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
            vprDEBUG(gadgetDBG_INPUT_MGR, 1)
               << "gadget::MotionStar: Unexpected bird format " << format
               << " given!\n"
               << "                    Defaulting to position/angles.\n"
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change number of birds while active\n"
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change run mode while active\n"
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
            vprDEBUG(gadgetDBG_INPUT_MGR, 1)
               << "gadget::MotionStar: Unexpected run mode " << mode
               << " given!\n"
               << "                    Defaulting to continuous.\n"
               << vprDEBUG_FLUSH;
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change bird report rate while active\n"
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
      vprDEBUG(gadgetDBG_INPUT_MGR, 2)
         << "gadget::MotionStar: Cannot change chassis measurement rate while active\n"
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


} // End of gadget namespace
