/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <boost/concept_check.hpp>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>

#include <vpr/vpr.h>
#include <vpr/System.h>
#include <vpr/Util/FileUtils.h>

#include <jccl/Config/ConfigChunk.h>
#include <gadget/Util/Debug.h>
#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Devices/Ascension/Flock/Flock.h>

#include <vector>


void initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::Flock>(inputMgr);
}

namespace gadget
{

/**
 * Configure constructor.
 *
 * @param port  such as "/dev/ttyd3"
 * @param baud  such as 38400, 19200, 9600, 14400, etc...
 * @param sync  sync type
 * @param block  blocking
 * @param numBrds  number of birds in flock (without transmitter)
 * @param transmit  transmitter unit number
 * @param hemi  hemisphere to track from
 * @param filt  filtering type
 * @param report  flock report rate
 * @param calfile  a calibration file, if "", then use none
 */
Flock::Flock(const char* const port, const int& baud, const int& sync,
             const bool& block, const int& numBrds, const int& transmit,
             const BIRD_HEMI& hemi, const BIRD_FILT& filt, const char& report,
             const char* const calfile)
   : mThread(NULL),
     mFlockOfBirds(port, baud, sync, block, numBrds, transmit, hemi, filt,
                   report, calfile)

{
   ;
}

bool Flock::config(jccl::ConfigChunkPtr c)
{
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "   Flock::Flock(jccl::ConfigChunk*)"
         << std::endl << vprDEBUG_FLUSH;

   // read in Position's config stuff,
   // --> this will be the port and baud fields
   if(! (Input::config(c) && Position::config(c)))
   {
      return false;
   }

   mPortName = c->getProperty<std::string>("port");
   mBaudRate = c->getProperty<int>("baud");

   // keep FlockStandalone's port and baud members in sync with Input's port
   // and baud members.
   mFlockOfBirds.setPort( mPortName );
   mFlockOfBirds.setBaudRate( mBaudRate );

   // set mFlockOfBirds with the config info.
   mFlockOfBirds.setSync( c->getProperty<int>("sync") );
   mFlockOfBirds.setBlocking( c->getProperty<bool>("blocking") );
   mFlockOfBirds.setNumBirds( c->getProperty<int>("num") );
   mFlockOfBirds.setTransmitter( c->getProperty<int>("transmitter") );
   mFlockOfBirds.setExtendedRange( c->getProperty<bool>("extendedRange") );
   mFlockOfBirds.setHemisphere( (BIRD_HEMI) c->getProperty<int>("hemi") ); //LOWER_HEMI
   mFlockOfBirds.setFilterType( (BIRD_FILT) c->getProperty<int>("filt") ); //

   // sanity check the report rate
   char r = c->getProperty<std::string>("report").c_str()[0];
   if ((r != 'Q') && (r != 'R') &&
       (r != 'S') && (r != 'T'))
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "   illegal report rate from configChunk, defaulting to every other cycle (R)"
         << std::endl << vprDEBUG_FLUSH;
      mFlockOfBirds.setReportRate( 'R' );
   }
   else
   {
      mFlockOfBirds.setReportRate( r );
   }

   // output what was read.
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
      << "    Flock Settings: " << std::endl
      << "          FlockStandalone::getTransmitter(): " << mFlockOfBirds.getTransmitter() << std::endl
      << "          FlockStandalone::getNumBirds(): " << mFlockOfBirds.getNumBirds() << std::endl
      << "          FlockStandalone::getBaudRate(): " << mFlockOfBirds.getBaudRate() << std::endl
      << "          FlockStandalone::getPort(): " << mFlockOfBirds.getPort() << std::endl
      << "     instance name : " << mInstName << std::endl
      << std::endl << vprDEBUG_FLUSH;

   // init the correction table with the calibration file.
   std::string calfile = c->getProperty<std::string>("calfile");
   mFlockOfBirds.initCorrectionTable(vpr::replaceEnvVars(calfile).c_str());

   return true;
}

Flock::~Flock()
{
   this->stopSampling();
}

