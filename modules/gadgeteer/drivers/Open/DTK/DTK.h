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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_DIVERSE_TOOLKIT_H_
#define _GADGET_DIVERSE_TOOLKIT_H_

#include <gadget/Devices/DriverConfig.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/InputMixer.h>
#include <drivers/Open/DTK/DTKMemorySegment.h>


namespace gadget
{

class DTKMemorySegment;
typedef DTKMemorySegment* DTKMemorySegmentHandle;

class dtkClient;

//class GADGET_CLASS_API DTK : public Input, public Position, public Digital,
//                           public Analog
class GADGET_CLASS_API DTK : public InputMixer<InputMixer<InputMixer<Input,Digital>,Analog>,Position>
{
public:
   /** Constructor. */
   DTK();

   /**
    * Destructor.
    *
    * @pre None.
    * @post Shared memory is released.
    */
   virtual ~DTK() throw ();

   /** Configures the device with a config chunk. */
   virtual bool config(jccl::ConfigChunkPtr c);

   /** Begins sampling. */
   bool startSampling();

   /** Main thread of control for this active object. */
   void controlLoop(void* nullParam);

   /** Stops sampling. */
   bool stopSampling();

   /** Samples data. */
   bool sample();

   /** Updates the sampled data. */
   void updateData();

   /** Returns the device name. */
   char* getDeviceName();

   /** Returns what chunk type is associated with this class. */
   static std::string getElementType();

   /**
    * Returns the receiver transform.
    *
    * @post Returns a pointer to the receiver's matrix.
    *
    * @param dev The reciever number.
    *
    * @note Clients of juggler should access tracker recievers as [0-n]
    *       For example, if you have recievers 1,2, and 4 with transmitter on 3,
    *       then you can access them, in order, as 0,1,2.
    */
   gmtl::Matrix44f* getPosData(int dev = 0); // 0 base

   /**
    * Returns the digital and analog data.
    *
    * @post Returns a boolean value where 0 = false and 1 = true.
    * @param d the button number
    * @note Since the tracker has multiple possible devices but digital.
    */
   int getDigitalData(int d = 0);
   float getAnalogData(int d = 0);

   /**
    * Get time of last update for this receiver.
    * @post Returns a pointer to the reciever's timestamp.
    * @param dev The reciever number.
    */
   jccl::TimeStamp* getPosUpdateTime (int dev = 0);

   /** Checks if the driver is active or not. */
   bool isActive()
   {
      return active;
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
   /**
    * Deletes this object.  This is an implementation of the pure virtual
    * gadget::Input::destroy() method.
    */
   virtual void destroy()
   {
      delete this;
   }

private:
   int getStationIndex(int stationNum, int bufferIndex);

   /** @name DTK related data (could be in a wrapper, but not necessary) */
   //@{
   bool active;

   dtkClient*      _client;

   int numPositional;
   int numDigital;
   int numAnalog;
   int numSegments;

   DTKMemorySegmentHandle *_dtkSegments;
   int *mDigitalData;
   float *mAnalogData;
   char*       port;
   //@}

   /** Starts the DTK Client Connection. */
   bool startDTK();

   /** Stops the DTK Client Connection. */
   bool stopDTK();
};

} // End of gadget namespace

#endif
