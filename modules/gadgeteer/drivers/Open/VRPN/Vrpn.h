/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

//=====================================================
// Vrpn
//
// Purpose:
//    VR Juggler VRPN Hi-Ball tracking class
//
// Author:
//    Jason Jerald
// Last Modified:  8-26-02
//
// Revisions:
//      Ported to to 1.1 DR2 and sgi platform Anthony Steed, 10-4-02
//======================================================

#ifndef _GADGET_VRPN_H_
#define _GADGET_VRPN_H_

#include <gadget/Devices/DriverConfig.h>
#include <vector>
#include <gadget/Type/Input.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/InputMixer.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Vec.h>
#include <gmtl/Quat.h>

#include <vrpn_Tracker.h>
#include <vrpn_Button.h>

#if ! defined(VRPN_CALLBACK)
#  define VRPN_CALLBACK
#endif


namespace gadget
{

/**
 * Class interfacing with vrpn sensor data located on the local machine in
 * a shared memory arena.
 *
 * @note A note on reciever access:
 *  Clients of Juggler should access tracker recievers as [0-n].  For
 *  example, if you have recievers 1,2, and 4 with transmitter on 3, then
 *  you can access the data, in order, as 0, 1, 2.
 *
 * @see gadget::Digital, gadget::Analog, gadget::Position
 */
class Vrpn : public InputMixer<InputMixer<Input,Digital>,Position>
{

public:

   /** Constructor. */
   Vrpn()
   {
   };

   /**
    * Destructor.
    *
    * @pre None.
    * @post Shared memory is released.
    */
   virtual ~Vrpn();

   /**
    * Configures the VRPN with the given config element.
    *
    * @pre e must be a element that has VRPN config information.
    * @post If e is a valid config element, the device is configured using
    *       its contents.  Otherwise, configuration fails and false is
    *       returned to the caller.
    *
    * @param e A pointer to a config element.
    *
    * @return true is returned if the device was configured succesfully.
    *         false is returned if the config element is invalid.
    */
   virtual bool config(jccl::ConfigElementPtr e);

   /** Begins sampling. */
   bool startSampling();

   /** Stops sampling. */
   bool stopSampling();

   /** Samples a value. */
   bool sample();

   /**
    * Update to the sampled data.
    *
    * @pre None.
    * @post Most recent value is copied over to temp area.
    */
   void updateData();

   /** Returns what element type is associated with this class. */
   static std::string getElementType();

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
   std::string mTrackerServer;
   std::string mButtonServer;
   int mTrackerNumber;
   int mButtonNumber;

   void handleTracker(const vrpn_TRACKERCB& t);
   void handleButton(const vrpn_BUTTONCB& b);

   void readLoop();

   bool mExitFlag;

   gmtl::Matrix44f getSensorPos(const unsigned int i);
   gadget::DigitalData getDigitalData(const unsigned int i);

   std::vector<DigitalData>  mCurButtons; /**< The current button states. */
   std::vector<PositionData> mCurPositions; /**< The current button states. */

   // Working space - AJS to remove
   std::vector<gadget::DigitalData> mButtons;
   std::vector<gmtl::Quatf> mQuats;
   std::vector<gmtl::Vec3f> mPositions;

   friend void VRPN_CALLBACK staticHandleTracker(void*, vrpn_TRACKERCB);
   friend void VRPN_CALLBACK staticHandleButton(void*, vrpn_BUTTONCB);
};

} // End of gadget namespace


#endif
