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

#ifndef _GADGET_SIM_DIGITALGLOVE_H
#define _GADGET_SIM_DIGITALGLOVE_H
//#pragma once

#include <gadget/gadgetConfig.h>

#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Glove.h>
#include <gadget/Type/Finger.h>
#include <gadget/Type/Hand.h>

#include <gadget/Devices/Sim/SimInput.h>

namespace gadget
{

/**
 *  Simulated digital device
 * Simulates a digital device from a keyboard device.
 * It allows any number of simulated digital devices to be created.
 *
 * When the key configured is held down, the digital device is on.
 * When the key is released, the device is no longer set.
 *
 * This class should not be used directly by the user.
 */
class SimDigitalGlove : virtual public Input, public Digital, public SimInput, public Glove
{
public:
   /**  Default Constructor */
   SimDigitalGlove();

   /**  Destructor */
   virtual ~SimDigitalGlove();

   /**
    *  Takes a config chunk
    *  The Juggler API calls this
    */
   virtual bool config( jccl::ConfigChunkPtr chunk );

   /**
    * Gets the digital data for the given "finger".
    * Returns digital 0 or 1, if "finger" makes sense.
    * Returns -1 if function fails or if devNum is out of range.
    * Use one of these indices to get the glove's digital data.
    * EX: int result = mGlove.getDigitalData( SimDigitalGlove::LTHUMB );
    *
    * @return 0 == open, 1 == contact.
    *
    * @note If devNum is out of range, function will fail, possibly issueing
    *       an error to a log or console - but will not ASSERT.<BR><BR>
    *
    * @note These should be the same integers as PinchGlove's
    */
   enum finger
   {
      LTHUMB = 0, LINDEX = 1, LMIDDLE = 2, LRING = 3, LPINKY = 4,
      RTHUMB = 6, RINDEX = 7, RMIDDLE = 8, RRING = 9, RPINKY = 10
   };

   /// dev = finger (see finger enum above)
   virtual const DigitalData getDigitalData( int dev = 0 )
   {
      //vprDEBUG(vprDBG_ALL,0)<<"*** SimDigitalGlove::getDigitalData("<<dev<<")\n"<< vprDEBUG_FLUSH;
      vprASSERT( dev < (int)mDigitalData.size() );    // Make sure we have enough space
      return (mDigitalData[dev]);
   }

   virtual int startSampling()
   {
     //vprDEBUG(vprDBG_ALL,3)<<"start\n"<<vprDEBUG_FLUSH;
       DigitalData temp;
       temp=0;
       for (int i=0;i<10;i++)
       {
           mDigitalData.push_back(temp);
       }
       addDigitalSample(mDigitalData);
       return 1;
   }

   virtual int stopSampling()
   {
     //vprDEBUG(vprDBG_ALL,3)<<"stop\n"<<vprDEBUG_FLUSH;
     return 1;
   }

   virtual int sample()
   {
     //vprDEBUG(vprDBG_ALL,3)<<"sample\n"<<vprDEBUG_FLUSH;
     return 1;
   }

   /** Updates the data. */
   virtual void updateData();

   void updateFingerAngles();

   static std::string getChunkType() { return std::string("SimDigitalGlove");}

// Gesture stuff:
   /**
    * Loads trained data for the gesture object.
    * Loads the file for trained data.
    *void loadTrainedFile(std::string fileName);
    */

   /**
    * Gets the current gesture.
    * @return id of current gesture.
    *virtual int getGesture();
    */

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
   std::vector<DigitalData>   mDigitalData;   /**< The digital data that we have */
   std::vector<KeyModPair>    mSimKeys;       /**< The keys to press for digital simulation */

   Hand                     mLeftHand, mRightHand;
   //int                     mCurGesture;   /**< The current gesture id */
};

} // End of gadget namespace

#endif