void Flock::controlLoop(void* nullParam)
{
   boost::ignore_unused_variable_warning(nullParam);

   // vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
   //    << "gadget::Flock: Spawned SampleBirds starting"
   //    << std::endl << vprDEBUG_FLUSH;

   // XXX: I can never exit!
   for (;;)
   {
      this->sample();
   }
}

int Flock::startSampling()
{
      // make sure birds aren't already started
   if (this->isActive() == true)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock was already started." << std::endl << vprDEBUG_FLUSH;
      return 0;
   }

   if (mThread == NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "    Getting flock ready....\n" << vprDEBUG_FLUSH;
      mFlockOfBirds.start();

      //sanity check.. make sure birds actually started
      if (this->isActive() == false)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "gadget::Flock failed to start.." << std::endl << vprDEBUG_FLUSH;
         return 0;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "gadget::Flock ready to go..\n" << vprDEBUG_FLUSH;

//      Flock* devicePtr = this;

      // Create a new thread to handle the control
      vpr::ThreadMemberFunctor<Flock>* memberFunctor =
          new vpr::ThreadMemberFunctor<Flock>(this, &Flock::controlLoop, NULL);
      mThread = new vpr::Thread(memberFunctor);

      if ( ! mThread->valid() )
      {
         return 0;  // Fail
      }
      else
      {
         return 1;   // success
      }
   }
   else
   {
      return 0; // already sampling
   }
}

int Flock::sample()
{
   std::vector< gadget::PositionData > cur_samples(mFlockOfBirds.getNumBirds());

   if (this->isActive() == false)
   {
      return 0;
   }

   int i;

   mFlockOfBirds.sample();

   // get an initial timestamp for this entire sample. we'll copy it into
   // each PositionData for this sample.
   if (!cur_samples.empty())
   {
       cur_samples[0].setTime();
   }

   vpr::Thread::yield();

   // For each bird
   for (i=0; i < (mFlockOfBirds.getNumBirds()); ++i)
   {
      // Transforms between the cord frames
      gmtl::Matrix44f transmitter_T_reciever;

      // We add 1 to "i" to account for the fact that FlockStandalone is
      // 1-based

      /*
      transmitter_T_reciever.makeZYXEuler(mFlockOfBirds.zRot( i+1 ),
                                                   mFlockOfBirds.yRot( i+1 ),
                                                   mFlockOfBirds.xRot( i+1 ));

      transmitter_T_reciever.setTrans(mFlockOfBirds.xPos( i+1 ),
                                               mFlockOfBirds.yPos( i+1 ),
                                               mFlockOfBirds.zPos( i+1 ));
      */
  /*    gmtl::identity(transmitter_T_reciever);
      gmtl::EulerAngleZYXf euler( gmtl::Math::deg2Rad(mFlockOfBirds.zRot( i+1 )),
                                  gmtl::Math::deg2Rad(mFlockOfBirds.yRot( i+1 )),
                                  gmtl::Math::deg2Rad(mFlockOfBirds.xRot( i+1 )) );
      gmtl::setRot( transmitter_T_reciever, euler );
      gmtl::setTrans( transmitter_T_reciever, gmtl::Vec3f( mFlockOfBirds.xPos( i+1 ),
                                                           mFlockOfBirds.yPos( i+1 ),
                                                           mFlockOfBirds.zPos( i+1 )) );
     */
      gmtl::identity(transmitter_T_reciever);
      gmtl::EulerAngleZYXf euler( gmtl::Math::deg2Rad(mFlockOfBirds.zRot( i )),
                                  gmtl::Math::deg2Rad(mFlockOfBirds.yRot( i )),
                                  gmtl::Math::deg2Rad(mFlockOfBirds.xRot( i )) );
      gmtl::setRot( transmitter_T_reciever, euler );
      gmtl::setTrans( transmitter_T_reciever, gmtl::Vec3f( mFlockOfBirds.xPos( i ),
                                                           mFlockOfBirds.yPos( i ),
                                                           mFlockOfBirds.zPos( i )) );

//      if (i==1)
//      {
//         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
//            << "Flock: bird1:    orig:" << Coord(theData[index]).pos
//            << std::endl << vprDEBUG_FLUSH;
//      }

      cur_samples[i].mPosData = transmitter_T_reciever;                                     // Store corrected xform back into data
      cur_samples[i].setTime (cur_samples[0].getTime());

//      if (i == 1)
//      {
//         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
//            << "Flock: bird1: xformed:" << Coord(theData[index]).pos
//            << std::endl << vprDEBUG_FLUSH;
//      }
   }

   // Add data sample
   addPositionSample(cur_samples);

   return 1;
}

