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

#ifndef _GADGET_ASCENSION_FLOCK_OF_BIRDS_H_
#define _GADGET_ASCENSION_FLOCK_OF_BIRDS_H_

#include <gadget/Devices/DriverConfig.h>
#include <vpr/Thread/Thread.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/InputMixer.h>
#include <drivers/Ascension/Flock/FlockStandalone.h>


namespace gadget
{

/**
 * Position derived class for running a Flock of Birds.
 * Also a wrapper class for the real Ascension Flock class "FlockStandalone"
 *
 *  Flock adds to the FlockStandalone class shared memory and threading.<br>
 *  Flock is a positional device driver for the Flock of Birds, the config
 *  element in the constructor should set up all the settings, for these to be
 *  changed the Flock has to be deleted and a new instance created with an
 *  updated config element.
 *
 * @note Some functions still remain for changing the options of
 *     the flock when its not in Sampling mode, but in order to stay
 *     consistent with the Input/vjPosition functionality these
 *     are only left for building apps without jccl::ConfigElement objects.
 * @note A note on reciever access:
 *   Clients of juggler should access tracker recievers as [0-n]
 *   For example, if you have recievers 1,2, and 4 with transmitter on 3,
 *   then you can access the data, in order, as 0,1,2.
 *
 * @see gadget::Position
 */
//class Flock : public Input, public Position
class Flock : public InputMixer<Input,Position>
{
public:
   /**
    * Configure Constructor.
    *
    * @param port  such as "/dev/ttyd3"
    * @param baud  such as 38400, 19200, 9600, 14400, etc...
    * @param sync  sync type.
    * @param block  blocking
    * @param numBrds  number of birds in flock
    * @param transmit  transmitter unit number
    * @param hemi  hemisphere to track from
    * @param filt  filtering type
    * @param report  flock report rate
    * @param calfile  a calibration file, if "", then use none
    *
    * @post configures internal data members,
    *          doesn't actually talk to the FOB yet.
    */
   Flock(const char* const port = "/dev/ttyS0",
         const int& baud = 38400,
         const int& numBrds = 3,
         const int& transmit = 3);
   ~Flock();


   /**  configure the flock with a config element. */
   virtual bool config(jccl::ConfigElementPtr e);

   /**  begin sampling */
   bool startSampling();

   /**  stop sampling */
   bool stopSampling();

   /**  sample data */
   bool sample();

   /**  update to the sampled data. */
   void updateData();

   /** Returns what element type is associated with this class. */
   static std::string getElementType();

   /**  see if the flock is active or not */
   bool isActive() const
   {
      return (mFlockOfBirds.getStatus() != FlockStandalone::CLOSED);
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

   void controlLoop(void* nullParam);

private:
   vpr::Thread*      mThread;      /**< The thread doing the flock sampling */
   FlockStandalone   mFlockOfBirds; /**< The actual Flock device object */
   bool              mExitFlag;
};

} // End of gadget namespace

#endif
