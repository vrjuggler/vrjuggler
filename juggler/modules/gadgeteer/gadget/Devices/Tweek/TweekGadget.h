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

#ifndef _GADGET_TWEEK_GADGET_H_
#define _GADGET_TWEEK_GADGET_H_

#include <gadget/Devices/DriverConfig.h>

#include <string>
#include <utility>
#include <vector>
#include <vpr/vpr.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Sync/CondVar.h>
#include <tweek/CORBA/CorbaManager.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/InputMixer.h>


namespace gadget
{
   class InputManager;
}

extern "C" GADGET_DRIVER_API(void) initDevice(gadget::InputManager* inputMgr);

namespace gadget
{

class TweekAnalogSubjectImpl;
class TweekDigitalSubjectImpl;
class TweekPositionSubjectImpl;

class TweekGadget :
   public InputMixer<InputMixer<InputMixer<Input, Digital>, Analog>, Position>
{
public:
   TweekGadget();

   virtual ~TweekGadget();

   /**
    * Configures this Tweek "device" with the given chunk.  Based on the
    * configuration, steps will be taken to initliaze the Tweek CORBA server
    * side.  Subject servants will be created and registered for each of the
    * requested devices.
    */
   virtual bool config(jccl::ConfigChunkPtr c);

   /**
    */
   virtual int startSampling();

   /**
    */
   virtual int stopSampling();

   /**
    * Samples each of the known "devices" (Tweek subjects).  This requires
    * that the Tweek initialization steps completed correctly.
    */
   virtual int sample();

   virtual void updateData();

   bool isActive()
   {
      return mTweekRunning;
   }

   static std::string getChunkType()
   {
      return std::string("TweekGadget");
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
   friend class gadget::TweekPositionSubjectImpl;
   friend class gadget::TweekDigitalSubjectImpl;
   friend class gadget::TweekAnalogSubjectImpl;

   void notifySample()
   {
      mSampleCondVar.acquire();
      {
         mSampleReady = true;
      }
      mSampleCondVar.release();

      mSampleCondVar.signal();
   }

   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

   /**
    * Iterates over the device collections and unregisters each one with the
    * Tweek Subject Manager.
    */
   void unregisterDevices();

   /**
    * Samples all of the registered "devices" until the thread is told to
    * exit.
    */
   void controlLoop(void* arg);

   /** @name Tweek-related members */
   //@{
   bool mTweekRunning;
   tweek::CorbaManager mCorbaMgr;
   //@}

   /** @name Sample thread members */
   //@{
   bool                                   mThreadRunning;
   vpr::ThreadMemberFunctor<TweekGadget>* mFunctor;
   vpr::Thread*                           mThread;
   bool                                   mSampleReady;
   vpr::CondVar                           mSampleCondVar;
   //@}

   /** @name Device collections */
   //@{
   typedef std::pair<TweekAnalogSubjectImpl*, std::string>   AnalogDev_t;
   typedef std::pair<TweekDigitalSubjectImpl*, std::string>  DigitalDev_t;
   typedef std::pair<TweekPositionSubjectImpl*, std::string> PosDev_t;

   std::vector<AnalogDev_t>  mAnalogDevs;
   std::vector<DigitalDev_t> mDigitalDevs;
   std::vector<PosDev_t>     mPosDevs;
   //@}
};

} // End of gadget namespace


#endif /* _GADGET_TWEEK_GADGET_H_ */