int Flock::stopSampling()
{
   if (this->isActive() == false)
   {
      return 0;
   }

   if (mThread != NULL)
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "Stopping the flock thread..." << vprDEBUG_FLUSH;

      mThread->kill();
      delete mThread;
      mThread = NULL;

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "  Stopping the flock..." << vprDEBUG_FLUSH;

      mFlockOfBirds.stop();

      // sanity check: did the flock actually stop?
      if (this->isActive() == true)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "Flock didn't stop." << std::endl << vprDEBUG_FLUSH;
         return 0;
      }

      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CONFIG_LVL)
         << "stopped." << std::endl << vprDEBUG_FLUSH;
   }

   return 1;
}


void Flock::updateData()
{
   if (this->isActive() == false)
   {
      return;
   }

   swapPositionBuffers();

   return;
}

void Flock::setHemisphere(const BIRD_HEMI& h)
{
   if (this->isActive())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock: Cannot change the hemisphere while active\n"
         << vprDEBUG_FLUSH;
      return;
   }
   mFlockOfBirds.setHemisphere( h );
}

void Flock::setFilterType(const BIRD_FILT& f)
{
   if (this->isActive())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock: Cannot change filters while active\n"
         << vprDEBUG_FLUSH;
      return;
   }
   mFlockOfBirds.setFilterType( f );
}

void Flock::setReportRate(const char& rRate)
{
  if (this->isActive())
  {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock: Cannot change report rate while active\n"
         << vprDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setReportRate( rRate );
}

void Flock::setTransmitter(const int& Transmit)
{
  if (this->isActive())
  {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock: Cannot change transmitter while active\n"
         << vprDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setTransmitter( Transmit );
}


void Flock::setNumBirds(const int& n)
{
  if (this->isActive())
  {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock: Cannot change num birds while active\n"
         << vprDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setNumBirds( n );
}


void Flock::setSync(const int& sync)
{
  if (this->isActive())
  {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock: Cannot change report rate while active\n"
         << vprDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setSync( sync );
}


void Flock::setBlocking(const bool& blVal)
{
  if (this->isActive())
  {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock: Cannot change report rate while active\n"
         << vprDEBUG_FLUSH;
      return;
  }
  mFlockOfBirds.setBlocking( blVal );
}


/**
 * Sets the port to use.
 * This will be a string in the form of the native OS descriptor.<BR>
 * ex: unix - "/dev/ttyd3", win32 - "COM3"
 *
 * @note this->isActive() must be false to use this function
 */
void Flock::setPort( const char* const serialPort )
{
    if (this->isActive())
    {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock: Cannot change port while active\n"
         << vprDEBUG_FLUSH;
      return;
    }
    mFlockOfBirds.setPort( serialPort );

    // keep Input's port and baud members in sync
    // with FlockStandalone's port and baud members.
    mPortName = serialPort;
}

/**
 * Gets the port used.
 * This will be a string in the form of the native OS descriptor.<BR>
 * ex: unix - "/dev/ttyd3", win32 - "COM3"
 */
void Flock::setBaudRate( const int& baud )
{
   // keep Input's port and baud members in sync
   // with FlockStandalone's port and baud members.
   mBaudRate = baud;

   if (this->isActive())
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_WARNING_LVL)
         << "gadget::Flock: Cannot change baud rate while active\n"
         << vprDEBUG_FLUSH;
      return;
   }
   mFlockOfBirds.setBaudRate( mBaudRate );
}

} // End of gadget namespace
