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

#include <cstdlib>
#include <string>
#include <boost/concept_check.hpp>
#include <vpr/vpr.h>
#include <vpr/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>

#include <gadget/Type/DeviceConstructor.h>
#include <gadget/Util/Debug.h>

#include <gadget/Devices/Tweek/TweekPositionSubjectImpl.h>
#include <gadget/Devices/Tweek/TweekDigitalSubjectImpl.h>
#include <gadget/Devices/Tweek/TweekAnalogSubjectImpl.h>
#include <gadget/Devices/Tweek/TweekGadget.h>


void initDevice(gadget::InputManager* inputMgr)
{
   new gadget::DeviceConstructor<gadget::TweekGadget>(inputMgr);
}

namespace gadget
{

TweekGadget::TweekGadget() : mTweekRunning(false), mThreadRunning(false),
                             mFunctor(NULL), mThread(NULL)
{
}

TweekGadget::~TweekGadget()
{
   if ( mThreadRunning )
   {
      stopSampling();
   }

   unregisterDevices();
   mTweekRunning = false;
}

bool TweekGadget::config(jccl::ConfigChunkPtr c)
{
   bool configured(false);
   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL) << "TweekGadget::config()\n"
                                                   << vprDEBUG_FLUSH;

   if ( Input::config(c) )
   {
      configured = true;

      // Extract the bits related to the CORBA Naming Service with which
      // we will be communicating.
      std::string ns_host  = c->getProperty<std::string>("nsHost");
      vpr::Uint16 ns_port  = c->getProperty<vpr::Uint16>("nsPort");
      std::string iiop_ver = c->getProperty<std::string>("iiopVer");

      try
      {
         vpr::ReturnStatus status;
         int argc(0);

         // Initialize the local ORB via the Tweek CORBA Manager.
         status = mCorbaMgr.init(std::string("TweekGadget"), argc, NULL,
                                 ns_host, ns_port, iiop_ver);

         // If that succeeded, we need to create a Subject Manager servant
         // for all the devices that will be created later.
         if ( status.success() )
         {
            status = mCorbaMgr.createSubjectManager();

            // If Subject Manager servant creation succeeded, we can say that
            // Tweek is running.  We also add an informational item to our
            // Subject Manager servant stating at what "level" it is operating.
            // This is done to distinguish our Subject Manager from any other
            // Subject Managers that may be executing in the same memory space.
            if ( status.success() )
            {
               mTweekRunning = true;
               mCorbaMgr.getSubjectManager()->addInfoItem("Level",
                                                          "Tweek Device");
            }
         }
         else
         {
            vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
               << "CORBA failed to initialize\n" << vprDEBUG_FLUSH;
         }
      }
      catch (...)
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
            << "Caught an unknown exception!\n" << vprDEBUG_FLUSH;
      }

      // If all of the above succeeded and Tweek is in an operational state,
      // we can proceed with the configuration.  Here, we create position,
      // digital, and analog devices based on the contents of our configuration
      // element.
      if ( mTweekRunning )
      {
         const std::string pos_dev_token("positionDevice");
         const int pos_devs = c->getNum(pos_dev_token);

         for ( int i = 0; i < pos_devs; ++i )
         {
            // Create a new servant for the i'th position device.
            TweekPositionSubjectImpl* new_pos_dev =
               new TweekPositionSubjectImpl(this);

            jccl::ConfigChunkPtr pos_dev =
               c->getProperty<jccl::ConfigChunkPtr>(pos_dev_token, i);
            Position::config(pos_dev);
            const std::string subject_name = pos_dev->getName();

            try
            {
               mCorbaMgr.getSubjectManager()->registerSubject(new_pos_dev,
                                                              subject_name.c_str());
               mPosDevs.push_back(PosDev_t(new_pos_dev, subject_name));

               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "Registered new position device subject named '"
                  << subject_name << "'\n" << vprDEBUG_FLUSH;
            }
            catch (...)
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << "Failed to register position device subject '"
                  << subject_name << "'\n" << vprDEBUG_FLUSH;
               delete new_pos_dev;
            }
         }

         const std::string dig_dev_token("digitalDevice");
         const int dig_devs = c->getNum(dig_dev_token);

         for ( int i = 0; i < dig_devs; ++i )
         {
            // Create a new servant for the i'th digital device.
            TweekDigitalSubjectImpl* new_dig_dev =
               new TweekDigitalSubjectImpl(this);

            jccl::ConfigChunkPtr dig_dev =
               c->getProperty<jccl::ConfigChunkPtr>(dig_dev_token, i);
            Digital::config(dig_dev);
            const std::string subject_name = dig_dev->getName();

            try
            {
               mCorbaMgr.getSubjectManager()->registerSubject(new_dig_dev,
                                                              subject_name.c_str());
               mDigitalDevs.push_back(DigitalDev_t(new_dig_dev, subject_name));

               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "Registered new digital device subject named '"
                  << subject_name << "'\n" << vprDEBUG_FLUSH;
            }
            catch (...)
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << "Failed to register digital device subject '"
                  << subject_name << "'\n" << vprDEBUG_FLUSH;
               delete new_dig_dev;
            }
         }

         const std::string ana_dev_token("analogDevice");
         const int ana_devs = c->getNum(ana_dev_token);

         for ( int i = 0; i < ana_devs; ++i )
         {
            // Create a new servant for the i'th analog device.
            TweekAnalogSubjectImpl* new_ana_dev =
               new TweekAnalogSubjectImpl(this);

            jccl::ConfigChunkPtr ana_dev =
               c->getProperty<jccl::ConfigChunkPtr>(ana_dev_token, i);
            Analog::config(ana_dev);
            const std::string subject_name = ana_dev->getName();

            try
            {
               mCorbaMgr.getSubjectManager()->registerSubject(new_ana_dev,
                                                              subject_name.c_str());
               mAnalogDevs.push_back(AnalogDev_t(new_ana_dev, subject_name));

               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
                  << "Registered new analog device subject named '"
                  << subject_name << "'\n" << vprDEBUG_FLUSH;
            }
            catch (...)
            {
               vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_CRITICAL_LVL)
                  << "Failed to register analog device subject '"
                  << subject_name << "'\n" << vprDEBUG_FLUSH;
               delete new_ana_dev;
            }
         }
      }
   }

   return configured;
}

int TweekGadget::startSampling()
{
   int status(0);

   // Do not bother to start the sample loop unless Tweek is actually
   // available to us.
   if ( mTweekRunning )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "TweekGadget::startSampling(): Starting sample loop thread\n"
         << vprDEBUG_FLUSH;

      mFunctor =
         new vpr::ThreadMemberFunctor<TweekGadget>(this,
                                                   &TweekGadget::controlLoop,
                                                   NULL);
      mThread = new vpr::Thread(mFunctor);

      if ( mThread->valid() )
      {
         status = 1;
      }
   }

   return status;
}

int TweekGadget::stopSampling()
{
   // This will cause the thread sample loop to exit.
   mThreadRunning = false;

   // Wait for the thread to shut down before deleting memory.
   mThread->join();

   if ( NULL != mThread )
   {
      delete mThread;
   }

   if ( NULL != mFunctor )
   {
      delete mFunctor;
   }

   return 1;
}

int TweekGadget::sample()
{
   int status(0);

   if ( isActive() )
   {
      vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
         << "TweekGadget::sample(): Sampling from devices\n" << vprDEBUG_FLUSH;

      std::vector<gadget::PositionData> pos_samples(mPosDevs.size());
      std::vector<gadget::DigitalData>  dig_samples(mDigitalDevs.size());
      std::vector<gadget::AnalogData>   ana_samples(mAnalogDevs.size());

      unsigned int i;

      for ( i = 0; i < pos_samples.size(); ++i )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
            << "TweekGadget::sample(): Reading from position subject " << i
            << std::endl << vprDEBUG_FLUSH;

         // Do math magic...
      }

      for ( i = 0; i < dig_samples.size(); ++i )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "TweekGadget::sample(): Reading from digital subject " << i
            << std::endl << vprDEBUG_FLUSH;

         dig_samples[i] = mDigitalDevs[i].first->getState();

         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_HVERB_LVL)
            << "TweekGadget::sample(): State is "
            << mDigitalDevs[i].first->getState() << std::endl
            << vprDEBUG_FLUSH;
      }

      for ( i = 0; i < ana_samples.size(); ++i )
      {
         vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_VERB_LVL)
            << "TweekGadget::sample(): Reading from analog subject " << i
            << std::endl << vprDEBUG_FLUSH;

         ana_samples[i] = mAnalogDevs[i].first->getValue();
      }

      addPositionSample(pos_samples);
      addDigitalSample(dig_samples);
      addAnalogSample(ana_samples);

      status = 1;
   }

   return status;
}

void TweekGadget::updateData()
{
   if ( isActive() )
   {
      swapPositionBuffers();
      swapDigitalBuffers();
      swapAnalogBuffers();
   }
}

void TweekGadget::unregisterDevices()
{
   for ( std::vector<PosDev_t>::iterator i = mPosDevs.begin();
         i != mPosDevs.end();
         ++i )
   {
      mCorbaMgr.getSubjectManager()->unregisterSubject((*i).second.c_str());
      delete (*i).first;
   }

   for ( std::vector<DigitalDev_t>::iterator i = mDigitalDevs.begin();
         i != mDigitalDevs.end();
         ++i )
   {
      mCorbaMgr.getSubjectManager()->unregisterSubject((*i).second.c_str());
      delete (*i).first;
   }

   for ( std::vector<AnalogDev_t>::iterator i = mAnalogDevs.begin();
         i != mAnalogDevs.end();
         ++i )
   {
      mCorbaMgr.getSubjectManager()->unregisterSubject((*i).second.c_str());
      delete (*i).first;
   }
}

void TweekGadget::controlLoop(void* arg)
{
   boost::ignore_unused_variable_warning(arg);

   mThreadRunning = true;

   while ( mThreadRunning )
   {
      // Make sure to read one sample before blocking on mSampleCondVar just
      // to initialize the sample buffers.
      sample();

      // Block until we are notified that there is new data to sample.  We
      // do this to avoid creating huge sample buffers since this device
      // would otherwise be non-blocking with all operations happening only
      // in local memory.
      mSampleCondVar.acquire();
      {
         while ( ! mSampleReady )
         {
            mSampleCondVar.wait();
         }

         // This is the safest place to change this setting.  Doing it here
         // ensures that we do not miss a sample (the mutex is locked), and
         // the next time we come through the loop, the sample(s) will have
         // been read.  Thus, the above test on mSampleReady will always be
         // correct.
         mSampleReady = false;
      }
      mSampleCondVar.release();
   }
}

} // End of gadget namespace
